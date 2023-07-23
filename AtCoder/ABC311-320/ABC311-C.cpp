#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC311 https://atcoder.jp/contests/abc311

/*
 * 深さ優先探索にてサイクル検出をして解く。
 * 
 * このグラフはfunctional graphであるため、
 * 適当な頂点からDFSすることで、必ずサイクルを見つけることができる。
 * 
 * よって頂点0からDFSし、サイクルを見つけるまで探索。
 * historyには先頭からサイクル検出部分までの頂点が入っているので、
 * そこからサイクル部分を抜き出して出力すればよい。
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
		g[i].push_back(a);  // i->Ai
	}

	vector<bool> seen(N, false), finished(N, false);
	vector<int> history;
	// functional graphのため、適当な頂点からDFSすればサイクルは必ず見つかる
	dfs_cycle_detection_directed(g, 0, seen, finished, history);

	// history[]の終端で検索し、サイクルの先頭を見つける
	int start = find(history.begin(), history.end(), history[history.size()-1]) - history.begin();

	// サイクルの頂点数を出力する
	// -1はhistoryの終端(サイクル先頭と重複しているので)、-startはサイクルに入る前
	cout << history.size()-1-start << endl;

	for(i = start; i < (int)history.size()-1; i++)
	{
		cout << history[i]+1;  // 1-indexedに戻す
		if(i != (int)history.size()-2) cout << " ";
	}
	cout << endl;

	return 0;
}
