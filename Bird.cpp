#include "Bird.h"

Bird::Bird()
    : velocity(0), gravity(1000.f), flapStrength(-350.f) {
    texture.loadFromFile("C:/Users/koval/OneDrive/Dokumenty/Git/Game/bird.png");
    sprite.setTexture(texture);
    sprite.setPosition(100, 300);
}

void Bird::flap() {
    velocity = flapStrength;
}

void Bird::update(sf::Time deltaTime) {
    velocity += gravity * deltaTime.asSeconds();
    sprite.move(0, velocity * deltaTime.asSeconds());

    if (sprite.getPosition().y < 0)
        sprite.setPosition(sprite.getPosition().x, 0);
    if (sprite.getPosition().y + sprite.getGlobalBounds().height > 600)
        sprite.setPosition(sprite.getPosition().x, 600 - sprite.getGlobalBounds().height);
}

sf::FloatRect Bird::getBounds() const {
    return sprite.getGlobalBounds();
}

void Bird::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Bird::getPosition() const {
    return sprite.getPosition();
}

void Bird::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
