#include <math.h>
#include <SFML/Graphics.hpp>

const double aspect = 3000000;
const double GG = 6.674*pow(10.0, -11);

class vec2 {
    public:
        double x = 0, y = 0;

        double getDirection() {
            double direction = atan(x/y);
            if(y == 0) {
                if (x < 0) { direction = M_PI / 2; }
                else { direction = - M_PI / 2; }
            }
            else if (y < 0) {
                if (x > 0) {
                    direction = M_PI / 2 + (M_PI / 2 + direction);
                } else {
                    direction = - (M_PI / 2) - (M_PI / 2 - direction);
                }
            }
            return direction;    
        }
        
        double getLength() { return sqrt(x*x+y*y); }

        void setVector(double direction, double length) {
            x = length * cos(direction);
            y = length * sin(direction);
        }        
};

vec2 convertToVector(double mass, long double force, double direction) {
    vec2 newVector;
    double velocity = (force / mass) / 100000 * 86400;
    newVector.setVector(direction, velocity);
    return newVector;
}

class Planet {
    public:
        sf::CircleShape shape;
        sf::Vertex line[2];
        double x = 0, y = 0; // in km 
        double long mass; 
        double radius; // in kg, km
        int id;
        bool isAlive = true;
        int color[3] = {rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55}; // rgb
        vec2 vector;

        void move() {
            x += vector.x;
            y += vector.y;
        }

        void setShape() {
            shape.setRadius((float)(radius / aspect));
            shape.setFillColor(sf::Color(color[0], color[1], color[2]));
            shape.setPosition(sf::Vector2f(
                x / aspect - radius / aspect, 
                y / aspect - radius / aspect
                )
                );
        }

        void setVertex() {
            line[0] = sf::Vertex(sf::Vector2f(x / aspect, y / aspect));
            line[1] = sf::Vertex(sf::Vector2f(x / aspect + vector.x, y / aspect + vector.y));
        }

        void setRadius() {
            radius = cbrt(3 * ((mass) / (5514)) / (3 * M_PI));
        }

        double getDistanceTo(Planet planet) {
            return sqrt((x-planet.x)*(x-planet.x) + (y-planet.y)*(y-planet.y));
        }

        double long getForceTo(Planet planet) {
            return (GG*mass*planet.mass)/(pow(getDistanceTo(planet), 2));
        }

        vec2 getVectorTo(Planet planet) {
            vec2 newVector;
            newVector.x = planet.x - x;
            newVector.y = planet.y - y;
            return newVector;
        }

        void addVecFromForce(Planet planet) {
            vec2 tmpVector = getVectorTo(planet);
            double long force = getForceTo(planet);
            vec2 addVector = convertToVector(mass, force, tmpVector.getDirection());
            vector.x += addVector.x;
            vector.y += addVector.y;
        }

        void eat(Planet planet) {
            vec2 tmpVector = getVectorTo(planet);
            vec2 addVector = getForceTo(planet);
            if(radius + planet.radius > tmpVector.getLength() - addVector.getLength()) {
                if(mass > planet.mass) {
                    mass += planet.mass;
                    planet.isAlive = false;
                }
            }
        }
};