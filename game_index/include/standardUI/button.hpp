#pragma once
#include <raylib.h>
#include <functional>
#include <cstdint>
namespace ui{

class Button{
    public:
        using Action = std::function<void()>;
        Button(Rectangle bounds,
            Texture2D idle, Texture2D hover, Texture2D pressed,
            Action onClick = nullptr, std::string name = NULL, ui::Button::DisplayDependency dependency = ui::Button::DisplayDependency::None)
        : bounds(bounds), idle(idle), hover(hover), pressed(pressed), 
          onClick(std::move(onClick)), name(name), DependencyType(dependency) {}

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
        enum class DisplayDependency : std::uint8_t {None, Previous, Custom};
    private:
        std::string name;
        enum class State : std::uint8_t {Idle, Hover, Pressed, Clicked};
        DisplayDependency DependencyType = DisplayDependency::None;
        Rectangle bounds;
        Texture2D idle, hover, pressed;
        Action onClick;
        State state = State::Idle;
};


} // Namespace ends here

