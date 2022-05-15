#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// https://atcoder.jp/contests/abc241

/*
 * 6マス分探索して、そのうち4マス以上が黒であれば条件を満たす。
 * 愚直に、各マス目について上下左右、またナナメ4方向、合計8方向を探索する。
 */
bool func(int i, int j, int di, int dj, int N, vector<vector<int>> &square)
{
	if((i+di*5) < 0 || (i+di*5) >= N || (j+dj*5) < 0 || (j+dj*5) >= N) return false;

	int count = 0;
	for(int k = 0; k < 6; k++)
	{
		if(square[i+di*k][j+dj*k] == 1) count++;
	}

	if(count >= 4) return true;
	else return false;
}

int main(void)
{
	int i, j, k;
	int N;
	cin >> N;
	vector<vector<int>> square(N, vector<int>(N, 0));
	for(i = 0; i < N; i++)
	{
		string str;
		cin >> str;
		for(j = 0; j < N; j++)
		{
			if(str[j] == '#') square[i][j] = 1;
		}
	}

	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			int count = 0;
			// 0-indexedで、マス目(i, j)について調査する
			if(func(i, j, 0, -1, N, square) ||  // 左
				func(i, j, 0, +1, N, square) || // 右
				func(i, j, -1, 0, N, square) || // 上
				func(i, j, +1, 0, N, square) || // 下
				func(i, j, +1, +1, N, square) || // 右下
				func(i, j, -1, +1, N, square) || // 右上
				func(i, j, +1, -1, N, square) || // 左下
				func(i, j, -1, -1, N, square)    // 左上
				)
			{
				cout << "Yes" << endl;
				return 0;
			}
		}
	}

	cout << "No" << endl;
	return 0;
}
