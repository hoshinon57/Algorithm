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

// ABC032 https://atcoder.jp/contests/abc032

/*
 * 3種のナップザック問題を解くやつ。
 * w<=1000とv<=1000は割愛。それぞれsolve2,solve3にて。
 * 
 * N<=30は半分全列挙で解く。
 * まず、要素を半分に分けてそれぞれbit全探索し、{重さw,価値v}で登録しておく。
 * 後半についてはソートしつつ、{重さw,重さw以下での価値の最大v}に変換する。
 * 
 * 前半の各要素{w1,v1}について、重さが(W-w1)以下で価値が最大のものを後半部分から探す。
 * これを{w2,v2}とすると、v1+v2の最大値が答。
 * 探し方は二分探索にて。先ほど {重さw,重さw以下での価値の最大v} に変換したので、二分探索で上手くいく。
 * 
 * 半分全列挙について、AC後にWEB検索して似た記事は以下。(特に実装面)
 *   公式解説
 *   http://ch3000.blog.fc2.com/blog-entry-40.html
 *   https://mayokoex.hatenablog.com/entry/2016/01/09/230000
 *   https://perogram.hateblo.jp/entry/2020/12/31/080304
 *   https://kmjp.hatenablog.jp/entry/2016/01/10/0900 これは二分探索ではなく尺取り法
**/

// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}

ll N, W;
vector<ll> v, w;

// N<=30
void solve1(void)
{
	ll i, j;
	ll N1 = N/2;  // [0,N1) N1個
	ll N2 = N-N1;  // [N1,N) N2個
	using pll = pair<ll,ll>;  // {w,v}
	vector<pll> d1;
	for(i = 0; i < (1<<N1); i++)
	{
		ll sw = 0, sv = 0;
		for(j = 0; j < N1; j++)
		{
			if( ((i>>j)&1) == 0) continue;
			sw += w[j];
			sv += v[j];
		}
		d1.push_back({sw, sv});
	}
	vector<pll> d2;
	for(i = 0; i < (1<<N2); i++)
	{
		ll sw = 0, sv = 0;
		for(j = 0; j < N2; j++)
		{
			if( ((i>>j)&1) == 0) continue;
			sw += w[j+N1];
			sv += v[j+N1];
		}
		d2.push_back({sw, sv});
	}
	sort(d2.begin(), d2.end());

	// d2を{重さw, 重さw以下での価値最大v}に変換
	for(i = 1; i < (int)d2.size(); i++)
	{
		chmax(d2[i].second, d2[i-1].second);
	}

	ll ans = 0;
	for(auto [w1,v1] : d1)
	{
		auto [idx,num] = bi_idxnum_ika(d2, make_pair(W-w1, INF64));
		if(idx == -1) continue;
		ll v2 = d2[idx].second;
		chmax(ans, v1+v2);
	}
	cout << ans << endl;
}

// w<=1000
void solve2(void)
{
	// dp[i][j] i個目まで見て、総重量jのときの価値最大  j:0～W
	ll i, j;
	vector<ll> dp(W+1, -INF64);
	dp[0] = 0;
	for(i = 0; i < N; i++)
	{
		vector<ll> ndp(W+1);
		ndp = dp;
		for(j = 0; j <= W; j++)
		{
			if(j-w[i] >= 0) chmax(ndp[j], dp[j-w[i]]+v[i]);
		}
		swap(dp, ndp);
	}
	ll ans = *max_element(dp.begin(), dp.end());
	cout << ans << endl;
}

// v<=1000
void solve3(void)
{
	// dp[i][j] i個目まで見て、総価値jのときの重量min j:200000
	ll i, j;
	const ll VMX = 200000;
	vector<ll> dp(VMX+1, INF64);
	dp[0] = 0;
	for(i = 0; i < N; i++)
	{
		vector<ll> ndp(VMX+1);
		ndp = dp;
		for(j = 0; j <= VMX; j++)
		{
			if(j-v[i] >= 0) chmin(ndp[j], dp[j-v[i]]+w[i]);
		}
		swap(dp, ndp);
	}
	ll ans = 0;
	for(j = 0; j <= VMX; j++)
	{
		if(dp[j] <= W) ans = j;
	}
	cout << ans << endl;
}

int main(void)
{
	ll i;
	cin >> N >> W;
	v.resize(N);
	w.resize(N);
	for(i = 0; i < N; i++) {cin >> v[i] >> w[i];}

	if(N <= 30) solve1();
	else if(*max_element(w.begin(), w.end()) <= 1000) solve2();
	else solve3();

	return 0;
}
