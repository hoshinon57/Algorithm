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

// ABC204 https://atcoder.jp/contests/abc204

/*
 * 自力で解けず、解説を見た。
 * ※「Tの大きい方から順に、オーブンの使用時間が短い方へ入れていく」という嘘解法で実装してしまった。
 *   この場合、T={4,3,3,2,2}というケースでans=8を出してしまいNG. (ans=7が可能)
 *   大反省。
 * 
 * 動的計画法で解く。
 * dp[i][j]を以下のように定義する。
 *   品物iまで見て、  (i:1～N)
 *   部分和をjにできるかどうか  (j:0～ΣT)
 * 
 * 考え方は以下。
 * 料理を2つのオーブンに振り分けることを考える。
 * オーブンの使用時間をAとB, またS=ΣT とすると、A+B=S.
 * A>=Bとしてしまってよく、この場合min(A)が答となる。
 * すると B=S-A より、式変形で
 *   A>=S-A
 *   2A>=S
 *   A>=S/2
 * となる。
 * よってN品から好きに選んだときに、Tの和がS/2以上である最小値が答となる。
 * 
 * 計算量はO(NS).
 * Sの最大は10^5であるため、全体で10^7程度となり間に合う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・貪欲法を思いついたが、計算量を考えると制約的に余裕がある。
 *   この時点で怪しいと気づくべき。
 *   (怪しく感じたものの、反例が見つけられなかったのでそのまま進めてしまった)
 * ・貪欲でWAとなったときに、何としても反例を見つける。
 * ・「DPで解く」という概要を見ても、そこから方針が思いつかなかった。この点も反省。
 *   立式や式変形をしながら考えてみる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> t(N+1);
	int tot = 0;  // Tの総和
	for(i = 1; i <= N; i++) {cin >> t[i]; tot+=t[i];}

	vector<vector<bool>> dp(N+1, vector<bool>(tot+1, false));
	dp[0][0] = true;
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= tot; j++)
		{
			dp[i][j] = dp[i-1][j];  // 品物iを選ばない
			if(j-t[i] >= 0)  // 品物iを選ぶ
			{
				if(dp[i-1][j-t[i]]) dp[i][j] = true;
			}
		}
	}

	for(j = (tot+1)/2; j <= tot; j++)  // S/2以上のもので選べる和のうち、最小値が答
	{
		if(dp[N][j])
		{
			cout << j << endl;
			break;
		}
	}

	return 0;
}
