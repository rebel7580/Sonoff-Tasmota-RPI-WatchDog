## Raspberry PI WatchDog

This software is based on Sonoff-Tasmota software (See https://github.com/arendst/Sonoff-Tasmota)

This software detects a hung Raspberry Pi by not receiving periodic pulses on GPIO00 from the Raspbeery Pi and triggers a relay on GPIO02 to momentarily cut power to the Raspberry Pi. Not particularly elegant, but it's a "last resort" when the Raspberry Pi is otherwise incommunicado.

Originally, this was a implemented based on other simpler sw; adding the watchdog functionallity to the Sonoff-Tasmota sw brings in all the web, MQTT and OTA upgrade capabilities, allowing better and easier control.

Custom ESP8266-01s based hardware is required. See https://github.com/rebel7580/Sonoff-Tasmota-RPI-WatchDog/blob/master/ESP8266%20RPI%20Watchdog%20v2%20sch.pdf

In reference to the Original 5.11.1b Sonoff-Tasmota sw on which this is based, here are the files moddifed/added:

Added:

* watchdog.ino - adds most of the watchdog code.

Changed:

* sonoff.ino - Add hooks in setup() and loop() to watchdog code

* webserver.ino - Add Watchdog Status page; web command and button on main page to access it.

The following settings help make the WatchDog "behave" properly:

 * ButtonTopic: Set to a topic that will be sent when the WatchDog receives a pulse from the RPI. This setting also prevents the pulse from directly toggling the state of the device. 
 * PowerOnState: 4 - Turn relay(s) on and disable further relay control. This leaves the device permenantly in the "ON" state. Since the actual relay is on GPIO2, not GPIO12, this has no effect on the actual relay.



