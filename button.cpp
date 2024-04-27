#include <fstream>
#include "button.h"

Button::Button(RenderWindow &win, Board &b, Timer &t) : window(win), board(b), timer(t) {
    leaderboard.readLines();
    float rowCount, colCount, mineCount;
    ifstream configFile("config.cfg");
    if (configFile.is_open()) {
        configFile >> colCount >> rowCount >> mineCount;
        configFile.close();
    } else {
        cerr << "Error: Unable to open config file" << endl;
        return;
    }

    // Load debug texture
    if (!faceHappyTexture.loadFromFile("images/face_happy.png")) {
        std::cerr << "Error: Could not load face_happy texture." << std::endl;
        return;
    }
    if (!faceWinTexture.loadFromFile("images/face_win.png")) {
        std::cerr << "Error: Could not load face_win texture." << std::endl;
        return;
    }
    if (!faceLoseTexture.loadFromFile("images/face_lose.png")) {
        std::cerr << "Error: Could not load face_lose texture." << std::endl;
        return;
    }
    if (!pauseTexture.loadFromFile("images/pause.png")) {
        std::cerr << "Error: Could not load pause texture." << std::endl;
        return;
    }
    if (!playTexture.loadFromFile("images/play.png")) {
        std::cerr << "Error: Could not load play texture." << std::endl;
        return;
    }
    if (!digitsTexture.loadFromFile("images/digits.png")) {
        std::cerr << "Error: Could not load digits texture." << std::endl;
        return;
    }
    if (!debugTexture.loadFromFile("images/debug.png")) {
        std::cerr << "Error: Could not load debug texture." << std::endl;
        return;
    }
    if (!leaderTexture.loadFromFile("images/leaderboard.png")) {
        std::cerr << "Error: Could not load debug texture." << std::endl;
        return;
    }

    debugSprite.setTexture(debugTexture);
    debugSprite.setPosition(colCount * 32 - 304, rowCount * 32 + 16);

    pauseSprite.setTexture(pauseTexture);
    pauseSprite.setPosition(colCount * 32 - 240, rowCount * 32 + 16);

    faceHappySprite.setTexture(faceHappyTexture);
    faceHappySprite.setPosition((colCount * 32)/2 - 32, rowCount * 32 + 16);

    leaderSprite.setTexture(leaderTexture);
    leaderSprite.setPosition(colCount * 32 - 176, rowCount * 32 + 16);

    digitsSprite.setPosition(33, rowCount * 32 + 16);
    digitsSprite.setTexture(digitsTexture);
}

void Button::draw() {
    window.draw(debugSprite);
    window.draw(faceHappySprite);
    window.draw(pauseSprite);
    window.draw(leaderSprite);

    float rowCount, colCount, mineCount;
    ifstream configFile("config.cfg");
    if (configFile.is_open()) {
        configFile >> colCount >> rowCount >> mineCount;
        configFile.close();
    } else {
        cerr << "Error: Unable to open config file" << endl;
        return;
    }

    int remainingMines = board.getMineCount() - board.getFlagCount();
    string mineCountString = to_string(abs(remainingMines));

    if (remainingMines < 0) {
        // Draw minus sign
        Sprite minusSprite(digitsTexture);
        minusSprite.setTextureRect(IntRect(10 * 21, 0, 21, 32)); // Position of minus sign in digits.png
        minusSprite.setPosition(5, 32*(rowCount + 0.5) + 16);
        window.draw(minusSprite);
    }

    for (int i = 0; i < mineCountString.size(); i++) {
        digitsSprite.setTextureRect(IntRect((mineCountString[i] - '0') * 21, 0, 21, 32));
        digitsSprite.setPosition(26 + (remainingMines < 0 ? 21 : 0) + i * 21, 32*(rowCount + 0.5) + 16);
        window.draw(digitsSprite);
    }

    if (board.hasAllNonMineTilesRevealed()) {
        faceHappySprite.setTexture(faceWinTexture); // Change to win face texture on victory
    }
    else if (board.clickedMine() && board.gamePaused) {
        faceHappySprite.setTexture(faceLoseTexture); // Change to lose face texture if a mine is clicked
    }
    else {
        faceHappySprite.setTexture(faceHappyTexture); // Reset to happy face texture
    }
}


void Button::handleMouseClick(Event::MouseButtonEvent event) {
    if (event.button == Mouse::Left) {
        if (board.clickedMine()) {
            if (faceHappySprite.getGlobalBounds().contains(event.x, event.y)) {
                board.restartGame(); // Restart the game
                if (board.gamePaused) {
                    pauseSprite.setTexture(pauseTexture); // Set pause button sprite to play
                    board.resumeBoard(); // Unpause the game
                    timer.resume(); // Resume the timer
                    faceHappySprite.setTexture(faceHappyTexture); // Change face sprite to happy face texture
                }
            }
            if (leaderSprite.getGlobalBounds().contains(event.x, event.y)) {
                leaderboardOpen = true;
                RenderWindow leaderboardWindow(sf::VideoMode(400, 300), "Leaderboard");
                while (leaderboardWindow.isOpen()) {
                    Event leaderboardEvent{};
                    while (leaderboardWindow.pollEvent(leaderboardEvent)) {
                        if (leaderboardEvent.type == sf::Event::Closed)
                            leaderboardWindow.close();
                    }
                    leaderboardWindow.clear(sf::Color::White);
                    leaderboard.draw(leaderboardWindow);
                }
                leaderboardWindow.display();
            }
        } else { // If a mine is not clicked, allow clicks on all buttons
            if (debugSprite.getGlobalBounds().contains(event.x, event.y)) {
                if (!board.gamePaused) {
                    toggleMode();
                    if (!revealMode) {
                        board.debugBoard();
                    } else {
                        board.hideBoard();
                    }
                }
            }
            if (faceHappySprite.getGlobalBounds().contains(event.x, event.y)) {
                board.restartGame(); // Restart the game
                faceHappySprite.setTexture(faceHappyTexture); // Change face sprite to happy face texture
                if (board.gamePaused) {
                    pauseSprite.setTexture(pauseTexture); // Set pause button sprite to play
                    board.resumeBoard(); // Unpause the game
                    timer.resume(); // Resume the timer
                }
            }
            if (pauseSprite.getGlobalBounds().contains(event.x, event.y)) {
                togglePause(); // Toggle game paused state
                if (paused) {
                    pauseSprite.setTexture(playTexture); // Set pause button sprite to play
                    board.pauseBoard();
                    timer.pause(); // Pause the timer
                } else {
                    board.resumeBoard();
                    pauseSprite.setTexture(pauseTexture); // Set play button sprite to pause
                    timer.resume(); // Resume the timer
                }
                return;
            }
            if (leaderSprite.getGlobalBounds().contains(event.x, event.y)) {
                leaderboardOpen = true;
                RenderWindow leaderboardWindow(sf::VideoMode(400, 306), "Leaderboard");
                while (leaderboardWindow.isOpen()) {
                    Event leaderboardEvent{};
                    while (leaderboardWindow.pollEvent(leaderboardEvent)) {
                        if (leaderboardEvent.type == sf::Event::Closed)
                            leaderboardWindow.close();
                    }
                    leaderboardWindow.clear(sf::Color::White);
                    leaderboard.draw(leaderboardWindow);
                }
                leaderboardWindow.display();
            }
        }
    }
}




