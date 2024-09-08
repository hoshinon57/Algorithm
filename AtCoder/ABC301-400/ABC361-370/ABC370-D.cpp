#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC370 https://atcoder.jp/contests/abc370

// 行ごと、列ごとにsetで壁の位置を持つ。
// クエリ(R,C)に対し、その前後にある壁をlower_bound()を用いて探す。

int main(void)
{
	// 0-indexed
	int y, x;
	int H, W, Q; cin >> H >> W >> Q;
	vector<set<int>> gyou(H), retu(W);  // gyou[y]:y行目にある壁の一覧
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++) gyou[y].insert(x);
	}
	for(x = 0; x < W; x++)
	{
		for(y = 0; y < H; y++) retu[x].insert(y);
	}

	// (yy,xx)にある壁を削除
	auto del = [&](int yy, int xx) -> void
	{
		gyou[yy].erase(xx);
		retu[xx].erase(yy);
	};
	while(Q > 0)
	{
		Q--;
		int r, c; cin >> r >> c;
		r--; c--;
		if(gyou[r].count(c) > 0) del(r, c);  // (R,C)に壁がある
		else
		{
			{  // 右側を探して削除
				auto itr = gyou[r].lower_bound(c);
				if(itr != gyou[r].end()) del(r, *itr);
			}
			{  // 左側を探して削除
				auto itr = gyou[r].lower_bound(c);
				if(itr != gyou[r].begin()) del(r, *prev(itr));
			}
			{  // 下側
				auto itr = retu[c].lower_bound(r);
				if(itr != retu[c].end()) del(*itr, c);
			}
			{  // 上側
				auto itr = retu[c].lower_bound(r);
				if(itr != retu[c].begin()) del(*prev(itr), c);
			}
		}
	}

	ll ans = 0;
	for(y = 0; y < H; y++) ans += gyou[y].size();
	cout << ans << endl;

	return 0;
}
