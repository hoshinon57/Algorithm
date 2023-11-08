#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC301 https://atcoder.jp/contests/abc301

/*
 * bitDPとBFSを用いて解く。メインはbitDP.
 * 
 * 頂点番号を
 *   0:スタート
 *   1:ゴール
 *   2～:お菓子マス
 * として管理する。
 * まずは各頂点間の距離dist[i][j]を、BFSで求める。
 * グリッドグラフ上のBFSを、頂点数だけ繰り返すイメージ。
 * 
 * 次に「各お菓子を取る/取らないの全パターン」について(2^food通り)、bitDPを行う。
 * スタートからゴールまでの距離がT以下のものについて、
 * 「お菓子を取る」個数が最大のものが答となる。
 */

vector<string> mp;
const int MAX_N = 20;  // 頂点数の最大 (スタート＋ゴール＋最大18個のお菓子)
int dist[MAX_N][MAX_N];  // dist[i][j]:頂点i->jへの距離

// dp[S][v]:
//   Sの各ビットをそれぞれの都市に割り当てて、
//   1になっているビットに対して好きな順に辿ったときの、最小距離
//   ただし最後にvに行く、という制約
//   未探索であれば-1
vector<vector<int>> dp;

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// dは要素数[H][W]で、未探索部分はINF32で埋めていること。
// 探索結果として、dに(sy,sx)を起点(=0)とした距離を格納する。
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &d)
{
	queue<pair<int,int>> que;

	d[sy][sx] = 0;
	que.push({sy, sx});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();
		for(int i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(d[ny][nx] != INF32) continue;

			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			d[ny][nx] = d[y][x] + 1;
			que.push({ny, nx});
		}	
	}
	return;
}

// Sの各ビットをそれぞれの都市に割り当てて、
// 1になっているビットに対して好きな順に辿ったときの、最小距離を返す
// ただしスタート地点を0とし、最後にvに行く、という制約
// N:頂点数
// 呼び出し元での事前準備：
//   dist[][]に頂点間の距離を設定する
//   dp[][]を未探索の-1にする
int bitDP(int S, int v, int N)
{
	// メモ化再帰で解く
	if(dp[S][v] != -1) return dp[S][v];  // メモ化済み

	if(S == 0)  // ベースケース
	{
		// 頂点0をスタートとするので、dp[0][0]は0, dp[0][*]はありえないためINF.
		if(v == 0) dp[S][v] = 0;
		else dp[S][v] = INF32;

		return dp[S][v];
	}

	if(((S >> v) & 1) == 0) {dp[S][v] = INF32; return dp[S][v];}  // Sに頂点vが含まれていない
	int S_prev = S & (~(1<<v));  // vに相当するビットを落とす

	int &d = dp[S][v];
	d = INF32;
	for(int u = 0; u < N; u++)
	{
		// S_prev内の頂点uを決めて、u->vへの遷移を考える
		if(dist[u][v] == INF32) continue;  // u->vへの辺が無ければスルー
		
		// dp[S][v] = dp[S-{v}][u] + dist[u][v]
		d = min(d, bitDP(S_prev, u, N) + dist[u][v]);
	}
	return d;
}

int main(void)
{
	ll i, j;
	ll H, W, T;
	cin >> H >> W >> T;
	mp.resize(H);
	vector<ll> x(2), y(2);  // ひとまずS,G分を確保
	int food = 0;  // お菓子の個数
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
		for(j = 0; j < W; j++)
		{
			switch(mp[i][j])
			{
			case 'S':  // スタートは頂点0
				y[0] = i; x[0] = j;
				break;
			case 'G':  // ゴールは頂点1
				y[1] = i; x[1] = j;
				break;
			case 'o':  // お菓子は頂点2以降
				y.push_back(i);
				x.push_back(j);
				food++;
				break;
			}
		}
	}

	// dist[i][j]を求める
	for(i = 0; i < food+2; i++)  // スタート＋ゴール＋お菓子
	{
		vector<vector<int>> d(H, vector<int>(W, INF32));  // d[H][W]
		bfs_grid_graph(H, W, y[i], x[i], d);
		for(j = 0; j < food+2; j++)
		{
			dist[i][j] = d[y[j]][x[j]];
		}
	}

	dp.resize(1<<MAX_N, vector<int>(MAX_N, -1));
	int ans = -1;
	for(i = 0; i < (1<<food); i++)  // 各お菓子を取る/取らない の全パターン
	{
		int S = (i<<2) | 0x03;  // お菓子をbit2以降に、bit0,1にそれぞれスタートとゴールを割り当てる
		if(bitDP(S, 1, food+2) <= T)  // 1:ゴールマス
		{
			ans = max(ans, __builtin_popcount(S) - 2);  // -2はスタートとゴールの分
		}
	}
	cout << ans << endl;

	return 0;
}
