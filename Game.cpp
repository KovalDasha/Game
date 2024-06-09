#include "Game.h"
#include <fstream>
#include <random>

Game::Game()
    : window(sf::VideoMode(800, 600), "Flappy Bird"), gameState(GameState::Menu), currentBirdIndex(1) {
    loadResources();
    gameObjects.push_back(std::make_unique<Bird>());
    bird = dynamic_cast<Bird*>(gameObjects.back().get());
    bird->setTexture(birdTextures[currentBirdIndex]);
    bird->setGravity(currentBirdIndex);
    gameObjects.push_back(std::make_unique<Pipe>(800, rand() % 300 + 100));
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    menuText.setFont(font);
    menuText.setCharacterSize(36);
    menuText.setFillColor(sf::Color::White);
    menuText.setString("Press SPACE to Start");
    menuText.setPosition(200, 300);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(36);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setString("Game Over! Press SPACE to Restart");
    gameOverText.setPosition(100, 300);

    characterText.setFont(font);
    characterText.setCharacterSize(24);
    characterText.setFillColor(sf::Color::White);
    characterText.setString("Use LEFT/RIGHT to select character");
    characterText.setPosition(150, 500);

    backgroundTexture.loadFromFile("C:/Users/koval/OneDrive/Dokumenty/Git/Game/background.jpg");
    backgroundSprite.setTexture(backgroundTexture);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();

        if (gameState == GameState::Playing) {
            update(clock.restart());
            handleCollisions();
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (gameState == GameState::Menu) {
                if (event.key.code == sf::Keyboard::Space) {
                    gameState = GameState::Playing;
                    clock.restart();
                } else if (event.key.code == sf::Keyboard::Right) {
                    selectNextCharacter();
                } else if (event.key.code == sf::Keyboard::Left) {
                    selectPreviousCharacter();
                }
            } else if (gameState == GameState::Playing) {
                if(currentBirdIndex != 1)
                {
                    if (event.key.code == sf::Keyboard::Space) {
                        bird->flap(1);
                    }
                }
                else
                {
                    if (event.key.code == sf::Keyboard::Up) {
                        bird->flap(1);
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        bird->flap(0);
                    }
                }
            } else if (gameState == GameState::GameOver) {
                if (event.key.code == sf::Keyboard::Space) {
                    resetGame();
                    gameState = GameState::Menu;
                }
            }
        }
        if (gameState == GameState::Playing) {
            if(currentBirdIndex == 1)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    bird->flap(1);  // Или любое другое действие
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    bird->flap(0);  // Или любое другое действие
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    for (auto& obj : gameObjects) {
        obj->update(deltaTime);
    }

    for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
        Pipe* pipe = dynamic_cast<Pipe*>(it->get());
        if (pipe) {
            if (pipe->getBounds().left + pipe->getBounds().width < 0) {
                it = gameObjects.erase(it);
                gameObjects.push_back(std::make_unique<Pipe>(800,(rand() % 2 == 1 ? rand() % 300 + 100 : rand() % 400 - 700)));
                scoreManager.incrementScore();
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    scoreText.setString("Score: " + std::to_string(scoreManager.getScore()));
}

void Game::handleCollisions() {
    for (const auto& obj : gameObjects) {
        Pipe* pipe = dynamic_cast<Pipe*>(obj.get());
        if (pipe && bird->getBounds().intersects(pipe->getBounds())) {
            gameState = GameState::GameOver;
        }
    }
}

void Game::render() {
    window.clear();
    window.draw(backgroundSprite);

    if (gameState == GameState::Playing) {
        for (const auto& obj : gameObjects) {
            window.draw(*obj);
        }
        window.draw(scoreText);
    } else if (gameState == GameState::Menu) {
        window.draw(menuText);
        window.draw(characterText);
        window.draw(*bird);  // Draw the current selected bird
    } else if (gameState == GameState::GameOver) {
        window.draw(gameOverText);
        window.draw(scoreText);
    }

    window.display();
}

void Game::loadResources() {
    font.loadFromFile("C:/Users/koval/OneDrive/Dokumenty/Git/Game/arial.ttf");

    sf::Texture birdTexture1, birdTexture2, birdTexture3;
    birdTexture1.loadFromFile("C:/Users/koval/OneDrive/Dokumenty/Git/Game/bird.png");
    birdTexture2.loadFromFile("C:/Users/koval/OneDrive/Dokumenty/Git/Game/samolet.png");
    birdTexture3.loadFromFile("C:/Users/koval/OneDrive/Dokumenty/Git/Game/airballon.png");

    birdTextures.push_back(birdTexture1);
    birdTextures.push_back(birdTexture2);
    birdTextures.push_back(birdTexture3);
}

void Game::saveGameState() {
    std::ofstream saveFile("C:/Users/koval/OneDrive/Dokumenty/Git/Game/savegame.txt");
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

void Game::resetGame() {
    gameObjects.clear();
    gameObjects.push_back(std::make_unique<Bird>());
    bird = dynamic_cast<Bird*>(gameObjects.back().get());
    bird->setTexture(birdTextures[currentBirdIndex]);
    bird->setGravity(currentBirdIndex);
    gameObjects.push_back(std::make_unique<Pipe>(800, rand() % 400));
    scoreManager.setScore(0);
    scoreText.setString("Score: 0");
}

void Game::selectNextCharacter() {
    currentBirdIndex = (currentBirdIndex + 1) % birdTextures.size();
    bird->setTexture(birdTextures[currentBirdIndex]);
    bird->setGravity(currentBirdIndex);
}

void Game::selectPreviousCharacter() {
    currentBirdIndex = (currentBirdIndex - 1 + birdTextures.size()) % birdTextures.size();
    bird->setTexture(birdTextures[currentBirdIndex]);
    bird->setGravity(currentBirdIndex);
}

