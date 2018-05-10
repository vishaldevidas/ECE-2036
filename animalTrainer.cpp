#include <string>
#include "mbed.h" 
#include "uLCD_4DGL.h"
#include "Speaker.h"
#include "SDFileSystem.h"

using namespace std;

uLCD_4DGL uLCD(p28, p27, p29); // create a global lcd object

Speaker mySpeaker(p21); // set up speaker

// set up buttons
DigitalIn pbLeft(p16); // left button
DigitalIn pbRight(p17); // right button
DigitalIn pbQuit(p18); // quit button

Timer t; // set timer

SDFileSystem sd(p5, p6, p7, p8, "sd"); // set up sd card


int getColour()
{
    int colour = rand()% 4; // generate a random number with corresponds to a random color
    int colourOut; // color based on the randon number
    switch(colour)
    {
        case 0: colourOut = WHITE;
                break;
        case 1: colourOut = RED;
                break;
        case 2: colourOut = GREEN;
                break;
        case 3: colourOut = BLUE;
                break;
    } // end of switch
    return colourOut;
} // end of getColour
  

void drawShape(int shapeIn, int xIn,int yIn) // draw the snapes using the type of shape and the center point
{
    int radius = (rand()% 7) + 2; // generate a random radius between 1 and 10 so that it wont go into another tile
    int colour = getColour();
    switch(shapeIn)
    {
        case 0: // circle
                uLCD.filled_circle(xIn,yIn,radius,colour);
                break;
        case 1: // square
                uLCD.filled_rectangle(xIn-radius,yIn-radius,xIn+radius,yIn+radius,colour);
                break;
        case 2: // tringle
                uLCD.line(xIn-radius,yIn+radius,xIn,yIn-radius,colour); // bottom left to the top center 
                uLCD.line(xIn,yIn-radius,xIn+radius,yIn+radius,colour); // top center to the bottom right 
                uLCD.line(xIn+radius,yIn+radius,xIn-radius,yIn+radius,colour); // bottom right to bottom left
                break;
    } // end of switch
}// end of drawShape

void printShape(int position,int x1,int y1) // find the position, shape, color and finally print it on the screen
{
    int shape = rand()% 3; // random number to pick a random shape 
    int x; // The x position of the object
    int y; // The y position of the object
    switch(position) // find which tile the object should be in, the cases correspond to the 18 tiles in each box
    {
        case 1: 
                x = x1+10;
                y = y1+10;
                break;
        case 2: 
                x = x1+30;
                y = y1+10;
                break;
        case 3: 
                x = x1+50;
                y = y1+10;
                break;
        case 4: 
                x = x1+10;
                y = y1+30;
                break;
        case 5: 
                x = x1+30;
                y = y1+30;
                break;
        case 6: 
                x = x1+50;
                y = y1+30;
                break;
        case 7: 
                x = x1+10;
                y = y1+50;
                break;
        case 8: 
                x = x1+30;
                y = y1+50;
                break;
        case 9: 
                x = x1+50;
                y = y1+50;
                break;
        case 10: 
                 x = x1+10;
                 y = y1+70;
                 break;
        case 11: 
                 x = x1+30;
                 y = y1+70;
                 break;
        case 12: 
                 x = x1+50;
                 y = y1+70;
                 break;
        case 13: 
                 x = x1+10;
                 y = y1+90;
                 break;
        case 14: 
                 x = x1+30;
                 y = y1+90;
                 break;
        case 15: 
                 x = x1+50;
                 y = y1+90;
                 break;
        case 16: 
                 x = x1+10;
                 y = y1+110;
                 break;
        case 17: 
                 x = x1+30;
                 y = y1+110;
                 break;
        case 18: 
                 x = x1+50;
                 y = y1+110;
                 break;
    } // end of switch
    drawShape(shape,x,y); // prints the shape onto the screen
} // end of print shape
int generateShapes(int x1,int y1) // generates a random shape
{
    int numOfObjects; 
    int position;
    bool spaces[18] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,}; 
    // true indicates that space is not taken
    numOfObjects = (rand()%14) + 2; // randomly generate number of objects to display;
    for(int i = 0; i < numOfObjects; i++)
    {
        position = (rand()%18) + 1; // position of the center point of the shape
        if(spaces[position-1])
        {
            spaces[position-1] = false;
        }
        else
        {
            while(!spaces[position-1])
            {
                position = (rand()%18) + 1;
            }
            spaces[position-1] = false;
        }
        printShape(position,x1,y1); // print the shape on to the screen 
    } // end of for statement
    return numOfObjects; // number of shapes in each box
    
} // end of generateShape 

float accuracyUpdate(float accuracy,int correct, int turn)
{
    accuracy = ((float)correct/(float)turn)*100;
    return accuracy;
}

void stats(int &trial, int &numCorrect, float &accuracy)
{
    int numWrong = trial - numCorrect;
    uLCD.printf("       Stats       \n");
    uLCD.printf("\nTotal trials = %d\n", trial);
    uLCD.printf("\nTotal correct = %d\n", numCorrect);
    uLCD.printf("\nTotal wrong = %d\n",numWrong);
    uLCD.printf("\nAccuracy = %.2f", accuracy);
}
void printTrialStats(int numOfObjectsL,int numOfObjectsR, int trial, float accuracy, float time)
{
    uLCD.printf("\nLeft = %d\n",numOfObjectsL);
    uLCD.printf("\nRight = %d\n",numOfObjectsR);
    uLCD.printf("\nTrial Number = %d\n",trial);
    uLCD.printf("\nAccuracy = %.2f\n",accuracy);
    uLCD.printf("\nTimetook(s) = %.2f\n",time);
}

  
int main(){
    
    int numOfObjectsL; // number of objects in the left box
    int numOfObjectsR; // number of objects in the right box
    bool END = false; // for the quit button 
    int trial = 0; // trial number
    int numCorrect = 0; // number of correct answers 
    float accuracy; // accuracy
    
    //start up screen
    uLCD.background_color(BLACK);
    uLCD.printf("\n\n\n\n\n\n     WELCOME!      \n");
    mySpeaker.PlayNote(200.0,0.25,0.1);
    mySpeaker.PlayNote(400.0,0.25,0.1);
    mySpeaker.PlayNote(800.0,0.25,0.1);
    mySpeaker.PlayNote(200.0,0.25,0.1);
    mySpeaker.PlayNote(500.0,0.25,0.1);
    mySpeaker.PlayNote(400.0,0.25,0.1);
    mySpeaker.PlayNote(200.0,0.25,0.1);
    wait(2);
    
    mkdir("/sd/Animal_Trainer", 0777);
    FILE *fp = fopen("/sd/Animal_Trainer/LemurResults.txt", "w");
    fprintf(fp,"STATS\n\n");
    do
    {
        //start by incrementing trial
        trial++;
        
        // generating the two boxes 
        uLCD.cls();
        uLCD.rectangle(0,0,60,120,WHITE);
        uLCD.rectangle(67,0,127,120,WHITE);
    
        // generating the objects in each box
        numOfObjectsL = generateShapes(0,0);
        numOfObjectsR = generateShapes(67,0);
        while(numOfObjectsL == numOfObjectsR) // if the number of objects in both boxes is the same, generated new random objects
        {
            numOfObjectsR = generateShapes(67,0);
        } // end of while loop
        char answer;
        if(numOfObjectsL < numOfObjectsR)
            answer = 'L';
        else
            answer = 'R';
            
        t.start(); // start the timer after the shapes are created
            
        // changing the mode of the buttons to pull up
        pbLeft.mode(PullUp);
        pbRight.mode(PullUp);
        pbQuit.mode(PullUp);
        wait(0.1);
    
        //now wait for a change in state of the buttons 
        bool GETOUT = false;
        char playerAnswer = 'E';
        while (!GETOUT) 
        {
            if (!pbLeft) //left button pressed 
            {
                t.stop(); // stop the timer as soon as the button is pressed
                GETOUT = true;
                playerAnswer = 'L';
            }//end if statement
            if (!pbRight) //right button pressed 
            {
                t.stop(); // stop the timer as soon as the button is pressed
                GETOUT = true;
                playerAnswer = 'R';
            } //end if statement
            if (!pbQuit) // quit button pressed
            {
                t.stop(); // stop the timer as soon as the button is pressed
                GETOUT = true;
                END = true;
            }
        }   //end while loop
        
        uLCD.cls();
        
         
        if(playerAnswer == answer)
        {
            numCorrect++;
            uLCD.printf("\nCorrect!! :)\n\n");
        }
        else if(playerAnswer == 'E')
        {
            stats(trial,numCorrect,accuracy);
        }
        else
        {
            uLCD.printf("\nIncorrect! :(\n\n");
        }
        if(!END)
        {
            accuracy = accuracyUpdate(accuracy,numCorrect,trial); // update the accuracy
            printTrialStats(numOfObjectsL,numOfObjectsR,trial,accuracy,t.read()); // print the stats after each trial
            
            // store the stats on the SD card
            if(fp == NULL) 
            {uLCD.printf("\nCould'nt Save! \n"); }
            fprintf(fp,"\n\nTrial Number = %d\n",trial);
            fprintf(fp,"Left = %d\n",numOfObjectsL);
            fprintf(fp,"Right = %d\n",numOfObjectsR);
            fprintf(fp,"Accuracy = %.2f\n",accuracy);
            fprintf(fp,"Timetook(s) = %.2f\n",t.read());
            
            uLCD.printf("\nSaved!\n");
            
            
            wait(3);
        }
        t.reset();
    }while(!END);
    
    fclose(fp);  
}


