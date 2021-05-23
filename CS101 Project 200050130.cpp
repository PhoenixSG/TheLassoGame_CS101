/*
Hello and welcome to My Lasso Code.
Since I am using an IDE (Codeblocks), I have maintained all the code in one file only.

Original code was around 450 lines long.
My code is about 1000 lines long and has sufficient comments everywhere for better understanding.

Please have a look at the help document "README.doc" before proceeding further.
Enjoy!

-Shashwat Garg
Roll no: 200050130
200050130@iitb.ac.in

*/


#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>
using namespace simplecpp;
//including the header files used throughout the program


/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//begin MovingObject class code


#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <composite.h>
#include <sprite.h>
using namespace simplecpp;


//class declaration + definition of small functions
class MovingObject : public Sprite {//inheriting Sprite class

    vector<Sprite*> parts;          //for moving several objects together
    double vx, vy;                  //kinematics
    double ax, ay;

    bool paused;    //paused status

    void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    {               //initialization (cartesian)
        vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
    }
                    //above members private

                    //below members public
    public:
                    //constructors, Cartesian and Polar
    MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
        : Sprite() {
        initMO(argvx, argvy, argax, argay, argpaused);
        }


    MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
        double angle_rad = angle_deg*PI/180.0;
        double argvx = speed*cos(angle_rad);
        double argvy = -speed*sin(angle_rad);
        initMO(argvx, argvy, argax, argay, argpaused);
    }
                    //end both constructors, Cartesian and Polar

    void set_vx(double argvx) { vx = argvx; }
    void set_vy(double argvy) { vy = argvy; }
    void set_ax(double argax) { ax = argax; }
    void set_ay(double argay) { ay = argay; }
                    //end value assignment functions

    void addPart(Sprite* p) {
        parts.push_back(p);
    }
                    //adding multiple parts to a single object. All parts behave coherently. Used for attaching coin to lasso.

    void pause() { paused = true; }
    void unpause() { paused = false; }
    bool isPaused() { return paused; }
                    //end pause related functions


    //after this, declarations only here, definition later
    double getXPos();
    double getYPos();
    void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);
    void nextStep(double t);
    void getAttachedTo(MovingObject *m);

};

#endif
                    //begin definition of MovingObject functions


    void MovingObject::nextStep(double t) { //t is the step time
        if(paused) { return; }

        for(size_t i=0; i<parts.size(); i++){
            parts[i]->move(vx*t, vy*t);
        }
        vx += ax*t;
        vy += ay*t;
    }                                                       // End MovingObject::nextStep()


    double MovingObject::getXPos() {
        return (parts.size() > 0) ? parts[0]->getX() : -1;
    }
    double MovingObject::getYPos() {
        return (parts.size() > 0) ? parts[0]->getY() : -1;
    }                                                       //End get coordinate functions


    void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
        for(size_t i=0; i<parts.size(); i++){
            parts[i]->moveTo(argx, argy);
        }

        double angle_rad = angle_deg*PI/180.0;
        double argvx = speed*cos(angle_rad);
        double argvy = -speed*sin(angle_rad);               //conversion of polar to cartesian
        vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
    }                                                       // End reset function. Only Polar Arguments supported.

    void MovingObject::getAttachedTo(MovingObject *m) {
        double xpos = m->getXPos();
        double ypos = m->getYPos();                         //Matching trajectory of object with the argument.
        for(size_t i=0; i<parts.size(); i++){
            parts[i]->moveTo(xpos, ypos);
        }                                                   //Match all parts of calling object to the 0th part of object m (argument)
        initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
    }                                                       //Used for attaching coin to lasso


//END MovingObject


/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//defining macros

#ifndef __LASSO_H__
#define __LASSO_H__


#define WINDOW_X 800
#define WINDOW_Y 600 //canvas size

#define STEP_TIME 0.03 //time interval, reduced to 0.03 from 0.05 for smoother simulation.

#define PLAY_X_START 100
#define PLAY_Y_START 100
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-PLAY_Y_START)   //Play area size definition

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5                       //Lasso shooter positioning


#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG -90
#define MAX_RELEASE_ANGLE_DEG 450
#define INIT_RELEASE_ANGLE_DEG 45               //Lasso angle

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define INIT_RELEASE_SPEED 100                  //Lasso speed

#define COIN_SPEED 120;
#define COIN_ANGLE_DEG 90;                      //Coin speed

#define LASSO_G 30
#define COIN_G 30                               //Gravity constant throughout the game for the sake of reality.

#define LASSO_SIZE 10
#define COIN_SIZE 7                             //Size of the circle of the lasso and coins



/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Begin Coin class



#ifndef __COIN_H__
#define __COIN_H__


class Coin : public MovingObject {//inheriting MovingObject

    protected:                  //I inherit a "specialCoin" class from here later. Thus using protected data members
    double coin_start_x;
    double coin_start_y;
    double release_speed;
    double release_angle_deg;
    double coin_ax;
    double coin_ay;
    //Polar data members above

    //Moving parts
    Circle coin_circle;

    public:
                            //constructors
    Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
        release_speed = speed;
        release_angle_deg = angle_deg;
        coin_ax = argax;
        coin_ay = argay;

        initCoin();         //initialize coin (normal)
        //first constructor for normal Coin class, default and unchanged
    }

    Coin(double speed, double angle_deg, double argax, double argay, bool argpaused) : MovingObject(speed, angle_deg, argax, argay, argpaused, 1) {
        release_speed = speed;
        release_angle_deg = angle_deg;
        coin_ax = argax;
        coin_ay = argay;
        // Second constructor for specialCoin class
    }

  void initCoin();
  void resetCoin();

};

        //definition of functions in Coin class

    void Coin::initCoin() {         //initialise coin to base position
        coin_start_x = (PLAY_X_START+WINDOW_X)/2;
        coin_start_y = PLAY_Y_HEIGHT;
        coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
        coin_circle.setColor(COLOR("yellow"));

        coin_circle.setFill(true);
        addPart(&coin_circle);
    }


    void Coin::resetCoin() {        //reset coin to base position
        double coin_speed = COIN_SPEED;
        double coin_angle_deg = COIN_ANGLE_DEG;
        coin_ax = 0;
        coin_ay = COIN_G;
        bool paused = true, rtheta = true;
        reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
    }


//END Coin class

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Begin class specialCoin


class specialCoin : public Coin{//Inheriting coin class

    int type;   //specify type of coin, which determines its colour, features and worth.

    public:

    specialCoin(double x, double y, double speed, double angle_deg, double argax, double argay, bool argpaused, int typecoin) :
        Coin( speed,  angle_deg,  argax,  argay,  argpaused){   // constructor to assign speed, direction and acceleration.
        type=typecoin;
        initspecial(x,y);                                       //assign position and color/type
    }

    void initspecial(double x, double y);
    void resetspecial();
    void repeatspecial();
    int whichtype(){return type;}
};
//END specialCoin declaration


//Begin definition of specialCoin functions
    void specialCoin::initspecial(double x, double y){  // characterise coin with the colors and assign start position

        resetspecial();                                 //randomise the coin's position, speed and acceleration.
        coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);

                                                        //Each type has different effects upon capturing the coin.
        if(type==1){
            coin_circle.setColor(COLOR("yellow"));  //normal coins, +1
        }
        else if(type==2){
            coin_circle.setColor(COLOR("green"));   //for lasso, increases lasso loop size for 10 seconds
        }
        else if(type==3){
            coin_circle.setColor(COLOR("blue"));    //speed coins, increases lasso max speed for 10 seconds
        }
        else if(type==4){
            coin_circle.setColor(COLOR("red"));     //precious coins, +3
        }
        else{
            cout<<"Error!! Coin type incorrect"<<endl;
        }
        coin_circle.setFill(true);
        addPart(&coin_circle);
    }


    void specialCoin::resetspecial() {  //This is a very interesting function.
                                        //This function assigns value to the coin attributes randomly, making the game more impressive/interesting.
        coin_start_x=rand()%700+150;
        coin_start_y=rand()%500;

        if(type!=1)release_speed=rand()%60+150; //specialCoins move faster
        else release_speed=rand()%60+50;
        release_angle_deg=rand()%180;

        coin_ax= rand()%20-20;
        coin_ay= COIN_G;            // Only gravity is assumed constant, conventionally, for obvious reasons

        bool paused = true, rtheta = true;
        reset_all(coin_start_x, coin_start_y, release_speed, release_angle_deg, coin_ax, coin_ay, paused, rtheta);
    }


    void specialCoin::repeatspecial(){  //The coin behaviour at the end of every iteration is condensed into this function.

                                        //This function was made to reduce code redundancy since we would be dealing with several coins at once.
        nextStep(STEP_TIME);    //move the coin

        if(isPaused()) {        //start the coin if it is paused
        unpause();
        }

        if(getYPos()< 0 ||getYPos() > PLAY_Y_HEIGHT || getXPos()> WINDOW_X || getXPos()<PLAY_X_START) {
          resetspecial();       //reset coin if it goes out of play area
        }
    }
#endif
//End class specialCoin


/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Begin class Lasso


class Lasso : public MovingObject {//inheriting MovingObject

    protected:                  //I inherit a "betterLasso" class from here later. Thus using protected data members
    double lasso_start_x;
    double lasso_start_y;
    double release_speed;
    double release_angle_deg;
    double lasso_ax;
    double lasso_ay;

    int LASSO_RADIUS=50;        //these 2 were Macros which have been made into variables.
    int MAX_RELEASE_SPEED=200;  //Lasso size and speed can be changed with power-ups now.


    // Moving parts
    Circle lasso_circle;
    Circle lasso_loop;

    // Non-moving parts
    Line lasso_line;
    Line lasso_band;

    // State info
    bool lasso_looped;
    specialCoin *the_coin;      //specialCoin object
    Coin *old_coin;             //Coin object
    int num_coins;              //track of number of coins

    void initLasso();

    public:                     //constructor, polar being used
    Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta)
    {
        release_speed = speed;
        release_angle_deg = angle_deg;
        lasso_ax = argax;
        lasso_ay = argay;
        initLasso();
    }

    void yank();
    void addAngle(double angle_deg);
    void addSpeed(double speed);                //for control during playing

    void nextStep(double t);
    void check_for_coin(Coin *coin);
    void draw_lasso_band();
    int getNumCoins() { return num_coins; }     //background functions

};

#endif
// End class Lasso declaration


//Definitions of Lasso class functions

    void Lasso::draw_lasso_band() {
        double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
        double arad = release_angle_deg*PI/180.0;
        double xlen = len*cos(arad);
        double ylen = len*sin(arad);
        lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
        lasso_band.setThickness(LASSO_THICKNESS);
    } // End draw_lasso_band


    void Lasso::initLasso() {       //initializing the Lasso
        lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
        lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
        lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
        lasso_circle.setColor(COLOR("red"));
        lasso_circle.setFill(true);
        lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
        lasso_loop.setColor(COLOR("green"));
        lasso_loop.setFill(true);

        addPart(&lasso_circle);
        addPart(&lasso_loop);
        lasso_looped = false;
        the_coin = NULL;
        num_coins = 0;

        lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
        lasso_line.setColor(COLOR("blue"));
        lasso_band.setColor(COLOR("black"));
        draw_lasso_band();          //End of lots of simple steps

    } // End initialisation


    void Lasso::yank() {
        bool paused = true, rtheta = true;
        reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
        lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
        lasso_loop.setFill(true);
        lasso_looped = false;       //lasso back to original position
        if(the_coin != NULL) {
            num_coins++;
            the_coin->resetspecial();
            the_coin=NULL;          //coin counted and reset back
        }
    } // End yank()


    void Lasso::addAngle(double angle_deg) {    //only works when lasso is paused, at start position
        release_angle_deg += angle_deg;
        if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
        if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }    //base cases
        bool paused = true, rtheta = true;
        reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta); //reset lasso
    } // End addAngle()


    void Lasso::addSpeed(double speed) {        //only works when lasso is paused, at start position
        release_speed += speed;
        if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
        if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }                    //base cases
        bool paused = true, rtheta = true;
        reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);  //reset lasso
    } // End addSpeed()


    void Lasso::nextStep(double stepTime) {     //works only when lasso is unpaused
        draw_lasso_band();
        MovingObject::nextStep(stepTime);
        if(getYPos() > PLAY_Y_HEIGHT || getXPos()> WINDOW_X || getXPos()<PLAY_X_START) { yank(); }      //yank if going out of play area
        lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
    } // End nextStep()


    void Lasso::check_for_coin(Coin *coinPtr) { //called after looping the Lasso
        double lasso_x = getXPos();
        double lasso_y = getYPos();
        double coin_x = coinPtr->getXPos();
        double coin_y = coinPtr->getYPos();
        double xdiff = (lasso_x - coin_x);
        double ydiff = (lasso_y - coin_y);
        double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
        if(distance <= LASSO_RADIUS) {          //checking if coin is in the range
            old_coin = coinPtr;
            old_coin->getAttachedTo(this);      //coin moves with the lasso
        }
    } // End check_for_coin()


// End Lasso class

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Begin betterLasso class

class betterLasso : public Lasso {//inheriting Lasso class

    int LASSO_RADIUS=50;        //these 2 were Macros which have been made into variables.
    int MAX_RELEASE_SPEED=200;  //Lasso size and speed can be changed with power-ups now.


    public:                     //constructor
    betterLasso(double release_speed, double release_angle_deg, double lasso_ax, double lasso_ay, bool paused, bool rtheta):
          Lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta){;}


    void check_for_specialcoin(specialCoin *coinPtr);

    void bonus_biggerLasso();
    void bonus_fasterLasso();
    void bonus_plusTwo();
    void normal_coin_collect();//power-up functions

    void yank(clock_t &t);   //argument time for powerup-cooldown management

    void loopit();
    void unloopit();        //for resolving the bug in earlier program

    void draw_lasso_band();     //band length can increase now, as max possible speed increases
    void addSpeed(double speed);
    void resetpowers();         //cooldown function

};
//End bettrLasso declaration


//Begin betterLasso functions' definition
    void betterLasso::yank(clock_t &t) {
        bool paused = true, rtheta = true;
        int type;

        if(the_coin != NULL) {
            type=the_coin->whichtype(); //find out type of coin collected

            if(type==1){
                normal_coin_collect();
            }
            else{t=clock();}        //powerup management for special coins

            if(type==2){
                bonus_biggerLasso();    //green coin
            }
            else if(type==3){
                bonus_fasterLasso();    //blue coin
            }
            else if(type==4){
                bonus_plusTwo();        //red coin
            }

            num_coins++;
            the_coin->resetspecial();
            the_coin=NULL;
        }
        reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
        lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
        lasso_loop.setFill(true);
        lasso_looped = false;
        draw_lasso_band();

    return ;
    }


    void betterLasso::check_for_specialcoin(specialCoin *coinPtr) {//function almost same as check_for_coin, just with specialCoin pointer as argument

        double lasso_x = getXPos();
        double lasso_y = getYPos();
        double coin_x = coinPtr->getXPos();
        double coin_y = coinPtr->getYPos();
        double xdiff = (lasso_x - coin_x);
        double ydiff = (lasso_y - coin_y);
        double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));

        if(distance <= LASSO_RADIUS) {
            the_coin = coinPtr;
            the_coin->getAttachedTo(this);
        }
    } //End check_for_specialcoin()


    void betterLasso::draw_lasso_band() {   //redefined to enable band size be indicator of lasso speed with powerups
        double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
        double arad = release_angle_deg*PI/180.0;
        double xlen = len*cos(arad);
        double ylen = len*sin(arad);
        lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
        lasso_band.setThickness(LASSO_THICKNESS);
    } // End draw_lasso_band()


    void betterLasso::loopit() {
        if(lasso_looped) { return; } // Already looped
        lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
        lasso_loop.setFill(false);
        lasso_looped = true;
    } // End loopit()


    void betterLasso::unloopit(){//This function hides the loop to make the game more interesting.

        lasso_looped=false;
        lasso_loop.reset(getXPos(), getYPos(), LASSO_SIZE/2);
        lasso_loop.setFill(true);

    }


    void betterLasso::addSpeed(double speed) { //redefined to avoid clashes with original variable MAX_RELEASE_SPEED in parent class
        release_speed += speed;
        if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
        if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
        bool paused = true, rtheta = true;
        reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

    } //End addSpeed()


    ///The next few powerup functions involve drawing several rectangles to display when a coin is collected.
    ///The powerup ability is displayed on the canvas along with instructions (if any)

    void betterLasso::bonus_biggerLasso(){
        LASSO_RADIUS+=10;

        Rectangle blank(600,560,400,40);
        blank.setColor(COLOR(255,255,255));
        blank.setFill(true);
        blank.imprint();
        Text biggerLasso(600,550,"You Just collected a green coin");
        Text biggerLasso2(600,570,"Your loop size has increased temporarily");
        biggerLasso2.setColor(COLOR(0,150,0));
        biggerLasso.setColor(COLOR(0,150,0));
        biggerLasso2.imprint();
        biggerLasso.imprint();
    }

    void betterLasso::bonus_fasterLasso(){
        MAX_RELEASE_SPEED+=50;

        Rectangle blank(600,560,400,40);
        blank.setColor(COLOR(255,255,255));
        blank.setFill(true);
        blank.imprint();
        Text fasterLasso(600,550,"You Just collected a blue coin");
        Text fasterLasso2(600,570,"Your loop speed limit has increased temporarily. Press =.");
        fasterLasso.setColor(COLOR(0,0,150));
        fasterLasso2.setColor(COLOR(0,0,150));
        fasterLasso.imprint();
        fasterLasso2.imprint();
    }

    void betterLasso::bonus_plusTwo(){
        num_coins+=2;

        Rectangle blank(600,560,400,40);
        blank.setColor(COLOR(255,255,255));
        blank.setFill(true);
        blank.imprint();
        Text plustwo(600,550,"You Just collected a red coin");
        Text plustwo2(600,570,"You get 3 coins at once!!");
        plustwo.setColor(COLOR(200,0,0));
        plustwo2.setColor(COLOR(200,0,0));
        plustwo.imprint();
        plustwo2.imprint();

    }


    void betterLasso::normal_coin_collect(){
        Rectangle blank(600,560,400,40);
        blank.setColor(COLOR(255,255,255));
        blank.setFill(true);
        blank.imprint();

        Text normal(600,550,"You Just collected a yellow coin");
        Text normal2(600,570,"+1 !!");
        normal.setColor(COLOR(100,100,0));
        normal2.setColor(COLOR(100,100,0));
        normal.imprint();
        normal2.imprint();
    }

    void betterLasso::resetpowers(){
        Rectangle blank(600,560,400,40);
        blank.setColor(COLOR(255,255,255));
        blank.setFill(true);
        blank.imprint();

        LASSO_RADIUS=50;
        MAX_RELEASE_SPEED=200;
        addSpeed(+RELEASE_SPEED_STEP);
    }
//End betterLasso class

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void thankyou(int coins){//exit message when pressed q

    Text outromsg(400,100,"Thank you for playing! Please see your results in the Command Prompt.");

    //print results here. Score, time taken.
    string level;

    if(coins>=0 && coins<10){level="Newbie";}
    else if(coins>=10 && coins<20){level="Pupil";}
    else if(coins>=20 && coins<40){level="Apprentice";}
    else if(coins>=40 && coins<60){level="Specialist";}
    else if(coins>=60 && coins<80){level="Expert";}
    else if(coins>=80 && coins<100){level="Master";}
    else if(coins>100){level="Grandmaster";}

    cout<<endl<<endl<<endl<<endl<<endl<<endl<<"You collected "<<coins<<" coins. You are at "<<level<<" level. Hope you had a good time"<<endl;
    cout<<"See you soon! Bye!"<<endl;

    outromsg.imprint();

return;
}


//End of all classes and functions. Finally the main function begins.
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

main_program {

    ::srand(time(0));           //Generate new set of random numebers each time

    initCanvas("Lasso", WINDOW_X, WINDOW_Y);

    string name;

    //GAME BEGINS
    Text intro(400,100,"Welcome To LASSO. Please see the rules in the command prompt");


    cout<<"Please enter your name: "<<endl;
    getline(cin,name);
    intro.hide();

    cout
    <<"Hello "<<name<<". Welcome to my game of LASSO. "<<endl
    <<"This game has a single timed level. You can play with a keyboard or a mouse."<<endl<<endl
    <<"Please use either one of mouse or keyboard to play. You have to collect as many coins as you can."<<endl<<endl
    <<"You collect coins by throwing the Lasso and looping it when it comes near a coin."<<endl
    <<"When you yank/pull the lasso, you have collected the coin"<<endl<<endl
    <<"Keep an eye out for the special coins; the red, blue and green coins"<<endl<<endl
    <<"The blue coin would enable you to increase you lasso's speed even more"<<endl
    <<"The green coin would increase your Lasso's loop size, making it easier to collect coins"<<endl
    <<"The red coin is a precious one, its worth 3 yellow coins"<<endl<<endl
    <<"You have 180 seconds. The results will be shown after the game, or if you quit in the middle"<<endl<<endl

    <<"The Controls are as follows:-"<<endl
    <<"Press  t  to throw the lasso"<<endl
    <<"While the lasso is in the air, press  l  to loop/catch any coins near the tip of the lasso."<<endl
    <<"This will show you a green loop depicting the range of the lasso."<<endl
    <<"If any coin is present in the loop, you will be able to grab it."<<endl
    <<"Once you have looped a coin, press  y  to yank/pull the lasso and collect the coin."<<endl
    <<"You can adjust the projection speed of the lasso by pressing the  = (+) and the  -  keys."<<endl
    <<"The angle of projection can be varied by pressing the   [   and   ]   keys near the +,- keys on the keyboard."<<endl<<endl;

    cout<<"Go to the Canvas to start the GAME of LASSO!"<<endl;

    {
    Rectangle Startplay(400,100,200,30);
    Text startplay(400, 100, "Click here to Start the game!");

    Rectangle ControlBox(400,350,200,250);
    Text control_heading(400,275,"Controls-");

    Text control_throw(400,325,"t: Throw");
    Text control_yank(400, 350, "y: Yank/Pull");
    Text control_loop(400, 375, "l: Loop/Catch");
    Text control_angle(400, 400, "[ or ]: Angle control");
    Text control_speed(400, 425, "=(+) or -: Speed control");       //Display controls on Canvas

    while(true){
        int click = getClick();
        int x_begin = click/65536, y_begin = click % 65536;//get the coordinates of the click

        if(x_begin>300 && x_begin<500 && y_begin>85 && y_begin<115){//start box
            break;
        }
    }                                                               //START THE GAME

    Startplay.hide();
    startplay.hide();
    }

    auto start = std::chrono::system_clock::now();                  //External time management
    clock_t time_initial, time_current;                             //for powerup cooldown management


    int stepCount = 0;
    float stepTime = STEP_TIME;
    float runTime = -1; // sec; -ve means infinite
    float currTime = 0;                                             //Internal time management

    double release_speed = INIT_RELEASE_SPEED; // m/s
    double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
    double lasso_ax = 0;
    double lasso_ay = LASSO_G;
    bool paused = true;
    bool rtheta = true;
    betterLasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);    // Draw lasso at start position


    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR("black"));
    Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
    b2.setColor(COLOR("black"));                                //Draw the axes

    string msg("Last command: ");
    Text charPressed(PLAY_X_START+70, PLAY_Y_HEIGHT+20, msg);
    char coinScoreStr[256];
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);    //Show last command and coins collected

    paused = true; rtheta = true;
    double coin_speed = COIN_SPEED;
    double coin_angle_deg = COIN_ANGLE_DEG;
    double coin_ax = 0;
    double coin_ay = COIN_G;                                            //define base variables for coin

        specialCoin coin1(300,450,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 1);
        specialCoin coin2(400,350,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 1);
        specialCoin coin3(375,100,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 1);
        specialCoin coin4(125,225,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 1);
        specialCoin coin5(250,200,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 1);     //make 5 normal coins
        specialCoin green_coin(200,300,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 2);
        specialCoin blue_coin(400,300,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 3);
        specialCoin red_coin(500,400,coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, 4);  //make 1 special coin of each type


    //External time management variables
    int timeint;
    int timedec;
    char timep[100];

    //Mouse event variables
    int xclick;
    int yclick;


    ///Buttons for control via mouse
    Rectangle ctrl_box_t(50,150,50,20);
    Text ctrl_throw(50,150,"Throw");

    Rectangle ctrl_box_y(50,180,50,20);
    Text ctrl_yank(50,180,"Yank");

    Rectangle ctrl_box_l(50,210,50,20);
    Text ctrl_loop(50,210,"Loop");

    Rectangle ctrl_box_addspeed(50,240,60,20);
    Text ctrl_increase(50,240,"Speed++");

    Rectangle ctrl_box_minusspeed(50,270,60,20);
    Text ctrl_decrease(50,270,"Speed--");

    Rectangle ctrl_box_clockrotate(50,300,60,20);
    Text ctrl_clockwise(50,300,"Angle++");

    Rectangle ctrl_box_antirotate(50,330,60,20);
    Text ctrl_anticlockwise(50,330,"Angle--");

    Rectangle ctrl_box_q(50,550,50,20);
    Text ctrl_quit(50,550,"QUIT");


  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    bool mousepending = mouseButtonReleaseEvent(e);     //detects only when click is released. Not click and hold.


    if(pendingEv) {
    char c = charFromEvent(e);

    if(mousepending){//control via mouse

        xclick=e.xbutton.x;
        yclick=e.xbutton.y; //receive coordinates of mouse click, assign respective value to c

        if(xclick<75 && xclick>25 && yclick>140 && yclick<160){
            c='t';
        }
        else if(xclick<75 && xclick>25 && yclick>170 && yclick<190){
            c='y';
        }
        else if(xclick<75 && xclick>25 && yclick>200 && yclick<220){
            c='l';
        }
        else if(xclick<80 && xclick>20 && yclick>230 && yclick<250){
            c='=';
        }
        else if(xclick<80 && xclick>20 && yclick>260 && yclick<280){
            c='-';
        }
        else if(xclick<80 && xclick>20 && yclick>290 && yclick<310){
            c='[';
        }
        else if(xclick<80 && xclick>20 && yclick>320 && yclick<340){
            c=']';
        }
        else if(xclick<75 && xclick>25 && yclick>525 && yclick<575){
            c='q';
        }

        else{
            c=0;
        }

    }

      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);          //for printing last command

    switch(c) {                             //action on the basis of input command
        case 't'://throw
	lasso.unpause();
	break;

        case 'y'://yank
    {
	lasso.yank(time_initial);

    //powerup management start
    time_current = clock();
    double time_taken = ((double)(time_current-time_initial))/CLOCKS_PER_SEC; // in seconds


    if(time_taken>10){
        lasso.resetpowers();
    }
	break;
	}

        case 'l'://loop
	lasso.loopit();
	lasso.check_for_specialcoin(&coin1);
	lasso.check_for_specialcoin(&coin2);
	lasso.check_for_specialcoin(&coin3);
	lasso.check_for_specialcoin(&coin4);
	lasso.check_for_specialcoin(&coin5);

    lasso.check_for_specialcoin(&green_coin);
    lasso.check_for_specialcoin(&blue_coin);
    lasso.check_for_specialcoin(&red_coin);
	wait(STEP_TIME*5);
	lasso.unloopit();       //loop is not visible throughout for rest of the time
	break;

        case '['://angle++
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;

        case ']'://angle--
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;

        case '-'://speed--
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;

        case '='://speed++
    if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
    break;

        case 'q'://quit
        {
    thankyou(lasso.getNumCoins());
	exit(0);
        }

        default:
    break;
      }

    }
    //end dealing with input event

    lasso.nextStep(stepTime);
    coin1.repeatspecial();
    coin2.repeatspecial();
    coin3.repeatspecial();
    coin4.repeatspecial();
    coin5.repeatspecial();
    green_coin.repeatspecial();
    blue_coin.repeatspecial();
    red_coin.repeatspecial();       //take next step for all Moving objects


    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;//Internal time management


    //External Time management, display on canvas
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = now-start;

    timeint=diff.count();
    timedec= (diff.count()-timeint)*10;

    sprintf(timep, "Time: %d.%d", timeint, timedec);
    Text time(45,30, timep);

    if(timeint>150){
        time.setColor(COLOR("red"));
    }
    time.imprint();
    //Time management over

    if(timeint>=180){
        break;
    }


  } // End for(;;)
    thankyou(lasso.getNumCoins());
    wait(5);
    return 0;
} // End main_program
