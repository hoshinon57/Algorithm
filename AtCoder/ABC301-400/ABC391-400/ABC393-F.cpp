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

// ABC393 https://atcoder.jp/contests/abc393

/*
 * LIS&クエリ先読み。公式解説の方針と同じ。
 *   https://atcoder.jp/contests/abc393/editorial/12252
 *   https://x.com/kyopro_friends/status/1890761194237046800
 * 
 * クエリ先読みにて以下を構築しておく。
 *   q[r]={no,x}, {no,x}, ... :  クエリR=rのときのクエリ番号とxを保持
 * 
 * 先頭から普通にLISを構築していき、位置iにおいてq[i]を参照する。
 * その時点でのLISにて、lis[*]<=x となる最大の*が答となる。
 * 
 * [ACまでの思考の流れ]
 * ・LISの気持ちを理解する。
 *   lis[i][j]: "要素のi個目まで見て、長さjであるLISの末尾の値の最小値"
 *   これをin-placeで解くことになる。
**/

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}

int main(void)
{
	// 0-indexed
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<vector<pair<ll,ll>>> q(N);  // q[R] = {クエリNo, X}, {}, {}, ...
	for(i = 0; i < Q; i++)
	{
		ll r, x; cin >> r >> x;
		r--;
		q[r].push_back({i, x});
	}

	vector<ll> lis(N, INF64);
	vector<ll> ans(Q);
	for(i = 0; i < N; i++)
	{
		ll idx = lower_bound(lis.begin(), lis.end(), a[i]) - lis.begin();  // idx番目を書き換え
		lis[idx] = a[i];
		for(auto [no,x] : q[i])
		{
			auto [dummy,num] = bi_idxnum_ika(lis, x);
			ans[no] = num;
		}
	}
	for(auto &e : ans) cout << e << endl;

	return 0;
}
