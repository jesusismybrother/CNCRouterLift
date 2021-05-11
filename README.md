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
| Item            | Name                                                                                                                                                                                                                                                                                                         | Quantity | Note                                      |
|-----------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------|-------------------------------------------|
| Mikrocontroller | [ESP32](https://www.aliexpress.com/item/1005001757645011.html?spm=a2g0o.productlist.0.0.5f4351a1LQTLjh&algo_pvid=fccc381f-b59c-4fb2-bcb3-861be3b7c831&algo_expid=fccc381f-b59c-4fb2-bcb3-861be3b7c831-0&btsid=2100bde716207352577692887ec1e6&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)      | 1        | Cheap and has onboard Bluetooth           |
| Stepper Motor   | [17HS4401](https://www.aliexpress.com/item/32591963406.html?spm=a2g0s.9042311.0.0.6b414c4di6SC9B)                                                                                                                                                                                                            | 1        | Any Nema 17 steppermotor will do          |
| Stepper Driver  | [Timatic 2100](https://reprap.org/wiki/TMC2100)                                                                                                                                                                                                                                                              | 1        | Should be selected according to the Motor |
| LCD             | [1604 with IC²](https://www.aliexpress.com/item/4000370284169.html?spm=a2g0o.productlist.0.0.52cf6c12EPFdrq&algo_pvid=ffb9654e-687d-4a5e-be7f-337257436c71&algo_expid=ffb9654e-687d-4a5e-be7f-337257436c71-0&btsid=0b0a182b16207329769032604ec805&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_) | 1        | Make sure to select the one with IC²      |
#### Mechanics

| Item               | Name        | Quantity | Note                                                                                   |
|--------------------|-------------|----------|----------------------------------------------------------------------------------------|
| Bearings           | [LM12UUOP](https://www.aliexpress.com/item/32777334438.html?spm=a2g0s.9042311.0.0.6b414c4di6SC9B)    | 4        |                                                                                        |
| Leadscrew Assembly | [This one](https://www.aliexpress.com/item/32599716891.html?spm=a2g0s.9042311.0.0.6b414c4di6SC9B)    | 1        | Pitch 1mm Lead 1mm 200mm long (Tell them to give you the correct couple for the Motor) |
| Linear Rails       | [SBR12 150mm](https://www.aliexpress.com/item/1005001571893671.html?spm=a2g0o.productlist.0.0.2c9c61b6QjXSjZ&algo_pvid=null&algo_expid=null&btsid=2100bdf116207326987664173e79fd&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_) | 1        |                                                                                        |
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

