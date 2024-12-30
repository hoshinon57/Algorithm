#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC276 https://atcoder.jp/contests/abc276

/*
 * 深さ優先探索で解く。
 * ★微妙な実装。今後はABC276-E-2.cppを参照のこと。
 * 
 * よくある実装の通りに、始点から4方向に探索を進めていく。
 * それまで進んだマスを保持しておき、経路が4以上になる状態で始点に戻ったら成功とみなす。
 * (これにより、始点から2手目で戻ってくる経路を除外できる)
 * 
 * 【備忘録】
 * マス(i,j)を探索済みかどうかを示すused変数は、関数の帰りがけ時には「未探索」には戻さない。
 * 求めたいのは経路の存在有無であるため、戻す必要はないため。
 * また、以降の枝刈りとしても使える。
 *   ⇒戻すとTLEとなった。
 */

int H, W;
int sx, sy;  // 始点
vector<vector<bool>> wall;  // wall[i][j]:(i,j)が壁ならtrue
vector<vector<bool>> used;  // used[i][j]:DFS時に(i,j)を探索済みならtrue
ll cnt = 0;

// (y,x)を起点として深さ優先探索を行う
// 同じマスを通らずに始点に戻れればtrueを返す
bool dfs(int y, int x)
{
	// 範囲外、または障害物ならfalse
	if(y < 0 || y >= H || x < 0 || x >= W) return false;
	if(wall[y][x]) return false;

	// 「長さ4以上の経路」という条件つきで、始点に戻ってきたらtrue
	// (この時点ではcntをインクリメントしていないため、3以上という条件)
	if(y == sy && x == sx && cnt >= 3) return true;

	// 探索済みならfalse
	// 始点はスタート時に探索済みになるため、上記のゴール判定より後に行う必要がある
	if(used[y][x]) return false;

	used[y][x] = true;
	cnt++;

	// 4方向それぞれへ探索を進める
	// trueで返ってきたらそのままtrueで返る
	for(int i = 0; i < 4; i++)
	{
		if(dfs(y+1, x)) return true;
		if(dfs(y-1, x)) return true;
		if(dfs(y, x+1)) return true;
		if(dfs(y, x-1)) return true;
	}

	// ここに来た場合、経路が見つからなかったということ。
	// 帰りがけ時にcntを戻しておく。
	cnt--;

	// ★used[]はfalseに戻さない。求めたいのは経路の存在有無であるため、戻す必要はない。
	//   また、以降の枝刈りとしても使える。
	//   ⇒戻すとTLEとなった。
	// used[y][x] = false;

	return false;
}

int main(void)
{
	int i, j;
	cin >> H >> W;
	wall.resize(H, vector<bool>(W, false));
	used.resize(H, vector<bool>(W, false));
	for(i = 0; i < H; i++)
	{
		string str;
		cin >> str;
		for(j = 0; j < W; j++)
		{
			if(str[j] == '.')
				;  // 処理なし
			else if(str[j] == '#')
				wall[i][j] = true;
			else
			{
				sx = j;
				sy = i;
			}
		}
	}

	if(dfs(sy, sx))
		cout << "Yes" << endl;
	else
		cout << "No" << endl;

	return 0;
}
