// NOTE: current TODOs only for intermediate, not for complete final project

// TODO: record screencast of gameplay
// TODO: add comments to finished program code
// TODO: add kill conditions for collision with snake pit border or snake segment
// TODO: make snake movement continuous based on last recorded arrow key input

/*
    I believe that we can use if(user input changes) then (run the switch code)

    That way we can continually move the snake even when there is no input

    Also we can store the direction instead of updating x and y and increment the head of the 
    snake by the direction
*/
// POTENTIAL HELP TO IMPLEMENT YOUR SOLUTION: https://stackoverflow.com/questions/4025891/create-a-function-to-check-for-key-press-in-unix-using-ncurses


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

int main() {
    // Explanation of these variables added below
    char snake[9999];
    char eraser[9999];
    char temporary[9999];

    // Adding initial snake and initial eraser size
    strcpy(snake, "----->");
    strcpy(eraser, "      ");

    // Initializing ncurses screen settings
    initscr();
    clear();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    // Creating snake pit border for given terminal window
    WINDOW * win = newwin(LINES - 1, COLS - 1, 0, 0);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    // Setting snake starting position
    int y = (LINES - 1) / 2;
    int x = (COLS - 1) / 5;

    int prevKey = 0;

    // Game loop
    while(1) {
        move(y,x);
        addstr(snake);
        refresh();
        sleep(1);
        
        // These lines are making the snake disappear
        move(y,x);
        addstr(eraser);
        
        // Waiting for user input
        int key = getch();

        // Checking if user reversed direction
        if((prevKey == KEY_UP && key == KEY_DOWN) || 
           (prevKey == KEY_DOWN && key == KEY_UP) ||
           (prevKey == KEY_RIGHT && key == KEY_LEFT) ||
           (prevKey == KEY_LEFT && key == KEY_RIGHT)) {
            endwin();
            return 0;
        }

        // Updating snake direction or exiting based on key input
        switch(key) {
            case KEY_UP:
                y -= 1;
                break;
            case KEY_DOWN:
                y += 1;
                break;
            case KEY_LEFT:
                x -= 1;
                break;
            case KEY_RIGHT:
                x += 1;
                break;
            case 'q':
                endwin();
                return 0;
        }

        // Updating user's last input
        prevKey = key;

        /*
            Snake growth code:
            As far as I could determine, C does not contain a simple way to add characters to the beginning
            of an array (like the unshift() method in JavaScript). Therefore, the code below transfers the
            current snake to a temporary placeholder, sets a "-" segment as the new snake array, and then
            appends the rest of the former snake to this by copying it over from temporary. The same
            process is repeated for the eraser of the old snake, except with a " " instead of a "-" char.
            Currently, the rate of growth is one snake segment per key press.
        */
        strcpy(temporary, snake);
        strcpy(snake, "-");
        strncat(snake, temporary, 9999);
        strcpy(temporary, eraser);
        strcpy(eraser, " ");
        strncat(eraser, temporary, 9999);
    }
}
