#pragma once

#include <soloud.h>
#include <soloud_wav.h>
#include <string>
#include <unordered_map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool init();
    void playSound(const std::string& id);
    void loadSound(const std::string& id, const std::string& filename);

private:
    SoLoud::Soloud engine;  // Declare the engine
    std::unordered_map<std::string, SoLoud::Wav> sounds;
};
