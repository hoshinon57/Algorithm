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

// ABC278 https://atcoder.jp/contests/abc278

/*
 * 2次元累積和で解く。
 * 
 * 2次元累積和 sum[H][W] を、数の種類であるN個用意する。
 * 各塗りつぶし問題について、
 *   塗りつぶしたマスに書かれていた個数
 *   全体に書かれていた個数
 * を比較し、不一致なら「値が残っている」と判断できる。
 */

const int MAX = 305;
int sum[MAX][MAX][MAX];  // [N][H][W]  数値Nの個数の累積和

int main(void)
{
	int i, j, k;
	int H, W, N; cin >> H >> W >> N;
	int h, w; cin >> h >> w;

	// 累積和の構築
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			int a; cin >> a;
			for(k = 1; k <= N; k++)
			{
				int add = 0;
				if(k == a) add = 1;
				sum[k][i+1][j+1] = sum[k][i+1][j] + sum[k][i][j+1] - sum[k][i][j] + add;
			}
		}
	}

	// y方向 [i,i+h)
	// x方向 [j,j+w)
	// の範囲を見る
	for(i = 0; i+h <= H; i++)
	{
		for(j = 0; j+w <= W; j++)
		{
			int ans = 0;
			for(k = 1; k <= N; k++)
			{
				// 値kについて、
				//   塗りつぶしたマスに書かれていた個数
				//   全体に書かれていた個数
				// を比較し、不一致なら「値が残っている」と判断できる
				int n1 = sum[k][i+h][j+w] - sum[k][i+h][j] - sum[k][i][j+w] + sum[k][i][j];
				int n2 = sum[k][H][W] - sum[k][H][0] - sum[k][0][W] + sum[0][0][0];
				if(n1 != n2) ans++;
			}
			cout << ans << " ";
		}
		cout << endl;
	}

	return 0;
}
