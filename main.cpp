#include <iostream>
#include <vector>
#include <math.h>
#include "lib.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

int main() {

    const int frameCap = 180;
    const int UpdatesPerFrame = 40;
    int curUpdate = 0;

    int index = 0;

    const int screenH = sf::VideoMode::getDesktopMode().height, 
    screenW = sf::VideoMode::getDesktopMode().width;

    sf::RenderWindow window(sf::VideoMode(), "My window", sf::Style::Fullscreen);

    window.setFramerateLimit(frameCap);

    sf::Vertex line[2];

    vector <Planet> planets;

    bool mouseClicked;

    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }       

        window.clear();

        mouseClicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if(mouseClicked) {
            int mouseCords[2] = {
                sf::Mouse::getPosition(window).x, 
                sf::Mouse::getPosition(window).y
            };

            Planet planet;
            planet.id = index;
            index++;
            planet.mass = 1 * pow(10, 24);
            planet.x = mouseCords[0] * aspect;
            planet.y = mouseCords[1] * aspect;

            line[0] = sf::Vertex(sf::Vector2f(mouseCords[0], mouseCords[1]));
            // Planet creation on mouse click
            while(mouseClicked) {
                window.clear(); 

                planet.mass += planet.mass / frameCap;
                planet.setRadius();
                planet.setShape();
                window.draw(planet.shape);

                for(auto &curPlanet : planets) {
                    curPlanet.move();
                }
                for(auto curPlanet : planets) {
                    if(curPlanet.isAlive) {
                        curPlanet.setRadius();
                        curPlanet.setShape();
                        window.draw(curPlanet.shape);
                    }
                }

                mouseCords[0] = sf::Mouse::getPosition(window).x;
                mouseCords[1] = sf::Mouse::getPosition(window).y;

                line[1] = sf::Vertex(sf::Vector2f(mouseCords[0], mouseCords[1]));

                window.draw(line, 2, sf::Lines);

                window.display();

                mouseClicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
            }
            
            mouseCords[0] = sf::Mouse::getPosition(window).x;
            mouseCords[1] = sf::Mouse::getPosition(window).y;

            planet.vector.x = (mouseCords[0] * aspect - planet.x) / frameCap / UpdatesPerFrame; //move how far the mouse moved every 5 seconds
            planet.vector.y = (mouseCords[1] * aspect - planet.y) / frameCap / UpdatesPerFrame;
            
            planets.push_back(planet);
            window.clear();
        }

        while(curUpdate < UpdatesPerFrame) {

            for(auto &curPlanet : planets) {
                for(auto &toPlanet : planets) {
                    if(curPlanet.id == toPlanet.id or !curPlanet.isAlive) continue;
                    curPlanet.addVecFromForce(toPlanet);
                    curPlanet.eat(toPlanet);
                }
            }

            for(auto &curPlanet : planets) {
                curPlanet.move();
            }

            curUpdate++;
        }

        curUpdate = 0;

        for(auto &curPlanet : planets) {
            if(curPlanet.isAlive) {
                curPlanet.setRadius();
                curPlanet.setShape();
                window.draw(curPlanet.shape);
            }
        }

        window.display();
    } 
}