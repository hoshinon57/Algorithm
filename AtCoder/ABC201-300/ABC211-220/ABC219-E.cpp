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

// ABC219 https://atcoder.jp/contests/abc219

/*
 * bit全探索で解く。
 * 
 * 4x4行列にていくつかの要素を1にすると、これをちょうど囲むようなお堀は1つに決まる。
 *   1 1 1 1
 *   0 0 1 1
 *   0 0 1 1
 *   1 1 1 1
 * であれば、問題文の最初の画像にあるようなお堀に該当する。
 * 全部で16マスであるので、堀のパターンは全部で2^16通り。これをbit全探索していく。
 * 
 * 生成したbit列(以下areaと書く)に対して、適切なお堀が作れたかの条件は、
 * (1)村がある部分はareaにて1であること
 * (2)areaが1の要素の連結成分数は1であること
 *    例えば以下のような堀はNG.
 *      1 1 0 0
 *      1 1 0 0
 *      0 0 1 1
 *      0 0 1 1
 * (3)堀の内部に穴が無いこと
 *    例えば以下のような堀はNG.
 *      1 1 1 0
 *      1 0 1 0
 *      1 1 1 0
 *      0 0 0 0
 * 以上3点を満たしていればOK.
 * 
 * (3)の判定方法については、周囲に0のマスを追加した6x6行列を考えたときに、
 * 0の連結成分数が1かどうかで判定できる。
 */

int a[4][4];

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// distは要素数[H][W]で、未探索部分はINF32で埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
// area:入力マップ  n:移動できるマスの値
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist,vector<vector<int>> &area, int n)
{
	queue<pair<int,int>> que;

	dist[sy][sx] = 0;
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
			if(dist[ny][nx] != INF32) continue;

			// 壁など 問題によって書き換える
			if(area[ny][nx] != n) continue;

			dist[ny][nx] = dist[y][x] + 1;
			que.push({ny, nx});
		}	
	}
	return;
}

// bit列のうち、[i][j]に相当する値0 or 1を返す
int bi(int bit, int i, int j)
{
	return ((bit>>(i*4+j))&1);
}

// bitで示されたお堀が条件を満たすか
bool check(int bit)
{
	int i, j;

	// 条件(1) a[i][j]=1なら、bit列でも1であること
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(a[i][j] == 1 && bi(bit, i, j) == 0) return false;
		}
	}

	// 周囲を0で囲んだ6x6行列を作成
	vector<vector<int>> area(6, vector<int>(6, 0));  // area[6][6]
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++) area[i+1][j+1] = bi(bit, i, j);
	}

	// 条件(2) areaが1の連結成分数は1であること
	vector<vector<int>> dist(6, vector<int>(6, INF32));  // dist[6][6]
	int cnt = 0;
	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 6; j++)
		{
			if(area[i][j] == 0) continue;
			if(dist[i][j] == INF32)
			{
				bfs_grid_graph(6, 6, i, j, dist, area, 1);
				cnt++;
			}
		}
	}
	if(cnt != 1) return false;

	// 条件(3) 堀の内部に穴が無いこと
	//         ⇒周囲を0として座標(0,0)からBFSし、値が0のマスに全て辿れるかで判定する
	dist.resize(6, vector<int>(6, INF32));
	bfs_grid_graph(6, 6, 0, 0, dist, area, 0);
	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 6; j++)
		{
			if(area[i][j] == 0 && dist[i][j] == INF32) return false;  // 値0のマスについて、(0,0)からのBFSで辿れなかった
		}
	}
	
	return true;
}

int main(void)
{
	int i, j;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++) cin >> a[i][j];
	}

	ll ans = 0;
	for(i = 0; i < 1<<16; i++)
	{
		if(check(i)) ans++;
	}
	cout << ans << endl;

	return 0;
}
