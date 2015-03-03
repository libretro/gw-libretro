# gwlua

A middleware to write Game & Watch-like games in [Lua](http://www.lua.org/). Provides pictures, sprites, timers and sounds.

**gwlua** is a *middleware*, meaning it knows nothing about how sound is actually played and sprites blitted onto the screen. You have to provide functions to do that according to the platform you're targetting.

For an working example of **gwlua**, see [gw-libretro](https://github.com/libretro/gw-libretro).

## Usage

Add gwlua.c and gwlua.h to your project. See gwlua.h for some documentation.

## Rationale

When I started porting [MADrigal](http://www.madrigaldesign.it/sim/)'s Game & Watch simulators to [libretro](http://www.libretro.com/), I had to make a decision on how to take advantage of the existing simulators' source code. The simulators are written in Delphi, so I could:

1. Try to compile the original source code with [Free Pascal](http://www.freepascal.org/): This would turn each simulator into its own libretro core, instead of a content able to be played by a core. In addition, I'd have to provide functions to make the Delphi compiled work, like Application and Form classes.
1. Try to compile the original source code with [Free Pascal](http://www.freepascal.org/) to some machine code (I was thinking about MIPS) and add a CPU emulator to the libretro core: This would let me package each game into a content, but I'd have to bootstrap high level stuff as machine code, i.e. picture loading and blitting.
1. Rewrite the original source code in another language: This would free me from Free Pascal, but would mean each simulator is a separate core just like the first option.

So I decided to go with an option that I believe has the best mix: rewrite the Pascal code in Lua, and package the Lua source code along with assets into a libretro content. The libretro core would then be able to read the archive and run the Lua code, which in turn can easily call native functions to deal with IO etc.

## Why Lua

I have experience with both the language and on embedding it in C/C++ programs, and it's syntax is similar to that of Pascal. In fact, I wrote a translator that takes Pascal source code and generates Lua source code. The translator is far from perfect, but it does the hard work, leaving me to just tweak a few lines.
