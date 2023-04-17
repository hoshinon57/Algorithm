#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 座標圧縮(Coordinate Compression)のメモ

/*
 * 1次元の座標圧縮の手順
 *   (1)座標圧縮したい元データと、圧縮結果を保持したいデータを用意する
 *      後者には元データをコピーしておく
 *   (2)元データをソートし、重複を削除する
 *   (2)lower_bound()にて、各データが「何番目に小さいか」を求める
 * 元データは手順(1)にて書き換えられる点に注意。
 * 計算量はO(NlogN).
 * 
 * [参考資料]
 * https://drken1215.hatenablog.com/entry/2021/08/09/235400
 * https://algo-logic.info/coordinate-compress/
 * 
 * [関連する問題]
 * AOJ ALDS1_5_D https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_5_D&lang=ja
 */

// 1次元の座標圧縮
// 計算量はO(NlogN)
void compression_one(void)
{
	vector<int> a = {10, 100, 1000, 1, 5, 1, 1};  // 座標圧縮したい元データ
	vector<int> a_comp = a;

	// 元データをソートし、重複を削除する
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する

	// それぞれの元データが「何番目に小さいか」をlower_bound()で求める
	for(int i = 0; i < (int)a_comp.size(); i++)
	{
		a_comp[i] = lower_bound(a.begin(), a.end(), a_comp[i]) - a.begin();
	}
	// a_comp = {2, 3, 4, 0, 1, 0, 0}
}

int main(void)
{
	compression_one();

	return 0;
}