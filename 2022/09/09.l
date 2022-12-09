%option 8bit main yylineno noyywrap fast align

%{
#include <stdbool.h>
typedef struct location { int x; int y; } LOC;
LOC H, T, s;
typedef enum direction { UP, DOWN, LEFT, RIGHT } DIR;
void move(const DIR d, const int times);

LOC train[10];

unsigned long hash(const LOC x);
unsigned visited[1024 * 1024] = {0};
unsigned visited2[1024 * 1024] = {0};
void    init(void);
static bool do_init = true;
DIR     get_dir(const char d);
int     dist(const LOC x, const LOC y);
void    answer(unsigned *visits);
%}

%%

[LDUR]\ [0-9]+  {   if(do_init) init(); move(get_dir(yytext[0]), atoi(yytext+2));}
\n              {   }
<<EOF>>         {   answer(visited); answer(visited2);    return 0;   }
.+              {   printf("Unknown characters: %s\n", yytext); exit(1);    }

%%

void answer(unsigned *visits)
{
    unsigned answer = 0;
    for(unsigned long i = 0; i < 1024 * 1024; i++)
        if(visits[i]>0)
            answer++;
    printf("%8d\n",answer);
}

void move_inner(const DIR d, LOC *head)
{
    if(d == UP)  head->y++;
    else if(d == DOWN) head->y--;
    else if(d == LEFT) head->x--;
    else if(d == RIGHT) head->x++;
    else    {   printf("Unknown direction\n");  exit(1);    }
    
}

void drag_inner(LOC *head, LOC *tail, unsigned *update)
{

    // tail: overlap (do nothing)
    if(tail->x == head->x && tail->y == head->y) {}
    // or same row
    else if(tail->y == head->y){
        if(tail->x == head->x-2){
            tail->x++;
        }
        else if(tail->x == head->x+2) {
            tail->x--;
        }
    }
    // or same column
    else if(tail->x == head->x){
        if(tail->y == head->y-2){
            tail->y++;
        }
        else if(tail->y == head->y+2){
            tail->y--;
        }
    }
    // or diagonal - move by quadrant only if at least 2 steps away
    else if(head->x > tail->x && head->y > tail->y && dist(*head,*tail) > 2){
        tail->x++;  tail->y++;
    }
    else if(head->x > tail->x && head->y < tail->y && dist(*head,*tail) > 2){
        tail->x++;  tail->y--;
    }
    else if(head->x < tail->x && head->y > tail->y && dist(*head,*tail) > 2){
        tail->x--;  tail->y++;
    }
    else if(head->x < tail->x && head->y < tail->y && dist(*head,*tail) > 2){
        tail->x--;  tail->y--;
    }

    if(update) update[hash(*tail)]++;

}

void move(const DIR d, const int times)   
{   
    int this_times = times;
    while(this_times--) 
    {
        move_inner(d, &H);   
        drag_inner(&H, &T, visited);
    }
    this_times = times;
    while(this_times--)
    {
        move_inner(d,&train[0]);
        for(int i = 0; i < 8; i++)
        {
            drag_inner(train + i, train + i + 1, NULL);
        }
        drag_inner(train + 8, train + 9, visited2);
    }
}

unsigned long hash(const LOC x)
{
    return (1024*x.x + x.y);
}

void init(void)
{
    s.x = s.y = 512;
    H.x = H.y = 512;
    T.x = T.y = 512;
    do_init = false;
    for(int i = 0; i < 10; i++) train[i].x = train[i].y = 512;
    visited[hash(s)]++;
    visited2[hash(s)]++;
}

DIR get_dir(const char d)
{
    DIR answer;
    if(d == 'L') answer = LEFT;
    else if (d == 'R') answer = RIGHT;
    else if (d == 'U') answer = UP;
    else if (d == 'D') answer = DOWN;
    else { printf("Unknown direction: %c\n",d); exit(1);    }
    return answer;
}

int dist(const LOC x, const LOC y)
{
    int xdist, ydist, answer;
    xdist = y.x-x.x;
    xdist = xdist < 0 ? -xdist : xdist; 

    ydist = y.y-x.y;
    ydist = ydist < 0 ? -ydist : ydist;

    answer = xdist + ydist;

    return answer;
}