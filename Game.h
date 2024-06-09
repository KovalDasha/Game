//#ifndef GAME_H
//#define GAME_H

//#endif // GAME_H

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Bird.h"
#include "Pipe.h"
#include "ScoreManager.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void loadResources();
    void saveGameState();
    void loadGameState();

    sf::RenderWindow window;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    Bird* bird;
    sf::Clock clock;
    ScoreManager scoreManager;

    sf::Font font;
    sf::Text scoreText;
};

