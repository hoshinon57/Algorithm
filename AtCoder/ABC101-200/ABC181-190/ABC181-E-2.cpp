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

// ABC181 https://atcoder.jp/contests/abc181

/*
 * 前後から累積和で解く。
 * 
 * 事前にHとWはソートしておく。
 * 例えばN=7にて、先生とペアを組む人をxとすると、
 *   x=0:(1,2),(3,4),(5,6)でペアを組む
 *   x=2:(0,1),(3,4),(5,6)でペアを組む
 *   x=4:(0,1),(2,3),(5,6)でペアを組む
 *   x=6:(0,1),(2,3),(4,5)でペアを組む
 * となる。(x=1,3,5は検討不要)
 * 
 * pre_hを「先頭から2つずつペアを組んでその差分を考えたときの累積和」
 * またsuf_hを「後ろから2つずつで同様」とすると、
 * pre_hからi個、suf_hから(N/2-i)個の組み合わせを全て求めていけばよい。
 * 残りの生徒は先生とペアであり、これはlower_bound()で前後の要素をチェックする。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}
	sort(h.begin(), h.end());
	vector<ll> w(M); for(i = 0; i < M; i++) {cin >> w[i];}
	w.push_back(-INF64);  // 番兵
	w.push_back(INF64);
	sort(w.begin(), w.end());
	vector<ll> pre_h(N/2+1), suf_h(N/2+1);  // pre_h[i]:前から2個ずつペアを作ったときの差分について、累積和  suf_h:後ろから2個ずつで同様
	for(i = 0; i < N/2; i++)
	{
		pre_h[i+1] = pre_h[i] + (h[i*2+1] - h[i*2]);
		suf_h[i+1] = suf_h[i] + (h[N-i*2-1] - h[N-i*2-2]);
	}

	ll ans = INF64;
	for(i = 0; i < N/2+1; i++)
	{
		ll sum = pre_h[i] + suf_h[N/2-i];  // 累積和を前からi個、後ろから(N/2-i)個使う
		auto itr = lower_bound(w.begin(), w.end(), h[i*2]);
		ll cr = (*itr - h[i*2]);  // 先生-生徒>=0 の最小値
		itr--;
		ll cl = h[i*2] - *itr;  // 生徒-先生>=0 の最小値
		chmin(ans, sum+cr);
		chmin(ans, sum+cl);
	}
	cout << ans << endl;

	return 0;
}
