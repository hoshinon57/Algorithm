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

// ABC105 https://atcoder.jp/contests/abc105

/*
 * 10進数から2進数表記に直すアルゴリズムをもとに、がちゃがちゃやってたら解けた。
 * 「いつもの」は、whileにて次のループ(桁)に行くとき、N/=2にて最下位ビットを落としているが、
 * 今回は今見ている桁が正なら-1, 負なら+1して最下位ビットを落とした。
 * 
 * 各種解説では、もっと「ちゃんとした」というか、どうあるべきかを元に実装している感じ。
 *   https://drken1215.hatenablog.com/entry/2018/08/13/123600
 *   https://betrue12.hateblo.jp/entry/2018/08/12/124557
 *   https://zenn.dev/wapa5pow/articles/abc105-c-d9c64231b932c133c062
 */

int main(void)
{
	ll N; cin >> N;
	string ans;

	if(N == 0)
	{
		cout << 0 << endl;
		return 0;
	}
	
	int b = 0;
	while(N != 0)
	{
		if((N&1) == 1)
		{
			if(b%2 == 0) N--;
			else N++;
			ans.push_back('1');
		}
		else ans.push_back('0');
		b++;
		N /= 2;
	}

	reverse(ans.begin(), ans.end());
	cout << ans << endl;

	return 0;
}
