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

// ABC382 https://atcoder.jp/contests/abc382

/*
 * 期待値DPで解く。いやーそれまでに一手必要な、難しかったけど良問だった。
 * 
 * まず、パックを1つ開けたときにレアカードが何枚入っているか、を求める。
 *   dp[i]:パックを1つ開けて、レアがi枚である確率 (i=0～N)
 * として、先頭のカードからPを使って計算できる。
 * 
 * で、すごろくっぽく考えたときに、
 *   スタートマスが0, ゴールマスがX
 *   N面のサイコロを1回振るごとにdp[i]マス進める
 *   サイコロの振る回数の期待値は？
 * という問題に読み替えることができる
 * ans[i]をマスiからゴールマスまで行くための期待値とすると、答はans[X].
 * 
 * ans[i] = Σ{j=0～N}((ans[i+j]+1)*dp[j])
 * となり、右辺にもans[0]が登場するため式整理して
 * ans[i] = ( Σ{j=1～N}((ans[i+j]+1)*dp[j]) + dp[0] ) / (1-dp[0])
 * となる。
 * これをansの大きい方から求めていく。
 */

int main(void)
{
	ll i, j;
	ll N, X; cin >> N >> X;
	vector<double> p(N); for(i = 0; i < N; i++) {cin >> p[i]; p[i]/=100;}

	vector<double> dp(N+1);
	dp[0] = 1.0;
	for(i = 0; i < N; i++)
	{
		vector<double> ndp(N+1);
		for(j = 0; j <= N; j++)
		{
			ndp[j] += dp[j] * (1.0-p[i]);
			if(j != N) ndp[j+1] += dp[j] * p[i];
		}
		swap(dp, ndp);
	}

	vector<double> ans(X+1);
	ans[X] = 0.0;
	for(i = X-1; i >= 0; i--)  // ans[i]を求める
	{
		double sum = 0.0;
		for(j = 1; j <= N; j++)  // jマス進む
		{
			int nxt = min(i+j,X);
			sum += (ans[nxt]+1.0) * dp[j];
		}
		sum += dp[0];
		ans[i] = sum / (1.0-dp[0]);
	}
	cout << std::fixed << std::setprecision(20);
	cout << ans[0] << endl;

	return 0;
}
