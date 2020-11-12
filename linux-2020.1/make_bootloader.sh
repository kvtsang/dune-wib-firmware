#!/bin/bash

petalinux-package --boot --u-boot --force --fpga ../*.bit

cd images/linux/
tar czf bootfs.tar.gz BOOT.BIN boot.scr image.ub 
