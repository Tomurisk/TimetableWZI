# TimetableWZI
Timetable**WZI** (*Timetable **with Zoom integration***) is a simple console application to ease the management of school subjects, while providing an efficient way of accessing Zoom remote classes.

# Requirements
This program was built using Visual Studio 2019 and written in C++, therefore a [32-bit Visual C++ 2015-2019 redistributable](https://aka.ms/vs/16/release/vc_redist.x86.exe) is required. It should be already present in most systems running Windows 10.

[Zoom client](https://zoom.us/download) is also required as the program will ask the user to input Zoom links, which will later be converted into arguments for launching Zoom meetings.

# Building TimetableWZI
Unfortunately some antivirus software may detect my program as a virus and there is nothing I can do about it. It is a false positive, but feel free to build it using instructions below!

1. Download [Visual Studio 2019 Community](https://visualstudio.microsoft.com/vs/).
2. Open the downloaded setup executable. Shortly after, a program called Visual Studio Installer should be installed on your computer. It will automatically open a window to select a workload. Uncheck all other boxes if checked, check **Desktop development with C++**, then click the **Install** button.
3. Download [a ZIP file of TimetableWZI's `main` branch](https://github.com/Tomurisk/TimetableWZI/archive/refs/heads/main.zip) or clone using `git`.
4. Supported file formats should be set to open with Visual Studio. Navigate to **Sources** directory and open the solution (.sln) file.
5. Change solution's configuration to **Release** and platform to **x86**.
6. Select **TimetableWZI** element in Solution Explorer, right click on it and select **Properties**. Navigate to Configuration Properties > C/C++ > Preprocessor > Preprocessor Definitions, then add `_CRT_SECURE_NO_WARNINGS` to the bottom.
7. Build the program with **Local Windows Debugger**. The output executable will be placed in a separate folder called **Release**, located in **Sources**.
