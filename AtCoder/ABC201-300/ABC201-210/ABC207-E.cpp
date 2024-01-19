#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC207 https://atcoder.jp/contests/abc207

/*
 * 自力で解けず、解説を見た。動的計画法で解く。
 *   https://blog.hamayanhamayan.com/entry/2021/06/27/005805
 *   https://twitter.com/kyopro_friends/status/1408789438126125064
 *   https://motsu-xe.hatenablog.com/entry/2021/06/26/232024
 * 
 * dp[i][j]を以下のように定義する。
 *   数列Aのi個目まで見て、  (i=1～N)
 *   そこで部分列Bjを作るときの、  (j=1～N)
 *   組み合わせの数
 * d[N][*]の総和が答になる。
 * 
 * dp[i][j]は、
 *   k<iかつ A(k+1)+A(k+2)+...+Ai の総和がjで割り切れる(※)ような全てのkについて、dp[k][j-1]を加算したもの
 * となる。
 * ただしこれだと計算量がO(N^3)となってしまう。
 * 
 * ここでAの累積和をa_sumとすると、
 * 上記(※)は a_sum[k]%j = a_sum[i]%j と同じこととなる。
 * よってDP計算をしつつ、 cnt[a_sum[i]%j] にdp[i][j-1]を加算していくことで、
 * dp[i][j] = cnt[a_sum[i]%j] として求めることができる。
 * これでO(N^2)で解ける。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・dp[]の定義は、まず数列のi個目まで見て、は定番。
 *   加えて、何個に切り分けたかという情報が無いと以降の遷移計算ができないので、これを追加する。
 * ・「ある区間[k,i]の和がjで割り切れる」は、
 *   累積和の(k-1)番目とi番目について、jで割った余りが等しいことで判定できる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> a_sum(N+1);
	for(i = 0; i < N; i++) a_sum[i+1] = a_sum[i] + a[i];
	// dp[i][j]
	//   a[]をi個目まで見て  i=1～N
	//   そこでBjを作れるときの  j=1～N
	//   組み合わせの数   mod 10^9+7
	vector<vector<ll>> dp(N+1, vector<ll>(N+1, 0));  // dp[N+1][N+1]
	dp[0][0] = 1;
	for(j = 1; j <= N; j++)  // jから先にループを回す B1,B2,...,を先に確定させる
	{
		vector<ll> cnt(N+5);
		if(j == 1) cnt[0] = 1;
		for(i = 1; i <= N; i++)
		{
			int idx = a_sum[i]%j;
//			dp[i][j] += cnt[idx];
//			dp[i][j] %= MOD;
			dp[i][j] = cnt[idx];
			cnt[idx] += dp[i][j-1];
			cnt[idx] %= MOD;
		}
	}
	ll ans = 0;
	for(i = 1; i <= N; i++)
	{
		ans += dp[N][i];
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
