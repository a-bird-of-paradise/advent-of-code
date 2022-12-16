%option 8bit noyylineno noyywrap main fast align

%{

typedef struct point { long x; long y; } POINT;
typedef struct pair { POINT first; POINT last; long tcab; } PAIR;

PAIR **list;
POINT first, last;
POINT *line;
void fill(long *x, long *y, char *text);
long excluded(const long y, const long min, const long max, const long flag);
long taxicab(const POINT a, const POINT b);
void search(const long n);

int n = 0;

%}

INT [+-]?[0-9]+

%%
"Sensor at "|": closest beacon is at "   {   /* skip */  }

"x="{INT}", y="{INT}/:   {   fill(&first.x,&first.y,yytext); }

"x="{INT}", y="{INT}$    {   fill(&last.x,&last.y,yytext);   }

\n  { if(n==0) list = calloc(0, sizeof(PAIR *));
    PAIR *new = malloc(sizeof(PAIR));
    new->first.x = first.x; new->first.y = first.y;
    new->last.x = last.x; new->last.y = last.y;
    new->tcab = taxicab(new->first, new->last);
    list = realloc(list, ++n * sizeof(PAIR *));
    list[n-1] = new;
}
<<EOF>> { 
    line = calloc(n, sizeof(POINT)); 
    printf("%20ld%20ld\n",
        excluded(10,INT32_MIN,INT32_MAX,1),
        excluded(2000000,INT32_MIN,INT32_MAX,1));
    search(20);
    search(4000000);
    free(line);
    return 0;
}

%%

void fill(long *x, long *y, char *text)
{
    char *comma = strchr(text,',');
    int comma_loc = comma-text;
    *comma = 0; *x = atoi(text+2);
    *comma = ',';   *y = atoi(text + comma_loc + 4);
}

long taxicab(const POINT a, const POINT b)
{
    return labs(a.x-b.x) + labs(a.y-b.y);
}

int cmp(const void *a, const void *b)
{
    if((*(POINT *)a).x < (*(POINT *)b).x) return -1;
    if((*(POINT *)a).x > (*(POINT *)b).x) return 1;
    if((*(POINT *)a).y < (*(POINT *)b).y) return -1;
    if((*(POINT *)a).y > (*(POINT *)b).y) return 1;
    return 0;
}

int cmplong(const void *a, const void *b)
{
    if((*(long *)a) < (*(long *)b)) return -1;
    if((*(long *)a) > (*(long *)b)) return 1;
    return 0;
}

long excluded(const long y, const long min, const long max, const long flag)
{
    long already_b = 0;

    for(int i = 0; i < n; i++)
    {
        long sb_dist = list[i]->tcab;
        long overlap = sb_dist - labs(y - list[i]->first.y);
        if(overlap < 0) {
            line[i].x = INT32_MIN; line[i].y = INT32_MIN;
        }
        else
        {
            line[i].x = list[i]->first.x - overlap; 
            line[i].y = list[i]->first.x + overlap;
            //lines totally outwith area are sentinelised
            if(line[i].y < min || line[i].x > max)
            {
                line[i].x = line[i].y = INT32_MIN;
            }
            // lines overlapping are trimmed
            else if(line[i].x < min) {
                line[i].x = min;
            }
            else if(line[i].y > max) {
                line[i].y = max;
            }
        }
    }
    qsort(line,n,sizeof(POINT),cmp);

    long answer = 0, open_x = INT32_MIN;

    for(int i = 0; i < n; i++)
    {
        if(line[i].x == INT32_MIN && line[i].y == INT32_MIN) continue; // skip sentinels

        if(open_x == INT32_MIN) {//first time
            open_x = line[i].x;
        } else if (line[i].y < line[i-1].y){ // segment contained within previous; extend this one
            line[i].y = line[i-1].y;
        } else if(line[i-1].y < line[i].x) { // close previous segment
            answer += line[i-1].y - open_x + 1;
            open_x = line[i].x;
            // if flag = 2 there is exactly one x, and we've found it, so return that
            if(flag == 2){
                answer = line[i].x-1;
                break;
            }
        }

        if(i == n-1) { // last time
            answer += line[i].y - open_x + 1;
        }

    }

    if(flag == 1)
    {
        long *xcludes = malloc(n * sizeof(long));
        for(int i = 0; i < n; i++)
        {
            xcludes[i] = INT32_MIN;
            if(list[i]->last.y == y || list[i]->first.y == y)  
                xcludes[i] = list[i]->last.x;
        }

        qsort(xcludes,n,sizeof(long),cmplong);

        for(int i = 0; i < n; i++)
        {
            if(xcludes[i] == INT32_MIN) continue;
            if(xcludes[i]!=xcludes[i-1]) already_b++;
        }
        free(xcludes);
    }
    return answer - already_b;
}

void search(const long n)
{
    for(long i = n; i>=0; i--){
        if(excluded(i,0,n,0) == n){
            long x = excluded(i,0,n,2);
            printf("%20ld%20ld%20ld\n",x,i,4000000*x+i);
            break;
        }
    }
}