// TODO: record screencast demo of final gameplay
// TODO: add comments to finished program code

// OPTIONAL: add colors and style features, win screen, etc.
// OPTIONAL: implement scoring and score display
// TODO: add win condition (snake's length reaches half the perimeter of the border)
// TODO: account for trophy out of range of snake
// TODO: start snake with random direction
// TODO: implement increased speed when snake gets longer
// TODO: fix growth after trophy eating


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

struct segment {
    int x, y;
};

struct trophy {
    int x, y, value;
};

//Growing
int growing = 0;

void newTrophy(struct trophy *trophy) {
    //logic here prevents the trophy from being printed on the border
    (*trophy).x = (rand() % (COLS - 2)) + 1;
    (*trophy).y = (rand() % (LINES - 2)) + 1;
    //value between 1 and 9
    (*trophy).value = (rand() % 9) + 1;
    move((*trophy).y, (*trophy).x);
    printw("%d", (*trophy).value);
    refresh();
}

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

    // Creating snake pit border for given terminal window
    WINDOW * win = newwin(LINES - 1, COLS - 1, 0, 0);       //creates window equal to size of screen
    refresh();                                              //refresh output
    box(win, 0, 0);                                         //draw box equal to size of window/screen
    wrefresh(win);                                          //refresh output
    nodelay(stdscr, TRUE);                                  //disables pause when prompting for input  

    //init snake in snake[]
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

    //draw initial snake
    int seg = 0;
    while(snake[seg].x != 0 && snake[seg].y != 0) {
        move(snake[seg].y, snake[seg].x);
        if(seg == 0) {
            addstr("@");
            refresh();
        }
        else {
            addstr("#");
            refresh();
        }
        seg++;
    }
    seg = 0;

    //values needed for Game Loop
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
    newTrophy(pTrophy);
    //Segment used for growth
    struct segment growSeg;
    growSeg.x = 0;
    growSeg.y = 0;

    // Game loop
    while(1) {
        usleep(150000); //# of microseconds to pause 100,000 = .1 seconds

        // Waiting for user input
        key = getch();

        // Updating snake direction or exiting based on key input
        //doesn't change to opposite direction
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
                endwin();
                clear();
                return 0;
        }

        //check collision
        //collision with borders
        if(snake[0].x == 0 || snake[0].x == COLS - 1 || snake[0].y == 0 || snake[0].y == LINES - 2) {
            endwin();
            clear();
            return 0;
        }
        //collision with body
        seg = 1;
        while(snake[seg].x != 0 && snake[seg].y != 0) {
            if(snake[0].x == snake[seg].x && snake[0].y == snake[seg].y) {
                endwin();
                clear();
                return 0;
            }
            seg++;
        }
        seg = 0;

        //move snake
        //shift snake segments
        while(snake[seg + 1].x != 0 && snake[seg + 1].y != 0) {
            temp[seg + 1].x = snake[seg].x;
            temp[seg + 1].y = snake[seg].y;
            seg++;
        }

        //erase last segment from terminal
        move(snake[seg].y, snake[seg].x);
        addstr(" ");
        refresh();

        seg = 1;
        //transfer temp back into snake
        while(snake[seg].x != 0 && snake[seg].y != 0) {
            snake[seg].x = temp[seg].x;
            snake[seg].y = temp[seg].y;
            seg++;
        }
        /*
        //check for growth
        if(growing != 0) {
            growing--;
            snake[seg + 1].x = snake[seg].x;
            snake[seg + 1].y = snake[seg].y;
            
        }*/

        //move head by dy and dx
        snake[0].x = snake[1].x + dx;
        snake[0].y = snake[1].y + dy;
        seg = 0;

        //reprint head, and replace previous head with "#"
        move(snake[0].y, snake[0].x);
        addstr("@");
        refresh();
        move(snake[1].y, snake[1].x);
        addstr("#");
        refresh();

        //collision with trophy
        if(snake[0].x == trophy.x && snake[0].y == trophy.y) {
            growing += trophy.value;
            newTrophy(pTrophy);
        }
    }
}
