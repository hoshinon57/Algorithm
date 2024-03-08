#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC343 https://atcoder.jp/contests/abc343

// 「i番目の人のスコア」と、「スコアがiである人数」をそれぞれ管理する。
// 後者の要素数を出力すれば良い。

int main(void)
{
	ll i;
	ll N, T; cin >> N >> T;
	vector<ll> sc(N);  // sc[i]:i番目の人のスコア
	map<ll, int> cnt;  // cnt[i]:スコアiの人数
	cnt[0] = N;  // N人全て0点
	for(i = 0; i < T; i++)
	{
		ll a, b; cin >> a >> b;
		a--;
		if(--cnt[sc[a]] == 0)  // 変動前のスコアを持つ人数が0人になれば、要素を削除
		{
			cnt.erase(sc[a]);
		}
		sc[a] += b;
		cnt[sc[a]]++;
		cout << cnt.size() << endl;
	}

	return 0;
}
