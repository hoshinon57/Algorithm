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

// ABC196 https://atcoder.jp/contests/abc196

/*
 * 深さ優先探索で解く。
 * 
 * 左上から順に畳を敷いていき、最後まで埋めたら解を+1する。
 * 1*1の半畳、2*1および1*2の畳それぞれを敷くパターンを、DFSで実装する。
 * 
 * 解説では、dfs()の引数に「次にチェックすべき空きマス」を入れていたが、自分の実装では入れなかった。
 * HWの上限が小さく、毎度左上から空きマスをチェックする実装でも十分だと判断したため。
 */

int H, W;
int ans = 0;

// 畳がそれぞれA枚、B枚残っていて、
// mp[][]に現在の敷いた情報(0:敷いていない、1:敷いている)が保持されているときに、
// DFSで左上から順に畳を敷いていく
void dfs(int A, int B, vector<vector<int>> &mp)
{
	int i, j;
	// 空きマスを探す
	int y = -1, x = -1;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] == 0)
			{
				y = i;
				x = j;
				break;
			}
		}
		if(y != -1) break;
	}
	if(y == -1)  // 全マス埋めた
	{
		ans++;
		return;
	}

	// (y,x)が空きマスなので、そこを起点に畳を置いてみる
	if(B > 0)  // 半畳
	{
		mp[y][x] = 1;
		dfs(A, B-1, mp);
		mp[y][x] = 0;
	}
	if(A > 0 && x+1 < W && mp[y][x+1] == 0)  // 横2マス、縦1マスを置けるか
	{
		mp[y][x] = mp[y][x+1] = 1;
		dfs(A-1, B, mp);
		mp[y][x] = mp[y][x+1] = 0;
	}
	if(A > 0 && y+1 < H && mp[y+1][x] == 0)  // 横1マス、縦2マスを置けるか
	{
		mp[y][x] = mp[y+1][x] = 1;
		dfs(A-1, B, mp);
		mp[y][x] = mp[y+1][x] = 0;
	}
	return;
}

int main(void)
{
	int A, B; cin >> H >> W >> A >> B;
	vector<vector<int>> mp(H, vector<int>(W, 0));  // mp[H][W]

	dfs(A, B, mp);
	cout << ans << endl;

	return 0;
}
