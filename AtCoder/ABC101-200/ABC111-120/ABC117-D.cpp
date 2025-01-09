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

// ABC117 https://atcoder.jp/contests/abc117

/*
 * 桁DPにて解く。
 * 
 * 桁数を固定し(MB)、例えばK=5なら b'0000....0101 として見るイメージ。
 * xorの加算なので、leading-zeroの考慮は不要。
 * 
 * dp[i][sm]を以下のように定義する。
 *   i bit目まで見て、 (i=0～(MB-1))
 *   そこまでの値が、N未満ならsm=1, Nと一致ならsm=0のときに、
 *   問題文にある式の最大値。
 * ★実装ではdp,ndpを用いて[i]の次元を削除している。以下その前提で記載する。
 * max(dp[0],dp[1])が答。
 * 
 * 事前にcnt[bit][j]として
 *   Aiのbit目が(1^j)である要素数
 * として計算しておく。
 * これにより、DP遷移にて次の値がdのときにcnt[*][d]にてXOR結果が1になる要素数が取得できる。
 * 
 * DP遷移は実装を参照。
 * 上の桁から、かつ入力が文字列ではなく数値であるため、iのループをデクリメントにしている。
 * 
 * ※最初、MBを50にしていて(2^50=10^15)
 *     cnt[i-1][d] * (1LL << (i-1));
 *   の部分でll型でもオーバーフローしてしまっていた。
 */

int main(void)
{
	const ll MB = 42;  // 0-indexedでこのbit数見る
	ll i, j, sm;
	ll N, K; cin >> N >> K;
	vector<ll> a(N);
	vector<vector<ll>> cnt(MB, vector<ll>(2, 0));
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		for(j = 0; j < MB; j++)
		{
			ll tmp = 1 ^ ((a[i]>>j)&1);  // xorするので反対側をインクリメント
			cnt[j][tmp]++;
		}
	}

	vector<ll> dp(2, -INF64);
	dp[0] = 0;
	// 配るDP
	for(i = MB-1; i > 0; i--)
	{
		vector<ll> ndp(2, -INF64);
		for(sm = 0; sm < 2; sm++)  // dp[i][sm]から配る
		{
			const int D = (K>>(i-1))&1;  // 配る先の値
			int ed = (sm ? 1 : D);
			for(int d = 0; d <= ed; d++)  // 次に使う値
			{
				int nsm = (sm || d < D);
				ll tmp = cnt[i-1][d] * (1LL << (i-1));  // 配る先にて、xor計算の総和がいくつになるか
				chmax(ndp[nsm], dp[sm] + tmp);
			}
		}
		swap(dp, ndp);
	}
	cout << max(dp[0], dp[1]) << endl;

	return 0;
}
