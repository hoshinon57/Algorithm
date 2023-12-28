#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC215 https://atcoder.jp/contests/abc215

/*
 * bitDPで解く。
 * 
 * dp[i][j][k]を以下のように定義する。
 *   i番目のコンテストまで見て、  (i=1～N)
 *   参加済みのコンテストをbitで表記したものをjとして、  (j=0～2^10-1)
 *   直近に参加したコンテストがk  (k=0～9)
 *   のときの、選び方の総数
 * 
 * もらうDPで、dp[i][j][k]への遷移は以下のようになる。
 * (1)i番目のコンテストに不参加のケース
 *   dp[i-1][j][k]を加算する。
 * (2)コンテストkに連続参加するケース
 *   dp[i-1][j][k]をもう一度加算する。
 * (3)コンテストkに初参加するケース
 *   jから「コンテストkに参加したbitを落としたもの」をj'とすると、
 *   dp[i-1][j'][*] を全ての*について加算する。
 * (2)(3)は「i番目のコンテストがkであること」「jにコンテストkに参加のbitが立っていること」
 * という条件があることに注意。
 * 
 * dp初期条件は、dp[0][0][0]=1のみで上手くいく。(説明省略)
 * 
 * dp[N][j][k] ただしj!=0 の総和が答。
 */

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

int main(void)
{
	int i, j, k;
	int N; cin >> N;
	string s; cin >> s;
	vector dp(N+1, vector((1<<10), vector<ll>(10, 0)));
	for(i = 0; i < 10; i++) dp[0][0][0] = 1;  // 初期値はこれで上手くいく
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j < 1<<10; j++)
		{
			for(k = 0; k < 10; k++)  // コンテストの種類
			{
				ll &d = dp[i][j][k];
				d = dp[i-1][j][k];  // i番目のコンテストに不参加

				// 以下、i番目のコンテストに参加することを考える
				if(s[i-1]-'A' != k) continue;  // i番目のコンテスト != k
				if(!isbiton(j, k)) continue;  // jに「コンテストkに参加」のbitが立っていない

				// コンテストkに初参加
				// jからコンテストkを消したものを加算していく
				ll jj = j;
				unbit(jj, k);
				for(int h = 0; h < 10; h++)
				{
					d += dp[i-1][jj][h];
				}

				// コンテストkに連続参加
				d += dp[i-1][j][k];
				d %= MOD;
			}
		}
	}

	ll ans = 0;
	for(j = 1; j < 1<<10; j++)  // j=0は1つもコンテストに参加していないためNG
	{
		for(k = 0; k < 10; k++)
		{
			ans += dp[N][j][k];
			ans %= MOD;
		}
	}
	cout << ans << endl;
	return 0;
}
