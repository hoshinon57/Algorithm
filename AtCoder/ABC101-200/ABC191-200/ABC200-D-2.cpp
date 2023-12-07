#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC200 https://atcoder.jp/contests/abc200

/*
 * 公式解説を参考に、bit全探索＆鳩ノ巣原理で解く。
 *   https://atcoder.jp/contests/abc200/editorial/1246
 * 
 * N個の要素から好きに選ぶ選び方は2^N通り。
 * 2^8>200 より、要素が8個あれば確実に条件を満たすB,Cは存在する。
 * よって先頭からMAX8個について、bit全探索すればよい。
 */

int main(void)
{
	const int MOD = 200;
	int i, j;
	int N; cin >> N;
	vector<int> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a[i] %= MOD;
	}
	N = min(N, 8);  // 2^8 > 200

	vector<set<int>> list(MOD);  // list[i]:和を200で割った余りがiになるような選び方の集合
	for(i = 1; i < (1<<N); i++)  // bitごとに考えて、1が立っているa[i]を選ぶ
	{
		int sum = 0;
		for(j = 0; j < N; j++)
		{
			if( ((i>>j)&1) == 0 ) continue;
			sum = (sum+a[j]) % MOD;
		}
		list[sum].insert(i);
	}

	auto print = [&](void) -> void
	{
		for(i = 0; i < MOD; i++)
		{
			if(list[i].size() <= 1) continue;
			cout << "Yes" << endl;
			auto itr = list[i].begin();
			for(j = 0; j < 2; j++)  // 2つ出力
			{
				int n = *itr;
				cout << __builtin_popcount(n) << " ";
				for(int k = 0; k < N; k++)
				{
					if( ((n>>k)&1) == 0 ) continue;
					cout << k+1 << " ";
				}
				itr++;
				cout << endl;
			}
			return;
		}
		cout << "No" << endl;
	};
	print();

	return 0;
}
