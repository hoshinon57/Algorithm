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

// AOJ DSL_3_A
// https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/3/DSL_3_A

// 尺取り法の練習問題

int main(void)
{
	int i;
	int N, S; cin >> N >> S;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	int l, r;
	r = 0;
	int ans = INF32;
	int sum = 0;
	for(l = 0; l < N; l++)  // [l,r)
	{
		while(r < N && sum < S)  // 条件を満たすまで移動
		{
			sum += a[r];
			r++;
		}
		if(sum >= S)
		{
			chmin(ans, r-l);
		}
		sum -= a[l];
		// lがrを超えることは無い
	}
	if(ans == INF32) ans = 0;
	cout << ans << endl;

	return 0;
}
