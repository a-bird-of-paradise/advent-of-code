#include "22.h"
#include <stdio.h>

void part_2_real_helper(NODE *this, NODE *that, 
FACING this_travel, FACING that_travel, 
FACING this_postdir, FACING that_postdir,
FACING this_whichedge, FACING that_whichedge)
{

    int i = 50;
    while(i>0)
    {
/*
        printf("[%p], x:%4d,y:%4d",this,this->x,this->y);
        printf(" travel :");print_facing(this_travel);
        printf(" postdir :");print_facing(this_postdir);
        printf(" which edge :");print_facing(this_whichedge);

        printf("[%p], x:%4d,y:%4d",that,that->x,that->y);
        printf(" travel :");print_facing(that_travel);
        printf(" postdir :");print_facing(that_postdir);
        printf(" which edge :");print_facing(that_whichedge);
        printf("\n");
*/
        this->is_edge = true; 
        this->post_edge_facing = this_postdir;
        that->is_edge = true; 
        that->post_edge_facing = that_postdir;

        switch(this_whichedge) {
            case LEFT: this->left = that; break;
            case RIGHT: this->right = that; break;
            case UP: this->up = that; break;
            case DOWN: this->down = that; break;
        }

        switch(that_whichedge) {
            case LEFT: that->left = this; break;
            case RIGHT: that->right = this; break;
            case UP: that->up = this; break;
            case DOWN: that->down = this; break;
        }

        switch (this_travel)
        {
        case LEFT:  this=this->left; break;
        case RIGHT: this=this->right; break;
        case UP:    this=this->up; break;
        case DOWN:  this=this->down; break;
        }

        switch (that_travel)
        {
        case LEFT:  that=that->left; break;
        case RIGHT: that=that->right; break;
        case UP:    that=that->up; break;
        case DOWN:  that=that->down; break;
        }

        this->which_edge = this_whichedge;
        that->which_edge = that_whichedge;

        i--;
    }
}

void init_part_2_real(void)
{
    init_part_1();
    NODE *this, *other;
    alt = true;
/*

 AB         rows 0 - 49
 C          row 50-99
DE          row 100-149
F           row150-199

cols 0-49;50-99;100-149


Fr = Eb
Dt = Cl
Al = Dl
At = Fl
Cr = Bb
Br = Er
Bt = Fb

*/


for(int row = 0; row < 200; row++)
    for(int col = 0; col < 150; col++)
        nodes[row][col].label=' ';

for(int row = 0; row < 50; row++)
    for(int col = 50; col < 100; col++)
        nodes[row][col].label = 'A';

for(int row = 0; row < 50; row++)
    for(int col = 100; col < 150; col++)
        nodes[row][col].label = 'B';

for(int row = 50; row <100; row++)
    for(int col = 50; col < 100; col++)
        nodes[row][col].label = 'C';

for(int row = 100; row <150; row++)
    for(int col = 0; col < 50; col++)
        nodes[row][col].label = 'D';

for(int row = 100; row <150; row++)
    for(int col = 50; col < 100; col++)
        nodes[row][col].label = 'E';

for(int row = 150; row <200; row++)
    for(int col = 0; col < 50; col++)
        nodes[row][col].label = 'F';
/*
for(int row = 0; row < 200; row++){
    for(int col = 0; col < 150; col++){
        printf("%c",nodes[row][col].label);
    }
    printf("\n");

}for(int row = 0; row < 200; row++){
    for(int col = 0; col < 150; col++){
        printf("%c",nodes[row][col].value);
    }
    printf("\n");
}
*/

// Al = Dl
this = &nodes[0][50]; other = &nodes[149][0];
part_2_real_helper(this,other,DOWN,UP,RIGHT,RIGHT,LEFT,LEFT);

// Br = Er
this = &nodes[49][149]; other = &nodes[100][99];
part_2_real_helper(this,other,UP,DOWN,LEFT,LEFT,RIGHT,RIGHT);

// Bt = Fb
this = &nodes[0][100]; other = &nodes[199][0];
part_2_real_helper(this,other,RIGHT,RIGHT,DOWN,UP,UP,DOWN);

// Cr = Bb
this = &nodes[50][99]; other = &nodes[49][100];
part_2_real_helper(this,other,DOWN,RIGHT,LEFT,UP,RIGHT,DOWN);

// Fr = Eb
this = &nodes[150][49]; other = &nodes[149][50];
part_2_real_helper(this,other,DOWN,RIGHT,LEFT,UP,RIGHT,DOWN);

// Dt = Cl
this = &nodes[100][0]; other = &nodes[50][50];
part_2_real_helper(this,other,RIGHT,DOWN,DOWN,RIGHT,UP,LEFT);

// At = Fl
this = &nodes[0][50]; other = &nodes[150][0];
part_2_real_helper(this,other,RIGHT,DOWN,DOWN,RIGHT,UP,LEFT);
}

// 77225 too low
// 129014 too low
// 156166 just right
// 174082 too high