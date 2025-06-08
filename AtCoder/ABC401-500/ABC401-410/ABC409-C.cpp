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

// ABC409 https://atcoder.jp/contests/abc409

/*
 * b=a+L/3, c=b+L/3 となる3点が正三角形。
 * aの位置は[0,L/3)となるので、これで全探索する。
 * 計算量はO(L).
 * 
 * コンテスト時は各点が点aになるとき、として数え上げた。
 * (abc,bca,cab, と3回ダブるので3で割る必要がある)
 *   https://atcoder.jp/contests/abc409/submissions/66532645
 * この場合は計算量はO(N).
 * 今回は制約よりL,Nの最大値は同一なので、どちらでも解ける。
**/

int main(void)
{
	ll i;
	ll N, L; cin >> N >> L;
	if(L%3 != 0)
	{
		cout << 0 << endl;
		return 0;
	}
	vector<ll> cnt(L);
	ll now = 0;
	cnt[0]++;
	for(i = 0; i < N-1; i++)
	{
		ll d; cin >> d;
		now = (now+d)%L;
		cnt[now]++;
	}
	
	ll ans = 0;
	for(i = 0; i < L/3; i++)
	{
		ll tmp = cnt[i] * cnt[i+L/3] * cnt[i+L/3*2];
		ans += tmp;
	}
	cout << ans << endl;

	return 0;
}
