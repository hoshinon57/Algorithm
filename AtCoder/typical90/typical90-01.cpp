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

// 競プロ典型90問:01 https://atcoder.jp/contests/typical90/tasks/typical90_a

/*
 * K個の切れ目の選び方を全て調べるのはTLE.
 * 「スコアxを達成できるか？」の判定問題を考えたときに、
 * xを大きくしていくと true,true,...,true,false,false,... と1回だけt/fが切り替わる。
 * ここの切り替わりを二分探索で求める。
 * 
 * 判定問題は貪欲法で求める。
 * 切れ目の左側から順に見ていって、長さがx以上になればそこで切っていく。
 * K回切れればOK. 最後の切れ目から右部分の処理に注意。
 * ※自分の実装では入力Aの末尾にLを入れることで、判定関数の実装を少し簡易にした。
 * 
 * 二分探索のあとのleft側が答。
 */

int main(void)
{
	int i;
	int N, L, K; cin >> N >> L >> K;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	a.push_back(L);  // 番兵

	int l = 1;
	int r = INF32;
	auto check = [&](int x) -> bool
	{
		int left = 0;
		int cnt = 0;
		for(auto &e : a)
		{
			if(e-left >= x)
			{
				cnt++;
				left = e;
			}
		}
		return (cnt>=K+1);
	};
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << l << endl;

	return 0;
}
