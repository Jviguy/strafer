//
// Created by jvigu on 8/23/2025.
//

#ifndef STRAFER_APP_H
#define STRAFER_APP_H
#include "StraferLib/StrafeManager.h"

class App {
public:
    App();
    ~App();
    void Loop();

    void Update();
    void Draw() const;

    void DrawPanels() const;
    void DrawHistoryLog() const;
    void DrawLiveVelocity() const;
    void DrawVirtualKeyboard() const;
    void DrawStatusPanel() const;
private:
    StrafeManager strafeManager;
    int screenWidth{1280}, screenHeight{720};
    float timeAccum{0.0f};
};


#endif //STRAFER_APP_H