#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題6.3
// 第7回 日本情報オリンピック 本選 問3「ダーツ」より
// https://www.ioi-jp.org/joi/2007/2008-ho-prob_and_sol/index.html
// https://www.ioi-jp.org/joi/2007/2008-ho-prob_and_sol/2008-ho.pdf

/*
 * 二分探索で解く。
 * (練習として自分で二分探索を実装してみた)
 * 
 * まず、a[]を2つ選んだ和の一覧を作っておき、a_2[]とする。(昇順ソートもしておく)
 * a_2[]から2つ選んだ和が、「4個選んだ総和」となる。
 * よってa_2[]の各要素に対して(eとする)、
 *   ・a_2[]の中から、eとの和がMを超える最初の要素を取得する。
 *   ・その1つ前の要素はeとの和はMを超えない。この値を求める。
 * 上記値の最大値が答となる。
 * 
 * 「重複も許して4個選ぶ」を簡単に実現するためには、a[]に0も入れておけば良い。
 */

// 昇順ソートされたa[]を先頭から見ていって、最初にnumより大きくなる要素番号を返す
// 該当する要素が無かった場合、末尾の要素番号+1を返す(つまり配列の要素数を返す)
int my_upper_bound(vector<int> &a, int num)
{
	int l = -1;
	int r = a.size();
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(a[mid] > num) r = mid;
		else l = mid;
	}
	return r;
}

int main(void)
{
	int i, j;
	int N, M;
	cin >> N >> M;
	vector<int> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	// ここまで入力

	a.push_back(0);  // 0を入れることで、「重複も許して4個選ぶ」を簡単に実現する
	vector<int> a_2;  // a[]どうしの和の一覧
	for(i = 0; i < (int)a.size(); i++)
	{
		for(j = 0; j < (int)a.size(); j++)
		{
			// 2個足した時点でM以下の場合のみ、追加
			// (この時点でMを超えるなら使えないので追加しない)
			if(a[i]+a[j] <= M)  a_2.push_back(a[i]+a[j]);
		}
	}
	sort(a_2.begin(), a_2.end());

	int answer = 0;
	for(auto &e : a_2)
	{
#if 0  // 通常のSTLライブラリを使う場合は、こちらを有効化
		// e+(*itr)はMより大きくなり、e+(itrの1つ前)はM以下となる
		auto itr = upper_bound(a_2.begin(), a_2.end(), M-e);

		// a_2には0を入れていること、またMより大きい要素は入れていないことから、
		// itr--しても範囲外にならないことは保証できる
		itr--;
		answer = max(answer, *itr+e);
#else  // 練習用に実装した二分探索を使う場合は、こちらを有効化
		// 処理の内容については、#if true側のコメントを参照
		int a_2_no = my_upper_bound(a_2, M-e);
		a_2_no--;
		answer = max(answer, a_2[a_2_no]+e);
#endif
	}
	cout << answer << endl;

	return 0;
}
