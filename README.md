cute56
======

cute56 is a Qt based simulator &amp; debugger for DSP 56001 development. Its target audience is mostly myself :) but if you would like to experiment with C/C++ and [Motorola DSP 56001] [2] assembler, it might be the right tool for you (without the need of installing [Hatari] [1] for instance). The main advantage (as I see it) is that when you manage to make a nice multiplatform library for common tasks (setting up the screen, timers, dsp communication, ... ) you can completely forget whether you're targeting the native platform (Atari m68k in my case) or the host.

  [1]: http://hatari.tuxfamily.org "Hatari"
  [2]: http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=DSP56001 "Motorola DSP 56001"

Prerequisites
-------------

I'll describe my scenario, if you don't want to mess with m68k at all, you can skip most of it. Also be aware of the fact that you need to compile this stuff by yourself, nothing for pussies :) First of all, you need [asm56k] [3] by Sqward, without it you can't compile and run your dsp code. If you know a better one let me know but this one is really good, nearly 100% compatible with the good old `qdsp`. For m68k code you need either [vasm] [4] by Frank Wille or, if you prefer to have your code in C (what I strongly recommend for the initial phase of your project, optimize as the last thing), download Vincent Riviere's great [m68k cross tools] [5]. And finally (not mandatory but it will make your start with this project easier) [Qt Creator] [6] by Digia. This will provide you also all the needed tools and libraries for compilation of the main project.

  [3]: https://bitbucket.org/sqward/asm56k "asm56k"
  [4]: http://sun.hasenbraten.de/vasm "vasm"
  [5]: http://vincent.riviere.free.fr/soft/m68k-atari-mint "m68k cross tools"
  [6]: http://qt-project.org/downloads "Qt Creator"

I've tested compilation on Arch Linux (both 32-bit and 64-bit), it should work out of the box on Windows and Mac, too. However, as it uses (for the time being) the deprecated OpenGL calls, it might not compile on your platform. To be investigated.

First try
---------

Suppose you've managed to compile and install all the prerequisites. That means `asm56k` and `qmake` are somewhere in your `PATH` or at least in a known location. Step by step tutorial:

  1. Open cute56.pro in Qt Creator. Confirm default build directories.
  2. You should see one project and three subprojects: `atari`, `display` and `dsplib`. In the bottom left corner you can choose `Debug` and `Release` configurations, let's stick with `Debug` for now.
  3. Right click on `dsplib`, choose `Build "dsplib"` (subprojects in Qt are incredibly stupid, can't track initial dependencies)
  4. The same for `display` and `atari`. You need to edit `atari.pro` and put a correct path to `asm56k` before.
  
If you see errors about undeclared `glMatrixMode()` and friends, that's the deprecated OpenGL issue. If you see errors about `-std=c99` and friends, that's because you're using a Visual C++ compiler. To be investigated :)

But, suppose you're a lucky user of a Linux distro and everything went smoothly. Run the program (`display`). You should see a message `Failed to load Atari Library`, that's fine, we haven't loaded any yet. So go to `Modules` and choose `libatari.so` (from your build directory) as the `Program` and `calc.p56` as the `DSP Binary`. You should see strange colours on the screen. Now you can press `F12` (or `Debug` -> `Enable`) and voila! You can debug your DSP code with `F11`. After some iterations you should see how the background color of the screen automatically changes, cool, isn't it. Now if you exit the simulator and run it again, the paths are remembered and the DSP binary will load & run automatically

Native build
------------

So you want to try it in a _real_ emulator. Open a console (oh yeah), go to the `atari-build` directory, edit the `Makefile` (if needed) and type `make`. This should produce `atari.tos` binary, which you can run in an emulator ([Hatari] [1]). `main.c` is quite a mess (a lot of `#ifdef`s for host/native build) but usually you want to put these things into a library and forget about them. I just didn't want to complicate it more (another project, dependency etc).

Warning
-------

Most of the code (everywhere) is just mess, mess, mess. Many things needs to be yet implemented (like writing to registers / memory, labels, different stepping, ...) and many fixed (thread synchronization, crashes on stopping / reloading etc). Feel free to send me a patch but we all known nobody will bother :)

Contact and license
-------------------

All this stuff is free software, licensed under GPLv3 license. In a nutshell: you don't need to provide source code to your demos developed with this but if you change something in the simulator and/or want to publish the binary, you should publish also the source code. Feel free to drop me an email to <miro.kropacek@gmail.com> with any questions / improvements / ideas.