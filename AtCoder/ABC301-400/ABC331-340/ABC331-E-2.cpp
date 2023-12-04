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
 * 公式解説を元に実装した。
 *   https://atcoder.jp/contests/abc331/editorial/7821
 * 
 * 副菜を降順ソートしておき、
 * 各主菜について先頭から副菜を1つずつ試していく。
 * キューから取り出して(それがNGリストにあるならば)、その主菜と次の副菜のペアをキューに積む。
 * するとキューには、残りの組み合わせのうち価格が最大のものが必ず含まれている。
 * 
 * また実装では、{価格,インデックス}のpairで持つのではなく、インデックス用配列を別途持つ形にしている。
 */

int main(void)
{
	int i;
	int N, M, L; cin >> N >> M >> L;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(M); for(i = 0; i < M; i++) {cin >> b[i];}
	vector<ll> b_idx(M);
	iota(b_idx.begin(), b_idx.end(), 0);
	sort(b_idx.begin(), b_idx.end(), [&](int x, int y){return b[x]>b[y];});  // 降順ソート
	vector<int> b_cur(N);  // [i]:主菜iとの組み合わせについて、副菜の何番目まで見たか
	set<pair<int,int>> ng;  // 組み合わせNGリスト
	for(i = 0; i < L; i++)
	{
		int c, d; cin >> c >> d;
		c--; d--;
		ng.insert({c, d});
	}

	priority_queue<pair<ll,int>> que;  // {sum, 使っている主菜の番号}
	for(i = 0; i < N; i++)
	{
		que.push({a[i]+b[b_idx[b_cur[i]]], i});
	}
	while(true)
	{
		auto [sum, a_idx] = que.top();
		que.pop();
		if(ng.count({a_idx, b_idx[b_cur[a_idx]]}) == 0)
		{
			cout << sum << endl;
			break;
		}
		if(++b_cur[a_idx] < M)  // まだ試していない副菜があるか
		{
			que.push({a[a_idx] + b[b_idx[b_cur[a_idx]]], a_idx});
		}
	}

	return 0;
}
