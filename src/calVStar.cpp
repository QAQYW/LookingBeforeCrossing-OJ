#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-5;

double calPower(double v, double c3, double c2, double c1, double c0) {
    return c3 * v * v * v + c2 * v * v + c1 * v + c0;
}

double calEnergy(double v, double c3, double c2, double c1, double c0) {
    return 1e3 / v * calPower(v, c3, c2, c1, c0);
}

// 三分法求单峰函数最值点
double calVStar(double c3, double c2, double c1, double c0) {
    double l = 0, r = 1e3, m1, m2, v = 1, t, e1, e2;
    while (fabs(calEnergy(l, c3, c2, c1, c0) - calEnergy(r, c3, c2, c1, c0)) >= EPS) {
        t = (r - l) / 3;
        m1 = l + t;
        m2 = r - t;
        e1 = calEnergy(m1, c3, c2, c1, c0);
        e2 = calEnergy(m2, c3, c2, c1, c0);
        if (e1 < e2) {
            r = m2;
        } else {
            l = m1;
        }
    }
    return (l + r) / 2;
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