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

// ABC317 https://atcoder.jp/contests/abc317

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i個目の選挙区まで見て、 (i=1～N)
 *   それまでの高橋派の議席数がjになるときの、  (j=0～ΣZ)
 *   必要な鞍替え人数
 * Zの総和は奇数なので、dp[N][(ΣZ+1)/2]～dp[N][ΣZ] の最小値が答。
 * 
 * 各選挙区について勝つためには何人を鞍替えさせればよいかを計算すればよく、
 * これは (X+Y+1)/2-X となる。ただし0で下限クリップ必要。
 * 
 * [ACまでの思考の流れ]
 * ・例えば1,2番目の選挙区について見終わったときに、
 *   以降の選挙区を見るにあたって必要な情報は「いくつ議席を取ったか？」であり、
 *   {2,0}と{1,1}のような取り方の詳細情報は「別にどうでもいい」となる。
 *   であれば、「これまでに取得した議席数」でDPができる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> x(N+1), y(N+1), z(N+1);
	ll zs = 0;  // ΣZ
	for(i = 1; i <= N; i++)
	{
		cin >> x[i] >> y[i] >> z[i];
		zs += z[i];
	}

	vector<vector<ll>> dp(N+1, vector<ll>(zs+1, INF64));  // dp[N+1][zs+1]
	dp[0][0] = 0;
	// もらうDP
	for(i = 1; i <= N; i++)
	{
		ll t = (x[i]+y[i]+1)/2 - x[i];  // 選挙区iで勝つための、鞍替え人数
		chmax(t, 0LL);  // 0未満はそもそも鞍替え不要
		for(j = 0; j <= zs; j++)
		{
			// 議席を取りにいかない
			dp[i][j] = dp[i-1][j];

			// 議席を取りにいく
			if(j-z[i] >= 0)
			{
				chmin(dp[i][j], dp[i-1][j-z[i]] + t);
			}
		}
	}
	
	ll ans = INF64;
	// dp[N][(zs+1)/2～zs]
	for(j = (zs+1)/2; j <= zs; j++)
	{
		chmin(ans, dp[N][j]);
	}
	cout << ans << endl;

	return 0;
}
