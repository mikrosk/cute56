cute56
======

cute56 is a Qt based simulator &amp; debugger for DSP 56001 development. Its target audience is mostly myself :) but if you would like to experiment with C/C++ and [Motorola DSP 56001](https://www.nxp.com/products/no-longer-manufactured/24-bit-general-purpose-digital-signal-processor:DSP56001?fsrch=1&sr=1&pageNum=1) assembler, it might be the right tool for you (without the need of installing [Hatari](http://hatari.tuxfamily.org) for instance). The main advantage (as I see it) is that when you manage to make a nice multiplatform library for common tasks (setting up the screen, timers, dsp communication, ... ) you can completely forget whether you're targeting the native platform (Atari m68k in my case) or the host.

Prerequisites
-------------

I'll describe my scenario, if you don't want to mess with m68k at all, you can skip most of it. Also be aware of the fact that you need to compile most of this stuff by yourself, nothing for pussies :) First of all, go to the `tools` directory and install `Win56000.exe`. Non-windows users, use [Wine](http://www.winehq.org) for this task. Alternatively, you can use [asm56k](https://bitbucket.org/sqward/asm56k) by Sqward (fixed version by Thorsten Otto [here](https://github.com/th-otto/asm56k)) but you need a minor tweak to calc.asm's macros (`src`/`dst` -> `\1`) and use different command line arguments. For m68k code you need either [vasm](http://sun.hasenbraten.de/vasm) by Frank Wille or, if you prefer to have your code in C (what I strongly recommend for the initial phase of your project, optimize as the last thing), download Vincent Riviere's great [m68k cross tools](http://vincent.riviere.free.fr/soft/m68k-atari-mint). Another option is to use Frank's great [vbcc](http://sun.hasenbraten.de/vbcc) but I'm more used to `gcc`. And finally (not mandatory but it will make your start with this project easier) [Qt Creator](https://www1.qt.io/offline-installers/#section-2) by The Qt Company. This will provide you also all the needed tools and libraries for compilation of the main project. You're looking for `Qt X.Y.Z for ...` archive. The minimum required Qt version is 5.4.

I've tested compilation on Arch Linux (both 32-bit and 64-bit) and Windows 7/64-bit, it should work out of the box.

First try
---------

Suppose you've managed to compile and install all the prerequisites. Make sure that `PATH` contains something like `C:\Qt\Qt5.1.0\5.1.0\msvc2012_64_opengl\bin`, i.e. the Qt libraries you've just installed (it's not set by default!).

  2. Go to `tools/lod2p56` and open `lod2p56.pro`. Confirm default build directories and build the release configuration. Copy the output .exe somewhere you can find it. Close the project.
  3. Open cute56.pro in Qt Creator. Confirm default build directories.
  4. You should see one project and three subprojects: `atari`, `display` and `dsplib`. In the bottom left corner you can choose `Debug` and `Release` configurations, let's stick with `Debug` for now.
  5. Edit paths in `atari.pro` for `ASM56000`, `CLDLOD` (if needed) and `LOD2P56` (this one you definitely need to change) and save.
  6. Right click on `dsplib`, choose `Build "dsplib"` (subprojects in Qt are incredibly stupid, can't track initial dependencies)
  7. The same for `display` and `atari`.
  8. Run the program (`display`).

You should see a message `Failed to load Atari Library`, that's fine, we haven't loaded any yet. So go to `Modules` and choose (from your build directory) `calc.p56` as the `DSP Binary` and `libatari.{so,dll}` as the `Program` (in this order). You should see strange colours on the screen. Now you can press `F12` (or `Debug` -> `Enable`) and voila! You can debug your DSP code with `F11`. After some iterations you should see how the background color of the screen automatically changes, cool, isn't it. Now if you exit the simulator and run it again, the paths are remembered and the DSP binary will load & run automatically

Troubleshooting
---------------
Right now, everything works for me but be aware of following. If using __Windows__, `asm56k` should be compiled against [MinGW](http://www.mingw.org) and not [Cygwin](http://www.cygwin.com) (because of dependencies, Unix path handling etc). Not saying it's not possible but it requires to be an advance user.

Visual C++
----------

Trolltech/Nokia/Digia/The Qt Company has developed a nice plugin for Visual Studio which allows you to import `qmake`'s project file and create a solution file out of it and therefore using Visual Studio's debugger directly (of course, you can always attach the debugger to your process from outside but it's less convenient). You can download it for free from the same location as the other Qt stuff. It should work out of the box, with exception of: flags (right click on `cute56` -> `Properties` -> `Configuration Properties` -> `C/C++` -> `Advanced` -> `Compile as` and set to `Compile as C++ code /TP`), `dsp.lib` linked to `display` (`display` -> `Properties` -> `Configuration Properties` -> `Linker` -> `Input` -> `Additional Dependencies`) and, should you use/want it, x64 build (`BUILD` -> `Configuration Manager` -> `Active Solution Platform` -> `New` -> `x64`). Sometimes you need to (re)set the Qt version, i.e. right click on the solution root -> `Change Solution's Qt version` and select the one you've installed. I've prepared a solution file for VS2012, if you use an older one, you need to follow the process described above.

Native build
------------

So you want to try it in a _real_ emulator. Open a console (oh yeah), go to the `atari-build` directory, edit the `Makefile` (if needed) and type `make`. This should produce `atari.tos` binary, which you can run in an emulator (Hatari). `main.c` is quite a mess (a lot of `#ifdef`s for host/native build) but usually you want to put these things into a library and forget about them. I just didn't want to complicate it more (another project, dependency etc). I guess this could be automated more (and perhaps included in the Qt Creator project) but I was lazy and in rush, blah blah.

Warning
-------

Most of the code (everywhere) is just mess, mess, mess. Many things needs to be yet implemented (like writing to memory, labels, different stepping, ...) and many fixed (thread synchronization, crashes on stopping / reloading etc). Feel free to send me a patch but we all known nobody will bother :)

Contact and license
-------------------

All this stuff is free software, licensed under GPLv3 license. In a nutshell: you don't need to provide source code to your demos developed with this but if you change something in the simulator and/or want to publish the binary, you should publish also the source code. Feel free to drop me an email to <miro.kropacek@gmail.com> with any questions / improvements / ideas.

Btw, the DSP emulation is based on commit https://github.com/hatari/hatari/commit/a027c905 from June 2009.
