//
// Ninja.h
//

#ifndef NINJA_H
#define NINJA_H

// Engine includes
#include "Object.h"

const std::string NINJA_STRING = "Ninja";
const std::string NINJA_IDLE = "ninja_idle";
const std::string NINJA_SLASH = "ninja_slash";
const std::string NINJA_SLASH_ELECTRIC = "ninja_slash_electric";

const std::string SWORD_SLASH = "sword_slash";
const std::string SWORD_SLASH_ELECTRIC = "sword_slash_electric";

enum AnimationState {
    IDLE,
    SLASH,
    ELECTRIC_SLASH
};

class Ninja : public df::Object {

private:
    // Forward value based on direction facing
    int FORWARD;

    // Down-forward value based on direction facing
    int DOWN_FORWARD;

    // True if player does electric input
    bool is_electric_slash;

    // True if slash attack has been spawned
    bool slash_spawned;

    // Countdown until end slash attack animation
    int slash_countdown;

    // Current animation time
    int current_animation_time;

    // Animation timer
    int time_to_animate;

    // Current animation state
    AnimationState state;

    // Handle step events
    void step();

    // Checks input buffer for Dragon Punch inputs
    void checkForInputs();

    // Starts slash
    void startSlash(bool is_electric);

    // Spawns a slash attack
    void spawnSlashAttack();

    // Changes animation state
    void changeAnimationState(AnimationState new_state);

    // True if animation is finished
    bool animationFinished();

public:
    Ninja();
    ~Ninja();
    int eventHandler(const df::Event* p_e) override;
    int draw();
};

#endif // NINJA_H