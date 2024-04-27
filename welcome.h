#include <SFML/Graphics.hpp>
#include <string>


using namespace std;
using namespace sf;


class WelcomeScreen {
public:
    WelcomeScreen();
    static void loadFont(Font& font, const string& filename);
    void displayWelcomeMessage(RenderWindow& window);
    void handleTextInput(Event& event);
    void handleEnterPress(Event& event);
    void run();
    void setText(Text &text, float x, float y);

private:
    RenderWindow window;
    Font font;
    Text welcomeText;
    Text enterNameText;
    Text playerNameText;
    string playerName;
    bool windowOpen;

};
