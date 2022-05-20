#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;

// ABC241 https://atcoder.jp/contests/abc241

/*
 * multisetを用いて要素を追加していく。
 * "2 x k"のとき、upper_bound()を用いた後にイテレータをk回動かせばよい。
 * "3 x k"のとき、lower_bound()を用いた後にイテレータを(k-1)回動かせばよい。
 * 
 * イテレータの範囲外チェックを楽にするため、事前にxの範囲外である値を5個登録しておく。
 * 5は 1<=k<=5 の条件より。
 */
int main(void)
{
	ll i;
	ll Q;
	cin >> Q;
	multiset<ll> a;
	for(i = 0; i < 5; i++)  // 番兵としてxの範囲を超えるものを、上下それぞれ5つ入れておく
	{
		a.insert(0);
		a.insert(INF64);
	}

	for(i = 0; i < Q; i++)
	{
		int kind;
		cin >> kind;
		if(kind == 1)
		{
			ll x;
			cin >> x;
			a.insert(x);
		}
		else if(kind == 2)
		{
			ll x, k;
			cin >> x >> k;
			auto itr = a.upper_bound(x);
			for(int j = 0; j < k; j++)
			{
				itr--;
			}
			if(*itr != 0)
			{
				cout << *itr << endl;
			}
			else
			{
				cout << -1 << endl;
			}
		}
		else  // kind==3
		{
			ll x, k;
			cin >> x >> k;
			auto itr = a.lower_bound(x);
			for(int j = 0; j < k-1; j++)
			{
				itr++;
			}
			if(*itr != INF64)
			{
				cout << *itr << endl;
			}
			else
			{
				cout << -1 << endl;
			}
		}
	}

	return 0;
}
