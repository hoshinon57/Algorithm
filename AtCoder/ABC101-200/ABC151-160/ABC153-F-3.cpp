#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC153 https://atcoder.jp/contests/abc153

/*
 * 尺取り法＆いもす法にて解く。
 * 
 * ・左側の敵から貪欲に倒していく
 * ・左側の敵にギリギリ当たるよう、なるべく右側で爆弾を使う
 * というのはABC153-F.cpp, ABC153-F-2.cppと同じ。
 * 
 * i番目のモンスターにギリギリ当たるよう右側に爆弾を使った時に、
 * ダメージを受けるモンスターの範囲を、半開区間を用いて [i, r[i]) で表す。
 * まずはr[i]を尺取り法で求める。
 * 
 * 次に、敵に与えたダメージの累計を total_damage[] にて管理する。
 * 左側の敵から順に見ていって、敵iのHPがまだ残っていれば、
 * total[i] にA*(爆弾の回数)を加算し、
 * total[r[i]] にA*(爆弾の回数)を減算する。
 * いもす法の要領で、iごとにtotal[i] += total[i-1]とすればよい。
 */

int main(void)
{
	// 0-indexed
	ll i;
	ll N, D, A;
	cin >> N >> D >> A;
	vector<pair<ll, ll>> mons(N);  // {座標, HP}
	for(i = 0; i < N; i++)
	{
		cin >> mons[i].first >> mons[i].second;
	}
	sort(mons.begin(), mons.end());

	// [i, r[i]) i番目のモンスターにギリギリ当たるよう右側に爆弾を使った時に、ダメージを受けるモンスターの範囲(半開区間)
	vector<ll> r(N);
	ll r_ = 0;
	for(i = 0; i < N; i++)
	{
		for(; r_ < N; r_++)
		{
			if(mons[i].first + 2*D < mons[r_].first) break;
		}
		r[i] = r_;
	}

	// 敵iにギリギリ当たるようなるべく右側で爆弾を使っていく -> [i,r[i]) の範囲の敵に当たる
	ll answer = 0;
	vector<ll> total_damage(N+1);  // 敵iに与えたダメージ累計  半開区間により[N]までアクセスするので+1
	for(i = 0; i < N; i++)  // i:モンスター
	{
		if(i != 0) total_damage[i] += total_damage[i-1];  // いもす法的な

		ll hp = mons[i].second - total_damage[i];  // 残HP
		if(hp <= 0) continue;  // 撃破済み

		ll count = (hp+(A-1))/A;  // 敵iに対して使う爆弾の数
		total_damage[i] += A*count;
		total_damage[r[i]] -= A*count;  // ダメージを受けるのはr[i]の手前まで
		answer += count;
	}
	cout << answer << endl;

	return 0;
}
