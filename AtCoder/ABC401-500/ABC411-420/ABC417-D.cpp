#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC417 https://atcoder.jp/contests/abc417

/*
 * 難しい。自力で解けず、解説を見た。
 * 
 * 制約より、以下がポイントか。
 * ・初期テンションが大きいとき、500以下になるまではずっとテンションが下がり続ける。
 * ・テンションがいちど500以下(1000以下、とみなしてもよい)まで下がったら、
 *   以降は1000を超えることはない。
 * 
 * dp[i][j]を以下のように定義する。
 *   i個目までプレゼントもらった状態で、(i=1～N, i=0はスタート前)
 *   そのときのテンションがj (j=0～1000)
 *   での、全プレゼントをもらった後の最終テンション
 * dp[N][j] = j は明らかで、これをいわゆる初期値とすると、後ろから計算できる。
 * これを事前計算しておく。
 * (もしくは再帰でもよいらしい)
 * 
 * 各クエリXについて、X<=1000であれば上記DPからO(1)で求められる。
 * X>1000の場合を考えると、
 *   X>1000の間はテンションが必ず下がり続ける
 *   X<=1000になったら、上記DPで求められる
 * ことから、入力Bの累積和を取っておくことで、どのタイミングでX<=1000になるかが二分探索で求められる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・制約に着目する。
 *   P,A,Bが小さい。特に初期テンションXの最大値に比べて非常に小さい。
 *   O(N*max(P)) とかができそう。
 * ・いろいろな値でシミュレーションしてみて、法則を見つける。
 *   今回であれば N*max(P) の全範囲で事前計算をすれば事足りる、とか。
**/

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<ll> p(N), a(N), b(N); for(i = 0; i < N; i++) {cin >> p[i] >> a[i] >> b[i];}
	vector<ll> bsum(b.size()+1); for(int kk = 0; kk < (int)b.size(); kk++) {bsum[kk+1] = bsum[kk] + b[kk];}

	const ll mx = 1000;
	vector<vector<ll>> dp(N+1, vector<ll>(mx+1, 0));  // dp[N+1][1000+1]
	for(i = 0; i <= mx; i++) dp[N][i] = i;
	for(i = N-1; i >= 0; i--)
	{
		for(j = 0; j <= mx; j++)
		{
			if(j <= p[i]) dp[i][j] = dp[i+1][j+a[i]];
			else dp[i][j] = dp[i+1][max(j-b[i], 0LL)];
		}
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll x; cin >> x;
		ll ans;
		if(x > mx)
		{
			// x <= 1000になるまでワープ
			// (x-1000)だけ下げる
			auto itr = lower_bound(bsum.begin(), bsum.end(), x-1000);
			if(itr == bsum.end())
			{
				ans = x - bsum.back();  // 最後まで1000以下にならない
			}
			else
			{
				auto idx = itr - bsum.begin();  // idx回プレゼントをもらう
				ans = dp[idx][x - bsum[idx]];
			}
		}
		else
		{
			ans = dp[0][x];
		}
		cout << ans << endl;
	}

	return 0;
}
