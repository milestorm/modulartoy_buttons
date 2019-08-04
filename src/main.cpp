#include <Arduino.h>
#include <OneButton.h>
#include "config.h"
#include "tools.h"
#include "flasher.h"
#include <Adafruit_NeoPixel.h>

// BUTTON GAME START

OneButton myButtons[] = {OneButton(BUTT_RED, true), OneButton(BUTT_GREEN, true), OneButton(BUTT_BLUE, true), OneButton(BUTT_YELLOW, true)};
Flasher myLeds[] = {Flasher(BUTT_LED_RED, 300, 300), Flasher(BUTT_LED_GREEN, 300, 300), Flasher(BUTT_LED_BLUE, 300, 300), Flasher(BUTT_LED_YELLOW, 300, 300)};

int activeLed = -1;

void turnOffAllLeds() {
	for (int i = 0; i < 4; i++) {
		myLeds[i].off();
	}
}

int turnOnRandomLed() {
	int rndNumber;

	// makes the next led different than the one, that is lit.
	do {
		rndNumber = randomGenerator(0,3);
	} while (rndNumber == activeLed);

	myLeds[rndNumber].permanentOn();
	return rndNumber;
}

void processPush(int buttonId) {
	if (myLeds[buttonId].isPermanentOn()) {
		turnOffAllLeds();
		activeLed = turnOnRandomLed();
	} else {
		myLeds[activeLed].flashingOn();
	}
	
}

void butt0Click() {
	processPush(0);
}

void butt1Click() {
	processPush(1);
}

void butt2Click() {
	processPush(2);
}

void butt3Click() {
	processPush(3);
}

// BUTTON GAME END

// RGB MIXER START
int rgbMixerPots[3] = {RGB_MIXER_POT_RED, RGB_MIXER_POT_GREEN, RGB_MIXER_POT_BLUE};
int rgbMixerPotPosition = RGB_MIXER_POT_WHITE;
int rgbMixerLedValues[3] = {0, 0, 0};
int rgbMixerPotPositionValue = 0;
bool updateStrip = false; // if values read are different from stored, then update

Adafruit_NeoPixel rgbStrip(RGB_MIXER_LED_COUNT, RGB_MIXER_LED_DATA, NEO_GRB + NEO_KHZ800);

void debugPixels() {
	Serial.print(rgbMixerLedValues[0]);
	Serial.print(", ");
	Serial.print(rgbMixerLedValues[1]);
	Serial.print(", ");
	Serial.print(rgbMixerLedValues[2]);
	Serial.print(" | position: ");
	Serial.println(rgbMixerPotPositionValue);
}

bool readPotValues() {
	int actualLedValues[3] = {0, 0, 0};
	int actualPosValue = 0;
	for (int i = 0; i < 3; i++) {
		actualLedValues[i] = analogRead(rgbMixerPots[i]) / 4;
		actualPosValue = analogRead(rgbMixerPotPosition) / 128;
	}

	// if values changed, than change them
	if ( actualPosValue != rgbMixerPotPositionValue || actualLedValues[0] != rgbMixerLedValues[0] || actualLedValues[1] != rgbMixerLedValues[1] || actualLedValues[2] != rgbMixerLedValues[2] ) {
		for (int i = 0; i < 3; i++) {
			rgbMixerLedValues[i] = actualLedValues[i];
			rgbMixerPotPositionValue = actualPosValue;
		}
		debugPixels();
		return true;
	} else {
		return false;
	}
	
}

void rgbMixerTick() {
	updateStrip = readPotValues();

	if (updateStrip == true) {
		rgbStrip.setPixelColor(rgbMixerPotPositionValue, rgbStrip.Color(rgbMixerLedValues[0], rgbMixerLedValues[1], rgbMixerLedValues[2]));
		rgbStrip.show();
	}
}

// RGB MIXER END

void setup() {
	Serial.begin(9600);

	// Button game
	for (int i = 0; i < 4; i++){
		myButtons[i].setPressTicks(60);
	}

	myButtons[0].attachPress(butt0Click);
	myButtons[1].attachPress(butt1Click);
	myButtons[2].attachPress(butt2Click);
	myButtons[3].attachPress(butt3Click);

	activeLed = turnOnRandomLed();

	// RGB mixer
	pinMode(RGB_MIXER_POT_RED, INPUT);
	pinMode(RGB_MIXER_POT_GREEN, INPUT);
	pinMode(RGB_MIXER_POT_BLUE, INPUT);
	pinMode(RGB_MIXER_POT_WHITE, INPUT);

	rgbStrip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	rgbStrip.show();            // Turn OFF all pixels ASAP
	rgbStrip.setBrightness(RGB_MIXER_LED_BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
	// Button game
	for (int i = 0; i < 4; i++){
		myButtons[i].tick();
		myLeds[i].tick();
	}

	// RGB mixer
	rgbMixerTick();
}