#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * bitDPで解く。
 * 
 * dp[S]を以下のように定義する。
 *   m = __builtin_popcount(S)とし、男性の先頭からm人まで使い、
 *   女性はSでbitが立っている部分の人を使うときの、
 *   有効なペアの総数
 * 
 * S=S1∪S2, ただしS2はbitが1つのみ立っているようなS1,S2を考えると、
 * mとS2でペアを組めるようなものについて、dp[S1]をdp[S]に加算していけばよい。
 * 
 * 計算量はO(N*2^N).
 */

int N;
vector<vector<int>> a;
vector<ll> dp;

ll func(int S)
{
	ll &d = dp[S];
	if(d != -1) return d;
	if(S == 0) return (d=1);  // ベースケース 使う人が0人は1通りとする

	int m = __builtin_popcount(S);  // 男性
	d = 0;
	for(int i = 0; i < N; i++)  // 女性
	{
		if(!isbiton(S, i)) continue;
		if(a[m-1][i] == 0) continue;
		// 男性mと女性iはペアを組める
		int S1 = S & (~(1<<i));
		d += func(S1);
		d %= MOD;
	}

	return d;
}

int main(void)
{
	int i, j;
	cin >> N;
	a.resize(N, vector<int>(N));
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++) cin >> a[i][j];
	}
	dp.resize(1<<N, -1);
	cout << func((1<<N)-1) << endl;

	return 0;
}
