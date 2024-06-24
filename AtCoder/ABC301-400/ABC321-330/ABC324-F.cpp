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

// ABC324 https://atcoder.jp/contests/abc324

/*
 * 自力で解けず、解説を見た。
 * 二分探索で解く。
 *   https://www.youtube.com/watch?v=YK2R4oztwRs
 *   https://atcoder.jp/contests/abc324/editorial/7405
 * 
 * 公式解説の通りで、「答がx以上か？」の判定問題を二分探索で繰り返すことで
 * 十分に精度の高い値を得ることができる。
 * 判定問題は b-cx を各辺の重みとしたグラフを考えて、頂点Nにおける値が正かどうかで判定できる。
 * (b/cがxより大きければ、その分目標に対してリードできる、というイメージ)
 * またグラフはDAGであるため、先頭の頂点からDPの要領で求めていくことができる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「平均値の最大化」は答を二分探索。 [典型]
 * ・(ΣA/ΣB)を最大化するような問題にて、ΣAとΣBを独立に考える方針だと
 *   2/3と2000/3000とでどちらが有利かは後々の値によって変わるので、やりづらい。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	double b, c;
};
using Graph = vector<vector<Edge>>;

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		double b, c; cin >> b >> c;
		g[u].push_back({v, b, c});
	}

	// 平均をxより大きくできればtrue
	auto check = [&](double x) -> bool
	{
		vector<double> dp(N, -INF32);  // DP値が負のケースもあるので、-INFで初期化
		dp[0] = 0.0;
		for(i = 0; i < N; i++)  // 頂点i
		{
			for(auto &e : g[i])  // i->e.to
			{
				double tmp = e.b - e.c*x;
				chmax(dp[e.to], dp[i] + tmp);
			}
		}
		return dp[N-1]>0.0;
	};

	double ok, ng;
	ok = 0.0;
	ng = (double)INF32;
	for(int _ = 0; _ < 100; _++)
	{
		double m = (ok+ng)/2.0;
		if(check(m)) ok = m;
		else ng = m;
	}
	cout << std::fixed << std::setprecision(15);
	cout << ok << endl;

	return 0;
}
