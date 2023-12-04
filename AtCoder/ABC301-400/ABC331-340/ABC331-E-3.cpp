#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <queue>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC331 https://atcoder.jp/contests/abc331

/*
 * ABC331-E.cppから、データの持ち方を{値,インデックス}からインデックス用配列を別途用意する版。
 * 実装の練習として。
 */

int main(void)
{
	int i;
	int N, M, L; cin >> N >> M >> L;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(M); for(i = 0; i < M; i++) {cin >> b[i];}
	vector<ll> a_idx(N), b_idx(M);
	iota(a_idx.begin(), a_idx.end(), 0);
	sort(a_idx.begin(), a_idx.end(), [&](int x, int y){return a[x]>a[y];});  // 降順ソート
	iota(b_idx.begin(), b_idx.end(), 0);
	sort(b_idx.begin(), b_idx.end(), [&](int x, int y){return b[x]>b[y];});  // 降順ソート
	set<pair<int,int>> ng;  // 組み合わせNGリスト
	for(i = 0; i < L; i++)
	{
		int c, d; cin >> c >> d;
		c--; d--;
		ng.insert({c, d});
	}

	set<pair<int,int>> chk;
	priority_queue<pair<ll,pair<int,int>>> que;  // {sum, {a_idx, b_idx}}
	que.push({a[a_idx[0]]+b[b_idx[0]], {0, 0}});
	while(true)
	{
		auto [sum, tmp] = que.top();
		auto [ai, bi] = tmp;
		que.pop();

		if(ng.count({a_idx[ai], b_idx[bi]}) == 0)
		{
			cout << sum << endl;
			break;
		}

		if(++ai < N && chk.count({ai,bi}) == 0)
		{
			que.push({a[a_idx[ai]]+b[b_idx[bi]], {ai, bi}});
			chk.insert({ai,bi});
		}
		ai--;
		if(++bi < N && chk.count({ai,bi}) == 0)
		{
			que.push({a[a_idx[ai]]+b[b_idx[bi]], {ai, bi}});
			chk.insert({ai,bi});
		}
	}

	return 0;
}
