#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題7
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2007ho/tasks/joi2007ho_c

/*
 * 柱を2つ選んだ状態で正方形を作ることを考える。
 * 2つ選んだ柱の座標の差分を{dx,dy}とおくと、作れる正方形は
 *   {dy, -dx}方向に辺を伸ばすパターン
 *   {-dy, dx}方向に辺を伸ばすパターン
 * の2つがある。
 * 辺を伸ばした先に柱があれば(これはsetを用いて判定した)、正方形を作ることができる。
 * 
 * 上記を、2つの柱の全組み合わせで判定する。
 * 計算量は、柱の全組み合わせを列挙するのにO(N^2),
 * setの操作にO(logN)であるので、O(N^2*logN)となる。
 * 
 * [補足]
 * setではなくunordered_set(操作が平均的にO(1))を使う手段もある。
 * それぞれのコードを提出したところ、実行時間は以下であった。
 *   set:580ms
 *   unordered_set:380ms
 * ただしunordered_setを使う場合、pairにはハッシュ関数が定義されていないため
 * 自分で追加してやる必要がある。
 * 以下URLを参考にした。
 * https://qiita.com/tqk/items/7c4df1eeb5ebaedcf714
 */

int main(void)
{
	int i, j;
	int N;
	cin >> N;
	vector<int> x(N), y(N);
	set<pair<int,int>> xy_set;  // 柱の座標の集合
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
		xy_set.insert({x[i], y[i]});
	}

	int answer = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // 頂点i,jの2つを選択
		{
			int dx = x[i]-x[j];
			int dy = y[i]-y[j];

			// 2頂点から正方形を作りたい場合、2頂点の座標の差分を{dx,dy}とおくと、
			//   {dy, -dx}方向に辺を伸ばすパターン
			//   {-dy, dx}方向に辺を伸ばすパターン
			// の2つがある。
			// それぞれ辺を伸ばした先の座標に、柱があるかを判定する
			if( (xy_set.find({x[i]+dy, y[i]-dx}) != xy_set.end() && xy_set.find({x[j]+dy, y[j]-dx}) != xy_set.end() )
				|| (xy_set.find({x[i]-dy, y[i]+dx}) != xy_set.end() && xy_set.find({x[j]-dy, y[j]+dx}) != xy_set.end() ) )
			{
				// ifがtrueなら、4つの柱で正方形を作ることができる

				// 三平方の定理より、(正方形の辺の長さ)=√(dx*dx+dy*dy) となる。
				// よって面積は dx*dx+dy*dy で求められる。
				answer = max(answer, dx*dx+dy*dy);
			}
		}
	}
	cout << answer << endl;

	return 0;
}
