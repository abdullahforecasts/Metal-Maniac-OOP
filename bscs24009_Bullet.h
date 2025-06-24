#pragma once
#include "bscs24009_GameExceptions.h"
#include <raylib.h>
using namespace std;

class Bullet {
protected:
    float x = 0;
    float y = 0;
    float outerRadius = 8;
    float innerRadius = 3;
    float speed = 5;
    bool active = false;
    int direction = 1; // 1 for right, -1 for left 
    Color outerColor = BLUE;
    Color innerColor = WHITE;

public:
    Bullet(float x = 0, float y = 0, float outer = 8, float inner = 3,
        float speed = 5, bool active = false, int dir = 1,
        Color outerCol = BLUE, Color innerCol = WHITE);

    void Update();
    void Draw();
    bool IsActive() const;
    void SetActive(bool state);
    float GetX() const;
    float GetY() const;
    float GetOuterRadius() const;
    bool CheckCollision(const Rectangle& rect);
    bool CheckCollision(const Bullet& other);
};