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

// ABC365 https://atcoder.jp/contests/abc365

/*
 * コンテスト時に考えた解法。
 * [典型]チェックする対象をi個目⇒i+1個目に変えるときにほとんど変化しないならば、差分に着目してO(1)で解く。
 * [典型]bit演算はbitごとに独立に考えられる。
 * 
 * 以下、0-indexedで記載し、また特定のbitについて記載する。
 * i->i+1での a[i]^a[i+1]^...^a[j] の変化点を見てみると、
 *   (1)全項目にてa[i]が除外される
 *   (2)単体のa[i+1]が除外される
 * となる。
 * 
 * すると、iのときに a[i]^a[i+1]^...^a[j] が0および1になる個数をそれぞれ cnt0,cnt1としておくと、
 * (1)はa[i]=1のときswap(cnt0,cnt1)という操作になる。
 * (2)は(1)の後、a[i+1]が0か1かでcnt*を-1する。
 * としてi+1をO(1)で計算できる。
 */

bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)

int main(void)
{
	int i, j;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = 0;
	for(int b = 0; b < 30; b++)  // bit b
	{
		ll cnt0 = 0, cnt1 = 0;
		for(i = 0; i < N-1; i++)
		{
			if(i == 0)  // 初回は愚直計算
			{
				int x = (int)isbiton(a[0], b);
				for(j = 1; j < N; j++)
				{
					x ^= (int)isbiton(a[j], b);
					if(x == 0) cnt0++;
					else cnt1++;
				}
			}
			else
			{
				// 全体についてa[i-1]を除外
				if(isbiton(a[i-1], b)) swap(cnt0, cnt1);

				// 単体のa[i]を除外
				if(isbiton(a[i], b)) cnt1--;
				else cnt0--;
			}
			ans += cnt1 * (1LL<<b);
		}
	}
	cout << ans << endl;

	return 0;
}
