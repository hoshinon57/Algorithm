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

// ABC330 https://atcoder.jp/contests/abc330

/*
 * x^2, y^2についてはDより大きいものは1つまで探索すれば十分。
 * まずは√Dより少し大きい程度まで、i*iの値を計算しておく。(正確には√Dの小数点以下切り上げ)
 * これをx^2としてイメージすると、各要素eについて
 *   (1)D-eの次の要素
 *   (2)D-eの手前の要素
 * がy^2の候補となる。
 * 
 * (1)(2)それぞれ |x^2+y^2-D| を計算していき、最小値が答。
 * 
 * [ACまでの思考の流れ]
 * ・Dの最大値とx^2より、√っぽい。
 * ・xは1.5*10^6程度が上限になりそう。つまりxについて全探索が可能。
 */

int main(void)
{
	ll i;
	ll D; cin >> D;
	vector<ll> p2;  // 0, 1, 4, 9, ...
	for(i = 0; i < 1500000; i++)  // 1.5*10^6 2乗すればDの最大よりも大きくなる値
	{
		p2.push_back(i*i);
	}

	ll ans = INF64;
	for(auto &e : p2)  // e=x^2のイメージ
	{
		auto itr = lower_bound(p2.begin(), p2.end(), D-e);
		ans = min(ans, abs(D-e-*itr));
		if(*itr != 0)
		{
			itr--;
			ans = min(ans, abs(D-e-*itr));
		}
		else {;}  // x^2>=Dのとき。この場合yの候補は0のみ
	}
	cout << ans << endl;

	return 0;
}
