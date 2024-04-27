#include "leaderboard.h"
#include <sstream>

Leader::Leader() {
    if (!font.loadFromFile("font.ttf")) {
        cerr << "Error loading font.ttf" << endl;
    }

    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(18);
    leaderboardText.setFillColor(Color::White);
}

void Leader::readLines() {
    ifstream file("leaderboard.txt");
    if (file.is_open()) {
        getline(file, line1);
        getline(file, line2);
        getline(file, line3);
        getline(file, line4);
        getline(file, line5);
        file.close();
    }
}

bool Leader::betterTime(const string& newTime, const string& oldTime) {
    int newMinutes, newSeconds, oldMinutes, oldSeconds;
    char dummy;
    stringstream newTimeStream(newTime), oldTimeStream(oldTime);
    newTimeStream >> newMinutes >> dummy >> newSeconds;
    oldTimeStream >> oldMinutes >> dummy >> oldSeconds;

    if (newMinutes < oldMinutes || (newMinutes == oldMinutes && newSeconds < oldSeconds))
        return true;

    return false;
}

void Leader::replaceLines(const string& playerName, const string& time) {
    if (betterTime(time, line5)) {
        line5 = playerName + " " + time;
    } else if (betterTime(time, line4)) {
        line5 = line4;
        line4 = playerName + " " + time;
    } else if (betterTime(time, line3)) {
        line4 = line3;
        line3 = playerName + " " + time;
    } else if (betterTime(time, line2)) {
        line3 = line2;
        line2 = playerName + " " + time;
    } else if (betterTime(time, line1)) {
        line2 = line1;
        line1 = playerName + " " + time;
    }

    ofstream file("leaderboard.txt");
    if (file.is_open()) {
        file << line1 << endl;
        file << line2 << endl;
        file << line3 << endl;
        file << line4 << endl;
        file << line5 << endl;
        file.close();
    }
}

void Leader::draw(RenderWindow& window) {
    // Clear the window
    window.clear(Color::Blue);

    // Draw leaderboard text
    leaderboardText.setString("Leaderboard:\n1. " + line1 + "\n2. " + line2 + "\n3. " + line3 + "\n4. " + line4 + "\n5. " + line5);
    FloatRect textRect = leaderboardText.getLocalBounds();
    leaderboardText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    leaderboardText.setPosition(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
    window.draw(leaderboardText);

    window.display();
}
