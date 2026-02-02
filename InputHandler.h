//
// InputHUD.h
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

// System includes
#include <vector>

// Engine includes.
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventStep.h"
#include "Object.h"
#include "InputFrame.h"
#include "InputDisplayEntry.h"
#include "TextBox.h"

const std::string INPUTHANDLER_STRING = "InputHandler";

const int LEFT = 1 << 0;	// 0001
const int RIGHT = 1 << 1;	// 0010
const int UP = 1 << 2;		// 0100
const int DOWN = 1 << 3;	// 1000

const int BTN_1 = 1 << 0;	// left punch
const int BTN_2 = 1 << 1;	// right punch
const int BTN_3 = 1 << 2;	// left kick
const int BTN_4 = 1 << 3;	// right kick

class InputHandler : public df::Object {

private:
	// A list of raw inputs
	std::vector<InputFrame*> inputBuffer;

	// A list of input display entries
	std::vector<InputDisplayEntry*> displayStack;

	// Tracks current direction state
	int directionState;

	// Tracks previous direction state
	int prevDirectionState;

	// Tracks directions pressed on current frame
	int directionPressed;

	// Tracks current button state
	int buttonState;

	// Tracks previous button state
	int prevButtonState;

	// Tracks buttons pressed on current frame
	int buttonPressed;

	// Frame counter (runs every step)
	int frameCounter;

	// True if player is in bonus stage
	bool is_bonus_stage;

	// Handle step event.
	int step(const df::EventStep* p_e);

	// Handle keyboard event.
	int keyboard(const df::EventKeyboard* p_e);

	// Updates the state of the direction/button
	// on the current and previous frame
	void updateEdges();

	// Updates the raw input stack
	void recordInput();

	// Updates the input display stack
	void updateDisplayStack(InputFrame* f);

	// Defines new input display entry
	InputDisplayEntry* newEntry(InputFrame* f);

	// Converts direction to string
	std::string directionToString(int direction);

	// Converts button to string
	std::string buttonToString(int button);

	// Updates inputs based on corresponding key pressed
	int keyPressed(df::Keyboard::Key key);

	// Updates inputs based on corresponding key released
	int keyReleased(df::Keyboard::Key key);

	// Input/Output stream
	std::stringstream ss;

public:
	// Constructor.
	InputHandler(bool isBonusStage = false);

	// Handle events.
	int eventHandler(const df::Event* p_e) override;

	// Returns raw input buffer stack
	std::vector<InputFrame*> getInputBuffer();

	// Returns input display stack
	std::vector<InputDisplayEntry*> getDisplayStack();

	// Returns frame counter
	int getFrameCounter();

	// Draws input display stack to screen
	void drawDisplayStack();
};

#endif // INPUTHANDLER_H
