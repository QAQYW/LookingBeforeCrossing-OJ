#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace std::chrono;


struct Coefficient {
    double c3, c2, c1, c0;
};
Coefficient coeffList[] = {
    {0.07, 0.0391, -13.196, 390.95},
    {0.03, 0.08, -16.3, 180},
    {0.274, 0.06, -16.3, 60},
    {0.37, 0.136, -31.6, 110},
    {0.37, 0.136, -110, 730},
    {0.37, 0.136, -150, 1160}
};
double vStarList[] = {13.9895, 13.9915, 4.7478, 5.2367, 9.8939, 11.5556};
int index = 5;
Coefficient coeff = coeffList[index];
double vStar = vStarList[index];


double calPower(double v) {
    return coeff.c3 * v * v * v + coeff.c2 * v * v + coeff.c1 * v + coeff.c0;
}


double calEnergy(double len, double v) {
    double p = calPower(v);
    double e = len / v * p;
    return e;
}

double calEnergy(const vector<double> &pos, const vector<double> &speed) {
    int sz = speed.size();
    double e = 0, t, v, len;
    for (int i = 0; i < sz; i++) {
		if (i > 0) len = pos[i] - pos[i - 1];
		else len = pos[i];
        v = speed[i];
        t = calEnergy(len, v);
        e += t;
    }
    return e;
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
        // pos.emplace_back(0);

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
        pos.clear();
        speed.clear();
        // pos.emplace_back(0);

        double last = 0;
        for (int i = 1; i <= n; i++) {
            double v = (sensors[i].r - last) / sensors[i].t;
            pos.emplace_back(sensors[i].r);
            speed.emplace_back(v);
            // speed.emplace_back(min(v, vStar));
            last = sensors[i].r;
        }
    }

    /// @brief assign the overlapping range to the right sensor, min{v, vStar}
    /// @param pos 
    /// @param speed 
    void solve_AllSouth(vector<double> &pos, vector<double> &speed) {
        pos.clear();
        speed.clear();
        // pos.emplace_back(0);
        
        for (int i = 1; i <= n; i++) {
            double v = (sensors[i + 1].l - sensors[i].l) / sensors[i].t;
            pos.emplace_back(sensors[i + 1].l);
            speed.emplace_back(v);
            // speed.emplace_back(min(v, vStar));
        }
    }
};


int main() {
    auto gst = high_resolution_clock::now();

    int T;
    string IN_PATH = "./input-test";
    string OUT_PATH = "./output-test";

    ifstream fin(IN_PATH);
    ofstream fout(OUT_PATH, ios::trunc);
    fout.close();

    // cin >> T;
    fin >> T;
    cout << "Read T = " << T << "\n";
    while (T--) {
        auto st = high_resolution_clock::now();

        // Problem prob = Problem();
        Problem prob = Problem(fin);
        vector<double> pos;
        vector<double> speed;
        prob.solve(pos, speed);

        auto ed = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(ed - st);

        double energy = calEnergy(pos, speed);
        double optimal = energy;

        // // test output
        // for (double d : pos) {cout << d << " ";}
        // puts("");
        // for (double v : speed) {cout << v << " ";}
        // puts("");

        fout.open(OUT_PATH, ios::out | ios::app);
        int sz = pos.size();
        fout << sz << "\n";
        for (int i = 0; i < sz; i++) {
            fout << pos[i];
            if (i == sz - 1) fout << '\n';
            else fout << ' ';
        }
        // sz = speed.size();
        for (int i = 0; i < sz; i++) {
            fout << speed[i];
            if (i == sz - 1) fout << '\n';
            else fout << ' ';
        }

        // fout << "time = " << duration.count() << " ms\n";
        // fout << "energy = " << to_string(energy) << "\n";

        fout.close();
        
        cout << "Looking\n";
        cout << "\ttime = " << duration.count() << " ms\n";
        cout << "\tenergy = " << to_string(energy) << "\n";

        prob.solve_AllNorth(pos, speed);
        energy = calEnergy(pos, speed);
        cout << "All North\n";
        cout << "\tenergy = " << to_string(energy) << "\n";
        cout << "\t" << to_string(energy / optimal * 100) << "\% of optimal\n";

        prob.solve_AllSouth(pos, speed);
        energy = calEnergy(pos, speed);
        cout << "All South\n";
        cout << "\tenergy = " << to_string(energy) << "\n";
        cout << "\t" << to_string(energy / optimal * 100) << "\% of optimal\n";

        puts("");
    }
    
    fin.close();

    auto ged = high_resolution_clock::now();
    auto gduration = duration_cast<milliseconds>(ged - gst);
    cout << "time = " << gduration.count() << " ms\n";

    puts("Over");
    
    return 0;
}