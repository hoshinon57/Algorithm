#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC285 https://atcoder.jp/contests/abc285

/*
 * 有向グラフとみなし、サイクル検出を行って解く。
 * 考え方はABC285-D.cppのDFSと同じであり、そちらを参照。
 * 本ソースコードではBFSで解く。
 * 
 * サイクル検出の手法は以下の記事を参考。
 *   https://qiita.com/drken/items/996d80bcae64649a6580  の3.5章
 * 入力の逆方向に辺を作り、出次数が0(シンク)である頂点をキューに積んでBFS開始。
 * 
 * BFSでは取り出した頂点vをグラフから削除する。
 * それにより出次数が0になった頂点があれば、新たにキューに積む。
 * 
 * 頂点ごとに「キューに積んだか否か」の情報を保持しておき、
 * BFS終了後に積まれていない頂点があれば、サイクル有りと判断できる。
 */

map<string, vector<string>> graph;
map<string, bool> seen;  // seen[v]:頂点vがキューに積まれたらtrue
map<string, int> deg;  // deg[v]:頂点vの出次数

int main(void)
{
	int N;
	cin >> N;
	for(int i = 0; i < N; i++)
	{
		string s, t;
		cin >> s >> t;
		graph[t].push_back(s);  // s->t の逆方向に辺を作る

		// 連想配列なので、事前に要素を作っておく
		seen[s] = seen[t] = false;
		deg[s]++;
		deg[t] = deg[t];  // 値は変えず、要素を作るだけの目的(最適化で消えちゃったりするかな…)
	}

	queue<string> que;
	// 出次数が0である頂点をキューに積む
	for(auto &e : deg)
	{
		if(e.second == 0)
		{
			que.push(e.first);
			seen[e.first] = true;
		}
	}

	while(!que.empty())
	{
		string v = que.front();
		que.pop();

		// 出次数が0である頂点vから辿れる頂点について(※)、そのdeg[]を-1する。
		// 頂点vを削除するイメージ。
		// (※)逆方向に辺を作っているので、正確には頂点vへと向かう頂点になる
		for(auto &e : graph[v])
		{
			deg[e]--;
			if(deg[e] == 0)  // 出次数が0になれば、キューに積む
			{
				que.push(e);
				seen[e] = true;
			}
		}
	}
	
	bool cycle = false;  // サイクルが存在するか
	// キューに積まれない頂点があれば、サイクル有りと判断できる
	for(auto &e : seen)
	{
		if(!e.second)  // seen[*] = false
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
