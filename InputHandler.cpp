//
// InputHandler.cpp
//

// System includes.
#include <stdlib.h>    // for rand()
#include <vector.h>    // for input stack
#include <fstream>
#include <iostream>
#include <sstream>

// Engine includes.
#include "EventStep.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"

// Game includes.
#include "InputHandler.h"
#include "InputFrame.h"
#include "InputDisplayEntry.h"
#include "Fruit.h"
#include "game.h"

InputHandler::InputHandler(bool isBonusStage) {
	setType(INPUTHANDLER_STRING);
	setSolidness(df::SPECTRAL);
	setVisible(false);
	registerInterest(df::STEP_EVENT);
	registerInterest(df::KEYBOARD_EVENT);
	registerInterest(df::MSE_EVENT);

	directionState = 0;
	prevDirectionState = 0;

	buttonState = 0;
	prevButtonState = 0;

	buttonPressed = 0;
	directionPressed = 0;

	frameCounter = 0;

	is_bonus_stage = isBonusStage;

	ss = {};
}

// Handle event.
// Return 0 if ignored, else 1.
int InputHandler::eventHandler(const df::Event* p_e) {

	// Step event.
	if (p_e->getType() == df::STEP_EVENT)
		return step((df::EventStep*)p_e);

	// Keyboard event.
	if (p_e->getType() == df::KEYBOARD_EVENT)
		return keyboard((df::EventKeyboard*)p_e);

	// If get here, have ignored this event.
	return 0;
}

// Handle step event.
int InputHandler::step(const df::EventStep* p_e) {
	// Update frame count
	frameCounter++;

	// Update direction and button edges
	updateEdges();

	// Record inputs
	recordInput();

	if (is_bonus_stage) {
		// Display input stack
		drawDisplayStack();
	}

	return 1;
}

int InputHandler::keyboard(const df::EventKeyboard* p_e) {
	// On Key Pressed
	if (p_e->getKeyboardAction() == df::KEY_PRESSED) {
		// Add input to direction/button states
		keyPressed(p_e->getKey());

		// Serialize
		if (p_e->getKey() == df::Keyboard::Z) {
			// Create Stream --------------------------------------------
			// Get all remaining Fruit objects and serialize
			df::ObjectList ol_f = WM.solidObjects();
			for (int i = 0; i < ol_f.getCount(); i++)
				if (dynamic_cast <Fruit*>(ol_f[i]))
					ol_f[i]->serialize(&ss);

			// Check if serializes properly
			if (ss.good())
				LM.writeLog("InputHandler::keyboard(): Fruits serialized to memory.");
			else
				LM.writeLog("InputHandler::keyboard(): Could not serialize to memory.");

			// Open File --------------------------------------------
			std::ofstream outFile(FILE_NAME, std::ios::binary);

			if (!outFile.is_open()) {
				LM.writeLog("InputHandler::keyboard(): Error!  Can't open file for writing.");
			}
			else {
				// Write Stream --------------------------------------------
				std::stringbuf* pbuf = ss.rdbuf();  // Get pointer to stringbuf.
				std::string str = pbuf->str();
				outFile.write(str.c_str(), str.size());

				// Close file --------------------------------------------
				outFile.close();
				ss.clear(); // clear stream
			}
		}

		//// Deserialize
		if (p_e->getKey() == df::Keyboard::X) {
			// Destroy all fruits on screen
			df::ObjectList ol_f = WM.solidObjects();
			for (int i = 0; i < ol_f.getCount(); i++)
				if (dynamic_cast <Fruit*>(ol_f[i]))
					WM.markForDelete(ol_f[i]);

			// Open file for reading.
			std::ifstream inFile(FILE_NAME, std::ios::binary);
			if (!inFile.is_open()) {
				LM.writeLog("InputHandler::keyboard(): Error!  Can't open file for writing.");
				return 1;
			}

			// Read entire file into stream.
			ss << inFile.rdbuf();
			if (!inFile) {
				LM.writeLog("InputHandler::keyboard(): Error!  During reading.");
				return 1;
			}

			// Close file.
			inFile.close();

			// Extract name from stream
			int str_len;
			ss.read(reinterpret_cast<char*>(&str_len), sizeof(str_len));

			std::string object_type;
			ss.read(reinterpret_cast<char*>(&object_type), str_len);

			// Create objects and deserialize
			if (object_type == "grapes" || object_type == "pear" || object_type == "apple" ||
				object_type == "banana" || object_type == "blueberries") {
				Fruit* p_f = new Fruit(object_type, is_bonus_stage);
				p_f->deserialize(&ss);
			}

		}
	}

	// On Key Released
	if (p_e->getKeyboardAction() == df::KEY_RELEASED) {
		// Remove input from direction/button states
		return keyReleased(p_e->getKey());;
	}

	// No key handled
	return 0;
}

void InputHandler::updateEdges() {
	// Update direction edges
	directionPressed = directionState & ~prevDirectionState;
	prevDirectionState = directionState;

	// Update button edges
	buttonPressed = buttonState & ~prevButtonState;
	prevButtonState = buttonState;
}

void InputHandler::recordInput() {
	// Create new InputFrame object
	InputFrame* f = new InputFrame();

	// Set all values based on current input
	f->setDirection(directionState);
	f->setDirectionPressed(directionPressed);
	f->setButton(buttonState);
	f->setButtonPressed(buttonPressed);
	f->setFrame(frameCounter);

	// Only record input if buffer is empty,
	// or a new button has been pressed
	if (inputBuffer.empty() ||
		inputBuffer.front()->getDirection() != f->getDirection() ||
		inputBuffer.front()->getButton() != f->getButton()) {
		// Insert input
		inputBuffer.insert(inputBuffer.begin(), f);
	}

	// Cap buffer size
	if (inputBuffer.size() > 30) {
		inputBuffer.pop_back();
	}

	// Pass input to display stack for updating
	updateDisplayStack(f);
}

void InputHandler::updateDisplayStack(InputFrame* current) {
	// If no existing entries, add new entry
	if (displayStack.empty()) {
		displayStack.push_back(newEntry(current));
		return;
	}

	// Create new input display entry
	InputDisplayEntry* top = displayStack.front();

	// If same input as previous frame, extend hold
	if (top->getDirection() == current->getDirection() && top->getButton() == current->getButton()) {
		if (top->getFramesHeld() == 999)
			return;
		top->setFramesHeld(top->getFramesHeld() + 1);
		return;
	}

	// If input changed, push new entry to top of stack
	displayStack.insert(displayStack.begin(), newEntry(current));

	// Limit stack size
	if (displayStack.size() > 15) {
		displayStack.pop_back();
	}
}

InputDisplayEntry* InputHandler::newEntry(InputFrame* f) {
	InputDisplayEntry* e = new InputDisplayEntry();
	e->setDirection(f->getDirection());
	e->setButton(f->getButton());
	e->setFramesHeld(1);
	return e;
}

void InputHandler::drawDisplayStack() {
	// Loop through input display stack
	for (int i = 0; i < displayStack.size(); i++) {
		// Create new input display entry
		InputDisplayEntry* e = displayStack.at(i);

		std::string line = directionToString(e->getDirection()) +
			" " +
			buttonToString(e->getButton());

		if (e->getFramesHeld() > 0) {
			line += "    [ " + std::to_string(e->getFramesHeld()) + "]";
		}

		DM.drawString(df::Vector(WM.getBoundary().getHorizontal() - 10.0f, 4.0f + (i * 1.0f)),
			line,
			df::CENTER_JUSTIFIED,
			df::YELLOW);
	}
}

std::string InputHandler::directionToString(int direction) {
	if (direction == 0) return "";

	if (direction == (DOWN | LEFT))  return "<-'";
	if (direction == (DOWN | RIGHT)) return "'->";
	if (direction == DOWN)           return "V";
	if (direction == LEFT)           return "<";
	if (direction == RIGHT)          return ">";
	if (direction == UP)             return "^^";

	return "";
}

std::string InputHandler::buttonToString(int button) {
	std::string s = "";

	if ((button & BTN_1) != 0) s += "1";
	if ((button & BTN_2) != 0) s += "2";
	if ((button & BTN_3) != 0) s += "3";
	if ((button & BTN_4) != 0) s += "4";

	return s;
}

std::vector<InputFrame*> InputHandler::getInputBuffer() {
	return inputBuffer;
}

std::vector<InputDisplayEntry*> InputHandler::getDisplayStack() {
	return displayStack;
}

int InputHandler::getFrameCounter() {
	return frameCounter;
}

int InputHandler::keyPressed(df::Keyboard::Key key) {
	switch (key) {
		// Directions
	case df::Keyboard::A:
		directionState |= LEFT;
		break;
	case df::Keyboard::D:
		directionState |= RIGHT;
		break;
	case df::Keyboard::W:
		directionState |= UP;
		break;
	case df::Keyboard::S:
		directionState |= DOWN;
		break;

		// Buttons
	case df::Keyboard::U:
		buttonState |= BTN_1;
		break;
	case df::Keyboard::I:
		buttonState |= BTN_2;
		break;
	case df::Keyboard::J:
		buttonState |= BTN_3;
		break;
	case df::Keyboard::K:
		buttonState |= BTN_4;
		break;
	}

	return 1;
}

int InputHandler::keyReleased(df::Keyboard::Key key) {
	switch (key) {
	case df::Keyboard::A:
		directionState &= ~LEFT;
		break;
	case df::Keyboard::D:
		directionState &= ~RIGHT;
		break;
	case df::Keyboard::W:
		directionState &= ~UP;
		break;
	case df::Keyboard::S:
		directionState &= ~DOWN;
		break;

		// Buttons
	case df::Keyboard::U:
		buttonState &= ~BTN_1;
		break;
	case df::Keyboard::I:
		buttonState &= ~BTN_2;
		break;
	case df::Keyboard::J:
		buttonState &= ~BTN_3;
		break;
	case df::Keyboard::K:
		buttonState &= ~BTN_4;
		break;
	}

	return 1;
}