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

// ABC095 https://atcoder.jp/contests/abc095

/*
 * 累積和のMAX. 累積MAXで解く。
 *   https://blog.hamayanhamayan.com/entry/2018/04/21/225805
 *   https://hiramekun.hatenablog.com/entry/2019/04/10/011230
 *   evima氏の公式解説
 * 
 * 向きを変えるのは1回までだろう、となる。
 * (1)時計回りにi個取り、そのまま退店するケース
 * (2)時計回りにi個取り、
 *   原点まで(反時計回りで)戻り、
 *   反時計回りに最大で(N-i)まで取り、退店するケース
 * の2パターンになりそう。
 * 
 * 時計回りにi個取り、は累積和で事前計算。
 * 反時計回りに最大で(N-i)まで取り、は累積和を取りつつ、そこまでの累積maxを事前計算。
 * とすれば上記のiについて全探索して解ける。
 * 
 * …が、サンプル4が合わず。
 * 悩んだ結果、「反時計回りにi個取り、そのまま退店」「反時計回りにi個、原点戻り、時計回りに最大(N-i)個取って退店」
 * という反対向きのケースが考慮漏れしていた。
 * 入力値を反転させて対応。
 * 
 * [ACまでの思考の流れ]
 * ・何らか固定して全探索したい気持ち。
 * ・「こう動くのが最適だろう」というのを検討した。
 * ・累積MAXという考え方。
**/

int main(void)
{
	ll i;
	ll N, C; cin >> N >> C;
	vector<ll> x(N), v(N); for(i = 0; i < N; i++) {cin >> x[i] >> v[i];}

	ll ans = 0;
	auto solve = [&](void) -> ll
	{
		vector<ll> sum(N+1);  // sum[i]:時計回りにi個取ったときの、差し引きカロリー
		ll prev = 0;
		for(i = 0; i < N; i++)
		{
			sum[i+1] = sum[i] - (x[i]-prev) + v[i];
			prev = x[i];
		}
		vector<ll> sumr(N+1), maxr(N+1);  // 反時計回りにi個取った時の差し引きカロリーと、そこまでのsumrの最大値(累積MAX)
		prev = C;
		for(i = 0; i < N; i++)
		{
			sumr[i+1] = sumr[i] - (prev-x[N-i-1]) + v[N-i-1];
			maxr[i+1] = max(maxr[i], sumr[i+1]);
			prev = x[N-i-1];
		}

		ll ret = 0;
		for(i = 0; i <= N; i++)
		{
			chmax(ret, sum[i]);  // 時計回りにi個取り、そのまま退店

			// 時計回りにi個取り、原点に戻り、反時計回りに最大(N-i)個
			ll tmp;
			if(i != 0) tmp = sum[i] - x[i-1] + maxr[N-i];
			else tmp = maxr[N];
			chmax(ret, tmp);
		}
		return ret;
	};

	chmax(ans, solve());
	// 反転
	for(auto &e : x) e = C-e;
	reverse(x.begin(), x.end());
	reverse(v.begin(), v.end());
	chmax(ans, solve());

	cout << ans << endl;

	return 0;
}
