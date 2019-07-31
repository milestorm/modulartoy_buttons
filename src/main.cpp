#include <Arduino.h>
#include <OneButton.h>
#include "config.h"
#include "tools.h"
#include "flasher.h"

OneButton myButtons[] = {OneButton(BUTT_RED, true), OneButton(BUTT_GREEN, true), OneButton(BUTT_BLUE, true), OneButton(BUTT_YELLOW, true)};
Flasher myLeds[] = {Flasher(LED_RED, 300, 300), Flasher(LED_GREEN, 300, 300), Flasher(LED_BLUE, 300, 300), Flasher(LED_YELLOW, 300, 300)};

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

void setup() {
	Serial.begin(9600);

	for (int i = 0; i < 4; i++){
		myButtons[i].setPressTicks(60);
	}

	myButtons[0].attachPress(butt0Click);
	myButtons[1].attachPress(butt1Click);
	myButtons[2].attachPress(butt2Click);
	myButtons[3].attachPress(butt3Click);

	activeLed = turnOnRandomLed();
}

void loop() {
	for (int i = 0; i < 4; i++){
		myButtons[i].tick();
		myLeds[i].tick();
	}
}