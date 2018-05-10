#include <string>
#include <vector>
#include <iostream>
#include "mbed.h" 
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "MMA8452.h"
#include "Speaker.h"
#include "LAME_Attack.h"

#define EARTH_HEIGHT 21
#define EARTH_WIDTH 22
#define ALIEN_HEIGHT 8 
#define ALIEN_WIDTH 11 
#define B 0x0000FF //BLUE
#define G 0x00FF00 //GREEN
#define X 0xFFFFFF //WHITE
#define _ 0x000000 //BLACK

using namespace std;

extern uLCD_4DGL uLCD;
extern vector <ScreenAliens*> aliens;
extern Speaker mySpeaker;

extern bool BulletShot;

Serial pc(USBTX,USBRX);
MMA8452 acc(p28, p27, 40000);

int Earth_sprite[EARTH_HEIGHT * EARTH_WIDTH] = {
_,_,_,_,_,_,_,_,B,B,B,B,B,B,_,_,_,_,_,_,_,_, 
_,_,_,_,_,G,G,B,B,B,B,B,B,B,G,G,G,_,_,_,_,_, 
_,_,_,G,G,G,G,B,B,B,B,B,B,B,G,G,G,G,G,_,_,_,
_,_,G,G,G,G,B,B,B,B,B,B,G,G,G,G,G,G,G,G,_,_, 
_,_,G,G,G,G,B,B,B,B,B,G,G,G,G,G,G,G,G,G,_,_, 
_,G,G,G,G,G,B,B,B,B,G,G,G,G,G,G,G,G,G,G,G,_, 
_,G,B,B,B,B,B,B,B,B,G,G,G,G,G,G,G,G,G,G,G,_, 
_,G,B,B,B,B,B,B,B,B,G,G,G,G,G,G,G,G,G,G,G,_, 
G,G,B,B,B,B,B,B,B,B,G,G,G,G,G,G,G,G,G,G,G,G,
G,G,B,G,G,B,B,B,B,B,B,G,G,G,G,G,G,G,G,G,G,G, 
G,G,G,G,G,G,B,B,B,B,B,B,G,G,G,G,G,G,G,G,G,G, 
B,B,G,G,G,G,B,B,B,B,B,B,G,G,G,G,G,G,G,G,G,G,
B,B,G,G,G,G,B,B,B,B,B,B,B,G,G,G,G,G,G,G,G,G, 
B,B,G,G,G,B,B,B,B,B,B,B,B,B,G,G,G,G,G,G,G,G, 
_,B,B,G,G,B,B,B,B,B,B,B,B,B,G,G,G,G,G,G,G,_, 
_,B,B,B,G,B,B,B,B,B,B,B,B,B,B,G,G,G,G,G,G,_, 
_,B,B,B,G,B,B,B,B,B,B,B,B,B,B,B,G,G,G,G,G,_, 
_,_,B,B,B,B,B,B,B,B,B,B,B,B,B,B,G,G,G,G,_,_, 
_,_,_,B,B,B,B,B,B,B,B,B,B,B,B,B,G,G,G,_,_,_, 
_,_,_,_,_,B,B,B,B,B,B,B,B,B,B,B,G,_,_,_,_,_,
_,_,_,_,_,_,_,_,B,B,B,B,B,B,_,_,_,_,_,_,_,_,
};

int JoeClear_sprite[ALIEN_HEIGHT * ALIEN_WIDTH] = { 
_,_,_,_,_,_,_,_,_,_,_, 
_,_,_,_,_,_,_,_,_,_,_, 
_,_,_,_,_,_,_,_,_,_,_, 
_,_,_,_,_,_,_,_,_,_,_, 
_,_,_,_,_,_,_,_,_,_,_, 
_,_,_,_,_,_,_,_,_,_,_, 
_,_,_,_,_,_,_,_,_,_,_, 
_,_,_,_,_,_,_,_,_,_,_,
};

extern int alienJoe_sprite[ALIEN_HEIGHT * ALIEN_WIDTH]; /*= {
_,_,G,G,_,_,_,G,G,_,_, 
_,_,_,G,G,_,G,G,_,_,_, 
_,_,G,_,_,G,_,_,G,_,_, 
_,G,G,G,G,G,G,G,G,G,_, 
G,G,G,G,G,G,G,G,G,G,G, 
G,_,G,G,G,G,G,G,G,_,G, 
_,_,_,G,_,_,_,G,_,_,_, 
_,_,G,G,_,_,_,G,G,_,_,
};*/


// set up buttons
PinDetect pbLeft(p21); // left button
PinDetect pbRight(p22); // right button
PinDetect pbShoot(p23); // shoot button
PinDetect pbQuit(p24); // Quit button

Timer t; // Timer

// Global variables
bool gameOver;
bool shootBullet = false;
bool updateShip = false;
char shipDir = 'N';
bool start = false;

//Spaceship ship;

void pbQuit_hit_callback(void){ // Quit Button
    if(!start)start = true;
    else gameOver = true; 
}
void pbLeft_hit_callback(void){ // Left Button
    shipDir = 'L';
    updateShip = true;
    //ship.update('L');
}
void pbRight_hit_callback(void){ // Right Button
    shipDir = 'R';
    updateShip = true;
    //ship.update('R');
}
void pbShoot_hit_callback(void){ // Shoot Button
    if(!BulletShot)
    shootBullet = true;
}
void pb_released_callback(void){ // Direction button not pressed
    shipDir = 'N';
    updateShip = false;
}

void moveAlien(){ // Accelerometer implementation
    
    // set up the device
    acc.setBitDepth(MMA8452::BIT_DEPTH_12);
    acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
    acc.setDataRate(MMA8452::RATE_200);
    
    int offsetx = 99;
    int offsety = 63;
    double factor = 10;
    
    double x = 0, y = 0, z = 0;
    while(!start) {
      
        uLCD.BLIT(-1*y*factor+offsety, -1*x*factor+offsetx, ALIEN_WIDTH, ALIEN_HEIGHT, JoeClear_sprite); // clear the object
        if(!acc.isXYZReady()) 
        {
            wait(0.01); 
        }
        else
        { 
      
        acc.readXYZGravity(&x,&y,&z); //get readings 
      
        uLCD.BLIT(-1*y*factor+offsety, -1*x*factor+offsetx, ALIEN_WIDTH, ALIEN_HEIGHT, alienJoe_sprite); // draw at new spot
        }
    }
}

int main(){
    
    // make buttons pull up 
    
    pbLeft.mode(PullUp);
    pbRight.mode(PullUp);
    pbShoot.mode(PullUp);
    pbQuit.mode(PullUp);
    wait(.01);
    
    pbLeft.attach_deasserted(&pbLeft_hit_callback);
    pbLeft.attach_asserted(&pb_released_callback);
    pbRight.attach_deasserted(&pbRight_hit_callback);
    pbRight.attach_asserted(&pb_released_callback);
    pbShoot.attach_deasserted(&pbShoot_hit_callback);
    pbQuit.attach_deasserted(&pbQuit_hit_callback);
    
    pbLeft.setSampleFrequency(10000);
    pbRight.setSampleFrequency(10000);
    pbShoot.setSampleFrequency(10000);
    pbQuit.setSampleFrequency(10000);
    
    // start screen
    uLCD.printf("\n\n SAVE \n\n");
    mySpeaker.PlayNote(200.0,0.25,0.1);
    uLCD.printf("\n\n THE \n\n");
    mySpeaker.PlayNote(400.0,0.25,0.1);
    uLCD.printf("\n\n EARTH \n\n");
    mySpeaker.PlayNote(800.0,0.25,0.1);
    
    moveAlien(); // move a tiny alien on the screen before game starts
    
    uLCD.cls(); // clear the screen 
    
    int type;
    int rndom;
    double x;
    double y;
    int speed;
    char dir;
    int checkBullet;
    int bulletSize;
    bool powerUp = true;
    bool winGame = false;
    
    
    
    for(int i = 0; i < 6; i++){ // loop 6 time and randomly generate 6 aliens and positions
        rndom = (rand()%6); 
        y = 14*i+14;
        x = (rand()%58) * 2;
        type = (rand()%4)+1;
        speed = rand()%14;
        rndom = (rand()%2)+1;
        switch(rndom){ // choose position based on random number
            case 1: dir = 'R';
                    break;
            case 2: dir = 'L';
                    break;
            default: dir = 'R';
        }
        switch(type){ // choose alien type based on random number
            case 1: aliens[i] = new AlienBob(speed,x,y,dir);
                    break;
            case 2: aliens[i] = new AlienAlice(14,x,y,dir);
                    break;
            case 3: aliens[i] = new AlienMat(speed,x,y,dir);
                    break;
            case 4: aliens[i] = new AlienJoe(x,y,dir);
                    break;
            default: aliens[i] = new AlienJoe(x,y,dir);
        }
    }
    
    Spaceship ship; // instantiate Ship 
    ship.draw(); // draw the ship
    
    Bullet* shot = NULL; // creat a bullet pointer 
    bulletSize = 1; // bullet size set to 1
    
    
    for(int i = 0; i < 6; i++) // loop throught each
        aliens[i]->draw(); // draw the aliens
    
    gameOver = false; // game not over, just started
    t.start(); // start timer
    while(!gameOver){ // loop till game over
        for(int i = 0; i < aliens.size(); i++){ // go throught each alien
            aliens[i]->update(); // update the alien
            
            if(updateShip)ship.update(shipDir); // if direction button pressed, update the ship
            
            
            if(aliens.size() == 1 && powerUp){ //if only one alien left power up the ship
                    bulletSize = 2; // increase the bullet size
                    ship.update('N'); //blink the ship to show the power up
                    mySpeaker.PlayNote(800.0,0.25,0.1); // make noices, cause its cool 
                    ship.update('N');
                    mySpeaker.PlayNote(800.0,0.25,0.1);
                    powerUp = false; // no more power ups
            }
               
            if(shootBullet){ // if shoot button pressed
                shot = new Bullet(ship.getX(),bulletSize); //create a pointer to new bullet
                BulletShot = true; // bullet on the screen
                shootBullet = false; // cant shoot again till the last one is gone
            } 
            if(BulletShot){ // if bullet on the screen
                checkBullet = shot->update(); // update the postion and check to see if hit
                if(checkBullet == 0){ // if shit or left the screen 
                    free(shot); // free the pointer 
                    BulletShot = false; // no bullet on the screen
                }
            }
            if(aliens.empty()){ // if all aliens have been hit 
                t.stop(); // stop timer
                uLCD.cls(); //clear out the lcd
                float time = t.read(); // read the timer
                uLCD.printf("\n\n Congratulations!!\n\n Earth Saved!\n\n\n(%.2f Seconds)", time); // end game display
                gameOver = true; // game is over
                winGame = true; // you win
                break; // dont loop anymore 
            }
            if(gameOver){ // if the quit button was pressed
                t.stop(); //  stop the timer
                uLCD.cls(); // clear the screen
                uLCD.printf("\n\n YOU QUIT \n\n"); // quit game screen
                mySpeaker.PlayNote(400.0,0.25,0.1); // music cause it cool
                mySpeaker.PlayNote(300.0,0.25,0.1);
                mySpeaker.PlayNote(200.0,0.25,0.1);
                mySpeaker.PlayNote(100.0,0.25,0.1);
                break; 
            }    
            
            
        
        }
    }
    uLCD.BLIT(52, 99, EARTH_WIDTH, EARTH_HEIGHT, Earth_sprite); // display a custom made earth on the bottom of the screen
    if(winGame){ // if won the game 
        mySpeaker.PlayNote(400.0,0.25,0.1); // play a different tone. a more cheerfull one 
        mySpeaker.PlayNote(100.0,0.25,0.1);
        mySpeaker.PlayNote(600.0,0.25,0.1);
        mySpeaker.PlayNote(800.0,0.25,0.1);
    }
    t.reset();
    
    
}