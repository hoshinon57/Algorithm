#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC232 https://atcoder.jp/contests/abc232

/*
 * 動的計画法で解く。
 * 
 * H*Wマス全てに対して愚直に操作するとTLE.
 * マスをグループ分けできないかを考える。
 * 
 * 例としてH=6,W=4 かつx1=2,y1=2 のときに操作をしてみると、
 *   DBDD
 *   CACC
 *   DBDD
 *   DBDD
 *   DBDD
 *   DBDD
 * のように4パターンの値のみ取ることが分かる。
 * 上記ではx1=2,y1=2としたが、これ以外の値でも同じで、
 *   A:スタートのマス
 *   B:スタートと同じ列のマス
 *   C:スタートと同じ行のマス
 *   D:それ以外のマス
 * と4パターンに分類できる。
 * 
 * 操作をi回まで行ったときの組み合わせの数をres[i][A～D]とすると、
 *   res[i+1][A] = res[i][B]*(H-1) + res[i][C]*(W-1)
 *   res[i+1][B] = res[i][A] + res[i][B]*(H-2) + res[i][D]*(W-1)
 *   res[i+1][C] = res[i][A] + res[i][C]*(W-2) + res[i][D]*(H-1)
 *   res[i+1][D] = res[i][B] + res[i][C] + res[i][D]*(H-2 + W-2)
 * と遷移する。
 * 
 * 出力は、(x2,y2)が上記A～Dのどれに属するかを判定する。
 * ただし問題文より、行を表すのがx、列を表すのがyになっているのに注意。
 * (問題文をしっかり読むべきだが、これは感覚とは逆で不親切だと思う…)
 */

const ll MOD = 998244353;
ll res[1000000+1][4] = {0};

int main(void)
{
	// 1-indexed
	int H, W, K;
	cin >> H >> W >> K;
	int x1, y1, x2, y2;
	cin >> x1 >> y1 >> x2 >> y2;

	res[0][0] = 1;  // スタートマス
	for(int i = 1; i <= K; i++)
	{
		res[i][0] = res[i-1][1]*(H-1) + res[i-1][2]*(W-1);
		res[i][0] %= MOD;

		res[i][1] = res[i-1][0] + res[i-1][1]*(H-2) + res[i-1][3]*(W-1);
		res[i][1] %= MOD;

		res[i][2] = res[i-1][0] + res[i-1][2]*(W-2) + res[i-1][3]*(H-1);
		res[i][2] %= MOD;

		res[i][3] = res[i-1][1] + res[i-1][2] + res[i-1][3]*(H-2+W-2);
		res[i][3] %= MOD;
	}

	ll ans;
	if(x1==x2 && y1==y2)
	{
		ans = res[K][0];
	}
	else if(x1==x2)  // 上からi行目、左からj列目を(i,j)とすると、入力は(x,y)となっており感覚と逆
	{
		ans = res[K][2];
	}
	else if(y1==y2)
	{
		ans = res[K][1];
	}
	else
	{
		ans = res[K][3];
	}
	cout << ans << endl;

	return 0;
}
