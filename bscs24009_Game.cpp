#include "bscs24009_Game.h"
#include "bscs24009_Constants.h"

void Game::LoadMusicSafe(Music& music, const string& filename) {
    music = LoadMusicStream(filename.c_str());
    if (music.ctxData == nullptr) {
        throw MusicLoadException(filename);
    }
}

void Game::LoadTextureSafe(Texture2D& texture, const string& filename) {
    texture = LoadTexture(filename.c_str());
    if (texture.id == 0) {
        throw TextureLoadException(filename);
    }
}

Game::Game() :
    player(static_cast<float>(WindowWidth) / 2.0f - static_cast<float>(PlayerWidth) - 500.0f,
        static_cast<float>(WindowHeight - PlayerHeight - 150)),
    jet(-3.0f * static_cast<float>(JetWidth), 100.0f),
    jetActive(false),
    enemyCount(MaxEnemies),
    maxEnemies(5),
    gameStarted(false),
    gameEnded(false),
    playerWon(false),
    musicPlaying(false),
    valor(0),
    valorTexture({ 0 }),
    adsEnabled(true),
    adTexture({ 0 }),
    adMusic({ 0 }),
    showingAd(false),
    currentTheme(1),
    currentSkin(1),
    unlockedThemes(1),
    unlockedSkins(1)
{
    try {
        LoadGameState();

        // Initialize enemies
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> rangeSelector(0, 1);
        uniform_int_distribution<> range1(-3 * WindowWidth / 2, -PlayerWidth - 10);
        uniform_int_distribution<> range2(WindowWidth + 10, 3 * WindowWidth / 2);

        for (int i = 0; i < MaxEnemies; ++i) {
            float enemyX = (rangeSelector(gen) == 0) ? static_cast<float>(range1(gen)) : static_cast<float>(range2(gen));
            float enemyY = static_cast<float>(WindowHeight - PlayerHeight - 130);
            enemies[i] = new Enemy(enemyX, enemyY);
        }

        // Load music and sounds
        LoadMusicSafe(themeSong, "theme.mp3");
        SetMusicVolume(themeSong, 0.1f);

        // Load end screen music
        LoadMusicSafe(winMusic, "chippi.mp3");
        LoadMusicSafe(loseMusic, "meow.mp3");
        SetMusicVolume(winMusic, 0.5f);
        SetMusicVolume(loseMusic, 0.5f);

        // Load UI textures
        LoadTextureSafe(heartFull, "hf.png");
        LoadTextureSafe(heartTextures[0], "h1.png");
        LoadTextureSafe(heartTextures[1], "h2.png");
        LoadTextureSafe(heartTextures[2], "h3.png");
        LoadTextureSafe(heartTextures[3], "h4.png");
        LoadTextureSafe(heartTextures[4], "h5.png");
        LoadTextureSafe(enemyFlag, "ef.png");
        LoadTextureSafe(valorTexture, "valor.png");
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Game initialization failed: %s", e.what());
        throw;
    }
}

Game::~Game() {
    try {
        SaveGameState();

        UnloadMusicStream(themeSong);
        UnloadMusicStream(winMusic);
        UnloadMusicStream(loseMusic);
        UnloadTexture(heartFull);
        for (int i = 0; i < 5; ++i) {
            UnloadTexture(heartTextures[i]);
        }
        UnloadTexture(enemyFlag);
        UnloadTexture(valorTexture);

        // Clean up enemies
        for (int i = 0; i < MaxEnemies; ++i) {
            delete enemies[i];
        }

        // Clean up ad resources if loaded
        if (adTexture.id != 0) UnloadTexture(adTexture);
        if (adMusic.ctxData != nullptr) UnloadMusicStream(adMusic);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Game destructor error: %s", e.what());
    }
}

void Game::Start() {
    try {
        gameStarted = true;
        PlayMusicStream(themeSong);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Game start error: %s", e.what());
    }
}

void Game::Update() {
    if (!gameStarted || gameEnded) {
        UpdateAd();
        return;
    }

    if (IsKeyPressed(KEY_Z)) {
        isPaused = !isPaused;
    }

    if (isPaused) {
        return; // Skip game updates while paused
    }

    try {
        UpdateMusicStream(themeSong);

        int scrollSpeed = 0;
        if (player.GetX() > WindowWidth / 3) {
            player.SetX(WindowWidth / 3);
            scrollSpeed = background.GetScrollSpeed();
        }

        background.Update(scrollSpeed);
        player.Update();

        for (int i = 0; i < MaxEnemies; ++i) {
            enemies[i]->Update(player.GetX());
        }

        if (jetActive) {
            jet.Update();
        }

        CheckCollisions();

        if (player.GetLife() <= 0) {
            gameEnded = true;
            playerWon = false;
            StopMusicStream(themeSong);
            PlayMusicStream(loseMusic);
            musicPlaying = true;
        }
        else if (enemyCount <= 0) {
            gameEnded = true;
            playerWon = true;
            StopMusicStream(themeSong);
            PlayMusicStream(winMusic);
            musicPlaying = true;
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Game update error: %s", e.what());
    }
}

void Game::Draw() {
    if (!gameStarted) return;

    if (showingAd) {
        DrawAd();
        return;
    }

    try {
        background.Draw();

        if (jetActive) {
            jet.Draw();
        }

        for (int i = 0; i < MaxEnemies; ++i) {
            enemies[i]->Draw();
        }

        player.Draw();
        DrawUI();

        if (isPaused) {
            DrawText("PAUSED", WindowWidth / 2 - 70, WindowHeight / 2 - 20, 40, RED);
        }

        if (gameEnded) {
            DrawEndScreen();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Game draw error: %s", e.what());
    }
}

void Game::CheckCollisions() {
    try {
        // Player bullets vs enemies
        for (int i = 0; i < MaxBullets; ++i) {
            Bullet* playerBullet = &player.GetBullets()[i];

            if (!playerBullet->IsActive()) continue;

            for (int j = 0; j < MaxEnemies; ++j) {
                if (playerBullet->CheckCollision(enemies[j]->GetHitbox())) {
                    playerBullet->SetActive(false);
                    enemies[j]->Respawn();
                    enemyCount--;
                    AddValor(1);
                }

                for (int k = 0; k < MaxBullets; ++k) {
                    Bullet* enemyBullet = &enemies[j]->GetBullets()[k];

                    if (playerBullet->IsActive() && enemyBullet->IsActive() &&
                        playerBullet->CheckCollision(*enemyBullet)) {
                        playerBullet->SetActive(false);
                        enemyBullet->SetActive(false);
                    }
                }
            }
        }

        // Enemy bullets vs player
        for (int i = 0; i < MaxEnemies; ++i) {
            for (int j = 0; j < MaxBullets; ++j) {
                Bullet* enemyBullet = &enemies[i]->GetBullets()[j];

                if (enemyBullet->IsActive() && enemyBullet->CheckCollision(player.GetHitbox())) {
                    enemyBullet->SetActive(false);
                    player.TakeDamage();
                }
            }
        }

        // Jet bullets vs player
        if (jetActive) {
            for (int i = 0; i < MaxJetBullets; ++i) {
                Bullet* jetBullet = &jet.GetBullets()[i];

                if (jetBullet->IsActive() && jetBullet->CheckCollision(player.GetHitbox())) {
                    jetBullet->SetActive(false);
                    player.TakeDamage();
                }
            }
        }

        // Player vs enemies
        for (int i = 0; i < MaxEnemies; ++i) {
            if (CheckCollisionRecs(player.GetHitbox(), enemies[i]->GetHitbox())) {
                enemies[i]->Respawn();
                player.TakeDamage();
            }
        }

        if (player.GetLife() <= 0) {
            gameEnded = true;
            playerWon = false;
            StopMusicStream(themeSong);
            ShowAd();
        }
        else if (enemyCount <= 0) {
            gameEnded = true;
            playerWon = true;
            StopMusicStream(themeSong);
            ShowAd();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Collision detection error: %s", e.what());
    }
}

void Game::DrawUI() {
    try {
        DrawTexture(heartFull, 10, 10, WHITE);

        int life = player.GetLife();
        if (life > 0 && life <= 5) {
            DrawTexture(heartTextures[life - 1], 120, 10, WHITE);
        }

        int valorX = WindowWidth / 2;
        DrawTexture(valorTexture, valorX, 10, WHITE);
        DrawText(TextFormat("%d", valor), valorX + 75, 20, 30, GOLD);

        DrawTexture(enemyFlag, 1585, 10, WHITE);
        DrawText(TextFormat("x%d", enemyCount), 1470, 50, 50, WHITE);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "UI drawing error: %s", e.what());
    }
}

void Game::DrawEndScreen() {
    try {
        static Texture2D winTexture = LoadTexture("win.png");
        static Texture2D loseTexture = LoadTexture("learn.png");

        if (winTexture.id == 0 || loseTexture.id == 0) {
            throw TextureLoadException("end screen texture");
        }

        if (musicPlaying) {
            if (playerWon) {
                UpdateMusicStream(winMusic);
            }
            else {
                UpdateMusicStream(loseMusic);
            }
        }

        if (playerWon) {
            DrawTexture(winTexture, 0, 0, WHITE);

            if (IsKeyPressed(KEY_N)) {
                enemyCount = maxEnemies + 5;
                maxEnemies += 5;
                jetActive = true;
                gameEnded = false;
                StopMusicStream(winMusic);
                PlayMusicStream(themeSong);
                musicPlaying = false;
            }
        }
        else {
            DrawTexture(loseTexture, 0, 0, WHITE);

            if (IsKeyPressed(KEY_R)) {
                player.Reset(
                    static_cast<float>(WindowWidth) / 2.0f - static_cast<float>(PlayerWidth) - 500.0f,
                    static_cast<float>(WindowHeight - PlayerHeight - 150)
                );

                for (int i = 0; i < MaxEnemies; ++i) {
                    enemies[i]->Respawn();
                }

                enemyCount = maxEnemies;
                gameEnded = false;
                StopMusicStream(loseMusic);
                PlayMusicStream(themeSong);
                musicPlaying = false;
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "End screen drawing error: %s", e.what());
    }
}

bool Game::ShouldClose() const {
    return WindowShouldClose() || IsKeyPressed(KEY_ESCAPE);
}

void Game::LoadAdResources() {
    try {
        if (!adsEnabled) return;

        adTexture = LoadTexture("ads.png");
        if (adTexture.id == 0) {
            throw TextureLoadException("ads.png");
        }

        adMusic = LoadMusicStream("seedA.mp3");
        if (adMusic.ctxData == nullptr) {
            throw MusicLoadException("seedA.mp3");
        }
        SetMusicVolume(adMusic, 0.5f);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Ad resources loading failed: %s", e.what());
        adsEnabled = false; // Disable ads if we can't load resources
    }
}

void Game::ShowAd() {
    if (!adsEnabled || showingAd) return;

    try {
        showingAd = true;
        StopMusicStream(themeSong);
        if (IsMusicStreamPlaying(winMusic)) StopMusicStream(winMusic);
        if (IsMusicStreamPlaying(loseMusic)) StopMusicStream(loseMusic);

        PlayMusicStream(adMusic);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Show ad error: %s", e.what());
        showingAd = false;
    }
}

void Game::UpdateAd() {
    if (!showingAd) return;

    try {
        UpdateMusicStream(adMusic);

        if (IsKeyPressed(KEY_S)) {
            showingAd = false;
            StopMusicStream(adMusic);

            if (playerWon) {
                PlayMusicStream(winMusic);
            }
            else {
                PlayMusicStream(loseMusic);
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Ad update error: %s", e.what());
    }
}

void Game::DrawAd() {
    if (!showingAd) return;

    try {
        float scale = min((float)WindowWidth / adTexture.width,
            (float)WindowHeight / adTexture.height);
        float width = adTexture.width * scale;
        float height = adTexture.height * scale;
        float x = (WindowWidth - width) / 2;
        float y = (WindowHeight - height) / 2;

        DrawTextureEx(adTexture, { x, y }, 0, scale, WHITE);
        DrawText("Press S to skip", WindowWidth / 2 - MeasureText("Press S to skip", 30) / 2,
            WindowHeight - 50, 30, WHITE);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Ad drawing error: %s", e.what());
    }
}

void Game::LoadGameState() {
    try {
        ifstream file("GameState.txt");
        if (file.is_open()) {
            file >> valor;
            int adsEnabledInt;
            file >> adsEnabledInt;
            adsEnabled = (adsEnabledInt != 0);
            file >> currentTheme;
            file >> currentSkin;
            file >> unlockedThemes;
            file >> unlockedSkins;
            file.close();

            player.LoadSkin();
            player.SetSkin(currentSkin);
            background.SetTheme(currentTheme);
        }
        else {
            valor = 0;
            adsEnabled = true;
            currentTheme = 1;
            currentSkin = 1;
            unlockedThemes = 1;
            unlockedSkins = 1;
            SaveGameState();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Game state loading failed: %s", e.what());
        // Reset to defaults
        valor = 0;
        adsEnabled = true;
        currentTheme = 1;
        currentSkin = 1;
        unlockedThemes = 1;
        unlockedSkins = 1;
    }
}

void Game::SaveGameState() {
    try {
        ofstream file("GameState.txt");
        if (file.is_open()) {
            file << valor << endl;
            file << (adsEnabled ? 1 : 0) << endl;
            file << currentTheme << endl;
            file << currentSkin << endl;
            file << unlockedThemes << endl;
            file << unlockedSkins;
            file.close();
        }
        else {
            throw GameException("Failed to open GameState.txt for writing");
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Game state saving failed: %s", e.what());
    }
}

void Game::SetTheme(int theme) {
    try {
        if (theme >= 1 && theme <= 2 && theme <= unlockedThemes) {
            currentTheme = theme;
            background.SetTheme(theme);
            SaveGameState();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Set theme error: %s", e.what());
    }
}

void Game::SetSkin(int skin) {
    try {
        if (skin >= 1 && skin <= 3 && skin <= unlockedSkins) {
            currentSkin = skin;
            player.SetSkin(skin);
            SaveGameState();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Set skin error: %s", e.what());
    }
}

void Game::AddValor(int amount) {
    try {
        valor += amount;
        SaveGameState();
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Add valor error: %s", e.what());
    }
}

void Game::DisableAds() {
    try {
        adsEnabled = false;
        SaveGameState();
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Disable ads error: %s", e.what());
    }
}

// Getters and setters
int Game::GetValor() const { return valor; }
bool Game::GetAdsEnabled() const { return adsEnabled; }
int Game::GetCurrentTheme() const { return currentTheme; }
int Game::GetCurrentSkin() const { return currentSkin; }
int Game::GetUnlockedThemes() const { return unlockedThemes; }
int Game::GetUnlockedSkins() const { return unlockedSkins; }

void Game::SetValor(int v) { valor = v; }
void Game::SetAdsEnabled(bool enabled) { adsEnabled = enabled; }
void Game::SetCurrentTheme(int theme) { currentTheme = theme; }
void Game::SetCurrentSkin(int skin) { currentSkin = skin; }
void Game::SetUnlockedThemes(int themes) { unlockedThemes = themes; }
void Game::SetUnlockedSkins(int skins) { unlockedSkins = skins; }