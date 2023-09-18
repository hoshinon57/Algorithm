#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC146 https://atcoder.jp/contests/abc146

/*
 * 累積和＋αで解く。
 * 公式解説の考え方が近かった。
 *   https://img.atcoder.jp/abc146/editorial.pdf
 * 
 * 入力例1でいろいろ操作してみると、
 *   入力  ：1 4 2 3 5
 *   累積和：0 1 5 7 10 15 (先頭に0付与)
 * であり、あるa_sum[i]について
 *   x個前のa_sum[j]との差分が、a_sum[i]との差をKで割った余りと等しい
 * ときに条件を満たすことが分かる。
 * そこでa_sum[i]をそれぞれインデックス分引いてみて、
 *   0 0 3 4 6 10
 * そこからKで割った余りは
 *   0 0 3 0 2 2  -> これをa_sum_ex[]と呼ぶ
 * となり、数値が一致する箇所と、条件を満たす部分列は一致することが分かる。
 * 
 * よって左から順に見ていって、a_sum_ex[i]と同じ値が左に何個あるかをmapを用いてカウントしていけばよい。
 * ただしK個以上左にある要素は除外する点に注意。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・要素の数や距離を考えるときは、各要素からインデックスを引いてみる
 * ・連続する値の和、ならば累積和を作ってみる
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> a_sum_ex(N+1);
	// 累積和を取り、そこからインデックス分引いてKで割った余りを設定する
	for(i = 0; i < N; i++)
	{
		a_sum_ex[i+1] = a_sum_ex[i] + a[i];
	}
	for(i = 0; i < N+1; i++)
	{
		a_sum_ex[i] -= i;
		a_sum_ex[i] %= K;
	}

	map<ll,ll> cnt;
	ll ans = 0;
	for(i = 0; i < N+1; i++)
	{
		// K個前を捨てる
		if(i-K >= 0)
		{
			cnt[a_sum_ex[i-K]]--;
		}
		ans += cnt[a_sum_ex[i]];
		cnt[a_sum_ex[i]]++;
	}
	cout << ans << endl;

	return 0;
}
