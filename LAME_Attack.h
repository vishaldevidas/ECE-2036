#include <iostream>
#include <string>
#include "mbed.h"


class ScreenAliens{ // base class for aliens
    public:
    ScreenAliens(double x, double y, char d = 'R');
    virtual void draw() = 0; // pure virtual function to draw the aliens
    virtual void update() = 0; // virtual function to update the aliens 
    void clear(); // function to clear the aliens
    double getX(); // returns X value of the alien 
    double getY(); // returns Y value of the alien
    char getDir(); // returns the initial direction of the aliens
    void setX(double x);
    void setY(double y);
    void setDir(char d);
    
    private:
    double xPos; // the x value of the alien
    double yPos; // the y value of the alien
    char dir; // the direction the alien goes initially    

}; // end of ScreenAliens

class AlienBob: public ScreenAliens{ // derived alien class
    public:
    AlienBob(double v,double x, double y, char d);
    virtual void draw(); 
    virtual void update();
    
    private:
    int speed; // speed of Bob
    bool up; // check for which sprite needs to bve displayed
}; // end of AlienBob

class AlienAlice: public ScreenAliens{
    public:
    AlienAlice(double v,double x, double y, char d);
    virtual void draw();
    virtual void update();
    
    private:
    int speed; // speed of Alice, faster than Bob

}; // end of AlienAlice

class AlienMat: public ScreenAliens{
    public:
    AlienMat(double v,double x, double y, char d);
    virtual void draw();
    virtual void update();
    
    private:
    int speed; // speed of Mat

}; // end of AlienMat

class AlienJoe: public ScreenAliens{ // speed of Joe changes randomly in update, so no speed variable
    public:
    AlienJoe(double x, double y, char d);
    virtual void draw();
    virtual void update();

}; // end of AlienJoe

class Spaceship{ 
    public:
    Spaceship();
    void draw(); //function to draw space ship
    void update(char dir); // update the spaceship based on button press
    void clear(); // clear the spaceship
    double getX(); // return the x value of the space ship
    
    private:
    double xPos; // x value of the spaceship
    double yPos; // y value of the spaceship
}; //end of Spaceship

class Bullet{
    public:
    Bullet(double x,int s); 
    void draw(); // function to draw bullet
    int update(); // updates the bullet and checks for aliens
    void clear(); // clears the bullet
    
    private:
    double xPos; // x value of the bullet
    double yPos; // y value of the bullet
    int size; // size of the bullet
}; // end of bullet
