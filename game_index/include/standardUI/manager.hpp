#pragma once
#include "button.hpp"
#include <vector>
#include <utility>

namespace ui{

class Manager{
    public:
        template<typename... Args>
        Button& emplaceButton(Args&&... args){
            return buttons.emplace_back(std::forward<Args>(args)...);
        }
        void updateAll(Vector2 mouse){
            for(auto& b : buttons) b.update(mouse);
        }  
        void drawAll() const{
            for(auto& b : buttons) b.draw();
        }
        void clear(){
            buttons.clear();
        }
    private:
        std::vector<Button> buttons;
};
} //Namespace ends here