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

// ABC412 https://atcoder.jp/contests/abc412

/*
 * コンテスト時、読解ミスとかもありめちゃくちゃハマった… 何やってんだ…
 * 
 * ドミノ1から始めて、
 * ・ドミノNが倒せる状況なら倒して即終了 (最小個数を求めるので)
 * ・そうでなければ、倒せるドミノのうち最大のものを採用
 *   倒せるものが無ければ、ないし倒せるとしても今より大きさが大きくならなければ、ドミノNには到達しない
 * の方針で実装する。
 * 
 * ドミノ1,Nは専用の変数に、
 * ドミノ2～(N-1)は配列で、昇順ソートしておく、
 * と実装が楽かなと。
**/

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}

void solve(void)
{
	int i;
	int N; cin >> N;
	ll s0; cin >> s0;
	vector<ll> s(N-2);
	for(i = 0; i < N-2; i++) cin >> s[i];
	ll se; cin >> se;
	sort(s.begin(), s.end());

	ll prev = s0;
	ll ans = 2;  // ドミノ1,Nのぶん
	while(true)
	{
		if(prev*2 >= se) break;  // ドミノNを倒せる

		// 大きさprevが倒せるドミノの最大を求める
		auto[idx,num] = bi_idxnum_ika(s, prev*2);
		if(idx == -1 || s[idx] <= prev)  // prevが倒せるドミノが無い、または倒せてもそれは大きくならない
		{
			ans = -1;
			break;
		}
		prev = s[idx];
		ans++;
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
