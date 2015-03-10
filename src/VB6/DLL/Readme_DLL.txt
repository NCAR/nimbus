Installing Real Time Software
-----------------------------
Copy MTP, Lib, Class, and Misc folders under VB6
Copy Mission Folder (eg PAVE)
Copy C:\MTP\RTA and C:\MTP\Setup folders
Copy C:\WINNT\SYSTEM32\janGraphics.dll
Copy C:\WINNT\SYSTEM32\vbpng1.dll
Copy C:\WINNT\SYSTEM32\zlib.dll
Make sure UID$ is correct in MTPsetup.INI (eg MTP on DC8)

Capture needs C:\WINNT\SYSTEM32\SSubTmr6.dll
Capture needs C:\WINNT\SYSTEM32\janGraphics.dll
Register dll's with: RegSvr32 SSubTmr6.dll and RegSvr32 janGraphics.dll
