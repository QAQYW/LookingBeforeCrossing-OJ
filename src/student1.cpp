#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

struct Sensor {
    double L, R, t;
};

/* -------------------------------------------------------------------------- */
/*                                   hack 1                                   */
/* -------------------------------------------------------------------------- */

int main() {
    ifstream fin("./doc/input");
    ofstream fout("./test-special/hack1");
    int T;
    // cin >> T; 
    fin >> T;
    while (T--) {
        double D;
        int n;
        // cin >> D >> n;  
        fin >> D >> n;

        vector<Sensor> sensors(n);
        for (int i = 0; i < n; i++) {
            // cin >> sensors[i].L >> sensors[i].R >> sensors[i].t;
            fin >> sensors[i].L >> sensors[i].R >> sensors[i].t;
        }

        vector<double> positions, speeds;

        positions.push_back(sensors[0].R);
        speeds.push_back(sensors[0].R / sensors[0].t);

        for (int i = 1; i < n; i++) {
            double dist = sensors[i].L - sensors[i - 1].R;
            double speed = dist / sensors[i].t;
            positions.push_back(sensors[i].R);
            speeds.push_back(speed);
        }

        double dist = D - sensors[n - 1].R;
        double speed = dist / sensors[n - 1].t;
        positions.push_back(D);
        speeds.push_back(speed);

        // cout << positions.size() << endl;
        fout << positions.size() << endl;
        for (double pos : positions) {
            // cout << fixed << setprecision(6) << pos << " ";
            fout << fixed << setprecision(6) << pos << " ";
        }
        // cout << endl;
        fout << endl;
        for (double spd : speeds) {
            // cout << fixed << setprecision(6) << spd << " ";
            fout << fixed << setprecision(6) << spd << " ";
        }
        // cout << endl;
        fout << endl;
    }
    fin.close();
    fout.close();
    return 0;
}
