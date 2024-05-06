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

// ABC042 https://atcoder.jp/contests/abc042

/*
 * 公式解説と同じ考え方だった。
 *   https://img.atcoder.jp/data/arc/058/editorial.pdf
 *   向きが異なるが、参考：https://drken1215.hatenablog.com/entry/2020/04/06/235844
 * 
 * 0-indexedで、またマスを(y,x)の順に記載する。
 * 
 * x=B～W-1のそれぞれのxについて
 *   (0,0) -> (H-1-A,x) -> (H-A,x) -> (H-1,W-1)
 * のルートを考えると、 (H-1-A,x) -> (H-A,x) の部分はxごとに唯一のルートとなる。
 * (ゴールまでに何らかのxに対して必ず1回だけ通る)
 * よって各xごとに
 *   (0,0) -> (H-1-A,x) のルート数
 *   (H-A,x) -> (H-1,W-1) のルート数
 * を積を求め、その総和が答となる。
 * 
 * [ACまでの思考の流れ]
 * ・x=B～W-1について、(H-1-A, x)のマスは必ず通る。
 *   ただしここから下に行くルートと右に行くルートがあり、
 *   右に行くと (H-1-A,x)と(H-1-A,x+1) の2マスが関係しあうのが面倒くさそう。
 * ・マスではなく「必ず1回、かつどれか1つのみ通るルート」ならどうだろうか。
 *   (H-1-A,x)->(H-1,x) は上記を満たすので、これをxごとに計算した和が答になりそう。
 */

const ll MOD = 1e9+7;
const ll NCK_SIZE = 200005;  // N<=2*10^5 など、問題文の制約に合わせる
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int main(void)
{
	ll H, W, A, B; cin >> H >> W >> A >> B;
	nCk_init();
	auto cal = [&](int sy, int sx, int gy, int gx) -> ll
	{
		int dy = gy - sy;
		int dx = gx - sx;
		return nCk(dy+dx, dx);
	};

	ll ans = 0;
	int y, x;
	y = H-1-A;
	for(x = B; x < W; x++)
	{
		ll tmp = cal(0, 0, y, x) * cal(y+1, x, H-1, W-1) % MOD;
		ans += tmp;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
