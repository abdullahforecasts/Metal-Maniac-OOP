#include "bscs24009_Player.h"

void Player::LoadSoundSafe(Sound& sound, const string& filename) {
    sound = LoadSound(filename.c_str());
    if (sound.stream.buffer == nullptr) {
        throw SoundLoadException(filename);
    }
}

Player::Player(float x, float y) :
    GameObject(x, y), width(PlayerWidth), height(PlayerHeight),
    speed(5), direction(1), currentFrame(0), frameCounter(0), frameSpeed(10),
    isJumping(false), jumpVelocity(0), gravity(1.0f), life(5), currentSkin(0)\
{
    try {
        // Load all skins
        LoadSkin();

        // Set initial texture (hrs for right-facing idle)
        texture = skinTextures[0][0]; // Default to skin 0, right idle
        frameRec = { 0.0f, 0.0f, static_cast<float>(texture.width) / 2.0f, static_cast<float>(texture.height) };
        hitbox = { x + 40.0f, y + 15.0f, static_cast<float>(width) - 60.0f, static_cast<float>(height) - 30.0f };

        LoadSoundSafe(stepSound, "step.mp3");
        LoadSoundSafe(shootSound, "efs.mp3");
        LoadSoundSafe(hurtSound, "ahh.mp3");

        for (int i = 0; i < MaxBullets; ++i) {
            bullets[i] = Bullet(0, 0, 8, 3, 5.0f, false, 1);
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player initialization failed: %s", e.what());
        throw;
    }
}

void Player::SetX(float newX) {
    x = newX;
    hitbox.x = x + 40.0f;
}

void Player::UpdateWithoutMovement() {
    try {
        UpdateJump();
        UpdateBullets();
        UpdateAnimation();
        hitbox.x = x + 40.0f;
        hitbox.y = y + 15.0f;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player update without movement error: %s", e.what());
    }
}

void Player::Reset(float startX, float startY) {
    try {
        x = startX;
        y = startY;
        life = 5;
        isJumping = false;
        jumpVelocity = 0;

        for (int i = 0; i < MaxBullets; ++i) {
            bullets[i].SetActive(false);
        }

        direction = 1;
        currentFrame = 0;
        frameCounter = 0;

        hitbox.x = x + 40.0f;
        hitbox.y = y + 15.0f;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player reset error: %s", e.what());
    }
}

void Player::Update() {
    try {
        HandleInput();
        UpdateJump();
        UpdateBullets();
        UpdateAnimation();
        hitbox.x = x + 40.0f;
        hitbox.y = y + 15.0f;
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player update error: %s", e.what());
    }
}

void Player::Draw() {
    try {
        DrawTextureRec(texture, frameRec, { x, y }, WHITE);

        for (int i = 0; i < MaxBullets; ++i) {
            bullets[i].Draw();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player draw error: %s", e.what());
    }
}

void Player::HandleInput() {
    try {
        if (IsKeyDown(KEY_LEFT)) {
            direction = 0;
            if (x > 0) {
                x -= static_cast<float>(speed);
                PlaySound(stepSound);
            }
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            direction = 1;
            if (x < static_cast<float>(WindowWidth - width)) {
                x += static_cast<float>(speed);
                PlaySound(stepSound);
            }
        }

        if (IsKeyPressed(KEY_SPACE) && !isJumping) {
            isJumping = true;
            jumpVelocity = -15.0f;
            PlaySound(stepSound);
        }

        if (IsKeyPressed(KEY_F)) {
            Shoot();
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player input handling error: %s", e.what());
    }
}

void Player::Shoot() {
    try {
        for (int i = 0; i < MaxBullets; ++i) {
            if (!bullets[i].IsActive()) {
                PlaySound(shootSound);
                bullets[i].SetActive(true);
                bullets[i] = Bullet(
                    x + (direction == 1 ? static_cast<float>(width) : 0.0f),
                    y + (static_cast<float>(height) / 2.0f + 15.0f),
                    8.0f, 3.0f, 5.0f, true,
                    direction == 1 ? 1 : -1
                );
                break;
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player shoot error: %s", e.what());
    }
}

void Player::UpdateJump() {
    try {
        if (isJumping) {
            y += jumpVelocity;
            jumpVelocity += gravity;

            if (y >= static_cast<float>(WindowHeight - height - 140)) {
                y = static_cast<float>(WindowHeight - height - 140);
                isJumping = false;
                jumpVelocity = 0.0f;
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player jump update error: %s", e.what());
    }
}

void Player::UpdateBullets() {
    try {
        for (int i = 0; i < MaxBullets; ++i) {
            bullets[i].Update();

            if (bullets[i].IsActive() &&
                (bullets[i].GetX() < 0 || bullets[i].GetX() > static_cast<float>(WindowWidth))) {
                bullets[i].SetActive(false);
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player bullets update error: %s", e.what());
    }
}

void Player::LoadSkin() {
    try {
        // Skin A (default)
        skinTextures[0][0] = LoadTexture("hrs.png");
        skinTextures[0][1] = LoadTexture("hls.png");
        skinTextures[0][2] = LoadTexture("hrr.png");
        skinTextures[0][3] = LoadTexture("hlr.png");

        // Skin B
        skinTextures[1][0] = LoadTexture("hrs1.png");
        skinTextures[1][1] = LoadTexture("hls1.png");
        skinTextures[1][2] = LoadTexture("hrr1.png");
        skinTextures[1][3] = LoadTexture("hlr1.png");

        // Skin C
        skinTextures[2][0] = LoadTexture("hrs2.png");
        skinTextures[2][1] = LoadTexture("hls2.png");
        skinTextures[2][2] = LoadTexture("hrr2.png");
        skinTextures[2][3] = LoadTexture("hlr2.png");

        // Verify textures loaded correctly
        for (int s = 0; s < 3; s++) {
            for (int d = 0; d < 4; d++) {
                if (skinTextures[s][d].id == 0) {
                    throw TextureLoadException("Player skin texture");
                }
            }
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player skin loading failed: %s", e.what());
        throw;
    }
}

void Player::SetSkin(int skin) {
    try {
        if (skin >= 1 && skin <= 3) {
            currentSkin = skin - 1;
            texture = (direction == 0)
                ? skinTextures[currentSkin][IsKeyDown(KEY_LEFT) ? 3 : 1]
                : skinTextures[currentSkin][IsKeyDown(KEY_RIGHT) ? 2 : 0];

            frameRec.width = static_cast<float>(texture.width) / 12.0f;
            frameRec.height = static_cast<float>(texture.height);
            frameRec.y = 0;
            frameRec.x = 0;
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player set skin error: %s", e.what());
    }
}

void Player::UpdateAnimation() {
    try {
        frameCounter++;
        if (frameCounter >= (60 / frameSpeed)) {
            frameCounter = 0;
            currentFrame++;

            bool isMoving = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT);

            if (IsKeyDown(KEY_LEFT)) {
                texture = skinTextures[currentSkin][3];
                frameRec.width = static_cast<float>(texture.width) / 12.0f;
                if (currentFrame >= 12) currentFrame = 0;
            }
            else if (IsKeyDown(KEY_RIGHT)) {
                texture = skinTextures[currentSkin][2];
                frameRec.width = static_cast<float>(texture.width) / 12.0f;
                if (currentFrame >= 12) currentFrame = 0;
            }
            else {
                if (direction == 1) {
                    texture = skinTextures[currentSkin][0];
                }
                else {
                    texture = skinTextures[currentSkin][1];
                }
                frameRec.width = static_cast<float>(texture.width) / 2.0f;
                if (currentFrame >= 2) currentFrame = 0;
            }

            frameRec.x = static_cast<float>(currentFrame) * (texture.width / (isMoving ? 12.0f : 2.0f));
        }
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player animation update error: %s", e.what());
    }
}

Player::~Player() {
    try {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                if (skinTextures[i][j].id != 0) {
                    UnloadTexture(skinTextures[i][j]);
                }
            }
        }

        UnloadSound(stepSound);
        UnloadSound(shootSound);
        UnloadSound(hurtSound);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player destructor error: %s", e.what());
    }
}

void Player::TakeDamage() {
    try {
        life--;
        PlaySound(hurtSound);
    }
    catch (const exception& e) {
        TraceLog(LOG_ERROR, "Player take damage error: %s", e.what());
    }
}

int Player::GetLife() const { return life; }
Bullet* Player::GetBullets() { return bullets; }
int Player::GetDirection() const { return direction; }