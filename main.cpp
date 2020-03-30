#include <iostream>
#include <unistd.h>
#include <curses.h>
#include <string>
using namespace std;

const char SNAKE_CHAR = '*';
const char FOOD_CHAR = 'o';
const int SNAKE_MAX_LEN = 10;
enum eDirection{STOP=0, LEFT, RIGHT, UP, DOWN};

int init_speed = 500000;
int snake_len = 0;
int border_x = 2;
int border_y = 2;
int food_x, food_y, score;
bool game_over = false;
int board_size = 20;
eDirection dir;

struct Snake{
    int x;
    int y;
};

struct Snake snake[SNAKE_MAX_LEN];

void draw_screen(WINDOW * win, int x, int y, int lines, int cols){
    int end_x = cols + x;
    int end_y = lines + y;
    for (int i=x; i<=end_x; i++){
        mvwaddch(win, y, i, ACS_HLINE);
    }
    for (int j=y+1; j<=end_y-1; j++){
        for (int i=x; i<=end_x; i++){
            if (i==x || i==end_x)
                mvwaddch(win, j, i, ACS_VLINE);
    }
    for (int i=x; i<=end_x; i++){
        mvwaddch(win, end_y, i, ACS_HLINE);
    }
    }
    
    mvwaddch(win, x, y, ACS_ULCORNER);
    mvwaddch(win, x, end_y, ACS_URCORNER);
    mvwaddch(win, end_x, y, ACS_LLCORNER);
    mvwaddch(win, end_x, end_y, ACS_LRCORNER);

    refresh();
}


void create_food(){
    food_x = rand() % (board_size - 1) + (border_x + 1);
    food_y = rand() % (board_size -1) + (border_y + 1);
}

void add_food(){
    mvaddch(food_y, food_x, FOOD_CHAR);
}
void init_snake(){
    snake[0].x = border_x + 3;
    snake[0].y = board_size/2;
}

void print_you_loose(){
    mvprintw(11, 11, "YOU LOSE!");
}

void print_score(){
    mvprintw(0, 0, ("SCORE: " + to_string(score)).c_str());
}

void move_snake(int x0, int y0){
    int prev_x = x0;
    int prev_y = y0;
    int curr_x, curr_y;
    if (snake_len>0){
        for (int i=1; i<=snake_len; i++){
            curr_x = snake[i].x;
            curr_y = snake[i].y;
            snake[i].x = prev_x;
            snake[i].y = prev_y;
            prev_x = curr_x;
            prev_y = curr_y;
        }
    }
}

void draw_snake(){
    for (int i=snake_len; i>=0; i--){
        mvaddch(snake[i].y, snake[i].x, SNAKE_CHAR);
    }
}

void input(){
    nodelay(stdscr, TRUE);
    int c = getch();    
    switch(c){
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case 'x':
            game_over = true;
            break;
    }
}

void try_switch(){
    int tempx = snake[0].x;
    int tempy = snake[0].y;
    switch(dir){
        case UP:
            tempy--;
            if (snake_len>0 && tempy==snake[1].y){
                snake[0].y++;
            }
            snake[0].y--;
            break;
        case DOWN:
            tempy++;
            if (snake_len>0 && tempy==snake[1].y){
                snake[0].y--;
            }
            snake[0].y++;
            break;
        case LEFT:
            tempx--;
            if (snake_len>0 && tempx==snake[1].x){
                snake[0].x++;
            }
            snake[0].x--;
            break;
        case RIGHT:
            tempx++;
            if (snake_len>0 && tempx==snake[1].x){
                snake[0].x--;
            }
            snake[0].x++;
            break;
    }
}

void logic(){

    int prev_x = snake[0].x;
    int prev_y = snake[0].y;

    try_switch();

    for(int i=1; i<=snake_len; i++){
        if (snake[i].x==snake[0].x && snake[i].y==snake[0].y && snake_len>0){

            print_you_loose();
            game_over=true;
            break;
        }
    }
    if (snake[0].x==border_x || snake[0].y==border_y || snake[0].x==border_x + board_size || snake[0].y==border_y + board_size){
        print_you_loose();
        game_over=true;
    }
    if (snake[0].x==food_x && snake[0].y==food_y){
        snake_len++;
        score++;
        create_food();
    }
    move_snake(prev_x, prev_y);
    draw_snake();
}

int main(){
    initscr();
    noecho();
    init_snake();
    create_food();
    while(!game_over){
        clear();
        noecho();
        keypad(stdscr, TRUE);
        draw_screen(stdscr, border_x, border_y, board_size, board_size);
        print_score();
        add_food();
        input();
        logic();
        refresh();
        usleep(init_speed);
    }  
}