#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:48 https://atcoder.jp/contests/typical90/tasks/typical90_av	

/*
 * 満点と部分点を合わせて大きい方から貪欲でOK.
 * 最初、priority_queueを使って難しく解いてしまった。
 * 
 * 制約の A/2<B<A より、
 * 「部分点は少ないけど満点はめっちゃ高い、なのでいったん損しても部分点を取る」
 * という戦略を考える必要は無い。
 */

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<ll> sc;
	for(i = 0; i < N; i++)
	{
		ll a, b; cin >> a >> b;
		sc.push_back(a-b);
		sc.push_back(b);
	}
	sort(sc.begin(), sc.end(), greater<ll>());

	ll ans = 0;
	for(i = 0; i < K; i++)
	{
		ans += sc[i];
	}
	cout << ans << endl;

	return 0;
}
