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

// ABC075 https://atcoder.jp/contests/abc075

/*
 * 全探索する。典型：基本は全探索
 * 自分の実装はO(N^3*logN)とかだが、各種解説はO(N^4)とかO(N^5)だった。
 * 
 * まず考察として、長方形の辺上には必ず1点以上存在する。（そうしないと無駄が発生する）
 * そこで、長方形の左辺と右辺に該当する2点で全探索する。
 * そうしたら、その左右に収まる点のy座標を全列挙した後、ソートする。
 * 各yを長方形の下側の辺とし、(K-1)先の要素を上側の辺とすると、K個含む長方形ができあがる。
**/

int main(void)
{
	ll i, j, k;
	ll N, K; cin >> N >> K;
	vector<ll> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}
	ll ans = INF64;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // 点iを長方形の左側の辺、点jを右側の辺
		{
			if(i == j) continue;
			if(x[i] > x[j]) continue;
			vector<ll> yli;
			for(k = 0; k < N; k++)
			{
				if(x[i] <= x[k] && x[k] <= x[j])
				{
					yli.push_back(y[k]);
				}
			}
			sort(yli.begin(), yli.end());
			for(k = 0; k < (int)yli.size(); k++)  // 長方形の下側の辺
			{
				int m = k+K-1;  // 長方形の上側
				if(m >= (int)yli.size()) continue;
				ll area = (x[j]-x[i]) * (yli[m]-yli[k]);
				chmin(ans, area);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
