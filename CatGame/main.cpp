#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#ifdef DONT_USE_RESOURCEPATH
    std::string resourcePath() { return std::string(); }
#else
#include "ResourcePath.hpp"
#endif


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
    sf::Font _font;
    sf::Text _scoreTxt, _timeTxt;
    sf::Music _music;
    sf::SoundBuffer _buffer;
    sf::Sound _sound;
    
    bool _movingUp, _movingLeft, _movingDown, _movingRight;
    int _score, _highScore;
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
_font(),
_score(),
_highScore(),
_scoreTxt(),
_timeTxt(),
_music(),
_buffer(),
_sound()
{
    if (!_playerTexture.loadFromFile(resourcePath() + "player.png") ){
        //errno
    }
    
    if (!_stickTexture.loadFromFile(resourcePath() + "stick.png") ){
        //errno
    }
    
    if(!_font.loadFromFile(resourcePath() + "Arial.ttf") ) {
        //errno
    }
    
    if(!_music.openFromFile(resourcePath() + "music.ogg") ) {
        //errno
    }
    
    if(!_buffer.loadFromFile(resourcePath() + "stick.wav")) {
        //errno
    }
    
    _sound.setBuffer(_buffer);
    _player.setTexture(_playerTexture);
    _stick.setTexture(_stickTexture);
    _player.setPosition(640.f/2 - _player.getGlobalBounds().width, 480.f/2 - _player.getGlobalBounds().height);
    _stick.setPosition(rand() % 630-35, rand() % 470-35);
    _window.setFramerateLimit(50);
    _music.setLoop(true);
    
    _scoreTxt.setFont(_font);
    _scoreTxt.setString("Score: " + std::to_string(_score));
    _scoreTxt.setColor(sf::Color::White);
    _scoreTxt.setPosition(10.f, 10.f);
    
    _timeTxt.setFont(_font);
    _timeTxt.setColor(sf::Color::White);
    _timeTxt.setPosition(550,10);
}

void Game::run() { //This is
    bool running{false};
    
    sf::Clock clock{};
    sf::Time start{};
    sf::Time end { sf::seconds(90) };
    
    while (_window.isOpen()) {
            processEvents();
        if(running) {
            start = clock.getElapsedTime();
            update();
            
            std::ostringstream t;
            
            int teim = static_cast<int>(end.asSeconds() - start.asSeconds()+1);
            
            t << std::setprecision(2)
                << std::setfill('0') << std::setw(2)
                << teim/60 << ":" << std::setfill('0')
                << std::setw(2) << teim % 60;
            
            _timeTxt.setString(t.str());
           
            render();
            
            
            if(start >= end) {
                running = false;
            }
            
        } else if(start >= end) {
            _music.stop();
            _window.clear();
            
            std::string hss{"SCORE: " + std::to_string(_score) + "\nHIGHSCORE: " + std::to_string(_highScore) + "\nPRESS ENTER TO RESTART"}; //highscore string
            
            if(_score > _highScore) {
                _highScore = _score;
            }
            
            
            sf::Text hsTxt{hss, _font}; //highscorer text
            hsTxt.setPosition(200.f, 200.f);
            
            _window.draw(hsTxt);
            
            _window.display();
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
                _music.play();
                start = clock.getElapsedTime();
                clock.restart();
                _score = 0;
                _scoreTxt.setString("SCORE: " + std::to_string(_score));
                continue;
            }
            
        } else {
            sf::Text prompt{"Press enter to start", _font};
  
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
                running = true;
                _music.play();
                start = clock.getElapsedTime();
                clock.restart();
            }
            
            prompt.setPosition(200.f, 200.f);
            
            _window.clear();
            _window.draw(prompt);
            _window.display();
        }
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
        _sound.play();
        _score++;
        _scoreTxt.setString("Score: " + std::to_string(_score));
        _stick.setPosition(std::rand() % (640 - static_cast<int>(_stick.getGlobalBounds().width)),
                           std::rand() % (480 - static_cast<int>(_stick.getGlobalBounds().height)));
    }

}

void Game::render() {
    _window.clear();
    _window.draw(_stick);
    _window.draw(_player);
    _window.draw(_scoreTxt);
    _window.draw(_timeTxt);
    _window.display();
}

int main()
{
    std::srand(time(nullptr)); //Seed random number generator
    Game cats{};
    cats.run();
}
