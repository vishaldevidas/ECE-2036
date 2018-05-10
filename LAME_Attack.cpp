#include <string>
#include <vector>
#include <iostream>
#include "mbed.h" 
#include "uLCD_4DGL.h"
#include "MMA8452.h"
#include "Speaker.h"
#include "LAME_Attack.h"

#define ALIEN_HEIGHT 8 
#define ALIEN_WIDTH 11 
#define _ 0x000000 //BLACK 
#define X 0xFFFFFF //WHITE
#define G 0x00FF00 //GREEN
#define R 0xFF0000 //RED
#define B 0x0000FF //BLUE
#define O 0xFFA500 //ORANGE
#define SHIP_SIZE 11
using namespace std;

uLCD_4DGL uLCD(p9, p10, p11); // uLCD object
Speaker mySpeaker(p26); // speaker object

vector <ScreenAliens*> aliens(6); // global vector of Alien pointers
bool BulletShot = false; // check to see if bullet on screen

int alienBobDown_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
_,_,R,_,_,_,_,_,R,_,_,
_,_,_,R,_,_,_,R,_,_,_,
_,_,R,R,R,R,R,R,R,_,_, 
_,R,R,_,R,R,R,_,R,R,_, 
R,R,R,R,R,R,R,R,R,R,R, 
R,_,R,R,R,R,R,R,R,_,R, 
R,_,R,_,_,_,_,_,R,_,R, 
_,_,_,R,R,_,R,R,_,_,_,
};

int alienBobUp_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
_,_,R,_,_,_,_,_,R,_,_, 
R,_,_,R,_,_,_,R,_,_,R, 
R,_,R,R,R,R,R,R,R,_,R, 
R,R,R,_,R,R,R,_,R,R,R, 
R,R,R,R,R,R,R,R,R,R,R, 
_,_,R,R,R,R,R,R,R,_,_, 
_,_,R,_,_,_,_,_,R,_,_, 
_,R,_,_,_,_,_,_,_,R,_,
};

int alienAlice_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
_,_,_,_,B,B,B,_,_,_,_, 
_,B,B,B,B,B,B,B,B,B,_, 
B,B,B,B,B,B,B,B,B,B,B, 
B,B,B,_,_,B,_,_,B,B,B, 
B,B,B,B,B,B,B,B,B,B,B, 
_,_,_,B,B,_,B,B,_,_,_, 
_,_,B,B,_,_,_,B,B,_,_, 
B,B,_,_,_,B,_,_,_,B,B,
};

int alienMat_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
_,_,_,_,_,_,_,_,_,_,_, 
_,_,X,X,X,X,X,X,X,_,_, 
_,X,X,X,X,X,X,X,X,X,_, 
_,X,X,_,_,X,_,_,X,X,_, 
_,X,X,X,X,X,X,X,X,X,_, 
_,_,X,X,X,X,X,X,X,_,_, 
_,X,_,X,X,_,X,X,_,X,_, 
X,X,_,_,_,X,_,_,_,X,X,
};

int alienJoe_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = {
_,_,G,G,_,_,_,G,G,_,_, 
_,_,_,G,G,_,G,G,_,_,_, 
_,_,G,_,_,G,_,_,G,_,_, 
_,G,G,G,G,G,G,G,G,G,_, 
G,G,G,G,G,G,G,G,G,G,G, 
G,_,G,G,G,G,G,G,G,_,G, 
_,_,_,G,_,_,_,G,_,_,_, 
_,_,G,G,_,_,_,G,G,_,_,
};

int Spaceship_sprite[SHIP_SIZE * SHIP_SIZE] = {
_,_,_,_,_,O,_,_,_,_,_, 
_,_,_,_,O,O,O,_,_,_,_, 
_,_,_,_,O,O,O,_,_,_,_, 
_,_,X,_,O,O,O,_,X,_,_, 
_,_,X,X,O,O,O,X,X,_,_, 
_,_,X,X,X,X,X,B,X,_,_, 
_,_,X,_,X,X,X,_,X,_,_, 
_,_,_,_,X,X,X,_,_,_,_,
_,_,G,G,G,G,G,G,G,_,_, 
_,G,G,_,G,G,G,_,G,G,_, 
G,G,_,_,G,G,G,_,_,G,G,
};

// ScreenAliens functions implimentation 
ScreenAliens::ScreenAliens(double x, double y, char d): xPos(0),yPos(0),dir(d){
    if(x >= 0 && x <= 116) xPos = x; // if alien on screen than set position
    if(y >= 0 && y <= 99) yPos = y;
}

void ScreenAliens::draw(){
    uLCD.filled_circle(xPos, yPos , 11,GREEN); 
}
void ScreenAliens::update(){
    
    clear(); // clear the alien
    
    if(dir == 'R'){ // if moving right
      if(xPos >= 116){ // if hit the wall
        xPos = xPos - 5; // bounce back
        dir = 'L'; // go left now
      }else{
        xPos = xPos + 5; // else go right
      }      
    }else{
      if(xPos == 0){ // if moving left and hit the wall
        xPos = xPos + 5; // bounce 
        dir = 'R'; // go right
      }else{
        xPos = xPos - 5; // else go left
      }  
    }// end of if else
    
    draw(); // draw at the new position
}
void ScreenAliens::clear(){ // clear the alien
    uLCD.filled_rectangle(xPos,yPos,xPos+11,yPos+8,BLACK); // cover it with black box 
}
double ScreenAliens::getX(){ // retutn the xPosintion 
    return xPos;
}
double ScreenAliens::getY(){ // return the yPosition
    return yPos;
}
char ScreenAliens::getDir(){ // return direction
    return dir;
}
void ScreenAliens::setX(double x){ // set the new xPosition
    if(x > 116) // set the wall right
        x = 116;
    if(x < 0) // set the wall left
        x = 0;
    xPos = x; //set new x
}
void ScreenAliens::setY(double y){ // set the new yPosition 
    if(y > 99) //set the walls up and down
        y = 99;
    if(y < 0)
        y = 0;
    yPos = y;
}
void ScreenAliens::setDir(char d){
    dir = d; // set the new directions
}
// AlienBob functions implimentation
AlienBob::AlienBob(double v,double x,double y,char d): ScreenAliens(x,y,d),speed(v),up(true){
    if(speed <= 5) speed = 5; // cap the speed at 5 and 10
    if(speed > 10) speed = 10; 
}
void AlienBob::draw(){
    
    if(up){
        uLCD.BLIT(getX(), getY(), ALIEN_WIDTH, ALIEN_HEIGHT, alienBobUp_sprite); // display the up position of Bob    
    }else{
        uLCD.BLIT(getX(), getY(), ALIEN_WIDTH, ALIEN_HEIGHT, alienBobDown_sprite); // display the down position of Bob
    }
}
void AlienBob::update(){
    double x = getX(); // get current X position of Bob
    char dir = getDir(); // get current direction of Bob 
    
    clear(); // clear out the old position 
    
    if(dir == 'R'){ // if Bob was going right 
      if(x >= 116){ // hit the right wall  
        x = x - speed; // bounced back
        dir = 'L'; // goes left now
      }else{
        x = x + speed; // move to the right
      }      
    }else{ // if Bob was going left
      if(x == 0){ // hit the left wall 
        x = x + speed; // bounce back
        dir = 'R'; // goes right now
      }else{
        x = x - speed; // move to the left
      }  
    }//end of if
    up = !up; // change position
    setX(x); // set the new X position
    setDir(dir); // set the new direction
    draw(); // draw Bob at new spot
}
// AlienAlice functions implimentation
AlienAlice::AlienAlice(double v,double x,double y, char d): ScreenAliens(x,y,d),speed(v){
    if(speed <= 8) speed = 8;// cap the speed at 8 and 14
    if(speed > 14) speed = 14;
}
void AlienAlice::draw(){

        uLCD.BLIT(getX(), getY(), ALIEN_WIDTH, ALIEN_HEIGHT, alienAlice_sprite); // draw Alice sprite
}
void AlienAlice::update(){
    double x = getX(); // get old X
    char dir = getDir(); // get old direction 
     
     clear(); // clear alien at old position 
    
    if(dir == 'R'){ // calculate new position
      if(x >= 116){
        x = x - speed;
        dir = 'L';
      }else{
        x = x + speed;
      }      
    }else{
      if(x == 0){
        x = x + speed;
        dir = 'R';
      }else{
        x = x - speed;
      }  
    }
    setX(x); // set new X
    setDir(dir); // set new direction
    draw(); // draw alien at new spot
}
// AlienMat functions implimentation
AlienMat::AlienMat(double v,double x,double y, char d): ScreenAliens(x,y,d),speed(v){
    if(speed <= 6) speed = 6; //cap the speed at 6 and 10
    if(speed > 10) speed = 10;
}
void AlienMat::draw(){

        uLCD.BLIT(getX(), getY(), ALIEN_WIDTH, ALIEN_HEIGHT, alienMat_sprite);// draw Mat sprite
}
void AlienMat::update(){
    double x = getX(); // get old X
    char dir = getDir(); // get old directions 
     clear(); // clear out the alien at old spot
    
    if(dir == 'R'){ // calculate the new position 
      if(x >= 116){
        x = x - speed;
        dir = 'L';
      }else{
        x = x + speed;
      }      
    }else{
      if(x == 0){
        x = x + speed;
        dir = 'R';
      }else{
        x = x - speed;
      }  
    }
    setX(x); // store the new X
    setDir(dir); // store the new direction
    draw(); // draw alien at the new position
}
// AlienJoe functions implimentation
AlienJoe::AlienJoe(double x,double y, char d): ScreenAliens(x,y,d){} // Joe changes speed randomly, so no V

void AlienJoe::draw(){

        uLCD.BLIT(getX(), getY(), ALIEN_WIDTH, ALIEN_HEIGHT, alienJoe_sprite); // draw Joe
}
void AlienJoe::update(){ 
    double x = getX(); // get old x
    char dir = getDir(); // get old direction
    int speed = (rand()%2)+7; // generate random speed
     
     clear(); // clear out the old alien
    
    if(dir == 'R'){ // calculate the new position
      if(x >= 116){
        x = x - speed;
        dir = 'L';
      }else{
        x = x + speed;
      }      
    }else{
      if(x <= 3){
        x = x + speed;
        dir = 'R';
      }else{
        x = x - speed;
      }  
    }
    setX(x); // store the new X
    setDir(dir); // store the new direction
    draw(); // draw the alien at new position
}

// Spaceship functions implimentation
Spaceship::Spaceship():xPos(58),yPos(117){}; //start at the bottom center

void Spaceship::draw(){
    uLCD.BLIT(xPos, yPos, SHIP_SIZE, SHIP_SIZE, Spaceship_sprite); // draw spaceship sprite
}

void Spaceship::update(char dir){
    clear(); // clear the spaceship at old position
    if(dir == 'R'){ // calculate the new position
        if(xPos <= 108)
            xPos = xPos+10;// if right pressed
    }else if(dir == 'L'){
        if(xPos > 10)
            xPos = xPos-10; // if left presses
    }
    draw(); // draw the spaceship at new position
}
void Spaceship::clear(){
    uLCD.filled_rectangle(xPos,yPos,xPos+10,yPos+10,BLACK); // draw a black box on the spaceship
}
double Spaceship::getX(){
    return xPos; // return the xPosition of the spaceship
}

// Bullet function implimentation
Bullet::Bullet(double x,int s):xPos(x+4),yPos(112),size(s){}; // bullet starts at the tip of the spaceship

void Bullet::draw(){
    uLCD.filled_rectangle(xPos,yPos,xPos+2*size,yPos+3*size,WHITE); // a white bar is the bullet
}

int Bullet::update(){ 
    clear(); // clear bullet at old spot
    if(yPos < 0){ // check to see if left the screen
        clear(); // clear it
        BulletShot = false; 
        return 0;
    }
    yPos = yPos-7; // if not move up the screen
    draw(); // draw at new position
    
    for(int i = 0; i < aliens.size(); i++){ // run throught all alive alien
        double alienX = aliens[i]->getX(); // get there position
        double alienY = aliens[i]->getY(); 
        
        if((xPos > alienX-2 && xPos < alienX+11+2*size) && (yPos > alienY-5*size && yPos < alienY+11)){ // check if the bullet hit
            mySpeaker.PlayNote(200.0,0.25,0.1); // if so make a sound
            aliens[i]->clear(); // clear the alien
            free(aliens[i]); // free the alien
            aliens.erase(aliens.begin()+i); // and also clear the spot in the vector
            clear(); // clear the bullet as well
            BulletShot = false; // get read to shoot again
            return 0;
        }
    }
    return 1;
}
void Bullet::clear(){
    uLCD.filled_rectangle(xPos,yPos,xPos+2*size,yPos+3*size,BLACK); // draw a black box to clear
}
