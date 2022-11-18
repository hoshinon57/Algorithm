#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 10000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題69
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc084/tasks/abc084_d

/*
 * 素数列挙と累積和で解く。
 * 
 * まずエラトステネスの篩を用いて、10^5までの素数を列挙する。
 * 
 * 次に以下の累積和を計算する。
 *   sum_2017[i+1]:i以下の"2017に似た数"の総数
 * iが"2017に似た数"のとき、sum_2017[i+1]=sum_2017[i]+1 にて、
 * そうでないときは +1 をしない形で計算できる。
 * 
 * 累積和の区間計算は半開区間の[l,r)に対して sum[r]-sum[l] となるため、
 * 入力[l,r]に対して [l,r+1) に置き換えて計算すればよい。
 */

const int N_MAX = 100000;

// エラトステネスの篩
// n以下の素数を列挙してprime_listに入れる
void make_prime_list(int n, set<int> &prime_list)
{
	int i, j;
	vector<bool> isPrime(n, true);

	for(i = 2; i <= n; i++)
	{
		if(!isPrime[i]) continue;
		prime_list.insert(i);  // iを素数に追加
		for(j = i+i; j <= n; j += i)  // iの倍数は素数ではない
		{
			isPrime[j] = false;
		}
	}
}

int main(void)
{
	int i;

	// N_MAX(=10^5)までの素数一覧を列挙する
	set<int> prime_list;
	make_prime_list(N_MAX, prime_list);

	vector<int> sum_2017(N_MAX+1);  // sum_2017[i+1]:i以下の"2017に似た数"の総数(累積和)
	for(i = 0; i <= N_MAX; i++)
	{
		if(prime_list.count(i) > 0 && prime_list.count((i+1)/2) > 0)
		{
			// iは2017に似た数
			sum_2017[i+1] = sum_2017[i] + 1;
		}
		else
		{
			sum_2017[i+1] = sum_2017[i];
		}
	}

	int Q;
	cin >> Q;
	for(i = 0; i < Q; i++)
	{
		int l, r;
		cin >> l >> r;
		// 累積和の区間計算は半開区間の[l,r)に対して sum[r]-sum[l] となる
		// 入力は[l,r]なので、[l,r+1)で計算する
		cout << sum_2017[r+1] - sum_2017[l] << endl;
	}

	return 0;
}
