#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC366 https://atcoder.jp/contests/abc366

/*
 * 3次元累積和を用いて解いてみた版。
 * 将来、3次元累積和が必要な問題が出たときに引っ張り出せるように。
 * 
 * 包除原理に近い。
 * 
 * 参考：
 *   公式解説
 *   https://qiita.com/mattsunkun/items/9969ce11d666dda4611f
 *   ChatGPTに「3次元累積和の構築方法を教えてください」で十分なものが出てきた
 */

ll a[105][105][105], sum[105][105][105];  // (x,y,z)

int main(void)
{
	int i, j, k;
	int N; cin >> N;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			for(k = 0; k < N; k++)
			{
				cin >> a[i][j][k];
			}
		}
	}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			for(k = 0; k < N; k++)
			{
				sum[i+1][j+1][k+1] = sum[i][j+1][k+1] + sum[i+1][j][k+1] + sum[i+1][j+1][k]
					 - sum[i][j][k+1] - sum[i][j+1][k] - sum[i+1][j][k]
					 + sum[i][j][k] + a[i][j][k];
			}
		}
	}
	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int lx, rx, ly, ry; cin >> lx >> rx >> ly >> ry;
		int lz, rz; cin >> lz >> rz;
		// 0-indexedに直して、半開区間にする [l,r)
		lx--; ly--; lz--;
		int ans = sum[rx][ry][rz] - sum[lx][ry][rz] - sum[rx][ly][rz] - sum[rx][ry][lz]
			+ sum[lx][ly][rz] + sum[lx][ry][lz] + sum[rx][ly][lz]
			- sum[lx][ly][lz];
		cout << ans << endl;
	}

	return 0;
}
