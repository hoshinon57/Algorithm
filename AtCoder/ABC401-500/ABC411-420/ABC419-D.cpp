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

// 文字ごとに正転か反転かを管理すればよく、いもす法で解ける。

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	string s, t; cin >> s >> t;

	vector<int> imos(N+5);
	for(i = 0; i < M; i++)
	{
		ll l, r; cin >> l >> r;
		l--; r--;
		r++;  // [l,r)
		imos[l]++;
		imos[r]--;
	}

	for(i = 1; i <= N; i++)
	{
		imos[i] += imos[i-1];
	}

	string ans;
	for(i = 0; i < N; i++)
	{
		if(imos[i]%2 == 0)
		{
			ans.push_back(s[i]);
		}
		else
		{
			ans.push_back(t[i]);
		}
	}
	cout << ans << endl;

	return 0;
}
