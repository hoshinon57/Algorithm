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

// ABC321 https://atcoder.jp/contests/abc321

/*
 * DFSで解く。
 * 321-like numberはあまり多くないので、DFSで全部作ってしまう。
 * ※解説より、各数字を使うか使わないかの組み合わせなので(0文字と0がNG)、
 *   2^10-2=1022 通りしかない、と理論的に求められる。
 * 
 * dfs( (末尾に追加する値), ここまでのDFSで作った値 )
 * として実装する。
 * 作ったリストをソートして、lst[K-1]が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・321-like numberを全て列挙できないか、とまずは考える。
 * ・狭義単調減少はDFSで実装しやすそう。
 */

ll K;
vector<ll> lst;  // 321-like numberの一覧

void dfs(int last, ll num)
{
	int i;
	lst.push_back(num);
	for(i = last-1; i >= 0; i--)
	{
		dfs(i, num*10+i);
	}
}

int main(void)
{
	int i;
	cin >> K;
	for(i = 1; i <= 9; i++)
	{
		dfs(i, i);
	}
	sort(lst.begin(), lst.end());
	cout << lst[K-1] << endl;
	
	return 0;
}
