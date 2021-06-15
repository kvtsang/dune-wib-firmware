#include "wib_cryo.h"
#include <string>

WIB_CRYO::WIB_CRYO() : WIB() {
    //for (int i = 0; i < 4; i++) {
    //    this->femb[i] = new FEMB_CRYO(i);
    //}
}

WIB_CRYO::~WIB_CRYO() {
    //for (int i = 0; i < 4; i++) {
    //    delete this->femb[i];
    //}
}

bool WIB_CRYO::set_pulser(bool on) {
    
    //FIXME do CRYO pulser control
    
    return true;
}

bool WIB_CRYO::reset_frontend() {
    bool success = true;
    glog.log("Disabling front end power\n");
    for (size_t i = 0; i < N_FEMBS; ++i)
      _femb_power_off(i);

    femb_power_config();
    
    glog.log("Disable link_master\n");
    femb_rx_mask(0xffff); // disable all 4 FEMBs
    return success;
}

bool WIB_CRYO::power_wib(const wib::PowerWIB &conf) {

    const bool conf_femb_on[] = {
      conf.femb0(), conf.femb1(), conf.femb2(), conf.femb3()};

    bool status = true;
    for (size_t i = 0; i < N_FEMBS; ++i) {
      status &= conf_femb_on[i] ? _femb_power_on(i) : _femb_power_off(i);
    }


    if (!status) {
      glog.log("Failed to power on enabled FEMBs, aborting\n");
      return false;
    }
    
    switch(conf.stage()) {
      case 0:
        glog.log("Stage 0: Do nothing");
        break;
      case 1:
        glog.log("Stage 1: Waiting for external reset and synchronization\n");
        // pdtbutler mst PROD_MASTER send-cmd 0 11
        io_reg_write(&this->regs, REG_TIMING_CMD_0, 0x0b00, 0xFF00); // timing command 0xb == 11 will run EDGE+ACT
        return status;
        break;
      case 2:
        glog.log("Stage 2: Resuming power ON after external reset and synchronization\n");
        io_reg_write(&this->regs, REG_TIMING_CMD_0, 0x00000, 0xFF00); // zero out timing commands
        break;
      default:
        glog.log("Unknown stage, aborting\n");
        return false;
    }
    return status;
}

bool WIB_CRYO::configure_wib(const wib::ConfigureWIB &conf) {
    glog.log("Configuring WIB-CRYO ...\n"); 

    if (conf.fembs_size() != N_FEMBS) {
        glog.log("Must supply exactly %i configurations\n", N_FEMBS);
        return false;
    }
    
    bool fembs_powered = true;
    uint32_t rx_mask = 0x0; // enable all link_master by default
    for (int i = 0; i < N_FEMBS; i++) { // Check FEMB power state (enabled FEMBs must be ON)
        if (conf.fembs(i).enabled()) {
            fembs_powered &= frontend_power[i];
        }
        else {
          // disable link_master for inactive FEMB
          rx_mask |= (0xf << (i*4));
        }
    }
    if (!fembs_powered) {
        glog.log("Enabled FEMBs must be powered\n");
        return false;
    }
    
    glog.log("Set rx_mask: %X\n", rx_mask);
    femb_rx_mask(rx_mask);

    
    // FIXME do CRYO config
    
    return true;
}

bool WIB_CRYO::_femb_power_off(size_t femb_idx) {
  if (femb_idx >= N_FEMBS) return false;
  //if (!frontend_power[femb_idx]) return true;

  glog.log("Power off FEMB %i\n", femb_idx);

  femb_power_en_ctrl(femb_idx, 0x00);

  frontend_power[femb_idx] = false;
  return true;
}

bool WIB_CRYO::_femb_power_on(size_t femb_idx) {
  if (femb_idx >= N_FEMBS) return false;
  if (frontend_power[femb_idx]) {
    glog.log("FEMB %i is already ON\n", femb_idx);
    return true;
  }

  double values[] = {dc2dc_o1, dc2dc_o2, dc2dc_o3, dc2dc_o4, ldo_a0, ldo_a1};
  std::string msgs[] ={
    "PWR_EN_DC2DC_O1_BRD%i [ON]\n",
    "PWR_EN_DC2DC_O2_BRD%i [ON]\n",
    "PWR_EN_DC2DC_O3_BRD%i [ON]\n",
    "PWR_EN_DC2DC_O4_BRD%i [ON]\n",
    "PWR_EN_A0_LDOBRD%i    [ON]\n",
    "PWR_EN_A1_LDOBRD%i    [ON]\n"
  };

  uint8_t i2c_data = 0;
  for (size_t bit = 0; bit < 6; ++bit) {
    if (values[bit] <= 0) continue;

    i2c_data += 1 << bit;
    glog.log(msgs[bit].c_str(), femb_idx);

  }

  if (i2c_data ==  0) {
    glog.log("Check DC2DC or LDO values\n");
    return false;
  }

  glog.log("Enabling DC2DC and LDO\n");
  glog.log("[DEBUG] femb_power_en_ctrl(%i, %i)\n", femb_idx, i2c_data);

  femb_power_en_ctrl(femb_idx, i2c_data);
  
  glog.log("Enabling PWR_BIAS_EN_BRD%i\n", femb_idx);
  femb_power_en_ctrl(femb_idx, 0xFF);

  frontend_power[femb_idx] = true;

  return true;
}
