//
// Created by jvigu on 8/23/2025.
//

#include "StraferLib/Strafe.h"

using std::chrono::steady_clock;

steady_clock::time_point Strafe::GetStartTimestamp() const {
    return startTimestamp_;
}

int64_t Strafe::GetRun() const {
    return run_;
}

KeyboardKey Strafe::GetInitialKey() const {
    return initialKey_;
}

steady_clock::duration Strafe::getTimeToStop() const {
    return timeToStop_;
}

uint8_t Strafe::Score() const {
    std::chrono::duration<float, std::milli> duration_ms = getTimeToStop();
    const float timeToStopMs = duration_ms.count();
    constexpr float idealTime = 100.0f;    // The "sweet spot" for a perfect 100.
    constexpr float maxEffectiveTime = 150.0f; // Any stop slower than this is a failed attempt.

    // --- Handle immediate failures ---
    if (constexpr float minEffectiveTime = 50.0f; timeToStopMs < minEffectiveTime || timeToStopMs > maxEffectiveTime) {
        return 0;
    }

    // --- Calculate the score ---
    // First, find the distance from the ideal time.
    const float distance = std::abs(timeToStopMs - idealTime);

    // Calculate the maximum possible distance from the ideal time within the effective range.
    // In this case, 150ms is further from 100ms than 70ms is. (50 vs 30)
    float maxDistance = maxEffectiveTime - idealTime; // 50.0f

    // The score is 100 when the distance is 0, and decreases linearly to 0
    // as the distance approaches its maximum.
    float score = 100.0f * (1.0f - (distance / maxDistance));

    // Clamp the score to ensure it's always between 0 and 100.
    return std::clamp(static_cast<int>(score), 0, 100);
}




