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

// ABC186 https://atcoder.jp/contests/abc186

/*
 * 自力で解けず、解説を見た。
 *   https://drken1215.hatenablog.com/entry/2020/12/20/015100
 *   https://twitter.com/kyopro_friends/status/1340294053586378757
 * 
 * S+xK = yN となるので(x,yは整数)、
 * S+xK ≡ 0 (mod N)
 * xK ≡ N-S (mod N)
 * となるxが存在すればよい。
 * 両辺にKの逆元をかければよいが、逆元の存在する条件はKとNが互いに素であること、となる。
 * 互いに素ではなくとも、gcd(N, S, K)でN,S,Kそれぞれを割ってあげることで、K'とN'が互いに素になればOK.
 * K'とN'が互いに素でないなら、解なし。
 * 
 * 後は以下を解く。
 * xK' ≡ N'-S' (mod N')
 * x ≡ (N'-S')K'^(-1) (mod N')
 */

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

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

void solve(void)
{
	ll N, S, K; cin >> N >> S >> K;
	ll g = gcd(gcd(N, S), K);
	N /= g;
	S /= g;
	K /= g;
	if(gcd(N, K) != 1)
	{
		cout << -1 << endl;
		return;
	}
	cout << (N-S) * modinv(K, N) % N << endl;
}

int main(void)
{
	int t; cin >> t;
	while(t > 0) { solve(); t--; }

	return 0;
}
