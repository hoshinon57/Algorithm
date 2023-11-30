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

// ABC322 https://atcoder.jp/contests/abc322

/*
 * コンテスト中に解けず、解説を見た。
 * 再帰を使い、回転や平行移動を全探索する。
 * 
 * 回転：4回転
 * 平行移動：-3～+3まで、x,yそれぞれ
 * でポリオミノを順に全探索する。
 * 
 * 平行移動したときに、'#'が範囲外になる場合はNG.
 * '#'を置こうとしたときに、すでに'#'が置かれていたらNG.
 */

vector<vector<vector<int>>> pol;  // 3つのポリオミノ

// 要素がN*Nであるaについて、右に90度回転させる
void rotate(vector<vector<int>> &a)
{
	int n = a.size();
	auto a_bk = a;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			a[i][j] = a_bk[n-1-j][i];
		}
	}
}

// idx番目のポリオミノを、dx,dy平行移動させておけるか試す
// 置ければtrueを返す
// now:現在の盤面
// true/falseに関わらず、nowは書き換えられる点に注意
bool put_test(int idx, int dy, int dx, vector<vector<int>> &now)
{
	int y, x;
	for(y = 0; y < 4; y++)
	{
		for(x = 0; x < 4; x++)
		{
			if(pol[idx][y][x] == 0) continue;
			// 以下、(y,x)は'#'
			int ny = y+dy;  // 平行移動先
			int nx = x+dx;
			if(ny < 0 || ny >= 4 || nx < 0 || nx >= 4) return false;  // 範囲外に置こうとしている
			if(now[ny][nx] == 1) return false;  // 既に置いている
			now[ny][nx] = 1;
		}
	}
	return true;
}

// idx:次にidx番目のポリオミノを置く
// now:現在の盤面
bool dfs(int idx, vector<vector<int>> &now)
{
	if(idx == 3)  // 全て置けた
	{
		return true;
	}

	int i, dy, dx;
	auto now_bk = now;
	for(i = 0; i < 4; i++)  // 回転
	{
		for(dy = -3; dy <= 3; dy++)
		{
			for(dx = -3; dx <= 3; dx++)
			{
				now = now_bk;
				if(!put_test(idx, dy, dx, now)) continue;  // 置けなかったら次へ
				if(dfs(idx+1, now)) return true;  // 再帰先からtrueで返ってきたら、そのままtrueで返す
			}
		}
		rotate(pol[idx]);
	}

	return false;
}

int main(void)
{
	int i, j, k;
	pol.resize(3, vector(4, vector<int>(4, 0)));
	int cnt = 0;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 4; j++)
		{
			string s; cin >> s;
			for(k = 0; k < 4; k++)
			{
				if(s[k] == '#')
				{
					pol[i][j][k] = 1;
					cnt++;
				}
			}
		}
	}
	if(cnt != 16)  // そもそも'#'が16個以外はNG
	{
		cout << "No" << endl;
		return 0;
	}

	vector<vector<int>> now(4, vector<int>(4, 0));
	YesNo(dfs(0, now));

	return 0;
}
