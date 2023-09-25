#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC308 https://atcoder.jp/contests/abc308

/*
 * Eについて全探索して解く。
 * yuto1115氏の公式解説が近い。
 *   https://atcoder.jp/contests/abc308/editorial/6708
 * 
 * i文字目に"E"があった場合、
 *   [0,i)文字目に A=k1 となる"M"の数
 *   [i+1,N)文字目に A=k2 となる"X"の数
 * をそれぞれ求めて、
 * mex(k1, A(i), k2) を答に加算していく。
 * k1, k2はそれぞれ0～2でループして探索する。
 * 
 * "M"の数と"X"の数は、事前に累積和を計算しておく。
 */

// a,b,cに含まれない最小の非負整数を返す
int mex(int a, int b, int c)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		if(a != i && b != i && c != i) break;
	}
	return i;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	string S; cin >> S;

	vector<vector<int>> M_sum(N+1, vector<int>(3, 0));  // M_sum[N+1][3]
	vector<vector<int>> X_sum(N+1, vector<int>(3, 0));  // M_sum[N+1][3]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < 3; j++)
		{
			M_sum[i+1][j] = M_sum[i][j];
			X_sum[i+1][j] = X_sum[i][j];
			if(S[i] == 'M' && a[i] == j) M_sum[i+1][j]++;
			if(S[i] == 'X' && a[i] == j) X_sum[i+1][j]++;
		}
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		if(S[i] != 'E') continue;
		int k1, k2;
		for(k1 = 0; k1 < 3; k1++)
		{
			for(k2 = 0; k2 < 3; k2++)
			{
				int k1_num = M_sum[i][k1];
				int k2_num = X_sum[N][k2] - X_sum[i+1][k2];
				ans += (ll)mex(k1, a[i], k2) * k1_num * k2_num;
			}
		}
	}
	cout << ans << endl;

	return 0;
}

