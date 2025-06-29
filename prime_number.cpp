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

/*
 * [memo]エラトステネスの篩を用いた高速素因数分解について
 * 1以上N以下の全整数を素因数分解する場合、愚直にやると O(N√N) かかる。(整数1個辺り√N, が全部でN個)
 * これをO(NlogN)で求める方法。
 * けんちょん氏の以下URL, 4-1章を参照。
 * https://qiita.com/drken/items/3beb679e54266f20ab63
 * これを用いてABC368-Fを高速に解くことができた。
 * https://atcoder.jp/contests/abc368/submissions/59467484
 */

// 素数列挙(エラトステネスの篩)
// N以下の素数を列挙してprime_listに入れる
// 計算量はO(NloglogN)とのこと。以下2つの記事を参考。
//   https://algo-method.com/descriptions/64
//   https://manabitimes.jp/math/992
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

// 区間の素数列挙(エラトステネスの区間篩)
// 区間[L,R]の素数を列挙してprime_listに入れる
// L,R: 1<=L<=R よって[L,R]=[1,1]は制約として合法(答は0個)
// rootR:
//   √R"以上"の値 1以上√R以下の素数を列挙する際に、配列確保に用いる
//   kth_root_integer()を用いるなり、制約から決め打ちするなり
// Rが10^14ぐらいまで、またR-Lが10^7ぐらいまで(Rに対して区間が狭い的な)場合に有効
// 計算量は M=max(R-L,√R) としてO(MloglogM)とか何とか。
//   https://algo-method.com/tasks/332/editorial
//   https://atcoder.jp/contests/abc412/editorial/13387
// [verify]アルゴリズムと数学 No.97
void make_prime_list_LR(long long L, long long R, long long rootR, vector<long long> &plist) {
	long long i, j;
	long long dif = R-L;
	vector<bool> isp1(rootR+1, true);  // 1～√Rまでの素数列挙用
	vector<bool> isp2(dif+1, true);  // isp2[i-L]: iが素数ならtrue i-Lで参照する
	for(i = 2; i*i <= R; i++) {
		if(!isp1[i]) continue;
		for(j = i*2; j*j <= R; j+=i) {
			isp1[j] = false;
		}

		long long st = ((L+i-1)/i)*i;  // L以上でiの倍数のうち最小値
		for(j = st; j <= R; j+=i) {
			if(j == i) continue;  // コーナーケース的な このときjは√R以下で列挙された素数そのもの
			isp2[j-L] = false;
		}
	}
	for(i = max(L,2LL); i <= R; i++) {  // max(L,2) 1は素数ではない
		if(isp2[i-L]) plist.push_back(i);
	}
}

// Nを素因数分解し、{素因数, 指数}のpairの形でprime_listに入れる
// 計算量はO(√N)
// 以下URLをほぼそのまま持ってきている
// https://qiita.com/drken/items/a14e9af0ca2d857dad23
// その他、参考URL：
//   https://algo-method.com/descriptions/119
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
