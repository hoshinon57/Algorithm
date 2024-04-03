#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC347 https://atcoder.jp/contests/abc347

/*
 * 時間方向の累積和を考えて解く。
 * 
 * 各クエリ時点での|S|を元に、|S|の累積和を求めておく。
 * するとiターン目に|S|に入り、jターン目に出たAについては、累積和の差分で増加量を求めることができる。
 * 
 * 全クエリの後、|S|に入ったままの要素についてのケアを忘れずに。
 * 
 * [ACまでの思考の流れ]
 * ・各クエリ時点での結果は不要で、最後の状態の値だけが要求されている。
 */

int main(void)
{
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<ll> ssum(Q+1);  // クエリごとの|S|の累積和
	set<ll> s;
	vector<ll> x(Q);
	for(i = 0; i < Q; i++)
	{
		cin >> x[i];
		x[i]--;
		if(s.count(x[i])) s.erase(x[i]);
		else s.insert(x[i]);
		ssum[i+1] = ssum[i] + s.size();
	}

	vector<ll> ev(N+1, -1);  // ev[ai]:aiに対するイベント状況 -1以外ならiターン目にSにinしており、-1ならout状態
	vector<ll> ans(N+1);
	for(i = 0; i < Q; i++)  // x[i]
	{
		if(ev[x[i]] == -1)  // out->in
		{
			ev[x[i]] = i;
		}
		else  // in->out
		{
			// ev[x[i]]ターン目にin
			// iターン目にout(Aiへの加算はされない)
			ans[x[i]] += ssum[i] - ssum[ev[x[i]]];
			ev[x[i]] = -1;
		}
	}

	for(i = 0; i < N; i++)
	{
		if(ev[i] != -1)  // inのままで終了している場合
		{
			ans[i] += ssum[Q] - ssum[ev[i]];
		}
		cout << ans[i];
		if(i != N-1) cout << " ";
	}
	cout << endl;

	return 0;
}
