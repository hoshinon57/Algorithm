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

// ABC399 https://atcoder.jp/contests/abc399

/*
 * 数値の登場位置を記録しておく。
 * 問題文におけるa<bのaについて、その左側について全探索する。
 *   aがA内で隣り合っていないこと、
 *   aの左側の要素bが、同じく隣り合っていないこと、
 *   それぞれもう片方の登場位置が隣り合っていること、
 * を満たせばOK.
 * aの右側も同様。
 * 
 * 各種解説では、
 *   要素a[i]とa[i+1]について考える
 *   答の組をsetで管理することで重複を省く
 * といった実装だった。なるほど賢い…
**/

void solve(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N*2); for(i = 0; i < N*2; i++) {cin >> a[i]; a[i]--;}
	vector<vector<ll>> idx(N);  // idx[N][2]
	for(i = 0; i < N*2; i++)
	{
		idx[a[i]].push_back(i);
	}

	// 数値idがA内で隣同士ならtrue
	auto tonari = [&](int id) -> bool
	{
		return (idx[id][1]-idx[id][0] == 1);
	};

	ll ans = 0;
	for(i = 0; i < N*2; i++)  // x=a[i]
	{
		ll x, y;  // 問題文におけるa,b
		x = a[i];
		if(i != idx[x][0]) continue;  // 登場した値が先頭の方であること
		if(tonari(x)) continue;

		// xの左側ないし右側の要素について判定
		auto f = [&](ll ypos) -> void
		{
			y = a[ypos];
			// x<yであること、またy同士で隣接していないこと
			if(x > y) return;
			if(tonari(y)) return;

			auto yidx_ano = ((ypos != idx[y][0]) ? idx[y][0] : idx[y][1]);  // xの隣にあるyで、もう片方のyの位置
			if(abs(yidx_ano - idx[x][1]) == 1) ans++;  // もう片方のyともう片方のxが隣同士ならOK
		};
		if(i != 0)  // xの左側
		{
			f(i-1);
		}
		if(i != 2*N-1)  // xの右側
		{
			f(i+1);
		}
	}
	cout << ans << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
