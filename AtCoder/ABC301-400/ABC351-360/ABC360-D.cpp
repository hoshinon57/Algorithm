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

// ABC360 https://atcoder.jp/contests/abc360

/*
 * [ACまでの思考の流れ]
 * ・すれ違うのは「正の向きのアリ」「負の向きのアリ」の組み合わせのみ。
 *   よって正の向きのアリを1匹ごとにカウントして総和を求める方針を考えた。
 * ・正の向きのアリについて、その初期位置をeとしたときに
 *     e <= x <= e+2T
 *   を満たすxが初期位置となる負の向きのアリとすれ違う。
 *   これを二分探索で求めればよい。
 */

int main(void)
{
	int i;
	ll N, T; cin >> N >> T;
	string s; cin >> s;
	vector<ll> al, ar;  // 左向き、右向きの初期位置
	for(i = 0; i < N; i++)
	{
		ll x; cin >> x;
		if(s[i] == '0') al.push_back(x);
		else ar.push_back(x);
	}
	sort(al.begin(), al.end());
	sort(ar.begin(), ar.end());

	ll ans = 0;
	for(auto &e : ar)
	{
		// 初期位置をeとしたときに、 e以上e+2T以下 となる負のアリの数
		ll tmp1 = upper_bound(al.begin(), al.end(), e+2*T) - al.begin();
		ll tmp2 = lower_bound(al.begin(), al.end(), e) - al.begin();
		ans += (tmp1-tmp2);
	}
	cout << ans << endl;

	return 0;
}
