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

// ABC303 https://atcoder.jp/contests/abc303

/*
 * 頂点ごとの次数で考える。
 * 
 * 次数の大きい方から順に見ていくと、それは星の中心になることが分かる。
 * このとき(次数+1)が星の頂点数になるので、Nから引いていく。N=0になれば終了。
 */
int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> jisuu(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		jisuu[u]++;
		jisuu[v]++;
	}
	sort(jisuu.begin(), jisuu.end(), greater<int>());  // 降順

	vector<int> ans;
	for(auto &e : jisuu)  // 次数の大きい方から順に見ていくと、それは星の中心である
	{
		ans.push_back(e);
		N -= (e+1);  // 次数+1が星の頂点数
		if(N == 0) break;
	}
	sort(ans.begin(), ans.end());

	for(auto &e : ans)
	{
		cout << e << " ";
	}
	cout << endl;

	return 0;
}
