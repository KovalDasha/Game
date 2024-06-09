#include "Game.h"
#include "Pipe.h"
#include <fstream>
//#include <random>

Game::Game()
    : window(sf::VideoMode(800, 600), "Flappy Bird") {
    loadResources();
    gameObjects.push_back(std::make_unique<Bird>());
    bird = dynamic_cast<Bird*>(gameObjects.back().get());
    gameObjects.push_back(std::make_unique<Pipe>(800, rand() % 400));
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update(clock.restart());
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            bird->flap();
    }
}

void Game::update(sf::Time deltaTime) {
    for (auto& obj : gameObjects) {
        obj->update(deltaTime);
    }

    // Check for collisions and out of screen pipes
    for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
        Pipe* pipe = dynamic_cast<Pipe*>(it->get());
        if (pipe) {
            if (pipe->getBounds().left + pipe->getBounds().width < 0) {
                it = gameObjects.erase(it);
                gameObjects.push_back(std::make_unique<Pipe>(800, rand() % 400));
                scoreManager.incrementScore();
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }

    // Update score text
    scoreText.setString("Score: " + std::to_string(scoreManager.getScore()));
}

void Game::render() {
    window.clear();
    for (const auto& obj : gameObjects) {
        window.draw(*obj);
    }
    window.draw(scoreText);
    window.display();
}

void Game::loadResources() {
    font.loadFromFile("arial.ttf");
}

void Game::saveGameState() {
    std::ofstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        saveFile << bird->getPosition().x << " " << bird->getPosition().y << " " << scoreManager.getScore() << "\n";
        saveFile.close();
    }
}

void Game::loadGameState() {
    std::ifstream loadFile("savegame.txt");
    if (loadFile.is_open()) {
        float x, y;
        int score;
        loadFile >> x >> y >> score;
        bird->setPosition(x, y);
        scoreManager.setScore(score);
        loadFile.close();
    }
}
