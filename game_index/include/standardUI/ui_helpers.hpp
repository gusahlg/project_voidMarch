#pragma once
#include <raylib.h>
#include <functional>
#include <cstdint>

namespace ui{


class UI_Helper{
    public:
        using Action = std::function<void()>;
        UI_Helper(Texture2D BG, Action action = nullptr)
        : background(BG), func(action) {}
        UI_Helper()
        : background{}, func(nullptr) {}
        void setupScale(float screenWidth, float screenHeight){
            float sizeMultiplier = screenHeight/background.height;
            drawArea = {(screenWidth - background.width * sizeMultiplier) / 2.0f, 0, background.width * sizeMultiplier, background.height * sizeMultiplier};
        }
        void updateFuncs(){
            func();
        }
        void draw() const{
            DrawTexturePro(background, {0,0,float(background.width),float(background.height)}, drawArea, {0,0}, 0, WHITE);
        }
    private:
        Rectangle drawArea;
        Texture2D background;
        Action func;
};




} // Namespace ends here