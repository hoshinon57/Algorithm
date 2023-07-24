#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC127 https://atcoder.jp/contests/abc127

/*
 * まず、連想配列を用いてカードを
 *   a_list[val] = valを値に持つカードの枚数
 * のように {値,枚数} の組み合わせで管理する。
 * これを、入力のAおよび操作のB,Cひっくるめて管理する。
 * 
 * すると求める最大値は、上記a_listの大きい方からN枚を加算したものになる。
 * 
 * ※けんちょん氏の解説などでは、ランレングス圧縮的な考え方と書かれていた。
 *   https://drken1215.hatenablog.com/entry/2019/06/15/021000
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	map<int,int> a_list;  // a_list[val] = valを値に持つカードの枚数
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		a_list[a]++;
	}
	for(i = 0; i < M; i++)
	{
		int b, c; cin >> b >> c;
		a_list[c] += b;
	}

	// AおよびB,Cひっくるめて、大きい方からN枚選ぶ
	ll ans = 0;
	auto itr = a_list.end();
	itr--;
	while(N > 0)
	{
		ll val = itr->first;
		ll cnt = itr->second;
		ans += val * min(cnt, (ll)N);  // 今見ているカードの枚数かNのうち、小さい方の数まで選べる
		N -= cnt;

		itr--;
	}
	cout << ans << endl;

	return 0;
}
