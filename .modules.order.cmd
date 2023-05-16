cmd_/home/pi/modI2c/modules.order := {   echo /home/pi/modI2c/arduino-i2c.ko; :; } | awk '!x[$$0]++' - > /home/pi/modI2c/modules.order
