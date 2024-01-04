#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC269 https://atcoder.jp/contests/abc269

/*
 * 2次元累積和的な考え方で解く。
 * 以下、問題文の通り1-indexedおよび閉区間で考える。
 * 
 * 以下、マス(i,j)の値をn(i,j)とする。
 * (1,1)から(y,x)までの総和を求める関数をfunc(y,x)とすると、
 *   func(B,D) - func(B,C-1) - func(A-1,D) + func(A-1,C-1)
 * が答となる。
 * よってfunc()を高速に求めればよい。
 * 
 * (1)
 * y,xをそれぞれ超えない最大の偶数をey,exとし、まずは(1,1)から(ey,ex)の範囲を考える。
 * この総和は
 *   1+n(ey,ex) の値が(ey*ex/2)個あったときの、その半分
 * になる。
 * 
 * (2)
 * 次にyが奇数のとき、最後の奇数行目を考えると(ただし(y,x)マスは除く)
 *   初項 n(y,1)
 *   末項 (初項)+(x/2-1)*2   ※公差は2
 *   項数 x/2
 * の等差数列の和となる。
 * (2')xが奇数のときも同様。ただし公差は2*M.
 * 
 * (3)
 * 最後にy,xがともに奇数のとき、(y,x)を考える。
 * これはn(y,x)で良い。
 * 
 * 以上、(1)-(2)-(2')+(3)がfuncの解となる。
 * 
 * [ACまでの思考の流れ]
 * ・特定の範囲は、マス(1,1)から(y,x)までの和を求める関数を用意することで
 *   2次元累積和の考え方で差分計算することができる。
 * ・例1の図から周期性を考えてみる。
 *   どうも偶数で区切ると一般化できそう。奇数部分は別途計算する感じで。
 */

ll N, M;

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

ll n(ll y, ll x)
{
	return ((y-1)*M+x)%MOD;
}

// (1,1)から(y,x)までの総和を求める 閉区間
ll func(ll y, ll x)
{
	ll ans = 0;
	ll inv2 = modinv(2, MOD);
	ll ey = (y/2)*2%MOD, ex = (x/2)*2%MOD;

	// (1,1)から偶数行/偶数列ぶん計算する
	// 1+n(ey,ex) の値が(ey*ex/2)個あったときの、その半分
	ll tmp = 1+n(ey, ex);
	tmp = tmp * (ey*ex%MOD*inv2%MOD) % MOD * inv2 % MOD;
	ans += tmp;

	// yが奇数のとき、最後の奇数行目 (ただし(y,x)マスを除く)
	if(y%2 == 1)
	{
		// 初項 n(y,1)
		// 末項 (初項)+(x/2-1)*2   ※公差は2なので*2
		// 項数 x/2
		ll a = n(y, 1);
		ll b = a+(x/2-1)*2;  // 末項
		tmp = x/2* (a+b) % MOD * inv2 % MOD;
		ans += tmp;
	}
	// 同様にxが奇数のとき、最後の奇数列目
	if(x%2 ==1)
	{
		ll a = n(1, x);
		ll b = a+(y/2-1)*(2*M)%MOD;  // 公差2M
		tmp = y/2 * (a+b) % MOD * inv2 % MOD;
		ans += tmp;
	}
	ans %= MOD;
	// y,xがともに奇数のときの、マス(y,x)
	if(y%2 == 1 && x%2 == 1)
	{
		ans += n(y, x);
	}
	ans %= MOD;

	return ans;
}

void solve(void)
{
	ll a, b, c, d; cin >> a >> b >> c >> d;
	ll ans = 0;
	ans += func(b, d);
	ll tmp1 = func(b, c-1);
	ll tmp2 = func(a-1, d);
	ll tmp3 = func(a-1, c-1);
	ans = (ans + MOD - tmp1) % MOD;
	ans = (ans + MOD - tmp2) % MOD;
	ans = (ans + tmp3) % MOD;
	cout << ans << endl;
}

int main(void)
{
	cin >> N >> M;
	int Q; cin >> Q;
	while(Q > 0)
	{
		solve();
		Q--;
	}

	return 0;
}
