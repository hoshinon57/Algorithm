#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC332 https://atcoder.jp/contests/abc332

/*
 * bitDPで解く。
 * 
 * dp[S][f]を以下のように定義する。
 *   Sの各bitをグッズに割り当て、
 *   bitが1のグッズを使ってf個の袋に分けるときの、
 *   (x1-wsum_divd)^2 + (x2-wsum_divd)^2 + ... +(xf-wsum_divd)^2 の最小値。
 *   ただし wsum_divd = 1/D*(x1+x2+...+xD) = 1/D*(w1+w2+...+wN) とする。
 * dp[(1<<N)-1][D] / D  が答。
 * 
 * 上記をbitDPにて、dp[S][f] を dp[S1][f-1] + dp[S2][1] に分解していく。 (S1∪S2=S)
 * 分解した部分の最小値をdp[S][f]に採用する。
 * 再帰のベースケースはf=1のとき。
 * 
 * [ACまでの思考の流れ]
 * ・求めたいものは (x1-wsum_divd)^2 + (x2-wsum_divd)^2 + ... + (xD-wsum_divd)^2 となる。
 * ・つまりグループ分けであり、制約からもbitDPっぽい。
 * ・x1+x2+...+xD はすなわち w1+w2+...+wN と同じなので、xの平均は固定値となる。
 * 
 * [memo]
 * ・最初、Σの部分をさらに整理して
 *     Σ(xi-xave)^2 = Σ(xi-(w1+w2+...+wN)/D)^2 = (1/D)^2*Σ(D*xi-(w1+w2+...+wN))^2
 *   としたところ(DP値を整数にできるというメリットがある)、WAになった。
 *   原因は、D*xiの部分が最大で10^10とかになり、全体の2乗部分でオーバーフローしてしまうため。
 */

int N, D;
vector<ll> w;
double wsum_divd = 0.0;  // 1/D*(x1+x2+...+xD) = 1/D*(w1+w2+...+wN)
vector<vector<double>> dp;

double func(int S, int f)
{
	double &d = dp[S][f];
	if(d > -0.5) return d;  // メモ化済み double型なので念のため-1と0の中間で判定

	int i;
	if(f == 1)  // ベースケース
	{
		double tmp = 0.0;
		for(i = 0; i < N; i++)
		{
			if(((S>>i)&1) == 0) continue;
			tmp += w[i];
		}
		tmp -= wsum_divd;
		tmp = tmp*tmp;
		d = tmp;
		return d;
	}

	d = (double)INF64;  // forループは必ず1回は処理される
	for(int T = S; T >= 0; T--)
	{
		T &= S;
		int T2 = S&(~T);  // T∪T2=S
		double tmp1 = func(T, f-1);
		double tmp2 = func(T2, 1);
		chmin(d, tmp1+tmp2);
	}
	
	return d;
}

int main(void)
{
	int i;
	cin >> N >> D;
	w.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> w[i];
		wsum_divd += w[i];
	}
	wsum_divd /= (double)D;
	dp.resize(1<<N, vector<double>(D+1, -1.0));

	cout << std::fixed << std::setprecision(15);
	cout << func((1<<N)-1, D) / D << endl;

	return 0;
}
