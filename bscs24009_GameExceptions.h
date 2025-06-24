#pragma once
#include <stdexcept>
#include <string>
using namespace std;
class GameException : public runtime_error {
public:
     GameException(const string& message)
        : runtime_error(message) {}
};

class InitializationException : public GameException {
public:
    InitializationException(const string& component)
        : GameException("Failed to initialize " + component) {
    }
};

class FileLoadException : public GameException {
public:
     FileLoadException(const string& filename)
        : GameException("Failed to load file: " + filename) {
    }
};

class TextureLoadException : public FileLoadException {
public:
     TextureLoadException(const string& filename)
        : FileLoadException("Texture " + filename) {
    }
};

class SoundLoadException : public FileLoadException {
public:
    SoundLoadException(const string& filename)
        : FileLoadException("Sound " + filename) {
    }
};

class MusicLoadException : public FileLoadException {
public:
    MusicLoadException(const string& filename)
        : FileLoadException("Music " + filename) {
    }
};

