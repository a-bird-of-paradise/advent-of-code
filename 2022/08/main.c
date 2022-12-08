#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int pmax(int x, int y) { return x > y ? x : y; }

int pmin(int x, int y) { return x < y ? x : y; }

int scenic_score(const int x, const int y, int **map, const int n)
{
    int answer, i, j, up, down, left, right;

    if(x == 0 || y == 0 || x == n-1 || y == n-1) return 0;

    // left
    left = 0; 
    for(j = y-1; j >= 0; j--)
    {
        left++;
        if(map[x][y] <= map[x][j]) break;
    }

    // right
    right = 0; 
    for(j = y+1; j < n; j++)
    {
        right++;
        if(map[x][y] <= map[x][j]) break;
    }

    // up
    up = 0;
    for(i = x + 1; i < n; i++)
    {
        up++;
        if(map[x][y] <= map[i][y]) break;
    }

    // down
    down = 0;
    for(i = x - 1; i >= 0; i--)
    {
        down++;
        if(map[x][y] <= map[i][y]) break;
    }

    // answer

    answer = left * right * up * down;

    return answer;
}

int main (int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t linesize = 1;

    int **map = 0, **scores = 0;
    int linecount = 0, linesize_orig, min, answer;

    bool **top_vis, **bottom_vis, **left_vis, **right_vis, **edge_vis, **total_vis;

    linesize = getline(&line, &len, stdin);
    linesize_orig = linesize - 1; // ignore trailing \n

    while(linecount < linesize_orig)
    {

        if(!map){
            map = malloc(linesize_orig * sizeof(*map));
            scores = malloc(linesize_orig * sizeof(*scores));
            top_vis = malloc(linesize_orig * sizeof(*top_vis));
            bottom_vis = malloc(linesize_orig * sizeof(*bottom_vis));
            left_vis = malloc(linesize_orig * sizeof(*left_vis));
            right_vis = malloc(linesize_orig * sizeof(*right_vis));
            edge_vis = malloc(linesize_orig * sizeof(*edge_vis));
            total_vis = malloc(linesize_orig * sizeof(*total_vis));

            for(int i = 0; i < linesize_orig; i++)
            {
                map[i] = malloc(linesize_orig * sizeof(map[0]));
                scores[i] = malloc(linesize_orig * sizeof(scores[0]));
                top_vis[i] = malloc(linesize_orig * sizeof(top_vis[0]));
                bottom_vis[i] = malloc(linesize_orig * sizeof(bottom_vis[0]));
                left_vis[i] = malloc(linesize_orig * sizeof(left_vis[0]));
                right_vis[i] = malloc(linesize_orig * sizeof(right_vis[0]));
                edge_vis[i] = malloc(linesize_orig * sizeof(edge_vis[0]));
                total_vis[i] = malloc(linesize_orig * sizeof(total_vis[0]));
            }
        }

        for(int i = 0; i < linesize_orig; i++)
        {
            map[linecount][i] = line[i] - '0';
            scores[linecount][i] = 0;
            top_vis[linecount][i] = false;
            bottom_vis[linecount][i] = false;
            left_vis[linecount][i] = false;
            right_vis[linecount][i] = false;
            edge_vis[linecount][i] = false;
            total_vis[linecount][i] = false;
        }
        linesize = getline(&line, &len, stdin) -1;

        linecount++;
    }

    // part 1; is this tree visible from that edge?

    for(int i = 0; i < linesize_orig; i++)
    {
        // top vis: true if this tree is taller than any before it
        min = 0;
        for(int j = 0; j < linesize_orig; j++)
        {
            if(map[i][j] > min)
            {
                min = map[i][j];
                top_vis[i][j] = true;
            }
        }
    }

    for(int i = 0; i < linesize_orig; i++)
    {
        // bot vis: true if this tree is taller than any after it
        min = 0;
        for(int j = linesize_orig-1; j >= 0; j--)
        {
            if(map[i][j] > min)
            {
                min = map[i][j];
                bottom_vis[i][j] = true;
            }
        }
    }

    for(int i = 0; i < linesize_orig; i++)
    {
        // right vis: true if this tree is taller than any after it
        min = 0;
        for(int j = linesize_orig-1; j >= 0; j--)
        {
            if(map[j][i] > min)
            {
                min = map[j][i];
                right_vis[j][i] = true;
            }
        }
    }

    for(int i = 0; i < linesize_orig; i++)
    {
        // left vis: true if this tree is taller than any before it
        min = 0;
        for(int j = 0; j < linesize_orig; j++)
        {
            if(map[j][i] > min)
            {
                min = map[j][i];
                left_vis[j][i] = true;
            }
        }
    }

    for(int i = 0; i < linesize_orig; i++)
        edge_vis[0][i] = edge_vis[linesize_orig-1][i] = edge_vis[i][0] = edge_vis[i][linesize_orig-1] = true;
    
    for(int i = 0; i < linesize_orig; i++)
        for(int j = 0; j < linesize_orig; j++)
            total_vis[i][j] = top_vis[i][j] || bottom_vis[i][j] || left_vis[i][j] || right_vis[i][j] || edge_vis[i][j];

    answer = 0;

    for(int i = 0; i < linesize_orig; i++)
        for(int j = 0; j < linesize_orig; j++)
            if(total_vis[i][j])
                answer++;

    printf("%8d\n",answer);

    // part 2: how scenic is this tree? 

    answer = 0;

    for(int i = 0; i < linesize_orig; i++)
    {
        for(int j = 0; j < linesize_orig; j++)
        {
           scores[i][j]=scenic_score(i,j,map,linesize_orig);
           answer = pmax(answer,scores[i][j]);
        }
    }

    printf("%8d\n",answer);

    // cleanup

    free(line);
    for(int i = 0; i < linesize_orig; i++)
    { 
        free(map[i]);
        free(scores[i]);
        free(top_vis[i]);
        free(bottom_vis[i]);
        free(left_vis[i]);
        free(right_vis[i]);
        free(edge_vis[i]);
        free(total_vis[i]);
    }
    free(map);
    free(scores);
    free(top_vis);
    free(bottom_vis);
    free(left_vis);
    free(right_vis);
    free(edge_vis);
    free(total_vis);

}