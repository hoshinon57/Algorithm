#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC142 https://atcoder.jp/contests/abc142

/*
 * 公約数のうち素数を選んでいくのが最善っぽい。
 * 約数列挙にてA,Bの公約数を求め、それが素数であるものが条件を満たす。
 * 1はカウントされないため、出力時に+1してあげるのが手っ取り早い。
 * ※エラトステネスの篩は、計算量がO(NloglogN)かつA,Bの最大が10^12のため使えない
 * 
 * ※解説では、
 *   (1)約数列挙ではなく素因数分解で求める
 *   (2)最大公約数を素因数分解したときに、その個数が答
 *   などの手法があり、これらの方がより簡潔だった。
 */

// Nの約数を列挙してdivisor[]に入れる
// 参考：https://qiita.com/drken/items/a14e9af0ca2d857dad23
void func_divisor(long long N, vector<long long> &divisor)
{
	for(long long i = 1; i*i <= N; i++)
	{
		if( N%i == 0 )  // iで割り切れるなら約数
		{
			divisor.push_back(i);
			if( i*i != N )
			{
				// N=16, j=2のとき、8も約数になる。それを追加する。
				// (これを行うことで、約数列挙の計算量がO(N)からO(√N)になる)
				// ただしj*j=N(例：4*4=16)といった場合を
				// 追加してしまうと重複してしまうので、それは省く。
				divisor.push_back(N/i);
			}
		}
	}

	// ソート有無は必要に応じて
	sort(divisor.begin(), divisor.end());

	return;
}

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

int main(void)
{
	ll A, B; cin >> A >> B;
	vector<ll> a_div_list;  // Aの約数一覧
	func_divisor(A, a_div_list);

	ll ans = 0;
	// AとBの公約数で、素数のものが選べる
	for(auto &e : a_div_list)
	{
		if(B%e == 0 && isPrime(e)) ans++;
	}
	cout << ans+1 << endl;  // +1は1の分

	return 0;
}
