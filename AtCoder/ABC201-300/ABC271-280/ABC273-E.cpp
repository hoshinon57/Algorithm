#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 2025/8に再解きしたソースコードに置き換えた。

struct dt
{
	ll pid;  // parent
	ll val;
};

int main(void)
{
	vector<dt> d;
	d.push_back({0, -1});
	ll now = 0;  // d[now]

	ll i;
	ll Q; cin >> Q;
	vector<ll> ans;
	unordered_map<ll,ll> idx;

	while(Q > 0)
	{
		Q--;
		string ki; cin >> ki;
		if(ki == "ADD")
		{
			ll x; cin >> x;
			d.push_back({now, x});
			now = d.size()-1;
		}
		if(ki == "DELETE")
		{
			now = d[now].pid;  // 根の親は根になるよう初期値設定済み
		}
		if(ki == "SAVE")
		{
			ll y; cin >> y;
			idx[y] = now;
		}
		if(ki == "LOAD")
		{
			ll z; cin >> z;
			now = idx[z];
		}
		ans.push_back(d[now].val);
	}

	int sz_ = (int)ans.size();
	// cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
