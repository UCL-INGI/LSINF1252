typedef struct {
    double coeff[10];
} poly;

double eval(poly *P, double x);
void derivee(poly *P, poly *Pderiv);
double racine(poly *P, double x0);
