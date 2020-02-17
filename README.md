# PC300 Doorbell Mod

This is a pretty popular doorbell and there were many that felt it's chime was less than pleasing. Therefor I embarked on modifying this doorbell to play a different tone. The goal was to make minimal modification so that it visibaly was hard to identify a modification was made.

I initally atempted to identify the chip that was being used in hope of reporogramming it without any luck. However, I was able to find a drop in microcontroller with a compatable pinout to the original and in the same package.

## Organization
Audio - Original audio files and projects
Code - MPLabX project file
Datasheets - Datasheets of devices on the PC300 and the new PIC12F157
PCB - Schematic of the PC300 created by manually tracing out conenctions on the 2 layer PCB.
Tools - Contains the PCM2h tool used to convert wav PCM files to C header files.

## Process
This is a quick summary of my steps to generate this.
 - Trace out PCB and understand circuit. Verify operating votlage and IO voltage ranges.
 - Find a microcontroller with the same package and compatable pinout and voltage. Settled on the PIC12F157.
 - Program the PIC12F157 (Setup PWM modules and create code to play out small audio samples)
 - Obtain the sound desired, reduce bitrate to lowest acceptable level, reduce clip size, and save as signed 8 bit PCM, all to make it fit on the chip. Settled on Mario's pipe warp sound at a bitrate of 7813Hz (8bit). This was due to matching the sampling rate of internal timers on the PIC.
 - Use the PCM2h utility to take the PCM audio file and generate an array for the microcontroller to use.
 - Play file and tune code for playback modes.
