#pragma once
#include "button.hpp"
#include <vector>
#include <utility>

namespace ui{

class Manager{
    public:
      template<typename... Args>
      Button& emplaceButton(Args&&... args){

      }  
};
}