#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題82
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2013

/*
 * いもす法で解く。
 * 
 * とりうる時刻の種類をMとして、「全ての時刻に対して、時刻表の各電車の範囲内かを探索する」手法では
 * 計算量がO(MN)となりTLE.
 * 
 * いもす法のテーブルにおいて
 *   発車時刻にて+1
 *   到着時刻にて-1
 * として記録し、シミュレート後のテーブルの最大値が答となる。
 * 
 * サンプルより、例えば03:00:00に到着した電車は、そのまま03:00:00に発車できる。
 * そのためいもす法の記録フェーズにて、 imos_table[(到着時刻)]--; としてよい。
 * ※この辺り問題次第で、 (到着時刻) と (到着時刻+1) のどちらをデクリメントするのかが変わる。
 * 
 * 計算量はO(M+N).
 */

const int IMOS_MAX = 235959;  // 23:59:59

int main(void)
{
	int i;
	int n;
	vector<int> imos_table;

	auto input = [&](void) -> void
	{
		for(i = 0; i < n; i++)		
		{
			for(int j = 0; j < 2; j++)
			{
				// いもす法 記録のフェーズ
				// 時刻12:34:56を123456に変換する
				string str;
				cin >> str;
				int num = stoi(str.substr(0, 2) + str.substr(3, 2) + str.substr(6, 2));
				if(j == 0)  // 発車時刻
				{
					imos_table[num]++;
				}
				else  // 到着時刻
				{
					imos_table[num]--;
				}
			}
		}
	};
	
	while(true)
	{
		cin >> n;
		if(n == 0) break;
		imos_table.assign(IMOS_MAX+1, 0);  // 記録フェーズ.シミュレートフェーズでIMOS_MAX+1までアクセスする

		input();

		// いもす法 シミュレートのフェーズ
		int answer = imos_table[0];
		for(i = 0; i < IMOS_MAX; i++)
		{
			imos_table[i+1] += imos_table[i];
			answer = max(answer, imos_table[i+1]);
		}
		cout << answer << endl;
	}

	return 0;
}
