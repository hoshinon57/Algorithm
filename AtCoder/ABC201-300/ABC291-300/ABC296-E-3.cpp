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

// ABC296 https://atcoder.jp/contests/abc296

/*
 * DFSにてサイクルを見つける自作ライブラリにて解いた版。
 * 
 * 未探索の頂点からDFSし、サイクルを見つけていく。
 * サイクルが見つかったら、historyの末尾にダブリがある仕様なので、
 * その値を先頭からfindすることで、サイクルの頂点数を求められる。
 */

using Graph = vector<vector<int>>;

// 有向グラフにて、頂点vを起点にDFS.
// サイクルを見つけたらtrueを返す。その場合、vからサイクル終端までをhistoryに保持する。
//   historyの例：0->1->2->3->4->2  サイクルは2-3-4の部分。末尾の2が2回出てくることに注意すること。
// seen, finishedの要素数は頂点数で、falseで初期化しておくこと。
// 
// このDFSにて、
// (1)1つの連結成分に2つ以上のサイクルがある場合、
//    サイクル有無の判定は可能。
//    サイクル検出は何らか1つなら可能。全てのサイクル検出は不可能。
// (2)Functional Graphなら連結成分ごとにサイクルは1個なので、
//    呼び出し元で未探索の頂点からDFSすることで全てのサイクルを検出可能。
bool dfs_cycle_detection_directed(Graph &g, int v, vector<bool> &seen, vector<bool> &finished, vector<int> &history)
{
	seen[v] = true;
	history.push_back(v);  // vを行きがけ時に登録し、帰りがけ時に削除

	for(auto &e : g[v])
	{
		if(finished[e]) continue;
		if(seen[e] && !finished[e])
		{
			// サイクル検出した
			finished[v] = true;  // 頂点vについて探索完了
			history.push_back(e);  // サイクルの終端
			return true;
		}
		if(dfs_cycle_detection_directed(g, e, seen, finished, history))  // サイクル検出してreturnしてきた場合は、再帰的にreturnしていく
		{
			finished[v] = true;  // この場合も、頂点vについて探索完了扱いとする
			return true;
		}
	}

	finished[v] = true;
	history.pop_back();
	return false;
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		a--;
		g[i].push_back(a);
	}

	int ans = 0;
	vector<bool> seen(N, false), finished(N, false);
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;
		vector<int> hist;
		if(dfs_cycle_detection_directed(g, i, seen, finished, hist))
		{
			// hist: 0->1->2->3->4->2
			// のようなとき、末尾の2がダブリになることを利用して、先頭から探索する
			auto b = hist.back();
			auto itr = find(hist.begin(), hist.end(), b);  // 先頭から探索することで、サイクル先頭が見つかる
			ans += (hist.end() - itr) - 1;
		}
	}
	cout << ans << endl;

	return 0;
}
