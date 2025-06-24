#pragma once
#include "bscs24009_GameObject.h"
#include "bscs24009_Bullet.h"
#include <raylib.h>
#include <iostream>
#include "bscs24009_Constants.h"
using namespace std;

class Player : public GameObject {
private:
    int width, height;
    int speed;
    int direction;
    int currentFrame;
    int frameCounter;
    int frameSpeed;
    Rectangle frameRec;
    bool isJumping;
    float jumpVelocity;
    float gravity;
    Bullet bullets[MaxBullets];
    int life;
    Sound stepSound;
    Sound shootSound;
    Sound hurtSound;
    Texture2D skinTextures[3][4]; // [skin][state]
    int currentSkin;

    void LoadSoundSafe(Sound& sound, const string& filename);

public:
    Player(float x, float y);
    ~Player();

    void SetX(float newX);
    void UpdateWithoutMovement();
    void Reset(float startX, float startY);
    void Update() override;
    void Draw() override;
    void HandleInput();
    void Shoot();
    void UpdateJump();
    void UpdateBullets();
    void LoadSkin();
    void SetSkin(int skin);
    void UpdateAnimation();

    void TakeDamage();
    int GetLife() const;
    Bullet* GetBullets();
    int GetDirection() const;
};