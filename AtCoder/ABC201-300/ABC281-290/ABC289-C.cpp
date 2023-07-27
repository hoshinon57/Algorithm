#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC289 https://atcoder.jp/contests/abc289

/*
 * bit全探索で解く。
 * 
 * 「S1～SMまでM個の集合から1個以上の集合を選ぶ」の全ての選び方をbit全探索し、
 * それぞれの選び方にて、選んだ集合が持つ値をリストアップしていく。
 * 1～Nまで全てリストアップしていたら、条件を満たす。
 * 
 * 計算量はO(2^M*MN).
 * bit全探索部分がO(2^M*M), その中での判定にO(N).
 */

int main(void)
{
	int i, j, k;
	int N, M;
	cin >> N >> M;
	vector<vector<int>> s(M);  // s[M][C]
	for(i = 0; i < M; i++)
	{
		int c;
		cin >> c;
		for(j = 0; j < c; j++)
		{
			int a;
			cin >> a;
			s[i].push_back(a);
		}
	}

	ll answer = 0;
	for(i = 1; i < (1<<M); i++)  // bit全探索 iのbitが1になっていれば、Siは選択対象
	{
		vector<bool> exist(N+1, false);  // exist[1]～[N]  集合の中にその値があればtrue
		for(j = 0; j < M; j++)  // Sj:S1～SM
		{
			if( ((i>>j)&1) == 0 ) continue;  // Sjは対象ではない

			// 集合Sjが持つ値をリストアップ
			for(auto &e : s[j])
			{
				exist[e] = true;
			}
		}

		// 選んだ集合の中に、1～Nまでの値が含まれるか
		bool ok = true;
		for(k = 1; k <= N; k++)
		{
			if(!exist[k])
			{
				ok = false;
				break;
			}
		}
		if(ok) answer++;
	}

	cout << answer << endl;
	
	return 0;
}
