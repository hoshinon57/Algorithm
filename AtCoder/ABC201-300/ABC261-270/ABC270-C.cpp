#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC270 https://atcoder.jp/contests/abc270

/*
 * 深さ優先探索で解く。
 * 
 * 頂点v->tのパスを求める関数dfs()を用意し、再帰を用いて探索していく。
 * このとき、探索が逆流しないようにvに来る前の頂点を引数に設定する(=parent)。
 * ゴールとなる頂点に到達したら、頂点をリストに追加しながら関数をreturnしていく。
 * 
 * リストにはゴールからスタート方向へ逆順に頂点が入っているので、
 * 逆順に出力するか、reverse()にて反転させる必要がある。
 */

using Graph = vector<vector<int>>;

vector<int> ans_list;  // X->Yへのパス上の頂点を列挙s

// 頂点v->tへのパスを深さ優先探索で求める
// parent：vに来る前の頂点 逆流防止用
// 戻り値：頂点tに到達したらtrue
bool dfs(Graph &g, int v, int t, int parent)
{
	// ゴールに到達
	if(v == t)
	{
		ans_list.push_back(v);
		return true;
	}

	// 頂点vと繋がっている頂点を列挙
	for(auto &e : g[v])
	{
		if(e == parent) continue;  // 親ならスルー
		if(dfs(g, e, t, v))  // trueで返ってきたら、頂点を追加してまたtrueで返る
		{
			ans_list.push_back(v);
			return true;
		}
	}

	return false;
}

int main(void)
{
	// 0-indexed
	int i;
	int N, X, Y;
	cin >> N >> X >> Y;
	X--;  // 0-indexedに変換
	Y--;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;  // 0-indexedに変換
		v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	// X->Yのパスを求めて、パス上の頂点をans_list[]に列挙する
	dfs(g, X, Y, -1);

	// Y->Xの方向に並んでいるので、反転する
	std::reverse(ans_list.begin(), ans_list.end());

	// 結果出力
	for(auto &e : ans_list)
	{
		cout << e+1 << " ";  // 1-indexedに戻すため+1
		// ※そういえば、最後の要素の後ろに空白があってもACになるのね
	}
	cout << endl;

	return 0;
}
