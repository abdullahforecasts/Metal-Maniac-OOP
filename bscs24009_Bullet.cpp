#include "bscs24009_Bullet.h"

Bullet::Bullet(float x, float y, float outer, float inner,
    float speed, bool active, int dir,
    Color outerCol, Color innerCol)
    : x(x), y(y), outerRadius(outer), innerRadius(inner),
    speed(speed), active(active), direction(dir),
    outerColor(outerCol), innerColor(innerCol)
{
}

void Bullet::Update() {
    if (!active) return;

    try {
        if (direction == 1) { // Right
            x += speed;
        }
        else if (direction == -1) { // Left
            x -= speed;
        }
        else if (direction == 0) { // Up
            y -= speed;
        }
        else if (direction == 2) { // Down
            y += speed;
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Bullet update error: %s", e.what());
    }
}

void Bullet::Draw() {
    if (!active) return;

    try {
        DrawCircle(x, y, outerRadius, outerColor);
        DrawCircle(x, y, innerRadius, innerColor);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Bullet draw error: %s", e.what());
    }
}

bool Bullet::IsActive() const { return active; }
void Bullet::SetActive(bool state) { active = state; }
float Bullet::GetX() const { return x; }
float Bullet::GetY() const { return y; }
float Bullet::GetOuterRadius() const { return outerRadius; }

bool Bullet::CheckCollision(const Rectangle& rect) {
    try {
        return CheckCollisionCircleRec({ x, y }, outerRadius, rect);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Bullet collision check error: %s", e.what());
        return false;
    }
}

bool Bullet::CheckCollision(const Bullet& other) {
    try {
        return CheckCollisionCircles({ x, y }, outerRadius,
            { other.x, other.y }, other.outerRadius);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Bullet-Bullet collision error: %s", e.what());
        return false;
    }
}