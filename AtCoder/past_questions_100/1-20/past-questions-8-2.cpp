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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題8
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/s8pc-6/tasks/s8pc_6_b

/*
 * 解説にある「中央値が最小」で解いてみた版。
 * 
 * まず、(スタートの座標)<=(ゴールの座標)としても問題ない。
 * このとき、 st->a->b->gl という経路が必ず最短経路となる。
 * すると答は Σ(|st-a|+|a-b|+|b-gl|) となる。
 * 
 * |st-a|と|b-gl|の部分について、「中央値が最小」の法則により
 * それぞれaの中央値、bの中央値がスタートとゴールになる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	ll ans = 0;
	vector<ll> a(N), b(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i]; ans += b[i]-a[i];}
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	ll st = a[a.size()/2];  // aの中央値
	ll ed = b[b.size()/2];
	for(i = 0; i < N; i++)
	{
		ans += abs(a[i]-st) + abs(b[i]-ed);
	}
	cout << ans << endl;

	return 0;
}
