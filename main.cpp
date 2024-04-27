#include <iostream>
#include "board.h"
#include "button.h"

using namespace std;

int main(){



    Board board;
    board.drawBoard(800, 612);

    return 0;
}





//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Chrono Clock");
//
//    // Load digits texture
//    sf::Texture digitsTexture;
//    if (!digitsTexture.loadFromFile("images/digits.png")) {
//        return EXIT_FAILURE;
//    }
//
//    // Create sprites for digits
//    sf::Sprite minutesDigit1(digitsTexture);
//    sf::Sprite minutesDigit2(digitsTexture);
//    sf::Sprite secondsDigit1(digitsTexture);
//    sf::Sprite secondsDigit2(digitsTexture);
//
//    // Set initial texture rects for digits
//    minutesDigit1.setTextureRect(sf::IntRect(0, 0, 21, 32));
//    minutesDigit2.setTextureRect(sf::IntRect(0, 0, 21, 32));
//    secondsDigit1.setTextureRect(sf::IntRect(0, 0, 21, 32));
//    secondsDigit2.setTextureRect(sf::IntRect(0, 0, 21, 32));
//
//    // Set initial positions for digits
//    minutesDigit1.setPosition(700 - 21, 16 + 16 * 32);
//    minutesDigit2.setPosition(700, 16 + 16 * 32);
//    secondsDigit1.setPosition(700 + 21, 16 + 16 * 32);
//    secondsDigit2.setPosition(700 + 42, 16 + 16 * 32);
//
//    // Load pause and play button textures
//    sf::Texture pauseTexture;
//    sf::Texture playTexture;
//    if (!pauseTexture.loadFromFile("images/pause.png") || !playTexture.loadFromFile("images/play.png")) {
//        return EXIT_FAILURE;
//    }
//
//    // Create pause and play button sprites
//    sf::Sprite pausePlayButton(pauseTexture);
//    pausePlayButton.setPosition(800 - 240, 16 + 16 * 32);
//
//    // Define clock and time variables
//    auto startTime = std::chrono::high_resolution_clock::now();
//    auto pauseStart = std::chrono::high_resolution_clock::now();
//    auto pauseEnd = std::chrono::high_resolution_clock::now();
//    bool isPaused = false;
//    std::chrono::seconds pausedTime(0);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
//                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//                if (pausePlayButton.getGlobalBounds().contains(mousePos)) {
//                    isPaused = !isPaused;
//                    if (isPaused) {
//                        pauseStart = std::chrono::high_resolution_clock::now();
//                        pausePlayButton.setTexture(playTexture);
//                    }
//                    else {
//                        pauseEnd = std::chrono::high_resolution_clock::now();
//                        pausedTime += std::chrono::duration_cast<std::chrono::seconds>(pauseEnd - pauseStart);
//                        pausePlayButton.setTexture(pauseTexture);
//                    }
//                }
//            }
//        }
//
//        if (!isPaused) {
//            auto currentTime = std::chrono::high_resolution_clock::now();
//            auto totalTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime) - pausedTime;
//
//            // Calculate minutes and seconds
//            int minutes = totalTime.count() / 60;
//            int seconds = totalTime.count() % 60;
//
//            // Update texture rects for digits
//            minutesDigit1.setTextureRect(sf::IntRect(((minutes / 10) * 21), 0, 21, 32));
//            minutesDigit2.setTextureRect(sf::IntRect(((minutes % 10) * 21), 0, 21, 32));
//            secondsDigit1.setTextureRect(sf::IntRect(((seconds / 10) * 21), 0, 21, 32));
//            secondsDigit2.setTextureRect(sf::IntRect(((seconds % 10) * 21), 0, 21, 32));
//        }
//
//        window.clear();
//        window.draw(minutesDigit1);
//        window.draw(minutesDigit2);
//        window.draw(secondsDigit1);
//        window.draw(secondsDigit2);
//        window.draw(pausePlayButton);
//        window.display();
//    }
//
//    return 0;
//}
