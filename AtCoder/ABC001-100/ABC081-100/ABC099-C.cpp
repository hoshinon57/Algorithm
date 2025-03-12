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

// ABC099 https://atcoder.jp/contests/abc099

/*
 * メモ化再帰で解く。
 * dp[n]を「n円を引き出すのに必要な操作回数の最小値」とする。
 * dp[n]は dp[n-1],dp[n-6],dp[n-6^2],...,dp[n-9],dp[n-9^2],... の最小値+1となる。
 * 6のべき乗はすぐにNを超えるので、遷移数も問題なし。
 * 
 * ※探索にて-1のケースを先頭に置くと、ローカル環境ではサンプル3(N=44852)で実行時に落ちた。AtCoder上ではAC.
 *   先頭に1があるので、1ずつ減って再帰していくケースでスタックを使い切ったのかな。
 *   降順ソートすると大きい方から減らしていくので、ローカル環境でもACした。
**/

vector<int> dp;
vector<int> li;

// dp[n]を返す
int func(int n)
{
	if(n == 0) return 0;
	if(n < 0) return INF32;
	int &d = dp[n];
	if(d != -1) return d;  // メモ済み

	d = INF32;
	for(auto &e : li)
	{
		chmin(d, func(n-e)+1);
	}

	return d;
}

int main(void)
{
	int i;
	int N; cin >> N;
	dp.resize(N+1, -1);

	li.push_back(1);
	i = 6;
	while(i <= N)
	{
		li.push_back(i);
		i *= 6;
	}
	i = 9;
	while(i <= N)
	{
		li.push_back(i);
		i *= 9;
	}
	// 以下が無いと、ローカル環境ではサンプル3(N=44852)で実行時に落ちた。AtCoder上ではAC.
	// 先頭に1があるので、1ずつ減って再帰していくケースでスタックを使い切ったのかな。
	// 降順ソートすると大きい方から減らしていくので、ローカル環境でもACした。
	sort(li.begin(), li.end(), greater<int>());

	cout << func(N) << endl;

	return 0;
}
