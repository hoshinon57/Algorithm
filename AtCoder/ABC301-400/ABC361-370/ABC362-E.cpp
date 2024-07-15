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

// ABC362 https://atcoder.jp/contests/abc362

/*
 * 自力解けず、解説を見た。
 * 「DPで、初項と第2項を固定し」辺りまでをチラ見して実装した内容。
 * ★ただし場合分けや実装量の観点から、次に実装したABC362-E-2.cppの方が良さそう。
 * 
 * dp[i][j][l]を以下のように定義する。
 *   初項がAi, 第2項がAjで、 (i,j=1～N, 1<j)
 *   長さがlである、 (l>=2)
 *   等差数列の個数
 * 
 * i,jについて2重ループを回し、DP値を求めていく。
 * dp[*][*][2]は常に1.
 * 第2項より右側にある要素、つまりj<kとなるkについて探索していく。
 * ここでAi!=Aj, つまり公差が0以外か0かで場合分けをした。
 * 
 * (1)公差が0以外のとき
 * もしAkが等差数列のx番目の要素になりうる場合、
 *   (Ak-Ai)/(Aj-Ai)=x-1  を整理して
 *   x=(Ak-Ai)/(Aj-Ai)+1  となる。
 * 除算部分が割り切れる場合、Akはx番目の要素となるので、
 * dp[i][j][x]にdp[i][j][x-1]を加算すればよい。
 * 
 * (2)公差が0のとき
 * Ai=Akとなる場合、等差数列の何番目の要素にもなりうるので、
 * hの大きい方から順にdp[i][j][h]にdp[i][j][h-1]を加算していく。
 * 
 * dp[*][*][k]の総和が各kについての答。
 */

int main(void)
{
	// 1-indexed
	int i, j, k;
	ll N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}

	vector dp(N+1, vector(N+1, vector<ll>(N+1, 0)));
	for(i = 1; i <= N; i++)
	{
		for(j = i+1; j <= N; j++)  // 初項Ai, 第2項Aj
		{
			dp[i][j][2] = 1;  // dp初期値
			for(k = j+1; k <= N; k++)
			{
				if(a[i] != a[j])  // 公差!=0
				{
					int aki = a[k]-a[i], aji = a[j]-a[i];
					if(aki%aji == 0)
					{
						int x = aki/aji + 1;
						if(x > 2 && x <= N)
						{
							// Akは第x項
							dp[i][j][x] += dp[i][j][x-1];
							dp[i][j][x] %= MOD;
						}
					}
				}
				else
				{
					if(a[k] == a[i])
					{
						for(int h = N; h > 2; h--)
						{
							dp[i][j][h] += dp[i][j][h-1];
							dp[i][j][h] %= MOD;
						}
					}
				}
			}
		}
	}

	cout << N;  // k=1 このケースだけ特別に出力
	for(k = 2; k <= N; k++)
	{
		// dp[*][*][k]の総和
		ll ans = 0;
		for(i = 1; i <= N; i++)
		{
			for(j = 1; j <= N; j++)
			{
				ans += dp[i][j][k];
				ans %= MOD;
			}
		}
		cout << " " << ans;
	}
	cout << endl;

	return 0;
}
