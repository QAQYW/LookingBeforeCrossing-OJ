#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <cassert>
#include <utility>
using namespace std;


#define _JUDGING    0
#define _AC         1
#define _FMT_ERR    2
#define _SOL_ERR    3

#define _AC_STR         "1"
#define _FMT_ERR_STR    "2"
#define _SOL_ERR_STR    "3"


char outfile[50];
char infile[50];
char tempoutfile[50];

const double ULP = 1e-3;

const double refanswer[10] = {
    1369518.856969,
    1444378.599822,
    2708622.067711,
    2560667.721779,
    2794243.558224,
    2493888.823353,
    2666444.579338,
    2480595.991079,
    2528497.274339,
    2709605.570435
};


struct Sensor{
    double l, r, t;
    Sensor(): l(-1), r(-1), t(-1) {}
    Sensor(double _l, double _r, double _t): l(_l), r(_r), t(_t) {}
};


double cal_power(double v) {
    return 0.37 * v * v * v + 0.136 * v * v - 150 * v + 1160;
}


class Problem{
private:
    int n;
    double len;
    vector<Sensor> sensors;
public:
    Problem(FILE *in_f) {
        fscanf(in_f, "%lf", &len);
        fscanf(in_f, "%d", &n);
        sensors = vector<Sensor> (n);
        for (int i = 0; i < n; i++) {
            fscanf(in_f, "%lf%lf%lf", &sensors[i].l, &sensors[i].r, &sensors[i].t);
        }
    }

    bool cal_energy(int sz, const vector<double> &pos, const vector<double> &speed, double &userans) {

        userans = 0;
        int sid = 0;
        int segid = 0;
        double curr = 0;
        while (sid < n) {
            // enter the range of next sensor
            while (curr < sensors[sid].l) {
                if (pos[segid] > sensors[sid].l) {
                    userans += (sensors[sid].l - curr) / speed[segid] * cal_power(speed[segid]);
                    curr = sensors[sid].l;
                    break;
                }
                userans += (pos[segid] - curr) / speed[segid] * cal_power(speed[segid]);
                curr = pos[segid++];
            }
            while (sensors[sid].t > ULP) {
                if (segid == sz) {
                    return false;
                }
                double d = sensors[sid].t * speed[segid];
                if (curr + d < sensors[sid].r && curr + d < pos[segid]) {
                    userans += sensors[sid].t * cal_power(speed[segid]);
                    curr += d;
                    break;
                }
                if (sensors[sid].r < pos[segid]) {
                    return false;
                }
                d = pos[segid] - curr;
                double t = d / speed[segid];
                sensors[sid].t -= t;
                userans += t * cal_power(speed[segid]);
                curr = pos[segid++];
            }
            ++sid;
        }
        while (len - curr > ULP) {
            if (segid == sz) {
                return false;
            }
            userans += (pos[segid] - curr) / speed[segid] * cal_power(speed[segid]);
            curr = pos[segid++];
        }
        return true;
    }

    double getLen() {return len;}
};

void judge(FILE *in_f, FILE *userout_f, FILE *stdout_f, int &judge_state, int &score, double refans) {
    // read input
    Problem prob = Problem(in_f);
    
    // read the student's output
    int sz;
    fscanf(userout_f, "%d", &sz);
    vector<double> pos(sz);
    vector<double> speed(sz);
    for (int i = 0; i < sz; i++) {
        fscanf(userout_f, "%lf", &pos[i]);
    }
    for (int i = 0; i < sz; i++) {
        fscanf(userout_f, "%lf", &speed[i]);
    }

    // format error
    if (fabs(pos.back() - prob.getLen()) > ULP) {
        judge_state = _FMT_ERR;
        return;
    }

    // calculate UAV energy consumption
    double userans = 0;
    bool solflag = prob.cal_energy(sz, pos, speed, userans);
    if (!solflag) {
        judge_state = _SOL_ERR;
        return;
    }

    // feasible solution, calculate score
    double ratio = (userans - refans) / refans;
    if (ratio <= 0.001) score += 10;
    else if (ratio <= 0.01) score += 9;
    else if (ratio <= 0.05) score += 8;
    else if (ratio <= 0.20) score += 6;
    else score += 4;
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("err: the number of args is %d, but 4 is needed\n", argc - 1);
        return 1;
    }

    strcpy(outfile, argv[1]);       // 标准答案
    strcpy(tempoutfile, argv[2]);   // 学生输出
    strcpy(infile, argv[3]);        // 测试数据

    FILE *in_f = fopen(infile, "r");
    FILE *userout_f = fopen(tempoutfile, "r");
    FILE *stdout_f = fopen(outfile, "r");

    int score = 0; // 记录学生得分
    int judge_state = _JUDGING;

    int T = 0;
    fscanf(in_f, "%d", &T);
    for (int i = 0; i < T; i++) {
        judge(in_f, userout_f, stdout_f, judge_state, score, refanswer[i]);
    }
    
    fclose(in_f);
    fclose(userout_f);
    fclose(stdout_f);

    if (judge_state == _JUDGING) { // accept
        judge_state = _AC;
    }

    string score_str = to_string(score);
    char res[50];
    char extra_info[50];
    if (judge_state == _AC) {
        strcpy(res, "Accepted");
        strcpy(extra_info, " ");
    // } else if (judge_state == _FMT_ERR) {
    //     strcpy(res, "AnswerWrong");
    //     strcpy(extra_info, "8");
    // } else if (judge_state == _SOL_ERR) {
    //     strcpy(res, "AnswerWrong");
    //     strcpy(extra_info, "8");
    } else {
        strcpy(res, "AnswerWrong");
        strcpy(extra_info, "8");
    }

    // 结果，得分，错误信息
    printf("%s %s %s", res, score_str.c_str(), extra_info);

    return 0;
}
