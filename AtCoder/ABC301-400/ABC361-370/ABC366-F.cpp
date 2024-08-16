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

// ABC366 https://atcoder.jp/contests/abc366

/*
 * 自力で解けず、解説を見た。動的計画法で解く。
 * 類題はEDPC-X(Tower) https://atcoder.jp/contests/dp/tasks/dp_x
 *   https://x.com/kyopro_friends/status/1822269025138708505
 * 
 * 例えばf1,f2について考えたとき、f2(f1(x))とf1(f2(x))のどちらが大きくなるかは、実はxによらず一意に決まる。
 *   f1=ax+b, f2=cx+dとしたときに、
 *   F1:f2(f1(x))=f2(ax+b)=acx+bc+d
 *   F2:f1(f2(x))=f1(cx+d)=acx+ad+b
 *   F1-F2=bc+d-(ad+b)=b(c-1)-d(a-1)
 * よりxに無関係であり、b(c-1)>d(a-1)であればF1>F2よりf1を先に使った方が良い。
 * よってまずはf1,f2,...,fNを上記の判定を用いてソートする。
 * 
 * 次に、ソートされたfから相異なるK個を選ぶことを考える。
 * ※先頭だか末尾のK個を選べばいいんじゃ？ と思ったが、そうではないみたい。
 *   実際、入力例1はソートするとf2->f1->f3と並ぶが、使うのはf2,f3である。
 * これはDPを用いて「i個目まで見て、j個選んだときの最大値」で求められる。
 * dp[N][K]が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・DPにて要素を使う順番が任意であるときに、実は最適な順序が一意に決まるケースがある、という考え方。 [典型]
 */

using pll = pair<ll,ll>;
bool mycomp(pll x, pll y)
{
	auto[a,b] = x;
	auto[c,d] = y;
	// return (b*(c-1) - a*(d-1) > 0);  // 計算式ミスってしばらくWAが続いた
	return (b*(c-1) > d*(a-1));
}

int main(void)
{
	int i, j;
	ll N, K; cin >> N >> K;
	vector<pll> ab(N);
	for(i = 0; i < N; i++) cin >> ab[i].first >> ab[i].second;
	sort(ab.begin(), ab.end(), mycomp);

	vector<vector<ll>> dp(N+1, vector<ll>(K+1, -INF64));  // dp[N+1][K+1]
	dp[0][0] = 1;
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= K; j++)
		{
			dp[i][j] = dp[i-1][j];  // i個目を使わないケース
			if(j == 0) continue;
			if(dp[i-1][j-1] == -INF64) continue;  // 積でのオーバーフローを防ぐ (DP初期値を -INF64/100 とかでも良い)
			// i個目を使うケース
			ll tmp = ab[i-1].first * dp[i-1][j-1] + ab[i-1].second;  // ab[]は0-indexedなので、[i-1]でアクセス
			chmax(dp[i][j], tmp);
		}
	}
	cout << dp[N][K] << endl;

	return 0;
}
