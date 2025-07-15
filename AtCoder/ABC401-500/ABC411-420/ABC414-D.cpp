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

// ABC414 https://atcoder.jp/contests/abc414

/*
 * 「区間をM個置く」として考える。
 * https://x.com/kyopro_friends/status/1944032441515020378
 * https://atcoder.jp/contests/abc414/editorial/13451
 * 
 * ソートしても問題ないので、Xはソートしておく。[典型]
 * 基地局が1つのとき、Xの左端から右端までカバーする必要があり、強度x=X.back()-X[0]となる。
 * 基地局が2個以上あると、最も離れているXの区間を1つスルーできる。
 * 
 * というのを考えていくと、
 * (M-1)個だけ、Xの区間の大きい方から除外していく形になる。
 * 
 * 区間の数は(N-1), 除外できる区間数は(M-1).
 * そしてM<=Nなので、除外処理で「除外しすぎないか？」といった配慮は不要。
**/

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> x(N); for(i = 0; i < N; i++) {cin >> x[i];}
	sort(x.begin(), x.end());
	vector<ll> df;
	for(i = 0; i < N-1; i++)
	{
		df.push_back(x[i+1]-x[i]);
	}
	sort(df.begin(), df.end(), greater<ll>());

	// M<=N
	// df.size()=N-1
	// (M-1)個だけ、区間を除外できる
	ll ans = x.back() - x[0];
	for(i = 0; i < M-1; i++)
	{
		ans -= df[i];
	}
	cout << ans << endl;

	return 0;
}
