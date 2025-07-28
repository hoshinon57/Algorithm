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

// 再解きした方針が当時と異なり、面白かったので残す。

/*
 * 二分探索＆累積和というのは同じ。
 * 左端lを[0,N)で全探索するのも同じ。
 * ここから判定問題ではない方針となった。
 * 
 * rem=K%(Sに含まれる'x'の個数) で、ループの残り部分に割り当てる'x'の個数が求められる。
 * [l,r)の区間が「'x'がrem個以内 の最長」とすると、upper_bound()-1でrが求められる。
 * (r-l)+ループ分 がlにおける長さ。
 * 
 * ただしlスタートということは[l,N*M)なので、最長はN*M-1に抑える必要がある。
**/

int main(void)
{
	ll i;
	ll N, M, K; cin >> N >> M >> K;
	string s; cin >> s;
	ll cnto=0, cntx=0;  // sに含まれるoとxの個数
	for(auto &e : s)
	{
		if(e == 'o') cnto++;
		else cntx++;
	}
	// 2周分の'x'の累積和  長さ2N
	auto ss = s+s;
	vector<ll> sumx(2*N+1);
	for(i = 0; i < N*2; i++)
	{
		sumx[i+1] = sumx[i];
		if(ss[i] == 'x') sumx[i+1]++;
	}

	ll ans = 0;
	ll lpcnt = K/cntx;
	ll rem = K%cntx;
	for(ll l = 0; l < N; l++)  // 左端lスタート
	{
		// [l,r) の区間がxがrem個以内 の最長
		ll r = upper_bound(sumx.begin(), sumx.end(), sumx[l]+rem) - sumx.begin() - 1;
		ll tmp = r-l;
		tmp += lpcnt*N;
		// lスタート ⇒ [l,N*M) なので N*M-lが最長
		chmin(tmp, N*M-l);
		chmax(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
