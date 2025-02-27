#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC114 https://atcoder.jp/contests/abc114

/*
 * 考え方はどの解説とも同じだが、実装方針はhamayanhamayan氏のが近かった。
 *   https://blog.hamayanhamayan.com/entry/2018/12/02/233028
 * 
 * ある数を素因数分解したときにp^e*p^e*p^e*... となったとすると、
 * その数の約数の個数は(p1+1)*(p2+1)*(p3+1)*... となる。
 * 
 * よってnum[p]を
 *   値N!を素因数分解したときに、p^eのeを保持
 * として定義すると、これは1～Nの各値について素因数分解した結果を加算していけば求められる。
 * 
 * 75は 3*25, 15*5, 3*5*5, 75 の4パターンの積で表せるので、
 * それぞれについてnum[]を用いて条件を満たすp^eがあるかを調べていく。
**/

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
	ll i, j, k;
	ll N; cin >> N;
	vector<ll> num(N+1);  // num[p]:値N!を素因数分解したときに、p^eのeを保持
	for(i = 1; i <= N; i++)
	{
		vector<pair<ll,ll>> pl;
		prime_factorize(i, pl);
		for(auto [p, e] : pl)
		{
			num[p] += e;
		}
	}

	int ans = 0;
	// 3*25 と 15*5
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= N; j++)
		{
			if(i == j) continue;
			if(num[i] >= 2 && num[j] >= 24)
			{
				ans++;
			}
			if(num[i] >= 14 && num[j] >= 4)
			{
				ans++;
			}
		}		
	}
	// 3*5*5
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= N; j++)
		{
			if(i == j) continue;
			// ここだけ、j,kがひっくり返ったやつをカウント除外したいので、k=j+1スタート
//			for(k = 1; k <= N; k++)
			for(k = j+1; k <= N; k++)
			{
				if(i == k || j == k) continue;
				if(num[i] >= 2 && num[j] >= 4 && num[k] >= 4)
				{
					ans++;
				}
			}
		}
	}
	// 75
	for(i = 1; i <= N; i++)
	{
		if(num[i] >= 74) ans++;
	}
	cout << ans << endl;

	return 0;
}
