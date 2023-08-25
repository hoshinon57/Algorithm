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

// ABC145 https://atcoder.jp/contests/abc145

/*
 * 動的計画法で解く。
 * 両側からDPする。以下を参考にした。
 *   公式解説の解法1 https://img.atcoder.jp/abc145/editorial.pdf
 *   https://sen-comp.hatenablog.com/entry/2019/11/17/003150
 * 
 * 料理iを除いた(N-1)品で(T-1)分まで食べて、ラストオーダーとして料理iを食べる、という考え方。
 * これを実現するには、
 *   左側からDPした結果dp1
 *   右側からDPした結果dp2
 * をまず計算しておき、各iについて
 *   料理iを食べず、料理1～(i-1)と料理(i+1)～Nを選んで、
 *   前者にj分、後者に(T-1-j)分費やすケースを考え、
 *   それらの美味しさの最大値に、料理iの美味しさを加算する
 * の最大値を求めればよい。
 */

int main(void)
{
	int i, j;
	int N, T; cin >> N >> T;
	vector<pair<int,int>> food(N);
	for(i = 0; i < N; i++)
	{
		cin >> food[i].first >> food[i].second;
	}

	// 左側からDP
	vector<vector<int>> dp1(N+1, vector<int>(T, 0));  // dp1[N+1][T]
	for(i = 1; i <= N; i++)  // 料理i
	{
		auto [a, b] = food[i-1];
		for(j = 0; j < T; j++)
		{
			dp1[i][j] = max(dp1[i][j], dp1[i-1][j]);
			if(j-a >= 0) dp1[i][j] = max(dp1[i][j], dp1[i-1][j-a] + b);
		}
	}

	// 右側からDP
	vector<vector<int>> dp2(N+1, vector<int>(T, 0));  // dp1[N+1][T]
	for(i = N-1; i >= 0; i--)
	{
		auto [a, b] = food[i];
		for(j = 0; j < T; j++)
		{
			dp2[i][j] = max(dp2[i][j], dp2[i+1][j]);
			if(j-a >= 0) dp2[i][j] = max(dp2[i][j], dp2[i+1][j-a] + b);
		}
	}

	int ans = 0;
	for(i = 0; i < N; i++)  // 料理iをラストオーダーとする
	{
		for(j = 0; j < T; j++)  // 料理(i-1)側にj分費やす。料理iをラストオーダーとしたいので、(T-1)分まで
		{
			int d1 = 0, d2 = 0;
			d1 = dp1[i][j];
			d2 = dp2[i+1][T-1-j];
			ans = max(ans, d1+d2+food[i].second);
		}
	}
	cout << ans << endl;

	return 0;
}
