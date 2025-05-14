#include <bits/stdc++.h>
using namespace std;

struct Coefficient {
    double c3, c2, c1, c0;
};
Coefficient coeffList[] = {
    {0.07, 0.0391, -13.196, 390.95}
};
double vStarList[] = {13.99};
int index = 0;
// Coefficient coeff = coeffList[index];
// double vStar = vStarList[index];

double calPower(double v) {
    return coeffList[index].c3 * v * v * v + coeffList[index].c2 * v * v + coeffList[index].c1 * v + coeffList[index].c0;
}

int main() {
    
    return 0;
}