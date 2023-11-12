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

// ABC328 https://atcoder.jp/contests/abc328

/*
 * ABC328-E.cppと同じく、辺の選び方を全探索する。
 * 今回はnext_permutaion()ではなく、DFSを実装した。
 * 
 * 枝刈りとして「今後全ての辺を選んでも足りない」ならばreturnするようにしたが、
 * 意外なことにこの有無で実行時間はほとんど変わらなかった。
 * (枝刈り有：88ms, 枝刈り無：90ms)
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

ll N, M, K;
vector<ll> u, v, w;
ll ans = INF64;

// 次にnxt番目の頂点から取る/取らないで分岐
// 選ぶ辺の番号はuseに、選んだ個数はcntに
void dfs(int nxt, vector<int> &use, int cnt)
{
	// N-1本選んだら、それが全域木になるかチェック
	if(cnt == N-1)
	{
		ll ww = 0;
		UnionFind uf(N);
		for(auto &e : use)
		{
			if(uf.issame(u[e], v[e])) return;
			uf.unite(u[e], v[e]);
			ww = (ww + w[e]) % K;
		}
		ans = min(ans, ww);
		return;
	}
	if(nxt == M) return;
//	if(cnt + (M-nxt) < N-1) return;  // 枝刈り 今後全ての辺を選んでも足りない

	// nxtを使わない
	dfs(nxt+1, use, cnt);
	// nxtを使う
	use.push_back(nxt);
	dfs(nxt+1, use, cnt+1);
	use.pop_back();
}

int main(void)
{
	int i;
	cin >> N >> M >> K;
	u.resize(M), v.resize(M), w.resize(M);
	for(i = 0; i < M; i++)
	{
		cin >> u[i] >> v[i] >> w[i];
		u[i]--; v[i]--;
	}

	vector<int> use;
	dfs(0, use, 0);
	cout << ans << endl;

	return 0;
}
