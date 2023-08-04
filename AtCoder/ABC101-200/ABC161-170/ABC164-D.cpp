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

// ABC164 https://atcoder.jp/contests/abc164

/*
 * 最初、M=2019としてO(NM)の解法でACしたが、想定解はO(N)のアルゴリズムだった。
 * 各種解説を参考にO(N)を実装したのが本ソースコード。
 *   https://drken1215.hatenablog.com/entry/2020/04/29/171300
 *   https://twitter.com/kyopro_friends/status/1254407777725734913
 * 
 * 文字列Sを後ろから1文字ずつ取っていくことを考える。
 * 先頭からi文字目～j文字目を取ったものが2019の倍数である、ということは、
 *   i文字目    ～N文字目を取った数値を2019で割った余り
 *   (j+1)文字目～N文字目を取った数値を2019で割った余り
 * が一致することになる。
 * 
 * よって後ろから1文字ずつ取っていき、そのときの数値をvalとする。
 * これまでにvalが何回出てきたかをcnt[]にて管理し、各桁ごとにcnt[val]を答に加算していけばよい。
 * ※cntの初期値はcnt[0]のみ1で、他は0
 */

int main(void)
{
	int i;
	string s; cin >> s;
	int N = (int)s.size();
	const ll MOD = 2019;

	int cnt[MOD] = {0};
	cnt[0] = 1;  // 何も取らない場合(=0)
	ll ans = 0;
	ll now = 0, prev = 0;
	ll p = 1;  // 10のべき乗の部分
	for(i = 0; i < N; i++)
	{
		ll c = (int)(s[N-i-1] - '0');  // 右側から取る
		now = (c * p + prev) % MOD;
		ans += cnt[now];
		cnt[now]++;
		// 次の文字のための準備
		p = (p*10)%MOD;
		prev = now;
	}
	cout << ans << endl;

	return 0;
}
