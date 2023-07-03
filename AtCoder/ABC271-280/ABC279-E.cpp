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

// ABC279 https://atcoder.jp/contests/abc279

/*
 * 自力で解けず、解説を見た。
 * 累積和的な考え方で解く。
 * 
 * 基本はkyopro_friends氏の解説の通り。
 *   https://atcoder.jp/contests/abc279/editorial/5311
 *   https://twitter.com/kyopro_friends/status/1596501785682006016
 * 
 * ＞すなわち、iを除いた1以上M以下の整数kについて、昇順に以下の操作を行う
 * について、「iより前の操作」と「iより後の操作」とで分けて考える。
 * 
 * (1)iより前の操作
 *   "1"の行き先を保持しておき、操作ごとに追従していく。(idx_one)
 *   各操作において、B[Ak]とB[Ak+1]の交換をするにあたり
 *     idx_one=Akであれば、idx_oneを+1する。つまり右側へずらす。
 *     idx_one=Ak+1であれば、-1する。つまり左側へずらす。
 * 
 * (2)iより後の操作
 *   b_latter[N]={1,2,3,...,N} という配列を用意しておく。
 *   b_latter[i]は、左からi番目が、最終的に何番目に行くかを指す。
 *   事前に、i=M,つまり後ろから順にAkとAk+1の交換を行っておく。
 *   各操作において、再度AkとAk+1の交換を行うことで、
 *   事前操作のうち最後の操作を「無かったこと」にできる。
 *   
 * 以上、各操作ごとに(1)と(2)の結果を組み合わせれば答となる。
 */

int main(void)
{
	// 1-indexed
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(M+1); for(i = 1; i < M+1; i++) {cin >> a[i];}
	vector<int> b_latter(N+1);
	for(i = 0; i < N+1; i++)
	{
		b_latter[i] = i;
	}

	int idx_one = 1;  // iより前の操作について、1の行き先(index)
	// M, M-1, ..., 1の順に操作を行う
	for(i = M; i >= 1; i--)
	{
		 // a[i]とa[i]+1
		 swap(b_latter[a[i]], b_latter[a[i]+1]);
	}

	for(i = 1; i <= M; i++)  // iを除いて操作する
	{
		// b_latterの更新
		// a[i], a[i]+1の交換操作を無かったことにする
		swap(b_latter[a[i]], b_latter[a[i]+1]);

		cout << b_latter[idx_one] << endl;

		// idx_oneの更新
		if(idx_one == a[i])
		{
			idx_one++;  // 右側へ
		}
		else if(idx_one == a[i]+1)
		{
			idx_one--;  // 左側へ
		}
	}
	
	return 0;
}
