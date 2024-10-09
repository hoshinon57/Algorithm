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

// ABC369 https://atcoder.jp/contests/abc369

/*
 * 公式解説のLISの考え方で実装してみた。
 *   https://atcoder.jp/contests/abc369/editorial/10835
 * 
 * [考え方]
 * マス(y,x)の集合をyについてソートすると、xに関するLISに帰着できる。
 * 
 * コイン(R,C)をRの順にソートしておくと、Cを用いてLISを求める問題に帰着できる。
 * LISには{C, コインのNo.}の形で登録しておき、
 * またNo.ごとに「1つ前のコインNo.」を保持するpre[]を用意する。
 * ※LISは上書きされていくので、復元にはpre[]が必要
 * 
 * するとpreを用いて末尾から先頭へと復元できる。
 */

using pii = pair<int,int>;
int main(void)
{
	int i;
	int H, W, N; cin >> H >> W >> N;
	vector<pii> rc(N);  // {R,C}
	for(i = 0; i < N; i++) cin >> rc[i].first >> rc[i].second;
	sort(rc.begin(), rc.end());

	vector<pii> lis(N+2, {INF32, -1});  // {C, コインNo.}
	vector<int> pre(N+2, -1);  // pre[x]:No.xのコインの1つ前のコインNo.
	for(i = 0; i < N; i++)
	{
		auto idx = upper_bound(lis.begin(), lis.end(), make_pair(rc[i].second, i)) - lis.begin();
		lis[idx] = {rc[i].second, i};
		if(idx > 0)
		{
			pre[i] = lis[idx-1].second;
		}
	}

	vector<pii> rt;  // {R,C}
	rt.push_back({H, W});
	int m = N-1;
	while(lis[m].first == INF32) m--;  // 末尾から見て、LISが登録された要素を探索(=コインの取得枚数のMAX)
	m = lis[m].second;  // 最後に拾うコインのNo.
	while(m != -1)
	{
		rt.push_back({rc[m].first, rc[m].second});
		m = pre[m];
	}
	rt.push_back({1, 1});
	reverse(rt.begin(), rt.end());

	cout << rt.size()-2 << endl;
	string s;
	for(i = 0; i < (int)rt.size()-1; i++)
	{
		// i -> i+1へ移動
		int dy = rt[i+1].first - rt[i].first;
		int dx = rt[i+1].second - rt[i].second;
		while(dy > 0) { dy--; s+='D'; }
		while(dx > 0) { dx--; s+='R'; }
	}
	cout << s << endl;

	return 0;
}
