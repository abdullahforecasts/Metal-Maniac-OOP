#include "bscs24009_Jet.h"

Jet::Jet(float x, float y) :
    GameObject(x, y)
{
    try {
        texture = LoadTexture("jet.png");
        if (texture.id == 0) {
            throw TextureLoadException("jet.png");
        }
        hitbox = { x, y, static_cast<float>(JetWidth), static_cast<float>(JetHeight) };

        for (int i = 0; i < MaxJetBullets; ++i) {
            bullets[i] = Bullet(0, 0, 12, 5, 5.0f, false, 2, GOLD);
            
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Jet initialization failed: %s", e.what());
        throw;
    }
}

Jet::~Jet() {
    try {
        UnloadTexture(texture);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Jet destructor error: %s", e.what());
    }
}

void Jet::Update() {
    try {
        x += speed;
        if (x > static_cast<float>(WindowWidth)) {
            x = -3.0f * static_cast<float>(JetWidth);
        }

        for (int i = 0; i < MaxJetBullets; ++i) {
            if (bullets[i].IsActive()) {
                bullets[i].Update();

                if (bullets[i].GetY() > static_cast<float>(WindowHeight)) {
                    bullets[i].SetActive(false);
                }
            }
        }

        DropBullets();
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Jet update error: %s", e.what());
    }
}

void Jet::Draw() {
    try {
        DrawTexture(texture, static_cast<int>(x), static_cast<int>(y), WHITE);

        for (int i = 0; i < MaxJetBullets; ++i) {
            bullets[i].Draw();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Jet draw error: %s", e.what());
    }
}

void Jet::DropBullets() {
    try {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dropChance(1, 100);

        if (dropChance(gen) <= 2) {
            for (int i = 0; i < MaxJetBullets; ++i) {
                if (!bullets[i].IsActive()) {
                    bullets[i].SetActive(true);
                    bullets[i] = Bullet(
                        x + static_cast<float>(JetWidth) / 2.0f,
                        y + static_cast<float>(JetHeight),
                        12.0f, 5.0f, 5.0f, true,
                        2,
                        GOLD
                    );
                    break;
                }
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Jet drop bullets error: %s", e.what());
    }
}

Bullet* Jet::GetBullets() { return bullets; }