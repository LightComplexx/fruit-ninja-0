//
// InputHUD.h
//

#ifndef INPUTHUD_H
#define INPUTHUD_H

// Engine includes.
#include "Color.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventStep.h"
#include "Object.h"

// Game includes
#include "InputFrame.h"
#include "InputHandler.h"

const std::string INPUTHUD_STRING = "InputHUD";

const int INPUT_F = 0;		// Forward
const int INPUT_N = 1;		// Neutral
const int INPUT_D = 2;		// Down
const int INPUT_DF = 3;		// Down Forward
const int INPUT_2 = 4;		// Slash

// Inputs that matches Ninjas
const int FORWARD = 1 << 1;
const int DOWN_FORWARD = (1 << 3) | FORWARD;

class InputHUD : public df::Object {

private:
	// Current step of the input
	int input_step;

	// Frame when current step was reached
	int step_frame;

	// Frames allowed per step
	int step_timeout;

	// Frame counter
	int frame_counter;

	// Handle step event.
	int step(const df::EventStep* p_e);

	// Handle mouse event.
	int mouse(const df::EventMouse* p_e);

	// Handle keyboard event.
	int keyboard(const df::EventKeyboard* p_e);

	// Updates current progress in the input
	void updateInputProgress();

	// Advances step in input progress
	void advanceStep();

	// Resets input progress
	void resetInputProgress();

	// True if current input matches current step in input
	bool isValidForStep(InputFrame* f);

	// temp input display
	df::TextBox* input_visual;

public:

	// Constructor.
	InputHUD();

	// Handle events.
	int eventHandler(const df::Event* p_e) override;

	// Draw.
	//int draw(void) override;
};

#endif // INPUTHUD_H
