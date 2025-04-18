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

// ABC091 https://atcoder.jp/contests/abc091

/*
 * 自力で解けず、解説を見た。けんちょん氏のが分かりやすかった。
 *   https://drken1215.hatenablog.com/entry/2018/09/08/204300
 * 
 * 以下、0-indexedで記載する。
 * 
 * xorの時点でbitごとに考えたい気持ちになる。
 * a+bのi bit目が1である個数が奇数なら、総xorのi bit目も1になる。
 * 
 * a+bの加算なのが厄介。ここができなかった。
 * i bit目について考えると、(i+1)より上のbitには興味がないので、2^(i+1)でMODを取ってしまう。
 * するとaについて固定したときに、
 *   a+bのi桁目が1
 *   a+b(mod 2^(i+1)) が2^i以上2^(i+1)未満
 *   2^i <= a+b < 2^(i+1)
 *   2^i-a <= b < 2^(i+1)-a
 *   1)2^i-a >= 0 のケース
 *     2^i-a <= b < 2^(i+1)-a となるbの個数を求めたい
 *   2)2^i-a < 0 のケース
 *     0 <= b < 2^(i+1)-a となるbの個数と、
 *     2^(i+1) + 2^i - a <= b となるbの個数の和を求めたい
 *   これらはbをソートしておくことで、二分探索で求められる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・xorはbitごとに考えてみる。
 * ・xorに加算が絡む場合、そのbitより上のbitには興味がないので、MODを取ってしまう。
 *   すると特定のbitが1になるような範囲が求まる。
**/

void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_miman(vector<T> &a, T val) {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {valより大きい最左のidx, valより大きい要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_koeru(vector<T> &a, T val) {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}
// {val以上の最左のidx, val以上の要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_ijou(vector<T> &a, T val)  {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}

int main(void)
{
	// 0-indexed
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i];}

	ll ans = 0;
	for(ll bi = 0; bi < 30; bi++)  // bi bit目について
	{
		// Bを2^(bi+1)でMOD取る
		vector<ll> bb(N);
		for(i = 0; i < N; i++)
		{
			bb[i] = b[i] % (1<<(bi+1));
		}
		sort(bb.begin(), bb.end());

		ll cnt = 0;  // そのbitが1になる個数
		for(i = 0; i < N; i++)  // a[i]
		{
			auto a_bk = a[i];
			a[i] %= (1<<(bi+1));
			ll tmp = 0;
			pair<ll,ll> t;
			if(a[i] <= (1<<bi))
			{
				t = bi_idxnum_ijou(bb, (1<<bi)-a[i]);
				tmp = t.second;
				t = bi_idxnum_ijou(bb, (1<<(bi+1))-a[i]);
				tmp -= t.second;
			}
			else
			{
				t = bi_idxnum_miman(bb, (1<<(bi+1))-a[i]);
				tmp = t.second;
				t = bi_idxnum_ijou(bb, (1<<(bi+1))+(1<<bi)-a[i]);
				tmp += t.second;
			}
			cnt += tmp;
			a[i] = a_bk;
		}
		if(cnt%2 == 1)
		{
			setbit(ans, bi);
		}
	}
	cout << ans << endl;

	return 0;
}
