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

// 競プロ典型90問:04 https://atcoder.jp/contests/typical90/tasks/typical90_d

// 行ごとの和と列ごとの和を足し、重複したa[i][j]を引けばよい。

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	vector<vector<int>> a(H, vector<int>(W, 0));  // a[H][W]
	vector<int> sum_gyou(H), sum_retu(W);
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			cin >> a[i][j];
			sum_gyou[i] += a[i][j];
			sum_retu[j] += a[i][j];
		}
	}

	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			int d = sum_gyou[i] + sum_retu[j] - a[i][j];
			cout << d;
			if(j != W-1) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
