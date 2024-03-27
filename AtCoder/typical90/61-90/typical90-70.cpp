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

// 競プロ典型90問:70 https://atcoder.jp/contests/typical90/tasks/typical90_br

/*
 * X,Y独立に計算する。ほぼ公式解説と同じ実装だった。
 *   https://github.com/E869120/kyopro_educational_90/blob/main/sol/070.cpp
 * 
 * マンハッタン距離の総和のため、X,Yそれぞれで考えて良い。
 * またX方向について、N個の要素との距離の総和の最小値は、要素の中央値を基点にした場合となる。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++) cin >> x[i] >> y[i];
	sort(x.begin(), x.end());  // X,Y独立なので、それぞれソートしてしまって良い
	sort(y.begin(), y.end());
	ll px = x[N/2], py = y[N/2];
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += abs(px-x[i]) + abs(py-y[i]);
	}
	cout << ans << endl;

	return 0;
}
