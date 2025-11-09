# CPP-OSINT-TOOLKIT
This repository contains a simple to navigate OSINT Toolkit in C++ made for gathering public information.

# TUTORIALS
## step-by-step

**Download a portable WinLibs MinGW-w64 build (no installer).**
Visit: https://winlibs.com/
 and download the latest WinLibs Mingw-w64 ZIP File.

**Extract the archive (you can use 7-Zip). Put the extracted folder somewhere like C:\winlibs.**

**Open Command Prompt and change to the folder with your .cpp file. Example:**

cd C:\Users\YourName\Desktop


**Compile using the portable g++ (use full path to the portable g++.exe):**

"C:\winlibs\mingw64\bin\g++.exe" ostkit.cpp -o ostkit.exe


**If your extracted folder name differs, adjust the path.**

"C:\winlibs\mingw64\bin\g++.exe" -std=c++17 ostkit.cpp -o ostkit.exe


**Run the resulting executable:**

ostkit.exe


or

.\ostkit.exe

And you're done!
