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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC247 https://atcoder.jp/contests/abc247

/*
 * 自力で解けず、各種解説を見た。
 *   https://atcoder.jp/contests/abc247/editorial/3719
 *   https://x.com/kyopro_friends/status/1513152863501172737
 *   https://qiita.com/kym3535/items/fef1bdb574834610401d
 *   https://qiita.com/u2dayo/items/f7ba85d7a55111dc6168
 *   https://scrapbox.io/ac2000-kyopro/ABC247__F_-_Cards
 *   https://s9ora.hatenablog.com/entry/2022/04/17/193809
 *   https://note.com/syamashi/n/nae96b08f8a1d
 * 
 * まず、グラフとして考えてみる。
 * 入力をpi,qiを結ぶ辺としてグラフを構築すると、連結成分ごとにx頂点x辺のサイクルができることが分かる。(各頂点の次数は常に2)
 * 連結成分ごとのカードの選び方を計算して、その積が答になる。
 * また選び方はカードの枚数のみに依存し、カードの値には影響しない。
 * 
 * 「連結成分ごとのカードの選び方」が自力で解けず、解説を見た。
 * 辺被覆になるような辺の選び方になる模様。
 * リュカ数という漸化式があるようだが、自分はDPを実装した。 -> cal()
 * 連結成分における要素数をnとして、dp[i][j]を以下のように定義する。
 *   i番目の辺まで見て、  (i=0～n-1)
 *   その辺を選ぶか選ばないか j=0:選ばない, j=1:選ぶ
 *   のときの、辺被覆になるような選び方の数
 * 2連続で「選ばない」がNGなので、それを元にDP遷移が決まる。
 * また円環であるため、最初の辺を選ぶ or 選ばないの2ケースでDPする。(ex:ABC251-E(Takahashi and Animals) )
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・入力をグラフにし、連結成分ごとに考えるところまでは思いついた。
 *   DPが思いつけなかった。
 * ・全ての点を覆うように辺を選ぶ ⇒ 辺被覆 というのを覚えておく。
 * ・DPにて円環があり計算しづらい場合は、最初の要素を選ぶ/選ばないで場合分けをする。[典型]
 *   参考：ABC251-E(Takahashi and Animals)
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

// AC後に気づいたが、n=1でも正しく求められている
ll cal(int n)
{
	int i;
	ll ret = 0;

	// dp[i][j]
	//   j:その辺を選ぶなら1, 選ばないなら0
	for(int x = 0; x < 2; x++)
	{
		vector<vector<ll>> dp(n, vector<ll>(2, 0));  // dp[n][2]
		// 最初の辺を選ぶ or 選ばないで2ケース計算する
		if(x == 0) dp[0][1] = 1;  // 最初の辺を選ぶ
		else dp[0][0] = 1;  // 最初の辺を選ばない

		for(i = 1; i < n; i++)
		{
			dp[i][1] = (dp[i-1][1] + dp[i-1][0]) % MOD;
			dp[i][0] = dp[i-1][1];
		}

		// 最初の辺を選ぶ場合は、最後の辺は選ぶ/選ばないどちらでもOK
		// 最初の辺を選ばない場合は、最後の辺は選ぶことが必須
		if(x == 0) ret += dp[n-1][0] + dp[n-1][1];
		else ret += dp[n-1][1];
	}

	return ret % MOD;
}

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> p(N); for(i = 0; i < N; i++) {cin >> p[i]; p[i]--;}
	vector<int> q(N); for(i = 0; i < N; i++) {cin >> q[i]; q[i]--;}
	UnionFind uf(N);
	for(i = 0; i < N; i++) uf.unite(p[i], q[i]);

	ll ans = 1;
	for(i = 0; i < N; i++)
	{
		if(uf.root(i) != i) continue;  // 連結成分ごと
		ans = ans * cal(uf.size(i)) % MOD;
	}
	cout << ans << endl;

	return 0;
}
