# SVision2

This will become svision2. 

Another GUI library. A re-implementation, next generation of svision:
http://dgi_il.tripod.com/sv/index.html


## Staus
Status is that this is not usable still, and it is under heavy development.

This project is known to compile under 
 
 * Linux: tested under Debian stable. Should work on any other Linux (tested only 64 bit, 
   but 32 shoud still compile and work).
 * Windows/MingGW: Tested under the default toolchain that ships with Qt 6.2. 
 * Windows/Clang(MSVC): I am having problems compiling it - as some conan packages do not
   compile under this compiler setup.
 * This project uses conan1. conan 2 is not usable yet, as some recipes
   have not been ported.

## Linux Setup

Install the packages for cmake, ninja, and basic X11 development
libraries - and python pip. For Debina/Ubuntu:

```
apt install cmake ninja-build python3-pip
```

Then install conan using `pip` (on Debian I found that `pipx` works and `pip` does not):

```
pip3 install conan==1.59.0
```

To compile from the command line:

```
cmake -s . -b build
cmake --build build
```

Any IDE compatible with CMake should be usable. I use
VSCode (it's a pain to setup on Windows, but on Linux it just
works). QtCreator will also work.

## Windows Setup
Install:
 * Qt 6.2.0 or above - just to have an IDE and usable toolchain 
     - https://www.qt.io/download-open-source - official
     - https://github.com/miurahr/aqtinstall -  alternative installer (command line based)
 * CMake 3.24 or above:
     - https://cmake.org/download/
 * Python - I get the python from the Microsoft store, seems like the best idea.
 * Ninja - make sure `c:\Qt\Tools\Ninja\` is on your path, or copy the `ninja.exe` so 
   somewhere on your path.
 * Conan - from a terminal write `pip install conan==1.59.0`

Start QtCreator - and open the `CMakeLists.txt` for this project. It will probably
fail at the `cmake` step. This is because `cmake` cannot execute `conan`. Fix:

 1. Open the *Project* group.
 2. Open your toolkit *Build* panel
 3. Inside *Environment* look for `path` and add
   `C:\Users\diego\AppData\Local\Packages\PythonSoftwareFoundation.Python.3.10_qbz5n2kfra8p0\LocalCache\local-packages\Python310\Scripts` - or the path that is used on your computer (HOW TO DEDUCT THIS PATH?)

Project should build now.

Note:
> This project does not use Qt at all! I recommend Qt just to get a working
> copmiler easily. All code is pure C++14 calling GDI to plot the
> images. 


## OSX setup

... I don't own a Mac, so a PR is welcomed. If you want to
buy me a Mac, contact me.

This is planned - and should be easy to port.