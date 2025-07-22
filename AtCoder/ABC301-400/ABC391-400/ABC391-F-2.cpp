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

// ABC391 https://atcoder.jp/contests/abc391

/*
 * 公式解説2にあった二分探索で解いてみた版。
 *   https://atcoder.jp/contests/abc391/editorial/12086
 * 
 * 内容は上記URLの通り。
 * priority_queueを使う解説1の方針：1048ms
 * 本ソースコードの二分探索の方針：244ms
 * となり、こちらの方が早かった。なんと。
**/

int main(void)
{
	ll i, j, k;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}
	sort(a.begin(), a.end(), greater<ll>());
	sort(b.begin(), b.end(), greater<ll>());
	sort(c.begin(), c.end(), greater<ll>());

	auto cal = [&](int ii, int jj, int kk) -> ll
	{
		return a[ii]*b[jj] + b[jj]*c[kk] + c[kk]*a[ii];
	};

	// ab+bc+ca>=x がK個以上あればtrue
	auto check = [&](ll x) -> bool
	{
		ll rem = K;
		for(i = 0; i < N; i++)  // i,j,kが大きくなると、ab+bc+caは小さくなる（もしくは同じ値）
		{
			if(cal(i, 0, 0) < x) break;
			for(j = 0; j < N; j++)
			{
				if(cal(i, j, 0) < x) break;
				for(k = 0; k < N; k++)
				{
					if(cal(i, j, k) < x) break;
					if(--rem == 0) return true;
				}
			}
		}
		return false;
	};

	ll ok = 0;
	ll ng = INF64;
	while(ng - ok > 1)
	{
		ll mid = (ok+ng)/2;
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
