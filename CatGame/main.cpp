#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"

class Game {
public:
    Game();
    void run();
private:
    void processEvents();
    void update();
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool pressed);
    
    sf::RenderWindow _window;
    sf::Texture _playerTexture;
    sf::Texture _stickTexture;
    sf::Sprite _player;
    sf::Sprite _stick;
    
    bool _movingUp, _movingLeft, _movingDown, _movingRight;
};


Game::Game()
: _window(sf::VideoMode(640,480), "CatGame"),
_player(),
_playerTexture(),
_stickTexture(),
_movingUp(),
_movingLeft(),
_movingDown(),
_movingRight()
{
    if (!_playerTexture.loadFromFile(resourcePath() + "player.png") ){
        //errno
    }
    
    if (!_stickTexture.loadFromFile(resourcePath() + "stick.png") ){
        //errno
    }
    
    _player.setTexture(_playerTexture);
    _stick.setTexture(_stickTexture);
    
    _player.setPosition(100.f, 100.f);
    _stick.setPosition(rand() % 640-35, rand() % 480-35);
    
    _window.setFramerateLimit(50);
}

void Game::run() {
    while (_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                _window.close();
                break;
            default:
                break;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool pressed) {
    if(key == sf::Keyboard::Up) {
        _movingUp = pressed;
    } else if(key == sf::Keyboard::Down) {
        _movingDown = pressed;
    } else if(key == sf::Keyboard::Left) {
        _movingLeft = pressed;
    } else if(key == sf::Keyboard::Right) {
        _movingRight = pressed;
    }
}

void Game::update() {
    sf::Vector2f movement(0.f, 0.f);
    float speed{2.f};
    
    if(_movingUp)
        movement.y -= speed;
    else if(_movingLeft)
        movement.x -= speed;
    else if(_movingDown)
        movement.y += speed;
    else if(_movingRight)
        movement.x += speed;
    
    _player.move(movement);
}

void Game::render() {
    _window.clear();
    
    _window.draw(_stick);
    _window.draw(_player);

    _window.display();
}

int main()
{
    srand(time(nullptr));
    Game cats;
    cats.run();
}
