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

// ABC415 https://atcoder.jp/contests/abc415

/*
 * コンテスト時に方針は合ったが、実装で大ポカしACできず。マジ無能。
 * 二分探索とDPで解く。
 * 
 * 以下、0-indexedで記載する。
 * まず公式解説にもある通り、「あるマスに何日目に到達する」かは一意に決まる。
 * マス(y,x)は(y+x-1)日目に到達するので、
 * 事前に a[y][x] += p[y+x-1] としてしまってよい。
 * 
 * 最初にx枚持つことで目的を達成できるか、を考える。二分探索する。
 * x=0でもOKの可能性があるので、初期値はng=-1.
 * 
 * 以下のdp値を考える。
 *   dp[y][x]:
 *     マス(y,x)に来た日にまだ倒れないときの、最大のコイン枚数
 *     ただし生きてそのマスを過ごせない場合、DP値は負(-INF)とする。
 * 上左マスからもらうDPをするが、遷移元が負であれば遷移させない。
 * dp値が0以上のマスだけを通ってゴールに到達できれば成功、のイメージ。
 * dp[H-1][W-1]が非負なら成功。
 * 
 * [ACまでの思考の流れ]
 * ・コンテスト時、二分探索はすぐに見えた。
 *   ただ配るDPの部分をBFSっぽく「配り先のマスの情報をキューに積む」という実装をやってしまった。なぜそっちを考えてしまったのか…
 *   複数の方向から同じマスに対してキューに積まれるので、WAではないがTLEする。
 *   これで最後までACできず。マジ無能すぎる。
 * ・同じ頂点やグリッドのマスに対して、2度以上キューに積むな。TLEする。
 *   (もちろん、ダイクストラのpriority_queueや0-1BFSのdequeは例外。通常のqueueの話)
**/

int main(void)
{
	ll i;
	ll y, x;
	ll H, W; cin >> H >> W;
	vector<vector<ll>> a(H, vector<ll>(W, 0));  // a[H][W]
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++) cin >> a[y][x];
	}
	vector<ll> p(H+W-1); for(i = 0; i < H+W-1; i++) {cin >> p[i];}
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			a[y][x] -= p[y+x];
		}
	}

	vector<vector<ll>> dp(H, vector<ll>(W, -INF64));
	auto check = [&](ll co) -> bool
	{
		// 毎度定義するのではなく値を再設定にすることで、 671ms->206ms
		for(y = 0; y < H; y++)
		{
			for(x = 0; x < W; x++) dp[y][x] = -INF64;
		}
		dp[0][0] = a[0][0] + co;

		// もらうDP 遷移元が負ならもらわない
		for(y = 0; y < H; y++)
		{
			for(x = 0; x < W; x++)  // dp[y][x]
			{
				if(y == 0 && x == 0) continue;
				ll &d = dp[y][x];
				// 上から
				if(y-1 >= 0 && dp[y-1][x] >= 0)
				{
					chmax(d, dp[y-1][x]);
				}
				// 左から
				if(x-1 >= 0 && dp[y][x-1] >= 0)
				{
					chmax(d, dp[y][x-1]);
				}
				d += a[y][x];
				if(d < 0) d = -INF64;  // ここに遷移できない 念のため-INFにしておく
			}
		}
		return dp[H-1][W-1] >= 0;
	};

	ll ng = -1;
	ll ok = INF64;
	while(ok - ng > 1)
	{
		ll mid = (ok+ng)/2;
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
