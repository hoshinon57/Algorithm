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

// ABC140 https://atcoder.jp/contests/abc140

/*
 * ※いやー、難しかった…！
 *   最初はゴリゴリ汚い実装で(WA重ねつつ)ACしたけれど、その後各種解説を見て、
 *   スマートな実装をようやく理解できた。
 * 
 * LRLRRL などのサンプルに対し、いくつか手作業でやってみると、
 *   RもしくはLの塊を丸ごと反転させる
 * のが最適っぽい。
 * このとき、1操作ごとに幸福な人は2人増える。ただし、端っこの場合のみ+1人にとどまる。
 * 
 * また、幸福な人の最大値は N-1 となる。
 * なので、ここは思いきって、
 *   初期状態で幸福な人をXとすると、操作回数をKとして
 *   X+2K が最終的に幸福な人の数になる。
 *   ただしN-1で上限クリップする。
 * と実装してしまってよい。
 * 
 * 「初期状態で幸福な人の数」は、L,Rによらず s[i]==s[i-1] の数としてよい。
 * LLL... のときはs[i]側が、RRR... のときはs[i-1]側が、幸福な人としてカウントされる。
 */

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	string s; cin >> s;
	int ans = 0;
	for(i = 1; i < N; i++)  // 初期状態で幸福な人の数を数える
	{
		if(s[i] == s[i-1]) ans++;  // LLL...  RRR...  のケース、どちらも正しく判定される
	}
	cout << min(ans + K*2, N-1) << endl;  // 操作ごとに+2, ただし上限はN-1.

	return 0;
}
