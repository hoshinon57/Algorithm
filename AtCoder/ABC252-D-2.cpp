#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC252 https://atcoder.jp/contests/abc252
// 動的計画法で解く版

/*
 * まず、数列Aに登場する値を小さい順に並べ、
 * 各値ごとに「何個あるか」をb[]に入れていく。
 * A={1,5,5,10,10,10}なら、b[]={1,2,3}を設定する
 * 
 * 結局のところ、数列Aはソートしても問題ない。
 * またA={1,2,3,3}とA={1,5,10,10}は答は同じ。
 * 
 * よって動的計画法で、以下のようにdp[j][i]を定める。
 *   b[]の0～i番目まで処理したときに、   i:0～b.size()-1
 *   かつここまで A(i),A(j),A(k) からj個選んだ時の、    j:0～3
 * 組み合わせの数
 * 
 * DPの遷移として、
 * ・i番目を選ぶ場合、
 *   (i-1)番目までの場合の数とi番目の値の数の積、だけ組み合わせがある。
 * ・i番目を選ばない場合、
 *   (i-1)番目までの組み合わせの数を、そのまま使う。
 * 
 * 最後にdp[3][b.size()-1]が求める解となる。
 */
int main(void)
{
	// 0-indexed
	int i, j;
	int N;
	cin >> N;
	vector<int> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	sort(a.begin(), a.end());  // 昇順ソート

	// a[]に登場する値を小さい順に並べ、
	// 各値ごとに「何個あるか」をb[]に入れていく。
	// a[]={1,5,5,10,10,10}なら、b[]={1,2,3}を設定する
	vector<int> b;
	int now = a[0];
	int count = 1;
	for(i = 1; i < N; i++)
	{
		if(now == a[i])
		{
			count++;
		}
		else
		{
			b.push_back(count); // i-1までの値の数を入れる
			now = a[i];
			count = 1;
		}
	}
	b.push_back(count);  // a[]末尾の分

	// dp[j][i]
	// b[]の0～i番目まで処理したときに、   i:0～b.size()-1
	// かつここまで A(i),A(j),A(k) からj個選んだ時の、    j:0～3
	// 整数の組の数
	vector<vector<ll>> dp(4, vector<ll>(b.size(), 0));
	dp[0][0] = 1;  // 0番目まで処理して、0個選ぶのは1通り
	dp[1][0] = b[0];  // 0番目まで処理して、1個選ぶのはb[0]通り
	for(i = 1; i < b.size(); i++)
	{
		dp[0][i] = 1;  // 0個選ぶのは常に1通り
		for(j = 1; j <= 3; j++)
		{
			// i番目を選ぶ場合、
			// (i-1)番目までの場合の数とi番目の値の数の積
			dp[j][i] += dp[j-1][i-1] * b[i];

			// i番目を選ばない場合、(i-1)番目までの場合の数
			dp[j][i] += dp[j][i-1];
		}
	}

	cout << dp[3][b.size()-1] << endl;
	
	return 0;
}
