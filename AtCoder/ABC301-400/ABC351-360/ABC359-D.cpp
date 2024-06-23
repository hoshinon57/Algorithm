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

// ABC359 https://atcoder.jp/contests/abc359

/*
 * 自力で解けず、解説を見た。DPで解く。
 * 
 * (i-1)文字目まで答が確定したとする。
 * そこから1文字足したものが条件を満たすかどうかは、末尾の(K-1)文字についての情報を持っていれば良さそう。
 * 
 * 別の考え方として、長さKの回文チェックが必要なので、
 * DP[i-1]からの遷移をふまえると、(K-1)文字ぶんの情報を持っていれば良さそう。
 * 
 * ということで、dp[i][j]を以下のように定義する。
 *   i文字目まで見て、  (i=1～N)
 *   末尾(K-1)文字、つまり[(i-K+2),i] の文字をbitで表現した値がjであるときの、  (j=0～2^(K-1)-1)
 *     (bit表現は0:A, 1:B)
 *   良い文字列の総数
 * 
 * DP遷移は、各iについて長さKのAB文字列パターンXを全て列挙し、
 *   Xの末尾が入力Sと一致すること
 *   Xが非回文であること
 * という事前チェックをしたのち、
 *   prev=(Xの先頭K-1ビットぶん)
 *   next=(Xの末尾K-1ビットぶん)
 * として dp[i][next] += dp[i-1][prev] となる。
 * 
 * DP初期値はdp[K-1][*]についてのみ求めておけばよくて、
 * 長さ(K-1)のAB文字列を全て列挙し、入力Sと矛盾しないものについて+1していく。
 * (長さK未満なので、回文チェックは不要)
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・(i-1)文字目までの答が分かっているとき、1文字足すことでi文字目の答が出せそう。 ⇒DPっぽい。
 * ・回文判定より、直近(K-1)のデータを持ってスライドさせていくイメージ。
 *   類題：https://atcoder.jp/contests/joi2012yo/tasks/joi2012yo_d (JOI パスタ)
 */

bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

int main(void)
{
	ll i, j, k;
	ll N, K; cin >> N >> K;
	string s; cin >> s;
	s = "_" + s;  // 1-indexedに変換

	vector<vector<ll>> dp(N+1, vector<ll>(1<<(K-1), 0));  // dp[N+1][1<<(K-1)]
	// 初期値としてdp[K-1][*]を求める
	for(j = 0; j < 1<<(K-1); j++)  // dp[K-1][j]
	{
		bool ok = true;
		for(k = 1; k <= K-1; k++)  // s[k]  長さK-1個について矛盾が無ければOK
		{
			if(s[k] == '?') continue;
			if(s[k] == 'A' && ((j>>(k-1))&1) == 0) continue;
			if(s[k] == 'B' && ((j>>(k-1))&1) == 1) continue;
			ok = false;
		}
		if(ok) dp[K-1][j]++;
	}

	// 回文ならtrue
	auto checkP = [&](int x) -> bool
	{
		bool isP = true;
		for(int h = 0; h < K/2; h++)  // 切り捨てOK
		{
			if(isbiton(x, h) != isbiton(x, K-h-1)) isP = false;
		}
		return isP;
	};

	for(i = K; i <= N; i++)
	{
		for(j = 0; j < 1<<K; j++)  // dp[i][j]
		{
			// 入力Sの末尾とjに矛盾があれば飛ばす
			if(s[i] == 'A' && ((j>>(K-1))&1) != 0) continue;
			if(s[i] == 'B' && ((j>>(K-1))&1) != 1) continue;

			// 回文なら飛ばす
			if(checkP(j)) continue;

			ll prev=j, next=j;
			unbit(prev, K-1);  // 最上位bitを落とす
			next>>=1;
			dp[i][next] += dp[i-1][prev];
			dp[i][next] %= MOD;
		}
	}

	ll ans = 0;
	for(auto &e : dp[N])
	{
		ans += e;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
