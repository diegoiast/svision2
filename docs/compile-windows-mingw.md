# Mingw on Windows

## QtCreator - simple install

The simplest way to get a working *FOSS* toolchain is to use Qt6. This way
not only you get a useble GCC, but also Ninja and qgood IDE.

 1. Qt 6.2.0 or above - just to have an IDE and usable toolchain
     - <https://www.qt.io/download-open-source> - official server
     - <https://github.com/miurahr/aqtinstall> - alternative installer (command line based)
 2. CMake 3.24 or above:
     - <https://cmake.org/download/>
 3. Python - I get the python from the Microsoft store, seems like the best idea.
 4. Ninja - make sure `c:\Qt\Tools\Ninja\` is on your path, or copy the `ninja.exe` so
    somewhere on your path.
 5. Conan - from Windows Terminal write `pip install conan==1.64.1`

Start QtCreator - and open the `CMakeLists.txt` for this project. It will probably
fail at the `cmake` step. This is because `cmake` cannot execute `conan`.

Fix - add `conan` to your path

 1. Open the *Project* group.
 2. Open your toolkit *Build* panel
 3. Inside *Environment* look for `path` and add
   `C:\Users\diego\AppData\Local\Packages\PythonSoftwareFoundation.Python.3.10_qbz5n2kfra8p0\LocalCache\local-packages\Python310\Scripts` - or the path that is used on your computer (HOW TO DEDUCT THIS PATH?)

Project should build now.

> *Note*:
>
> This project does not use Qt at all! I recommend Qt just to get a working
> copmiler easily. All code is pure C++14/17 calling GDI/Win32 to plot the
> images.

## VSCode

I am able to use VSCode to edit, compile and debug the code under Windows 10.
You should install the needed toolchain "somehow" (I opted to use the toolchain
in QtCreator).

Documentation about installing MinGW indipendently is welcome.
