#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 二分探索の実装テンプレートやメモ
// 実装テンプレートはmain()を参照。

/*
 * [関連する問題]
 * ABC265-D
 * ABC257-D 個人的に良問
 * ABC246-D
 * ABC269-E while()の条件が実装テンプレートと異なる
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
