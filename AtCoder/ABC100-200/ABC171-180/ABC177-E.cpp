#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC177 https://atcoder.jp/contests/abc177

/*
 * まずはnot coprime判定を行う。
 * これはAの各要素のGCDを計算し、1以外であればnot coprimeと判定できる。
 * 
 * 次にpairwise coprimeか setwise coprimeの判定を行う。
 * Aの各要素を素因数分解し、登場した素数を管理していく。
 * 登場した素数が被っていればsetwise coprimeと判定できる。
 * 最後まで被らなければ、pairwise coprimeとなる。
 * 
 * ※登場した素数の管理について、setとvector<bool>とで実行時間に大差は無かった。
 * 
 * 計算量はO(N√A)であり、N,Aともに最大10^6のため間に合わなそうに見えるが、
 * 10^6以下の素数は78000個ほどなので、「素数が被ったらsetwiseとして処理終了」で十分間に合うと推測した。
 * (鳩ノ巣原理的な)
 */

long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

// Nを素因数分解し、{素因数, 指数}のpairの形でprime_listに入れる
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
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	// not coprime判定
	int g = a[0];
	for(i = 1; i < N; i++)
	{
		g = gcd(g, a[i]);
	}
	if(g != 1)
	{
		cout << "not coprime" << endl;
		return 0;
	}

	set<int> d_list;  // これまでに登場した素因数の集合
	for(i = 0; i < N; i++)
	{
		vector<pair<ll,ll>> prime;
		prime_factorize(a[i], prime);
		for(auto &e : prime)
		{
			if(d_list.count(e.first) > 0)
			{
				cout << "setwise coprime" << endl;
				return 0;
			}
			d_list.insert(e.first);
		}
	}
	cout << "pairwise coprime" << endl;

	return 0;
}
