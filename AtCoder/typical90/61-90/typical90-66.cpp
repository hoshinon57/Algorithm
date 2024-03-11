#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:66 https://atcoder.jp/contests/typical90/tasks/typical90_bn

/*
 * sum[x]を、これまで値xが数列aに登場する回数の期待値とする。
 * a[i]=Liであったとき、 sum[Li+1]～sum[100] までの総和が転倒数の期待値となる。
 * a[i]=Liとなる確率は 1.0/(R-L+1.0).
 * 
 * あとはsum[x]を更新しつつ、上記を各a[i]について計算していけばよい。
 * 計算量はO(N^2*max(R-L)).
 * ※公式解法ではsum[]を使わないからか、O(N^2*max(R-L)^2)だった。
 */

int main(void)
{
	int i, j, k;
	int N; cin >> N;
	vector<int> l(N), r(N);
	for(i = 0; i < N; i++)
	{
		cin >> l[i] >> r[i];
	}

	double ans = 0.0;
	vector<double> sum(100+1, 0.0);  // sum[x]:値xが数列aに登場する回数の期待値
	for(i = 0; i < N; i++)
	{
		double tmp = 0.0;  // a[i]に着目したときの、転倒数の期待値
		for(j = l[i]; j <= r[i]; j++)  // [L,R]から一様ランダム
		{
			for(k = j+1; k <= 100; k++)
			{
				tmp += sum[k];
			}
			sum[j] += 1.0/(r[i]-l[i]+1.0);
		}
		ans += tmp / (double)(r[i]-l[i]+1.0);
	}
	cout << std::fixed << std::setprecision(10);
	cout << ans << endl;

	return 0;
}
