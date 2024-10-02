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

// ARC022 https://atcoder.jp/contests/arc022

/*
 * 尺取り法で解く。
 * 参考：https://qiita.com/drken/items/ecd1a472d3a0e7db8dce
 * 
 * 半開区間[l,r)を考えたとき、
 * [l,r)が条件を満たすならば [l-1,r) および [l,r-1) も条件を満たす。
 * よって尺取り法を使うことができる。
 * 
 * 味a[*]についてsetで管理していけばよい。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	set<int> s;
	int l, r = 0;
	int ans = 0;
	for(l = 0; l < N; l++)  // [l,r)
	{
		while(r < N && s.count(a[r]) == 0)  // rを1つ進めても、「同じ味が含まれない」という条件を満たすか
		{
			s.insert(a[r]);
			r++;
		}
		chmax(ans, r-l);
		if(l==r) r++;  // [l,r)で囲む要素が0この場合  本問ではありえないが、お作法として
		else s.erase(a[l]);  // a[l]を除外する
	}
	cout << ans << endl;	

	return 0;
}
