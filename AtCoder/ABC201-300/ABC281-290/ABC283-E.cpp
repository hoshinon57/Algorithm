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

// ABC283 https://atcoder.jp/contests/abc283

/*
 * 自力で解けず、解説を見た。
 * 動的計画法で解く。
 * 
 * 行列の上の行から順に決めていくことを考える。
 * i行目について操作をすると、i-1行目が孤立するか否かが確定する。
 * またi-1行目の状態は、i-2,i-1,iの3行にのみ依存する。
 * 
 * dp[i][j][k]を以下のように定義する。
 *   i行目まで見て、 (i:1～H)
 *   j:i行目が 0:正転, 1:反転:1
 *   k:i-1行目が 0:正転, 1:反転
 *   のときに、i-1行目に孤立した要素が無いような状況の、操作回数の最小値
 * 
 * 先頭と末尾の行に番兵的なダミーデータを入れておくと、処理がやりやすい。
 * (Aの値を-1にしておくと、番兵の値が孤立状態に影響を与えなくて済む)
 * 
 * dp[i][j][k]を決める際にはdp[i-1][k][l:0～1]を用いて、
 *   i-2行目:l, i-1行目:k, i行目:j  が正転反転状態を示すときの、i-1行目に孤立要素があるか
 * の判定を行う。
 * 孤立要素がなければ、dp[i][j][k]には、自身とdp[i-1][j][l]との最小値を設定する。
 * またj=1のときはi行目に操作を行っているので、dpに+1する。
 * 
 * 番兵の行であるdp[H+1]まで処理すると、ここにはi行目の結果が保持されている。
 * よってdp[H+1][*][*]の最小値が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・うーん…、方針は見えたんだけど、細かい実装が検討つかなかった。
 * ・dp[i][j][k]の定義はできたんだけど、DPの遷移が思いつかなかったかな。
 *   dp[i-1][k][*]を使う、という発想ができなかった。
 */

int H, W;
// i-2行目:l, i-1行目:k, i行目:j  が正転反転状態を示すときに、i-1行目に孤立要素があるかを調べる
bool check(vector<vector<int>> &a, int i, int l, int k, int j)
{
	vector<int> l_ = a[i-2], k_ = a[i-1], j_ = a[i];
	int x;
	for(x = 0; x < W; x++)
	{
		if(l == 1) l_[x] = 1 - l_[x];  // 反転処理
		if(k == 1) k_[x] = 1 - k_[x];
		if(j == 1) j_[x] = 1 - j_[x];
	}
	for(x = 0; x < W; x++)
	{
		bool ok = false;
		if(x != 0)
		{
			if(k_[x] == k_[x-1]) ok = true;
		}
		if(x != W-1)
		{
			if(k_[x] == k_[x+1]) ok = true;
		}
		if(k_[x] == l_[x]) ok = true;
		if(k_[x] == j_[x]) ok = true;
		if(!ok) return false;
	}
	return true;
}

int main(void)
{
	int i, j, k, l;
	cin >> H >> W;
	vector<vector<int>> a(H+2, vector<int>(W, -1));  // a[H+1][W]  枠外は-1
	for(i = 1; i < H+1; i++)
	{
		for(j = 0; j < W; j++) cin >> a[i][j];
	}

	vector dp(H+2, vector(2, vector<int>(2, INF32)));
	dp[1][0][0] = 0;  // 最初の行が正転
	dp[1][1][0] = 1;  // 最初の行が反転
	for(i = 2; i <= H+1; i++)
	{
		for(j = 0; j < 2; j++)
		{
			for(k = 0; k < 2; k++)  // dp[i][j][k]を決める
			{
				// dp[i-1][k][0]とdp[i-1][k][1]を使う
				for(l = 0; l < 2; l++)  // dp[i-1][k][l]
				{
					// i-2行目:l, i-1行目:k, i行目:j  が正転反転状態
					// ここからi-1行目の孤立有無を調べる
					if(check(a, i, l, k, j)) chmin(dp[i][j][k], dp[i-1][k][l]);
				}
				// j=1なら今回反転するので、+1する
				// H+1行目は番兵なので、実際には反転させない
				if(j==1 && i != H+1)
				{
					dp[i][j][k] = min(dp[i][j][k]+1, INF32);
				}
			}
		}
	}
	
	int ans = INF32;
	for(j = 0; j < 2; j++)
	{
		for(k = 0; k < 2; k++)
		{
			chmin(ans, dp[H+1][j][k]);
		}
	}
	if(ans == INF32) ans = -1;
	cout << ans << endl;
	return 0;
}
