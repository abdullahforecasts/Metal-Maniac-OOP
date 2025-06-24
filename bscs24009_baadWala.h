#pragma once
// Static variables to hold textures and music
static Music loadSound;
static Texture2D sl, s1, s2, s3, s4, so, sc, se, shop, themes, skins, ads, valorTexture;
static bool texturesLoaded = false;
static bool soundPlaying = false;

// Skin textures
static Texture2D hrr, hlr, hrs, hls;     // Skin A (default)
static Texture2D hrr1, hlr1, hrs1, hls1; // Skin B
static Texture2D hrr2, hlr2, hrs2, hls2; // Skin C

void LoadTextureSafe(Texture2D& texture, const string& filename) {
    texture = LoadTexture(filename.c_str());
    if (texture.id == 0) {
        throw TextureLoadException(filename);
    }
}

void LoadAllTextures() {
    if (texturesLoaded) return;

    try {
        loadSound = LoadMusicStream("loadSound.mp3");
        if (loadSound.ctxData == nullptr) {
            throw MusicLoadException("loadSound.mp3");
        }

        LoadTextureSafe(sl, "sl.png");
        LoadTextureSafe(s1, "s1.png");
        LoadTextureSafe(s2, "s2.png");
        LoadTextureSafe(s3, "s3.png");
        LoadTextureSafe(s4, "s4.png");
        LoadTextureSafe(so, "so.png");
        LoadTextureSafe(sc, "sc.png");
        LoadTextureSafe(se, "se.png");
        LoadTextureSafe(shop, "shp.png");
        LoadTextureSafe(themes, "thms.png");
        LoadTextureSafe(skins, "skns.png");
        LoadTextureSafe(ads, "adp.png");
        LoadTextureSafe(valorTexture, "valor.png");

        // Load skin textures
        LoadTextureSafe(hrr, "hrr.png");
        LoadTextureSafe(hlr, "hlr.png");
        LoadTextureSafe(hrs, "hrs.png");
        LoadTextureSafe(hls, "hls.png");

        LoadTextureSafe(hrr1, "hrr1.png");
        LoadTextureSafe(hlr1, "hlr1.png");
        LoadTextureSafe(hrs1, "hrs1.png");
        LoadTextureSafe(hls1, "hls1.png");

        LoadTextureSafe(hrr2, "hrr2.png");
        LoadTextureSafe(hlr2, "hlr2.png");
        LoadTextureSafe(hrs2, "hrs2.png");
        LoadTextureSafe(hls2, "hls2.png");

        texturesLoaded = true;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Texture loading failed: %s", e.what());
        throw;
    }
}

void UnloadAllTextures() {
    try {
        if (texturesLoaded) {
            UnloadMusicStream(loadSound);
            UnloadTexture(sl); UnloadTexture(s1); UnloadTexture(s2);
            UnloadTexture(s3); UnloadTexture(s4); UnloadTexture(so);
            UnloadTexture(sc); UnloadTexture(se); UnloadTexture(shop);
            UnloadTexture(themes); UnloadTexture(skins); UnloadTexture(ads);
            UnloadTexture(valorTexture);

            // Unload skins
            UnloadTexture(hrr); UnloadTexture(hlr); UnloadTexture(hrs); UnloadTexture(hls);
            UnloadTexture(hrr1); UnloadTexture(hlr1); UnloadTexture(hrs1); UnloadTexture(hls1);
            UnloadTexture(hrr2); UnloadTexture(hlr2); UnloadTexture(hrs2); UnloadTexture(hls2);

            texturesLoaded = false;
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Texture unloading failed: %s", e.what());
    }
}

void DrawPlayerSkin(int x, int y, int currentSkin) {
    try {
        switch (currentSkin) {
        case 1: DrawTexture(hrr, x, y, WHITE); break;
        case 2: DrawTexture(hrr1, x, y, WHITE); break;
        case 3: DrawTexture(hrr2, x, y, WHITE); break;
        default: DrawTexture(hrr, x, y, WHITE); break;
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player skin drawing failed: %s", e.what());
    }
}

void HandleSkinMenu(Game& game) {
    bool inSkins = true;
    while (inSkins) {
        try {
            UpdateMusicStream(loadSound);

            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(skins, 0, 0, WHITE);
            DrawTexture(valorTexture, WindowWidth - 100, 10, WHITE);
            DrawText(TextFormat("%d", game.GetValor()), WindowWidth - 50, 20, 30, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_A)) {
                game.SetCurrentSkin(1);
                game.SaveGameState();
            }
            else if (IsKeyPressed(KEY_B)) {
                if (game.GetUnlockedSkins() < 2 && game.GetValor() >= 1000) {
                    game.SetValor(game.GetValor() - 1000);
                    game.SetUnlockedSkins(2);
                }
                if (game.GetUnlockedSkins() >= 2) {
                    game.SetCurrentSkin(2);
                }
                game.SaveGameState();
            }
            else if (IsKeyPressed(KEY_C)) {
                if (game.GetUnlockedSkins() >= 2 && game.GetUnlockedSkins() < 3 && game.GetValor() >= 2000) {
                    game.SetValor(game.GetValor() - 2000);
                    game.SetUnlockedSkins(3);
                }
                if (game.GetUnlockedSkins() >= 3) {
                    game.SetCurrentSkin(3);
                }
                game.SaveGameState();
            }
            else if (IsKeyPressed(KEY_BACKSPACE)) {
                inSkins = false;
            }
        }
        catch (const exception& e) {
            TraceLog(LOG_ERROR, "Skin menu error: %s", e.what());
            inSkins = false;
        }
    }
}

bool StartAnimation() {
    try {
        // Load all required textures and sounds
        LoadAllTextures();

        // Initialize music
        loadSound = LoadMusicStream("loadSound.mp3");
        if (loadSound.ctxData == nullptr) {
            throw MusicLoadException("loadSound.mp3");
        }

        PlayMusicStream(loadSound);
        SetMusicVolume(loadSound, 8.0f);

        // Initial logo display
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(sl, 0, 0, WHITE);
        EndDrawing();
        WaitTime(2.0);

        // Loading screens sequence
        Texture2D loadingScreens[] = { s1, s2, s3, s4 };
        for (int i = 0; i < 4; i++) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(loadingScreens[i], 0, 0, WHITE);
            EndDrawing();
            WaitTime(1.25);
        }

        // Wait for ENTER key to proceed to main menu
        while (!IsKeyPressed(KEY_ENTER)) {
            try {
                UpdateMusicStream(loadSound);

                if (WindowShouldClose()) {
                    UnloadAllTextures();
                    UnloadMusicStream(loadSound);
                    return false;
                }

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(s4, 0, 0, WHITE);
                EndDrawing();
            }
            catch (const exception& e) {
                TraceLog(LOG_ERROR, "Animation loop error: %s", e.what());
                if (WindowShouldClose()) {
                    UnloadAllTextures();
                    UnloadMusicStream(loadSound);
                    return false;
                }
            }
        }
        WaitTime(0.09);  // Debounce

        // Main menu loop
        Game game;
        while (true) {
            try {
                UpdateMusicStream(loadSound);

                if (WindowShouldClose()) {
                    UnloadAllTextures();
                    UnloadMusicStream(loadSound);
                    return false;
                }

                // Load game state for currency display
                game.LoadGameState();

                // Draw main menu
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(so, 0, 0, WHITE);
                DrawTexture(valorTexture, WindowWidth - 170, 10, WHITE);
                DrawText(TextFormat("%d", game.GetValor()), WindowWidth - 100, 20, 30, GOLD);
                EndDrawing();

                // Handle menu options
                if (IsKeyPressed(KEY_P)) {
                    WaitTime(0.09);
                    return true;
                }

                if (IsKeyPressed(KEY_C)) {
                    WaitTime(0.09);
                    bool inCredits = true;
                    while (inCredits) {
                        try {
                            UpdateMusicStream(loadSound);
                            if (WindowShouldClose()) {
                                UnloadAllTextures();
                                UnloadMusicStream(loadSound);
                                return false;
                            }

                            BeginDrawing();
                            ClearBackground(BLACK);
                            DrawTexture(sc, 0, 0, WHITE);
                            EndDrawing();

                            if (IsKeyPressed(KEY_BACKSPACE)) {
                                inCredits = false;
                                WaitTime(0.09);
                            }
                        }
                        catch (const exception& e) {
                            TraceLog(LOG_ERROR, "Credits menu error: %s", e.what());
                            inCredits = false;
                        }
                    }
                }

                if (IsKeyPressed(KEY_E)) {
                    try {
                        game.SaveGameState();
                        BeginDrawing();
                        ClearBackground(BLACK);
                        DrawTexture(se, 0, 0, WHITE);
                        EndDrawing();
                        WaitTime(7.0);
                        UnloadAllTextures();
                        UnloadMusicStream(loadSound);
                        return false;
                    }
                    catch (const exception& e) {
                        TraceLog(LOG_ERROR, "Exit screen error: %s", e.what());
                        UnloadAllTextures();
                        UnloadMusicStream(loadSound);
                        return false;
                    }
                }

                if (IsKeyPressed(KEY_S)) {
                    WaitTime(0.09);
                    bool inShop = true;
                    while (inShop) {
                        try {
                            UpdateMusicStream(loadSound);
                            if (WindowShouldClose()) {
                                UnloadAllTextures();
                                UnloadMusicStream(loadSound);
                                return false;
                            }

                            game.LoadGameState();

                            BeginDrawing();
                            ClearBackground(BLACK);
                            DrawTexture(shop, 0, 0, WHITE);
                            DrawTexture(valorTexture, WindowWidth - 170, 10, WHITE);
                            DrawText(TextFormat("%d", game.GetValor()), WindowWidth - 100, 20, 30, GOLD);
                            EndDrawing();

                            if (IsKeyPressed(KEY_T)) {
                                WaitTime(0.09);
                                bool inThemes = true;
                                while (inThemes) {
                                    try {
                                        UpdateMusicStream(loadSound);
                                        if (WindowShouldClose()) {
                                            UnloadAllTextures();
                                            UnloadMusicStream(loadSound);
                                            return false;
                                        }

                                        BeginDrawing();
                                        ClearBackground(BLACK);
                                        DrawTexture(themes, 0, 0, WHITE);
                                        DrawTexture(valorTexture, WindowWidth - 170, 10, WHITE);
                                        DrawText(TextFormat("%d", game.GetValor()), WindowWidth - 100, 20, 30, GOLD);
                                        EndDrawing();

                                        if (IsKeyPressed(KEY_A)) {
                                            game.SetCurrentTheme(1);
                                            game.SaveGameState();
                                            WaitTime(0.09);
                                        }
                                        else if (IsKeyPressed(KEY_B)) {
                                            if (game.GetUnlockedThemes() < 2 && game.GetValor() >= 500) {
                                                game.SetValor(game.GetValor() - 500);
                                                game.SetUnlockedThemes(2);
                                            }
                                            if (game.GetUnlockedThemes() >= 2) {
                                                game.SetCurrentTheme(2);
                                            }
                                            game.SaveGameState();
                                            WaitTime(0.09);
                                        }
                                        else if (IsKeyPressed(KEY_BACKSPACE)) {
                                            inThemes = false;
                                            WaitTime(0.09);
                                        }
                                    }
                                    catch (const exception& e) {
                                        TraceLog(LOG_ERROR, "Themes menu error: %s", e.what());
                                        inThemes = false;
                                    }
                                }
                            }
                            else if (IsKeyPressed(KEY_S)) {
                                WaitTime(0.09);
                                bool inSkins = true;
                                while (inSkins) {
                                    try {
                                        UpdateMusicStream(loadSound);
                                        if (WindowShouldClose()) {
                                            UnloadAllTextures();
                                            UnloadMusicStream(loadSound);
                                            return false;
                                        }

                                        BeginDrawing();
                                        ClearBackground(BLACK);
                                        DrawTexture(skins, 0, 0, WHITE);
                                        DrawTexture(valorTexture, WindowWidth - 170, 10, WHITE);
                                        DrawText(TextFormat("%d", game.GetValor()), WindowWidth - 100, 20, 30, GOLD);
                                        EndDrawing();

                                        if (IsKeyPressed(KEY_A)) {
                                            game.SetCurrentSkin(1);
                                            game.SaveGameState();
                                            WaitTime(0.09);
                                        }
                                        else if (IsKeyPressed(KEY_B)) {
                                            if (game.GetUnlockedSkins() < 2 && game.GetValor() >= 1000) {
                                                game.SetValor(game.GetValor() - 1000);
                                                game.SetUnlockedSkins(2);
                                            }
                                            if (game.GetUnlockedSkins() >= 2) {
                                                game.SetCurrentSkin(2);
                                            }
                                            game.SaveGameState();
                                            WaitTime(0.09);
                                        }
                                        else if (IsKeyPressed(KEY_C)) {
                                            if (game.GetUnlockedSkins() >= 2 && game.GetUnlockedSkins() < 3 && game.GetValor() >= 2000) {
                                                game.SetValor(game.GetValor() - 2000);
                                                game.SetUnlockedSkins(3);
                                            }
                                            if (game.GetUnlockedSkins() >= 3) {
                                                game.SetCurrentSkin(3);
                                            }
                                            game.SaveGameState();
                                            WaitTime(0.09);
                                        }
                                        else if (IsKeyPressed(KEY_BACKSPACE)) {
                                            inSkins = false;
                                            WaitTime(0.09);
                                        }
                                    }
                                    catch (const exception& e) {
                                        TraceLog(LOG_ERROR, "Skins menu error: %s", e.what());
                                        inSkins = false;
                                    }
                                }
                            }
                            else if (IsKeyPressed(KEY_A)) {
                                WaitTime(0.09);
                                bool inAds = true;
                                while (inAds) {
                                    try {
                                        UpdateMusicStream(loadSound);
                                        if (WindowShouldClose()) {
                                            UnloadAllTextures();
                                            UnloadMusicStream(loadSound);
                                            return false;
                                        }

                                        BeginDrawing();
                                        ClearBackground(BLACK);
                                        DrawTexture(ads, 0, 0, WHITE);
                                        DrawTexture(valorTexture, WindowWidth - 170, 10, WHITE);
                                        DrawText(TextFormat("%d", game.GetValor()), WindowWidth - 100, 20, 30, GOLD);
                                        EndDrawing();

                                        if (IsKeyPressed(KEY_D)) {
                                            if (game.GetAdsEnabled() && game.GetValor() >= 2999) {
                                                game.SetValor(game.GetValor() - 2999);
                                                game.SetAdsEnabled(false);
                                                game.SaveGameState();
                                            }
                                            WaitTime(0.09);
                                        }
                                        else if (IsKeyPressed(KEY_BACKSPACE)) {
                                            inAds = false;
                                            WaitTime(0.09);
                                        }
                                    }
                                    catch (const exception& e) {
                                        TraceLog(LOG_ERROR, "Ads menu error: %s", e.what());
                                        inAds = false;
                                    }
                                }
                            }
                            else if (IsKeyPressed(KEY_BACKSPACE)) {
                                inShop = false;
                                WaitTime(0.09);
                            }
                        }
                        catch (const exception& e) {
                            TraceLog(LOG_ERROR, "Shop menu error: %s", e.what());
                            inShop = false;
                        }
                    }
                }
            }
            catch (const exception& e) {
                TraceLog(LOG_ERROR, "Main menu error: %s", e.what());
                if (WindowShouldClose()) {
                    UnloadAllTextures();
                    UnloadMusicStream(loadSound);
                    return false;
                }
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Start animation failed: %s", e.what());
        try {
            UnloadAllTextures();
            if (loadSound.ctxData != nullptr) {
                UnloadMusicStream(loadSound);
            }
        }
        catch (...) {
            // If cleanup fails, there's nothing more we can do
        }
        return false;
    }
}
