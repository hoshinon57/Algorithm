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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題9
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2008yo/tasks/joi2008yo_d

/*
 * 以下、0-indexedで記載する。
 * 
 * まずは星座の0番目の星と、写真の星の1つに対応させてみる（＝重ねてみる）。
 * このとき星座の1番目以降の星について、写真上の対応する座標に星が存在するならば、
 * そこに星座があると判断できる。
 * 上記処理を、写真の星n個それぞれについて全探索すればよい。
 * 
 * base_x, y:星座の0番目の星の座標
 * diff_x[i], y[i]:0番目の星とi番目の星の、座標の差分
 * とおく。
 * また写真において特定の座標に星があるかどうかは、setにて管理する。
 * 
 * 計算量は、星を重ね合わせる部分がO(MN),setの操作がO(logN)であるので、
 * O(NMlogN) となる。
 */
int main(void)
{
	// 0-indexed
	int i, j;
	int m;
	cin >> m;
	int base_x, base_y;  // 星座における、0番目の星の座標
	vector<int> diff_x(m), diff_y(m);  // diff_x[i]:星座にて、0番目の星とi番目の星のx座標の差分
	for(i = 0; i < m; i++)
	{
		if(i == 0)
		{
			cin >> base_x >> base_y;
		}
		else
		{
			cin >> diff_x[i] >> diff_y[i];
			// i番目の星は、0番目の星からどれだけ離れているか
			diff_x[i] -= base_x;
			diff_y[i] -= base_y;
		}
	}
	int n;
	cin >> n;
	vector<int> star_x(n), star_y(n);
	set<pair<int,int>> star_set;  // 写真における星の座標の集合 {x,y}
	for(i = 0; i < n; i++)
	{
		cin >> star_x[i] >> star_y[i];
		star_set.insert({star_x[i], star_y[i]});
	}
	// 入力ここまで

	// 星座の0番目の星を、写真の星の1つ1つに重ねていき、全て重なったら星座とみなす
	for(i = 0; i < n; i++)  // 星座の0番目の星を、写真のi番目の星に重ねてみる
	{
		bool ok = true;
		for(j = 1; j < m; j++)  // 星座の0番目の星を重ねているので、j=1から判定する
		{
			int x = star_x[i] + diff_x[j];
			int y = star_y[i] + diff_y[j];
			if(star_set.count({x, y}) == 1)
			{
				; // 星座のj番目の星までは重なった 処理継続
			}
			else
			{
				// 重ならなかったので、写真の次の星に重ねてみる
				ok = false;
				break;
			}
		}
		if(ok)
		{
			cout << star_x[i] - base_x << " " << star_y[i] - base_y << endl;
			break;
		}
	}

	return 0;
}
