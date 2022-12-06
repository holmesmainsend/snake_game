// SUBMIT EVERYTHING MONDAY

// TODO: add comments to finished program code

// OPTIONAL: add sounds


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>

/*
**Declare structs for project 
**segment -> stores coordinates of each snake segment
**trophy -> stores value, duration, and coordinates of the current trophy
** Mike and Sean
*/
struct segment {
    int x, y;
};

struct trophy {
    int x, y, value, dur;
};

/*
**Global variables 
**Used in multible functions 
**Mike and Sean
*/
//First run variable
bool initialRun = true;
//Growing
int growing = 0;
//Scoring
int score = 0;

/*
**Inputs: pointer to trophy struct, coordinates of snake head as x and y, and a boolean value to determine logic when the trophy is eaten
**Purpose: change location of trophy when trophy expires, increment score when trophy is eaten, and generate new trophy in both cases.
** Mike and Sean
*/
void newTrophy(struct trophy *trophy, int x, int y, bool eaten) {
    //increment score
    if(eaten)
        score += (*trophy).value;
    //value between 1 and 9
    (*trophy).value = (rand() % 9) + 1;
    //set duration
    (*trophy).dur = 10 - (*trophy).value;
    //sets position of new trophy within a set distance of snake, and inside the border
    do {
        (*trophy).x = ((rand() % 20) - 10) + x;
        (*trophy).y = ((rand() % 20) - 10) + y;
    }
    while((*trophy).x >= COLS || (*trophy).x <= 0 || (*trophy).y >= LINES || (*trophy).y <= 0);

    //print trophy
    move((*trophy).y, (*trophy).x);
    printw("%d", (*trophy).value);
    refresh();
}

/*
**Purpose: Print fail state message to screen
** Mike
*/
void youLose() {
    if(COLS > 67 && LINES > 3) {
        /*
         ___ ___  _______  _______      __     _______  _______  _______  __
        |   |   ||       ||   |   |    |  |   |       ||   ____||   ____||  |
         \     / |   -   ||   |   |    |  |__ |   -   ||____   ||   ____||__|
          |___|  |_______||_______|    |_____||_______||_______||_______||__|
        */
        move((LINES / 2) - 2, (COLS / 2) - 35);
        addstr(" ___ ___  _______  _______      __     _______  _______  _______  __ ");
        refresh();
        move((LINES / 2) - 1, (COLS / 2) - 35);
        addstr("|   |   ||       ||   |   |    |  |   |       ||   ____||   ____||  |");
        refresh();
        move((LINES / 2) - 0, (COLS / 2) - 35);
        addstr(" \\     / |   -   ||   |   |    |  |__ |   -   ||____   ||   ____||__|");
        refresh();
        move((LINES / 2) + 1, (COLS / 2) - 35);
        addstr("  |___|  |_______||_______|    |_____||_______||_______||_______||__|");
        refresh();
    }
    else {
        move(LINES / 2, (COLS / 2) - 5);
        addstr("YOU LOSE!");
        refresh();
    }
    sleep(3);
    endwin();
    clear();
}

/*
**Purpose: Initialize the game window and important game elements. Then run the game loop in a while loop. 
** MIke and Sean
*/
int main() {
    //array of segment positions on screen. index[0] is head of snake.
    int snakeMaxSize = 9999;
    struct segment snake[snakeMaxSize];

    // Initializing ncurses screen settings
    initscr();              //init screen
    clear();                //clear screen
    curs_set(0);            //make cursor invisible
    noecho();               //turn off input display
    keypad(stdscr,TRUE);    //toggle on keyboard input
    
    // Color themes (1 = 49 in ASCII)
    // Sean
    // Inits color combinations
    start_color();
    init_pair(49, COLOR_WHITE, COLOR_BLACK);     //classic theme
    init_pair(50, COLOR_BLACK, COLOR_WHITE);     //inverted
    init_pair(51, COLOR_GREEN, COLOR_RED);       //holiday
    init_pair(52, COLOR_RED, COLOR_YELLOW);      //condiments
    init_pair(53, COLOR_BLACK, COLOR_YELLOW);    //electric
    init_pair(54, COLOR_WHITE, COLOR_BLUE);      //seasnake
    init_pair(55, COLOR_BLACK, COLOR_BLACK);     //impossible

    int themeVal = 0;

    // Theme menu with built-in input validation
    // Sean
    // Prints main menu to screen and waits for input from user to enter the desired theme
    while(themeVal < 49 || themeVal > 55) {
        move((LINES / 2) - 2, (COLS / 2) - 9);
        addstr("SELECT THEME:");
        move((LINES / 2) - 1, (COLS / 2) - 9);
        addstr("1 = CLASSIC");
        move((LINES / 2), (COLS / 2) - 9);
        addstr("2 = INVERTED");
        move((LINES / 2) + 1, (COLS / 2) - 9);
        addstr("3 = HOLIDAY");
        move((LINES / 2) + 2, (COLS / 2) - 9);
        addstr("4 = CONDIMENTS");
        move((LINES / 2) + 3, (COLS / 2) - 9);
        addstr("5 = ELECTRIC");
        move((LINES / 2) + 4, (COLS / 2) - 9);
        addstr("6 = SEASNAKE");
        move((LINES / 2) + 5, (COLS / 2) - 9);
        addstr("7 = IMPOSSIBLE");
        refresh();
        themeVal = getch();
    }
    
    // Implementing theme based on user input
    attron(A_BOLD);
    attron(COLOR_PAIR(themeVal));
    refresh();

    // Creating snake pit border for given terminal window
    // Sean
    WINDOW * win = newwin(LINES - 1, COLS - 1, 0, 0);       //creates window equal to size of screen
    refresh();                                              //refresh output
    box(win, 0, 0);                                         //draw box equal to size of window/screen
    wrefresh(win);                                          //refresh output
    nodelay(stdscr, TRUE);                                  //disables pause when prompting for input  
    //Print score display
    move(0,0);
    addstr("Score:     ");
    refresh();

    // init snake in snake[]
    // Mike
    // generates first 5 segments values and stores them in snake[]. coordinates (0,0) are used to represent NULL values.
    for(int i = 0; i < snakeMaxSize; i++) {
        //THe number here sets the initial length of the snake
        if(i < 5) {
            //first 5 elements get coordinate values
            snake[0 + i].x = ((COLS - 1) / 5) + (5 - i);
            snake[0 + i].y = ((LINES - 1) / 2);
        }
        else {
            //all others get the coordinate (0,0) which we will use as a NULL value
            snake[i].x = 0;
            snake[i].y = 0;
        }
    }
    
    // Mike and Sean
    // Below are all values needed for Game Loop
    int key = 0;
    int dx = 1;
    int dy = 0;
    //temp array for shifting
    struct segment temp[snakeMaxSize];
    //temp single segment for moving snake
    struct segment temp1;
    temp1.x = 0;
    temp1.y = 0;
    //Trophy creation
    struct trophy trophy;
    struct trophy *pTrophy = &trophy;
    srand(time(NULL));
    newTrophy(pTrophy, snake[0].x, snake[0].y, 0);
    //Segment used for growth
    struct segment growSeg;
    growSeg.x = 0;
    growSeg.y = 0;
    //time storage
    int timer = 0;
    int prevTime = time(NULL);
    //init seg
    int seg = 0;

    // Game loop
    // Mike and Sean
    while(1) {
        // Mike and Sean 
        // pauses game loop after every frame for an amount of time proportional to the length of the current snake
        // 150000 initially (# of microseconds to pause 100,000 = .1 seconds)
        usleep(150000 - (score * 3000));
        timer = time(NULL);

        // Random initial vertical direction (or user input awaiting)
        // Sean
        if(initialRun == false) {
            key = getch();
        } else if(initialRun == true) {
            if((rand() % 2) == true) {
                key = KEY_UP;
            } else {
                key = KEY_DOWN;
            }
            initialRun = false;
        }

        // Updating snake direction or exiting based on key input
        // Mike and Sean
        // **doesn't change to opposite direction, instead of ending the game**
        switch(key) {
            case KEY_UP:
                if(dy != 1) {
                    dy = -1;
                    dx = 0;
                }
                //printw("UP");
                break;
            case KEY_DOWN:
                if(dy != -1) {
                    dy = 1;
                    dx = 0;
                }
                //printw("DOWN");
                break;
            case KEY_LEFT:
                if(dx != 1) {
                    dy = 0;
                    dx = -1;
                }
                //printw("LEFT");
                break;
            case KEY_RIGHT:
                if(dx != -1) {
                    dy = 0;
                    dx = 1;
                }
                //printw("RIGHT");
                break;
            case 'q':
                youLose();
                return 0;
                break;
        }

        // check collision
        // Mike 
        // collision with borders. Only the head needs to be checked. The head's location is checked to be within the current window's max coordinates
        if(snake[0].x == 0 || snake[0].x == COLS - 1 || snake[0].y == 0 || snake[0].y == LINES - 2) {
            youLose();
            return 0;
        }
        // collision with body
        // Mike
        // A while loop is used to check if the head of the current snake is located on the same location as any of its body segments
        seg = 1;
        while(snake[seg].x != 0 && snake[seg].y != 0) {
            if(snake[0].x == snake[seg].x && snake[0].y == snake[seg].y) {
                youLose();
                return 0;
            }
            seg++;
        }
        seg = 0;

        // move and grow snake
        // Mike and Sean
        // temp[] is used to store snake segment values and shift all values to the next index. snake[0] is the head of the snake
        // shift snake segments
        while(snake[seg + 1].x != 0 && snake[seg + 1].y != 0) {
            temp[seg + 1].x = snake[seg].x;
            temp[seg + 1].y = snake[seg].y;
            seg++;
        }
        //growing. This grows the snake by one segment in snake[]
        if(growing != 0) {
            temp[seg + 1].x = snake[seg].x;
            temp[seg + 1].y = snake[seg].y;
            growing--;
        }
        //if grow==0 then erase otherwise pause the eraser. This grows the snake by one segment on screen
        else {
            //erase last segment from terminal
            move(snake[seg].y, snake[seg].x);
            attroff(COLOR_PAIR(themeVal));
            addstr(" ");
            attron(COLOR_PAIR(themeVal));
            refresh();
        }
        seg = 1;
        //transfer temp back into snake
        while(temp[seg].x != 0 && temp[seg].y != 0) {
            snake[seg].x = temp[seg].x;
            snake[seg].y = temp[seg].y;
            seg++;
        }
        //move head by dy and dx. 
        snake[0].x = snake[1].x + dx;
        snake[0].y = snake[1].y + dy;
        seg = 0;

        // reprint head, and replace previous head with "#"
        // Updates the snake on screen to show the old head position as a body segment
        move(snake[0].y, snake[0].x);
        addstr("@");
        refresh();
        move(snake[1].y, snake[1].x);
        addstr("#");
        refresh();

        // collision with trophy
        // Mike and Sean
        // checks if head coordinates are on a trophy and calls newTrophy() with eaten = true.
        if(snake[0].x == trophy.x && snake[0].y == trophy.y) {
            growing += trophy.value;
            newTrophy(pTrophy, snake[0].x, snake[0].y, 1);
            prevTime = time(NULL);
            move(0, 7);
            printw("%d", score);
            refresh();
        }
        // if trophy wasn't eaten then check if trophy has expired
        // Mike and Sean
        // if timer expires then calls newTrophy() with eaten = false.
        else if((timer - prevTime) >= trophy.dur) {
            move(trophy.y, trophy.x);
            attroff(COLOR_PAIR(themeVal));
            addstr(" ");
            attron(COLOR_PAIR(themeVal));
            newTrophy(pTrophy, snake[0].x, snake[0].y, 0);
            prevTime = time(NULL);
        }

        // Check for win condition
        // Mike
        // Displays message when win condition is met.
        // Winning score is equal to half the perimeter's length. I think this will be way too large no matter the window size
        if(score >= COLS + LINES) {
            if(COLS > 67 && LINES > 3) {
                /*
                 ___ ___  _______  _______      ________  _______  _______  __ 
                |   |   ||       ||   |   |    |  |  |  ||_     _||    |  ||  |
                 \     / |   -   ||   |   |    |  |  |  | _|   |_ |       ||__|
                  |___|  |_______||_______|    |________||_______||__|____||__|                                                                                                                                                                                                           
                */
                move((LINES / 2) - 2, (COLS / 2) - 32);
                addstr(" ___ ___  _______  _______      ________  _______  _______  __ ");
                refresh();
                move((LINES / 2) - 1, (COLS / 2) - 32);
                addstr("|   |   ||       ||   |   |    |  |  |  ||_     _||    |  ||  |");
                refresh();
                move((LINES / 2) - 0, (COLS / 2) - 32);
                addstr(" \\     / |   -   ||   |   |    |  |  |  | _|   |_ |       ||__|");
                refresh();
                move((LINES / 2)  + 1, (COLS / 2) - 32);
                addstr("  |___|  |_______||_______|    |________||_______||__|____||__|");
                refresh();
            }
            else {
                move(LINES / 2, (COLS / 2) - 4);
                addstr("YOU WIN!");
            }
            sleep(3);
            break;
        }
    }
    endwin();
    clear();
    return 0;
}
