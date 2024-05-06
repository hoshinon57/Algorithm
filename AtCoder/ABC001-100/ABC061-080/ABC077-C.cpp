#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC077 https://atcoder.jp/contests/abc077

/*
 * 二分探索で解く。
 * 
 * A,B,Cを事前に昇順ソートしておく。
 * あるB(i)について条件を満たすAの数は、lower_bound()でB(i)以上になる値を探し、それより手前の要素数となる。
 * 同様に条件を満たすCの数は、upper_bound()でB(i)より大きくなる値を探し、そこ以降の要素数となる。
 * 上記2つの積が、B(i)に対して条件を満たす組み合わせとなる。
 * これをBについて全探索すればよい。
 * 
 * アルゴリズム本の章末問題6.2にて引用された問題であり、
 * 練習として自分で二分探索を実装してみた。
 */

// 昇順ソートされたa[]を先頭から見て行って、最初にnum以上になるa[]の要素番号を返す
// 該当する要素が無かった場合、末尾の要素番号+1を返す(つまり要素数と一致)
ll my_lower_bound(vector<ll> &a, ll num)
{
	// a=1,2,2,3,3 num=3 -> ret=3
	// a=1,2,2,3,3 num=0 -> ret=0
	// a=1,2,2,3,3 num=4 -> ret=5
	ll l = -1;
	ll r = a.size();
	while(r - l > 1)
	{
		ll mid = (r+l)/2;
		if(a[mid] >= num) r = mid;
		else l = mid;
	}
	return r;
}

// 昇順ソートされたa[]を先頭から見て行って、最初にnumより大きくなるa[]の要素番号を返す
// 該当する要素が無かった場合、末尾の要素番号+1を返す(つまり要素数と一致)
ll my_upper_bound(vector<ll> &a , ll num)
{
	// a=1,2,2,3,3 num=2 -> ret=3
	// a=1,2,2,3,3 num=0 -> ret=0
	// a=1,2,2,3,3 num=3 -> ret=5
	ll l = -1;
	ll r = a.size();
	while(r - l > 1)
	{
		ll mid = (r+l)/2;
		if(a[mid] > num) r = mid;
		else l = mid;
	}
	return r;
}

int main(void)
{
	ll i;
	ll N;
	cin >> N;
	vector<ll> a(N), b(N), c(N);
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < N; i++) cin >> b[i];
	for(i = 0; i < N; i++) cin >> c[i];
	// ここまで入力

	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	sort(c.begin(), c.end());

	// 3要素の大小関係なので、真ん中のbについて全探索する
	ll answer = 0;
	for(auto &e : b)
	{
#if 0  // 通常のSTLライブラリを使う場合は、こちらを有効化
		auto a_itr = lower_bound(a.begin(), a.end(), e);
		auto c_itr = upper_bound(c.begin(), c.end(), e);
		answer += (a_itr - a.begin()) * (c.end() - c_itr);
#else  // 練習用に実装した二分探索を使う場合は、こちらを有効化
		ll a_num = my_lower_bound(a, e);
		ll b_num = c.size() - my_upper_bound(c, e);
		answer += a_num * b_num;
#endif
	}
	cout << answer << endl;

	return 0;
}
