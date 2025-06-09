#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

struct EnemySpawn {
    std::string type;
    int pathIndex;
    float delay;
};

class LevelLoader {
public:
    bool loadFromFile(const std::string& filename);

    const std::vector<std::vector<sf::Vector2f>>& getPaths() const;
    const std::map<int, std::vector<EnemySpawn>>& getWaveMap() const;

private:
    std::vector<std::vector<sf::Vector2f>> paths;
    std::map<int, std::vector<EnemySpawn>> waveMap;
};

#endif // LEVELLOADER_H
