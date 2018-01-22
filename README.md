## Raspberry PI WatchDog is based on Sonoff-Tasmota software (See https://github.com/arendst/Sonoff-Tasmota)

This detects a hung Raspberry Pi by not receiving periodic pulses on GPIO00 from the Raspbeery Pi and triggers a relay on GPIO02 to momentaryily cut power to the Raspberry PI. Not particularly elegant, but it's a "last resort" when the Raspberry Pi is otherwise incommunicato.

In reference to the Original 5.11.1b Sonoff-Tasmota sw on which this is based, here ate the files moddifed/added:
Added:
watchdog.ino - adds most of the watchdog code.

Changed:
sonoff.ino
webserver.ino
support.ino
setting.ino
settings.h
user_config.h
i18n.h
en_GB.h


