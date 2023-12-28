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

// ABC143 https://atcoder.jp/contests/abc143

/*
 * kyopro_friends氏の解説と同じ実装。
 *   https://twitter.com/kyopro_friends/status/1185552412158726144
 * 
 * Lをソートしておき、Li,Lj,Lkをi<j<kとなるように取るようにする。
 * i,jについて全探索し、残ったkを求めることを考える。
 * 不等式は Lk<Li+Lj のみを考えればよくなるので、これは二分探索で求めることができる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> l(N); for(i = 0; i < N; i++) {cin >> l[i];}
	sort(l.begin(), l.end());
	// Ai<Aj<Ak
	int ans = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N-1; j++)
		{
			// l[i]+l[j]よりも小さい要素を求める -> lower_bound()の1つ手前になる
			// kはj+1以上なので、jを引く
			int n = lower_bound(l.begin(), l.end(), l[i]+l[j]) - l.begin() - 1;
			ans += n-j;
		}
	}
	cout << ans << endl;

	return 0;
}
