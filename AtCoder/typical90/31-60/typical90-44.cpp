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

// 競プロ典型90問:44 https://atcoder.jp/contests/typical90/tasks/typical90_ar

/*
 * T=2の全体シフトは愚直にやらず、基点をずらしてO(1)で対応する。[典型]
 * 
 * ※シフト量は1なので、dequeでも解けるみたい。
 *   https://atcoder.jp/contests/typical90/editorial/6398
 */

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int b = 0;  // 基点
	while(Q > 0)
	{
		int t, x, y; cin >> t >> x >> y;
		x--; y--;
		if(t == 1)
		{
			int idx1 = (b+x)%N;
			int idx2 = (b+y)%N;
			swap(a[idx1], a[idx2]);
		}
		else if(t == 2)
		{
			b = (b-1+N)%N;
		}
		else
		{
			int idx1 = (b+x)%N;
			cout << a[idx1] << endl;
		}
		Q--;
	}

	return 0;
}
