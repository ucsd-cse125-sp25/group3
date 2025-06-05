#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
    engine.deinit();  // Properly shutdown audio engine
}

bool AudioManager::init() {
    SoLoud::result res = engine.init();
    if (res != SoLoud::SO_NO_ERROR) {
        std::cerr << "Audio init failed with code: " << res << std::endl;
        return false;
    }
    return true;
}

void AudioManager::loadSound(const std::string& id, const std::string& filename) {
    // Use emplace to construct in-place in the map (no move!)
    auto& wav = sounds[id];  // creates reference to map entry
    SoLoud::result res = wav.load(filename.c_str());

    if (res != SoLoud::SO_NO_ERROR) {
        std::cerr << "Failed to load sound: " << filename << std::endl;
        sounds.erase(id);  // clean up if failed
    }
}

void AudioManager::playSound(const std::string& id) {
    auto it = sounds.find(id);
    if (it != sounds.end()) {
        engine.play(it->second);
    } else {
        std::cerr << "Sound ID not found: " << id << std::endl;
    }
}
