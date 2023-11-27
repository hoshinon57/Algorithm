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

// ABC218 https://atcoder.jp/contests/abc218

/*
 * 要素がN*Nである2次元配列について、
 *   90度回転させる関数
 *   「データが無い」部分を詰めて左上に平行移動する関数
 * を用意する。
 * 
 * まずTを左上に詰めておき、
 * Sを90度ずつ回転し、左上に詰めたものがTと一致すればOK.
 */

// 要素がN*Nであるaについて、「データが無い」部分を詰めて全要素を左上に平行移動する。
// pass:この値を「データが無い」とみなす
void align(vector<vector<int>> &a, int pass)
{
	int i, j;
	int offset_x=0, offset_y=0;
	int n = a.size();
	for(i = 0; i < n; i++)  // 上から1行ずつ見ていって、「データが無い行」がいくつあるか調べる
	{
		bool ok = true;
		for(j = 0; j < n; j++) { if(a[i][j]!=pass) ok = false; }
		if(!ok) { offset_y = i; break; }
	}
	for(i = 0; i < n; i++)  // 左から1列ずつ見ていって、「データが無い列」がいくつあるか調べる
	{
		bool ok = true;
		for(j = 0; j < n; j++) { if(a[j][i]!=pass) ok = false; }
		if(!ok) { offset_x = i; break; }
	}

	vector<vector<int>> a_bk = a;
	a.assign(n, vector<int>(n, 0));
	for(i = 0; i < n; i++)
	{
		if(i + offset_y >= n) break;
		for(j = 0; j < n; j++)
		{
			if(j + offset_x >= n) break;
			a[i][j] = a_bk[i+offset_y][j+offset_x];
		}
	}
}

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

bool solve(void)
{
	int i, j;
	int N; cin >> N;
	vector<vector<int>> s(N, vector<int>(N, 0));
	vector<vector<int>> t(N, vector<int>(N, 0));
	for(i = 0; i < N; i++)
	{
		string str; cin >> str;
		for(j = 0; j < N; j++)
		{
			if(str[j] == '#') s[i][j] = 1;
		}
	}
	for(i = 0; i < N; i++)
	{
		string str; cin >> str;
		for(j = 0; j < N; j++)
		{
			if(str[j] == '#') t[i][j] = 1;
		}
	}

	// Sを90度ずつ回転し、左上に詰めたものがTと一致すればOK
	align(t, 0);  // Tは先だって詰めておく
	for(i = 0; i < 4; i++)
	{
		align(s, 0);
		if(s == t) return true;
		rotate(s);
	}

	return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
