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

// ABC062 https://atcoder.jp/contests/abc062

/*
 * 左右から累積和、という典型がある。それに近い。
 *   https://drken1215.hatenablog.com/entry/2019/04/28/114800
 * 
 * ある要素間で区切ることを考える。
 * このときの最善スコアは、(区切りの左側で大きい方からN個)-(右側で小さい方からN個)となる。
 * これを区切りについて全探索する。
 * 
 * 大きい方/小さい方からN個には優先度付きキューを用いた。
 * 実装ではtl[i]を「左からa[i]までの要素のうち、大きい方からN個の総和」としている。
 * tr[i]も同様。
 * 後は実装を参照。
**/

int main(void)
{
	ll i;
	ll N; cin >> N;
	ll N3 = N*3;
	vector<ll> a(N3); for(i = 0; i < N3; i++) {cin >> a[i];}

	// 左側
	priority_queue<ll, vector<ll>, greater<ll>> que1;  // 最小値取り出し
	ll now = 0;
	vector<ll> tl(N*3, -INF64), tr(N*3, INF64);
	for(i = 0; i < N*2; i++)
	{
		if(i < N)
		{
			que1.push(a[i]);
			now += a[i];
			if((int)que1.size() == N) tl[i] = now;  // N個たまった
		}
		else
		{
			if(que1.top() < a[i])
			{
				now -= que1.top();
				now += a[i];
				que1.pop();
				que1.push(a[i]);
			}
			tl[i] = now;
		}
	}

	// 右側
	priority_queue<ll> que2;  // 最大値取り出し
	now = 0;
	for(i = N3-1; i >= N; i--)
	{
		if(i >= N*2)
		{
			que2.push(a[i]);
			now += a[i];
			if((int)que2.size() == N) tr[i] = now;
		}
		else
		{
			if(que2.top() > a[i])
			{
				now -= que2.top();
				now += a[i];
				que2.pop();
				que2.push(a[i]);
			}
			tr[i] = now;
		}
	}

	ll ans = -INF64;
	for(i = 0; i < N3-1; i++)  // 範囲外はINFや-INFにしているので、この範囲でよい
	{
		chmax(ans, tl[i] - tr[i+1]);  // 左側をa[i]までにすると、右側はa[i+1]から
	}
	cout << ans << endl;

	return 0;
}
