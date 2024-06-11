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

// ABC355 https://atcoder.jp/contests/abc355

// イベントソートで解く。
// Lで現在の区間数を+1, Rで-1する。
// L,Rが同じ座標の場合、Lを優先。

int main(void)
{
	int i;
	int N; cin >> N;
	vector<pair<int,int>> ev;
	for(i = 0; i < N; i++)
	{
		int l, r; cin >> l >> r;
		ev.push_back({l, 0});
		ev.push_back({r, 1});  // lの方がイベント優先
	}
	sort(ev.begin(), ev.end());

	ll ans = 0;
	ll cnt = 0;
	for(auto [pos, k] : ev)
	{
		if(k == 0)  // L
		{
			ans += cnt;
			cnt++;
		}
		else  // R
		{
			cnt--;
		}
	}
	cout << ans << endl;

	return 0;
}
