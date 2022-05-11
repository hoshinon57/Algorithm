#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 素数判定、素数列挙

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
	vector<long long> prime_list;
	make_prime_list(N, prime_list);
	for(auto &&a : prime_list)
	{
		cout << a << " ";
	}
	cout << endl;

	return 0;
}
