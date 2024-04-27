#include <chrono>
#include "board.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>

using namespace std;
using namespace sf;



class Button {
private:
    Sprite faceHappySprite;
    Texture debugTexture;
    RenderWindow& window;
    Sprite debugSprite;
    Board& board;
    Timer timer;
    Texture faceHappyTexture;
    Texture faceWinTexture;
    Texture faceLoseTexture;
    Texture pauseTexture;
    Texture playTexture;
    Texture digitsTexture;
    Texture leaderTexture;
    Sprite pauseSprite;
    Sprite digitsSprite;
    Sprite leaderSprite;
    bool revealMode = true;
    bool paused = false;
    Leader leaderboard;
    string playerName;
    bool leaderboardOpen{};

public:
    Button(RenderWindow &win, Board &b, Timer &t);

    std::function<void()> onVictoryCallback;
    void setOnVictoryCallback(std::function<void()> callback) { onVictoryCallback = callback; }

    void draw();
    void handleMouseClick(Event::MouseButtonEvent event);
    void toggleMode() {
        revealMode = !revealMode;
    }
    void togglePause(){
        paused = !paused;
    }
    void resumeTimer() {

        timer.resume();
    }
    void pauseTimer() {
        timer.pause();
    }

};
