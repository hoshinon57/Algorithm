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

// 競プロ典型90問:58 https://atcoder.jp/contests/typical90/tasks/typical90_bf

/*
 * ダブリングで解いた。
 * db[k][i]を、「数値iの状態から、ボタンを2^k回押した後の値」として定義する。
 */

int main(void)
{
	int i, k;
	const int MOD = 1e5;
	const int LOG_K = 60;  // 10^18 < 2^60
	vector<vector<int>> db(LOG_K+1, vector<int>(MOD, 0));  // db[LOG_K+1][MOD]
	// db[0][*]を計算
	for(i = 0; i < MOD; i++)
	{
		int sum = 0;
		int j = i;
		while(j > 0)
		{
			sum += j%10;
			j /= 10;
		}
		db[0][i] = (i+sum)%MOD;
	}
	// db[k][*]を計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < MOD; i++)
		{
			db[k][i] = db[k-1][db[k-1][i]];
		}
	}

	ll N, K; cin >> N >> K;
	int ans = N;
	for(i = 0; i <= LOG_K; i++)
	{
		if((K>>i)&1)
		{
			ans = db[i][ans];
		}
	}
	cout << ans << endl;

	return 0;
}
