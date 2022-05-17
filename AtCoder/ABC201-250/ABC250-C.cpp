#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC250 https://atcoder.jp/contests/abc250

/*
 * 1-indexedで計算する。
 * 
 * 操作は「左からx(i)番目のボール」ではなく、「整数x(i)が書かれているボール」に対して行う。
 * よって操作対象のボールをいちいち探しているとTLE.
 * 
 * そのため、左からi番目のボールに書かれている数 a[i] に加えて、
 * 整数iが書かれているボールの場所 place[i] を一緒に管理する。
 * 操作ごとにa[]とplace[]の入れ替えを行っていけばよい。
 */
int main(void)
{
	// 1-indexed
	int i;
	int N, Q;
	cin >> N >> Q;
	vector<int> x(Q+1);  // x[1]～x[Q]
	for(i = 1; i<= Q; i++)
	{
		cin >> x[i];
	}
	vector<int> a(N+1);  // a[1]～a[N]  左からi番目のボールに書かれている数
	vector<int> place(N+1);  // place[1]～place[N]   place[3]なら、3が書かれたボールの場所(1-indexed)
	for(i = 1; i <= N; i++)
	{
		a[i] = i;
		place[i] = i;
	}

	/*
	以下はnot 右端の場合。右端なら+1を-1に置き換える。
	・a[]の交換
	  x[i]=3なら、まず3が書かれているボールの場所を探す -> place[3]
	  (place[3])番目なので、a[place[3]], a[place[3]+1]を交換する
	・place[]の交換
	  x[i]=3なら、3が書かれているボールの右のボールのplace[]を探す -> place[a[place[3]+1]]
	  place[3], place[a[place[3]+1]]を交換する
	*/
	for(i = 1; i <= Q; i++)
	{
		int tmp1, tmp2, tmp3, tmp4;  // 2回swap()するため、配列添字を事前に計算しておく
		if(place[x[i]] != N)
		{
			tmp1 = place[x[i]];
			tmp2 = place[x[i]]+1;
			tmp3 = x[i];
			tmp4 = a[place[x[i]]+1];
		}
		else  // 右端
		{
			tmp1 = place[x[i]];
			tmp2 = place[x[i]]-1;
			tmp3 = x[i];
			tmp4 = a[place[x[i]]-1];
		}
		swap(a[tmp1], a[tmp2]);
		swap(place[tmp3], place[tmp4]);
	}

	for(i = 1; i <= N; i++)
	{
		cout << a[i];
		if(i != N) cout << " ";
	}
	cout << endl;

	return 0;
}
