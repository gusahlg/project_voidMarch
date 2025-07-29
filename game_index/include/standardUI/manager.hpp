#pragma once
#include "button.hpp"
#include "ui_helpers.hpp"
#include <vector>
#include <utility>
#include <functional>
#include <type_traits>
namespace ui{

class Manager{
    public:
    using DP = Button::DisplayDependency;
    using State = Button::State;
        template<typename... Args>
        // Rectangle, idle tex, hover tex, pressed tex, onClick lambda.
        Button& emplaceButton(Args&&... args){
            return buttons.emplace_back(std::forward<Args>(args)...);
        }
        void updateAll(Vector2 mouse){
            for(auto& b : buttons){
                if(b.DependencyType == DP::Custom){
                    for(const auto& p : buttons){
                        if(b.targetName == p.name && p.state == State::Hover || p.state == State::Pressed){
                            b.update(mouse);
                        }
                    }
                }
                else b.update(mouse);
            }
        }  
        void drawAll() const{
            for(auto& b : buttons){
                if(b.DependencyType == DP::Custom){
                    for(const auto& p : buttons){
                        if(b.targetName == p.name && p.state == State::Hover || p.state == State::Pressed){
                            b.draw();
                        }
                    }
                }
                else b.draw();
            }
        }
        // For more easily seing borders, great for debugging.
        void highlightOutlines(){
            for(const auto& b : buttons) b.drawOutline();
        }
        void clearAll(){
            buttons.clear();
        }
        // Texture2D and func vector.
        UI_Helper& setupHelper(Texture2D background, std::function<void()> action = nullptr){
            Helper = UI_Helper{background, std::move(action)}; return Helper;
        }
        UI_Helper Helper;
    private:
        std::vector<Button> buttons;
};
} //Namespace ends here