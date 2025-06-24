#pragma once
#include "bscs24009_GameObject.h"
#include "bscs24009_Bullet.h"
#include <raylib.h>
#include <random>
#include "bscs24009_Constants.h"

class Jet : public GameObject {
private:
    float speed = 3.0f;
    Bullet bullets[MaxJetBullets];

public:
    Jet(float x, float y);
    ~Jet();

    void Update() override;
    void Draw() override;
    void DropBullets();
    Bullet* GetBullets();
};