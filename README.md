Marshmallow Game Engine
=======================

**“The only game engine that tastes better slightly burned!”**

About
-----

*marshmallow_h* is an open-source game engine focused on 8-bit and 16-bit era
2D video games. This engine will act as the backbone and toolkit for the
Marshmallow Entertainment System.

<http://marshmallow.me/>

Source
------

	git clone --recursive git://github.com/gamaral/marshmallow_h.git

Notice the *--recursive* switch, it's needed so that submodules get checked
out along with the main repository.

Building
--------

I include many predefined cmake init-caches, please take a look at the "cmake"
directory in the project root, filenames are pretty self-explanatory.

### Linux/BSD

Run the following from project root:

	cd build
	cmake -C ../cmake/Cache-opengl-glx.cmake ..
	make

Marshmallow's go-to compiler is Clang, the example above uses gcc since it's
almost guaranteed to be on the system (unlike Clang).

### Mac OSX

Run the following from project root using Terminal:

	cd build
	cmake -G Xcode -C ../cmake/Cache-opengl-qt4.cmake ..
	make

Launch Xcode and open the project file in the build directory, CGL is currently
unsupported (due to my lazyness, so SDL and QT4 can be used for now).

### Windows

Run the following from project root using the Command Line terminal:

	cd build
	cmake -C ..\cmake\Cache-opengl-wgl.cmake ..

Now open Visual Studio and open the MARSHMALLOW solution located in the build
directory.

Tested on Win7 x86/x64 (VS2010)

### Raspberry Pi

If you wish to test out marshmallow_h on the [Raspberry
Pi](http://www.raspberrypi.com/), you have two ways of building.

#### Native

Run the following commands in your *marshmallow_h* base:

	cd build
	cmake -C ../cmake/Cache-raspberrypi.cmake ..
	make

#### Cross-compile

To start, you will need to clone my RPi toolchain:

	git clone --depth 1 git://github.com/gamaral/rpi-toolchain.git

At this point you have the base of the toolchain but no actual submodules, you
will now need to *quick-clone* the submodules:

	cd rpi-toolchain
	scripts/quick-clone

The *rpi-toolchain* requires some special environment variables to be exported
in order to work with *marshmallow_h*, this is handled by the nifty *env* file
in the toolchain base:

	source rpi-toolchain/env

Keep in mind you might need to *source* that same environment file again if you
plan to reconfigure at a later time.

We are now ready to configure *marshmallow_h*, change into the base directory
and run the following commands:

	cd build
	cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-raspberrypi.cmake -C ../cmake/Cache-raspberrypi.cmake ..
	make

