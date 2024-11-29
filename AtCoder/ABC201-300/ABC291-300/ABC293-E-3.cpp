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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint;

// ABC293 https://atcoder.jp/contests/abc293

// 以下を参考に再帰で解いた版。
//   https://x.com/kyopro_friends/status/1634551534473322498
//   https://x.com/ks4m/status/1634558616031367169
// 
// X=nのときの求める値をSnとすると、
//   nが奇数：Sn = S(n-1) + A^(n-1)
//   nが偶数：Sn = S(n/2) * (A^(n/2) + 1)
// となる。これをmodintを用いながら解く。
// 参考URLではメモ化再帰と書いてあるが、メモしなくて十分に間に合う。
// 再帰にてnの値は減るのみで、同じ値にて再帰関数が呼ばれることは無いため。

int main(void)
{
	ll A, X, M; cin >> A >> X >> M;
	modint::set_mod(M);

	auto r = [&](auto &self, ll n) -> mint
	{
		mint ans;
		mint a = A;
		if(n == 1) return 1;
		if(n%2 == 1)
		{
			ans = self(self, n-1) + a.pow(n-1);
		}
		else
		{
			ans = self(self, n/2) * (a.pow(n/2) + 1);
		}
		return ans;
	};
	mint ans = r(r, X);
	cout << ans.val() << endl;

	return 0;
}
