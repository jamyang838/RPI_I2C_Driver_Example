cmd_/home/pi/modI2c/Module.symvers := sed 's/\.ko$$/\.o/' /home/pi/modI2c/modules.order | scripts/mod/modpost -m -a  -o /home/pi/modI2c/Module.symvers -e -i Module.symvers   -T -
