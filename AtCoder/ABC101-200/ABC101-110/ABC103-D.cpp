#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC103 https://atcoder.jp/contests/abc103

/*
 * Union-Findで解く。
 * 解説を見ると区間スケジューリング問題だった、なるほど…
 * 
 * ng[i][*]を、
 *   島iと繋いではいけない島の一覧
 * として入力a,bから作成する。
 * このときng[b]にaを入れる形とする(a<b)。
 * 
 * 若い番号から順に (i,i+1) を繋ぐか否かを考えると、
 * ng[i+1][*]の一覧に、島iと同じグループの島があったら繋いじゃダメそう、と考えた。
 * これをUnionFindで実装した。
 * 
 * [ACまでの思考の流れ]
 * ・グラフ問題っぽい。
 * ・橋(辺)を壊すのは一般的に大変なので、逆方向で考えるのが典型。
 * ・左から順に、「その橋を繋げていいかどうか」を考えてみる。
 *   ⇒Union-Findでいけそう。
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
	// 1-indexed
	int i;
	int N, M; cin >> N >> M;
	UnionFind uf(N+1);
	vector<vector<int>> ng(N+1);  // ng[i]:島iと繋いではいけない島の番号一覧 ただしいずれもi未満
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		ng[b].push_back(a);
	}

	int ans = 0;
	for(i = 1; i <= N-1; i++)  // i,i+1を繋ぐ
	{
		bool ok = true;
		for(auto &e : ng[i+1])
		{
			if(uf.issame(i, e)) ok = false;
		}
		if(ok) uf.unite(i, i+1);  // 繋いでOK
		else ans++;  // 繋いじゃダメ
	}
	cout << ans << endl;

	return 0;
}
