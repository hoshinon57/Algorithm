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
 * 頂点Nの全域木の辺数はN-1.
 * 制約よりN=8,M=28が最大値であり、このとき辺の選び方は 28C7=1,184,040 と多くない。
 * よってM個からN-1個の辺の選び方を全探索し、それが全域木になるかを判定していく。
 * 
 * 全探索にはnext_permutation()を用いた。
 * 各辺について選ばない場合をF, 選ぶ場合をTとして、
 * 要素数がMでうちN-1個だけTであるbool型配列を用意すると、next_permutaion()にてうまいことやってくれる。
 * 
 * あとは上記配列についてUnionFindにて辺を張っていき、
 * 全域木になる(=最後まで閉路が作られない)場合の重みについて、最小値を求めればよい。
 * 
 * [ACまでの思考の流れ]
 * ・「Kで割った余りの最小値」を求めるため、通常のMSTを解くGreedy的な方法は無さそう。
 * ・最大でN=8という制約が気になる。
 * ・辺Mは最大で28, 全域木にするには使う辺は(N-1)=7個。
 *   辺の選び方は 28C7=10^6 程度で少ない。
 *   これを全探索できれば解けないか。
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

// useで示された辺を使うときに、全域木になるか判定
void check(vector<bool> &use)
{
	int i;
	ll ww = 0;
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		if(!use[i]) continue;
		if(uf.issame(u[i], v[i])) return;  // 閉路ができたら全域木にならない (使う辺はN-1個であるため)
		uf.unite(u[i], v[i]);
		ww = (ww + w[i]) % K;
	}
	ans = min(ans, ww);
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

	// M個から(N-1)個を選ぶため、要素数がM, trueが(N-1)個である配列を用意する
	vector<bool> use(M, false);
	for(i = 0; i < N-1; i++) { use[i] = true; }
	sort(use.begin(), use.end());

	do {
		check(use);
	} while(next_permutation(use.begin(), use.end()));
	cout << ans << endl;

	return 0;
}
