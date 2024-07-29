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

// ABC224 https://atcoder.jp/contests/abc224

/*
 * 解法の方向性は思いついたが、実装方針が悪く解説を見た。
 * BFSで解く。
 *   https://blog.hamayanhamayan.com/entry/2022/03/21/163326
 *   https://atcoder.jp/contests/abc244/editorial/3599
 *   https://note.com/syamashi/n/ne7a3fa8c882f
 * 
 * 手元で色々書いてみると、以下2つの情報があれば良さそう、というのが分かる。
 * (1)各頂点について、これまで通った回数の偶奇(0 or 1)
 * (2)今どの頂点にいるか
 * ここから、dist[S][v]を以下のように定義する。
 *   S:各頂点をbitに割り当て、通った回数が偶数なら0, 奇数なら1
 *   v:現在いる頂点
 * 
 * 求める値は、各Sについてdist[S][*]の最小値を加算していったもの、となる。
 * 
 * dist[]の初期値は以下となる。
 *   dist[0][*]=0 (入力例1より)
 *   dist[(vビット目のみ立っている)][v]=1
 * 後者を基準としてBFSを行う。
 * BFSなので、dist=1となるN要素を最初にまとめてキューに入れておく点に注意。 ⇒ つまり、同時進行で始める
 * ※前者を基準としたBFSも可能と思うが、少し実装が面倒そうになるので不採用とした。
 * vと繋がっている頂点eについて、Snext=Sのeビット目を反転した値 として探索すればよい。
 * 
 * 最短距離なので、DFSではなくBFSを使う必要がある。
 * 
 * [反省点 / [どう思考すれば解法にたどり着けるか]]
 * ・(以下、自分の実装では、という前置きが入る)
 *   BFSではなく再帰で解こうとしたが、解が合わなかった。
 *   再帰を続けるとループが発生するので、そこの判定がうまくできなかった。
 *   ※ランキング上位の提出を5,6個見たが、再帰で解いている人はいなかった。
 * ・bitDP(再帰)でループが発生するような場合、非再帰で実装するのが良いかもしれない。
 * ・最短距離はDFSではなくBFS.
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	vector<vector<int>> dist(1<<N, vector<int>(N, INF32));  // dist[S][v]
	for(i = 0; i < N; i++) dist[0][i] = 0;

	queue<pair<int,int>> que;  // {S,v}
	for(i = 0; i < N; i++)
	{
		que.push({1<<i, i});
		dist[1<<i][i] = 1;
	}

	while(!que.empty())
	{
		auto [S,v] = que.front();
		que.pop();
		for(auto &e : g[v])
		{
			int Snext = S^(1<<e);  // bit:eを反転
			if(dist[Snext][e] != INF32) continue;
			dist[Snext][e] = dist[S][v] + 1;
			que.push({Snext, e});
		}
	}

	ll ans = 0;
	for(i = 0; i < (1<<N); i++)
	{
		int tmp = INF32;
		for(j = 0; j < N; j++)  // Sについて、dist[S][j]の最小値を求める
		{
			chmin(tmp, dist[i][j]);
		}
		ans += tmp;
	}
	cout << ans << endl;

	return 0;
}
