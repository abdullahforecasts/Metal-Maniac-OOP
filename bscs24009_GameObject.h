#pragma once
#include <raylib.h>

class GameObject {
protected:
    float x, y;
    Rectangle hitbox;
    Texture2D texture;

public:
    GameObject(float x, float y);
    virtual ~GameObject() {}

    virtual void Update() = 0;
    virtual void Draw() = 0;

    Rectangle GetHitbox() const;
    float GetX() const;
    float GetY() const;
};