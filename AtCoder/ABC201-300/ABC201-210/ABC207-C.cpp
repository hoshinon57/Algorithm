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

// ABC207 https://atcoder.jp/contests/abc207

/*
 * N=2000なので、O(N^2)解法が可能。
 * 全i,jの組み合わせにて1つ1つ条件をチェックして間に合う。
 * 
 * 見ての通り、閉区間と開区間が混在するのが厄介。
 * 要素を10倍し、開区間なら±1して閉区間扱いに統一する。
 * すると2区間が共通部分を持つかは
 *   max(l1, l2) <= min(r1, r2)
 * で判定できる。
 * 
 * ※2つの閉区間の共通部分は、 [max(l1,l2), min(r1,r2)] と表すことができる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> l(N), r(N);
	for(i = 0; i < N; i++)
	{
		int t; cin >> t;
		cin >> l[i] >> r[i];
		l[i] *= 10;
		r[i] *= 10;
		if(t == 1) {;}
		else if(t == 2) r[i]--;
		else if(t == 3) l[i]++;
		else
		{
			l[i]++; r[i]--;
		}
	}

	int ans = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			ll l2 = max(l[i], l[j]);
			ll r2 = min(r[i], r[j]);
			if(l2 <= r2) ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
