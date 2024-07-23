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

// ABC363 https://atcoder.jp/contests/abc363

/*
 * evima氏解説の「再帰で全探索」というフレーズだけは見て解いた。
 *   https://www.youtube.com/watch?v=nsa-yNV7Xbg
 *   https://atcoder.jp/contests/abc363/editorial/10441
 * 
 * 基本はNyaan氏の公式解説と同じ。
 * Nの約数を列挙し、条件を満たすものを左右から構築していく。
 * 約数の数は少ないので十分に間に合う。(N=10^12までなら最大6720個らしい)
 * 
 * 約数列挙は、持っているライブラリから以下を手直しした。
 * ・i=1は列挙しない
 * ・0が含まれる約数は列挙しない
 * ・√Nまで列挙する
 * 
 * 深さ優先探索では、まずnが回文値なら探索終了。
 * そうでない場合、約数eおよびその回文値で割り切れるなら、それを採用して再帰。
 * 
 * 出力は多少ごちゃごちゃするが上手いことやる。実装を参照。
 * 
 * [ACまでの思考の流れ]
 * ・約数の個数は少ない。[典型]
 */

vector<ll> divi, ans;

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

bool dfs(ll n)
{
	// nが回文値なら採用して終了
	if(n == num_rev(n) && valid_zero(n))
	{
		ans.push_back(n);
		return true;
	}

	for(auto &e : divi)  // 約数の列挙時、0が無いようにしている
	{
		// eとその回文値で割り切れる必要がある
		if(n%e != 0) continue;
		ll rev_e = num_rev(e);
		if((n/e)%rev_e != 0) continue;

		ans.push_back(e);
		if(dfs(n/e/rev_e)) return true;
		ans.pop_back();
	}

	return false;
}

int main(void)
{
	ll N; cin >> N;
	func_divisor(N, divi);

	if(!dfs(N))
	{
		cout << -1 << endl;
		return 0;
	}
	auto ans_rev = ans;
	ans.pop_back();  // 折り返し部分を削除
	reverse(ans_rev.begin(), ans_rev.end());
	string out;
	for(auto &e : ans) out += to_string(e) + '*';
	for(auto &e : ans_rev) out += to_string(num_rev(e)) + '*';
	out.pop_back();  // 末尾の*
	cout << out << endl;

	return 0;
}
