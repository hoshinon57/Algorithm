#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題14
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/s8pc-4/tasks/s8pc_4_b

/*
 * bit全探索で解く。
 * 
 * 全建物の中からK個選び、選んだものを見えるように高さを増やす。
 * この選び方をbit全探索にて実現する。
 * 
 * 左から順に見ていったときに、これまで最も高い建物の高さを保持しておく(=prev_max)。
 * 「見えるようにする」建物であれば、この値と自身の建物の高さを比較し、必要なだけ高さを増やす。
 * 
 * 「見えなくてもよい」建物であっても、prev_maxの更新は必要。
 * 
 * 計算量はO(N*2^N).
 */

int main(void)
{
	// 0-indexed
	int i,j;
	int N, K;
	cin >> N >> K;
	vector<int> a(N);
	for(i = 0; i < N; i++) cin >> a[i];

	ll answer = INF64;
	for(i = 0; i < (1<<N); i++)
	{
		// iについてビットが1であるa[i]について、見えるようにする
		if(__builtin_popcount(i) != K) continue;  // ビットが1になっているのがK個のときのみ

		int prev_max = -1;  // 左から順に見ていったときに、これまで最も高い建物の高さ
		ll sum = 0;  // 高さを増やす量
		for(j = 0; j < N; j++)
		{
			if((i & (1<<j)) == 0)  // bitが1のもののみ処理
			{
				// 高さの更新だけは必要
				// a[j]の高さを増す必要は無いため、prev_maxに+1は不要
				prev_max = max(prev_max, a[j]);
				continue;
			}

			int target = max(prev_max+1, a[j]);  // j番目の建物は、どこまで高くすれば見えるか
			sum += target - a[j];  // j番目の建物の高さを増やす量
			prev_max = target;  // 高さを更新
		}
		answer = min(answer, sum);
	}
	cout << answer << endl;

	return 0;
}
