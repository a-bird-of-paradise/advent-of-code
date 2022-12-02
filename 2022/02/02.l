%option noyywrap main

%{
#include <stdio.h>
#include <stdlib.h>
long score = 0, score2 = 0;
%}

%%

A\ X    {   score += (3 + 1);   score2 += (0 + 3);  }
A\ Y    {   score += (6 + 2);   score2 += (3 + 1);  }
A\ Z    {   score += (0 + 3);   score2 += (6 + 2);  }

B\ X    {   score += (0 + 1);   score2 += (0 + 1);  }
B\ Y    {   score += (3 + 2);   score2 += (3 + 2);  }
B\ Z    {   score += (6 + 3);   score2 += (6 + 3);  }

C\ X    {   score += (6 + 1);   score2 += (0 + 2);  }
C\ Y    {   score += (0 + 2);   score2 += (3 + 3);  }
C\ Z    {   score += (3 + 3);   score2 += (6 + 1);  }

\r?\n   {   }

<<EOF>> {   printf("%8ld %8ld\n",score,score2); return  0;  }

%%