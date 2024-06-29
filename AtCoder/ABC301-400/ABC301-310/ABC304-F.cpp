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

// ABC304 https://atcoder.jp/contests/abc304

/*
 * 自力で解けず、解説を見た。
 * 周期Mの短い方から求めていく。
 *   https://x.com/kyopro_friends/status/1665005518480617472
 *   https://scrapbox.io/kani/ABC304_-_F_-_Shift_Table
 *   https://qiita.com/hossie/items/1f9fd893928cf7327119
 * 
 * まず青木君の周期Mがeのときについて考えてみる。
 * 入力Sを周期eごとに分割し、'.'となるiが1つでも存在すれば(i%e)日目は出勤必要。
 * そうでない部分は出勤or休みどちらでもよく、
 * この数をcとすると2^cのシフト表を作れる。
 * 
 * 入力例3 ####.####.## を例とすると、M=4について
 *   ####
 *   .###
 *   #.##
 * より、周期4にて1,2日目は出勤必要で、3,4日目はどちらでもよい。
 * よって2^2=4パターン作れる。
 * 
 * しかしM=4にて #### のケースと、M=2にて ## のケースがダブるのが厄介。
 * M=2にて作ったシフト表はM=4,6,8,... などその倍数でも作れることから、
 * Mが小さい方から計算して重複を減算すればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・約数について何やかんや行うとき、小さい方から計算していき、
 *   その倍数については減算することでダブリを除外するという考え方。
 */

// Nの約数を列挙してdivisor[]に入れる
// 参考：https://qiita.com/drken/items/a14e9af0ca2d857dad23
void func_divisor(long long N, vector<long long> &divisor)
{
	for(long long i = 1; i*i <= N; i++)
	{
		if( N%i == 0 )  // iで割り切れるなら約数
		{
			divisor.push_back(i);
			if( i*i != N )
			{
				// N=16, j=2のとき、8も約数になる。それを追加する。
				// (これを行うことで、約数列挙の計算量がO(N)からO(√N)になる)
				// ただしj*j=N(例：4*4=16)といった場合を
				// 追加してしまうと重複してしまうので、それは省く。
				divisor.push_back(N/i);
			}
		}
	}

	// ソート有無は必要に応じて
	sort(divisor.begin(), divisor.end());

	return;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	vector<ll> cnt(N+1);  // cnt[i]:M=iにおけるシフト表の個数

	vector<ll> div;
	func_divisor(N, div);
	div.pop_back();  // M自身は不要

	ll ans = 0;
	for(auto &e : div)  // e:Mの約数
	{
		vector<bool> wk(e, false);  // wk[i]:周期e日でi日目に働く必要があればtrue
		ll c = e;  // 働くor働かない どっちでも良い日の数
		for(i = 0; i < N; i++)
		{
			if(s[i] == '.' && !wk[i%e])
			{
				wk[i%e] = true;
				c--;
			}
		}
		cnt[e] += modpow(2, c, MOD);  // シフト表の数は2^c
		cnt[e] %= MOD;
		ans += cnt[e];
		ans %= MOD;

		// 重複分を減らす
		// (e=3におけるシフト表は、e=6,9,12,... で重複するため減算する)
		for(i = e*2; i < N; i+=e)
		{
			cnt[i] = cnt[i] - cnt[e] + MOD;
			cnt[i] %= MOD;
		}
	}
	cout << ans << endl;

	return 0;
}
