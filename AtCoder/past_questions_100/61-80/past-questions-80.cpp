#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題80
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/gigacode-2019/tasks/gigacode_2019_d

/*
 * 2次元累積和で解く。
 * 
 * 考えられる土地の選び方を全探索し、
 * 予算内で購入できるうち最大の面積を求める、のが基本の考え方。
 * ただしこの全探索には、購入する土地の
 *   高さと幅について、それぞれ1～Hおよび1～Wで全探索
 *   左上の座標について、それぞれ1～Hおよび1～Wで全探索
 * する必要があり、この時点でO(H^2*W^2).
 * これに加え、各選び方についてA(i,j)を足していくのはTLEとなりそう。
 * ここで2次元累積和を用いる。
 * 
 * a_sum[y][x]を、[0,y) x [0,x) の半開区間におけるA(i,j)の総和として構築すれば、
 * A(i,j)を足していく部分をO(1)で計算できる。
 * 
 * 計算量はO(H^2*W^2).
 * H,Wの最大値は125であり H^2*W^2 = 2.4*10^8 ほどになるが、
 * 購入する土地の面積が大きくなるにつれ探索範囲も小さくなるため、
 * 実際にこの回数ループするわけではなく十分間に合う。(82msだった)
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int H, W;
	ll K, V;
	cin >> H >> W >> K >> V;
	using vvl = vector<vector<ll>>;
	vvl a(H, vector<ll>(W));  // a[y][x]
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			cin >> a[i][j];
		}
	}
	vvl a_sum(H+1, vector<ll>(W+1));
	for(i = 0; i < H; i++)  // y
	{
		for(j = 0; j < W; j++)  // x
		{
			a_sum[i+1][j+1] = a_sum[i+1][j] + a_sum[i][j+1] - a_sum[i][j] + a[i][j];
		}
	}

	int answer = 0;  // 購入できる土地の最大の面積
	for(int height = 1; height <= H; height++)  // 購入する土地の高さ
	{
		for(int width = 1; width <= W; width++)  // 購入する土地の幅
		{
			for(int sy = 0; sy + height <= H; sy++)  // 購入する土地の左上の座標Y
			{
				for(int sx = 0; sx + width <= W; sx++)  // 購入する土地の左上の座標X
				{
					// [sy,sy+height) x [sx,sx+width) の土地を買う
					int dy = sy+height;
					int dx = sx+width;
					ll value;  // 土地の価格
					value = a_sum[dy][dx] - a_sum[dy][sx] - a_sum[sy][dx] + a_sum[sy][sx];  // 各マスについてA(i,j)の総和
					value += height*width * K;  // 1マスごとにK円かかる
					if(value > V) continue;  // 予算オーバー

					answer = max(answer, height*width);
				}
			}
		}
	}
	cout << answer << endl;

	return 0;
}
