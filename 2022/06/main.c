#include <stdio.h>
#include <stdlib.h>

char *line = NULL;
size_t len = 0;
ssize_t linesize = 0;

void seenfunc(const int n)
{
    int seen[255] = {0};
    int seencount = 0;

    for(int i = 0; i < n; i++) seen[line[i]]++;
    for(int i = 0; i <255; i++) if(seen[i]>0) seencount++;

    for(int i = n; i < linesize; i++){
        if(seencount == n) {
            printf("%d\n", i);
            break;
        }
        seen[line[i-n]]--;
        seen[line[i]]++;
        seencount = 0;
        for(int i = 0; i <255; i++) if(seen[i]>0) seencount++;
    }
}

int main(int argc, char** argv)
{
    linesize = getline(&line, &len, stdin);

    seenfunc(4);
    seenfunc(14);

    if(line) free(line);

    return 0;
}