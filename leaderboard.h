#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

class Leader{
private:
    string line1;
    string line2;
    string line3;
    string line4;
    string line5;
    Font font;
    Text leaderboardText;
public:
    Leader();
    void readLines();
    void replaceLines(const string& playerName, const string& time);
    void draw(RenderWindow& window);
    bool betterTime(const string& newTime, const string& oldTime);
};
