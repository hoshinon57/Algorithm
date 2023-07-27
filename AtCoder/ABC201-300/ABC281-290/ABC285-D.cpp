#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC285 https://atcoder.jp/contests/abc285

/*
 * 有向グラフとみなし、サイクル検出を行って解く。
 * 
 * A->B
 * B->C
 * C->D
 * といった入力ならユーザ名の変更が可能。(グラフの終端から逆方向に処理していけばよい)
 * 
 * A->B
 * B->C
 * C->A
 * であれば不可能。
 * 
 * ユーザ名のS->Tへの変更希望を、有向グラフの辺として考えたときに、
 * 入力データにサイクルが無ければ変更可能、サイクルがあれば変更不可能となる。
 * よってサイクル検出をDFSを用いて解く。
 * 
 * サイクル検出の手法は以下の記事を参考。
 *   https://github.com/drken1215/book_algorithm_solution/blob/master/solutions/chap13.md  の13.6 (有向サイクル検出)
 *   https://qiita.com/drken/items/a803d4fc4a727e02f7ba  の4.6章
 * 
 * また、頂点は文字列であるため、グラフを連想配列を用いて実現する。
 *   map<string, vector<string>> graph;
 * の形で定義することで、頂点が数値である通常の場合とほぼ同じ実装になる。
 */

map<string, vector<string>> graph;
map<string, bool> seen, finish;  // それぞれDFSの行きがけ順、帰りがけ順にてtrueにする

// 頂点vを起点とした深さ優先探索
// サイクルを検出したらtrueを返す
bool dfs(string v)
{
	seen[v] = true;

	for(auto &e : graph[v])  // e:頂点vからの行き先一覧
	{
		// 以前のDFSにて探索済み
		if(seen[e] && finish[e]) continue;

		// 今回のDFSにて探索中の頂点にまた来たので、サイクル検出とみなせる
		if(seen[e] && !finish[e]) return true;

		// 再帰先でサイクルを検出したら、そのまま抜けていく
		if(dfs(e)) return true;
	}

	finish[v] = true;

	return false;
}

int main(void)
{
	int N;
	cin >> N;
	for(int i = 0; i < N; i++)
	{
		string s, t;
		cin >> s >> t;
		graph[s].push_back(t);  // s->t
		// 連想配列なので、事前に要素を作っておく
		seen[s] = seen[t] = false;
		finish[s] = finish[t] = false;
	}

	bool cycle = false;  // サイクルが存在するか
	for(auto &e : graph)
	{
		if(seen[e.first]) continue;  // 探索済みならスルー
		if(dfs(e.first))
		{
			cycle = true;
			break;
		}
	}

	if(!cycle)
		cout << "Yes" << endl;
	else
		cout << "No" << endl;

	return 0;
}
