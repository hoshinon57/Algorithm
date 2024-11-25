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

// ABC381 https://atcoder.jp/contests/abc381

/*
 * コンテスト時、苦戦した。尺取り法で解く。
 *   https://x.com/kyopro_friends/status/1859956952446705706
 *   (mechanicalpenciI氏のは読む気が起きないので見てない)
 * 
 * 1122数列の定義および「連続する部分列」という条件から、Aから始点を1つ選び、2つずつ取っていくことになる。
 * ならば始点を偶数と奇数とで2回やれば良さそう、となる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i]; a[i]--;}

	int ans = 0;
	for(int _ = 0; _ < 2; _++)  // 偶数,奇数スタートそれぞれ
	{
		int l, r;
		vector<int> exist(N);  // exist[i]=1なら登場済み
		for(l = _, r = _; l < N; l+=2)  // [l,r)は条件を満たす
		{
			while(r < N-1 && exist[a[r]] == 0 && a[r] == a[r+1])  // rを伸ばしても大丈夫か a[r]が未登場で、a[r]=a[r+1].
			{
				exist[a[r]] = 1;
				r += 2;
			}
			chmax(ans, r-l);
			// a[l],a[l+1]が消える
			if(l == r) r += 2;
			else exist[a[l]] = 0;
		}
	}
	cout << ans << endl;

	return 0;
}
