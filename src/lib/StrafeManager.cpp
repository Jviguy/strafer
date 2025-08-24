//
// Created by jvigu on 8/23/2025.
//

#include "StraferLib/StrafeManager.h"

#include <cmath>
#include <iostream>

constexpr float MAX_SPEED = 215.0f;       // Max speed with a primary weapon
constexpr float SV_ACCELERATE = 5.5f;     // Default in-game value
constexpr float SV_FRICTION = 4.0f;         // Standard engine value for ground friction
constexpr float SURFACE_FRICTION = 1.0f;  // Normal ground

void StrafeManager::Update(const bool aKey, const bool dKey, const float deltaTime) {
    const float frictionAmount = std::abs(currentVelocity) * SV_FRICTION * deltaTime;
    // --- Apply Friction ---
    // This is the standard Source engine friction model.
    // It's an exponential decay based on current velocity.
    float frictionLoss = currentVelocity * SV_FRICTION * deltaTime;
    currentVelocity -= frictionLoss;

    // --- Apply Player-Controlled Acceleration ---
    // Determine the player's "wish direction" from input.
    float wishDir = 0.0f;
    if (aKey && !dKey) {
        wishDir = -1.0f; // Wish to move left
    } else if (dKey && !aKey) {
        wishDir = 1.0f;  // Wish to move right
    }

    // This block only runs if the player is actively trying to move.
    if (wishDir != 0.0f) {
        // This is the formula directly from your research.
        // It calculates the amount of velocity to add this tick.
        const float accelAmount = SV_ACCELERATE * MAX_SPEED * deltaTime * SURFACE_FRICTION;

        // Apply the acceleration in the desired direction.
        currentVelocity += wishDir * accelAmount;
    }

    // --- Clamp Velocity to Max Speed ---
    // Ensure the player never exceeds the game's hard speed limit.
    currentVelocity = std::clamp(currentVelocity, -MAX_SPEED, MAX_SPEED);

    // --- Snap to Zero ---
    // The "stop speed" threshold to prevent infinitely coasting.
    constexpr float STOP_SPEED_THRESHOLD = 1.0f;
    if (std::abs(currentVelocity) < STOP_SPEED_THRESHOLD && !aKey && !dKey) {
        currentVelocity = 0.0f;
    }
    switch (strafeState) {
        case StrafeState::Idle:
            // if user presses any strafe key then we start a strafe.
            if (aKey != dKey && std::abs(currentVelocity) >= 200) {
                StartStrafe(aKey ? KEY_A : KEY_D);
            }
            break;
        case StrafeState::Strafing: {
            const bool hasPressedOppositeKey = aKey && dKey;

            // First, check if there's an initial key set at all.
            // If not, this is a bug in our state machine.
            if (!initialKey) {
                throw std::logic_error("In Strafing state but initialKey is not set!");
            }

            // Now, we can safely access the value to check for key release.
            const bool hasReleasedInitialKey = (initialKey.value() == KEY_A && !aKey) ||
                                       (initialKey.value() == KEY_D && !dKey);

            // Finally, check if either of our stopping conditions are true.
            if (hasPressedOppositeKey || hasReleasedInitialKey) {
                StartStoppingPhase();
            }
            break;
        }
        case StrafeState::Stopping:
            // if we stopped then we are done lol.
            if (std::abs(currentVelocity) <= 80) {
                strafeHistory.push_back(FinishStrafe());
            }
            break;
    }
}

const Strafe* StrafeManager::GetLastStrafe() const {
    if (strafeHistory.empty()) {
        return nullptr;
    }
    return &strafeHistory.back();
}

const StrafeState &StrafeManager::GetCurrentState() const {
    return strafeState;
}

float StrafeManager::GetCurrentVelocity() const {
    return currentVelocity;
}


void StrafeManager::StartStrafe(const KeyboardKey key) {
    if (strafeState != StrafeState::Idle) {
        throw std::logic_error("Can't restart strafe before finishing old one.");
    }
    strafeState = StrafeState::Strafing;
    initialKey.emplace(key);
}

void StrafeManager::StartStoppingPhase() {
    if (strafeState != StrafeState::Strafing) {
        throw std::logic_error("Can't stop a strafe that isn't started.");
    }
    strafeState = StrafeState::Stopping;
    stopwatch.emplace(steady_clock::now());
}

Strafe StrafeManager::FinishStrafe() {
    if (strafeState != StrafeState::Stopping) {
        throw std::logic_error("FinishStrafe called before counter started.");
    }
    if (!stopwatch) {
        throw std::logic_error("Stopwatch value not set, before finishing strafe");
    }
    if (!initialKey) {
        throw std::logic_error("Initial key not set, before finishing strafe");
    }
    const auto timeToStop = steady_clock::now() - stopwatch.value();
    const Strafe strafe(steady_clock::now(), nextRunNumber++, initialKey.value(), timeToStop);
    Reset();
    return strafe;
}

void StrafeManager::Reset() {
    strafeState = StrafeState::Idle;
    initialKey.reset();
}