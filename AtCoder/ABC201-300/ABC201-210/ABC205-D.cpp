#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC205 https://atcoder.jp/contests/abc205

/*
 * 二分探索で解く。
 * kyopro_friends氏、TumoiYorozu氏の解法が近い。
 *   https://twitter.com/kyopro_friends/status/1404073708390473732
 *   https://atcoder.jp/contests/abc205/editorial/2091
 * 
 * 「ある数xは何番目に小さいか？」は、lower_bound()でx以下のAiの種類数を求めることで、簡単に求められる。
 * これは単調増加になっており、K番目/K+1番目の切り替わり部分が答となる。
 * 
 * 自分の実装ではM=INF64として、計算量はO(Q*logM*logN)となる。
 * 
 * [ACまでの思考の流れ]
 * ・Ai,Kiの上限が10^18と大きい。
 * ・「小さい方から数えてK番目」は単調増加。
 * ・ある数xについて、それ以下であるAiの個数は簡単に求められる。
 */

vector<ll> a;
void solve(void)
{
	ll K; cin >> K;
	ll l = 0, r = INF64;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		auto check = [&](ll n) -> bool
		{
			auto idx = lower_bound(a.begin(), a.end(), n) - a.begin();
			return (n - idx <= K);
		};
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << l << endl;
}

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	a.resize(N);
	for(i = 0; i < N; i++) {cin >> a[i];}

	while(Q > 0)
	{
		solve();
		Q--;
	}

	return 0;
}
