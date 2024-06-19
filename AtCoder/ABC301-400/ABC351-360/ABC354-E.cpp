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

// ABC354 https://atcoder.jp/contests/abc354

/*
 * bitDPで解く。割とシンプルな問題。
 * 
 * カード番号をbitに割り当て、まだ残っているカードを1, 無くなったカードを0にする。
 * カードの集合Sとし、dp[S]を「この状態から始めたときに、自分が勝つなら1, 相手が勝つなら0」と定義する。
 * これを再帰を使って解く。
 * 
 * Sから遷移できる全ての遷移先について、1つでも「相手の負け」になるものがあれば、
 * それを選べば良いので「自分の勝ち」となる。
 */

vector<int> dp;
vector<int> a, b;
int N;

// dp[S]を返す
// 自分の勝ちなら1, 相手の勝ちなら0
int func(int S)
{
	int &d = dp[S];
	if(d != -1) return d;

	int i, j;  // 1,2枚目に選ぶカード
	bool win = false;
	// 全ての遷移先をチェックし、「相手の負け」が1つでもあればそれを選んで自分の勝ちにできる
	for(i = 0; i < N; i++)
	{
		if(((S>>i)&1) == 0) continue;  // カードが残っていない
		for(j = i+1; j < N; j++)
		{
			if(((S>>j)&1) == 0) continue;  // カードが残っていない
			if(a[i]!=a[j] && b[i]!=b[j]) continue;
			int S2 = S & (~(1<<i)) & (~(1<<j));
			if(func(S2) == 0) win = true;  // 相手が負けのとき、自分の勝ち
		}
	}

	if(win) d = 1;
	else d = 0;

	return d;
}

int main(void)
{
	int i;
	cin >> N;
	dp.resize(1<<N, -1);
	a.resize(N);
	b.resize(N);
	for(i = 0; i < N; i++) cin >> a[i] >> b[i];

	string ans = "Aoki";
	if(func((1<<N)-1) == 1) ans = "Takahashi";
	cout << ans << endl;

	return 0;
}
