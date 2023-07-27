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
 * ABC264-E.cpp に対して、発電所をM個それぞれ区別して解く。
 * 「発電所を1つにまとめても問題ない」という考えに至らない可能性があるため、
 * その場合でも解けるようにするのが目的。
 * 時系列を逆に見て辺を追加していく、という方針は同じ。
 * 
 * power[i]:地点iに電気が通っているならtrue
 * power_city_num:電気が通っている都市の数
 * を更新していく。
 * 電線が1本も繋がっていない場合、power[]は発電所ならtrue, 都市ならfalseになる。またpower_city_numは0.
 * 電線を(u,v)で繋げる場合、
 *   u,vの片方がtrue, もう片方がfalse
 * の場合に、新たに電気が繋がる都市が発生する。
 * その都市の数は、Union-Findにて繋がっていない方のsize()にて求まる。
 * 詳細はmake_line()のコメントを参照。
 */

vector<int> u, v;
vector<bool> power;  // power[i]:地点iに電気が通っているならtrue
int power_city_num = 0;  // 電気が通っている都市の数

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

// graphNoで指定した電線を接続し、
// それによって新たに電気が繋がるようになる都市の数を更新する。
void make_line(int graphNo, UnionFind &uf)
{
	bool power_u, power_v;  // graphNoの電線で繋がるu,vについて、電気が通っているか
	power_u = power[uf.root(u[graphNo])];
	power_v = power[uf.root(v[graphNo])];

	/*
	 * u,vを接続するにあたり、新たに電気が繋がるようになる都市はいくつか？を考える。(=power_city_numの変化)
	 * ・u,vともに電気が繋がっていない場合 (power_u,vともにfalse)
	 *   power_city_numは変化しない
	 * ・u,vともに電気が繋がっている場合 (power_u,vともにtrue)
	 *   power_city_numは変化しない
	 * ・u,vのどちらか片方だけ、電気が繋がっている場合 (power_u,vの片方がtrue, もう片方がfalse)
	 *   power_uの方が電気が繋がっているとして、
	 *   power_v側の地点の数だけ、新たに電気が繋がることになる。
	 *   power_v側には電気が繋がっていない
	 *   ＝power_v側に発電所は無い
	 *   ＝Union-Findにてsize(v側)がそのまま都市の数になる。
	 * 
	 * ※u,vが元々繋がっているか、という考慮は不要。
	 *   元々繋がっている場合、power_u,vはともにfalseか、ともにtrueのどちらかとなる。
	 */

	if(power_u && !power_v)
	{
		power_city_num += uf.size(v[graphNo]);  // v側のサイズだけ、新たに電気が繋がる
		power[uf.root(v[graphNo])] = true;  // v側にも電気が繋がった
	}
	if(!power_u && power_v)
	{
		power_city_num += uf.size(u[graphNo]);
		power[uf.root(u[graphNo])] = true;
	}

	uf.unite(u[graphNo], v[graphNo]);  // power_u,vの状態を問わず、統合しておけばOK.
}

int main(void)
{
	// 0-indexed
	int i;
	int N, M, E;
	cin >> N >> M >> E;
	u.resize(E);
	v.resize(E);
	for(i = 0; i < E; i++)
	{
		cin >> u[i] >> v[i];
		u[i]--;  // 0-indexedに変換
		v[i]--;
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

	// 地点iに電気が通っているかを示すpower[]の初期化
	power.assign(N+M, false);
	for(i = N; i < N+M; i++)
	{
		power[i] = true;  // 発電所はtrue
	}

	// 都市と発電所をUnion-Findで管理する
	UnionFind uf(N+M);  // 0～N-1が都市、N～M-1が発電所を指す
	for(i = 0; i < E; i++)
	{
		if(!event[i])  // イベントで切れずに最後まで残る電線について、ここで接続
		{
			make_line(i, uf);
		}
	}

	vector<int> answer;
	for(i = Q-1; i >= 0; i--)  // イベントを逆方向に見ていく
	{
		answer.push_back(power_city_num);  // 逆方向に見ていくことから、電線を接続する前に記録しておく
		make_line(x[i], uf);
	}

	reverse(answer.begin(), answer.end());
	for(auto &e : answer)
	{
		cout << e << endl;
	}

	return 0;
}
