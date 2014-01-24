## Da Bling!
Simple project to provide some flashing eye candy for the childrens
carnival costume accessories. In this case it consists of some LEDs
and a dual 7 segment LED display.

The 7 segment display shows some counting and random pattern via
analog digital converter.

### How to get and build
Cmake is used for an out-of-source build of the project including
the upload via avrdude.

```bash
git clone git@github.com:mkleemann/DaBling.git /path/to/clone/in
cd /path/to/clone/in
git submodule init
git submodule update
mkdir -p /path/to/build/in
cd /path/to/build/in
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=/path/to/clone/in/cmake/generic-gcc-avr.cmake /path/to/clone/in
make
```

Use `make help` to get all possible targets to use. To upload use
`make upload-dabling`. Dokumentation is being build by using
`make docu`. The target is only available if `doxygen` is found
while generating the build files.

***NOTE:***
For a Windows build environment consult the README file of the cmake
toolchain.

### Some details
Port D of the atmega8 is used for the digit LEDs. Port pins B6 and B7
are used to switch between right and left digit of the display
(common cathode) via an npn transistor.

Port pin C0 is the adc input channel used for the random pattern.

All other port pins are used as LED base for some effects.

