#include "funcs.h"

NODE *root_dir = 0;
NODE *current_dir = 0;

void init(void)
{
    root_dir = malloc(sizeof(NODE));

    char *root_dir_name = "/";

    root_dir->name = strdup(root_dir_name); // so can be freed
    root_dir->is_directory = true;
    root_dir->size = 0;
    root_dir->children = NULL;
    root_dir->next = NULL;
    root_dir->parent = root_dir;
}

NODE *find_child(NODE *base, char *name)
{
    NODE *this_node = base->children;

    while(this_node)
    {
        if(!strcmp(this_node->name,name)) break;
        this_node = this_node->next;
    }
    return this_node;
}

void add_child(NODE *base, char *name, unsigned long long size, bool is_directory)
{
    NODE *new_child = malloc(sizeof(NODE));
    new_child->children = NULL;
    new_child->name = strdup(name);
    new_child->next = NULL;
    new_child->size = size;
    new_child->is_directory = is_directory;
    new_child->parent = base;

    NODE *last_child;
    if(base->children){
        last_child = base->children;
        while(last_child->next)
        {
            last_child = last_child->next;
        }
        last_child->next = new_child;
    } else {
        base->children = new_child;
    }
}

void dir_print(NODE const * const to_print, unsigned int level)
{
    unsigned int thislevel = level;
    while(thislevel--) printf("|");

    printf("[%p][%p][%p] %s %llu\n",
        to_print,
        to_print->children,
        to_print->next,
        to_print->name,
        to_print->size);
    
    NODE *this_child = to_print->children;

    while(this_child)
    {
        dir_print(this_child, level + 1);
        this_child = this_child->next;
    }

}

void print_tree() 
{
    dir_print(root_dir,0);
}

unsigned long long calc_node_size(NODE * to_calc)
{
    unsigned long long answer = 0;
    NODE *next_child = to_calc->children;

    if(!to_calc->is_directory) return to_calc->size;

    while(next_child)
    {
        answer += calc_node_size(next_child);
        next_child = next_child->next;
    }

    to_calc->size = answer;

    return answer;

}

void set_node_sizes(void) { calc_node_size(root_dir);   }

unsigned long long sum_below_inner(NODE *to_sum, const unsigned long long x)
{
    unsigned long long answer = 0;

    NODE *next_child = to_sum->children;

    while(next_child)
    {
        if(next_child->is_directory)
        {
            answer += next_child->size <= x ? next_child->size : 0;
            answer += sum_below_inner(next_child,x);
        }
        next_child = next_child->next;
    }

    return answer;
}

void sum_below(const unsigned long long x)
{
    printf("%8llu\n",sum_below_inner(root_dir,x));
}

unsigned long long smallest_above(NODE *to_search, const unsigned long long x)
{
    unsigned long long answer = UINT64_MAX, child_answer;
    
    NODE *next_child = to_search->children;

    while(next_child)
    {
        if(next_child->is_directory)
        {
            answer = (next_child->size < answer && 
                        next_child->size >= x) ? next_child->size : answer;
            child_answer = smallest_above(next_child,x);

            answer = (child_answer < answer && child_answer >= x) ? child_answer : answer;
        }
        next_child = next_child->next;
    }

    return answer;
}

void smallest_suitable_dir(const unsigned long long disk_size, const unsigned long long needed)
{
    printf("%8llu\n",smallest_above(root_dir, needed - (disk_size - root_dir->size)));
}