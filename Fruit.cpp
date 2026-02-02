//
// Fruit.cpp
//

// System includes
#include <fstream>
#include <iostream>
#include <sstream>

// Engine includes.
#include "EventCollision.h"
#include "EventOut.h"
#include "EventView.h"
#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"

// Game includes.
#include "game.h"
#include "Fruit.h"
#include "Points.h"
#include "Sword.h"
#include "SlashAttack.h"
#include "util.h"

// Constructor - supply name of Fruit (matches Sprite).
Fruit::Fruit(std::string name, bool isBonusStage) {

	setType(name);
	if (setSprite(name) != 0)
		LM.writeLog("Fruit::Fruit(): Error! Unable to find sprite: %s",
			name.c_str());
	m_first_out = true; // To ignore first out of bounds (when spawning).
	is_bonus_stage = isBonusStage;
	setSolidness(df::SOFT);
}

// Handle event.
int Fruit::eventHandler(const df::Event* p_e) {

	// Out of bounds event.
	if (p_e->getType() == df::OUT_EVENT)
		return out((df::EventOut*)p_e);

	// Collision event.
	if (p_e->getType() == df::COLLISION_EVENT)
		return collide((df::EventCollision*)p_e);

	// Not handled.
	return 0;
}

// Handle out events.
int Fruit::out(const df::EventOut* p_e) {

	if (m_first_out) { // Ignore first out (when spawning).
		m_first_out = false;
		return 1;
	}

	int points = (is_bonus_stage ? -100 : -25);

	// Each out is a "miss", so lose points.
	df::EventView ev(POINTS_STRING, points, true);
	WM.onEvent(&ev);

	// Destroy this Fruit.
	WM.markForDelete(this);

	// Handled.
	return 1;
}

// Handle collision events.
int Fruit::collide(const df::EventCollision* p_e) {

	// Sword collision means ninja sliced this Fruit.
	if (p_e->getObject1()->getType() == SWORD_STRING && !is_bonus_stage) {

		// Add points.
		df::EventView ev(POINTS_STRING, +10, true);
		WM.onEvent(&ev);

		// Destroy this Fruit.
		WM.markForDelete(this);
	}

	// Ninja slash collides with fruit
	if (p_e->getObject2()->getType() == SLASHATTACK_STRING) {
		SlashAttack* slash = dynamic_cast <SlashAttack*> (p_e->getObject2());
		int points = (slash->getIsElectric() ? 1000 : 100);

		if (points == 1000) {
			// Shake screen (severity 5 pixels x&y, duration 5 frames).
			DM.shake(5, 5, 5);
		}

		// Add points.
		df::EventView ev(POINTS_STRING, +points, true);
		WM.onEvent(&ev);

		// Destroy this Fruit.
		WM.markForDelete(this);
	}

	// Handled.
	return 1;
}

// Destructor.
Fruit::~Fruit() {

	// If inside the game world and engine not shutting down,
	// create explosion and play sound.
	if (df::boxContainsPosition(WM.getBoundary(), getPosition()) &&
		GM.getGameOver() == false) {
		df::explode(getAnimation().getSprite(), getAnimation().getIndex(), getPosition(),
			EXPLOSION_AGE, EXPLOSION_SPEED, EXPLOSION_ROTATE);

		// Play "splat" sound.
		std::string sound = "splat-" + std::to_string(rand() % 6 + 1);
		play_sound(sound);
	}
}

// Setup starting conditions.
void Fruit::start(float speed) {

	df::Vector begin, end;

	// Get world boundaries.
	int world_x = (int)WM.getBoundary().getHorizontal();
	int world_y = (int)WM.getBoundary().getVertical();
	df::Vector world_center(world_x / 2.0f, world_y / 2.0f);

	// Different spawn location for regular
	// and bonus mode
	int spawn;

	// Extra random variables for non-bonus stage
	float hor_y_position_1 = (float)(rand() % world_y);
	float hor_y_position_2 = (float)(rand() % world_y);

	if (is_bonus_stage) {
		spawn = 1;
		hor_y_position_1 = world_center.getY();
		hor_y_position_2 = world_center.getY();
	}
	else { 
		spawn = rand() % 4; 
	}

	// Pick random side (Top, Right, Bottom, Left) to spawn.
	switch (spawn) {

	case 0: // Left.
		begin.setX(0 - 3.0f);
		begin.setY(hor_y_position_1);
		end.setX(world_x + 3.0f);
		end.setY(hor_y_position_2);
		break;

	case 1: // Right.
		begin.setX(world_x + 3.0f);
		begin.setY(hor_y_position_1);
		end.setX(0 - 3.0f);
		end.setY(hor_y_position_2);
		break;

	case 2: // Bottom.
		begin.setX((float)(rand() % world_x));
		begin.setY(world_y + 3.0f);
		end.setX((float)(rand() % world_x));
		end.setY(0 - 3.0f);
		break;

	case 3: // Top.
		begin.setX((float)(rand() % world_x));
		begin.setY(0 - 3.0f);
		end.setX((float)(rand() % world_x));
		end.setY(world_y + 3.0f);
		break;

	default:
		break;
	}

	// Move Object into position.
	WM.moveObject(this, begin);

	// Set velocity towards opposite side.
	df::Vector velocity = end - begin;
	velocity.normalize();
	setDirection(velocity);
	setSpeed(speed);
}

int Fruit::serialize(std::stringstream* p_ss, unsigned int attr) {
	// Write length of string for Object Type
	int str_len = this->getType().length();
	p_ss->write(std::to_string(str_len).c_str(), sizeof(str_len));

	// Write Object Type
	p_ss->write(this->getType().c_str(), str_len);

	return df::Object::serialize(p_ss, attr);
}