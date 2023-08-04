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

// ABC164 https://atcoder.jp/contests/abc164

/*
 * 最初に考えた方法。
 * N=|S|, M=2019として計算量はO(NM).
 * |S|の最大が2*10^5のため、NM=4*10^8にもなるが、TLEせず何とか通った。
 * C++であるのと、定数倍が軽いのがTLEしなかった理由か。
 * 
 * dp[i][j]を以下のように定義する。
 *   Sのi文字目までを使ったときに、
 *   その数値を2019で割った余りがjとなる
 *   数値の総数
 * ただしこれだとメモリ制限を超過するため、dp[j]のみの1次元とした。
 * DPを使いまわす形。
 * 
 * DPの遷移としては、配るDPにて以下のようになる。
 *   (i-1)桁目までの数値を2019で割った余りがjのときに、
 *   (j*10 + i桁目の値)%2019
 *   を次のDPに加算する
 * 
 * 各DPにおける[0]の値の総和が答。
 */

int main(void)
{
	int i, j;
	string s; cin >> s;
	int N = (int)s.size();
	vector<int> num(N);
	for(i = 0; i < N; i++)
	{
		num[i] = (int)(s[i]-'0');
	}
	vector<int> dp(2019);

	// 配るDP
	ll ans = 0;
	for(i = 0; i < N-1; i++)
	{
		vector<int> dp_new(2019);
		dp[num[i]]++;  // i桁目のみを使う場合
		// i桁目までを10倍してi+1桁目のnumを足す
		for(j = 0; j < 2019; j++)  // 2019で割った余りがj
		{
			int n = j*10 + num[i+1];
			n %= 2019;
			dp_new[n] += dp[j];
		}
		ans += dp_new[0];
		dp = dp_new;
	}

	cout << ans << endl;

	return 0;
}
