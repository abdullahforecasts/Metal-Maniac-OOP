#include "bscs24009_Game.h"
#include "bscs24009_baadWala.h"

int main() {
    try {
        // Initialize window and audio
        InitWindow(WindowWidth, WindowHeight, "Metal Maniac");
        if (!IsWindowReady()) {
            throw InitializationException("window");
        }

       InitAudioDevice();
        if (!IsAudioDeviceReady()) {
            CloseWindow();
            throw InitializationException("audio device");
        }

        SetTargetFPS(60);

        if (!StartAnimation()) {
            CloseAudioDevice();
            CloseWindow();
            return 0;
        }

        try {
            Game game;
            game.Start();
            game.LoadAdResources();

            while (!game.ShouldClose()) {
                game.Update();
                BeginDrawing();
                ClearBackground(WHITE);
                game.Draw();
                EndDrawing();
            }
        }
        catch (const exception& e) {
            TraceLog(LOG_ERROR, "Game runtime error: %s", e.what());
            cerr << "cat scenes ";
        }

        CloseAudioDevice();
        CloseWindow();
        return 0;
    }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
}