#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#pragma once

using namespace std;
using namespace sf;

enum class TileState {
    Hidden,
    Revealed,
    Flagged,
    Number1,
    Number2,
    Number3,
    Number4,
    Number5,
    Number6,
    Number7,
    Number8,
    Bomb,
    Paused
};

class Tile {
private:
    int adjacentMinesCount;

    Sprite revealedSprite;
    Sprite hiddenSprite;
    Texture hiddenTexture;
    Texture revealedTexture;
    Texture flaggedTexture;
    Texture numberTextures[8];
    Texture mineTexture;
    Vector2f position;
    TileState state;
    Sprite flagSprite;
    Sprite bombSprite;
    Sprite numberSprites[8];
    Sprite pausedSprite;
    Texture pausedTexture;

    vector<Tile*> neighbors; // Neighboring tiles

public:
    Tile(Texture& hiddenTexture, Texture& revealedTexture, Texture& flagTexture,
         Texture numberTextures[8], Texture& mineTexture, Texture& pausedTexture, Vector2f position);

    void drawTile(RenderWindow& window, bool isRevealed);
    void changeTile(TileState newState);

    TileState getState() {
        return state;
    }

    void setAdjacentMinesCount(int count) {
        adjacentMinesCount = count;
    }
    int getAdjacentMinesCount() const;



};
