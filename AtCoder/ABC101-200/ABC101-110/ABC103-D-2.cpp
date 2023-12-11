#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC103 https://atcoder.jp/contests/abc103

/*
 * 公式解説を元に、区間スケジューリング問題として解く。
 * 
 * 入力(a,b)について、bの昇順にソートしておく。
 * ソート順に見ていくと、a～b間の橋を1つ壊す必要があるが、
 *   すでにその間にて壊れている橋があれば、さらに壊す必要が無い
 *   無いならば、b直前の橋を壊すのが最善 (bでソートしているので)
 * となる。
 * 直前に壊した橋の位置をprevとして保持すれば上記を実装できる。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	using pii = pair<int,int>;
	vector<pii> ng(M);
	for(i = 0; i < M; i++)
	{
		cin >> ng[i].first >> ng[i].second;
	}
	sort(ng.begin(), ng.end(), [&](pii a, pii b){ return a.second < b.second; });

	int ans = 0;
	int prev = -1;  // 前回壊した橋の位置 (prev,prev+1)の間
	for(auto &e : ng)
	{
		if(prev < e.first)
		{
			ans++;
			prev = e.second-1;
		}
		else {;}  // 前回壊した橋で、今回のng(=e)も大丈夫
	}
	cout << ans << endl;

	return 0;
}
