#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC409 https://atcoder.jp/contests/abc409

/*
 * コンテスト時、考察は合っていたが実装方針が悪くTLEが取れなかった。残念。
 * 
 * 非連結である頂点対を全て管理していく。
 * 制約より、O(頂点数^2)が十分に間に合う。
 * 
 * priority_queueに {頂点どうしの距離, 頂点番号1, 同2} として登録していく。
 * 最初は辺が無いので全ペアを登録することになる。
 * 
 * クエリ1：
 * 新頂点には辺が無いので、既存の頂点全てとの距離をキューに積む。
 * 
 * クエリ2：
 * ここが本題。
 * 出力したい値kは、キューの先頭のdistとなる。
 * しかし連結済みの頂点が積まれている可能性があるため、事前チェックは必要。
 * 全頂点が連結であれば-1を出力。
 * 
 * kが求まったら、キューの先頭のdistがkのあいだ、UnionFindで連結しキューから取り除く。
 * 
 * クエリ3：
 * UnionFindでやるだけ。
 * 
 * ※コンテスト時の反省
 * コンテスト時、priority_queueではなくsetで管理したらTLEだった。
 * 同じようにハマってる人が結構多かった。
 *   https://x.com/PCTprobability/status/1931345995876753726
 *   https://x.com/_bo9chan/status/1931468110131036183
 * 後にざっと計測すると、登録処理がsetは遅いみたい。
 * ただsetの方が楽なので…、
 *   TLEしたとき、最大/最小など何らか1つの要素にしかアクセスしないなら、setをpqに置き換えてみる
 * というのを覚えておくのが現実的か。
**/

// 「問題解決力を鍛える!アルゴリズムとデータ構造」のcode11.3を写経したもの
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

	// 連結成分ごとに頂点をvector<int>で返す
	// ex) 頂点0,1,3と2,5が同一グループの場合、 {{0,1,3},{2,5},{4}} といった形で返す
	// 連結成分単位での順番は不定になると思われる
	// 計算量は頂点数をNとしてO(N)
	vector<vector<int>> groups(void) {
		vector<vector<int>> member(parent.size());  // member[v]:要素vを親とするグループ
		for(int v = 0; v < (int)parent.size(); v++) {
			member[root(v)].push_back(v);
		}
		// memberの空要素を削除して返す
		vector<vector<int>> res;
		for(auto &e : member) {
			if(!e.empty()) res.push_back(e);
		}
		return res;
	}
};

int main(void)
{
	ll i, j;
	ll N, Q; cin >> N >> Q;
	UnionFind uf(3010);
	vector<ll> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}
	using dt = array<ll,3>;  // {dist, 頂点u, 頂点v}  u<vとする
	priority_queue<dt, vector<dt>, greater<dt>> que;  // 連結でない2頂点の全組み合わせについて、距離を保持

	// 最初は辺が無い
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			ll di = abs(x[i]-x[j]) + abs(y[i]-y[j]);
			que.push({di, i, j});
		}
	}

	while(Q > 0)
	{
		Q--;
		int ki; cin >> ki;
		if(ki == 1)
		{
			int a, b; cin >> a >> b;
			for(i = 0; i < N; i++)
			{
				ll di = abs(x[i]-a) + abs(y[i]-b);
				que.push({di, i, N});
			}
			x.push_back(a);
			y.push_back(b);
			N++;  // Nを現在の頂点数として更新していく
		}
		if(ki == 2)
		{
			// queを先頭から見ていくが、連結済みがありえるので事前チェックして削除しておく
			while(!que.empty())
			{
				auto[di, u, v] = que.top();
				if(uf.issame(u, v))
				{
					que.pop();
				}
				else break;
			}
			// 全頂点が連結
			if(uf.size(0) == N)
			{
				cout << -1 << endl;
				continue;
			}
			ll k = que.top()[0];
			cout << k << endl;
			while(!que.empty())
			{
				auto[di, u, v] = que.top();
				if(di != k) break;
				uf.unite(u, v);
				que.pop();
			}
		}
		if(ki == 3)
		{
			int u, v; cin >> u >> v;
			u--; v--;
			YesNo(uf.issame(u, v));
		}
	}

	return 0;
}
