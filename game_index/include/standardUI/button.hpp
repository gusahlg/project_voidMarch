#pragma once
#include <raylib.h>
#include <functional>
#include <cstdint>
namespace ui{

class Button{
    public:
        using Action = std::function<void()>;
        enum class DisplayDependency : std::uint8_t {None, Previous, Custom};
        Button(Rectangle bounds,
            Texture2D idle, Texture2D hover, Texture2D pressed,
            Action onClick = nullptr, std::string name = {}, ui::Button::DisplayDependency dependency = ui::Button::DisplayDependency::None, std::string targetName = {})
        : bounds(bounds), idle(idle), hover(hover), pressed(pressed), 
          onClick(std::move(onClick)), name(name), DependencyType(dependency), targetName(targetName) {}

        void update(Vector2 mouse){
            bool inside = CheckCollisionPointRec(mouse, bounds);
            if(!inside) state = State::Idle;
            else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) state = State::Pressed;
            else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && state == State::Pressed){
                state = State::Clicked;
                if(onClick) onClick();
            }
            else state = State::Hover;
        }
        void draw() const{
            const Texture2D tex = state == State::Hover ? hover : state == State::Pressed ? pressed : idle;
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
    private:
        Rectangle bounds;
        Texture2D idle, hover, pressed;
        Action onClick;
};


} // Namespace ends here

