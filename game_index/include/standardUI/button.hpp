#pragma once
#include <raylib.h>
#include <functional>
#include <cstdint>
namespace ui{

class Button{
    public:
        using Action = std::function<void()>;
        enum class DisplayDependency : std::uint8_t {None, OnHover, OnClick};
        Button(Vector2 pos, Texture2D tex, Action onClick = nullptr, std::string name = {}, ui::Button::DisplayDependency dependency = ui::Button::DisplayDependency::None, std::string targetName = {})
        : tex(tex), onClick(std::move(onClick)), name(name), DependencyType(dependency), targetName(targetName) {bounds=Rectangle{pos.x,pos.y,(float)tex.width/3,(float)tex.height};}

        void update(Vector2 mouse){
            bool inside = CheckCollisionPointRec(mouse, bounds);
            if(!inside) state = State::Idle;
            else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) state = State::Pressed;
            else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && state == State::Pressed){
                state = State::Clicked;
                if(toggled) toggled = false;
                else toggled = true;
                if(onClick) onClick();
            }
            else state = State::Hover;
        }
        void draw() const{
            float frameW = bounds.width;
            float srcX = 0.0f;
            switch(state){
                case State::Idle:    srcX = 0.0f;           break;
                case State::Hover:   srcX = frameW;         break;
                case State::Pressed:
                case State::Clicked: srcX = frameW * 2.0f;  break;
            }
            Rectangle src{ srcX, 0.0f, frameW, bounds.height };
            Vector2   dst{ bounds.x, bounds.y };
            DrawTextureRec(tex, src, dst, WHITE);
        }
        void drawOutline() const{
            DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, RED);
        }
        DisplayDependency DependencyType = DisplayDependency::None;
        std::string name;
        std::string targetName; // Used for checking when to update when set to custom dependency.
        enum class State : std::uint8_t {Idle, Hover, Pressed, Clicked};
        State state = State::Idle;
        bool toggled = false;
    private:
        Rectangle bounds;
        Texture2D tex;
        Action onClick;
};


} // Namespace ends here

