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

// ABC292 https://atcoder.jp/contests/abc292

/*
 * 解説を元に、調和級数を用いたO(NlogN)で解く。
 *   https://atcoder.jp/contests/abc292/editorial/5872
 *   https://drken1215.hatenablog.com/entry/2023/05/16/230900
 * 
 * ※最初はAB,CDごとに約数列挙するO(N√N)で解いた。
 *   https://atcoder.jp/contests/abc292/submissions/39684767
 * 
 * まず、num[i]:iの約数の個数として、num[1]～num[N]まで求める。
 * これは調和級数の考え方でO(NlogN)で求められる。実装を参照。
 * 
 * 後は、i=ABとしてiについて全探索し、
 *   num[i]*num[N-i]
 * の総和が答。
 */

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<ll> num(N+1);  // num[i]:iの約数の個数
	for(i = 1; i <= N; i++)
	{
		for(j = i; j <= N; j+=i) num[j]++;
	}

	ll ans = 0;
	for(i = 1; i < N; i++)  // i=AB
	{
		j = N-i;  // j=CD
		ans += num[i] * num[j];
	}
	cout << ans << endl;

	return 0;
}
