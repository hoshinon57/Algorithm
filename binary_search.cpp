#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 二分探索の実装テンプレートやメモ
// 実装テンプレートはmain()を参照。
// 三分探索はternary_search()を参照。

/*
 * [関連する問題]
 * ABC223-C ただし、二分探索を使わない解法の方がスマート
 * ABC229-D 解説では尺取り法がメイン
 * ABC265-D
 * ABC257-D 個人的に良問
 * ABC246-D
 * ABC269-E while()の条件が実装テンプレートと異なる
 * ABC270-E 答ではないものを二分探索する問題
 * ABC271-C パッと思いつくのはシミュレーションだが、二分探索でも解ける
 * ABC279-D 三分探索
 * ABC299-D インタラクティブ問題
 * ABC309-C
 * ABC312-C
 * ARC054-B 三分探索、二分探索 良問
 */

// 二分探索の実装練習
int my_lower_bound(vector<int> &a, int num);
int my_upper_bound(vector<int> &a, int num);

bool check(int num)
{
	// numを用いた判定をここに実装する

	return true;
}

// 二分探索の実装テンプレート
int main(void)
{
	// まず、lとrの定義を考える。
	// ある判定問題 check(num) があったときに、
	//   常に check(l)==true
	//   常に check(r)==false
	// のように定義する。
	// (true/falseが逆でも良い)

	// lの初期値は、必ずcheck(l)==trueとなる値にする。
	// rも同様。
	// ★while()の処理より、l,rの初期値はmidに選ばれることは無い、ということに留意する。
	//   そのため配列の範囲外を初期値に設定しても良い。
	int l = -1;  // -1, 0, ... など、問題文に合わせて設定する
	int r = INF32;  // INF32, INF64, N+1, ... など、問題文に合わせて設定する

	// "r - l > 1" という条件より、
	// whileが条件を満たすならば、lとrの間には必ず1つ以上の要素が存在する。この中からmidが選ばれる。
	// whileから抜けるとき、lとrは隣接している。
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid))
		{
			l = mid;
		}
		else
		{
			r = mid;
		}
	}
	// l,r,またはその他の何を出力するかは、問題文に合わせて決める。
	cout << l << endl;

	/*
	ABC269-Eを元にしたメモ。	
	ソートされた数列から目的のものを探すのではなく、
	1つだけ例外データがあるような数列から探すような場合について。
	例：数列 a={1,1,1,1,0,1,1,1} に対し、a[l]～a[r]の和を返すような関数func(l,r)があったときに、
	    a[]の中から0の場所を探したい

	閉区間[l,r]の中に目的のデータがあるとして、そこから二分探索で範囲を狭めていく。
	l=0(下限値), r=N-1(上限値) を初期値として、実装方針は以下。
	--------------------------------
	while(r > l) {     // または while(l != r)
	  mid = (r+l)/2;
	  func(l, mid);  // 閉区間[l,mid]で調べる
	  if( *** )  r = mid;  // [l,mid]に目的のデータがある場合
	  else l = mid+1;      // [l,mid]に目的のデータが無い場合
	}
	// l=rになるとwhile()から抜ける。
	// l(=r)に目的の値が保存されている。
	--------------------------------
	while()の条件が、本ファイルの実装テンプレートと異なる。

	lがNGケース、rがOKケースとなる二分探索とは違い、[l,r]の中から特定のデータを探す手法になる。
	よってwhile()から抜けるのは l=r となったとき。
	(本ファイルの実装テンプレートでは、l+1=r となったときにwhileから抜ける)
	*/

	// 二分探索の実装練習
	vector<int> a = {1,2,2,2,3,3};
	cout << my_lower_bound(a, 3) << endl;  // 4
	cout << my_lower_bound(a, 0) << endl;  // 0
	cout << my_lower_bound(a, 4) << endl;  // 6
	cout << my_upper_bound(a, 2) << endl;  // 4
	cout << my_upper_bound(a, 0) << endl;  // 0
	cout << my_upper_bound(a, 3) << endl;  // 6

	return 0;
}

// 二分探索の実装練習
// 昇順ソートされたa[]を先頭から見て行って、最初にnum以上になるa[]の要素番号を返す
// 該当する要素が無かった場合、末尾の要素番号+1を返す(つまり要素数と一致)
int my_lower_bound(vector<int> &a, int num)
{
	int l = -1;
	int r = a.size();
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(a[mid] >= num)
		{
			r = mid;
		}
		else
		{
			l = mid;
		}
	}
	return r;
}

// 二分探索の実装練習
// 昇順ソートされたa[]を先頭から見て行って、最初にnumより大きくなるa[]の要素番号を返す
// 該当する要素が無かった場合、末尾の要素番号+1を返す(つまり要素数と一致)
int my_upper_bound(vector<int> &a, int num)
{
	int l = -1;
	int r = a.size();
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(a[mid] > num)
		{
			r = mid;
		}
		else
		{
			l = mid;
		}
	}
	return r;
}

// 三分探索の実装メモ
// ＞たかだか一つしか極値のない関数fにおける極値を探索するアルゴリズム
// 参考：
//   https://qiita.com/ganyariya/items/1553ff2bf8d6d7789127
//   AtCoderフォルダのABC279-D.cpp
// 隣り合う評価値が等しくなるようなケースでは正しく求められない点に注意。
// (狭義単調減少/増加である必要がある。広義単調減少/増加はダメ)
// 参考：
//   https://twitter.com/e869120/status/1401537322731020293
//   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2021/0606_abc204
// 
// 与えられた式のグラフがどんなものか感覚を掴むには、
//   https://www.geogebra.org/graphing?lang=ja
// などのサイトを使うのが良さそう。
void ternary_search(void)
{
	// 以下、いずれも「最小値を求めたい場合」とする

	// l,rが整数の場合のケース
	{
	ll l, r;
	l = 0;
	r = INF64;
	auto func = [&](ll c) -> double
	{
		// 何らか計算する
		return 0.0;
	};
	while(r - l > 2)
	{
		ll c1, c2;
		c1 = (l*2+r)/3;  // 3分割し、l側の点
		c2 = (l+r*2)/3;  // 3分割し、r側の点
		if(func(c1) > func(c2)) l = c1;  // l～c1間には解は無いので、lを右へ更新
		else r = c2;  // c2～r間には解は無いので、rを左に更新
	}
	// ループを抜けた時点で r-l=2, つまり l,l+1,r という並びになっている
	// func(l+1)を出力するか、
	// 安全策でl～rのそれぞれのfunc()の値を計算し、その最小値を出力する。
	}

	// l,rが小数の場合のケース (実際の問題での採用実績はまだ無いので注意)
	{
	double l, r;
	l = 0.0;
	r = (double)INF64;
	auto func = [&](double c) -> double
	{
		// 何らか計算する
		return 0.0;
	};
	// l,rの差が極小になったら終了にするか、
	// 規定回数実施したら終了にするか、2通りの考え方がある
	// 以下は前者での実装
	while(true)
	{
		// l,rの差が極小になれば終了
		if(r-l < 0.0000001) break;  // 10^(-7)

		double c1, c2;
		c1 = (l*2+r)/3;
		c2 = (l+r*2)/3;
		if(func(c1) > func(c2)) l = c1;
		else r = c2;
	}
	// func(l),func(r)のどちらを出力してもよい。差はほぼ0なので。
	}
}
