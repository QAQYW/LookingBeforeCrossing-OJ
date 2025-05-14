#include <bits/stdc++.h>
using namespace std;

double calPower(double v, double c3, double c2, double c1, double c0) {
    return c3 * v * v * v + c2 * v * v + c1 * v + c0;
}

double calVStar(double c3, double c2, double c1, double c0) {
    //
}

int main() {
    double c3, c2, c1, c0;
    while (1) {
        cout << "Input coefficients:\n";
        cout << "c3 = "; cin >> c3;
        cout << "c2 = "; cin >> c2;
        cout << "c1 = "; cin >> c1;
        cout << "c0 = "; cin >> c0;
        cout << "vStar = " << calVStar(c3, c2, c1, c0) << "\n";
        cout << "\n---------------------------------------\n";
    }
    return 0;
}