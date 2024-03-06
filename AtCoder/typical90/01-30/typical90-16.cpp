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

// 競プロ典型90問:16 https://atcoder.jp/contests/typical90/tasks/typical90_p

/*
 * 全探索で解く。
 * 
 * A,B円硬貨を使う枚数について全探索する。
 * これらをi,j枚とすると、残りの支払いは N-(A*i+B*i) となる。
 * これが0未満ならそもそもNG.
 * Cで割り切れるなら、Cで割った値をkとし、i+j+kが答の候補。
 * 
 * 使う枚数は最大で10^4枚であることから、計算量は10^8とかになる。
 * ただし処理がシンプルであることと、枝刈りをすることで十分に間に合う。
 */

int main(void)
{
	ll N, A, B, C; cin >> N >> A >> B >> C;
	int i, j, k;
	int ans = INF32;
	for(i = 0; i < 10000; i++)  // A円硬貨
	{
		for(j = 0; j < 10000; j++)  // B円硬貨
		{
			ll zan = N-(i*A+j*B);
			if(zan < 0) break;
			if(zan % C == 0)
			{
				k = zan / C;
				chmin(ans, i+j+k);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
