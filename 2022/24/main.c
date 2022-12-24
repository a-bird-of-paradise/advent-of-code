#include "structs.h"
#include "lex.yy.h"
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.h"
#include <stdbool.h>

BLIZZARD_LIST list;

int max_col = INT16_MIN, max_row = INT16_MIN, min_col = INT16_MAX, min_row = INT16_MAX;
char field[27][127];
unsigned int locations[27][127];

void fill_field(void)
{
    for(int i = 0; i <= max_row; i++) {
        for(int j = 0; j <= max_col; j++) {
            field[i][j]='.';
        }
    }

    BLIZZARD_LIST_ITEM *this = list.head;

    while(this)
    {
        if(field[this->b->row][this->b->col] == '.'){
            field[this->b->row][this->b->col]= this->b->d;
        }
        else if( field[this->b->row][this->b->col] == 'v' 
                || field[this->b->row][this->b->col] == '^' 
                || field[this->b->row][this->b->col] == '<' 
                || field[this->b->row][this->b->col] == '>') {
            field[this->b->row][this->b->col] = '2';
        }
        else {
            field[this->b->row][this->b->col]++;
        }
        this = this->next;
    }
}

void print_field(void)
{

    for(int i = 0; i <= max_row; i++) {
        for(int j = 0; j <= max_col; j++) {
            if(locations[i][j] != LOC_EMPTY) printf("%c",'a' -1 + locations[i][j]);
            else printf("%c",field[i][j]);
        }
        /* printf(" ");
        for(int j = 0; j <= max_col; j++) {
            printf("%c",field[i][j]);
        } */

        printf("\n");
    }
}

void tick()
{
    BLIZZARD_LIST_ITEM *this = list.head;

    while(this)
    {
        switch(this->b->d){
            case 'v':   this->b->row = mod(this->b->row+1,max_row+1); break;
            case '^':   this->b->row = mod(this->b->row-1,max_row+1); break;
            case '<':   this->b->col = mod(this->b->col-1,max_col+1); break;
            case '>':   this->b->col = mod(this->b->col+1,max_col+1); break;
        }
        this=this->next;
    }
}

bool update_locations()
{

    unsigned int prev_locations[27][127];
    int kmin, kmax, lmin, lmax;

    static bool can_start_return = false, can_start_final = false;

    for(int i = 0; i <= max_row; i++)
        for(int j = 0; j <= max_col; j++)
            prev_locations[i][j] = locations[i][j];

    for(int i = 0; i <= max_row; i++) {
        for(int j = 0; j <= max_col; j++) {
            locations[i][j] = LOC_EMPTY;
            if(field[i][j] == '.') {
                kmin = max(0, i-1);
                kmax = min(max_row, i+1);
                lmin = max(0, j-1);
                lmax = min(max_col,j+1);
                for(int k = kmin; k <= kmax; k++) {
                    if(prev_locations[k][j] & LOC_OUT) {
                        locations[i][j] |= LOC_OUT;
                    }
                    if(prev_locations[k][j] & LOC_RETURN) {
                        locations[i][j] |= LOC_RETURN;
                    }
                    if(prev_locations[k][j] & LOC_FINAL) {
                        locations[i][j] |= LOC_FINAL;
                    }
                }
                for(int l = lmin; l <= lmax; l++) {
                    if(prev_locations[i][l] & LOC_OUT) {
                        locations[i][j] |= LOC_OUT;
                    }
                    if(prev_locations[i][l] & LOC_RETURN) {
                        locations[i][j] |= LOC_RETURN;
                    }
                    if(prev_locations[i][l] & LOC_FINAL) {
                        locations[i][j] |= LOC_FINAL;
                    }
                }
            }
        }
    }
    // init: if [0][0] empty, fill it (can always wait)
    if(field[0][0] == '.') locations[0][0] |= LOC_OUT;

    if(field[max_row][max_col] == '.' && can_start_return) locations[max_row][max_col] |= LOC_RETURN;
    if(field[0][0] == '.' && can_start_final) locations[0][0] |= LOC_FINAL;
    if(locations[max_row][max_col] & LOC_OUT) can_start_return = true;
    if(locations[0][0] & LOC_RETURN) can_start_final = true;

    return locations[max_row][max_col] & LOC_FINAL;
}

int main(int argc, char **argv)
{
    list.head = NULL;
    yylex();

    BLIZZARD_LIST_ITEM *this = list.head;

    while(this)
    {
        max_col = this->b->col > max_col ? this->b->col : max_col;
        max_row = this->b->row > max_row ? this->b->row : max_row;
        min_col = this->b->col < min_col ? this->b->col : min_col;
        min_row = this->b->row < min_row ? this->b->row : min_row;
        this=this->next;
    }

    //printf("%4d %4d %4d %4d\n", min_row,max_row,min_col,max_col);

    for(int i = 0; i <= max_row; i++) {
        for(int j = 0; j <= max_col; j++) {
            locations[i][j]=LOC_EMPTY;
        }
    }

    bool done = false;
    int counter = 1;

    fill_field();
    //print_field(); printf("\n");

    while(!done)
    {
        tick();
        fill_field();
        done = update_locations();
        //print_field(); printf("\n");
        counter++;
    }
    printf("%d\n",counter);
    return 0;
}