Meteor
======

Meteor is a Nintendo Gameboy Advance emulator. The CLI and GTK frontends run
only on Linux. You can use the libretro implementation if you want to run it on
Microsoft Windows or if you want features from the SSNES frontend.

GTK frontend build
==================

Installation
------------

To build Meteor, you need the following dependencies:

- CMake
- Gtkmm
- sigc++ (installed with gtkmm)
- libao
- SFML
- zlib
- OpenGL

Type the following commands to build Meteor:

	$ mkdir build
	$ cd build
	$ cmake ..
	$ make
	$ make install

You may need root privileges for the last command. You can change the
installation prefix by typing:

	cmake -DCMAKE_INSTALL_PREFIX=<prefix> ..

Uninstallation
--------------

In the build directory type:

	$ xargs rm < install_manifest.txt

You may need root privileges to do this.

libretro build
==============

To build the libretro implementation, you need no dependency. Just go to the
libretro folder and type make, this will create a libsnes.so, retro.dll or
libretro.dylib.
