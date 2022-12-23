#include "22.h"
#include <stdio.h>

void part_2_test_helper(NODE *this, NODE *that, 
FACING this_travel, FACING that_travel, 
FACING this_postdir, FACING that_postdir,
FACING this_whichedge, FACING that_whichedge)
{

    int i = 4;
    while(i>0)
    {

        printf("[%p], x:%4d,y:%4d",this,this->x,this->y);
        printf(" travel :");print_facing(this_travel);
        printf(" postdir :");print_facing(this_postdir);
        printf(" which edge :");print_facing(this_whichedge);

        printf("[%p], x:%4d,y:%4d",that,that->x,that->y);
        printf(" travel :");print_facing(that_travel);
        printf(" postdir :");print_facing(that_postdir);
        printf(" which edge :");print_facing(that_whichedge);
        printf("\n");

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

void init_part_2_test(void)
{
    init_part_1();
    print_nodes();
    NODE *this, *other;
    
/*

  1
234
  56

*/
//3top <-> 1left, 3: down, 1: right
printf("x");
this = &nodes[4][4]; other = &nodes[0][8];
part_2_test_helper(this,other,RIGHT,DOWN,DOWN,RIGHT,UP,LEFT);

//3bot <-> 5left, 3: up, 5: right
printf("x");
this = &nodes[7][4]; other = &nodes[11][8];
part_2_test_helper(this,other,RIGHT,UP,UP,RIGHT,DOWN,LEFT);

// 4right <-> 6top, 4:left, 6: down
printf("x");
this = &nodes[4][11]; other = &nodes[8][15];
part_2_test_helper(this,other,DOWN,LEFT,LEFT,DOWN,RIGHT,UP);

// 5bot <-> 2bot, 5: up, 2:up
printf("x");
this = &nodes[11][8]; other = &nodes[7][3];
part_2_test_helper(this,other,RIGHT,LEFT,UP,UP,DOWN,DOWN);

//6right <-> 1right, 6:left, 1:left
printf("x");
this = &nodes[8][15]; other = &nodes[4][11];
part_2_test_helper(this,other,DOWN,UP,LEFT,LEFT,RIGHT,RIGHT);

//6bot <-> 2left, 6:up, 2:right
printf("x");
this = &nodes[11][12]; other = &nodes[7][3];
part_2_test_helper(this,other,RIGHT,LEFT,UP,RIGHT,DOWN,LEFT);

// 2top <-> 1top, 1:bot, 2:bot
printf("x");
this = &nodes[4][3]; other = &nodes[0][8];
part_2_test_helper(this,other,LEFT,RIGHT,DOWN,DOWN,UP,UP);

}