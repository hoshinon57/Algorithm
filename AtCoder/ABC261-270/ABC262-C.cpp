#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC262 https://atcoder.jp/contests/abc262

/*
 * 以下、1-indexedとする。
 * 
 * まず、どういう数列が条件を満たすかを考える。
 * (1)a=(X, X, 3, 4, 5) のとき
 *   (i,j)=(3,4),(3,5),(4,5) が条件を満たす。
 *   つまり、a(i)=iかつa(j)=jのペアであればよい。
 * (2)a=(2, 1, 5, X, 3) のとき
 *   (i,j)=(1,2),(3,5) が条件を満たす。
 *   つまり、a(a(i))=i となればよい。
 * 
 * あとは、i<jによる重複カウントに注意して実装する。
 * (1)は入力時にa(i)=iとなるリストを用意しておき、
 * a(j)=jとなるjの数をupper_bound()で求めた。
 * ※ただしこれはConbinationを用いてまとめて計算した方が良い。
 */
int main(void)
{
	// 1-indexed
	int i;
	int N;
	cin >> N;
	vector<int> a(N+1);  // a1, a2, a3, ..., aN
	vector<int> eq_list;  // i=a(i)となるiのリスト
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
		if(i == a[i])
		{
			eq_list.push_back(i);
		}
	}
	
	ll answer = 0;
	for(i = 1; i <= N; i++)  // iについて全探索
	{
		if(i == a[i])
		{
			// この場合、i<jについてj==a[j]となるjの数が答
			auto itr = upper_bound(eq_list.begin(), eq_list.end(), i);
			int tmp = eq_list.end() - itr;
			answer += tmp;
		}
		else
		{
			// この場合、a[a[i]]==iならOK
			// ただしi<jという制約により、i<a[i]であること
			if(i >= a[i]) continue;

			if(a[a[i]] == i)
			{
				answer++;
			}
		}
	}
	cout << answer << endl;

	return 0;
}
