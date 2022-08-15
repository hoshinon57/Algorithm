#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC264 https://atcoder.jp/contests/abc264

/*
 * 最初BFSにて実装したがTLEが1つ解消できず、解説を見てUnion-Findにて実装した。
 * 
 * ＞辺の追加と連結性の判定を得意とするデータ構造は Union Find である. 
 * https://kazun-kyopro.hatenablog.com/entry/ABC/264/E
 * 
 * グラフの問題において「辺を削除する」は扱いづらい。
 * そのため逆方向に考えて、「辺を追加していく」ように処理する。
 * 
 * また、逆方向の時系列に見ていくことを踏まえると、
 * M個ある発電所については1つでまとめて考えても問題ない。
 * よってN個の都市(No.0～N-1), 1個の発電所(No.N)として管理する。
 * 
 * 以上を踏まえて実装していく。
 * ・まずはイベントで切れない電線をピックアップし、先だってUnion-Findで統合しておく。
 * ・次にイベントを逆方向に見ていき、イベントに対応する電線をつなげていく。
 *   電気が通っている都市の数は、"N番のグループのサイズ数-1"で表せる(-1は発電所の分)。
 */

struct UnionFind
{
	vector<int> parent;  // parent[x]:xの親の番号 xが根であれば-1
	vector<int> siz;  // siz[x]:xの属するグループの頂点数

	// 頂点数nで初期化
	UnionFind(int n) : parent(n, -1), siz(n, 1) {}

	// 根を求める
	int root(int x)
	{
		if(parent[x] == -1) return x;  // xが根であれば、xを返す
		else
		{
			parent[x] = root(parent[x]);
			return parent[x];
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y)
	{
		return root(x) == root(y);  // 根が同一なら同じグループ
	}

	// xを含むグループとyを含むグループを併合する
	bool unite(int x, int y)
	{
		// x,yをそれぞれ根まで移動させる
		x = root(x);
		y = root(y);

		// すでに同じグループであれば何もしない
		if(x == y) return false;

		// 頂点数が少ない方が子になるようにする
		if(siz[x] < siz[y])
		{
			// xをyの子にする
			parent[x] = y;
			siz[y] += siz[x];
		}
		else
		{
			// yをxの子にする
			parent[y] = x;
			siz[x] += siz[y];
		}
		return true;
	}

	// xを含むグループの頂点数
	int size(int x)
	{
		return siz[root(x)];
	}
};

int main(void)
{
	// 0-indexed
	int i;
	int N, M, E;
	cin >> N >> M >> E;
	vector<int> u(E), v(E);
	for(i = 0; i < E; i++)
	{
		cin >> u[i] >> v[i];
		u[i]--;  // 0-indexedに変換
		v[i]--;
		// 逆方向の時系列に見ていくことを踏まえると、
		// M個ある発電所については1つでまとめて考えても問題ない。 -> N番でまとめる
		u[i] = min(u[i], N);
		v[i] = min(v[i], N);
	}
	int Q;
	cin >> Q;
	vector<int> x(Q);
	vector<bool> event(E, false);  // イベントにて切れる電線ならtrue
	for(i = 0; i < Q; i++)
	{
		cin >> x[i];
		x[i]--;  // 0-indexedに変換
		event[x[i]] = true;  // イベントで切れる電線
	}
	// ここまで入力

	// 都市と発電所をUnion-Findで管理する
	UnionFind uf(N+1);  // 0～N-1が都市、Nが発電所を指す
	for(i = 0; i < E; i++)
	{
		if(!event[i])  // イベントで切れずに最後まで残る電線について、ここで接続
		{
			uf.unite(u[i], v[i]);
		}
	}

	vector<int> answer;
	for(i = Q-1; i >= 0; i--)  // イベントを逆方向に見ていく
	{
		// 逆方向に見ていくことから、unite()の前にサイズを計算する
		// 電気が通っている都市の数：N番のグループのサイズ数-1 (-1は発電所の分)
		answer.push_back(uf.size(N)-1);
		uf.unite(u[x[i]], v[x[i]]);
	}

	reverse(answer.begin(), answer.end());
	for(auto &e : answer)
	{
		cout << e << endl;
	}

	return 0;
}
