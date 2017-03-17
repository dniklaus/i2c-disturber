# i2c-disturber
Wiring / Arduino based Embedded Application, powered by [PlatformIO](http://platformio.org "Cross-platform build system")

The project is initially created for Arduino DUE (Atmel SAM / ARM).

## Purpose
This project builds up an Arduino Framework based I2C bus bit error injectior application and it comprises of several components helping with debugging and integrating the embedded applications on [Arduino](http://arduino.cc).

The command line interface provides the following functionality:  


This skeleton application can help you to build up your own Arduino Framework based applications with focus on IoT. 

This application uses the following copmonents:

* [Timer](https://github.com/dniklaus/arduino-utils-timer)
* [Debug-Cli](https://github.com/ERNICommunity/debug-cli)
* [Dbg-Trace](https://github.com/ERNICommunity/dbg-trace)

## How to build for Eclipse CDT
  1. Create a directory where your Eclipse Workspace will be stored and where this project shall be cloned into. E.g. `C:\git\pio-prj`
  2. Clone this repository recursively into the folder you created before, `git clone --recursive git@github.com:dniklaus/wiring-iot-skeleton.git`
  3. Open a command shell in the just cloned project folder, i.e in `C:\git\pio-prj\wiring-iot-skeleton`
  4. Run the command `pio init -b nodemcuv2 --ide eclipse`
  5. Run the command `pio run`

## Open project in Eclipse CDT
  6. Open Eclipse CDT, choose the folder you created before as workspace, i.e `C:\git\pio-prj`
  7. Import the project with File->Import->General->Existing Projects into Workspace, choose the `i2c-disturber` (i.e `C:\git\pio-prj\i2c-disturber`)
  8. Connect PC to the Arduino DUE programming port using a micro USB cable.
  9. Edit the file platformio.ini, alter the line `upload_port = COM4` if your Arduino DUE is connected to another COM port
 10. Run the command `pio run -t upload` in order to upload the sketch

## Connect Terminal Emulation
In order to run the CLI commands, a terminal emulation program shall be used. The one giving you the best experience will be the [HTerm](http://www.der-hammer.info/terminal/). 
Load the _hterm-com18.cfg_ file to configure HTerm properly. Alter the COM18 accordingly to the one that has been selected on your computer.

## Debug Features
### Debug CLI Commands
#### Command Tree
     dbg                      Debug CLI root node
       tr                     Debug Trace Port config access
         heap                 Particular Trace Port (heap: see below in chapter Trace Port)
           out                Trace Output config access for the current trace port
             get              Show the assigned Trace Output
             set <outName>    Set a particular Trace Output name
             list             Show all available Trace Output names (and the currently selected)
           lvl                Trace Level config access for the current trace port
             get              Show the current trace level
             set <level>      Set a particular trace level
             list             Show all available trace levels (and the currenntly selected)
       dbg 
         i2c 
           start
           stop
           seq
              run
              quit
              stat
           

#### Example commands
* `dbg tr heap lvl set debug`
* `dbg i2c run`


### Trace Port
|Trace Port|default level|functionality|
|----------|-------------|:------------|
|heap|info|if set to debug level: automatically print free heap memory [bytes], every 10 seconds|

## Library Usage
This chapter lists all the libraries this project is using.

### PlatformIO Libraries
|ID|Name|URL|Description|
|:--|:-------|:----------------|:-----------------------|
|173|SerialCommand|https://github.com/kroimon/Arduino-SerialCommand|A Wiring/Arduino library to tokenize and parse commands received over a serial port.|
|550|ThingSpeak|https://github.com/mathworks/thingspeak-particle|"MathWorks": ThingSpeak Communication Library for Arduino & ESP8266|


### Homebrew Libraries
|Name|URL|Description|
|:------|:---------------------|:-------------------------------|
|Timer|https://github.com/dniklaus/arduino-utils-timer|Universal recurring or non-recurring Timer|
|Debug-Cli|https://github.com/ERNICommunity/debug-cli|Debug CLI for Embedded Applications - Command Line  Interface for debugging and testing based on object oriented tree structure.|
|Dbg-Trace|https://github.com/ERNICommunity/dbg-trace|Debug Trace component for Embedded Applications - Debug and Trace Log message system based on trace ports with adjustable levels.|
|RamUtils|https://github.com/dniklaus/arduino-utils-mem|Arduino Memory Utilities|

