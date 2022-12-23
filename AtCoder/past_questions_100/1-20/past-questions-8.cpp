#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題8
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/s8pc-6/tasks/s8pc_6_b

/*
 * スタートとゴールを、10^9個あるマスそれぞれについて調べようとすると、到底間に合わない。
 * よってスタートとゴールの候補をどうにかして削ることを考える。
 * 
 * スタート(ゴールでも同じ)地点が、ある品物XとYの間に仮置きしたときに、
 * スタートをXの方向に近づけていくと、以下3パターンが考えられる。
 * (1)合計が小さくなっていくケース：
 *   YよりXの側に品物が多い場合。
 *   これは、少なくともXまでスタート地点を動かす方が改善される。
 * (2)合計が大きくなっていくケース：
 *   XよりYの側に品物が多い場合。
 *   これは(1)と逆に、Yまでスタート地点を動かす方が改善される。
 * (3)合計が変わらないケース：
 *   Xの側とYの側で品物が同じ数の場合。
 *   これは逆に言えば、スタート地点をX(ないしY)にまで動かしても問題ない。
 * 
 * であれば、スタートとゴールの候補は、品物のあるマスに限定できると考える。
 * よってスタートとゴールの位置を、全ての品物のマスに設定して全探索し、その最小値が答となる。
 * 
 * 計算量はO(N^3).
 */

int main(void)
{
	int i, j, k;
	int N;
	cin >> N;
	vector<ll> a(N), b(N);
	vector<ll> sg;  // スタート及びゴールの座標の候補
	for(i = 0; i < N; i++)
	{
		cin >> a[i] >> b[i];
		sg.push_back(a[i]);
		sg.push_back(b[i]);
	}

	ll answer = INF64;
	for(i = 0; i < (int)sg.size(); i++)  // スタートをsg[i]とする
	{
		for(j = 0; j < (int)sg.size(); j++)  // ゴールをsg[j]とする
		{
			ll sum = 0;
			for(k = 0; k < N; k++)  // 人kに対して移動時間を計算
			{
				ll tmp1, tmp2;
				// スタート, A, B, ゴールの経路
				tmp1 = abs(sg[i] - a[k]) + abs(a[k] - b[k]) + abs(b[k] - sg[j]);
				// スタート, B, A, ゴールの経路
				tmp2 = abs(sg[i] - b[k]) + abs(b[k] - a[k]) + abs(a[k] - sg[j]);
				// 2経路のうち短い方
				sum += min(tmp1, tmp2);

				// [補足]
				// A<Bであるので、スタート地点<ゴール地点と決め打ちして
				// 「スタート, A, B, ゴールの経路」のみを計算しても良いみたい。（各所の解説より）
				// この場合、sg[]を事前にソートしておく形になりそう。
			}
			answer = min(answer, sum);
		}
	}
	cout << answer << endl;

	return 0;
}
