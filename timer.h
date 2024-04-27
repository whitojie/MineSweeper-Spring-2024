#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

class Timer {
public:
    Timer();

    void update();
    void draw(sf::RenderWindow& window);
    void pause();
    void resume();
    std::string getTime();
    void reset();

private:
    sf::Texture digitsTexture;
    sf::Sprite minutesTens;
    sf::Sprite minutesOnes;
    sf::Sprite secondsTens;
    sf::Sprite secondsOnes;

    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point pauseStart;
    std::chrono::high_resolution_clock::time_point pauseEnd;
    std::chrono::milliseconds pausedTime;
    bool paused;
};