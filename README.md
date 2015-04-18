# gw-libretro

**gw-libretro** is a [libretro](http://www.libretro.com/) core that runs Game & Watch simulators.

It runs simulators converted from source code available in [MADrigal](http://www.madrigaldesign.it/sim/). Each simulator was converted with [pas2lua](https://github.com/leiradel/pas2lua), which was written specifically for this purpose, and uses [bstree](https://github.com/leiradel/bstree), which was also specifically written to obfuscate the generated Lua source code as per MADrigal's request.

The converted games are available at [libretro's buildbot site](http://bot.libretro.com/assets/cores/gw/). Unpack the zip and read the README.txt for instructions.
