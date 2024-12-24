# Visual Studio

Code is tested to compiler using MSVC 2019, community edition. Newer versions
should also work - this is just untested.

## Download the code

I found the best way to work on git on Windows is to use [GitHub Desktop](https://desktop.github.com/).

## Install build system

 1. Install Visual Studio
 2. CMake 3.24 or above:
     - <https://cmake.org/download/>
 3. Python - I get the python from the Microsoft store, seems like the best idea.
 4. Ninja - <https://ninja-build.org/>
     - UNpack the zip (a single `ninja.exe`) into `%HOMEDRIVE%%HOMEPATH%\ninja`
       (usually `c:\user\diego\ninja`).  
 5. Conan
      - from Windows Terminal write `pip install conan==1.66.0`
 6. Setup environment variable `%PATH%` - to contain the needed path
     - Visit <https://helpdeskgeek.com/windows-10/add-windows-path-environment-variable/>
       to understand how to do this.
     - Add the following paths:
        - `c:\program files\cmake\bin`
        - `%HOMEDRIVE%%HOMEPATH%\ninja`
        - `C:\Users\diego\AppData\Local\Packages\PythonSoftwareFoundation.Python.3.10_qbz5n2kfra8p0\LocalCache\local-packages\Python310\Scripts` (adapt this to your system)

## Opening Visual Studio

Just open a directory from Visual Studio - and configure the CMake project. No other
configuration is needed.
