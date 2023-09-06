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

// ABC318 https://atcoder.jp/contests/abc318

/*
 * コンテスト中に解けず、解説を見た。
 * 公式解説を参考にbitDPで解く。
 * https://atcoder.jp/contests/abc318/editorial/7066
 * 
 * 辺を選ぶことは頂点を2つ選ぶことと同義であり、
 * bitDP(S, N)を以下のように定義する。
 *   S:ビットごとに頂点と見立てて、1になっていればその頂点を選び済み or 選ばないことを決定した のどちらか
 *   N:頂点数
 * bit(1<<N -1, N)が答となる。
 * 
 * 選ぶ2頂点のうち、1つ目は「bitが立っているもののうち、先頭のを固定で使う」としてよい。
 * 2つ目は「bitが立っているものを1つずつ選択して再帰」とする。
 * 
 * 2つの頂点を選ぶときに同じ頂点も許可することで、Nが奇数の場合に対応できる。
 * (同じ頂点を選ぶ場合、辺として使わないと同義になる)
 * 
 * [どう思考すれば解法にたどり着けるか]
 * うーん、「複数個ずつ選び、その選び方によってスコアが変わる」ようなケースはbitDP, という典型なのかなあ。
 * Nが奇数もありうるのが難しいが、これは「1つしか選ばない場合は、その頂点は使わない」という位置づけにして、
 * dist[]もそうなるように設定するという考え方は、今後使えるようにしておきたい。
 */

vector<vector<ll>> dist;
vector<ll> dp;

// メモ化再帰にて辺の重みの総和の最大値を返す
// S:ビットごとに頂点と見立てて、1になっていればその頂点を選び済み or 選ばないことを決定した のどちらか
ll bitDP(int S, int N)
{
	int i;
	if(dp[S] != -1) return dp[S];  // メモ化済み
	if(S == 0) return 0;  // ベースケース

	int l, r;  // 選ぶ頂点の1つ目と2つ目
	// 1つ目はbitが立っているもののうち、先頭のを固定で使う
	for(i = 0; i < N; i++)
	{
		if(S&(1<<i))
		{
			l = i;
			break;
		}
	}
	// 2つ目はbitが立っているものからそれぞれ選んで再帰
	for(i = 0; i < N; i++)
	{
		if((S&(1<<i)) == 0) continue;
		r = i;
		int S_prev = (S & (~(1<<l))) & (S & (~(1<<r)));  // 再帰用に使う頂点のbitを落とす

		// dp[S] = dp[S-{l,r}] + dist[l][r]
		// l,rが同じ頂点を指す場合、dist[]は0になる。この場合、その頂点は「使わないことを決定した」という位置づけとなる。
		dp[S] = max(dp[S], bitDP(S_prev, N) + dist[l][r]);
	}

	return dp[S];
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	dist.assign(N, vector<ll>(N, 0));  // dist[N][N]
	dp.assign(1<<N, -1);  // dp[1<<N]
	for(i = 0; i < N-1; i++)
	{
		for(j = i+1; j < N; j++)
		{
			cin >> dist[i][j];
			dist[j][i] = dist[i][j];
		}
	}
	cout << bitDP((1<<N)-1, N) << endl;

	return 0;
}
