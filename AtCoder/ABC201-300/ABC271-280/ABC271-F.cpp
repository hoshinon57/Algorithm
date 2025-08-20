#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 2025/8に再解きしたものに置き換えた。
// 先頭の解説はそのまま。
// 主なソースコードの変化点は、
// ・前から、後ろからをそれぞれのDFS関数を用意した。
//   盤面を回転しながら関数を1つで流用するより、2つ書いちゃう方が楽な気がする。
// ・前から、はvectorで、後ろから、はunordered_mapで管理した。

// ABC271 https://atcoder.jp/contests/abc271

/*
 * コンテスト時に解けず、解説を見た。
 * 半分全列挙で解く。
 * 
 * N*N行列のとき、(0,0)から(N-1,N-1)への移動回数は(N-1)+(N-1)となる。
 * うち(N-1)回を縦方向への移動とすると、 (2N-2)C(N-1) が移動方法の数となる。
 * これはN=20のとき3*10^10通りとかになり、愚直に探索してもTLE.
 * 
 * ここで半分全列挙を用いる。
 * x+y=N-1 となる対角線のマスは必ず通ることから、
 *   (1,1)からのxor
 *   (N,N)からのxor
 * を各対角線のマスについて組み合わせればよい。
 * これはmapを用いて、 mp[i][xor]:上からi行目のゴールについて、そこまでのxor値の組み合わせ と定義した。
 * (N,N)からは、盤面を反転させると同じ処理を使いまわせて楽。
 * ※反転は、90度回転のライブラリを持ってたのでそれを2回行った
 * 
 * 対角線のマスは2回計算されることになる点に注意。
 * (N,N)からの探索時は計算から除外するようにした。
 * これにより、(1,1)からの探索と(N,N)の探索とで、値が一致するものが組み合わせ可能となる。
 * 
 * 組み合わせは、mp[i]とmp[N-i-1]を比較する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・移動回数は2(N-1)回。つまりN=20のとき38回となる。
 *   メタだが、40に近いので半分全列挙を考えてみる。
 */

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<vector<ll>> a(N, vector<ll>(N, 0));  // a[N][N]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++) cin >> a[i][j];
	}

	vector<vector<ll>> fr(N);  // fr[y] = {xor, xor, ...}
	auto dfs_front = [&](auto &self, ll y, ll x, ll val) -> void
	{
		val ^= a[y][x];
		if(y+x+1 == N)
		{
			fr[y].push_back(val);
			return;
		}
		self(self, y+1, x, val);
		self(self, y, x+1, val);
	};
	dfs_front(dfs_front, 0, 0, 0);

	vector<unordered_map<ll,ll>> bk(N);  // bk[y][xor] = xorとなる数
	auto dfs_back = [&](auto &self, ll y, ll x, ll val) -> void
	{
		if(y+x+1 == N)
		{
			bk[y][val]++;
			return;
		}
		val ^= a[y][x];  // bk側は終端でxorさせないようにする
		self(self, y-1, x, val);
		self(self, y, x-1, val);
	};
	dfs_back(dfs_back, N-1, N-1, 0);

	ll ans = 0;
	for(ll y = 0; y < N; y++)  // fr[y]とbk[y]を組み合わせる
	{
		for(auto &e : fr[y])
		{
			if(bk[y].count(e) > 0) ans += bk[y][e];
		}
	}
	cout << ans << endl;

	return 0;
}
