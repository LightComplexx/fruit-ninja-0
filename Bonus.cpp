//
// Bonus.cpp
//

// Engine includes
#include "GameManager.h"
#include "WorldManager.h"
#include "EventStep.h"

// Game includes
#include "Bonus.h"
#include "EventBonus.h"
#include "Timer.h"
#include "Grocer.h"
#include "Ninja.h"
#include "Sword.h"
#include "InputHandler.h"
#include "game.h"

Bonus::Bonus() {
	// Sets object to type Slash
	setType("Bonus");

	// Sets sprite for 'Dimensional Slash' text
	if (setSprite("bonus_stage") == 0)
		time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown() * 4;
	else
		time_to_live = 0;

	// Makes the Bonus text spectral so it can pass through objects 
	// and not impede movement
	setSolidness(df::SPECTRAL);

	// Draws the Bonus text in the foreground
	setAltitude(df::MAX_ALTITUDE);

	// Sets location to top center of window
	setLocation(df::CENTER_CENTER);

	// Registers step event
	registerInterest(df::STEP_EVENT);
}

Bonus::~Bonus() {
	// Repopulate world in "bonus" mode
	new Grocer(true);
	new Timer(30);
	new InputHandler(true);
	new Ninja;

	// Create "Bonus" event and send to interested Objects
	EventBonus bonus;
	WM.onEvent(&bonus);
}

int Bonus::eventHandler(const df::Event* p_e) {
	// Life countdown
	if (p_e->getType() == df::STEP_EVENT) {
		time_to_live--;
		return 1;
	}

	return 0;
}

int Bonus::draw() {
	// Flash text until time delay ends
	if (time_to_live <= 0) {
		WM.markForDelete(this);
		return -1;
	}
	return df::Object::draw();
}
