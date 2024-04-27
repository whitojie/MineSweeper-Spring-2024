#include "timer.h"
#include <iostream>
using namespace std;

Timer::Timer() : paused(false) {
    // Load digits texture
    if (!digitsTexture.loadFromFile("images/digits.png")) {
        std::cerr << "Failed to load digits texture!" << std::endl;
        return;
    }

    // Create sprites for digits
    minutesTens.setTexture(digitsTexture);
    minutesOnes.setTexture(digitsTexture);
    secondsTens.setTexture(digitsTexture);
    secondsOnes.setTexture(digitsTexture);

    // Set initial texture rectangles
    minutesTens.setTextureRect(sf::IntRect(0, 0, 21, 32));
    minutesOnes.setTextureRect(sf::IntRect(0, 0, 21, 32));
    secondsTens.setTextureRect(sf::IntRect(0, 0, 21, 32));
    secondsOnes.setTextureRect(sf::IntRect(0, 0, 21, 32));

    int numColumns = 25;
    int numRows = 16;
    int xOffset = (numColumns * 32) - 97;
    int yOffset = 32 * (numRows + 0.5) + 16;

    minutesTens.setPosition(xOffset, yOffset);
    minutesOnes.setPosition(xOffset + 21, yOffset);
    secondsTens.setPosition(xOffset + 50, yOffset);
    secondsOnes.setPosition(xOffset + 71, yOffset);

    // Start the clock
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::update() {
    if (!paused) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime) - pausedTime;
        auto totalMilliseconds = elapsed.count();

        // Update timer display
        int minutes = totalMilliseconds / (60 * 1000);
        int seconds = (totalMilliseconds / 1000) % 60;

        int minutesTensDigit = minutes / 10;
        int minutesOnesDigit = minutes % 10;
        int secondsTensDigit = seconds / 10;
        int secondsOnesDigit = seconds % 10;

        minutesTens.setTextureRect(sf::IntRect(21 * minutesTensDigit, 0, 21, 32));
        minutesOnes.setTextureRect(sf::IntRect(21 * minutesOnesDigit, 0, 21, 32));
        secondsTens.setTextureRect(sf::IntRect(21 * secondsTensDigit, 0, 21, 32));
        secondsOnes.setTextureRect(sf::IntRect(21 * secondsOnesDigit, 0, 21, 32));
    }
}

void Timer::draw(sf::RenderWindow& window) {
    window.draw(minutesTens);
    window.draw(minutesOnes);
    window.draw(secondsTens);
    window.draw(secondsOnes);
}

void Timer::pause() {
    if (!paused) {
        paused = true;
        pauseStart = std::chrono::high_resolution_clock::now();
    }
}

void Timer::resume() {
    if (paused) {
        paused = false;
        pauseEnd = std::chrono::high_resolution_clock::now();
        pausedTime += std::chrono::duration_cast<std::chrono::milliseconds>(pauseEnd - pauseStart);
    }
}

string Timer::getTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime) - pausedTime;
    auto totalMilliseconds = elapsed.count();

    int minutes = totalMilliseconds / (60 * 1000);
    int seconds = (totalMilliseconds / 1000) % 60;

    std::string timeString = std::to_string(minutes) + ":";

    if (seconds < 10) {
        timeString += "0";
    }
    timeString += std::to_string(seconds);

    return timeString;
}

void Timer::reset() {
    paused = false;
    pausedTime = std::chrono::milliseconds(0);
    startTime = std::chrono::high_resolution_clock::now();
}

