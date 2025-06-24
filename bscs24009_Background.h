#pragma once
#include "bscs24009_Constants.h"
#include "bscs24009_GameExceptions.h"
#include <raylib.h>
using namespace std;

class Background {
private:
    Texture2D* currentTextures[2];
    Texture2D defaultTextures[2];
    Texture2D premiumTextures[2];
    int positions[2];
    int width, height;

public:
    Background();
    ~Background();

    void SetTheme(int theme);
    void Update(int scrollSpeed);
    void Draw();
    int GetScrollSpeed() const;
};