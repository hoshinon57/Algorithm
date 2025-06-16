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

// ABC410 https://atcoder.jp/contests/abc410

/*
 * dp[i]:
 *   その敵まで見て、
 *   残HP=iのときの、残魔力の最大値
 * としてDP.
 * 
 * 似た問題：
 *   https://atcoder.jp/contests/abc364/tasks/abc364_e (Maximum Glutton)
 *   https://atcoder.jp/contests/abc325/tasks/abc325_f (Sensor Optimization Dilemma)
 * 
 * [ACまでの思考の流れ]
 * ・dp[N][H][M]とかだと到底間に合わない。
 *   上記dpの持ち方だとvalはbool値になる。
 *   valがbool値であればkeyをvalにする典型が使えないか。(キーと値の入れ替えテク)
**/

int main(void)
{
	ll i, j;
	ll N, H, M; cin >> N >> H >> M;
	vector<ll> a(N), b(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i];}

	vector<ll> dp(H+1, -INF32);  // dp[i]: そこまで見て、残HP=iのときの、残魔力の最大値 できないなら-INF
	dp[H] = M;
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		vector<ll> ndp(H+1, -INF32);
		bool ok = false;  // 敵iを倒すことができるか
		for(j = 0; j <= H; j++)
		{
			// 物理攻撃
			// dp[j]のjがAiだけ減る  もらうDPならndp[j]とdp[j+Ai]
			if(j+a[i] <= H) chmax(ndp[j], dp[j+a[i]]);

			// 魔法攻撃
			// dp[j]の値がBiだけ減る  もらうDPならndp[j]とdp[j]-Bi
			if(dp[j] - b[i] >= 0) chmax(ndp[j], dp[j]-b[i]);

			if(ndp[j] >= 0) ok = true;
		}
		if(ok)
		{
			ans = i+1;
		}
		swap(dp, ndp);
	}
	cout << ans << endl;

	return 0;
}
