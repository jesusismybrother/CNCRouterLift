# CNCRouterLift
An open source CNC Routerlift with support for an Android App.
For a video demonstration of the entire Project click [here](www.youtube.com/uniquelymade).

### Introduction

This repository is composed of three different parts
* Electronics
* Mechanics
* Android App
 
### Electronics
In this folder you will find:

* Code
The Code for the project is written in Visual Studio Code in combination with PlatformIO.
If you are more comfortable with the Android IDE you can open the file src/main.cpp with an editor of your choice and copy the code to a new Android project.

* Pinout
This is just a document that tells you what pins should be soldered to which pins.

### Mechanics

This folder contains the .stp file of the entire assembly.
This format should be openable in most CAD programs such as Fusion360, Solidworks, NX, etc.

You can see it in Fusion360 with [this link](https://a360.co/2SwOq4j), but to actually use it you will need to upload the .stp file to your own Fusion360 accout.

[Here is a Tutorial on how to do it.](https://knowledge.autodesk.com/support/fusion-360/learn-explore/caas/sfdcarticles/sfdcarticles/How-to-import-or-open-a-file-in-Autodesk-Fusion-360.html)



### Android App
This folder containes the finished .apk for you to install on your android device.
It also containes an archived version of the [MIT App Inventor](http://appinventor.mit.edu/) project used to create the app.

### Other information

#### BOM

#### Electronics
| Item            | Name          | Quantity | Note                                      |
|-----------------|---------------|----------|-------------------------------------------|
| Mikrocontroller | ESP32         | 1        | Cheap and has onboard Bluetooth           |
| Stepper Motor   | 17HS4401      | 1        | Any Nema 17 steppermotor will do          |
| Stepper Driver  | Timatic 2100  | 1        | Should be selected according to the Motor |
| LCD             | 1604 with IC² | 1        | Make sure to select the one with IC²      |

#### Mechanics

| Item               | Name        | Quantity | Note                                                                                   |
|--------------------|-------------|----------|----------------------------------------------------------------------------------------|
| Bearings           | LM12UUOP    | 4        |                                                                                        |
| Leadscrew Assembly | This one    | 1        | Pitch 1mm Lead 1mm 200mm long (Tell them to give you the correct couple for the Motor) |
| Linear Rails       | SBR12 150mm | 1        |                                                                                        |
#### Pinout

| Pin | Destination             |
|-----|-------------------------|
| 4   | Button → "Unit"           |
| 5   | Button → "Zero"           |
| 12  | Stepper Driver → "Dir"    |
| 14  | Stepper Driver → "Step"   |
| 15  | Button → "Down"          |
| 18  | Button → "Change"         |
| 19  | Button → "Park"           |
| 21  | LCD → SDA                 |
| 22  | LCD → SCL                 |
| 23  | Limit Switch            |
| 27  | Button → "Up"             |
| 32  | Stepper Driver → "Enable" |

