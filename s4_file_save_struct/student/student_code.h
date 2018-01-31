typedef struct point{
    int x;
    int y;
    int z;
} point_t;

int save(struct point* pt, size_t len, char* filename);