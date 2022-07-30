#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC049 https://atcoder.jp/contests/abc049

/*
 * Union-Findで解く。
 * 出力は合っているもののTLEを解消できず、解説を見た。
 * 参考：
 *   https://drken1215.hatenablog.com/entry/2021/07/28/014400
 *   https://tutuz.hateblo.jp/entry/2018/07/25/225115
 * 
 * まず、都市を頂点に、道路および鉄道を辺とみなしてUnion-Findを適用する。
 * このとき道路用のUnion-Find, 鉄道用のUnion-Findをそれぞれ用意する。
 * 
 * 道路について {1,2,3} {4,5,6,7}
 * 鉄道について {1,2} {3,4} {5} {6,7}
 * となったときに、都市1について道路でも鉄道でも同じグループであるものを列挙すればよい。
 * ※ここの上手い列挙方法を思いつかず、TLEになった。
 * 
 * 列挙方法は、都市iについて、
 *   ((道路のUnion-Findにて、都市iの根), (鉄道のUnion-Findにて、都市iの根))
 * となるpairを用意する。
 * 異なる都市についてこのpairが同一であれば、この都市は道路・鉄道ともに連結している。
 * あとはこれをmapでカウントすればよい。
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
	int N, K, L;
	cin >> N >> K >> L;

	// 道路で繋がっている都市をUnion-Findでグループ化する
	vector<int> p(K), q(K);
	UnionFind uf_road(N);
	for(i = 0; i < K; i++)
	{
		cin >> p[i] >> q[i];
		p[i]--;
		q[i]--;
		uf_road.unite(p[i], q[i]);
	}

	// 鉄道で繋がっている都市をUnion-Findでグループ化する
	vector<int> r(L), s(L);
	UnionFind uf_rail(N);
	for(i = 0; i < L; i++)
	{
		cin >> r[i] >> s[i];
		r[i]--;
		s[i]--;
		uf_rail.unite(r[i], s[i]);
	}

	// ((道路のUnion-Findにて、都市iの根), (鉄道のUnion-Findにて、都市iの根))
	// となるpairを考えたときに、これが同一になる都市は道路・鉄道ともに連結している。
	// よってこれをmapでカウントする。
	map<pair<int,int>, int> answer;
	for(i = 0; i < N; i++)
	{
		answer[make_pair(uf_road.root(i), uf_rail.root(i))]++;
	}
	for(i = 0; i < N; i++)
	{
		cout << answer[make_pair(uf_road.root(i), uf_rail.root(i))] << endl;
	}

	return 0;
}
