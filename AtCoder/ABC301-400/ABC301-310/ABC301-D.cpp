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

// ABC301 https://atcoder.jp/contests/abc301

/*
 * コンテスト中に解けず、解説を見た。
 * 
 * 文字列を先頭（数値としては上の桁）から見ていく。
 * 最初の'?'をまず1, それ以外の'?'は0としてみなしたときに、
 *   Nより大きい：最初の'?'は0
 *   Nより小さい：最初の'?'は1
 * として確定させる。
 * これを次以降の桁も見ていく。
 * 
 * 最終的にできた数値が答。(ただしNより大きい場合、-1が答)
 */
int main(void)
{
	int i;
	string s; cin >> s;
	ll N; cin >> N;

	// strを数値化して返す
	// '?'は0とみなす
	auto conv = [&](string str) -> ll
	{
		ll ret = 0;
		int k = 0;
		reverse(str.begin(), str.end());  // 反転した方が計算しやすい
		for(k = 0; k < (int)str.size(); k++)
		{
			if(str[k] == '0' || str[k] == '?') continue;  // ?は0とみなす
			ret += ((ll)1<<k);
		}
		return ret;
	};

	for(i = 0; i < (int)s.size(); i++)
	{
		if(s[i] == '?')
		{
			s[i] = '1';  // まずは1を試す
			if(conv(s) > N) s[i] = '0';  // Nより大きかったら、0にする
		}
	}

	ll ans = conv(s);
	if(ans > N) ans = -1;
	cout << ans << endl;

	return 0;
}
