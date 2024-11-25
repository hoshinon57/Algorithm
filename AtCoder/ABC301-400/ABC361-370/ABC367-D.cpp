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

// ABC367 https://atcoder.jp/contests/abc367

/*
 * 典型要素として、
 * ・Mの倍数はMOD Mで考える。MOD Mで同じ値になる部分に着目する。
 * ・区間の和は累積和で考える。
 * ・円環は2周してみる。
 * などがある。
 * 
 * ここから、直近(N-1)個の区間の歩数(MOD M)を管理し、同じものをカウントしていけばよい。
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i]; a[i] %= M;}
	vector<ll> sum(N*2);
	for(i = 0; i < N*2; i++)
	{
		if(i == 0) sum[i] = a[i];
		else sum[i] = (sum[i-1] + a[i%N]) % M;
	}
	
	vector<ll> cnt(M);
	for(i = 0; i < N; i++)  // 1周目
	{
		cnt[sum[i]]++;
	}

	ll ans = 0;
	for(i = N; i < N*2; i++)  // 2周目
	{
		cnt[sum[i-N]]--;
		ans += cnt[sum[i]];
		cnt[sum[i]]++;		
	}
	cout << ans << endl;

	return 0;
}
