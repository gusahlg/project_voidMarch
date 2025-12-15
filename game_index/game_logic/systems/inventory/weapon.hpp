#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <functional>
#include <cstddef> 
#include "../combat/combat.hpp"
#include "../../include/global/constants.hpp"
// This is for defining items.
namespace ItemSys {
template<class Derived>
class Utility {
public:
    ~Utility() = default;

    Utility(const Utility&) = default;
    Utility& operator=(const Utility&) = default;
    Utility(Utility&&) = default;
    Utility& operator=(Utility&&) = default;

    Utility(int damage, float cooldownSec, int range, std::function<void()> attackEffect) 
    : dmg_(damage), attacking_(false), spriteW_(0), spriteH_(0),
      animDelay_(0), animTimer_(0.0f), totalFrames_(1), currentFrame_(0), cooldown_(cooldownSec),
      timer_(cooldownSec), range_(range), attackFunction(std::move(attackEffect)),
      src_{0, 0, 0, 0}
    {}
    bool attackReady(float dt){
        timer_ -= dt;
        return timer_ <= 0.f;
    }
    void setSprite(Texture2D tex){
        sprite_ = tex;
    }
    void setSpriteSheet(int columns, int rows, int frames, int animDelay){
        spriteW_ = sprite_.width/columns;
        spriteH_ = sprite_.height/rows;
        totalFrames_ = frames;
        animDelay_ = animDelay;
        animTimer_ = static_cast<float>(animDelay_);
        currentFrame_ = 0;
        src_ = Rectangle{0.0f, 0.0f, static_cast<float>(spriteW_), static_cast<float>(spriteH_)};
    }
    void tickAnim(float dt){
        if(attacking_) static_cast<Derived*>(this)->tickAttackAnimImpl(dt);
        else static_cast<Derived*>(this)->tickIdleAnimImpl(dt);
    }
    template<class... Args>
/*  The all purpose draw function, redirects to implementation functions,
    this allows for variadic behaviour between subclasses whilst withholding 
    maximum performance.
*/
    void draw(Args&&... args) const{
        static_cast<Derived const*>(this)->drawImpl(std::forward<Args>(args)...);
    }
    template<class... Args>

/*  The all purpose attack function, redirects to implementation functions,
    this allows for variadic behaviour between subclasses whilst withholding 
    maximum performance.
*/
    void attack(Args&&... args) const{
        static_cast<Derived const*>(this)->attackImpl(std::forward<Args>(args)...);
    }
protected:
    int dmg_;
    bool attacking_;
    Texture2D sprite_;
    int spriteW_;
    int spriteH_;
    int animDelay_;
    float animTimer_;
    int currentFrame_;
    float cooldown_;
    int totalFrames_;
    float timer_;
    int range_;
    std::function<void()> attackFunction;
    Rectangle src_;
    
    // The default draw and attack functions (no override)
    void defaultDraw() const{
        DrawTexturePro(sprite_, src_, Rectangle{playerRect.x, playerRect.y, src_.width, src_.height}, {0,0}, 0, WHITE);
    }
    void defaultAttack() const{
        
    }
    void defaultTickAnim(float dt){
        animTimer_ -= dt * 1000.f;
        if(animTimer_ <= 0.0f){
            animTimer_ = static_cast<float>(animDelay_);
            currentFrame_ = (currentFrame_ + 1) % totalFrames_;
            src_.x = static_cast<float>((currentFrame_ * spriteW_) % sprite_.width);
            src_.y = 0;
        }
    }
};

class Default : public Utility<Default>{
    void drawImpl()const{defaultDraw();}
    void attackImpl(){defaultAttack();}
    void tickIdleAnimImpl(float dt){defaultTickAnim(dt);}
};

class RangedWeapon : public Utility<RangedWeapon>{
public:
    using Base = Utility<RangedWeapon>;
    RangedWeapon(int damage, float cooldownSec, int range,
                 std::function<void()> attackFunc,
                 float projW, float projH, float projSpeed)
    : Base(damage, cooldownSec, range, std::move(attackFunc)),
      projectileW_(projW), projectileH_(projH), projectileSpeed_(projSpeed) {}

    void drawImpl() const{
        Vector2 mousePos = Vector2Subtract(mouseWorld, playerPixCenter);
        mousePos = Vector2Normalize(mousePos);
        float rotation = atan2f(mousePos.x, mousePos.y) * RAD2DEG;
        DrawTexturePro(sprite_, src_, Rectangle{playerPixCenter.x, playerPixCenter.y, src_.width, src_.height}, Vector2{src_.width/2.f, src_.height/2.f}, rotation, WHITE);
    }
    void attackImpl() const{
        Vector2 dir = Vector2Subtract(mouseWorld, playerPixCenter);
        float len2 = dir.x*dir.x + dir.y*dir.y;
        if(len2 > 1e-8f){
            float inv = 1.0f / sqrtf(len2);
            dir = {dir.x*inv, dir.y*inv};
        } 
        else{
            dir = {1.f, 0.f};
        }
        Combat::spawnProjectile(playerPixCenter, dir, projectileW_, projectileH_, projectileSpeed_, dmg_);
    }
    void updateProjectiles(Level& lvl, float dt) const{
        Combat::updateProjectiles(lvl, dt);
    }
    void tickAttackAnimImpl(float dt){
        animTimer_ -= dt * 1000.f;
        if(animTimer_ <= 0.0f){
            animTimer_ = static_cast<float>(animDelay_);
            currentFrame_ = (currentFrame_ + 1) % totalFrames_;
            src_.x = static_cast<float>((currentFrame_ * spriteW_) % sprite_.width);
            src_.y = src_.height;
        }
    }
    void tickIdleAnimImpl(float dt){
        defaultTickAnim(dt);
    }
private:
    const float projectileW_;
    const float projectileH_;
    const float projectileSpeed_;
};
class MeleeWeapon : public Utility<MeleeWeapon>{
public:
    using Base = Utility<MeleeWeapon>;
    MeleeWeapon(int damage, float cooldownSec, int range,
                std::function<void()> attackFunc)
    : Base(damage, cooldownSec, range, std::move(attackFunc)) {}  

    void drawImpl() const{
        Vector2 mousePos = Vector2Subtract(mouseWorld, playerPixCenter);
        float rotation;
        if(mousePos.y > playerPixCenter.y) mousePos.y = -1;
        else mousePos.y = 1;
        if(mousePos.x > playerPixCenter.x) mousePos.x = -1;
        else mousePos.x = 1;
        rotation = atan2f(mousePos.x, mousePos.y) * RAD2DEG;
        DrawTexturePro(sprite_, src_, Rectangle{playerPixCenter.x, playerPixCenter.y, src_.width, src_.height}, Vector2{src_.width/2.f, src_.height/2.f}, rotation, WHITE);
    }
    void attackImpl() const{
        Combat::enemyCircleSectorCollHandler(range_, playerPixCenter, dmg_);
    }
    void tickAttackAnimImpl(float dt){
        animTimer_ -= dt * 1000.f;
        if(animTimer_ <= 0.0f){
            animTimer_ = static_cast<float>(animDelay_);
            currentFrame_ = (currentFrame_ + 1) % totalFrames_;
            src_.x = static_cast<float>((currentFrame_ * spriteW_) % sprite_.width);
            src_.y = src_.height;
        }
    }
    void tickIdleAnimImpl(float dt){
        defaultTickAnim(dt);
    }
};
class Weapon : public Utility<Weapon>{
public:
    using Base = Utility<Weapon>;
    Weapon(int damage, float cooldownSec, int range,
                std::function<void()> attackFunc)
    : Base(damage, cooldownSec, range, std::move(attackFunc)) {}
    void drawImpl() const{
        defaultDraw();
    }
    void attackImpl(){
        defaultAttack();
    }
    void tickAttackAnimImpl(float dt){
        
    }
    void tickIdleAnimImpl(float dt){
        defaultTickAnim(dt);
    }
};
}