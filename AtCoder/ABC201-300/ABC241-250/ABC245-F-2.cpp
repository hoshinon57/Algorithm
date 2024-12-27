#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC245 https://atcoder.jp/contests/abc245

/*
 * noshi91氏の解説を元に実装した内容。
 *   https://atcoder.jp/contests/abc245/editorial/3667
 * 
 * 出次数が0の頂点vは削除してよい。その頂点に来ても条件を満たせないため。
 * vを削除した後、vへの辺がある頂点の出次数を減らし、それがまた0になれば削除してよい。
 * というのを繰り返していくと、削除されずに残った頂点数が答となる。
 * 
 * 入力と逆方向にグラフを構築すると実装が楽。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・どういう頂点が答になるか？
 *   ⇒サイクル上の頂点、およびサイクルへと行ける頂点
 *   ⇒ならば、それを満たさない頂点はどういう頂点か？
 *   ⇒少なくとも、出次数=0の頂点が該当する。
 *     該当した頂点を削除し、また出次数=0となった頂点があれば、それも該当する。
 *   という流れで、出次数=0の頂点をキューを用いて列挙していく形かなあ。
 * ・グラフを反転した方が実装しやすい、という考え方。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph rg(N);  // 逆方向グラフを構築
	vector<int> in(N);  // 入次数 (逆方向のグラフなので、本来は出次数)
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		rg[v].push_back(u);
		in[u]++;
	}

	queue<int> que;
	for(i = 0; i < N; i++)
	{
		if(in[i] == 0) que.push(i);
	}

	int ans = N;
	while(!que.empty())
	{
		auto v = que.front();  // 頂点vを削除
		que.pop();
		ans--;
		for(auto &e : rg[v])
		{
			in[e]--;
			if(in[e] == 0) que.push(e);
		}
	}
	cout << ans << endl;

	return 0;
}
