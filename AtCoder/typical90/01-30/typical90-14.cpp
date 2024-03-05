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

// 競プロ典型90問:14 https://atcoder.jp/contests/typical90/tasks/typical90_n
// ソートして貪欲法

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += abs(a[i]-b[i]);
	}
	cout << ans << endl;

	return 0;
}
