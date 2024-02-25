#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC342 https://atcoder.jp/contests/abc342

/*
 * Ai*Ajが平方数になる条件を考えてみる。
 * 例えばAi=12=(2^2)*(3^1)のとき、Aj=(3^1)*(X^2)という形なら条件を満たす。
 * 同様にAi=2160=(2^4)*(3^3)*(5^1)のとき、Aj=(3^1)*(5^1)*(X^2)ならOK.
 * 
 * つまりAiを素因数分解したときに、指数が奇数である素因数をかけた値をDiとすると、
 * Di=DjとなるAjが条件を満たす。
 * よってDiの登場回数をカウントしていけば良い。
 * 
 * Ai=0だけ特殊で、相手は何でも条件を満たす。
 * 0だけ別処理とし、0の登場回数をカウントして求めていった。
 * 
 * Xの素因数分解はO(√N)なので、全体の計算量はO(N*√MAXA)となり、9*10^7ほど。
 * 結構ギリギリかなーと思いきや113msで、だいぶ余裕があった。
 * 
 * [ACまでの思考の流れ]
 * ・ABC254-D(Together Square)を思い出した。
 * ・Ai*Ajが平方数になる条件を、色々紙に書きながら考えた。
 *   Aiについて「平方数になるための端数」的な値を求めて、それが同じAjなら条件を満たすっぽい。
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
	int i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end());  // Ai=0に対する処理をやりやすくするため、ソートしておく

	ll ans = 0;
	ll zero_cnt = 0;  // Ai=0の数
	vector<ll> cnt(200000+5);  // cnt[i]:(指数が奇数である素因数の積)=iとなるAの登場回数
	for(i = 0; i < N; i++)
	{
		if(a[i] == 0)
		{
			ans += zero_cnt;
			zero_cnt++;
		}
		else
		{
			vector<pair<ll,ll>> plist;
			prime_factorize(a[i], plist);
			ll tmp = 1;
			for(auto [x, e] : plist)  // x^e
			{
				if(e%2 == 1)  // 指数が奇数のものをかけていく」
				{
					tmp *= x;
				}
			}
			ans += cnt[tmp];
			cnt[tmp]++;
			ans += zero_cnt;
		}
	}
	cout << ans << endl;

	return 0;
}
