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

// ABC011 https://atcoder.jp/contests/abc011

/*
 * X,Y独立して考え、二項係数を用いて解く。
 * 
 * まず、X,YをDで割り、「何回移動するか」の値にしておく。
 * 割り切れなかったら答は0.
 * 
 * X,Y独立して考える。[典型]
 * x方向にxx回移動する、としてxxを固定したときを考えると、y方向の移動回数yy=N-xx.
 * まず、全てゴール方向に移動しても届かないならダメ。
 * またxxとXの偶奇が異なってもダメ。
 * 
 * このときゴールできる場合の数、ないし確率は
 *   N回の移動からx方向にxx回選ぶ
 *   x方向の移動のうち、+側に(x+(xx-x)/2)回、-側に((xx-x)/2)回選ぶ
 *   y方向も同様
 * より、
 * C(N,xx)/2^N * C(xx,(xx-x)/2)/2^xx * C(yy,(yy-y)/2)/2^yy
 * となる。
 * これをxxごとに総和を取ったら答。
 * 
 * 途中計算の最大値に注意。
 *   C(1000,500)は10^299,
 *   2^1000は10^300,
 *   doubleの最大値は10^308
 * なので、long double型で計算したら通った。
 * ※各種解説では「double型ではダメ」「long doubleでもダメ」とあったが、long doubleは大丈夫だった。
 * 
 * [ACまでの思考の流れ]
 * ・X,Y独立して考える。
 * ・X方向の移動回数を固定したときの確率を求める。
**/

// C(a,b)を事前計算
long double cm[1005][1005];
void com(void)
{
	ll a, b;
	for(a = 0; a < 1005; a++)
	{
		for(b = 0; b <= a; b++)
		{
			if(a == 0 || b == 0)
			{
				cm[a][b] = 1;
				continue;
			}
			// パスカルの三角形
			cm[a][b] = cm[a-1][b] + cm[a-1][b-1];
		}
	}
}

// 2^nを事前計算
long double p2[1005];
void p2cal(void)
{
	ll i;
	long double ret = 1.0;
	for(i = 0; i <= 1000; i++)
	{
		p2[i] = ret;
		ret *= 2;
	}
}

long double solve(void)
{
	ll N, D; cin >> N >> D;
	ll x, y; cin >> x >> y;
	if(x%D != 0 || y%D != 0) return 0;
	x /= D;
	y /= D;
	if(x < 0) x = -x;
	if(y < 0) y = -y;

	long double ans = 0.0;
	for(ll xx = 0; xx <= N; xx++)  // x方向にxx回
	{
		ll yy = N-xx;
		// 全て+側でも届かないならNo
		// 偶奇異なるならNo
		if(x > xx || y > yy) continue;
		if((xx-x)%2 != 0 || (yy-y)%2 != 0) continue;
		
		long double tmp = 0.0;
		tmp = cm[N][xx] / p2[N];
		tmp *= cm[xx][(xx-x)/2] / p2[xx];
		tmp *= cm[yy][(yy-y)/2] / p2[yy];
		ans += tmp;
	}
	return ans;
}

int main(void)
{
	com();
	p2cal();

	cout << std::fixed << std::setprecision(20);
	cout << solve() << endl;

	return 0;
}
