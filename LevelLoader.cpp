#include "LevelLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

bool LevelLoader::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    enum class Section { None, Paths, Waves };
    Section currentSection = Section::None;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line == "[Paths]") {
            currentSection = Section::Paths;
            continue;
        } else if (line == "[Waves]") {
            currentSection = Section::Waves;
            continue;
        }

        if (currentSection == Section::Paths) {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;

            std::string val = line.substr(pos + 1);
            std::vector<sf::Vector2f> path;

            std::stringstream ss(val);
            std::string coord;
            while (std::getline(ss, coord, ';')) {
                // Usuń spacje
                coord.erase(std::remove_if(coord.begin(), coord.end(), ::isspace), coord.end());

                int x, y;
                if (sscanf(coord.c_str(), "(%d,%d)", &x, &y) == 2) {
                    path.emplace_back(static_cast<float>(x), static_cast<float>(y));
                }
            }
            paths.push_back(path);
        }

        if (currentSection == Section::Waves) {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;

            std::string key = line.substr(0, pos);
            std::string val = line.substr(pos + 1);

            int waveIndex;
            sscanf(key.c_str(), "Wave%d", &waveIndex);

            std::stringstream ss(val);
            std::string typeStr, pathStr, delayStr;

            std::getline(ss, typeStr, ',');
            std::getline(ss, pathStr, ',');
            std::getline(ss, delayStr);

            EnemySpawn spawn;
            spawn.type = typeStr;
            spawn.pathIndex = std::stoi(pathStr);
            spawn.delay = std::stof(delayStr);

            waveMap[waveIndex].push_back(spawn);
        }
    }

    return true;
}

const std::vector<std::vector<sf::Vector2f>>& LevelLoader::getPaths() const {
    return paths;
}

const std::map<int, std::vector<EnemySpawn>>& LevelLoader::getWaveMap() const {
    return waveMap;
}
