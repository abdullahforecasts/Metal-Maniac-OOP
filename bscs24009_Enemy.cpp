#include "bscs24009_Enemy.h"
#include <random>

void Enemy::LoadSoundSafe(Sound& sound, const string& filename) {
    sound = LoadSound(filename.c_str());
    if (sound.stream.buffer == nullptr) {
        throw SoundLoadException(filename);
    }
}

Enemy::Enemy(float x, float y) :
    GameObject(x, y), width(PlayerWidth + 125), height(PlayerHeight), speed(2), direction(1)
{
    try {
        textures[0] = LoadTexture("ers.png");
        textures[1] = LoadTexture("els.png");
        if (textures[0].id == 0 || textures[1].id == 0) {
            throw TextureLoadException("Enemy texture");
        }

        texture = textures[1];
        hitbox = { x + 25.0f, y - 10.0f, static_cast<float>(width) - 125.0f, static_cast<float>(height) - 30.0f };

        LoadSoundSafe(shootSound, "efs.mp3");

        for (int i = 0; i < MaxBullets; ++i) {
            bullets[i] = Bullet(0, 0, 8, 3, 5.0f, false, 1, RED);
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Enemy initialization failed: %s", e.what());
        throw;
    }
}

Enemy::~Enemy() {
    try {
        UnloadTexture(textures[0]);
        UnloadTexture(textures[1]);
        UnloadSound(shootSound);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Enemy destructor error: %s", e.what());
    }
}

void Enemy::Update() {
    throw GameException("Base Enemy Update called - use Update(float) instead");
}

void Enemy::Update(float playerX) {
    try {
        if (playerX > x) {
            direction = 1;
            texture = textures[0];
        }
        else {
            direction = -1;
            texture = textures[1];
        }

        if (playerX > x) {
            x += static_cast<float>(speed);
        }
        else {
            x -= static_cast<float>(speed);
        }

        hitbox.x = x + 25.0f;

        for (int i = 0; i < MaxBullets; ++i) {
            bullets[i].Update();

            if (bullets[i].IsActive() &&
                (bullets[i].GetX() < 0 || bullets[i].GetX() > static_cast<float>(WindowWidth))) {
                bullets[i].SetActive(false);
            }
        }

        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> shootChance(1, 100);

        if (shootChance(gen) == 1) {
            Shoot();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Enemy update error: %s", e.what());
    }
}

void Enemy::Draw() {
    try {
        DrawTexture(texture, static_cast<int>(x), static_cast<int>(y), WHITE);

        for (int i = 0; i < MaxBullets; ++i) {
            bullets[i].Draw();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Enemy draw error: %s", e.what());
    }
}

void Enemy::Shoot() {
    try {
        for (int i = 0; i < MaxBullets; ++i) {
            if (!bullets[i].IsActive()) {
                bullets[i].SetActive(true);
                bullets[i] = Bullet(
                    x + (direction == 1 ? static_cast<float>(width) : -5.0f),
                    y + 50.0f,
                    8.0f, 3.0f, 5.0f, true,
                    direction,
                    direction == 1 ? GREEN : RED
                );
                break;
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Enemy shoot error: %s", e.what());
    }
}

void Enemy::Respawn() {
    try {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> rangeSelector(0, 1);
        uniform_int_distribution<> range1(-3 * WindowWidth / 2, -PlayerWidth - 10);
        uniform_int_distribution<> range2(WindowWidth + 10, 3 * WindowWidth / 2);

        if (rangeSelector(gen) == 0) {
            x = static_cast<float>(range1(gen));
        }
        else {
            x = static_cast<float>(range2(gen));
        }
        y = static_cast<float>(WindowHeight - PlayerHeight - 130);
        hitbox.x = x + 25.0f;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Enemy respawn error: %s", e.what());
    }
}

Bullet* Enemy::GetBullets() { return bullets; }