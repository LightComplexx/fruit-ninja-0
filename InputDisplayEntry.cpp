//
// InputDisplayEntry.cpp
//

// Game includes.
#include "InputDisplayEntry.h"

int InputDisplayEntry::getDirection() {
    return direction;
}

int InputDisplayEntry::getButton() {
    return button;
}

int InputDisplayEntry::getFramesHeld() {
    return framesHeld;
}

void InputDisplayEntry::setDirection(int direction) {
    this->direction = direction;
}

void InputDisplayEntry::setButton(int button) {
    this->button = button;
}

void InputDisplayEntry::setFramesHeld(int framesHeld) {
    this->framesHeld = framesHeld;
}