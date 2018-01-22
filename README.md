## Raspberry PI WatchDog is based on Sonoff-Tasmota software (See https://github.com/arendst/Sonoff-Tasmota)

This software detects a hung Raspberry Pi by not receiving periodic pulses on GPIO00 from the Raspbeery Pi and triggers a relay on GPIO02 to momentarily cut power to the Raspberry Pi. Not particularly elegant, but it's a "last resort" when the Raspberry Pi is otherwise incommunicado.

Custom ESP8266-01s based hardware is required.

In reference to the Original 5.11.1b Sonoff-Tasmota sw on which this is based, here are the files moddifed/added:
Added:
watchdog.ino - adds most of the watchdog code.

Changed:
sonoff.ino - Add hooks to watchdog code
webserver.ino - Add Watchdog Status page; web command and button on main page to access it.
user_config.h
i18n.h
en_GB.h


