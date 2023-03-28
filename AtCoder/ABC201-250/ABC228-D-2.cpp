#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC228 https://atcoder.jp/contests/abc228

/*
 * A[x]!=-1であるようなxの集合を管理する。これをsとする。
 * t=1のクエリについて、s.lower_bound(x)にて代入すべきA[]の要素番号をO(logN)で求めることができる。
 * その後、sからは要素を削除する。
 * 
 * A[]の末尾を越えて先頭に戻るケースもあるので、
 * s.lower_bound(x)で見つからなかった場合は、再度s.lower_bound(0)で先頭から探索する。
 */

int main(void)
{
	ll i;
	const ll N = 1 << 20;
	vector<ll> a(N, -1);
	set<ll> s;  // A[x]!=-1であるようなxの集合
	for(i = 0; i < N; i++)
	{
		s.insert(i);
	}

	ll Q;
	cin >> Q;
	while(Q > 0)
	{
		ll t, x;
		cin >> t >> x;
		if(t == 1)
		{
			ll idx = x%N;
			auto itr = s.lower_bound(idx);
			if(itr != s.end())
			{
				a[*itr] = x;
				s.erase(itr);
			}
			else
			{
				// 見つからなかったので、先頭から再探索
				// 問題の制約より、今度は見つかることは保証される
				itr = s.lower_bound(0);
				a[*itr] = x;
				s.erase(itr);
			}
		}
		else
		{
			cout << a[x%N] << endl;
		}
		Q--;
	}

	return 0;
}
