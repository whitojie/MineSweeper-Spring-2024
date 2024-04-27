#include "board.h"
#include <iostream>
#include <fstream>
#include "button.h"
#include <cstdlib>
#include <queue>
#include "welcome.h"

using namespace std;
using namespace sf;

// Constructor
Board::Board() {
    gamePaused = false;
    colCount = 0;
    rowCount = 0;
    mineCount = 0;

    // Load configuration from file
    ifstream configFile("config.cfg");
    if (configFile.is_open()) {
        configFile >> colCount >> rowCount >> mineCount;
        configFile.close();
        cout << "Columns: " << colCount << " Rows: " << rowCount << " Mines: " << mineCount << endl;
    } else {
        cerr << "Error: Unable to open config file" << endl;
        return;
    }


    int height = rowCount * 32;
    int width = colCount * 32;
    window.create(VideoMode(width, height + 100), "Hello Window");

    // Load textures
    if (!tileHiddenTexture.loadFromFile("images/tile_hidden.png")) {
        cout << "Could not load tile texture" << endl;
        return;
    }
    if (!tileRevealedTexture.loadFromFile("images/tile_revealed.png")) {
        cout << "Could not load tile_revealed texture" << endl;
        return;
    }
    if (!pausedTexture.loadFromFile("images/tile_revealed.png")) {
        cout << "Could not load tile_revealed texture" << endl;
        return;
    }
    if (!flagTexture.loadFromFile("images/flag.png")) {
        cout << "Could not load flag texture" << endl;
        return;
    }
    if (!mineTexture.loadFromFile("images/mine.png")) {
        cout << "Could not load mine texture" << endl;
        return;
    }
    // Load number textures
    for (int i = 0; i < 8; i++) {
        string filename = "images/number_" + to_string(i + 1) + ".png";
        if (!numberTextures[i].loadFromFile(filename)) {
            cout << "Could not load number texture: " << filename << endl;
            return;
        }
    }

    // initialize revealed, flagged, and mines
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            revealed[i][j] = false;
            flagged[i][j] = false;
        }
    }

    // initialize all tiles for rowCount and colCount
    for (int i = 0; i < colCount; i++) {
        vector<Tile> row;
        for (int j = 0; j < rowCount; j++) {
            Tile tile(tileHiddenTexture, tileRevealedTexture, flagTexture, numberTextures, mineTexture, pausedTexture, Vector2f(i * 32, j * 32));
            row.push_back(tile);
            if (tile.getState() >= TileState::Number1 && tile.getState() <= TileState::Number8) {
                numberTiles.push_back(&row.back()); // Push back the address of number tiles
            }
        }
        tiles.push_back(row);
    }

    previousTileState.resize(colCount, vector<TileState>(rowCount, TileState::Hidden));

    placeMines();
}


void Board::restartGame() {
    // Clear the current board
    clearBoard();

    // Reset revealed and flagged arrays
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            revealed[i][j] = false;
            flagged[i][j] = false;
        }
    }

    timer.reset();


    // Place new mines
    placeMines();
}


void Board::clearBoard() {
    for (int i = 0; i < colCount; ++i) {
        for (int j = 0; j < rowCount; ++j) {
            bombs[i][j] = false;
            tiles[i][j].changeTile(TileState::Hidden);
        }
    }
}


void Board::placeMines() {
    // Initialize bombs
    bombs.resize(colCount);
    for (int i = 0; i < colCount; ++i) {
        bombs[i].resize(rowCount, false);
    }

    int minesPlaced = 0;
    while (minesPlaced < mineCount) {
        int x = rand() % colCount;
        int y = rand() % rowCount;
        if (!bombs[x][y]) {
            bombs[x][y] = true;
            minesPlaced++;
        }
    }

    cout << "The mines are placed" << endl;
    calculateAdjacentMines();
}


int Board::getMineCount() const {
    return mineCount;
}


int Board::getFlagCount() {
    int flagCount = 0;
    for (int i = 0; i < colCount; ++i) {
        for (int j = 0; j < rowCount; ++j) {
            if (flagged[i][j]) {
                flagCount++;
            }
        }
    }
    return flagCount;
}


void Board::debugBoard() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            if (bombs[i][j]) {
                tiles[i][j].changeTile(TileState::Bomb);
            } else {
                revealed[i][j] = false;  // Hide tiles without bombs
                tiles[i][j].changeTile(TileState::Hidden);
            }
        }
    }
}


void Board::hideBoard() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            revealed[i][j] = false;
            // Change the tile state back to hidden
            tiles[i][j].changeTile(TileState::Hidden);
        }
    }
}


void Board::drawBoard(int width, int height) {
    Button button(window, *this, timer);
    timer.pause();

    // Open the welcome window
    WelcomeScreen welcomeScreen;
    welcomeScreen.run();



    // main event loop
    while (window.isOpen()) {
        timer.resume();
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // get the position of the mouse click
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    // convert mouse position to tile indices
                    int tileX = mousePosition.x / 32;
                    int tileY = mousePosition.y / 32;
                    // if the tile is within bounds and not already revealed, reveal it
                    if (tileX >= 0 && tileX < 25 && tileY >= 0 && tileY < 16 && !revealed[tileX][tileY] && !flagged[tileX][tileY]) {
                        if (!gamePaused) {
                            if (bombs[tileX][tileY]) {
                                loss();
                            } else {
                                // If the clicked tile is not a bomb and has no adjacent mines, recursively reveal adjacent tiles
                                if (tiles[tileX][tileY].getAdjacentMinesCount() == 0) {
                                    recursive(tileX, tileY);
                                }
                                else{
                                    tiles[tileX][tileY].changeTile(TileState::Revealed);
                                    revealed[tileX][tileY]= true;
                                }
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    // get the position of the mouse click
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    // convert mouse position to tile or something
                    int tileX = mousePosition.x / 32;
                    int tileY = mousePosition.y / 32;
                    if (!gamePaused && tileX >= 0 && tileX < 25 && tileY >= 0 && tileY < 16) {
                        if (!flagged[tileX][tileY] && !revealed[tileX][tileY]) {
                            tiles[tileX][tileY].changeTile(TileState::Flagged);
                            flagged[tileX][tileY] = true;
                        } else if (flagged[tileX][tileY]) {
                            tiles[tileX][tileY].changeTile(TileState::Hidden);
                            flagged[tileX][tileY] = false;
                        }
                        cout << "Flag count: " << getFlagCount() << endl;
                    }
                }
                button.handleMouseClick(event.mouseButton);
            }
        }

        // drawing tiles based on their state
        window.clear(Color::White);
        for (int i = 0; i < colCount; i++) {
            for (int j = 0; j < rowCount; j++) {
                tiles[i][j].drawTile(window, revealed[i][j]);
            }
        }

        // Update and draw the timer
        timer.update();
        timer.draw(window);

        button.draw();
        window.display();

        victory();
    }
}


void Board::calculateAdjacentMines() {
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            // Skip if the current tile is a bomb
            if (bombs[i][j]) continue;

            int adjacentMines = 0;

            // Iterate through neighboring tiles
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;

                    // if the neighboring tile is within bounds and contains a bomb
                    if (nx >= 0 && nx < colCount && ny >= 0 && ny < rowCount && bombs[nx][ny]) {
                        adjacentMines++;
                    }
                }
            }

            // Update the adjacent mines count for the current tile
            tiles[i][j].setAdjacentMinesCount(adjacentMines);
        }
    }
}


// Recursive function to reveal tiles
void Board::recursive(int x, int y) {
    if (x < 0 || x >= colCount || y < 0 || y >= rowCount) {
        cout << "No clue what this does" << endl;
        return;
    }

    // If the tile is already revealed, flagged, or has a mine, stop recursion
    if (revealed[x][y] || flagged[x][y] || bombs[x][y]) {
        return;
    }

    tiles[x][y].changeTile(TileState::Revealed);
    revealed[x][y] = true;

    // If the clicked tile is a numbered tile, stop recursion
    if (tiles[x][y].getState() >= TileState::Number1 && tiles[x][y].getState() <= TileState::Number8) {
        return;
    }

    // If the clicked tile has no adjacent mines, recursively reveal adjacent tiles
    if (tiles[x][y].getAdjacentMinesCount() == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx;
                int ny = y + dy;

                // Recursively reveal tiles if they are valid and not numbered
                if (nx >= 0 && nx < colCount && ny >= 0 && ny < rowCount && !(dx == 0 && dy == 0) &&
                    !(tiles[nx][ny].getState() >= TileState::Number1 && tiles[nx][ny].getState() <= TileState::Number8)) {
                    recursive(nx, ny);
                }
            }
        }
    }

}


void Board::pauseBoard() {
    // Pause the timer
    timer.pause();

    // Save the elapsed time before pausing

    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            previousTileState[i][j] = tiles[i][j].getState();
            revealed[i][j] = true;
            tiles[i][j].changeTile(TileState::Paused);
        }
    }
    gamePaused = true;
}

void Board::resumeBoard() {
    // Restore the previous state of each tile
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            tiles[i][j].changeTile(previousTileState[i][j]);
            revealed[i][j] = (previousTileState[i][j] == TileState::Revealed);
        }
    }

    timer.resume();
    gamePaused = false;
}

void Board::victory() {
    allNonMineTilesRevealed = true;

    // Check if all non-mine tiles are revealed
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            if (!bombs[i][j] && !revealed[i][j]) {
                allNonMineTilesRevealed = false; // Found an unrevealed non-mine tile
                break; // No need to continue inner loop for this row
            }
        }
        if (!allNonMineTilesRevealed)
            break;
    }

    // If all non-mine tiles are revealed and the game is not paused, trigger victory
    if (allNonMineTilesRevealed && !gamePaused) {
        cout << "Congratulations! You have won!" << endl;
        gamePaused = true;
        timer.pause();
    }
}


void Board::loss() {
    cout << "You clicked on a mine! Game Over!" << endl;
    timer.pause();
    gamePaused = true;
    mineClicked = true;

    // Reveal all mines
    for (int i = 0; i < colCount; i++) {
        for (int j = 0; j < rowCount; j++) {
            if (bombs[i][j]) {
                revealed[i][j] = true;
                tiles[i][j].changeTile(TileState::Bomb);
            }
        }
    }
}



