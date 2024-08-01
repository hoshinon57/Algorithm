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

// ABC359 https://atcoder.jp/contests/abc359

/*
 * 自力で解けず、解説を見た。貪欲法で解く。
 *   https://atcoder.jp/contests/abc359/editorial/10260
 *   https://x.com/kyopro_friends/status/1804518188761440393
 * 
 * N頂点の木の次数の総和は2N-2となる。
 * これを「各頂点の次数を1以上にする」という前提で振り分けると、どのように振り分けても木が構築できるっぽい。
 * 
 * ある頂点について次数を増やしていくと、そのコストは
 *   1*Ai, 4*Ai, 9*Ai, 16*Ai, ...
 * と単調増加する。
 * つまり「後に小さいコストがあるから、今回だけは大きいコストを採用してもトータルでは得」
 * といったことは無く、貪欲に現在の最小コストを採用していけばよい。
 * これを優先度付きキューにて実装する。
 * 
 * d[i]を現在の頂点iの次数として、初期値1の状態からスタートする。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・各頂点の次数をどう振り分けても木が作れそう、というところまでは思いついた。
 * ・頂点の次数がどれも1の状態から始めて構築していく、というのを思いつけるようにする。
 *   dを増やしたときにコストがどう変化するかを考える。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> d(N, 1);  // d[i]:現在の頂点iの次数

	using pll = pair<ll,ll>;
	ll ans = 0;
	priority_queue<pll, vector<pll>, greater<pll>> que;
	for(i = 0; i < N; i++)  // 最初にN個積む
	{
		que.push({(4-1)*a[i], i});  // (2^2-1^2)*Ai
		ans += a[i];
	}

	for(i = 0; i < N-2; i++)  // 次数の総和は(2N-2)であり、既にN個取ってるので残りは(N-2).
	{
		auto [dd,idx] = que.top();
		que.pop();
		d[idx]++;
		ll nxt = ((d[idx]+1)*(d[idx]+1)-d[idx]*d[idx]) * a[idx];  // 次数を1つ増やすにあたり増えるコスト
		que.push({nxt, idx});
		ans += dd;
	}
	cout << ans << endl;

	return 0;
}
