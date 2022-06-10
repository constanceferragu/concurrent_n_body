#include <SFML/Graphics.hpp>
#include <unistd.h>


int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	int t = 0;
    // run the program as long as the window is open
    while (t < 5)
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with white color
        window.clear(sf::Color::White);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
		sleep(3);
		t ++;
    }

    return 0;
}



