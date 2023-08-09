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

// ABC194 https://atcoder.jp/contests/abc194

/*
 * 公式解説を参考に実装した。
 * https://atcoder.jp/contests/abc194/editorial/827
 * 
 * 2次元配列posを用意し、pos[i]にはA[j]=iとなる位置jを入れておく。
 * iの小さい順に見ていって、pos[i][j+1]-pos[i][j] > M となるようなiが存在すれば、それが答となる。
 * ※番兵として-1,Nを入れておくと実装が楽。
 */

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<vector<int>> pos(N);  // pos[i]:A[j]=iとなる位置jを入れていく
	for(i = 0; i < N; i++)
	{
		pos[i].push_back(-1);  // 番兵
	}
	for(i = 0; i < N; i++)
	{
		pos[a[i]].push_back(i);
	}
	for(i = 0; i < N; i++)
	{
		pos[i].push_back(N);  // 番兵
	}

	int ans = N;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < (int)pos[i].size()-1; j++)
		{
			if(pos[i][j+1] - pos[i][j] > M)
			{
				ans = min(ans, i);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
