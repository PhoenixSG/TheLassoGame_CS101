# TheLassoGame_CS101
This is a game made using simpleCPP as part of my CS101 course in Semester-1 at IITB


Hello! Thank you for downloading my Game of Lasso. Below is a video link where I
explain my code and details about my program. The controls/instructions are a part of
the game itself.
Video Link-
https://drive.google.com/drive/folders/1oQf05eBF0qBiyK73Cc4alrmjebNawRij?usp=sharing

Below is a description of my experience with the project and features I have
added-

-Bugs Solved from original code-
1. Earlier, you could increase the number of coins simply by pressing 'y' once you
had caught a single coin. The pointer was not set to reset after yanking. This has
been corrected.
2. Earlier, once one had pressed the 'l' key, the loop would remain visible until the
lasso was yanked. This gave the player an unfair advantage. He was able to see
when a coin was in the range. This has been considered as a bug and has been
removed.



-I have added the following features to the game-
1. Multiple coins come on the screen at random points of time and from random
directions. The coins have different speeds and accelerations.
2. The game is timed and you are given a rating based on the number of coins you
have collected.
3. Coins are color coded and collecting some special coins has a special effect on
the lasso.*
4. These coins have the following powers- 

      a. Blue- Can increase Speed of Lasso
    
      b. Green- Can increase Loop size of Lasso5.
    
      c. Red- Worth 3 Yellow(normal) coins

5. You can play with the MOUSE!. There are buttons on the Canvas which enable
you to play just with the click of a mouse.
6. Time Control has been added to the game. A timer is present on the top left
which indicates how much time is left in the game. Time management has also
been used to shut off the powerups effects after some time of collecting the coin.
7. An Attempt has been made to make the game more graphical and user friendly.
Several colours are given to coins and Lasso. Text boxes and messages come
on the screen throughout the game.
8. Automatic yanking and coin collection if lasso goes out of screen


*To do this I have made a derived class 'specialCoin' from the original Coin class, which
has all the new features. It associates a number with the coin, denoting its type. I have
also created a class called 'betterLasso' which inherits the original Lasso class and
implements the features like speed and loop size variation.


-Final Comments/Tips
1. The lasso can only hold one coin at a time. The priority is
red>blue>green>yellow.
2. The lasso cannot collect a coin while it is paused. You need to throw it first.
3. Please use either mouse or keyboard to play. Both simultaneously mess with the
XEvent and related functions.
4. Every time you collect a special coin, the cooldown resets. Use it to your
advantage to use multiple powers at the same time.
5. Enjoy!!
