//
// Created by jvigu on 8/23/2025.
//

#include "App.h"
#include "raylib.h"

const char* GetStateString(const StrafeState state) {
    switch (state) {
        case StrafeState::Idle:            return "Idle";
        case StrafeState::Strafing:        return "Strafing";
        case StrafeState::Stopping:        return "Stopping";
            // Add other states as you implement them
        default:                           return "Unknown";
    }
}

Color GetStateColor(StrafeState state) {
    switch (state) {
        case StrafeState::Idle: return GRAY;
        case StrafeState::Strafing: return SKYBLUE;
        case StrafeState::Stopping: return ORANGE;
        default: return WHITE;
    }
}

App::App() {
    InitWindow(screenWidth, screenHeight, "Strafer.exe");
    SetTargetFPS(360);
}

App::~App() {
    CloseWindow();
}

void App::Loop() {

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

constexpr float TICK_INTERVAL = 1.0f / 64.0f;

void App::Update() {
    timeAccum += GetFrameTime();
    while (timeAccum >= TICK_INTERVAL) {
        strafeManager.Update(IsKeyDown(KEY_A), IsKeyDown(KEY_D), TICK_INTERVAL);
        timeAccum -= TICK_INTERVAL;
    }
}


void App::Draw() const {
    BeginDrawing();
    ClearBackground({25, 25, 35, 255}); // Dark charcoal background

    // --- Draw Background Grid ---
    DrawGrid(20, 1.0f);

    // --- Draw UI Panels ---
    DrawPanels();

    // --- Left Pane: History Log ---
    DrawHistoryLog();

    // --- Right Pane: Live Feedback ---
    DrawLiveVelocity();
    DrawVirtualKeyboard();
    DrawStatusPanel();

    EndDrawing();
}

void App::DrawPanels() const {
    // A semi-transparent panel for the left side
    DrawRectangleRounded({10, 10, (float)screenWidth / 2 - 15, (float)screenHeight - 20}, 0.05f, 10, {45, 45, 55, 180});
    // A semi-transparent panel for the right side
    DrawRectangleRounded({(float)screenWidth / 2 + 5, 10, (float)screenWidth / 2 - 15, (float)screenHeight - 20}, 0.05f, 10, {45, 45, 55, 180});
}

void App::DrawHistoryLog() const {
    const int posX = 30;
    int posY = 25;
    // Header
    DrawText("ATTEMPT HISTORY", posX, posY, 20, WHITE);
    posY += 30;
    DrawText("#", posX + 5, posY, 10, GRAY);
    DrawText("TIME (ms)", posX + 50, posY, 10, GRAY);
    DrawText("SCORE", posX + 150, posY, 10, GRAY);
    posY += 10;
    DrawLine(posX, posY, screenWidth / 2 - 30, posY, GRAY);
    posY += 10;

    // Mock history data
    const std::deque<Strafe> history = strafeManager.GetStrafes();
    for (size_t i = 0; i < history.size(); ++i) {
        const auto& strafe = history[i];
        Color textColor = (strafe.Score() == 100) ? LIME : WHITE; // Highlight perfect scores

        // Zebra striping
        if (i % 2 == 0) {
            DrawRectangle(posX - 10, posY, screenWidth / 2 - 30, 20, {0, 0, 0, 20});
        }
        std::chrono::duration<float, std::milli> duration_ms = strafe.getTimeToStop();
        const float ms = duration_ms.count();
        DrawText(TextFormat("%d", strafe.GetRun()), posX + 5, posY, 20, textColor);
        DrawText(TextFormat("%.2f", ms), posX + 50, posY, 20, textColor);
        DrawText(TextFormat("%d/100", strafe.Score()), posX + 150, posY, 20, textColor);
        posY += 25;
    }
}

void App::DrawLiveVelocity() const {
    const float velocityRatio = strafeManager.GetCurrentVelocity() / 215.0f; // Value from -1.0 to 1.0

    constexpr int barWidth = 400;
    constexpr int barHeight = 30;
    const int barX = screenWidth / 2 + (screenWidth / 2 - barWidth) / 2;
    constexpr int barY = 50;

    DrawRectangleRounded({static_cast<float>(barX), static_cast<float>(barY), static_cast<float>(barWidth), static_cast<float>(barHeight)}, 0.2f, 8, DARKGRAY);

    float fillWidth = std::abs(velocityRatio) * barWidth;
    Rectangle fillRect = {(velocityRatio > 0) ? static_cast<float>(barX) : static_cast<float>(barX) + barWidth - fillWidth, static_cast<float>(barY), fillWidth, static_cast<float>(barHeight)};
    DrawRectangleRec(fillRect, SKYBLUE);

    DrawRectangleRoundedLines({static_cast<float>(barX), static_cast<float>(barY), static_cast<float>(barWidth), static_cast<float>(barHeight)}, 0.2f, 8, {35, 35, 45, 255});
    DrawText(TextFormat("%.2f u/s", strafeManager.GetCurrentVelocity()), barX + 10, barY + 7, 20, WHITE);
}

void App::DrawVirtualKeyboard() const {
    const bool aIsDown = strafeManager.GetCurrentVelocity() < -50.0f; // Mock keyboard state
    const bool dIsDown = strafeManager.GetCurrentVelocity() > 50.0f;

    constexpr int keySize = 60;
    constexpr int keySpacing = 10;
    constexpr auto keyReleasedColor = DARKGRAY;
    constexpr auto keyPressedColor = SKYBLUE;
    constexpr auto keyReleasedTextColor = LIGHTGRAY;
    constexpr auto keyPressedTextColor = WHITE;

    const int centerX = static_cast<int>(screenWidth * 0.75f);
    const int centerY = static_cast<int>(screenHeight * 0.6f);

    const Rectangle keyA = {static_cast<float>(centerX) - keySize - (keySpacing / 2.0f), static_cast<float>(centerY),
        static_cast<float>(keySize), static_cast<float>(keySize)};
    const Rectangle keyD = {static_cast<float>(centerX) + (keySpacing / 2.0f), static_cast<float>(centerY),
        static_cast<float>(keySize), static_cast<float>(keySize)};

    // Draw A Key
    DrawRectangleRounded(keyA, 0.1f, 8, aIsDown ? keyPressedColor : keyReleasedColor);
    DrawRectangleRoundedLines(keyA, 0.1f, 8, BLACK);
    DrawText("A", static_cast<int>(keyA.x) + 22, static_cast<int>(keyA.y) + 15, 30, aIsDown ? keyPressedTextColor : keyReleasedTextColor);

    // Draw D Key
    DrawRectangleRounded(keyD, 0.1f, 8, dIsDown ? keyPressedColor : keyReleasedColor);
    DrawRectangleRoundedLines(keyD, 0.1f, 8, BLACK);
    DrawText("D", static_cast<int>(keyD.x) + 22, static_cast<int>(keyD.y) + 15, 30, dIsDown ? keyPressedTextColor : keyReleasedTextColor);
}

void App::DrawStatusPanel() const {
    const int panelX = screenWidth / 2 + 25;
    const int panelY = screenHeight - 100;

    const char* stateText = GetStateString(strafeManager.GetCurrentState());
    const Color stateColor = GetStateColor(strafeManager.GetCurrentState());

    DrawText("STATUS:", panelX, panelY, 20, WHITE);
    DrawText(stateText, panelX + 100, panelY, 20, stateColor);
}
