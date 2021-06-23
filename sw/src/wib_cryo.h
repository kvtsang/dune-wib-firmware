#ifndef wib_cryo_h
#define wib_cryo_h

#include "wib.h"
#include "femb_cryo.h"

class WIB_CRYO : public WIB {

public:

    WIB_CRYO();
    virtual ~WIB_CRYO();

    // Turns calibration pulser on or off 
    virtual bool set_pulser(bool on);
    
    // Control power sequence for this WIB's frontend 
    virtual bool power_wib(const wib::PowerWIB &conf);
    
    // Configure the frontend for this WIB
    virtual bool configure_wib(const wib::ConfigureWIB &conf);

    static const size_t N_FEMBS = 4;

    bool cryo_cmd(const std::string &args);
    
protected:

    // Interface to each of the 4 FEMBs 
    //FEMB_CRYO* femb[N_FEMBS];
    
    // Reset front end to a powered off state
    virtual bool reset_frontend();

    inline bool _femb_power_on(size_t femb_idx);
    inline bool _femb_power_off(size_t femb_idx);
};

#endif
