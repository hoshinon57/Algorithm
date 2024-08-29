#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC152 https://atcoder.jp/contests/abc152

/*
 * 自力で解けず、解説を見た。余事象と包除原理を用いて解く。
 *   https://blog.hamayanhamayan.com/entry/2020/01/25/121049
 *   https://x.com/kyopro_friends/status/1218893628673708032
 * 
 * 「黒い辺が1つ以上」というのは面倒なので、「全てが白い辺、の余事象」で考えると楽。
 * ある制約にてパスに含まれる辺の数をx, それ以外の辺の数をyとすると(x+y=N-1)、
 * xの辺を全て白にする場合の数は、
 *   x:1通り
 *   y:白黒自由なので、2^y通り
 * となり、1*2^y = 2^(N-1-x) 通りとなる。
 * 事前に、各制約ごとに含まれるパスを調べておく。 -> dfs_tree_path()
 * 
 * 制約が複数ある場合、使う辺をorしたものが対象となる。
 * 包除原理より、全ての制約の組み合わせについて
 * 使う制約が奇数なら加算、偶数なら減算していけばよい。
 * 
 * 全ての辺についての塗り方は2^(N-1)通りであるので、
 * ここから前述の包除原理で得た値を引いたものが答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「～～が1つ以上」は余事象で考えてみる。
 * ・複数の要素(制約)を組み合わせるなどで、M=20の制約は包除原理を考えてみる。
 */

using Graph = vector<vector<int>>;

// 木であるグラフについて、頂点s,tのパスを探索し通る頂点一覧をpath[]に設定する
// 呼び出し元からは dfs(g, s, t, path) のように呼び出す
// 初回呼び出し時にs,tをswapするため(※)、もし有向グラフにて使いたい場合はrev=0として呼び出すこと
// (※)探索アルゴリズムより、pathにはt->s方向のパスが格納されるため
// s-tパスが存在しない場合は、pathに変化なし
// p:sの親
bool dfs_tree_path(Graph &grp, int s, int t, vector<int> &path, int rev = 1, int p = -1)
{
	if(rev == 1) swap(s, t);
	if(s == t)
	{
		path.push_back(s);
		return true;
	}
	for(auto &e : grp[s])
	{
		if(e == p) continue;
		if(dfs_tree_path(grp, e, t, path, 0, s))
		{
			path.push_back(s);
			return true;
		}
	}
	return false;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	Graph g(N);
	vector<vector<int>> ed_no(N, vector<int>(N, -1));  // 逆引き用 ed_no[a][b]:頂点a,bを結ぶ辺番号
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
		ed_no[a][b] = ed_no[b][a] = i;
	}
	int M; cin >> M;
	vector<ll> con(M);  // com[x]:x番目の制約について、i番目の辺を通るならiビット目を1
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		vector<int> path;
		dfs_tree_path(g, u, v, path);
		for(j = 0; j < (int)path.size()-1; j++)
		{
			ll e = ed_no[path[j]][path[j+1]];
			con[i] |= 1LL<<e;
		}
	}

	ll ans = 0;
	for(i = 1; i < (1<<M); i++)
	{
		ll mxcon = 0;
		// 複数の制約をor条件で組み合わせる
		for(j = 0; j < M; j++)
		{
			if((i>>j)&1) mxcon |= con[j];
		}
		int cnt = __builtin_popcountll(mxcon);
		ll tmp = modpow(2, (N-1)-cnt, INF64);
		if(__builtin_popcountll(i)%2 == 1) ans += tmp;
		else ans -= tmp;
	}
	ans = (1LL<<(N-1)) - ans;
	cout << ans << endl;

	return 0;
}
