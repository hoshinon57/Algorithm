#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// AOJ DSL_3_B
// https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/3/DSL_3_B

// 尺取り法の練習問題。
// setにて区間に含まれる値の一覧を管理する。

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> cnt(K+1);  // cnt[i]:値iを何個含むか
	set<int> s;  // 見ている区間に存在する値の一覧

	int l, r;
	r = 0;
	int ans = INF32;
	for(l = 0; l < N; l++)  // [l,r)
	{
		int v;
		while(r < N && (int)s.size() < K)
		{
			v = a[r];
			if(v <= K)
			{
				cnt[v]++;
				if(cnt[v] == 1) s.insert(v);
			}
			r++;
		}
		if((int)s.size() == K)
		{
			chmin(ans, r-l);
		}
		// lを移動のための準備
		v = a[l];
		if(v <= K)
		{
			cnt[v]--;
			if(cnt[v] == 0) s.erase(v);
		}
		// lがrを超えることは無い
	}
	if(ans == INF32) ans = 0;
	cout << ans << endl;

	return 0;
}
