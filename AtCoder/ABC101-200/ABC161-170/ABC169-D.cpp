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

// ABC169 https://atcoder.jp/contests/abc169

/*
 * うまく発想できずに苦戦した…勘違いも多々あった… 素因数分解で解く。
 * 参考記事：
 *   https://drken1215.hatenablog.com/entry/2020/06/01/010800
 *   https://blog.hamayanhamayan.com/entry/2020/06/01/210704
 * 
 * N = p^e * p^e * p^e * ... のように素因数分解すると、p^eごとに独立の操作となる。
 * よって各p^eについて操作回数を求め、加算していけばよい。
 * p^eについての操作回数はeにのみ依存し、
 *   1+2+3+...x <= e となる最大のx
 * となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・方針が立ちにくい場合、まずは手を動かしてみる。
 * ・本問についてはNを素因数分解したときに、それぞれのp^eごとに独立していることが分かれば、実装が見えてくる。
 */

// Nを素因数分解し、{素因数, 指数}のpairの形でprime_listに入れる
// 以下URLをほぼそのまま持ってきている
// https://qiita.com/drken/items/a14e9af0ca2d857dad23
// その他、参考URL：
//   https://algo-method.com/descriptions/119
void prime_factorize(long long N, vector<pair<long long, long long>> &prime_list)
{
	long long i;
	// 入力Nに対して、2から√Nの各整数で割れるだけ割っていく
	for(i = 2; i*i <= N; i++)
	{
		if(N%i != 0) continue;  // iが素因数でなければ次へ

		long long ex = 0;  // 指数
		while(N%i == 0)  // 割れる限り割る
		{
			N /= i;
			ex++;
		}
		prime_list.push_back({i, ex});  // iで割り切ったら結果を保存
	}
	if(N != 1)  // 割り切った後に1以外の値で残っていれば、それも素因数
	{
		prime_list.push_back({N, 1});
	}

	return;
}

int main(void)
{
	ll N; cin >> N;
	vector<pair<ll,ll>> plist;
	prime_factorize(N, plist);
	int ans = 0;
	for(auto [p,e] : plist)
	{
		int cnt = 0;
		for(int i = 1; i < 60; i++)  // 2^60 > 10^12
		{
			cnt += i;
			if(cnt > e) break;
			ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
