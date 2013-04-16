#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

int main()
{
    // create the window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::Window window(sf::VideoMode(800, 800), "OpenGL", sf::Style::Default, settings);

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
        }

        // clear the buffers
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        GLenum err;
        while((err = glGetError())) {                                
          std::cout << "OpenGL error: " << err << std::endl;                        
        }

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    return 0;
}
