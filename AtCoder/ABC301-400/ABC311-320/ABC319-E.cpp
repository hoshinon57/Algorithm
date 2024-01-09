#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC319 https://atcoder.jp/contests/abc319

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc319/editorial/7100
 *   https://twitter.com/kyopro_friends/status/1700509057965326372
 * 
 * 各PのLCMをXとすると、出発時刻がkとk+Xとでは各バス停での待ち時間は同じとなる。
 * P<=8という制約より、LCM(1,2,3,...,8)=840であるため、
 * 出発時刻が10と850の場合はバスの周期が同じ。
 * 
 * よって出発時刻を0～839の計840通りについて、移動時間を事前にO(N)で計算しておく。
 * するとクエリqについては
 *   cal[q%840] + q-q%840
 * とO(1)で求められる。
 * ※q-q%840は(q/840)*840でも良い
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・うーん、これどうすれば思いつけるようになるんだろうか…
 *   Nが小さい場合で手で書いてみて規則性を見つける？
 *   「Pの倍数で出発」だったらLCMにひもづけて考える？
 */

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

int main(void)
{
	const ll m = 840;  // LCM(1,2,...,8)
	int i, j;
	ll N, X, Y; cin >> N >> X >> Y;
	vector<ll> p(N-1), t(N-1);
	for(i = 0; i < N-1; i++) cin >> p[i] >> t[i];

	// 事前計算
	vector<ll> cal(m);
	for(i = 0; i < m; i++)  // 時刻iにスタート
	{
		ll now = i+X;
		for(j = 0; j < N-1; j++)
		{
			now = ceil_div(now, p[j]) * p[j] + t[j];  // ceil(now/p[j])*p[j] にバスが出発
		}
		now += Y;
		cal[i] = now;
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll q; cin >> q;
		ll ans = cal[q%m] + (q - q%m);
		// ll ans = cal[q%m] + (q/m)*m;  // こちらでも同じ
		cout << ans << endl;
	}

	return 0;
}
