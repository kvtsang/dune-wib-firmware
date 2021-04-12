#!/usr/bin/env python3

import os
import sys
import time
import argparse

from wib import WIB
import wib_pb2 as wibpb

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Change the FEMB voltages on a WIB (first turns FEMBs OFF)')
    parser.add_argument('--wib_server','-w',default='127.0.0.1',help='IP of wib_server to connect to [127.0.0.1]')
    parser.add_argument('--dc2dc', type=float, nargs=4, help='DC2DC Voltages [V], default=4V',
                        metavar=('DC2DC_O1', 'DC2DC_O2', 'DC2DC_O3', 'DC2DC_O4'),
                        default=[4.0]*4)
    parser.add_argument('--ldo', type=float, nargs=2, help='LDO Voltages [V], default=2.5V',
                        metavar=('A0_LDO', 'A1_LDO'),
                        default=[2.5]*2)

    args = parser.parse_args()

    wib = WIB(args.wib_server)
    req = wibpb.ConfigurePower()
    
    req.dc2dc_o1, req.dc2dc_o2, req.dc2dc_o3, req.dc2dc_o4 = args.dc2dc
    req.ldo_a0, req.ldo_a1 = args.ldo
    print(req)
    
    rep = wibpb.Status()
    print('Sending ConfigurePower command...')
    wib.send_command(req,rep)
    print(rep.extra.decode('ascii'))
    print('Successful:',rep.success)
