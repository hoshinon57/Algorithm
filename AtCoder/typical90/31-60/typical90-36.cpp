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

// 競プロ典型90問:36 https://atcoder.jp/contests/typical90/tasks/typical90_aj

/*
 * 試しに1次元で考えてみると、距離の最大値は
 *   最も左にある点との距離
 *   最も右にある点との距離
 * のどちらかとなる。
 * つまり、距離最大となりうる点は限定されている。
 * これを2次元でも同じように考える。
 * 
 * マンハッタン距離は45度回転という典型から、
 * 各点について x+y, x-y の2つの値をそれぞれ求めておく。
 * するとそれぞれの最大値/最小値との差分、計4つの中に必ず答が含まれる。
 * 
 * [ACまでの思考の流れ]
 * ・マンハッタン距離は45度回転。[典型]
 * ・距離最大となりうる点は限定されている。
 */

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	vector<ll> x(N), y(N);
	vector<ll> xy1, xy2;  // xy1:x+y, xy2:x-y
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
		xy1.push_back(x[i]+y[i]);
		xy2.push_back(x[i]-y[i]);
	}
	sort(xy1.begin(), xy1.end());
	sort(xy2.begin(), xy2.end());
	
	while(Q > 0)
	{
		Q--;
		int q; cin >> q;
		q--;
		ll xy11 = x[q]+y[q];
		ll xy22 = x[q]-y[q];
		ll ans = max({abs(xy1.front() - xy11), abs(xy1.back() - xy11), abs(xy2.front() - xy22), abs(xy2.back() - xy22)});
		cout << ans << endl;
	}

	return 0;
}
