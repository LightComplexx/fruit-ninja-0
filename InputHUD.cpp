//
// InputHUD.cpp
//

// System includes.
#include <stdlib.h>  // for rand()

// Engine includes.
#include "EventStep.h"
#include "EventView.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"

// Game includes.
#include "InputHUD.h"
#include "InputHandler.h"
#include "Points.h"
#include "util.h"

InputHUD::InputHUD() {
	setType(INPUTHUD_STRING);
	setSolidness(df::SPECTRAL);
	setAltitude(df::MAX_ALTITUDE);

	//// Pick random kudos to show.
	//df::Animation a = getAnimation();
	//const df::Sprite* p_sprite = getAnimation().getSprite();
	//if (p_sprite) {
	//    int count = p_sprite->getFrameCount();
	//    int kudos = rand() % count;
	//    a.setIndex(kudos);
	//}
	//a.setSlowdownCount(-1); // Not animated.
	//setAnimation(a);

	// Pick random location.
	df::Vector p(WM.getBoundary().getHorizontal() / 8 +
		rand() % (3 * (int)WM.getBoundary().getHorizontal() / 4),
		WM.getBoundary().getVertical() / 8 +
		rand() % (3 * (int)WM.getBoundary().getVertical() / 4));
	setPosition(p);

	registerInterest(df::STEP_EVENT);

	input_step = 0;

	step_frame = 0;

	frame_counter = 0;

	step_timeout = 44;

	input_visual = new df::TextBox();
	input_visual->setPosition(df::Vector(WM.getBoundary().getHorizontal() / 2, WM.getBoundary().getVertical() - 5));
	input_visual->setViewString("input_visual");
	input_visual->setSize(df::Vector(28, 1));
}

// Handle event.
// Return 0 if ignored, else 1.
int InputHUD::eventHandler(const df::Event* p_e) {

	// Step event.
	if (p_e->getType() == df::STEP_EVENT)
		return step((df::EventStep*)p_e);

	// If get here, have ignored this event.
	return 0;
}

// Handle step event.
int InputHUD::step(const df::EventStep* p_e) {
	// Check Input Progress each frame
	updateInputProgress();

	// Handled.
	return 1;
}

void InputHUD::updateInputProgress() {
	// Pull current InputFrame from inputBuffer
	std::vector<InputFrame*> inputBuffer;
	std::string input_progress = "";

	df::ObjectList ol = WM.objectsOfType(INPUTHANDLER_STRING);
	if (ol.getCount() > 0 && (dynamic_cast <InputHandler*> (ol[0]))) {
		InputHandler* p_h = dynamic_cast <InputHandler*> (ol[0]);
		inputBuffer = p_h->getInputBuffer();
		frame_counter = p_h->getFrameCounter();
	}


	InputFrame* f = inputBuffer.front();

	// Reset on input timeout
	if (frame_counter - step_frame > step_timeout) {
		input_progress = "help";
	    resetInputProgress();
	    return;
	}

	switch (input_step) {

	case INPUT_F:
		if ((f->getDirectionPressed() & FORWARD) != 0) {
			input_progress = "->" + std::to_string(frame_counter);
			advanceStep();
		}
		break;

	case INPUT_N:
		if (f->getDirectionPressed() != 0 &&
			!isValidForStep(f)) {
			input_progress = "-> | *";
			advanceStep();
		}
		break;

	case INPUT_D:
		if (f->getDirection() == DOWN) {
			input_progress = "-> | * | V";
			advanceStep();
		}
		break;

	case INPUT_DF:
		if (f->getDirection() == DOWN_FORWARD) {
			if ((f->getButtonPressed() & BTN_2) != 0) {
				input_progress = "-> | * | V | '-> + 2";
				resetInputProgress();
			}
		}
		else if ((f->getDirectionPressed() & DOWN_FORWARD) != 0) {
			input_progress = "-> | * | V | '->";
			advanceStep();
		}
		break;

	case INPUT_2:
		if (f->getDirection() == DOWN_FORWARD && (f->getButtonPressed() & BTN_2) != 0) {
			input_progress = "-> | * | V | '-> | 2";
			resetInputProgress();
		}
		break;

	default:
		input_progress = "";
		resetInputProgress();
	}

	// Wrong input  reset
	if (isValidForStep(f)) {
		resetInputProgress();
	}

	DM.drawString(input_visual->getPosition(),
		input_progress,
		df::CENTER_JUSTIFIED,
		df::YELLOW);
}

void InputHUD::advanceStep() {
	input_step++;
	step_frame = frame_counter;
}

void InputHUD::resetInputProgress() {
	input_step = 0;
	step_frame = frame_counter;
	input_visual->setText("");
}

bool InputHUD::isValidForStep(InputFrame* f) {
	switch (input_step) {
	case INPUT_F:
		return f->getDirection() != FORWARD && f->getDirection() != 0;

	case INPUT_N:
		return f->getDirection() != 0;

	case INPUT_D:
		return f->getDirection() != DOWN;

	case INPUT_DF:
		return f->getDirection() != DOWN_FORWARD;
	}
	return false;
}

//void InputHUD::updateIcons() {
//    for (int i = 0; i < icons.length; i++) {
//        icons[i].active = (i < ewfgStep);
//        icons[i].scale = icons[i].active ? 1.2 : 1.0;
//    }
//}
//
//void InputHUD::drawIcons(float x, float y) {
//    for (int i = 0; i < icons.length; i++) {
//        pushMatrix();
//        translate(x + i * 64, y);
//        scale(icons[i].scale);
//        tint(icons[i].active ? color(255, 255, 0) : color(255));
//        image(icons[i].img, 0, 0);
//        popMatrix();
//    }
//}