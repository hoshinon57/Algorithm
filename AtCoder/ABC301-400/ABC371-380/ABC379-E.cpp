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

// ABC379 https://atcoder.jp/contests/abc379

/*
 * 公式解説を元に実装した内容。(コンテスト時は遅延セグ木を持ち出して解いた)
 *   https://atcoder.jp/contests/abc379/editorial/11311
 *   https://x.com/kyopro_friends/status/1855250259515183286
 * 
 * S=3579で考えてみると、以下の和となる。
 *    3
 *   35
 *  357
 * 3579
 *    5
 *   57
 *  579
 *    7
 *   79
 *    9
 * Sの上の桁から見ていったときに、i桁目の値をdとして、
 *   now=pre
 *   nowにd*(i+1)を加算 上からi桁目の文字は(i+1)回加算されるため
 *   keta[i]=now
 *   pre=now
 * を繰り返していく。
 * 
 * keta[i]は答のi桁目の値となるが、10以上の値が入っているので調整が必要。
 * 今度は下の桁から処理していく。
 * Sには1～9の範囲しかとらないことから、処理にていわゆるキャリーが0になれば処理終了、としてよい。
 * 
 * [ACまでの思考の流れ]
 * ・文字ごとに答への寄与を考えてみる。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	vector<ll> keta(N+10);  // 上から見ていってi桁目 (i=0～N-1)

	ll pre = 0;
	for(i = 0; i < N; i++)  // 上の桁から見ていく
	{
		ll now = pre;
		now += (s[i]-'0') * (i+1);  // (この桁の値) * (上から何桁目か)
		keta[i] = now;
		pre = now;
	}

	string ans;
	ll c = 0;
	i = N-1;  // 下の桁から
	while(true)
	{
		if(i >= 0)
		{
			c += keta[i];
		}
		if(c == 0) break;
		ans += (char)(c%10 + '0');
		c /= 10;
		i--;
	}
	reverse(ans.begin(), ans.end());
	cout << ans << endl;

	return 0;
}
