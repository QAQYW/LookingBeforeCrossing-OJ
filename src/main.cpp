#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace std::chrono;


struct Coefficient {
    double c3, c2, c1, c0;
};
Coefficient coeffList[] = {
    {0.07, 0.0391, -13.196, 390.95}
};
double vStarList[] = {13.9895};
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

    /// @brief Read from file & Initialization 
    /// @param fin 
    Problem(ifstream &fin) {
        fin >> len >> n;
        sensors = vector<Sensor>(n + 2);
        for (int i = 1; i <= n; i++) {
            fin >> sensors[i].l >> sensors[i].r >> sensors[i].t;
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
                    dNext = sensors[iViewSouth + 1].l;
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

    /// @brief assign the overlapping range to the left sensor, min{v, vStar}
    /// @param pos 
    /// @param speed 
    void solve_AllNorth(vector<double> &pos, vector<double> &speed) {

    }

    /// @brief assign the overlapping range to the right sensor, min{v, vStar}
    /// @param pos 
    /// @param speed 
    void solve_AllSouth(vector<double> &pos, vector<double> &speed) {
        
    }
};


int main() {
    int T;
    string IN_PATH = "./input";
    string OUT_PATH = "./output-test";

    ifstream fin(IN_PATH);

    // cin >> T;
    fin >> T;
    while (T--) {
        auto st = high_resolution_clock::now();

        Problem prob = Problem(fin);
        vector<double> pos;
        vector<double> speed;
        prob.solve(pos, speed);

        auto ed = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(ed - st);

        // test output
        // for (double d : pos) {cout << d << " ";}
        // puts("");
        // for (double v : speed) {cout << v << " ";}
        // puts("");
        pos.insert(pos.begin(), 0);
        ofstream fout;
        fout.open(OUT_PATH, ios::out | ios::app);
        int sz = pos.size();
        for (int i = 0; i < sz; i++) {
            fout << pos[i];
            if (i == sz - 1) fout << '\n';
            else fout << ' ';
        }
        sz = speed.size();
        for (int i = 0; i < sz; i++) {
            fout << speed[i];
            if (i == sz - 1) fout << '\n';
            else fout << ' ';
        }

        fout << "time = " << duration.count() << " ms\n";

        fout.close();
    }
    
    fin.close();

    return 0;
}