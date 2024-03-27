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

// ABC102 https://atcoder.jp/contests/abc102

/*
 * Aとb+iで考えるのではなく、A-iとbで考える。
 * すると求めたい値は
 *   |(A1-1)-b| + |(A2-2)-b| + |(A3-3)-b| + ... + |(AN-N)-b|
 * となる。
 * この場合、(Ai-i)の中央値をbにするのが最適。
 * 類題：典型90-70(Plant Planning) https://atcoder.jp/contests/typical90/tasks/typical90_br
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a[i] -= (i+1);  // ai-iに置き換え
	}
	sort(a.begin(), a.end());
	ll ans = 0;
	for(auto &e : a)
	{
		ans += abs(e-a[N/2]);  // 中央値との差
	}
	cout << ans << endl;

	return 0;
}
