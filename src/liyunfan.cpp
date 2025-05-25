#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;

double calculate_optimal_speed() {
    double a = 1.11, b = 0.272, c = -150;
    return (-b + sqrt(b*b - 4*a*c)) / (2*a);
}

void solve_drone_flight(double D, int n, vector<vector<double>>& sensors) {
    double optimal_speed = calculate_optimal_speed();

    vector<double> speeds;
    vector<double> positions;

    double current_pos = 0;

    for (int i = 0; i < n; i++) {
        double L_i = sensors[i][0];
        double R_i = sensors[i][1];
        double t_i = sensors[i][2];

        if (current_pos < L_i) {
            speeds.push_back(optimal_speed);
            positions.push_back(L_i);
            current_pos = L_i;
        }

        double distance_in_range = R_i - current_pos;
        double required_speed = distance_in_range / t_i;

        speeds.push_back(required_speed);
        positions.push_back(R_i);
        current_pos = R_i;
    }

    if (current_pos < D) {
        speeds.push_back(optimal_speed);
        positions.push_back(D);
    }

    // cout << speeds.size() << endl;
    // for (size_t i = 0; i < speeds.size(); i++) {
    //     cout << fixed << setprecision(6) << speeds[i];
    //     if (i < speeds.size() - 1) cout << " ";
    // }
    // cout << endl;
    // for (size_t i = 0; i < positions.size(); i++) {
    //     cout << fixed << setprecision(6) << positions[i];
    //     if (i < positions.size() - 1) cout << " ";
    // }
    // cout << endl;
    ofstream fout("./test-special/lyf-output", ios::app | ios::out);
    fout << speeds.size() << endl;
    for (size_t i = 0; i < positions.size(); i++) {
        fout << positions[i];
        if (i < positions.size() - 1) fout << " ";
    }
    fout << endl;
    for (size_t i = 0; i < speeds.size(); i++) {
        fout << speeds[i];
        if (i < speeds.size() - 1) fout << " ";
    }
    fout << endl;
}

int main() {
    ifstream fin("./doc/input");
    int T;
    // cin >> T;
    fin >> T;
    for (int t = 0; t < T; t++) {
        double D;
        // cin >> D;
        fin >> D;
        int n;
        // cin >> n;
        fin >> n;
        vector<vector<double>> sensors(n, vector<double>(3));
        for (int i = 0; i < n; i++) {
            // cin >> sensors[i][0] >> sensors[i][1] >> sensors[i][2];
            fin >> sensors[i][0] >> sensors[i][1] >> sensors[i][2];
        }

        solve_drone_flight(D, n, sensors);
    }
    fin.close();
    return 0;
}