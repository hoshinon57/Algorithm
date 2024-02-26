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

// 競プロ典型90問:28 https://atcoder.jp/contests/typical90/tasks/typical90_ab

// 2次元いもす法でぐわっと。

int main(void)
{
	int i, j;
	int N; cin >> N;
	const int S = 1000;
	vector<vector<int>> imos(S+2, vector<int>(S+2, 0));  // imos[y][x]
	for(i = 0; i < N; i++)
	{
		int lx, ly, rx, ry; cin >> lx >> ly >> rx >> ry;
		imos[ly][lx]++;
		imos[ly][rx]--;
		imos[ry][lx]--;
		imos[ry][rx]++;
	}
	for(i = 0; i <= S; i++)  // y
	{
		for(j = 1; j <= S; j++)  // x
		{
			imos[i][j] += imos[i][j-1];
		}
	}
	for(j = 0; j <= S; j++)  // x
	{
		for(i = 1; i <= S; i++)  // y
		{
			imos[i][j] += imos[i-1][j];
		}
	}
	vector<int> ans(N+1);
	for(i = 0; i <= S; i++)
	{
		for(j = 0; j <= S; j++)
		{
			ans[imos[i][j]]++;
		}
	}
	for(i = 1; i <= N; i++)
	{
		cout << ans[i] << endl;
	}

	return 0;
}
