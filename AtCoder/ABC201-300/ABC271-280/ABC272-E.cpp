#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC272 https://atcoder.jp/contests/abc272

/*
 * コンテスト中に解けず、解説を見て実装した。
 * 以下、1-indexedで記載する。
 *  
 * 「Aに含まれない」「非負の最小整数」という条件から、答は0～Nの範囲になる。
 * 
 * ・A[1]が0～Nの範囲になるのは何回目から何回目までの操作か
 * ・A[2]も同様に何回目から何回目までの操作か
 * ・A[3]～A[N]も同様に…
 * と事前に求めるのは、計算量がO(N^2)で間に合わないように一見思える。
 * ただしA[x]に対して0～Nの範囲になるのは N/x 回である。
 * すると N/1 + N/2 + N/3 + ... + N/N = N*(1/1 + 1/2 + 1/3 + ... + 1/N) であり、
 * これは調和級数により O(N*logN) 相当になる。よって間に合う。
 * ※A[x]の初期値が負の場合、何回目の操作で0以上になるかをO(1)で求めるのが前提。
 * 
 * 実装方針としては、まず各回の操作ごとに0～Nの範囲で含まれる数をnum_list[x]にて列挙する。
 * その後、num_list[]ごとに0から順に見ていき、存在しない値が見つかればそれが答となる。
 * (例えば num_list[2]={1,2,0,5} であれば、2回目の操作後にて非負の最小整数は3である)
 * 
 * [補足]
 * num_list[]をvectorではなくsetで保持する場合、setの操作はO(logN)のため全体ではO(N*logN*logN)となる。
 * ただこの場合でもTLEにはならなかったことを確認した。
 *   vectorでの実装：498ms
 *   setでの実装：1109ms
 */
int main(void)
{
	// 1-indexed
	int i, j;
	int N, M;
	cin >> N >> M;
	vector<vector<int>> num_list(M+1, vector<int>());  // num_list[i]:i回目の操作後にて、0～Nの範囲で含まれる数を列挙
	for(i = 1; i <= N; i++)
	{
		int a;
		cin >> a;
		// A[i]について、0～Nの範囲になる操作回を計算し、num_listに追加する
		// (初期値が0以上か0未満かで処理を分けたが、まとめることも可能だった)
		if(a >= 0)  // 1回目からリストに入れる
		{
			a += i;  // 1回目の操作
			for(j = 1; j <= M; j++)
			{
				if(a > N) break;  // Nを超えたら終了
				num_list[j].push_back(a);
				a += i;
			}
		}
		else  // 初期値が負の場合、0以上になる回からリストに入れる
		{
			int start = (-a+(i-1)) / i;  // 0以上の数になるのは何回目か
			a += i*start;  // (start)回目の操作
			for(j = start; j <= M; j++)
			{
				if(a > N) break;
				num_list[j].push_back(a);
				a += i;
			}
		}
	}

	for(i = 1; i <= M; i++)
	{
		vector<bool> exist(N+1, false);  // exist[x]:i回目の操作後、xは数列の中に存在するか 添字は0～Nの範囲
		for(auto e : num_list[i])
		{
			exist[e] = true;
		}

		for(j = 0; j <= N; j++)
		{
			if(!exist[j])
			{
				cout << j << endl;
				break;
			}
		}
	}

	return 0;
}
