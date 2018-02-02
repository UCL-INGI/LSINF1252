static int c = 0;
if(c == 0)
    c = 1;
else
    c *= 2;
if(c > 4096)
    c = 1;
return c;