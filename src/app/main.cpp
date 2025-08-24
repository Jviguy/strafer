#include <memory>

#include "App.h"

int main() {
    const auto app = std::make_unique<App>();
    app->Loop();
}