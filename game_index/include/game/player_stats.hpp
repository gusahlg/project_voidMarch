// for game.cpp and sprites.
#pragma once
#include <cstdint>
#include <array>
#include <raylib.h>

// --- Game choices set elsewhere ---
enum class Prof : std::uint8_t { Necromancer, Craftsman, Wizard, Dualist };
extern Prof currentProf;

enum class Race : std::uint8_t { Spacelizard, Voidcrawler, Mecha_sapien, Human };
extern Race currentRace;

// Order is important: use indexes 0..7
enum class WalkDir : std::uint8_t {
    Up = 0, UpLeft, UpRight, Down, DownLeft, DownRight, Right, Left, _COUNT
};

// Optional: cardinal directions for rolling
enum class CardDir : std::uint8_t { Up = 0, Down, Left, Right, _COUNT };

class PlayerTexManager {
public:
    static PlayerTexManager& instance() {
        static PlayerTexManager inst;
        return inst;
    }

    // Load per race once (call during preload when currentRace is known)
    void loadWalkFor(Race r) {
        unloadWalk(); // clear previous if any
        switch (r) {
            case Race::Voidcrawler:
                loadWalk_( // file paths from your current code
                    "assets/graphics/void_crawler/void_crawler3.png", // Up
                    "assets/graphics/void_crawler/void_crawler4.png", // UpLeft
                    "assets/graphics/void_crawler/void_crawler3.png", // UpRight
                    "assets/graphics/void_crawler/void_crawler1.png", // Down
                    "assets/graphics/void_crawler/void_crawler2.png", // DownLeft
                    "assets/graphics/void_crawler/void_crawler1.png", // DownRight
                    "assets/graphics/void_crawler/void_crawler1.png", // Right (placeholder = your asset naming)
                    "assets/graphics/void_crawler/void_crawler2.png"  // Left  (placeholder = your asset naming)
                );
                break;

            case Race::Spacelizard:
                loadWalk_(
                    "assets/graphics/space_lizard/LizardUpRight.png",   // Up
                    "assets/graphics/space_lizard/LizardUpLeft.png",    // UpLeft
                    "assets/graphics/space_lizard/LizardUpRight.png",   // UpRight
                    "assets/graphics/space_lizard/SpaceLizardLeft.png", // Down
                    "assets/graphics/space_lizard/SpaceLizardLeft.png", // DownLeft
                    "assets/graphics/space_lizard/SpaceLizardRight.png",// DownRight
                    "assets/graphics/space_lizard/SpaceLizardRight.png",// Right
                    "assets/graphics/space_lizard/SpaceLizardLeft.png"  // Left
                );
                break;

            case Race::Human:
                loadWalk_(
                    "assets/graphics/human/human2.png", // Up
                    "assets/graphics/human/human1.png", // UpLeft
                    "assets/graphics/human/human2.png", // UpRight
                    "assets/graphics/human/human3.png", // Down
                    "assets/graphics/human/human4.png", // DownLeft
                    "assets/graphics/human/human3.png", // DownRight
                    "assets/graphics/human/human3.png", // Right
                    "assets/graphics/human/human4.png"  // Left
                );
                break;

            case Race::Mecha_sapien:
                // TODO: assets not present in branch; wire when you add them
                break;
        }
        setFilterPoint_();
        walkLoaded_ = true;
    }

    // Later: do the same for roll if/when you consolidate roll sheets
    void loadRollFor(Race /*r*/) {/* optional – see notes below */}

    const Texture2D& walkSheet(WalkDir d) const {
        return walk_[static_cast<size_t>(d)];
    }

    // Optional convenience if you later manage roll:
    const Texture2D& rollSheet(CardDir c) const {
        return roll_[static_cast<size_t>(c)];
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
    ~PlayerTexManager() { unloadAll(); } // safe on shutdown if window still open

    void unloadWalk() {
        if (!walkLoaded_) return;
        for (auto& t : walk_) if (t.id != 0) UnloadTexture(t), t = Texture2D{};
        walkLoaded_ = false;
    }
    void unloadRoll() {
        if (!rollLoaded_) return;
        for (auto& t : roll_) if (t.id != 0) UnloadTexture(t), t = Texture2D{};
        rollLoaded_ = false;
    }

    void loadWalk_(const char* up, const char* upLeft, const char* upRight,
                   const char* down, const char* downLeft, const char* downRight,
                   const char* right, const char* left) {
        const char* files[8] = { up, upLeft, upRight, down, downLeft, downRight, right, left };
        for (size_t i = 0; i < 8; ++i) walk_[i] = LoadTexture(files[i]);
    }

    void setFilterPoint_() {
        for (auto& t : walk_) if (t.id != 0) SetTextureFilter(t, TEXTURE_FILTER_POINT);
        for (auto& t : roll_) if (t.id != 0) SetTextureFilter(t, TEXTURE_FILTER_POINT);
    }

    std::array<Texture2D, static_cast<size_t>(WalkDir::_COUNT)> walk_{};
    std::array<Texture2D, static_cast<size_t>(CardDir::_COUNT)> roll_{}; // optional for later
    bool walkLoaded_ = false;
    bool rollLoaded_ = false;
};
