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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC113 https://atcoder.jp/contests/abc113

/*
 * DPで解く。
 * 
 * dp,ndpのやり方で、dp[i]を
 *   上からある地点まで見たときの、左からi番目の位置にいる通り数
 * と定義する。
 * dp[K]が答。(0-indexedに変換する)
 * 
 * 横線の置き方をbit全探索する。
 * このとき、1になっているbitが連続していたらNGなので、それはチェックして除外しておく。
 * tmp=dpとしておき、横線があればtmp[i],tmp[i+1]をswapすると上手くいく。
 * 各探索にてこのtmpをndpに加算していく。
 */

bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)

int main(void)
{
	ll H, W, K; cin >> H >> W >> K;
	K--;  // 0-indexed
	vector<mint> dp(W);
	dp[0] = 1;  // DP初期値

	// nにてbitが1なのが連続していたらNG
	auto ok = [](ll n) -> bool
	{
		for(ll ii = 0; ii < 10; ii++)
		{
			if(isbiton(n, ii) && isbiton(n, ii+1)) return false;
		}
		return true;
	};

	ll i, j;
	for(ll _ = 0; _ < H; _++)
	{
		vector<mint> ndp(W);
		for(i = 0; i < (1<<(W-1)); i++)  // 横線は(W-1)本
		{
			if(!ok(i)) continue;
			vector<mint> tmp = dp;
			for(j = 0; j < (W-1); j++)
			{
				if( ((i>>j)&1) == 1 )
				{
					swap(tmp[j], tmp[j+1]);  // 横線があるので、交換
				}
			}
			for(j = 0; j < W; j++)
			{
				ndp[j] += tmp[j];
			}
		}
		swap(dp, ndp);
	}
	cout << dp[K].val() << endl;

	return 0;
}
