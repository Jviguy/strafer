//
// Created by jvigu on 8/23/2025.
//

#ifndef STRAFER_STRAFE_H
#define STRAFER_STRAFE_H
#include <chrono>
#include "raylib.h"

class Strafe {
    using steady_clock = std::chrono::steady_clock;
public:
    Strafe(const steady_clock::time_point startTimestamp, const int64_t run, const KeyboardKey initialKey,
        const steady_clock::duration timeToStop) :
    startTimestamp_{startTimestamp},
    run_{run},
    timeToStop_{timeToStop}
    {}

    [[nodiscard]] steady_clock::time_point GetStartTimestamp() const;

    [[nodiscard]] int64_t GetRun() const;

    [[nodiscard]] KeyboardKey GetInitialKey() const;

    [[nodiscard]] steady_clock::duration getTimeToStop() const;

    [[nodiscard]] uint8_t Score() const;
private:
    steady_clock::time_point startTimestamp_;
    int64_t run_;
    KeyboardKey initialKey_;
    steady_clock::duration timeToStop_;
};


#endif //STRAFER_STRAFE_H