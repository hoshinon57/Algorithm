#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題79より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc106

/*
 * 2次元累積和で解く。
 * 
 * まず考えたのは、1次元の累積和で考えると、
 *   1 6
 *   2 4
 * のような入力L,Rがあったときに、sum={0,1,2,2,2,1,1} のようになる。
 * これでは
 *   1 4
 *   2 6
 * の入力と区別がつかない。
 * ということは、何らか情報を追加してやる必要がある。
 * ⇒では2次元にしよう。
 * 
 * 1 6の入力(列車)に対して、これが該当するようなクエリを考えると、
 *   0 6, 0 7, 0 8, ... 0 ∞
 *   1 6, 1 7, 1 8, ... 1 ∞
 * となる。つまり入力[L,R]について [0～L, R～∞]が該当する。
 * すると[0,R]に+1, [L+1, R]に-1を設定して2次元の累積和を取れば、
 * クエリ[p,q]に対してsum[p][q]が答になりそう。
 * 
 * 後は上記を実装する。
 * https://qiita.com/drken/items/56a6b68edef8fc605821 の2次元累積和のコードと異なり、閉区間で計算している。
 * 計算量はO(N^2+Q).
 */

using vvi = vector<vector<int>>;

int main(void)
{
	int i, j;
	int N, M, Q;
	cin >> N >> M >> Q;
	vvi train(N+5, vector<int>(N+5));  // train[L][R]
	vvi train_sum(N+5, vector<int>(N+5));  // train_sum[L][R]:閉区間[0,L] x [0,R]の累積和
	for(i = 0; i < M; i++)
	{
		int L, R;
		cin >> L >> R;
		train[0][R]++;
		train[L+1][R]--;
	}

	// 2次元累積和の構築
	for(i = 1; i <= N; i++)  // 端っこだけは別計算
	{
		train_sum[i][0] = train_sum[i-1][0] + train[i][0];
		train_sum[0][i] = train_sum[0][i-1] + train[0][i];
	}
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= N; j++)
		{
			train_sum[i][j] = train_sum[i-1][j] + train_sum[i][j-1] - train_sum[i-1][j-1] + train[i][j];
		}
	}

	// クエリ処理
	for(i = 0; i < Q; i++)
	{
		int p, q;
		cin >> p >> q;
		cout << train_sum[p][q] << endl;
	}

	return 0;
}
