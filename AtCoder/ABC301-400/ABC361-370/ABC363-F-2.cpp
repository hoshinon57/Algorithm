#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC363 https://atcoder.jp/contests/abc363

/*
 * snuke氏, evima氏, 公式解説(Nyaan氏)らの実装を参考にした内容。
 * 再帰関数にて、メモ化再帰して文字列を返すようにした。
 * ABC363-F.cppに比べて実装がシンプルになった。
 *   https://atcoder.jp/contests/abc363/editorial/10441
 *   https://atcoder.jp/contests/abc363/submissions/55750516
 *   https://atcoder.jp/contests/abc363/submissions/55827719
 * 
 * いわゆるオーソドックスなメモ化再帰にしているが、snuke氏の実装を見ると、
 * 「再度同じ値で呼ばれた時点で、それは最終的に回文にできない」
 * となるので、2回目呼ばれたら-1を返すようにしている。なるほど…
 */

vector<ll> divi;

bool valid_zero(ll n);

// Nの約数を列挙してdivisor[]に入れる
// 参考：https://qiita.com/drken/items/a14e9af0ca2d857dad23
// 本問(ABC363-F)用にいくつか手直ししている
void func_divisor(long long N, vector<long long> &divisor)
{
//	for(long long i = 1; i*i <= N; i++)
	for(long long i = 2; i*i <= N; i++)  // i=1は不要
	{
//		if( N%i == 0 )  // iで割り切れるなら約数
		if( N%i == 0 && valid_zero(i))
		{
			divisor.push_back(i);
			if( i*i != N )
			{
				// divisor.push_back(N/i);  // √Nまでの約数を求める
			}
		}
	}

	// ソート有無は必要に応じて
	sort(divisor.begin(), divisor.end());

	return;
}

// nを反転させた値を返す
ll num_rev(ll n)
{
	auto s = to_string(n);
	reverse(s.begin(), s.end());
	return stoll(s);
}

// 数値に0を含んでいなければtrue
bool valid_zero(ll n)
{
	auto s = to_string(n);
	return (count(s.begin(), s.end(), '0') == 0);
}

map<ll, string> memo;
string dfs(ll n)
{
	if(memo.count(n) > 0) return memo[n];
	string &s = memo[n];
	if(n == num_rev(n) && valid_zero(n))
	{
		s = to_string(n);  // 回文値なら採用して終了
		return s;
	}

	for(auto &e : divi)
	{
		ll reve = num_rev(e);
		if(n%e != 0) continue;
		if((n/e)%reve != 0) continue;

		auto nxt = dfs(n/e/reve);
		if(nxt == "-1") continue;

		s = to_string(e) + "*" + nxt + "*" + to_string(reve);
		return s;
	}

	return (s = "-1");
}

int main(void)
{
	ll N; cin >> N;
	func_divisor(N, divi);
	cout << dfs(N) << endl;

	return 0;
}
