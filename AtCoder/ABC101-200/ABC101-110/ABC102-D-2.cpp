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

// ABC102 https://atcoder.jp/contests/abc102

/*
 * 二分探索の部分を尺取り法で実装してみた版。
 * lambda関数をf()からf2()に変えた形。
 * 
 * 尺取り法はまだまだ実装がヘタだのう…
**/

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	
	auto f2 = [&](vector<ll> &sum) -> vector<vector<ll>>
	{
		vector<vector<ll>> li(N+1);
		ll m = 0;  // 左側2個の部分列から、最左用にm個取る
		for(i = 2; i <= N-2; i++)  // 先頭からi個取ったところで、2個/2個の区切りとする
		{
			// 左側の和 < 右側の和 である限界までmを進める
			while(m+2 <= i && (sum[m+1]-sum[0]) < (sum[i]-sum[m+1]))
			{
				m++;
			}

			// 候補は 左側の和 < 右側の和 のところと、右側の和 < 左側の和 のところ
			ll tmp1 = abs((sum[m]-sum[0])-(sum[i]-sum[m]));  // [0,m), [m,i)
			ll tmp2 = abs((sum[m+1]-sum[0])-(sum[i]-sum[m+1]));  // [0,m+1), [m+1,i)
			if(tmp1 < tmp2)
			{
				li[i].push_back(sum[m]-sum[0]);
				li[i].push_back(sum[i]-sum[m]);
			}
			else
			{
				li[i].push_back(sum[m+1]-sum[0]);
				li[i].push_back(sum[i]-sum[m+1]);
			}
		}
		return li;
	};

	auto f = [&](vector<ll> &sum) -> vector<vector<ll>>
	{
		vector<vector<ll>> li(N+1);
		for(i = 2; i <= N-2; i++)  // 先頭からi個取ったところで、2個/2個の区切りとする
		{
			auto idx = lower_bound(sum.begin(), sum.end(), sum[i]/2);
			ll a1 = *idx;
			ll a2 = sum[i]-a1;
			idx--;
			ll b1 = *idx;
			ll b2 = sum[i]-b1;
			if(abs(a2-a1) < abs(b2-b1))  // 差が小さい方を採用
			{
				li[i].push_back(a1);
				li[i].push_back(a2);
			}
			else
			{
				li[i].push_back(b1);
				li[i].push_back(b2);
			}
		}
		return li;
	};

	vector<ll> sum_f(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {sum_f[kk+1] = sum_f[kk] + a[kk];}
//	auto li_f = f(sum_f);
	auto li_f = f2(sum_f);

	reverse(a.begin(), a.end());
	vector<ll> sum_b(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {sum_b[kk+1] = sum_b[kk] + a[kk];}
//	auto li_b = f(sum_b);
	auto li_b = f2(sum_b);

	ll ans = INF64;
	for(i = 2; i <= N-2; i++)
	{
		// li_f[i], li_b[N-i] を合算
		ll ma = max(max(li_f[i][0], li_f[i][1]), max(li_b[N-i][0], li_b[N-i][1]));
		ll mi = min(min(li_f[i][0], li_f[i][1]), min(li_b[N-i][0], li_b[N-i][1]));
		chmin(ans, ma-mi);
	}
	cout << ans << endl;

	return 0;
}
