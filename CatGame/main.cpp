#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    sf::Texture _playerTexture, _stickTexture;
    sf::Sprite _player, _stick;
    sf::Font font;
    sf::Text scoreTxt;
    sf::Music music;
    
    bool _movingUp, _movingLeft, _movingDown, _movingRight;
    int score;
};

Game::Game():
_window(sf::VideoMode(640,480), "CatGame"),
_player(),
_playerTexture(),
_stickTexture(),
_movingUp(),
_movingLeft(),
_movingDown(),
_movingRight(),
font(),
score(),
scoreTxt(),
music()
{
    if (!_playerTexture.loadFromFile(resourcePath() + "player.png") ){
        //errno
    }
    
    if (!_stickTexture.loadFromFile(resourcePath() + "stick.png") ){
        //errno
    }
    
    if(!font.loadFromFile(resourcePath() + "Arial.ttf") ) {
        //errno
    }
    
    if(!music.openFromFile(resourcePath() + "music.ogg") ) {
        //errno
    }
    
    _player.setTexture(_playerTexture);
    _stick.setTexture(_stickTexture);
    _player.setPosition(100.f, 100.f);
    _stick.setPosition(rand() % 630-35, rand() % 470-35);
    _window.setFramerateLimit(50);
    music.setLoop(true);
    
    
    scoreTxt.setFont(font);
    scoreTxt.setCharacterSize(24);
    scoreTxt.setString("Score: " + std::to_string(score));
    scoreTxt.setColor(sf::Color::White);
}

void Game::run() { //This is
    music.play();
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
                //Nothing to do here;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool pressed) {
    /*If clause for the player keyboard events (derp) */

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
    sf::Vector2f movement{0.f, 0.f};
   
    float speed{5.f}; //speed of the player

    if(_movingUp && _player.getPosition().y > 0)
        movement.y -= speed; //up
    if(_movingLeft && _player.getPosition().x > 0)
        movement.x -= speed; //left
    if(_movingDown && _player.getPosition().y < (480 - static_cast<int>(_player.getGlobalBounds().height)))
        movement.y += speed; //down
    if(_movingRight && _player.getPosition().x < (640 - static_cast<int>(_player.getGlobalBounds().width)))
        movement.x += speed; //right
    
    //Pythagoras or something
    if (movement.x != 0.f && movement.y != 0.f)
        movement /= std::sqrt(2.f);
    
    _player.move(movement); //move the player
    
    /* collision */
    if(_player.getGlobalBounds().intersects(_stick.getGlobalBounds())) {
        score++;
        scoreTxt.setString("Score: " + std::to_string(score));
        _stick.setPosition(std::rand() % (640 - static_cast<int>(_stick.getGlobalBounds().width)),
                           std::rand() % (480 - static_cast<int>(_stick.getGlobalBounds().height)));
    }

}

void Game::render() {
    _window.clear();
    _window.draw(_stick);
    _window.draw(_player);
    _window.draw(scoreTxt);
    _window.display();
}

int main()
{
    std::srand(time(nullptr)); //Seed random number generator
    Game cats{};
    cats.run();
}
