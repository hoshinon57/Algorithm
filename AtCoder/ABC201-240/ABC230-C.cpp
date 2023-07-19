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

// ABC230 https://atcoder.jp/contests/abc230

/*
 * 公式解説の別解と同じ解き方。
 * 
 * P<=i<=Q, R<=j<=Sの範囲で探索して(制約より、探索する数は3*10^5以下)、
 * 各マスごとに白黒どちらかを判別すればよい。
 *   i-A==  j-B  であれば黒。(A,B)の右下/左上方向。
 *   i-A==-(j-B) であれば黒。(A,B)の右上/左下方向。
 * となる。
 */

int main(void)
{
	ll N, A, B; cin >> N >> A >> B;
	ll P, Q, R, S; cin >> P >> Q >> R >> S;
	ll i, j;
	for(i = P; i <= Q; i++)
	{
		for(j = R; j <= S; j++)
		{
			char out = '.';
			if( (i-A == j-B) || (i-A == -(j-B)) ) out = '#';
			cout << out;
		}
		cout << endl;
	}

	return 0;
}
