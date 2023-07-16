#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC242 https://atcoder.jp/contests/abc242

/*
 * 動的計画法で解く。
 * 
 * 以下、1-indexedで考える。
 * 回文を考えるので、長さNの文字列Sについて 1～(N+1)/2 文字目まで見ればよい。
 * 例：長さ4なら1～2文字目まで、長さ5なら1～3文字目まで
 * 
 * 例として"CBCDDD"を考える。前述より1～3文字目まで考えればよい。
 * 1文字目にAorBを選ぶ場合、2文字目はA～Zの26種を何でも選ぶことができる。3文字目も同様。
 * 1文字目にCを選ぶ場合、以下のように分岐する。
 *   2文字目にAを選ぶ場合、3文字目は同様に26種から選べる。
 *   2文字目にBを選ぶ場合、3文字目は「AorBか、Cか」となる。
 * 
 * よってdp[i][j]を以下のように定義する。
 *   i文字目まで見たときに、  i:1～(N+1)/2
 *   j=0:それまでSと同じ文字列
 *   j=1:Sより辞書順で前に来る文字列
 *   の組み合わせの数
 * 
 * 遷移は以下のようになる。
 * dp[i][0]は常に1 (Sと同じ文字列なので)
 * dp[i][1]は、
 *   dp[i-1][0]から、i文字目がSより辞書順で前に来る文字を選ぶ場合の数(i文字目がCなら、A,Bの2通り)
 *   dp[i-1][1]を26倍したもの(i文字目はA～Zまで何でも選べるため)
 * の和となる。
 * 
 * 最後に、dp[*][0]で最後まで作った文字列からなる回文が、条件を満たすかを判定する必要がある。
 * 例えば"ABCDDD", "ABCAAA"からはどちらも"ABCCBA"になるが、
 * 前者は条件を満たし、後者は満たさないため。
 */

const ll MOD = 998244353;

ll solve(int N, string &S)
{
	// 1-indexed
	int len_half = (N+1)/2;  // 回文ということを踏まえ、何文字目まで見るか Sが4文字なら2まで、3文字なら2まで

	// dp[i][j]
	// i文字目まで見たときに、  i:1～(N+1)/2
	// j=0:それまでSと同じ文字列
	// j=1:Sより辞書順で前に来る文字列
	// の組み合わせの数
	vector<vector<ll>> dp(len_half+1, vector<ll>(2, 0));
	dp[0][0] = 1;

	for(int i = 1; i <= len_half; i++)
	{
		dp[i][0] = 1;
		dp[i][1] += (S[i-1]-'A')%MOD;  // S[i-1]が'C'なら2, ’A'なら0
		dp[i][1] += (dp[i-1][1]*26)%MOD;  // A～Zの何でも選べる
	}

	// 入力"ABCZAZ"について、"ABCCBA"が条件を満たすか判定
	string tmp1, tmp2;
	// "ABCZAZ" -> "ABC" -> "CBA" を作る
	tmp1 = S.substr(0, N/2);
	reverse(tmp1.begin(), tmp1.end());
	// "ABCZAZ" -> "ZAZ" を作る
	tmp2 = S.substr((N+1)/2, N/2);
	if(tmp1 > tmp2)
	{
		// 条件を満たさないので、組み合わせの数から除外する
		dp[len_half][0] = 0;
	}

	return (dp[len_half][0]+dp[len_half][1])%MOD;
}

int main(void)
{
	int T;
	cin >> T;
	for(int i = 0; i < T; i++)
	{
		// 1-indexed
		int N;
		string S;
		cin >> N;
		cin >> S;
		cout << solve(N, S) << endl;
	}

	return 0;
}
