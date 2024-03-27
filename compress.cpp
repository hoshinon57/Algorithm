#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

// 座標圧縮(Coordinate Compression)のライブラリ

/*
 * 1次元の座標圧縮の手順
 *   (1)座標圧縮したい元データと、圧縮結果を保持したいデータを用意する
 *      後者には元データをコピーしておく
 *   (2)元データをソートし、重複を削除する
 *   (2)lower_bound()にて、各データが「何番目に小さいか」を求める
 * 元データは手順(1)にて書き換えられる点に注意。
 * 計算量はO(NlogN).
 * 
 * 圧縮前の値を求める方法はmain()を参照。
 * 
 * [参考資料]
 * https://drken1215.hatenablog.com/entry/2021/08/09/235400
 * https://algo-logic.info/coordinate-compress/
 * 
 * [関連する問題]
 * AOJ ALDS1_5_D https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_5_D&lang=ja
 * ABC113-C (座標圧縮ではないが、考え方は似ている)
 * ABC213-C
 * ABC231-F
 * ABC261-F (座標圧縮を使わない方法もあると思う)
 * ABC277-C (mapでも十分)
 * ABC309-F
 */

// 1次元の座標圧縮
// a：座標圧縮したい元データ 処理途中で要素が書き換えられる点に注意(保持したい場合は参照を外す)
// 返り値：圧縮後のデータ
// 計算量はO(NlogN)
template <typename T>
vector<T> compression_one(vector<T> &a)
{
	vector<T> a_comp = a;

	// 元データをソートし、重複を削除する
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する

	// それぞれの元データが「何番目に小さいか」をlower_bound()で求める
	for(int i = 0; i < (int)a_comp.size(); i++)
	{
		a_comp[i] = lower_bound(a.begin(), a.end(), a_comp[i]) - a.begin();
	}

	return a_comp;
}

int main(void)
{
	vector<int> a = {10, 100, 1000, 1, 5, 1, 1};  // 座標圧縮したい元データ
	vector<int> a_comp = compression_one<int>(a);
	// a = {1,5,10,100,1000} に置き換わる
	vector<int> a_test = {2, 3, 4, 0, 1, 0, 0};
	assert(a_comp == a_test);
	// a[a_comp[i]] で、圧縮前のi番目の値を求められる

	using ll = long long;
	ll p = 1LL<<60;
	vector<ll> l = {p, p+5, p+7, p, p-1};  // 座標圧縮したい元データ
	vector<ll> l_comp = compression_one<ll>(l);
	vector<ll> l_test = {1, 2, 3, 1, 0};
	assert(l_comp == l_test);

	return 0;
}
