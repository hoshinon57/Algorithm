#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 素数判定、素数列挙、素因数分解

// Nが素数かどうか判定
bool isPrime(long long N)
{
	if(N < 2) return false;

	// √Nを上限とすれば十分
	for(long long i = 2; i*i <= N; i++)
	{
		if(N%i == 0) return false;
	}

	return true;
}

// 素数列挙(エラトステネスの篩)
// N以下の素数を列挙してprime_listに入れる
void make_prime_list(long long N, vector<long long> &prime_list)
{
	vector<bool> isPrime(N+1, true);
	isPrime[0] = isPrime[1] = false;
	for(long long i = 2; i <= N; i++)
	{
		if(!isPrime[i]) continue;
		// iは素数
		prime_list.push_back(i);
		// iの倍数を素数から外す
		for(long long j = i+i; j <= N; j += i)
		{
			isPrime[j] = false;
		}
	}
	return;
}

// Nを素因数分解し、{素因数, 指数}のpairの形でprime_listに入れる
// 以下URLをほぼそのまま持ってきている
// https://qiita.com/drken/items/a14e9af0ca2d857dad23
void prime_factorize(long long N, vector<pair<long long, long long>> &prime_list)
{
	long long i;
	// 入力Nに対して、2から√Nの各整数で割れるだけ割っていく
	for(i = 2; i*i <= N; i++)
	{
		if(N%i != 0) continue;  // iが素因数でなければ次へ

		long long ex = 0;  // 指数
		while(N%i == 0)  // 割れる限り割る
		{
			N /= i;
			ex++;
		}
		prime_list.push_back({i, ex});  // iで割り切ったら結果を保存
	}
	if(N != 1)  // 割り切った後に1以外の値で残っていれば、それも素因数
	{
		prime_list.push_back({N, 1});
	}

	return;
}

int main(void)
{
	long long N;
	cin >> N;
#if 0
	if(isPrime(N))
	{
		cout << N << ":is prime number." << endl;
	}
	else
	{
		cout << N << ":is NOT prime number." << endl;
	}
#endif
	// 素数列挙
	vector<long long> prime_list;
	make_prime_list(N, prime_list);
	for(auto &&a : prime_list)
	{
		cout << a << " ";
	}
	cout << endl;

	// 素因数分解
	cin >> N;
	vector<pair<long long, long long>> prime_factor_list;
	prime_factorize(N, prime_factor_list);
	cout << N << ":";
	for(auto &e : prime_factor_list)
	{
		for(int i = 0; i < e.second; i++) cout << " " << e.first;
	}
	cout << endl;

	return 0;
}
