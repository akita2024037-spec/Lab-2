#include <Arduino.h>

#ifndef ACTIVE_EXERCISE
#define ACTIVE_EXERCISE 1
#endif

#if ACTIVE_EXERCISE < 1 || ACTIVE_EXERCISE > 4
#error "ACTIVE_EXERCISE must be between 1 and 4"
#endif

namespace {
constexpr uint8_t RED_LED_PIN = 26;
constexpr uint8_t GREEN_LED_PIN = 27;
constexpr uint8_t YELLOW_LED_PIN = 12;
constexpr uint8_t BLUE_LED_PIN = 14;
constexpr uint8_t LIGHT_SENSOR_PIN = 33;
constexpr uint8_t BUTTON_PIN = 25;

#if ACTIVE_EXERCISE == 1
constexpr uint8_t chasePins[] = {
    RED_LED_PIN, GREEN_LED_PIN, YELLOW_LED_PIN,
    BLUE_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN
};
constexpr const char *chaseNames[] = {
    "RED", "GREEN", "YELLOW", "BLUE", "YELLOW", "GREEN"
};
constexpr size_t CHASE_LENGTH = sizeof(chasePins) / sizeof(chasePins[0]);
size_t chaseStep = 0;
#elif ACTIVE_EXERCISE == 2
unsigned long lastStatisticsAt = 0;
#elif ACTIVE_EXERCISE == 3
unsigned long lastAlertReadAt = 0;
bool alertActive = false;
#elif ACTIVE_EXERCISE == 4
constexpr uint8_t ledPins[] = {
    RED_LED_PIN, GREEN_LED_PIN, YELLOW_LED_PIN, BLUE_LED_PIN
};
constexpr unsigned long DEBOUNCE_MS = 30;
uint8_t pressCount = 0;
bool lastRawButtonState = LOW;
bool stableButtonState = LOW;
unsigned long lastButtonChangeAt = 0;

void showPressCount() {
    for (uint8_t index = 0; index < 4; ++index) {
        digitalWrite(ledPins[index], index < pressCount ? HIGH : LOW);
    }
}
#endif
}

void setup() {
    Serial.begin(115200);

#if ACTIVE_EXERCISE == 1
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
#elif ACTIVE_EXERCISE == 2 || ACTIVE_EXERCISE == 3
    pinMode(LIGHT_SENSOR_PIN, INPUT);
    const unsigned long now = millis();
#if ACTIVE_EXERCISE == 2
    lastStatisticsAt = now;
#else
    lastAlertReadAt = now;
#endif
#elif ACTIVE_EXERCISE == 4
    for (uint8_t pin : ledPins) {
        pinMode(pin, OUTPUT);
    }
    pinMode(BUTTON_PIN, INPUT);
    lastRawButtonState = digitalRead(BUTTON_PIN);
    stableButtonState = lastRawButtonState;
    lastButtonChangeAt = millis();
    showPressCount();
#endif
}

void loop() {
#if ACTIVE_EXERCISE == 1
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);

    digitalWrite(chasePins[chaseStep], HIGH);
    Serial.print("chase=");
    Serial.println(chaseNames[chaseStep]);
    delay(150);
    chaseStep = (chaseStep + 1) % CHASE_LENGTH;
#elif ACTIVE_EXERCISE == 2
    const unsigned long now = millis();
    if (now - lastStatisticsAt >= 1000) {
        lastStatisticsAt = now;
        int minimum = 4095;
        int maximum = 0;
        int total = 0;

        for (uint8_t sample = 0; sample < 10; ++sample) {
            const int reading = analogRead(LIGHT_SENSOR_PIN);
            if (reading < minimum) minimum = reading;
            if (reading > maximum) maximum = reading;
            total += reading;
        }

        Serial.print("min=");
        Serial.print(minimum);
        Serial.print(" max=");
        Serial.print(maximum);
        Serial.print(" avg=");
        Serial.println(total / 10);
    }
#elif ACTIVE_EXERCISE == 3
    const unsigned long now = millis();
    if (now - lastAlertReadAt >= 300) {
        lastAlertReadAt = now;
        const int reading = analogRead(LIGHT_SENSOR_PIN);

        if (!alertActive && reading > 3000) {
            alertActive = true;
            Serial.println("ALERT=1");
        } else if (alertActive && reading < 2500) {
            alertActive = false;
            Serial.println("ALERT=0");
        }
    }
#elif ACTIVE_EXERCISE == 4
    const unsigned long now = millis();
    const bool rawButtonState = digitalRead(BUTTON_PIN);

    if (rawButtonState != lastRawButtonState) {
        lastRawButtonState = rawButtonState;
        lastButtonChangeAt = now;
    }

    if (rawButtonState != stableButtonState &&
        now - lastButtonChangeAt >= DEBOUNCE_MS) {
        stableButtonState = rawButtonState;
        if (stableButtonState == HIGH) {
            pressCount = (pressCount + 1) % 5;
            showPressCount();
            Serial.print("count=");
            Serial.println(pressCount);
        }
    }
#endif
}