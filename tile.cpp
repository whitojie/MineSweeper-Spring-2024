#include <SFML/Graphics.hpp>
#include "tile.h"

using namespace std;
using namespace sf;

Tile::Tile(Texture& hiddenTexture, Texture& revealedTexture, Texture& flagTexture,
           Texture numberTextures[8], Texture& mineTexture, Texture& pausedTexture, Vector2f position)
        : state(TileState::Hidden), adjacentMinesCount(0) {
    this->hiddenTexture = hiddenTexture;
    this->revealedTexture = revealedTexture;
    this->flaggedTexture = flagTexture;
    this->mineTexture = mineTexture;
    this->pausedTexture = pausedTexture;


    // Copy number textures from the array to member variables
    for (int i = 0; i < 8; i++) {
        this->numberTextures[i] = numberTextures[i];
    }
    hiddenSprite.setTexture(hiddenTexture);
    hiddenSprite.setPosition(position);
    revealedSprite.setTexture(revealedTexture);
    revealedSprite.setPosition(position);
    flagSprite.setTexture(flagTexture);
    flagSprite.setPosition(position);
    bombSprite.setTexture(mineTexture);
    bombSprite.setPosition(position);
    pausedSprite.setTexture(pausedTexture);
    pausedSprite.setPosition(position);

    for (int i = 0; i < 8; i++){
        numberSprites[i].setTexture(numberTextures[i]);
        numberSprites[i].setPosition(position);
    }
}

void Tile::drawTile(RenderWindow& window, bool isRevealed) {
    if (!isRevealed) {
        window.draw(hiddenSprite); // Draw the hidden tile if it's not revealed
    }
    if (isRevealed) {
        window.draw(revealedSprite);
        if (adjacentMinesCount > 0 && adjacentMinesCount <= 8) {
            window.draw(numberSprites[adjacentMinesCount - 1]);
        }
    }

    if (!isRevealed && state == TileState::Bomb) {
        window.draw(bombSprite);
    }

    if(isRevealed && state == TileState::Bomb){
        window.draw(bombSprite);
    }

    if (state == TileState::Flagged) {
        window.draw(flagSprite);
    }
    if (state == TileState::Paused) {
        window.draw(pausedSprite);
    }


}

void Tile::changeTile(TileState newState) {
    state = newState;
    switch (state) {
        case TileState::Hidden:
            hiddenSprite.setTexture(hiddenTexture);
            break;
        case TileState::Revealed:
            revealedSprite.setTexture(revealedTexture);
            break;
        case TileState::Flagged:
            flagSprite.setTexture(flaggedTexture);
            break;
        case TileState::Bomb:
            bombSprite.setTexture(mineTexture);
            break;
        case TileState::Paused:
            pausedSprite.setTexture(pausedTexture);
            break;
        case TileState::Number1:
            numberSprites[0].setTexture(numberTextures[0]);
            break;
        case TileState::Number2:
            numberSprites[1].setTexture(numberTextures[1]);
            break;
        case TileState::Number3:
            numberSprites[2].setTexture(numberTextures[2]);
            break;
        case TileState::Number4:
            numberSprites[3].setTexture(numberTextures[3]);
            break;
        case TileState::Number5:
            numberSprites[4].setTexture(numberTextures[4]);
            break;
        case TileState::Number6:
            numberSprites[5].setTexture(numberTextures[5]);
            break;
        case TileState::Number7:
            numberSprites[6].setTexture(numberTextures[6]);
            break;
        case TileState::Number8:
            numberSprites[7].setTexture(numberTextures[7]);
            break;
    }
}

int Tile::getAdjacentMinesCount() const {
    return adjacentMinesCount;
}




//if (isRevealed) {
//  window.draw(revealedSprite);
//  if (adjacentMinesCount > 0 && adjacentMinesCount <= 8) {
//      window.draw(numberSprites[adjacentMinesCount - 1]);
//  }
//}

