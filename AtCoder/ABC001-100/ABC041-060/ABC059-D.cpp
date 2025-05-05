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

// ABC059 https://atcoder.jp/contests/abc059

/*
 * 実験したら一目で分かったやつ。でも証明はできなかった。
 * https://drken1215.hatenablog.com/entry/2024/10/17/035413
 * を理解するのが良さそう。
 * 
 * ・負けパターンの局面からは、どう操作しても非負けパターンになる
 * ・非負けパターンの局面からは、負けパターンに遷移できる操作が必ずある
 * こういうことか。
**/

vector<vector<int>> dp;  // dp[x][y] 残り石がx,yのときから初めて、勝てるなら1

// 実験用コード
bool cal(ll x, ll y)
{
	if(dp[x][y] != -1)
	{
		return (dp[x][y] == 1);
	}
	int &d = dp[x][y];
	ll i;
	bool win = false;
	for(i = 2; i <= x; i+=2)
	{
		if(!cal(x-i, y+i/2)) win = true;  // 相手が負けるなら自分勝ち
	}
	for(i = 2; i <= y; i+=2)
	{
		if(!cal(x+i/2, y-i)) win = true;
	}
	if(win) d = 1;
	else d = 0;
	return win;
}

int main(void)
{
	ll X, Y; cin >> X >> Y;
	if(abs(X-Y)<=1) cout << "Brown" << endl;
	else cout << "Alice" << endl;
	return 0;

	// 以下、実験用コード
	dp.resize(X*2, vector<int>(Y*2, -1));
	cal(X, Y);

	ll i, j;
	for(i = 0; i < 50; i++)
	{
		for(j = 0; j < 50; j++)
		{
			cal(i, j);
			cout << dp[i][j] << "  ";
		}
		cout << endl;
	}

	return 0;
}
