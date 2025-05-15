#include<bits/stdc++.h>
using namespace std;


const double MIN_RANGE = 10;
const double MAX_RANGE = 100;

const double MIN_SPEED = 0.1;
const double MAX_SPEED = 20;

const double MIN_RATIO = 0.1;
const double MAX_RATIO = 0.9;


struct Sensor {
    double len;
    double time;
    double left, right;
    Sensor() : len(-1), time(-1), left(-1), right(-1) {}
};


// double myrand(double minval, double maxval, mt19937 gen) {
//     uniform_real_distribution<double> udist(minval, maxval);
//     return udist(gen);
// }


/// @brief 
/// @param n Number of sensors
/// @return Sensor list
vector<Sensor> make_random(int n) {
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<double> unirange(MIN_RANGE, MAX_RANGE);
    uniform_real_distribution<double> unispeed(MIN_SPEED, MAX_SPEED);
    uniform_real_distribution<double> uniratio(MIN_RATIO, MAX_RATIO);

    vector<Sensor> sensors(n);
    for (int i = 0; i < n; i++) {
        sensors[i].len = unirange(gen);
        sensors[i].time = sensors[i].len / unispeed(gen);
    }
    shuffle(sensors.begin(), sensors.end(), gen);

    sensors[0].left = 0;
    sensors[0].right = sensors[0].len;
    double lmost = 0, rmost = sensors[0].right, temp;
    for (int i = 1; i < n; i++) {
        double ratio = uniratio(gen);
        temp = max(lmost, rmost - sensors[i].len);
        sensors[i].left = temp + (rmost - temp) * ratio;
        sensors[i].right = sensors[i].left + sensors[i].len;
        lmost = sensors[i].left;
        rmost = sensors[i].right;
    }

    return sensors;
}


int main() {

    string outpath = "./input";

    ofstream fout;
    int tot = 10;   // number of test data
    int num = 1000; // number of sensors
    double length;

    fout.open(outpath, ios::out);
    fout << tot << "\n";
    fout.close();

    for (int i = 1; i <= 8; i++) {
        vector<Sensor> sensors = make_random(num);
        length = sensors.back().right;
        fout.open(outpath, ios::out | ios::app);
        fout << length << "\n" << num << "\n";
        for (int i = 1; i <= num; i++) {
            fout << sensors[i].left << " " << sensors[i].right << " " << sensors[i].time << "\n";
        }
        fout.close();
    }

    return 0;
}