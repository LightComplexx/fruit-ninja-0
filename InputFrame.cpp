//
// InputFrame.cpp
//

// Game includes.
#include "InputFrame.h"

int InputFrame::getDirection() {
    return direction;
}

int InputFrame::getDirectionPressed() {
    return directionPressed;
}

int InputFrame::getButton() {
    return button;
}

int InputFrame::getButtonPressed() {
    return buttonPressed;
}

int InputFrame::getFrame() {
    return frame;
}

void InputFrame::setDirection(int direction) {
    this->direction = direction;
}

void InputFrame::setDirectionPressed(int directionPressed) {
    this->directionPressed = directionPressed;
}

void InputFrame::setButton(int button) {
    this->button = button;
}

void InputFrame::setButtonPressed(int buttonPressed) {
    this->buttonPressed = buttonPressed;
}

void InputFrame::setFrame(int frame) {
    this->frame = frame;
}