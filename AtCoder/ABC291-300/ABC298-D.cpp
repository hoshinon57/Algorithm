#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC298 https://atcoder.jp/contests/abc298

/*
 * 数列をキューで管理していく。
 * 各クエリごとに「最上位の桁は10の何乗か」を更新していき、
 * 削除クエリにて、 (キューの先頭の値)*10^p を減算する。
 */

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

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
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
	ll Q;
	cin >> Q;
	ll p = 0;  // 最上位の桁が10の何乗か
	ll answer = 1;
	queue<ll> que;
	que.push(1);

	while(Q > 0)
	{
		int query;
		cin >> query;
		if(query == 1)
		{
			ll x;
			cin >> x;
			que.push(x);

			answer = (answer*10)%MOD + x;
			answer %= MOD;
			p++;
		}
		else if(query == 2)
		{
			ll x = que.front();
			que.pop();

			answer = answer + MOD - (x*modpow(10, p, MOD))%MOD;  // 先頭の桁を減算 x*10^p
			answer %= MOD;
			p--;
		}
		else
		{
			cout << answer << endl;
		}
		Q--;
	}

	return 0;
}
