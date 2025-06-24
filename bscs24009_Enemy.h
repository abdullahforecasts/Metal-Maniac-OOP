#pragma once
#include "bscs24009_GameObject.h"
#include "bscs24009_GameExceptions.h"
#include "bscs24009_Constants.h"
#include <raylib.h>
#include "bscs24009_Bullet.h"

class Enemy : public GameObject {
private:
    int width, height;
    int speed;
    int direction;
    Texture2D textures[2];
    Bullet bullets[MaxBullets];
    Sound shootSound;

    void LoadSoundSafe(Sound& sound, const string& filename);

public:
    Enemy(float x, float y);
    ~Enemy();

    void Update() override;
    void Update(float playerX);
    void Draw() override;
    void Shoot();
    void Respawn();
    Bullet* GetBullets();
};