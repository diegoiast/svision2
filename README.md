# SVision2

This is SVision2. Another GUI library. A continuation of *SVision for MSDOS*: [SVision for MSDOS](http://dgi_il.tripod.com/sv/).

![License](https://img.shields.io/github/license/elcuco/svision2.svg) <img src="https://img.shields.io/github/languages/top/elcuco/svision2.svg" alt="Languages"/> <img src="https://img.shields.io/github/repo-size/elcuco/svision2.svg" alt="Repository size"/> <img src="https://img.shields.io/github/commit-activity/y/elcuco/svision2" alt="Activity"/>

![SVision2 - on Linux](sv2.png "Written in C++, Linux/Windows/OSX (?) support")

## Goals

* The main goal is to be able to replace QtWidgets.
* Should be using modern idioms, and tools.
  * CMake for building.
  * Conan for package management (vcpkg support sounds nice).
  * For formatting is done using `clang-format`.
  * RAII (new widgets are `std::make_shared<Button>`)
  * [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance),
     instead of nested inheritance trees, move shared behaviours into
     different helper classes (see `src/buttonstates.h` (used by `Button` and `Checkbox`).
  * Avoid using the C/C++ precompiler (no `#ifdef`).
  * No [PIMPL](https://en.cppreference.com/w/cpp/language/pimpl). While its a nice idea, it complicates things when you want
     to debug.
    * C++ has no concept of ABI. Do not fight it.
    * When needed - ABI will be enforced by a set of tools that will query
         public APIs
  * When using platform agnostic objects:
    * By default you get one specific to your platform.
    * On Windows when you ask for `Timer` in practice you get a `Win32Timer`, and you can see on the debugger the `UINT_PTR timer_id`
    * On Posix (Linux/OSX/*BSD), you get a `PosixTimer` and you can see its `timer_t timerid`.
* Using "new C++ syntax": `auto func() -> const int;` - for everything.
* Initiailization of object is done in public (in the header, for trivial thing, instead of
   constructor). Always uses `int foo = 0; Point location = {}`. So you can look for `=` and
   know when a member is assigned by default.
* *WIP*: Gitlab mirror.
* *TODO*: Github actions to verifying, and building code on merge.
* *TODO*: For font management - use Freetype.
* *TODO*: For text displaying - use Harfbuzz.
* *TODO*: This library should be a C++20 module.
* *TODO*: This library should be on Conan, vcpkg, etc for easy consuming.

## Status

Status is that this is not usable still, and it is under heavy development.

* Linux: tested under Debian stable. Should work on any other Linux (tested only 64 bit,
   but 32 shoud still compile and work).
* Windows/MingGW: Tested under the default toolchain that ships with Qt 6.2.
* Windows/Clang(MSVC): Using MSVC 2019.
* This project uses conan1. conan 2 is not usable yet, as some recipes
   have not been ported.

For compilation instructions see [Compilation instructions](docs/compile.md)

* [Linux instructions](docs/compile-linux.md)
* [Windows/MINGW instructions](docs/compile-windows-mingw.md)
* [VisualStudio instructions](docs/compile-windows-vs.md)

## OSX setup

... I don't own a Mac, so a PR is welcomed. If you want to
buy me a Mac, contact me. This is planned - and should be easy to port.

## Porting to new platforms

The design of this toolkit is trivial: everything is a raster image. No GPU.
Each widget has its own buffer it writes to it, and it gets painted
into the parents

1. `src/platform-foo.{h,cpp}`: Implement a new `struct PlatformFOO : Platform`:
   1. `open_window()` returns your platform window (see bellow).
   2. `invalidate(PlatformWindow &w)` first cast to your own window:
      `auto window = static_cast<PlatformWindowFOO *>(&w);` (no need to validate, if someone
      is sending a different class on purpose - an uglier problem happens).
   3. `show_window()` - again, cast to your own class, to get the correct handle (`platformwin32.cpp`
      is a good simple example).
   4. `invalidate()` - again, cast to your own class, to get the correct handle.
      This paints the window content into your platform windowing system.
   5. `main_loop()` - note that you should not exit on last window open. See `platformwin32.cpp` for
      reference.
   6. Convert local event, keyboard events to the ones on this system. See
      `convert_x11_key_event()`  as example.
   7. Add at the bottom of the header `using ThePlatform = PlatformFOO`.
2. `PlatformWindowFOO`: this class defines a new window. You can add your local code (`HWND` or `XID`).
   Do this internally in the `PlatformFOO.cpp` file - only forward declare this in the header, as
   this is an pure implementation detail.
3. `src/timer.{h,cpp}`: You will also need a platform timer. inside `timer.h`, add define `struct FooTimer : PlatformTimer` - and at the end `using Timer = FooTimer`.
