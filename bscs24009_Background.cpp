#include "bscs24009_Background.h"

Background::Background() :
    width(WindowWidth), height(WindowHeight)
{
    try {
        defaultTextures[0] = LoadTexture("bg1.png");
        defaultTextures[1] = LoadTexture("bg2.png");
        premiumTextures[0] = LoadTexture("nbg1.png");
        premiumTextures[1] = LoadTexture("nbg2.png");

        if (defaultTextures[0].id == 0 || defaultTextures[1].id == 0 ||
            premiumTextures[0].id == 0 || premiumTextures[1].id == 0) {
            throw TextureLoadException("background texture");
        }

        SetTheme(1);
        positions[0] = 0;
        positions[1] = width;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Background initialization failed: %s", e.what());
        throw;
    }
}

Background::~Background() {
    try {
        UnloadTexture(defaultTextures[0]);
        UnloadTexture(defaultTextures[1]);
        UnloadTexture(premiumTextures[0]);
        UnloadTexture(premiumTextures[1]);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Background destructor error: %s", e.what());
    }
}

void Background::SetTheme(int theme) {
    try {
        if (theme == 1) {
            currentTextures[0] = &defaultTextures[0];
            currentTextures[1] = &defaultTextures[1];
        }
        else {
            currentTextures[0] = &premiumTextures[0];
            currentTextures[1] = &premiumTextures[1];
        }

        currentTextures[0]->width = width;
        currentTextures[0]->height = height;
        currentTextures[1]->width = width;
        currentTextures[1]->height = height;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Background set theme error: %s", e.what());
    }
}

void Background::Update(int scrollSpeed) {
    try {
        if (scrollSpeed != 0) {
            positions[0] -= scrollSpeed;
            positions[1] -= scrollSpeed;

            if (positions[0] + width <= 0) {
                positions[0] = positions[1] + width;
            }
            if (positions[1] + width <= 0) {
                positions[1] = positions[0] + width;
            }
            if (positions[0] >= width) {
                positions[0] = positions[1] - width;
            }
            if (positions[1] >= width) {
                positions[1] = positions[0] - width;
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Background update error: %s", e.what());
    }
}

void Background::Draw() {
    try {
        DrawTexture(*currentTextures[0], positions[0], 0, WHITE);
        DrawTexture(*currentTextures[1], positions[1], 0, WHITE);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Background draw error: %s", e.what());
    }
}

int Background::GetScrollSpeed() const {
    return 4;
}