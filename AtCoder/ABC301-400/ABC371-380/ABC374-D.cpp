#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC374 https://atcoder.jp/contests/abc374

// 線分の順序を順列全探索で、線分のどちらの端点から引くかをbit全探索で解く。

// 2点(x1,y1),(x2,y2)の距離の "2乗" を返す
// 実距離を求めたい場合は呼び出し元で sqrt(dist_squared()) など行うこと
// 引数はlong longにしているが、2乗の関係で引数が10^9を超えるとオーバーフローするため注意 (10^9を超える問題はあまり出ないと思うが)
ll dist_squared(ll x1, ll y1, ll x2, ll y2) {
	ll dx = x1-x2, dy = y1-y2;
	return dx*dx+dy*dy;
}

int main(void)
{
	int i, j;
	int N, S, T; cin >> N >> S >> T;
	vector<int> a(N), b(N), c(N), d(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i] >> c[i] >> d[i];}
	vector<int> idx(N);
	iota(idx.begin(), idx.end(), 0);

	double ans = INF64;
	do
	{
		for(i = 0; i < (1<<N); i++)
		{
			int x=0, y=0;
			double t=0;
			for(j = 0; j < N; j++)
			{
				int id = idx[j];  // 見ている線分の番号
				if( ((i>>j)&1) == 0)  // now->(A,B)->(C,D)
				{
					t += sqrt(dist_squared(x, y, a[id], b[id])) / S;
					t += sqrt(dist_squared(a[id], b[id], c[id], d[id])) / T;
					x = c[id];
					y = d[id];
				}
				else  // now->(C,D)->(A,B)
				{
					t += sqrt(dist_squared(x, y, c[id], d[id])) / S;
					t += sqrt(dist_squared(c[id], d[id], a[id], b[id])) / T;
					x = a[id];
					y = b[id];
				}
			}
			chmin(ans, t);
		}
	} while (next_permutation(idx.begin(), idx.end()));
	cout << std::fixed << std::setprecision(20);
	cout << ans << endl;

	return 0;
}
