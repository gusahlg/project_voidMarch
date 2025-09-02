#pragma once
#include <raylib.h>
#include <functional>
#include <cstdint>
namespace ui{

class Button{
    public:
        using Action = std::function<void()>;
        enum class DisplayDependency : std::uint8_t {None, OnHover, OnClick};
        Button(Rectangle bounds, Texture2D tex, 
            Action onClick = nullptr, std::string name = {}, ui::Button::DisplayDependency dependency = ui::Button::DisplayDependency::None, std::string targetName = {})
        : bounds(bounds), tex(tex), 
          onClick(std::move(onClick)), name(name), DependencyType(dependency), targetName(targetName) {bounds.x=tex.width/3;}

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
            int x = state == State::Hover ? 0 : state == State::Pressed ? bounds.width : bounds.width*2;
            DrawTextureRec(tex, {0,0, bounds.width, bounds.height}, {bounds.x, bounds.y}, WHITE);
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

