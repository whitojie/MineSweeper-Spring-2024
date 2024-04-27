#include "welcome.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

void WelcomeScreen::setText(Text &text, float x, float y){
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
    text.setPosition(Vector2f(x, y));
    }
    
WelcomeScreen::WelcomeScreen() {
    int rowCount = 0;
    int colCount = 0;
    int mineCount = 0;
    ifstream configFile("config.cfg");
    if (configFile.is_open()) {
        configFile >> colCount >> rowCount >> mineCount;
        configFile.close();
        cout << "Columns: " << colCount << " Rows: " << rowCount << " Mines: " << mineCount << endl;
    } else {
        cerr << "Error: Unable to open config file" << endl;
    }
    float width = colCount * 32;
    float height = (rowCount * 32) + 100;

    float textHeight = (height / 2) - 150;
    float textWidth = width / 2;

    float inputHeight = (height / 2) - 75;
    float inputWidth = width / 2;

    float outputHeight = (height / 2) - 45;
    float outputWidth = width / 2;

    this->window.create(VideoMode(width, height), "Welcome to Minesweeper!", Style::Close);
    this->playerName = "";
    this->windowOpen = true;

    this->loadFont(this->font, "font.ttf");

    this->welcomeText.setFont(this->font);
    this->welcomeText.setString("WELCOME TO MINESWEEPER!");
    this->welcomeText.setCharacterSize(24);
    this->welcomeText.setFillColor(Color::White);
    this->setText(this->welcomeText, textWidth, textHeight);

    this->enterNameText.setFont(this->font);
    this->enterNameText.setString("Enter your name:");
    this->enterNameText.setCharacterSize(20);
    this->enterNameText.setFillColor(Color::White);
    this->setText(this->enterNameText, inputWidth, inputHeight);

    this->playerNameText.setFont(this->font);
    this->playerNameText.setCharacterSize(18);
    this->playerNameText.setFillColor(Color::Yellow);
    this->setText(this->playerNameText, outputWidth, outputHeight);
}

void WelcomeScreen::loadFont(Font& font, const string& filename) {
    if (!font.loadFromFile(filename)) {
        cout << "Font not loaded" << endl;
    }
}

void WelcomeScreen::displayWelcomeMessage(RenderWindow& window) {
    window.clear(Color::Blue); // Set background color to blue
    window.draw(welcomeText);
    window.draw(enterNameText);
    window.draw(playerNameText);
    window.display();
}

void WelcomeScreen::handleTextInput(Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if ((event.text.unicode >= 65 && event.text.unicode <= 90) || // Uppercase letters
            (event.text.unicode >= 97 && event.text.unicode <= 122)) { // Lowercase letters
            if (playerName.size() < 10) {
                // Append the entered character to the name
                playerName += static_cast<char>(event.text.unicode);

                // Convert the name to lowercase
                for (char& c : playerName) {
                    c = tolower(c);
                }

                // Capitalize first letter
                if (!playerName.empty()) {
                    playerName[0] = toupper(playerName[0]);
                }

                // Update the displayed name text
                playerNameText.setString(playerName);

                // Get the bounding box of the player name text
                sf::FloatRect bounds = playerNameText.getLocalBounds();

                // Calculate the new position of the text to keep it centered
                float textWidth = bounds.width;
                float windowWidth = window.getSize().x;
                float newPositionX = (windowWidth - textWidth) / 2;

                // Update the position of the text
                playerNameText.setPosition(newPositionX, playerNameText.getPosition().y);

                cout << playerName << endl;
            }
        } else if (event.text.unicode == 8 && !playerName.empty()) {
            // Handle backspace key to remove the last character
            playerName.pop_back();
            playerNameText.setString(playerName);

            // Get the bounding box of the player name text
            sf::FloatRect bounds = playerNameText.getLocalBounds();

            // Calculate the new position of the text to keep it centered
            float textWidth = bounds.width;
            float windowWidth = window.getSize().x;
            float newPositionX = (windowWidth - textWidth) / 2;

            // Update the position of the text
            playerNameText.setPosition(newPositionX, playerNameText.getPosition().y);

            cout << playerName << endl;
        }
    }
}


void WelcomeScreen::handleEnterPress(Event& event) {
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && !playerName.empty()) {
        windowOpen = false;
    }
}

void WelcomeScreen::run() {
    while (window.isOpen() && windowOpen) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::TextEntered) {
                handleTextInput(event);
            } else if (event.type == Event::KeyPressed) {
                handleEnterPress(event);
                if (!windowOpen && event.key.code == Keyboard::Enter) {
                    window.close();
                }
            }
        }
        displayWelcomeMessage(window);
    }

}

