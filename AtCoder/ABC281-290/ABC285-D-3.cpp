#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC285 https://atcoder.jp/contests/abc285

/*
 * DFSでサイクル検出をして解く。
 * 
 * 基本はABC285-D.cppと同じだが、以下2点が異なる。(特に後者)
 * ・有向グラフにてサイクル検出をするライブラリを使用する
 * ・頂点を文字列で扱うのではなく、連想配列で数値に変換したものをグラフにて扱う
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

bool solve(void)
{
	int i;
	int N; cin >> N;
	Graph g(N*2);  // 全ユーザがS,T異なる場合、N*2頂点となる
	map<string, int> conv;  // ユーザー名 -> 頂点番号 の対応
	int idx = 0;
	for(i = 0; i < N; i++)
	{
		string s, t; cin >> s >> t;
		if(conv.count(s) == 0) conv[s] = idx++;
		if(conv.count(t) == 0) conv[t] = idx++;
		g[conv[s]].push_back(conv[t]);  // s->t
	}

	vector<bool> seen(N*2, false), finished(N*2, false);
	vector<int> history;
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;
		history.clear();
		if(dfs_cycle_detection_directed(g, i, seen, finished, history)) return false;  // サイクルがある場合、ユーザ名変更はできない
	}

	return true;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
