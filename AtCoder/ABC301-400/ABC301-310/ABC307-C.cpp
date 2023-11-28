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

// ABC307 https://atcoder.jp/contests/abc307

/*
 * 「Xを置く位置を固定する」「AをXにかすらせるように置く」など、以下の公式解説と考えが近かった。
 *   https://atcoder.jp/contests/abc307/editorial/6690
 * 
 * Xの左上を(0,0)に固定し、
 * A,Bの左上を(0,0)から上下左右それぞれ-9～+9平行移動させて置く。
 * ※-9と+9なのは、シートサイズの最大は10であり、
 *   ＞切り出されたシートはシートA,Bの黒いマスをすべて含む。
 *   という制約と合わせてこの範囲で動かせば十分であるため。
 * このとき、A,Bの#マスがXの範囲外に置かれる場合はNG.
 * 
 * 上記をDFSで全探索していき、
 * シートA,Bを置けたケースにて、それとシートXが完全一致していればOK.
 */

int h[3], w[3];
int sht[3][10][10];  // 順にシートA,B,X

// 次にidx=0:シートA, 1:シートBを置く
// 現在の置き方はnow
// シートA,Bを置いたのち、Xとnowが完全一致すればOK, trueを返す
bool dfs(int idx, vector<vector<int>> now)
{
	if(idx == 2)  // シートA,Bを置いた
	{
		// Xと作ったシートnowが完全一致すればOK
		int y, x;
		for(y = 0; y < h[2]; y++)
		{
			for(x = 0; x < w[2]; x++)
			{
				if(sht[2][y][x] != now[y][x]) return false;
			}
		}
		return true;
	}

	int dy, dx;
	auto now_bk = now;
	for(dy = -9; dy <= 9; dy++)  // Y方向平行移動
	{
		for(dx = -9; dx <= 9; dx++)  // X方向平行移動
		{
			now = now_bk;
			bool ok = true;
			// dy,dxだけ平行移動して、シート(idx)を置く
			// その際、#をシートXの範囲外に置こうとしたらNG
			for(int y = 0; y < h[idx]; y++)
			{
				int ny = y+dy;
				for(int x = 0; x < w[idx]; x++)
				{
					int nx = x+dx;
					if(sht[idx][y][x] == 0) continue;
					if(ny < 0 || ny >= h[2] || nx < 0 || nx >= w[2]) { ok = false; break; }  // #を、シートXの範囲外に置こうとしている
					now[ny][nx] = 1;
				}
				if(!ok) break;
			}
			if(ok)
			{
				if(dfs(idx+1, now)) return true;
			}
		}
	}

	return false;
}

bool solve(void)
{
	vector<vector<int>> now(h[2], vector<int>(w[2]));  // now[h[2]][w[2]]
	return dfs(0, now);
}

int main(void)
{
	int i, j, k;
	for(i = 0; i < 3; i++)
	{
		cin >> h[i] >> w[i];
		for(j = 0; j < h[i]; j++)
		{
			string s; cin >> s;
			for(k = 0; k < w[i]; k++)
			{
				if(s[k] == '#') sht[i][j][k] = 1;
			}
		}
	}

	YesNo(solve());

	return 0;
}
