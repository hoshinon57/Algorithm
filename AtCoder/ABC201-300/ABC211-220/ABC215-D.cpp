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

// ABC215 https://atcoder.jp/contests/abc215

/*
 * 素因数分解と、エラトステネスの篩の考え方で解く。
 * 
 * まず、各Aを素因数分解して一覧として保持しておく。これらは答からは除外される。
 * その後、各素数xについて、xの倍数も答から除外していく。
 * 
 * 最終的に残ったものが答。
 * 
 * ※ちなみに、公式解説と同じ方針だった。
 *   「この部分を行わないとTLEします」に引っかかってTLEしたのも同じだった。
 *   https://atcoder.jp/contests/abc215/editorial/2482
 *   https://twitter.com/kyopro_friends/status/1429077248833855488
 */

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
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<bool> check(100005, false);

	// 各Aについて素因数分解
	// これらは答から除外される
	for(i = 0; i < N; i++)
	{
		vector<pair<ll,ll>> p_list;
		prime_factorize(a[i], p_list);
		for(auto &e : p_list)
		{
			check[e.first] = true;
		}
	}

	// 素因数の倍数も、答から除外される
	for(i = 1; i <= M; i++)
	{
		if(!check[i]) continue;
		for(j = i; j <= M; j+=i)
		{
			check[j] = true;
		}
	}

	// 除外されなかったものが答
	int cnt = 0;
	for(i = 1; i <= M; i++)
	{
		if(!check[i]) cnt++;
	}
	cout << cnt << endl;
	for(i = 1; i <= M; i++)
	{
		if(!check[i]) cout << i << endl;
	}

	return 0;
}
