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

// ABC403 https://atcoder.jp/contests/abc403

/*
 * メモ化再帰で解く。
 * 
 * dp[i][0]: 値がiになる式について、最後の(最も外側の)計算が'+'であるもの。
 * dp[i][1]: 値がiになる式について、最後の(最も外側の)計算が'*'であるもの。
 * とする。
 * dp[N][*]の短い方が答。
 * 
 * 加算ならiと(v-i)を、
 * 乗算ならiとv/iを組み合わせる。
 * 
 * '+'の次に'*'で結合する場合、()を外側に付与する必要がある。
 * 
 * メモ化のとき、「文字列が !="" ならメモ済み」とした場合は実行時間がだいぶ遅かった。(TLEはしなかったが)
 * size()!=0で判定すれば問題なかった。
**/

// dp[N+1][2]
vector<vector<string>> dp;
vector<bool> memo;

// dp[v][0],dp[v][1]を計算する (値は返さない、呼び出し元で別途dp[]にアクセスする)
void f(int v)
{
	string &d0 = dp[v][0];
	string &d1 = dp[v][1];
//	if(d != "") return d;  // なぜかこれ遅い
//	if(d0.size() != 0) return;  // これなら問題なし
	if(memo[v]) return;
	memo[v] = true;

	// chmin(s,t)的な
	auto upd = [](string &s, string &t) -> void
	{
		if(s == "") s = t;
		else
		{
			if(s.size() > t.size()) s = t;
		}
	};
	// s,tで(長さがあり)短い方を返す
	auto cmp = [](string &s, string &t) -> string
	{
		if(s == "") return t;
		if(t == "") return s;
		if(s.size() <= t.size()) return s;
		else return t;
	};

	if(v == 1)		d0 = d1 = "1";
	if(v == 11)		d0 = d1 = "11";
	if(v == 111)	d0 = d1 = "111";
	if(v == 1111)	d0 = d1 = "1111";

	// add
	int i;
	string ans;
	for(i = 1; i < v; i++)  // i+(val-i) = val
	{
		f(i);
		string tmp0 = dp[i][0];
		string tmp1 = dp[i][1];
		ans = cmp(tmp0, tmp1);  // 短い方
		ans += '+';
		f(v-i);
		tmp0 = dp[v-i][0];
		tmp1 = dp[v-i][1];
		ans += cmp(tmp0, tmp1);

		upd(d0, ans);
	}

	// mul
	ans = "";
	for(i = 2; i < v; i++)
	{
		if(v%i != 0) continue;
		f(i);
		string tmp0 = '(' + dp[i][0] + ')';  // 乗算の場合、直前の計算が加算なら()で囲む
		string tmp1 = dp[i][1];
		ans = cmp(tmp0, tmp1);  // 短い方
		ans += '*';
		f(v/i);
		tmp0 = '(' + dp[v/i][0] + ')';
		tmp1 = dp[v/i][1];
		ans += cmp(tmp0, tmp1);

		upd(d1, ans);
	}
}

int main(void)
{
	int N; cin >> N;
	dp.resize(N+1, vector<string>(2));
	memo.resize(N+1, false);

	f(N);
	string ans0 = dp[N][0];
	string ans1 = dp[N][1];
	string ans;
	if(ans0 == "") ans = ans1;
	else if(ans1 == "") ans = ans0;
	else
	{
		if(ans0.size() < ans1.size()) ans = ans0;
		else ans = ans1;
	}
	cout << ans << endl;

	return 0;
}
