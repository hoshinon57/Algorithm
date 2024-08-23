#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC281 https://atcoder.jp/contests/abc281

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc281/editorial/5367
 *   https://drken1215.hatenablog.com/entry/2022/12/13/144200
 * 
 * 公式解説の通りで、上の桁から順に確定させていく。
 * Aのbi桁目が0か1かでAを分離し、この集合をそれぞれA0,A1とすると、以下3パターンに分かれる。
 * (1)A0の要素が存在しない
 *    A1のbi桁目を0にすることで、答のbi桁目を0にできる。
 * (2)A1の要素が存在しない
 *    (1)と同じく、答のbi桁目を0にできる。
 * (3)A0,A1どちらも要素が存在する
 *    まず、bi桁目はどうやっても1になる。
 *    そのうえで、(bi-1)桁目についてA0,A1でそれぞれ再帰して、そのうちの小さい方を採用する。
 * 
 * [実装の備忘録]
 * #else側に、最初にACした実装を残している。
 * これは、再帰先へ送る引数のbit操作を行ってから再帰している。
 * (ベースケースにてaの値そのものを返す形)
 * bit操作が増えるため定数倍が悪いかなと思ったが、#if 1側とほとんど変化なしだった。
 * #if 1側:123ms, #else側:127ms
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・再帰の段数が最大でMで、要素の値は再帰全体で1回(定数回)しか操作しないとき、
 *   計算量はO(NM)で抑えることができる。
 * ・最小/最大の数値を構築したい場合、上位の桁から並べたときに辞書順最小(最大)を求めることになる。
 *   よって上位の桁から貪欲に決めていくことを考えてみる。
 */

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

#if 1
// biビット目以下について解を返す
int dfs(int bi, vector<ll> &a)
{
	if(bi == -1) return 0;  // ベースケース
	vector<ll> a0, a1;  // iビット目が0と1に分ける
	for(auto &e : a)
	{
		if(isbiton(e, bi)) a1.push_back(e);
		else a0.push_back(e);
	}

	ll ret = INF64;
	if(a0.size() == 0)
	{
		// 0側が無いので、1側のbitを0にすることで答えのbiビット目を0にできる
		ret = dfs(bi-1, a1);
	}
	else if(a1.size() == 0)
	{
		// 1側が無いので、0側をそのまま採用して答えのbiビット目を0にできる
		ret = dfs(bi-1, a0);
	}
	else
	{
		// 0側と1側でminを取る
		// biビット目は必ず1になる
		ll tmp1 = dfs(bi-1, a0);
		ll tmp2 = dfs(bi-1, a1);
		ret = (1<<bi) | min(tmp1, tmp2);
	}
	return ret;
}

#else  // 最初にACした実装 再帰先へ送る引数のbit操作を行ってから再帰する
int dfs(int bi, vector<ll> &a)  // biビット目
{
	if(bi == -1) return a[0];  // aに複数要素が含まれることもあり得るが、ここまで来ればaの全要素は同じ値
	vector<ll> a0, a1;  // iビット目が0と1に分ける
	ll ret = INF64;
	for(auto &e : a)
	{
		if(isbiton(e, bi)) a1.push_back(e);
		else a0.push_back(e);
	}

	if(a0.size() == 0)
	{
		// 0側が無いので、1側のbitを0にしたものを採用
		for(auto &e : a1) unbit(e, bi);
		ret = dfs(bi-1, a1);
	}
	else if(a1.size() == 0)
	{
		// 1側が無いので、0側をそのまま採用
		ret = dfs(bi-1, a0);
	}
	else
	{
		// 0側のbitを1にしたものと、
		// 1側をそのまま採用したものとで、minを取る
		for(auto &e : a0) setbit(e, bi);
		ll tmp1 = dfs(bi-1, a0);
		ll tmp2 = dfs(bi-1, a1);
		ret = min(tmp1, tmp2);
	}
	return ret;
}
#endif

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	cout << dfs(29, a) << endl;

	return 0;
}
