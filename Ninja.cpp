//
// Ninja.cpp
//

// System includes
#include <stdlib.h> // For rand()
#include <vector.h> // For std::vector

// Engine includes
#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"
#include "ResourceManager.h"
#include "EventStep.h"

// Game includes
#include "Ninja.h"
#include "Kudos.h"
#include "Sword.h"
#include "InputHandler.h"
#include "InputFrame.h"
#include "SlashAttack.h"

Ninja::Ninja() {
	// Sets object type to Ninja
	setType(NINJA_STRING);

	// Link to "ninja_idle" sprite
	setSprite(NINJA_IDLE);

	// Registers keyboard, mouse, step events 
	registerInterest(df::STEP_EVENT);

	// Sets position to the middle of the window
	df::Vector p(5, WM.getBoundary().getVertical() / 2);
	setPosition(p);

	setSolidness(df::SOFT);

	// Initiliaze DOWN and DOWN_FORWARD
	FORWARD = RIGHT;
	DOWN_FORWARD = DOWN | FORWARD;

	is_electric_slash = false;
	slash_spawned = false;

	setAltitude(0);   // foreground
	setVisible(true);

	time_to_animate = 0;
	current_animation_time = 0;
	slash_countdown = 0;

	state = IDLE;
}

Ninja::~Ninja() {
	// Destroys Ninja and ends game

	// Shake screen (severity 20 pixels x&y, duration 10 frames).
	DM.shake(20, 20, 10);

	// Make a big explosion with particles.
	df::addParticles(df::SPARKS, getPosition(), 2, df::BLUE);
	df::addParticles(df::SPARKS, getPosition(), 2, df::YELLOW);
	df::addParticles(df::SPARKS, getPosition(), 3, df::RED);
	df::addParticles(df::SPARKS, getPosition(), 3, df::RED);
}

// Records keyboard and step events
int Ninja::eventHandler(const df::Event* p_e) {
	// Step events
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	return 0;
}

// Decrease rate restriction counters
void Ninja::step() {
	// Checks every step for inputs
	checkForInputs();

	// Decrease animation time
	if (current_animation_time > 0)
		current_animation_time--;

	// Spawn attack exactly once at animation end
	if ((state == SLASH || state == ELECTRIC_SLASH) &&
		!slash_spawned &&
		animationFinished()) {

		spawnSlashAttack();
		slash_spawned = true;
	}

	// Return to idle after recovery
	if (slash_countdown > 0)
		slash_countdown--;
	else if (state != IDLE) {
		changeAnimationState(IDLE);
	}
}

int Ninja::draw() {
	return df::Object::draw();
}

void Ninja::checkForInputs() {
	// Variables to be pulled from input handler
	std::vector<InputFrame*> inputBuffer;
	int currentFrame = -1;

	df::ObjectList ol = WM.objectsOfType(INPUTHANDLER_STRING);
	if (ol.getCount() > 0 && (dynamic_cast <InputHandler*> (ol[0]))) {
		InputHandler* p_h = dynamic_cast <InputHandler*> (ol[0]);
		inputBuffer = p_h->getInputBuffer();
		currentFrame = p_h->getFrameCounter();
	}

	// Counts each step in the input
	int step = 0;

	// Frame to start input check from
	int startFrame = -1;

	// Determines if down-forward and 2 were
	// pressed on the same frame
	bool sameFrame = false;

	for (int i = 0; i < inputBuffer.size(); i++) {
		InputFrame* f = inputBuffer.at(i);

		switch (step) {
			// Step 0: DF + 2 (if same frame, skip to down check)
		case 0:
			if ((f->getButtonPressed() & BTN_2) != 0 && f->getFrame() == currentFrame) {
				startFrame = f->getFrame();
				if (f->getDirection() == DOWN_FORWARD && startFrame - f->getFrame() == 0) {
					sameFrame = true;
					step = 2;
					break;
				}
				else step = 1;
			}
			break;

			// Step 1: [ If not electric, check DF ]
		case 1:
			if ((f->getDirectionPressed() & DOWN_FORWARD) != 0 && startFrame - f->getFrame() <= 20) {
				step = 2;
			}
			break;

			// Step 2: down
		case 2:
			// If this input is down, continue
			if (f->getDirection() == DOWN) {
				step = 3;
				break;
			}
			else if (f->getDirection() != DOWN && sameFrame) {
				// If you triggered an electric, check previous 
				// input for false positive (pressed DF+2 too late)
				InputFrame* prev_f;
				if (i > 0) {
					prev_f = inputBuffer.at(i - 1);

					// If false positive, shift to wind god fist check
					if (prev_f->getDirection() == DOWN_FORWARD) {
						sameFrame = false;
						break;
					}
				}
			}
			else step = 0;
			break;

			// Step 3: neutral
		case 3:
			if (f->getDirection() == 0 && startFrame - f->getFrame() <= 44) {
				step = 4;
			}
			else step = 0;
			break;

			// Step 4: forward
		case 4:
			if ((f->getDirectionPressed() & FORWARD) != 0) {
				// Timing check
				if (startFrame - f->getFrame() <= 26 && sameFrame) {
					is_electric_slash = true;
				}
				else {
					is_electric_slash = false;
				}
				startSlash(is_electric_slash);
				return;
			}
			break;
		}
	}
}

void Ninja::startSlash(bool is_electric) {
	changeAnimationState(is_electric ? ELECTRIC_SLASH : SLASH);
	slash_countdown = time_to_animate;
}

void Ninja::changeAnimationState(AnimationState new_state) {
	if (state == new_state)
		return;

	state = new_state;
	slash_spawned = false;

	switch (state) {
	case IDLE:
		setSprite(NINJA_IDLE);
		break;

	case SLASH:
		setSprite(NINJA_SLASH);
		break;

	case ELECTRIC_SLASH:
		setSprite(NINJA_SLASH_ELECTRIC);
		break;
	}

	getAnimation().setIndex(0);

	time_to_animate =
		getAnimation().getSprite()->getFrameCount() *
		getAnimation().getSprite()->getSlowdown();

	current_animation_time = time_to_animate;
}

bool Ninja::animationFinished() {
	return getAnimation().getIndex() ==
		getAnimation().getSprite()->getFrameCount() - 1;
}

void Ninja::spawnSlashAttack() {
	// Spawn Electric kudos if electric
	if (is_electric_slash) {
		new Kudos(true);
	}

	new SlashAttack(
		df::Vector(getPosition().getX() + 10.0f, getPosition().getY()),
		is_electric_slash
	);
}