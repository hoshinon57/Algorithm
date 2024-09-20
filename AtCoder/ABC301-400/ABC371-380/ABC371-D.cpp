#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cassert>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC371 https://atcoder.jp/contests/abc371

// Pの累積和を求めておけば、
// (R以下の要素の累積和) - (L未満の要素の累積和)
// で求められる。
// ※コンテスト時、実装に手間取ったのでライブラリを作ってみた。

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数}
template<typename T> pair<int,int> bi_idxnum_miman(vector<T> &a, T val) {int idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {val以下の最右のidx, val以下の要素数}
template<typename T> pair<int,int> bi_idxnum_ika(vector<T> &a, T val)   {int idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {valより大きい最左のidx, valより大きい要素数}
template<typename T> pair<int,int> bi_idxnum_koeru(vector<T> &a, T val) {int idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(int)a.size()-idx};}
// {val以上の最左のidx, val以上の要素数}
template<typename T> pair<int,int> bi_idxnum_ijou(vector<T> &a, T val)  {int idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(int)a.size()-idx};}

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> x(N); for(i = 0; i < N; i++) {cin >> x[i];}
	vector<ll> p(N); for(i = 0; i < N; i++) {cin >> p[i];}
	vector<ll> p_sum(p.size()+1); for(int kk = 0; kk < (int)p.size(); kk++) {p_sum[kk+1] = p_sum[kk] + p[kk];}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll l, r; cin >> l >> r;
		ll ans = 0;
		// xの先頭n個の累積和 -> x_sum[n]
		auto [idx1,n1] = bi_idxnum_ika(x,r);
		ans += p_sum[n1];
		auto [idx2,n2] = bi_idxnum_miman(x,l);
		ans -= p_sum[n2];
		cout << ans << endl;
	}

	return 0;
}
