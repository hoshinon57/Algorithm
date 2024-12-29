#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題81
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc014/tasks/abc014_3

/*
 * いもす法で解く。
 * 
 * 色の種類をMとして、「全ての色に対して、各人が買うか買わないかを探索する」手法では
 * 計算量がO(MN)となりTLE.
 * 
 * 一般的ないもす法の実装にて、最も購入者が多い部分を探索する。
 * 入力a,bに対して買うのはa<=x<=bであるため、
 * "記録フェーズ"にてカウントを落とすのはb+1である点に注意。
 * 
 * 計算量はO(M+N).
 */

const int AB_MAX = 1000000;

int main(void)
{
	int i;
	int n;
	cin >> n;
	vector<int> imos_table(AB_MAX+2);  // 記録フェーズで[b+1]までアクセスする
	// いもす法 記録フェーズ
	for(i = 0; i < n; i++)
	{
		int a, b;
		cin >> a >> b;
		imos_table[a]++;
		imos_table[b+1]--;  // 買うのはa<=x<=bであるので、b+1にて落とす
	}

	// いもす法 シミュレートのフェーズ
	int answer = imos_table[0];
	for(i = 0; i < AB_MAX; i++)
	{
		imos_table[i+1] += imos_table[i];
		answer = max(answer, imos_table[i+1]);
	}
	cout << answer << endl;

	return 0;
}
