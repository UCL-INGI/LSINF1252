typedef struct point{
    double x;
    double y;
    double z;
} point_t;

int save(struct point* pt, size_t len, char* filename);