#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC318 https://atcoder.jp/contests/abc318

/*
 * コンテスト中に解けず、解説を見た。
 * climpet氏の解説を元に解く。
 *   https://atcoder.jp/contests/abc318/editorial/7094
 * 
 * 基本は上記解説のままだが、自身の整理として。
 * a[]={1, x, 1, x, x, 1} のようなケースでa[5]=1の部分を考えると、
 *    (5-0-1) + (5-2-1) - 重複分1((i,j,k)=(0,2,5)のケース)
 *  = (5-1)*2 - (0+2) - 1(重複分)
 *  = (i-1)*(これまでのa[x]=a[i]となる要素数) - (これまでのa[x]=a[i]となるxの総和) - 1(重複分)
 * となる。
 * 以下実装にて、
 *   これまでのa[x]=a[i]となる要素数：cnt[]
 *   これまでのa[x]=a[i]となるxの総和：sum[]
 * にて簡易に表せることが分かる。
 * 
 * 重複分については、例えばその時点でcnt[]=5について考えてみると、
 *   (5-1) + (4-1) + (3-1) + (2-1) + (1-1)
 * = Σ(cnt[]-1)
 * = (cnt[]-1)*cnt/2
 * となることが分かる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<ll> cnt(N+1);  // cnt[x]:a[i]=xとなる要素数
	vector<ll> sum(N+1);  // sum[x]:a[i]=xとなる要素について、iの総和
	ll ans = 0;
	for(i = 0; i < N; i++)  // a[i]
	{
		ll &c = cnt[a[i]];
		ll &s = sum[a[i]];
		ans += (i-1) * c - s - c*(c-1)/2;
		c++;
		s += i;
	}
	cout << ans << endl;

	return 0;
}
