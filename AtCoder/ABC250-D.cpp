#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC250 https://atcoder.jp/contests/abc250

/*
 * まず素数を列挙する。
 * その際p<qとq^3があることより、10^6までで列挙を打ち切って良い。
 * 
 * 次にpを固定し、条件を満たすqを1つずつ探索していく。
 * 単純にp*q^3を計算するとlong longを超えることがあるため、
 * 除算を使ってまずN超えの判定を行う。
 * ⇒その後改めてp*q^3計算を行ったが、これは不要だった気がする…
 * 
 * [memo]
 * 解説では素数判定にエラトステネスの篩や
 * p,qの探索に尺取り法などの記載があるが、
 * どちらも使わずとも実行時間には余裕があった。
 */

// Nまでの素数一覧を、昇順にprime_listに入れる
void make_prime_list(int N, vector<ll> &prime_list)
{
	prime_list.push_back(2);
	for(ll i = 3; i <= N; i++)  // iに関して素数判定
	{
		bool isPrime = true;
		// i未満の各素数で割っていき、どれでも割り切れなかったらiは素数
		for(ll j = 0; j < prime_list.size(); j++)
		{
			if(prime_list[j]*prime_list[j] > i) break;  // √iを超える分は計算不要
			if(i % prime_list[j] == 0)
			{
				isPrime = false;
				break;
			}
		}
		if(isPrime) prime_list.push_back(i);
	}
}

int main(void)
{
	ll N;
	cin >> N;
	vector<ll> prime_list;
	make_prime_list(1000000, prime_list);  // 素数列挙 q^3があることから、10^6までの列挙で良い

	ll p,q;  // 問題文のp:prime_list[p], q:prime_list[q]
	ll count = 0;
	for(p = 0; p < prime_list.size()-1; p++)
	{
		for(q = p+1; q < prime_list.size(); q++)
		{
			// 単純にp*q^3を計算するとlong longを超えることがあるため、
			// 除算を使ってまず判定しておく
			if(N/(prime_list[q]*prime_list[q]) < prime_list[p]*prime_list[q]) break;

			ll tmp = prime_list[p]*prime_list[q]*prime_list[q]*prime_list[q];
			if(tmp <= N)
			{
				count++;
			}
			else break;
		}
	}

	cout << count << endl;

	return 0;
}
