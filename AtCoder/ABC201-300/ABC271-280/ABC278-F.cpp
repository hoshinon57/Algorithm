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

// ABC278 https://atcoder.jp/contests/abc278

// 先頭の解説はそのままで、ソースコードを2025/8に再解きしたものに置き換えた。
// 元のソースコードは呼び出し元で初回の単語を全探索していたが、
// 再帰関数内で「初回か否か」を判定するようにして、呼び出し元では1回にした。
// 
// 先頭の解説はそのまま。細かい実装は異なるので注意だが、おおよその方針は同じ。

/*
 * bitDP,メモ化再帰で解く。
 * 
 * dp[S][v]を以下のように定義する。
 *   使用済みの単語ならbitが1, 未使用は0
 *   最後に使った単語がv
 *   の状態で手番が来たときに、勝てるなら1, 負けるなら2. (未計算は0)
 * またdist[i][j]を、単語i->jで繋げられるならtrueとする。
 * 
 * dp[S][v]は、
 *   未使用の単語で単語vから繋げられる単語xで、
 *   dp[S∪x][x]=2 となるような単語が1つでもあれば1, 1つも無ければ2.
 * となる。
 * dp[S][v]から見てdp[S∪x][x]は相手であり、これが2ということは自分が勝つという考え方。
 * 相手が負ける遷移が1つでもあれば、自分はそれを選べばよい。
 * 
 * 先手は初期状態にてどの単語も選べるので、
 * 各単語を選んだときに後手が負けるケースが1つでもあれば、先手の勝ち。1つも無ければ後手の勝ち。
 */

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

ll N;
vector<string> s;
vector<vector<ll>> dp;  // 勝ち1, 負け0, 未探索-1

// dp[b][pr]を返す
int f(ll b, ll pr)
{
	auto &d = dp[b][pr];
	if(d != -1) return d;

	bool first = false;
	if(__builtin_popcountll(b) == N) first = true;  // 初回

	ll i;
	bool win = false;
	for(i = 0; i < N; i++)
	{
		// Siが使用済みならスルー
		// 「初回ではない」 かつ Spr -> Si が繋がらないならスルー
		if(!isbiton(b, i)) continue;
		if(!first && s[pr].back() != s[i][0]) continue;
		ll bb = b;
		unbit(bb, i);
		if(!f(bb, i)) win = true;  // 1つでも負けパターンがあれば、勝ち
	}
	if(win) d = 1;
	else d = 0;

	return d;
}

int main(void)
{
	ll i;
	cin >> N;
	s.resize(N);
	dp.resize(1LL<<N, vector<ll>(N, -1));
	for(i = 0; i < N; i++)
	{
		cin >> s[i];
	}
	auto ans = f((1LL<<N)-1, 0);
	if(ans == 1) cout << "First" << endl;
	else cout << "Second" << endl;

	return 0;
}
