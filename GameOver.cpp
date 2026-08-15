//
// GameOver.cpp
//

// Engine includes.
#include "DisplayManager.h"
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "GameOver.h"
#include "Points.h"
#include "Bonus.h"
#include "util.h"

GameOver::GameOver(bool isBonusStage) {

	setType(GAMEOVER_STRING);
	setDrawValue(false);

	// Animate "game over" sprite one time.
	if (setSprite("gameover") == 0)
		m_time_to_live = getAnimation().getSprite()->getFrameCount() *
		getAnimation().getSprite()->getSlowdown();
	else
		m_time_to_live = 0;

	// Put in center of window.
	setLocation(df::CENTER_CENTER);

	// Register for step event.
	registerInterest(df::STEP_EVENT);

	// Shake screen (severity 20 pixels x&y, duration 10 frames).
	DM.shake(20, 20, 10);

	LM.writeLog("GameOver::GameOver(): created");

	is_bonus_stage = isBonusStage;
}

// Handle event.
int GameOver::eventHandler(const df::Event* p_e) {

	// Step event.
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

// Count down to end of G-A-M-E O-V-E-R message.
int GameOver::step() {

	m_time_to_live--;

	if (m_time_to_live <= 0) {
		WM.markForDelete(this);

		// Check points amount for bonus eligibility
		df::ObjectList ol = WM.objectsOfType(POINTS_STRING);
		if (ol.getCount() > 0 && (dynamic_cast <Points*> (ol[0]))) {
			Points* p_p = dynamic_cast <Points*> (ol[0]);
			if (p_p->getValue() >= 1000 && !is_bonus_stage) {
				// Create new "Bonus" object and draw text
				Bonus* bonus_text = new Bonus();
				bonus_text->draw();
			}
			else {
				GM.setGameOver(true);
			}
		}
	}

	if (m_time_to_live == 175)
		play_sound("game-over");

	// Handled.
	return 1;
}

int GameOver::draw() {
	return Object::draw();
}
