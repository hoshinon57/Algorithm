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

// ABC100 https://atcoder.jp/contests/abc100

// drken氏の方針と同じ。
// https://drken1215.hatenablog.com/entry/2018/06/16/234000

int main(void)
{
	int i;
	ll N, M; cin >> N >> M;
	vector<ll> x(N), y(N), z(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i] >> z[i];}

	ll ans = -INF64;
	for(int _1 = 0; _1 < 2; _1++)
	{
		for(int _2 = 0; _2 < 2; _2++)
		{
			for(int _3 = 0; _3 < 2; _3++)
			{
				vector<ll> d;
				for(i = 0; i < N; i++)
				{
					d.push_back(x[i]+y[i]+z[i]);
				}
				sort(d.begin(), d.end(), greater<ll>());
				ll sum = reduce(d.begin(), d.begin()+M, 0LL);
				chmax(ans, sum);

				for(auto &e : z) e = -e;
			}
			for(auto &e : y) e = -e;
		}
		for(auto &e : x) e = -e;
	}
	cout << ans << endl;

	return 0;
}
