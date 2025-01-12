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

// ABC388 https://atcoder.jp/contests/abc388

/*
 * ABC388-E.cppのコメントを元に、決め打ち二分探索で解いた版。
 * 公式解説の方針。
 *   https://atcoder.jp/contests/abc388/editorial/11901
 * 
 * 「鏡餅をk個作れるか？」の判定問題を考える。
 * k個作れないなら(k+1)個も作れないし、k個作れるなら(k-1)個も作れるので、二分探索で求められる。
 * 判定問題は、先頭k個と末尾k個でマッチングできるかで判定できる。
 * 
 * 例えばN=8で、餅番号を 0 1 2 3 4 5 6 7 とする。
 * k=3のとき、0-5,1-6,2-7でマッチングさせるのが最善。
 * 例えば0-4,1-6,2-7でマッチングできたとき、0-4を0-5にしても損しない。
 * また0-6,1-5,2-7でマッチングで来たとき、餅5,6を交換しても損しない。
 * といった考え方…と思う。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	// x個の鏡餅を作れるならtrue (x個のマッチング問題)
	auto check = [&](ll x) -> bool
	{
		ll li = 0;
		ll ri = N-x;
		while(ri < N)
		{
			if(a[li] <= a[ri]/2) {;}
			else return false;
			li++; ri++;
		}
		return true;  // 最後までマッチングできた
	};

	ll ok = 0;
	ll ng = (N/2)+1;
	while(ng - ok > 1)
	{
		ll mid = (ok+ng)/2;
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
