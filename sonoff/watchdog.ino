/*
  watchdog.ino - Raspberry Pi WatchDog timer add-on to Sonoff-Tasmota firmware

  Copyright (C) 2018  Ron Boston

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sonoff.h"                         // Enumaration used in user_config.h

extern char log_data[];      // defined in sonoff.ino
extern char mqtt_data[];     // defined in sonoff.ino

// set pin numbers (defined in sonoff.ino):
extern const int relayPin;    // the number of the relay pin
extern const int watchDogPin; // the number of the watchdog input pin

// Debounce
int watchDogState;            // the current reading from the input pin
int lastWatchDogState = HIGH; // the previous reading from the input pin
// the following are unsigned longs because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long watchDogDelay = 120000; // WatchDog timeout - 5 mins
unsigned long lastWatchDogTime = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long relayDelay = 2000;    // Time relay is active
unsigned long lastRelayTime = 0;
unsigned long lastReconnectAttempt = 0;

// Array for recording watchdog timeouts
#define EXPIRETIMES 5
String expireTimes[EXPIRETIMES] = {"0", "0", "0", "0", "0"};
int indexTimes = 0;

/*
  Debounce an input

  This function derived from debounce code
  http://www.arduino.cc/en/Tutorial/Debounce
  This example code is in the public domain.

  modified 17 Sep 2017
  by Ron Boston
 */
void debounce() {
    // read the state of the watchdog input
    int reading = digitalRead(watchDogPin);
    // check to see if watchdog input active
    // If the input changed, due to noise or pressing:
    if (reading != lastWatchDogState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // if the watchdog input state has changed:
        if (reading != watchDogState) {
            watchDogState = reading;
          // only input state is LOW
            if (watchDogState == LOW) {
               lastWatchDogTime = millis();
            }
            snprintf_P(log_data, sizeof(log_data), PSTR("WDG: Watchdog Press!! r:%d; last:%d"), reading, lastWatchDogState);
            AddLog(LOG_LEVEL_INFO);         
        }
    }
    // save the reading.  Next time through the loop,
    // it'll be the lastWatchDogState
    lastWatchDogState = reading;
}

// If WatchDog time has expired, turn on relay
void checkWatchDog () {
    if (lastWatchDogTime == 0) {
        return;
    }
    
    char stopic[TOPSZ];

    if ((millis() - lastWatchDogTime) > watchDogDelay) {
        if (lastRelayTime == 0) {
            digitalWrite(relayPin, LOW);
            snprintf_P(log_data, sizeof(log_data), PSTR("WDG: Start Relay"));
            AddLog(LOG_LEVEL_INFO);
            lastWatchDogTime = 0;
            lastRelayTime = millis();
            storeExpires();
            GetTopic_P(stopic, 1, Settings.mqtt_topic, "POWER");
            snprintf_P(mqtt_data, sizeof(mqtt_data),"CYCLE at %s", GetDateAndTime().c_str());
            MqttPublish(stopic);
        }
    }
}

// If relay is on, stop it when timeout is reached
void checkRelay () {
    if (lastRelayTime == 0) {
        return;
    }
    if ((millis() - lastRelayTime) > relayDelay) {
        snprintf_P(log_data, sizeof(log_data), PSTR("WDG: Stop Relay"));
        AddLog(LOG_LEVEL_INFO);
        digitalWrite(relayPin, HIGH);
        lastRelayTime = 0;
    } 
}

// Store WatchDog Expire timestamps
void storeExpires () {
    indexTimes++;
    if (indexTimes > EXPIRETIMES) {
        indexTimes = 1;
    }
    expireTimes[indexTimes-1] = GetDateAndTime();
    snprintf_P(log_data, sizeof(log_data), PSTR("WDG: Expire: index: %d; time: %s;"), indexTimes, expireTimes[indexTimes-1].c_str());
    AddLog(LOG_LEVEL_INFO);
}
