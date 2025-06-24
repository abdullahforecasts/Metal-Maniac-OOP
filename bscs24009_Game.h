#pragma once
#include "bscs24009_Background.h"
#include "bscs24009_Player.h"
#include "bscs24009_Enemy.h"
#include "bscs24009_Jet.h"
#include "bscs24009_Constants.h"
#include "bscs24009_GameExceptions.h"
#include "bscs24009_GameObject.h"
#include <raylib.h>
#include <string>
#include <fstream>
#include <random>

class Game {
private:
    // Game objects
    Background background;
    Player player;
    Enemy* enemies[MaxEnemies];
    Jet jet;
    bool jetActive;
    int enemyCount;
    int maxEnemies;




    // Audio
    Music themeSong;
    Sound stepSound;
    Music winMusic;
    Music loseMusic;
    bool musicPlaying;

    // UI elements
    Texture2D heartFull;
    Texture2D heartTextures[5];
    Texture2D enemyFlag;
    Texture2D valorTexture;

    // Game state
    bool gameStarted;
    bool gameEnded;
    bool playerWon;

    // Ads system
    int valor;
    bool adsEnabled;
    Texture2D adTexture;
    Music adMusic;
    bool showingAd;

    // Shop system
    int currentTheme;
    int currentSkin;
    int unlockedThemes;
    int unlockedSkins;

    // Resource management
    Texture2D sl, s1, s2, s3, s4, so, sc, se, shop, themes, skins, ads;
    Texture2D hrr, hlr, hrs, hls;     // Skin A
    Texture2D hrr1, hlr1, hrs1, hls1; // Skin B
    Texture2D hrr2, hlr2, hrs2, hls2; // Skin C
    Music loadSound;
    bool texturesLoaded = false;
    bool soundPlaying = false;

    // Private methods
    void LoadMusicSafe(Music& music, const std::string& filename);
    void LoadTextureSafe(Texture2D& texture, const string& filename);

    // extra #####################
    bool isPaused = false;

public:
    Game();
    ~Game();

    // Core game loop
    void Play();
    void Start();
    void Update();
    void Draw();
    bool ShouldClose() const;

    // Game systems
    void CheckCollisions();
    void DrawUI();
    void DrawEndScreen();
    void LoadAdResources();
    void ShowAd();
    void UpdateAd();
    void DrawAd();
    void LoadGameState();
    void SaveGameState();

    // Shop functions
    void SetTheme(int theme);
    void SetSkin(int skin);
    void AddValor(int amount);
    void DisableAds();

    // Getters
    int GetValor() const;
    bool GetAdsEnabled() const;
    int GetCurrentTheme() const;
    int GetCurrentSkin() const;
    int GetUnlockedThemes() const;
    int GetUnlockedSkins() const;

    // Setters
    void SetValor(int v);
    void SetAdsEnabled(bool enabled);
    void SetCurrentTheme(int theme);
    void SetCurrentSkin(int skin);
    void SetUnlockedThemes(int themes);
    void SetUnlockedSkins(int skins);

};