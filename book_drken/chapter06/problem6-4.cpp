#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題6.4
// POJ No.2456 "Aggressive cows" より
// http://poj.org/problem?id=2456

/*
 * 最小値の最大化問題。
 * 二分探索と貪欲法で解く。
 * 
 * まず、「2つの小屋の距離がx以上離れているように、小屋をM個選ぶことは可能か？」
 * という判定を考える。(これは貪欲法で解く)
 * このxの最大値が、求める答となる。
 * 
 * l:必ずM個選べる距離  r:必ずM個選べない距離 と定義し、二分探索で範囲を狭めていく。
 * l+1=rとなったときのlが、上記のxの最大値になる。
 */

// 昇順ソートされたa[]からM個選ぶときに、それぞれがnum以上離れているような選び方が可能か
bool check(vector<int> &a, int M, int num)
{
	// 貪欲法で、先頭から順に「前回選んだ要素との差がnum以上」になる要素を選んでいく
	int prev = -INF32;
	for(auto &e : a)
	{
		if(e-prev >= num)
		{
			M--;
			prev = e;
		}
	}

	if(M <= 0) return true;  // M個以上選べた
	else return false;
}

int main(void)
{
	int i;
	int N, M;
	cin >> N >> M;
	vector<int> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	// ここまで入力

	// 引用元は非ソート済みだが、本の問題文ではソート済み
	// sort(a.begin(), a.end());

	// l:必ずM個選べる距離  r:必ずM個選べない距離  とする
	int l = 0;
	int r = INF32;
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(a, M, mid))
		{
			l = mid;  // midでM個選べる -> lへ
		}
		else
		{
			r = mid;  // midでM個選べない -> rへ
		}
	}
	cout << l << endl;

	return 0;
}
