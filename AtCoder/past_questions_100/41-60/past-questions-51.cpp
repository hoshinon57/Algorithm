#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 10007;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題51
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2014yo/tasks/joi2014yo_d

/*
 * bitDPで解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i日目まで終わって、   (i:1～N / 1-indexed)
 *   i日目のメンバーがj(bit0:Jくん, bit1:Oくん, bit2:Iくん で参加が1, 不参加が0)のときの、  (j=0～7(2^3-1))
 *   スケジュールの通り数
 * 
 * DPの遷移は以下のようになる。
 * (1)まず、その日の責任者不在はNGであり、dp=0.
 * (2)1日目はJくんが鍵を持っているので、
 *   Jくん不在ならdp=0, 参加ならdp=1.
 * (3)2日目以降について、昨日の全パターン dp[n-1][0～7] からの遷移を考えたときに、
 *   昨日の参加者と本日の参加者とで重複したメンバーがいなければ、鍵の関係でdp=0.
 *   重複メンバーがいれば、dpに昨日分dp[n-1][*]を加算する。
 *   
 * 上記をメモ化再帰で実装した。
 * dp[N][1],dp[N][2], ..., dp[N][7]の総和が答となる。
 * 
 * メンバーは3人で固定であり、計算量は日数にのみ影響する。
 * よってO(N).
 */

int N;
vector<int> master;  // master[i]:i日目の責任者 0～2の順でJ.O,Iくん

// dp[i][j]
//   i日目まで終わって、   (i:1～N / 1-indexed)
//   i日目のメンバーがj(bit0:Jくん, bit1:Oくん, bit2:Iくん で参加が1, 不参加が0)のときの、  (j=0～7(2^3-1) )
//   スケジュールの通り数
vector<vector<int>> dp;

// dp[n][member]をメモ化再帰で計算して返す
int func(int n, int member)
{
	int &d = dp[n][member];  // 記載簡略化

	if(d != -1) return d;  // メモ化済み

	if(((1<<master[n])&member) == 0)  // この日の責任者不在はNG
	{
		d = 0;
		return d;
	}
	if(n == 1)  // 初日はJくんが鍵を持っている
	{
		if((member&1) == 0) d = 0;  // Jくん不在はNG
		else d = 1;  // 初日なので1通り
		return d;
	}

	// 2日目以降
	int i;
	int ret = 0;
	// iの下位3bitを、それぞれ昨日のJ,O,Iくんの参加不参加に割り当てる
	for(i = 0; i < 1<<3; i++)  // 昨日dp[n-1][*]の全パターンをチェック
	{
//		if(((1<<master[n-1])&i) == 0) continue;  // 昨日の責任者不在はNG
		if((i&member) == 0) continue;  // 昨日と今日で重複するメンバーがいない場合、鍵が開けられないのでNG
		ret += func(n-1, i);  // dp[n-1][i] -> dp[n][member] は成立する
	}
	d = ret % MOD;

	return d;
}
int main(void)
{
	int i;
	cin >> N;
	master.resize(N+1);
	string str;
	cin >> str;
	for(i = 1; i <= N; i++)
	{
		if(str[i-1] == 'J')
			master[i] = 0;
		else if(str[i-1] == 'O')
			master[i] = 1;
		else
			master[i] = 2;
	}
	dp.resize(N+1, vector<int>(1<<3, -1));

	int answer = 0;
	// 最終日のN日目について、dp[N][1],dp[N][2], ... , dp[N][7]までの総和を求める
	for(i = 0; i < 1<<3; i++)
	{
		answer += func(N, i);
	}
	answer %= MOD;
	cout << answer << endl;

	return 0;
}
