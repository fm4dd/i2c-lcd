# Setting up the I2C bus on a Beaglebone Black

The Beaglebone Black provides three I2C connections, two of them only available by changing the default GPIO pin configuration. In my use case, I am operating a 2x7 character LCD on I2C bus #2, and a battery-buffered RTC on I2C bus #3. 

GPIO pin configurations are loaded at boot time from device tree binary (.dtb) files, which can be compied from configuration source files (.tds). A number of pre-compiled .dtb files exist for use with popular capes.

Two enable I2C #2 and #3, the following steps were taken:

First step is to de-compile the running default device tree binary file into a editable source (.dts) file. We are using the device tree compiler dtc:

```
mkdir ~/enable-i2c
cd enable-i2c
dtc -I dtb -O dts am57xx-beagle-x15.dtb > am57xx-beagle-x15.dts
```

Note: Compiling the decompiled am57xx-beagle-x15.dts back will create a identical dtb binary.

Next, I copy the decompiled am57xx-beagle-x15.dts to am335x-boneblack-fm4dd.dts, and I adjust/add the pin settings for the i2c bus definitions. Then I compile a new device tree overlay binary file.

Device tree compiler (dtc) command line usage example:
```
### dtc -O dtb -o MY-GPIO-setup-00A0.dtbo -b 0 -@ MY-GPIO-setup.dts ###
```

Compiler options:
 -O dtb is the output format. We're outputting device tree binaries.
 -o is the output filename.
 -b 0 is setting the physical boot CPU. (a zero)
 -@ generates a symbols node as part of the dynamic DT loading of the overlay

In my case, the command I run is:
```
dtc -O dtb -o am335x-boneblack-fm4dd.dtb -b 0 -@ am335x-boneblack-fm4dd.dts
```

Finally, we copy the dtb file to the boot location:
```
cp am335x-boneblack-fm4dd.dtb /boot/dtbs/4.1.17-ti-rt-r47
```

and enable the custom overlay in /boot/uEnv.txt, e.g. as follows:
```
###BeagleBone Black: Loading FM4DD custom I2C pin assignments:
dtb=am335x-boneblack-fm4dd.dtb
```

Notes: Any mistakes may break the next boot. In that case, we need a fallback into a rescue boot, such as starting from the micro SD card with a stock BBB image (even a old Debian 7.5 image, fitting into 2GB, works fine. After forcing a SD card boot by holding the S2 boot switch during power-up, we login, mount the eMMC partition, and edit /boot/uEnv.txt back. Remove SD card and boot from eMMC into the original setup.

If the .dts file edit was correct and BBB boots up, we now have all three I2C busses available:
```
root@athos:/home/ubuntu/source/i2c-lcd# i2cdetect -r -y 0
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- UU -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- UU -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: UU -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: UU -- -- -- -- -- -- --
root@athos:/home/ubuntu/source/i2c-lcd# i2cdetect -r -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- 32 -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
root@athos:/home/ubuntu/source/i2c-lcd# i2cdetect -r -y 2
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- 3e --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- UU UU UU UU -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```

Notes: 
- On I2C2 I have the 2x7 character LCD under the address 3e.
- On I2C1 I have the Epson RX 8025N RTC clock with address 32.

See also:
http://beagleboard.narkive.com/M53sw2BE/how-to-make-bbb-pins-work-after-ubuntu-trusty-install
