#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:32 https://atcoder.jp/contests/typical90/tasks/typical90_af

/*
 * 順列全探索で解く。公式解説とほぼ同じ実装。
 *   https://github.com/E869120/kyopro_educational_90/blob/main/sol/032.cpp
 * 
 * N=10と制約が小さいので、選手の全ての並べ方を全探索しても間に合う。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<vector<int>> a(N, vector<int>(N, 0));  // a[N][N]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++) cin >> a[i][j];
	}
	vector<vector<bool>> ng(N, vector<bool>(N, false));  // ng[N][N]
	int M; cin >> M;
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		ng[x][y] = ng[y][x] = true;
	}

	vector<int> arr(N);
	iota(arr.begin(), arr.end(), 0);
	int ans = INF32;
	do
	{
		// 不仲の組み合わせチェック
		bool ok = true;
		for(i = 0; i < N-1; i++)
		{
			if(ng[arr[i]][arr[i+1]]) ok = false;
		}
		if(!ok) continue;

		int sum = 0;
		for(i = 0; i < N; i++)
		{
			// 人：arr[i]
			// 区間：i
			sum += a[arr[i]][i];
		}
		chmin(ans, sum);
	} while (next_permutation(arr.begin(), arr.end()));
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
