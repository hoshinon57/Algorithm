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

// ABC223 https://atcoder.jp/contests/abc223

/*
 * 自力で解けず、解説を見た。
 * 
 * 長方形が2つのケースを考えると、
 *   長方形が横方向に並ぶ
 *   長方形が縦方向に並ぶ
 * の2通り。
 * また「横方向に並ぶ」とき、1つ目の長方形はY方向にめいっぱい伸ばしたものが最善。
 * このときX方向の長さは (長方形の面積)/Y の切り上げとなる。
 * 
 * 3通りの場合も考え方は同じで、
 *   3つをどの順に置いていくか
 *   横方向/縦方向どちらをめいっぱいにして置くか
 * をDFSを用いて全探索していく。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・まずは長方形が2つのケースで考えてみる。
 * ・面積が決まっていることから、長方形を中途半端に置くのは無駄で、
 *     X方向にめいっぱい伸ばした形で置く
 *     Y方向にめいっぱい伸ばした形で置く
 *   の2択になる、というのにたどり着けるようにする。
 */

ll X, Y;
ll s[3];  // A, B, C

// 以降、残りの長方形が配置できればtrue
// rx, ry:残りエリアの幅と高さ
// idx:次に使うs[]
bool dfs(ll rx, ll ry, int idx)
{
	ll nx, ny;

	// 3つ置けた
	if(idx == 3) return true;

	// そもそも残りエリアの面積が足りなければfalse
	if(rx*ry < s[idx]) return false;

	// 横いっぱいに置くパターン
	nx = rx;
	ny = (s[idx]+nx-1)/nx;  // 切り上げ
	if(dfs(rx, ry-ny, idx+1)) return true;

	// 縦いっぱいに置くパターン
	ny = ry;
	nx = (s[idx]+ny-1)/ny;
	if(dfs(rx-nx, ry, idx+1)) return true;

	return false;
}

bool solve(void)
{
	int i, j;
	cin >> X >> Y;
	cin >> s[0] >> s[1] >> s[2];

	// A,B,Cをどの順にはめていくか、計6パターン
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 2; j++)
		{
			if(dfs(X, Y, 0)) return true;
			swap(s[1], s[2]);
		}
		swap(s[1], s[2]);
		swap(s[0], s[1]);
	}

	return false;
}

int main(void)
{
	YesNo(solve());
	return 0;
}
