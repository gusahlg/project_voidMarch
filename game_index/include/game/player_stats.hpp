// game_index/include/game/player_stats.hpp
// for game.cpp and sprites.
#pragma once
#include <cstdint>
#include <array>
#include <raylib.h>

// --- Declared elsewhere in your codebase ---
enum class Prof : std::uint8_t { Necromancer, Craftsman, Wizard, Dualist };
extern Prof currentProf;

enum class Race : std::uint8_t { Spacelizard, Voidcrawler, Mecha_sapien, Human };
extern Race currentRace;

// We use your existing Direction enum everywhere.
// Forward-declare only; we DO NOT rely on Direction::_COUNT here.
// Expected order used by your code (IMPORTANT):
// 0:Up, 1:Down, 2:Left, 3:Right, 4:UpLeft, 5:UpRight, 6:DownLeft, 7:DownRight

// Optional: cardinals for roll storage (internal indexing only)
enum class CardDir : std::uint8_t { Up = 0, Down, Left, Right, _COUNT };

class PlayerTexManager {
public:
    static PlayerTexManager& instance() {
        static PlayerTexManager inst;
        return inst;
    }

    // Call during preload when currentRace is known
    void loadWalkFor(Race r) {
        unloadWalk(); // clear previous if any
        switch (r) {
            case Race::Voidcrawler:
                // ORDER MUST MATCH Direction (see header comment)
                loadWalk_(
                    "assets/graphics/void_crawler/void_crawler3.png", // Up
                    "assets/graphics/void_crawler/void_crawler1.png", // Down
                    "assets/graphics/void_crawler/void_crawler2.png", // Left
                    "assets/graphics/void_crawler/void_crawler1.png", // Right
                    "assets/graphics/void_crawler/void_crawler4.png", // UpLeft
                    "assets/graphics/void_crawler/void_crawler3.png", // UpRight
                    "assets/graphics/void_crawler/void_crawler2.png", // DownLeft
                    "assets/graphics/void_crawler/void_crawler1.png"  // DownRight
                );
                break;

            case Race::Spacelizard:
                loadWalk_(
                    "assets/graphics/space_lizard/LizardUpRight.png",    // Up
                    "assets/graphics/space_lizard/SpaceLizardLeft.png",  // Down
                    "assets/graphics/space_lizard/SpaceLizardLeft.png",  // Left
                    "assets/graphics/space_lizard/SpaceLizardRight.png", // Right
                    "assets/graphics/space_lizard/LizardUpLeft.png",     // UpLeft
                    "assets/graphics/space_lizard/LizardUpRight.png",    // UpRight
                    "assets/graphics/space_lizard/SpaceLizardLeft.png",  // DownLeft
                    "assets/graphics/space_lizard/SpaceLizardRight.png"  // DownRight
                );
                break;

            case Race::Human:
                loadWalk_(
                    "assets/graphics/human/human2.png", // Up
                    "assets/graphics/human/human3.png", // Down
                    "assets/graphics/human/human4.png", // Left
                    "assets/graphics/human/human3.png", // Right
                    "assets/graphics/human/human1.png", // UpLeft
                    "assets/graphics/human/human2.png", // UpRight
                    "assets/graphics/human/human4.png", // DownLeft
                    "assets/graphics/human/human3.png"  // DownRight
                );
                break;

            case Race::Mecha_sapien:
                // TODO: add when assets exist
                break;
        }
        setFilterPoint_();
        walkLoaded_ = true;
    }

    // Optional (hook up when your roll assets are consolidated)
    void loadRollFor(Race r) {
        unloadRoll();
        // Example for Voidcrawler; comment out or adjust if not ready:
        if (r == Race::Voidcrawler) {
            // Internal order: Up, Down, Left, Right
            roll_[0] = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll4.png"); // Up
            roll_[1] = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll1.png"); // Down
            roll_[2] = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll2.png"); // Left
            roll_[3] = LoadTexture("assets/graphics/void_crawler/animations/roll/void_crawler_roll1.png"); // Right
            setFilterPoint_();
            rollLoaded_ = true;
        }
    }

    // Accessors used by gameplay/draw
    const Texture2D& walk(Direction d) const {
        return walk_[static_cast<size_t>(d)];
    }

    const Texture2D& roll(Direction d) const {
        return roll_[card_(d)];
    }

    void unloadAll() {
        unloadWalk();
        unloadRoll();
    }

    // Non-copyable
    PlayerTexManager(const PlayerTexManager&) = delete;
    PlayerTexManager& operator=(const PlayerTexManager&) = delete;

private:
    PlayerTexManager() = default;
    ~PlayerTexManager() { unloadAll(); } // safe if called before CloseWindow()

    static constexpr size_t card_(Direction d) {
        // Map full Direction to cardinal index (0:Up,1:Down,2:Left,3:Right)
        // Works regardless of the actual underlying values of Direction.
        switch (static_cast<int>(d)) {
            // Up family
            case 0: /*Up*/      return 0;
            case 4: /*UpLeft*/  return 0;
            case 5: /*UpRight*/ return 0;
            // Down family
            case 1: /*Down*/      return 1;
            case 6: /*DownLeft*/  return 1;
            case 7: /*DownRight*/ return 1;
            // Left / Right
            case 2: /*Left*/  return 2;
            default: /*Right*/ return 3;
        }
    }

    void unloadWalk() {
        if (!walkLoaded_) return;
        for (auto& t : walk_) if (t.id != 0) { UnloadTexture(t); t = Texture2D{}; }
        walkLoaded_ = false;
    }

    void unloadRoll() {
        if (!rollLoaded_) return;
        for (auto& t : roll_) if (t.id != 0) { UnloadTexture(t); t = Texture2D{}; }
        rollLoaded_ = false;
    }

    // ORDER: Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight
    void loadWalk_(const char* up,
                   const char* down,
                   const char* left,
                   const char* right,
                   const char* upLeft,
                   const char* upRight,
                   const char* downLeft,
                   const char* downRight) {
        const char* files[8] = { up, down, left, right, upLeft, upRight, downLeft, downRight };
        for (size_t i = 0; i < 8; ++i) walk_[i] = LoadTexture(files[i]);
    }

    void setFilterPoint_() {
        for (auto& t : walk_) if (t.id != 0) SetTextureFilter(t, TEXTURE_FILTER_POINT);
        for (auto& t : roll_) if (t.id != 0) SetTextureFilter(t, TEXTURE_FILTER_POINT);
    }

    // FIXED SIZES: avoid depending on Direction::_COUNT in this header
    std::array<Texture2D, 8> walk_{};
    std::array<Texture2D, 4> roll_{};
    bool walkLoaded_ = false;
    bool rollLoaded_ = false;
};
