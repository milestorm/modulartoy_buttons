#include <Arduino.h>
#include <OneButton.h>
#include "config.h"
#include "tools.h"
#include "flasher.h"

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
int rgbMixerLeds[4] = {RGB_MIXER_LED_RED, RGB_MIXER_LED_GREEN, RGB_MIXER_LED_BLUE, RGB_MIXER_LED_WHITE};
int rgbMixerPots[4] = {RGB_MIXER_POT_RED, RGB_MIXER_POT_GREEN, RGB_MIXER_POT_BLUE, RGB_MIXER_POT_WHITE};
int rgbMixerLedValues[4] = {0, 0, 0, 0};

void rgbMixerTick() {
	for (int i = 0; i < 4; i++) {
		rgbMixerLedValues[i] = analogRead(rgbMixerPots[i]);
		analogWrite(rgbMixerLeds[i], rgbMixerLedValues[i]/4);
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
	pinMode(RGB_MIXER_LED_RED, OUTPUT);
	pinMode(RGB_MIXER_LED_GREEN, OUTPUT);
	pinMode(RGB_MIXER_LED_BLUE, OUTPUT);
	pinMode(RGB_MIXER_LED_WHITE, OUTPUT);

	//pinMode(RGB_MIXER_POT_RED, INPUT);
	//pinMode(RGB_MIXER_POT_GREEN, INPUT);
	//pinMode(RGB_MIXER_POT_BLUE, INPUT);
	//pinMode(RGB_MIXER_POT_WHITE, INPUT);
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