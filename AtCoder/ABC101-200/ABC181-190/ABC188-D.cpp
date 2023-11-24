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

// ABC188 https://atcoder.jp/contests/abc188

/*
 * イベントソートで解く。公式解説と同じ考え方。
 *   https://atcoder.jp/contests/abc188/editorial/346
 * 日程が最大10^9のため、(座標圧縮などしないと)いもす法は使えない。
 * 
 * 入力a,b,cに対して、
 *   a日目にコストが+c
 *   b+1日目にコストが-c
 * となるようなイベントを考える。
 * 
 * これを日でソートして、小さい方の要素から順に処理していく。
 * 要素を順に見ていき、
 *   日が前回から変化したら、前回の日からの期間ぶんコストを足す
 *   コストを増減する
 * を繰り返していく。
 * 
 * 最後はコスト=0で終わることから、
 * 末尾に番兵を入れて個別に判定…といったのは不要。
 */

int main(void)
{
	int i;
	ll N, C; cin >> N >> C;
	vector<pair<ll,ll>> ev;
	for(i = 0; i < N; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		// [a,b+1) の半開区間で考える
		ev.push_back({a, c});
		ev.push_back({b+1, -c});
	}
	sort(ev.begin(), ev.end());

	ll ans = 0;
	ll nowCost = 0, nowDay = 0;
	for(auto &[d,c] : ev)
	{
		// 日が変化したら、直前の日からの期間ぶんコストを足す
		if(d != nowDay)
		{
			ans += (d-nowDay) * min(nowCost,C);
			nowDay = d;
		}
		nowCost += c;
	}
	cout << ans << endl;

	return 0;
}
