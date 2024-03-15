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

// ABC125 https://atcoder.jp/contests/abc125

/*
 * ユーザー解説を元に実装した。
 *   https://drken1215.hatenablog.com/entry/2019/04/27/224100_1
 * 
 * 整数Aiを1つ書き換える処理は、
 *   Ai以外のN-1個の要素のGCDを求めて、
 *   そのGCD値をAiに設定する
 * と読み替えることができる。
 * これをAの各要素について計算し、その最大値が答となる。
 * 
 * 要素のある1点について着目したい場合、左右から累積和という考え方ができる。
 * 今回は"累積GCD"として、
 *   要素を左から見ていったときの累積GCD
 *   要素を右から見ていったときの累積GCD
 * を求めておく。
 * そして各iについて、a[i]の前からi個、後ろから(N-i-1)個の累積GCDを合わせていくと、その最大値が答となる。
 */

long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> a_gcd(N+1), a_gcd_pr(N+1);
	for(i = 0; i < N; i++)
	{
		a_gcd[i+1] = gcd(a_gcd[i], a[i]);
	}
	reverse(a.begin(), a.end());
	for(i = 0; i < N; i++)
	{
		a_gcd_pr[i+1] = gcd(a_gcd_pr[i], a[i]);
	}

	ll ans = 1;
	for(i = 0; i < N; i++)  // a[i]の前からi個, 後ろから(N-i-1)個, 計N-1個のGCD
	{
		chmax(ans, gcd(a_gcd[i], a_gcd_pr[N-i-1]));
	}
	cout << ans << endl;

	return 0;
}
