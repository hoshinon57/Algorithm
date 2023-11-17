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

// ABC216 https://atcoder.jp/contests/abc216

/*
 * 二分探索で解く。
 * 
 * 楽しさが大きい順に乗っていくのが最善。
 * ただし愚直に1つずつ乗っていくとKの制約により間に合わない。
 * ※各種解説を見ると、ここをうまく処理することで貪欲法的な感じで解けるみたい。
 * 
 * 「楽しさがx以上のアトラクションに乗る場合に、乗れる回数」という判定問題を考える。これはO(N)で解ける。
 * xを増やしていくと、どこかで乗れる回数が
 * 　K回より多い
 * 　K回以下
 * が切り替わる。ここの間に答があり、切り替わりを二分探索で求める。
 * 
 * 判定関数にて乗れる回数以外に「楽しさの総和」を取得できるようにしておく。
 * 二分探索にて
 *   l:K回より多い
 *   r:K回以下
 *   r-l=1
 * となるl,rが求まるので、rのときの楽しさに加え、
 * 残り回数にlをかけて足したものが答となる。
 * 
 * [ACまでの思考の流れ]
 * ・「楽しさ」が多い順に乗っていくのが最善だが、愚直に計算するとTLE.
 * ・逆に考えて、「楽しさがx以上のアトラクションがある限り、乗り続ける」とすると
 *   乗れる回数がK回をまたぐ部分がどこかに存在する。
 *   これを二分探索で求める。
 * ・端数が出てくるので、その計算には注意。
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll l = -1;
	ll r = INF64;
	// 楽しさがx以上のアトラクションに乗る場合に、乗れる回数を返す
	// またamuにはその楽しさの総和を代入する
	auto func = [&](ll x, ll &amu) -> ll
	{
		ll tot = 0;
		ll cnt = 0;
		amu = 0;
		for(auto &e : a)  // e:アトラクションの楽しさ
		{
			if(e < x) continue;
			// e+(e-1)+(e-2)+...+x を求める
			cnt = e-x+1;  // 乗れる回数
			amu += (e+x)*cnt/2;
			tot += cnt;
		}
		return tot;
	};
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		ll amu;
		if(func(mid, amu) > K) l = mid;
		else r = mid;
	}

	ll ans = 0;
	ll n = K - func(r, ans);  // n:残り回数
	if(r != 0) ans += n*l;
	cout << ans << endl;

	return 0;
}
