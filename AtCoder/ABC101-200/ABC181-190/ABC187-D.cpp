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

// ABC187 https://atcoder.jp/contests/abc187

// 青木派-高橋派の数をdiffとすると、初期状態ではΣAとなる。
// 高橋氏が演説をするごとにdiffが 2A+B 減っていくので、
// これが大きい方から演説していけばよい。
// diffが0未満になれば終了。

int main(void)
{
	// 演説するごとに、青木がA減らし、高橋がA+B増やす -> 2A+B変化する
	// 逆転したタイミングで終了
	int i;
	ll N; cin >> N;
	vector<ll> n;
	ll diff = 0;  // 青木派 - 高橋派
	for(i = 0; i < N; i++)
	{
		ll a, b; cin >> a >> b;
		n.push_back(a+a+b);
		diff += a;
	}
	sort(n.begin(), n.end(), greater<ll>());

	ll ans = 0;
	for(auto &e : n)
	{
		ans++;
		diff -= e;
		if(diff < 0) break;
	}
	cout << ans << endl;

	return 0;
}
