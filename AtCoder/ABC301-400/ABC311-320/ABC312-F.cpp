#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC312 https://atcoder.jp/contests/abc312

/*
 * 各種解説を元に、累積和の考え方で解いた。
 *   https://twitter.com/kyopro_friends/status/1685286939870797824
 *   https://atcoder.jp/contests/abc312/editorial/6853
 * ※ちなみに自力AC時はもっと泥臭いコードだった…
 *   https://atcoder.jp/contests/abc312/submissions/48936517
 * 
 * T=0の品物だけを考えると、満足度が大きい順に取っていくのが最適。
 * またT=1,2の品物だけを考えると、
 *   缶切りが0個のときは、T=2のうち最もXが大きいものを取り、
 *   缶切りが1個以上のときは、T=1のうち満足度が大きい順に取っていく
 * のが最適になる。
 * 
 * よって
 *   a_sum[i]:T=0の品物にi回使うときの、満足度の総和
 *   bc_sum[i]:T=1,2の品物にi回使うときの、満足度の総和
 * として定義すると、これは累積和の考え方で求められて、
 * i=0～Mの各iについて a_sum[i]+bc_sum[M-i] の最大が答となる。
 */

int main(void)
{
	int i;
	ll N, M; cin >> N >> M;
	vector<ll> a, b, c;
	for(i = 0; i < N; i++)
	{
		ll t, x; cin >> t >> x;
		if(t == 0) a.push_back(x);
		else if(t == 1) b.push_back(x);
		else c.push_back(x);
	}
	sort(a.begin(), a.end(), greater<ll>());
	sort(b.begin(), b.end());
	sort(c.begin(), c.end());

	vector<ll> a_sum(M+1);
	for(i = 0; i < M; i++)
	{
		if(i >= (int)a.size()) a_sum[i+1] = a_sum[i];  // T=0の品物が残っていない
		else a_sum[i+1] = a_sum[i] + a[i];
	}
	int ope = 0;  // 缶切りの数
	vector<ll> bc_sum(M+1);
	for(i = 0; i < M; i++)
	{
		if(ope == 0)  // 缶切りが無いのでT=2から1つ取る
		{
			if(!c.empty())
			{
				ope += c.back();
				c.pop_back();
			}
			bc_sum[i+1] = bc_sum[i];  // 満足度は変わらない
		}
		else  // 缶切りがあるならばT=1から取る
		{
			if(!b.empty())
			{
				bc_sum[i+1] = bc_sum[i] + b.back();
				b.pop_back();
				ope--;
			}
			else
			{
				bc_sum[i+1] = bc_sum[i];  // もうT=1は残ってないので、満足度は変わらない
			}
		}
	}

	ll ans = 0;
	for(i = 0; i <= M; i++)
	{
		chmax(ans, a_sum[i] + bc_sum[M-i]);
	}
	cout << ans << endl;

	return 0;
}
