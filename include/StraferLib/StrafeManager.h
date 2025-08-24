//
// Created by jvigu on 8/23/2025.
//

#ifndef STRAFER_STRAFEMANAGER_H
#define STRAFER_STRAFEMANAGER_H
#include <chrono>
#include <deque>

#include "raylib.h"
#include "Strafe.h"

enum class StrafeState {
    Idle,
    Strafing,
    Stopping
};
class StrafeManager {
using steady_clock = std::chrono::steady_clock;
public:
    void Update(bool aKey, bool dKey, float deltaTime);

    [[nodiscard]] const std::deque<Strafe>& GetStrafes() const { return strafeHistory; }

    [[nodiscard]] const Strafe* GetLastStrafe() const;

    [[nodiscard]] const StrafeState& GetCurrentState() const;

    [[nodiscard]] float GetCurrentVelocity() const;

private:
    StrafeState strafeState{StrafeState::Idle};
    std::deque<Strafe> strafeHistory;
    int nextRunNumber{1};

    float currentVelocity{0.0f};
    std::optional<KeyboardKey> initialKey;
    std::optional<steady_clock::time_point> stopwatch;

    void StartStrafe(KeyboardKey key);

    void StartStoppingPhase();

    Strafe FinishStrafe();

    void Reset();
};


#endif //STRAFER_STRAFEMANAGER_H