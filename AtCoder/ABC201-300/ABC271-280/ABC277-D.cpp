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

// ABC277 https://atcoder.jp/contests/abc277

/*
 * まず、Aはソートして良い。
 * 値の差が0または1であるような区間[l,r)を、各lについて求めていく。
 * その区間和を全体の総和から引いたもの、の最小値が答。
 * 
 * lの更新は+1ではなくrを代入で良い。
 * 
 * 見ているカードがN-1から0に戻るときなど、実装に注意。
 * (これ、よくコンテスト時は一発ACできたな…)
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end());
	vector<ll> sum(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {sum[kk+1] = sum[kk] + a[kk];}

	int l,r;
	ll ans = INF64;
	for(l = 0; l < N; )  // lが左端
	{
		r = l+1;
		while(true)
		{
			if(r != N)
			{
				if(a[r] - a[r-1] <= 1) r++;
				else break;
			}
			else
			{
				if(a[r-1] == M-1 && a[0] == 0) r = 1;
				else break;
			}
			if(l == r%M)  // rは1周してきているので
			{
				cout << 0 << endl;
				return 0;
			}
		}
		ll tmp;
		if(l < r) tmp = sum[r] - sum[l];
		else  // 1周してきた場合
		{
			// [0,r) [l,N)
			tmp = (sum[r]-sum[0]) + (sum[N]-sum[l]);
		}
		chmin(ans, (sum[N]-sum[0])-tmp);
		if(l >= r) break;  // 1周してきたら、これ以上は無駄
		l = r;
	}
	cout << ans << endl;

	return 0;
}
