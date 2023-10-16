#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC324 https://atcoder.jp/contests/abc324

/*
 * 全ての平方数について、それを並べ替えたものがSと一致するかを判定すればよい。
 * 制約より、平方数は最大で10^13まで見ればよいので、
 * 0から √(10^13) までの各iに対して1つずつi*iを判定していけばよく、十分に間に合う。
 * 
 * コンテスト時、判定は数値の登場回数をカウントして実施したが、
 * 解説を参考に、以下実装では文字列比較にて行った。
 * (こちらの方が実行時間はかかるが、実装は簡易と感じる)
 * 
 * Sおよびi*iを文字列としてソートすれば、文字列一致で判定できる。
 * i*iについて足りない桁数は0で埋めておく処理が必要。
 * 
 * [ACまでの思考の流れ]
 * ・最初はbitDPかと思ったが、どうも上手く行かなかったのでとりやめ。
 * ・N=13の制約があやしいなあ。
 *   これが平方数になるなら、i*iのiは6～7桁か。これならiについて全探索できそう。
 */

int main(void)
{
	ll i, j;
//	const ll RT_MAX = 4*1e6;  // 2乗で10^13を超える値
	const ll RT_MAX = 3500000;  // 2乗で10^13を超える値
	int N; cin >> N;
	string s; cin >> s;
	sort(s.begin(), s.end());

	ll ans = 0;
	for(i = 0; i <= RT_MAX; i++)
	{
		string is = to_string(i*i);
#if 1
		for(j = (int)is.size(); j < N; j++)  // 足りない桁数を0で埋める
		{
			is.push_back('0');
		}
#else
		// 解説を参考にこれでもOK
		if((int)is.size() < N) is.resize(N, '0');
#endif
		sort(is.begin(), is.end());
		if(is == s) ans++;
	}
	cout << ans << endl;

	return 0;
}
