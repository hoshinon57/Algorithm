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

// ABC278 https://atcoder.jp/contests/abc278

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

bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

int N;
const int MAX_N = 16;
bool dist[MAX_N][MAX_N];  // dist[i][j]:単語i->jが可能ならtrue
// dp[S][v]:
//   使用済みの単語ならbitが1, 未使用は0
//   最後に使った単語がv
//   の状態で手番が来たときに、勝てるなら1, 負けるなら2. (未計算は0)
int dp[1<<MAX_N][MAX_N];

int bitDP(int S, int v)
{
	if(dp[S][v] != 0) return dp[S][v];  // メモ化済み

	int i;
	bool win = false;
	for(i = 0; i < N; i++)  // i番目の単語
	{
		if(isbiton(S, i)) continue;  // 単語使用済み
		if(!dist[v][i]) continue;  // 単語v->iへは遷移できない
		ll nxtS = S;
		setbit(nxtS, i);
		if(bitDP(nxtS, i) == 2) win = true;  // 次の手番が負け、つまり自分が勝ち
	}
	int &d = dp[S][v];
	if(win) d = 1;
	else d = 2;
	return d;
}

int main(void)
{
	int i, j;
	cin >> N;
	vector<string> s(N); for(i = 0; i < N; i++) {cin >> s[i];}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // dist[i][j], 単語i->j
		{
			if(s[i].back() == s[j][0]) dist[i][j] = true;
			else dist[i][j] = false;
		}
	}

	bool win = false;
	for(i = 0; i < N; i++)  // 先手が各単語を使うケースを調べる
	{
		if(bitDP(1<<i, i) == 2) win = true;  // 後手が負ける単語が1つでもあれば、勝ち
	}
	if(win) cout << "First" << endl;
	else cout << "Second" << endl;

	return 0;
}
