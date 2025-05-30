#include <bits/stdc++.h>
using namespace std;

struct Segment {
    double l, r, v;
};
int n;
double D;
vector<double> L, R, t, Tsum;
vector<Segment> segs;
double v_opt;   // ��Լ���ܺ������ٶ�
double solve_v(double C) {
    double v = v_opt;  // ��ʼ����Լ������
    for (int it = 0; it < 30; it++) {
        double F = 0.74 * v * v * v + 0.136 * v * v - C;
        double dF = 3 * 0.74 * v * v + 2 * 0.136 * v;
        v -= F / dF;
        if (v < 1e-6) v = 1e-6;
    }
    return v;
}

// �������� [idxL..idxR]������ [a..b] �ڵ���
void yds(int idxL, int idxR, double a, double b) {
    if (idxL > idxR) {
        if (b > a) segs.push_back({ a, b, v_opt });
        return;
    }
    // ���ܶ����� [i..k]
    double bestD = -1;
    int bi = -1, bk = -1;
    for (int i = idxL; i <= idxR; i++) {
        double li = max(L[i], a);
        for (int k = i; k <= idxR; k++) {
            double rk = min(R[k], b);
            if (rk <= li) continue;
            double sumt = Tsum[k] - Tsum[i - 1];
            double dens = sumt / (rk - li);
            if (dens > bestD) {
                bestD = dens;
                bi = i; bk = k;
            }
        }
    }
    double li = max(L[bi], a), rk = min(R[bk], b);
    double sumt = Tsum[bk] - Tsum[bi - 1];
    double v_star = (rk - li) / sumt;  // = 1/s*
    // ������
    yds(idxL, bi - 1, a, li);
    // �м�����
    segs.push_back({ li, rk, v_star });
    // ������
    yds(bk + 1, idxR, rk, b);
}

int main() {
    ifstream fin("./doc/input");
    ofstream fout("./test-special/hack2");
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    {
        double C = 1160.0;
        // ţ�ٵ���
        double v = 10.0;
        for (int it = 0; it < 50; it++) {
            double F = 0.74 * v * v * v + 0.136 * v * v - C;
            double dF = 3 * 0.74 * v * v + 2 * 0.136 * v;
            v -= F / dF;
        }
        v_opt = v;
    }

    int T;
    // cin >> T;
    fin >> T;
    cout << fixed << setprecision(6);
    while (T--) {
        // cin >> D >> n;
        fin >> D >> n;
        L.assign(n + 1, 0);
        R.assign(n + 1, 0);
        t.assign(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            // cin >> L[i] >> R[i] >> t[i];
            fin >> L[i] >> R[i] >> t[i];
        }
        // ǰ׺��
        Tsum.assign(n + 1, 0);
        for (int i = 1; i <= n; i++) Tsum[i] = Tsum[i - 1] + t[i];
        segs.clear();
        // �ݹ� YDS
        yds(1, n, 0.0, D);
        // �ϲ����ڡ���ͬ�ٶȵ�����
        vector<Segment> Q;
        sort(segs.begin(), segs.end(), [](auto& a, auto& b) { return a.l < b.l; });
        for (auto& s : segs) {
            if (!Q.empty() && abs(Q.back().v - s.v) < 1e-8 && abs(Q.back().r - s.l) < 1e-8) {
                Q.back().r = s.r;
            }
            else {
                Q.push_back(s);
            }
        }
        // ���
        int m = Q.size();
        // cout << m << "\n";
        fout << m << "\n";
        for (int i = 0; i < m; i++) {
            // cout << Q[i].r << (i + 1 < m ? ' ' : '\n');
            fout << Q[i].r << (i + 1 < m ? ' ' : '\n');
        }
        for (int i = 0; i < m; i++) {
            // cout << Q[i].v << (i + 1 < m ? ' ' : '\n');
            fout << Q[i].v << (i + 1 < m ? ' ' : '\n');
        }
    }
    fin.close();fout.close();
    return 0;
}