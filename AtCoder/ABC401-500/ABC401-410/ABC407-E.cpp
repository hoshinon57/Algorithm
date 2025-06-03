#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC407 https://atcoder.jp/contests/abc407

/*
 * コンテスト時に解けず、各種解説を見た。貪欲法で解く。
 *   https://atcoder.jp/contests/abc407/editorial/13106
 *   https://x.com/kyopro_friends/status/1926276491555250553
 *   https://x.com/paruma184/status/1927697492352594052
 * 
 * 解説の通りで、カッコ列が合法の条件は、(を+1,)を-1として和を取っていったときに
 *   末尾まで見たときの総和が0
 *   途中の和が常に0以上
 * という典型がある。
 * これを発展(?)させると、0-indexedで
 *   0番目までに(が1以上
 *   1番目までに(が1以上
 *   2番目までに(が2以上
 *   3番目までに(が2以上
 *   4番目までに(が3以上
 *   5番目までに(が3以上
 *   ...
 * となる。
 * よって0,2,4,6,...番目まで見たときに、それまでの最大値を1つ採用すればそれが最善となる。
 * これはpriority_queueを用いて実装できる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・いやーこれどうやったら思いつけるんだ…
 *   カッコ列の典型にこういうのもあるよ、という形で覚えるのが手っ取り早いかな…
**/

void solve(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N*2); for(i = 0; i < N*2; i++) {cin >> a[i];}
	// 0-indexedで0,2,4,6,... ごと見ていく
	priority_queue<ll> que;
	ll ans = 0;
	for(i = 0; i < N*2; i++)
	{
		que.push(a[i]);
		if(i%2 == 0)  // 1つ取る
		{
			ans += que.top();
			que.pop();
		}
	}
	cout << ans << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
