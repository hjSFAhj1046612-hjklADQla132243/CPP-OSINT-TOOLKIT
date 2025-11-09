# CPP-OSINT-TOOLKIT
A powerful, easy-to-use OSINT (Open Source Intelligence) multitool written in C++, designed to streamline and accelerate the process of gathering public information.
With a simple command-driven interface, this toolkit enables fast reconnaissance and digital footprint mapping—perfect for researchers, security professionals, and anyone diving into open-source investigations.

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

# Tool itself
## Prefix
The prefix when you're running a command is "!", type "!help" in your terminal to view all the commands.

## List of (most) Commands
!whois <domain>
!dns <domain>
!ipgeo <ip>
!google <query>
!sherlock <username>
!harvest <domain>
!subdomains <domain>
!emailcheck <email>
!iplookup <ip>
!breach <email>
!phone <number>
!urlinfo <url>
!social <domain>
!help

