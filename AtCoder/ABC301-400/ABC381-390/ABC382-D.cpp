#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC382 https://atcoder.jp/contests/abc382

/*
 * 再帰を用いて数列を列挙する手法で解く。
 * 
 * A1,A2,A3,...の順に数列を作っていく。
 * このとき A(i-1)+10<=Ai やMに関する制約から手で色々計算すると、Mが最大でも
 *   Ai-A(i-1) が10を超える数  正確には Ai-A(i-1)-10 の総和
 * は9まで、と分かる。
 * そのため上記が現在何回まで起きたかをカウントしておき、それを超えない範囲で列挙していく。
 * この枝刈りが無いとTLEとなった。
 */

vector<vector<int>> ans;

// rem: Ai-A(i-1) が10overにできる残り回数 これは9以下
void rec(vector<int> &a, int N, int M, int rem)
{
	int i;
	if((int)a.size() == N)
	{
		ans.push_back(a);
		return;
	}

	for(i = 0; i <= rem; i++)
	{
		int nxt;
		if((int)a.size() == 0)  // 初回
		{
			nxt = i+1;
		}
		else
		{
			nxt = a.back() + 10 + i;
		}
		if(nxt > M) break;
		a.push_back(nxt);
		rec(a, N, M, rem-i);
		a.pop_back();
	}
}

int main(void)
{
	int N, M; cin >> N >> M;
	vector<int> a;
	rec(a, N, M, 9);

	cout << ans.size() << endl;
	for(auto &e1 : ans)
	{
		int sz_ = (int)e1.size();
		for(int i = 0; i < sz_; i++) {
			cout << e1[i];
			if(i != sz_-1) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
