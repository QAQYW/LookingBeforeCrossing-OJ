#include <bits/stdc++.h>
using namespace std;


struct Coefficient {
    double c3, c2, c1, c0;
};
Coefficient coeffList[] = {
    {0.07, 0.0391, -13.196, 390.95}
};
double vStarList[] = {13.98};
int index = 0;
Coefficient coeff = coeffList[index];
double vStar = vStarList[index];

double calPower(double v) {
    return coeff.c3 * v * v * v + coeff.c2 * v * v + coeff.c1 * v + coeff.c0;
}

struct Sensor {
    double l, r, t;
    Sensor(): l(0), r(0), t(0) {}
    Sensor(double _l, double _r, double _t): l(_l), r(_r), t(_t) {}
    // Sensor(Sensor _s): l(_s.l), r(_s.r), t(_s.t) {}
};


class Problem {
private:
    int n;      // number of sensors
    double len; // length of flight
    vector<Sensor> sensors; // list of sensors, first and last sensors (sensors[0] and sensors[n + 1]) are empty
public:
    /// @brief Read from terminal & Initialization 
    Problem() {
        cin >> len >> n;
        sensors = vector<Sensor>(n + 2);
        for (int i = 1; i <= n; i++) {
            cin >> sensors[i].l >> sensors[i].r >> sensors[i].t;
        }
        sensors[n + 1] = Sensor(sensors[n].r, sensors[n].r, 0);
    }

    /// @brief Looking Before Crossing
    /// @param pos list of speed changing positions
    /// @param speed list of speed in segments
    void solve(vector<double> &pos, vector<double> &speed) { 
        pos.clear();
        speed.clear();

        vector<double> tsum = vector<double>(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            tsum[i] = tsum[i - 1] + sensors[i].t;
        }

        double d = 0; // current distance
        for (int i = 0; i < n;) {
            int iNext;
            double dNext;
            double v;
            double vViewNorth = 1e9;
            double vViewSouth = 0;
            int iViewNorth = i;
            int iViewSouth = i;
            for (int j = i + 1; j <= n; j++) {
                double vDoorNorth = (sensors[j].r - d) / (tsum[j] - tsum[i]);
                double vDoorSouth = (sensors[j + 1].l - d) / (tsum[j] - tsum[i]);
                if (vDoorSouth > vViewNorth) {
                    v = vViewNorth;
                    iNext = iViewNorth;
                    dNext = sensors[iViewNorth].r;
                    break;
                }
                if (vDoorNorth < vViewSouth) {
                    v = vViewSouth;
                    iNext = iViewSouth;
                    dNext = sensors[iViewSouth + 1].s;
                    break;
                }
                if (vViewNorth > vDoorNorth) {
                    vViewNorth = vDoorNorth;
                    iViewNorth = j;
                }
                if (vViewSouth < vDoorSouth) {
                    vViewSouth = vDoorSouth;
                    iViewSouth = j;
                }
            }
            
            // if the northeast corner can be viewed directly
            if (vViewNorth == vViewSouth) { // todo 允许误差
                v = vViewNorth;
                iNext = n;
                dNext = sensors[n].r;
            }

            if (v > vStar) {
                // reconstruct rooms
                v = vStar;
            }

            i = iNext;
            d = dNext;

            pos.emplace_back(d);
            speed.emplace_back(v);
        }
    }
};


int main() {
    int T;
    // string IN_PATH = "./input";
    // string OUT_PATH = "./myanswer";

    cin >> T;
    while (T--) {
        Problem prob = Problem();
        Solver sol = Solver(&prob);
        vector<double> pos;
        vector<double> speed;
        prob.solve(pos, speed);
    }
    return 0;
}