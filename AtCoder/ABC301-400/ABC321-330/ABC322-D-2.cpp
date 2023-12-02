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

// ABC322 https://atcoder.jp/contests/abc322

/*
 * 気合の9重ループで解く。
 * 参考：https://atcoder.jp/contests/abc322/submissions/46100941
 * 
 * とはいえ…供養レベルで残しておくイメージで…
 */

vector<vector<int>> p1, p2, p3;  // ポリオミノ

void rotate(vector<vector<int>> &p)
{
	int i, j;
	auto pw = p;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			pw[j][3-i] = p[i][j];
		}
	}
	p = pw;
}

bool put_sub(vector<vector<int>> &grid, vector<vector<int>> &p, int dx, int dy)
{
	int i, j;
	for(i = 0; i < 4; i++)  // y
	{
		for(j = 0; j < 4; j++)  // x
		{
			if(p[i][j] == 0) continue;
			// 以下、p[i][j]=1
			int ny = i+dy;
			int nx = j+dx;
			if(ny < 0 || ny >= 4 || nx < 0 || nx >= 4) return false;
			if(grid[ny][nx]) return false;
			grid[ny][nx] = 1;
		}
	}

	return true;
}

bool put(int dx1, int dy1, int dx2, int dy2, int dx3, int dy3)
{
	vector<vector<int>> grid;
	grid.resize(4, vector<int>(4));

	if(!put_sub(grid, p1, dx1, dy1)) return false;
	if(!put_sub(grid, p2, dx2, dy2)) return false;
	if(!put_sub(grid, p3, dx3, dy3)) return false;

	return true;
}

bool solve(void)
{
	int i, j;
	int dir1,  dx1,  dy1,  dir2,  dx2,  dy2,  dir3,  dx3,  dy3;
	int cnt = 0;
	p1.resize(4, vector<int>(4));
	p2.resize(4, vector<int>(4));
	p3.resize(4, vector<int>(4));
	for(i = 0; i < 4; i++) {
		string s; cin >> s;
		for(j = 0; j < 4; j++) if(s[j] == '#') {p1[i][j] = 1; cnt++; }
	}
	for(i = 0; i < 4; i++) {
		string s; cin >> s;
		for(j = 0; j < 4; j++) if(s[j] == '#') {p2[i][j] = 1; cnt++; }
	}
	for(i = 0; i < 4; i++) {
		string s; cin >> s;
		for(j = 0; j < 4; j++) if(s[j] == '#') {p3[i][j] = 1; cnt++; }
	}
	if(cnt != 16) return false;

	for(dir1 = 0; dir1 < 4; dir1++) {
	for(dx1 = -3; dx1 <= 3; dx1++) {
	for(dy1 = -3; dy1 <= 3; dy1++) {
		for(dir2 = 0; dir2 < 4; dir2++) {
		for(dx2 = -3; dx2 <= 3; dx2++) {
		for(dy2 = -3; dy2 <= 3; dy2++) {
			for(dir3 = 0; dir3 < 4; dir3++) {
			for(dx3 = -3; dx3 <= 3; dx3++) {
			for(dy3 = -3; dy3 <= 3; dy3++) {
				if(put(dx1, dy1, dx2, dy2, dx3, dy3)) return true;
			}
			}
			rotate(p3);
			}
		}
		}
		rotate(p2);
		}
	}
	}
	rotate(p1);
	}
	return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
