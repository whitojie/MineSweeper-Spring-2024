#include "tile.h"
#include "timer.h"
#include "leaderboard.h"
#include <SFML/Graphics.hpp>

#pragma once

class Board {
public:
    // Constructor
    Board();
    void recursive(int x, int y);
    void placeMines();
    void drawBoard(int width, int height);
    void debugBoard();
    void hideBoard();
    void calculateAdjacentMines();
    int getMineCount() const;
    int getFlagCount();
    void restartGame();
    void clearBoard();
    void pauseBoard();
    void resumeBoard();
    void victory();
    void loss();
    bool hasAllNonMineTilesRevealed() const {
        return allNonMineTilesRevealed;
    }
    bool clickedMine() const {
        return mineClicked;
    }



private:
    int colCount;
    int rowCount;
    int mineCount;
    friend class Button;
    RenderWindow window;
    Texture tileHiddenTexture;
    Texture tileRevealedTexture;
    Texture flagTexture;
    Texture mineTexture;
    Texture pausedTexture;
    Texture numberTextures[8];
    bool revealed[25][16]{};
    bool flagged[25][16]{};
    bool gamePaused;
    Timer timer;
    bool timerPaused = false;
    Clock clock;
    Time elapsedTime;
    bool allNonMineTilesRevealed = false;
    bool mineClicked = false;





    vector<vector<Tile>> tiles;
    vector<vector<bool>> bombs;
    vector<Tile* > numberTiles;
    vector<vector<TileState>> previousTileState;

};