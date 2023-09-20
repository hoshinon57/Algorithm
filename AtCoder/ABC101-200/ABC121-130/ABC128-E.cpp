#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <map>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC128 https://atcoder.jp/contests/abc128

/*
 * イベントソートで解く。
 * 公式解説とおおよそ同じ考え方だった。
 * https://img.atcoder.jp/abc128/editorial.pdf
 * 
 * 入力S,Tについて、座標Xについて時刻[S,T)が通行止めになると考えて良い。
 * すると、例えば{X,S,T}={2,7,10}のとき、出発時刻が[5,8)の人はX=2でストップすることになる。
 * 
 * そこで以下3種類のイベントを用意する。
 * (1)工事終了イベント：{time:T-X, kind:1, X}
 *    timeに出発する人は、座標Xでは止まらない。
 * (2)工事開始イベント：{time:T-X, kind:2, X}
 *    timeに出発する人は、座標Xで止まることになる。
 * (3)人の出発イベント：{timd:D, kind:3, i}
 *    人iがtimeに出発する。
 * 
 * これをtime,kindの小さい順に処理していく。
 * (1)(2)について、Xをsetに追加/削除していく。
 * このsetは「今スタートすると、この座標でストップする」を表し、先頭の値が答となる。
 * 
 * timeが同じときを踏まえ、kindは終了、開始、出発の順の大きさとする。
 * 詳細は割愛。
 */

int main(void)
{
	using vll = vector<ll>;
	int i;
	ll N, Q; cin >> N >> Q;
	priority_queue<vll, vector<vll>, greater<vll>> que;
	for(i = 0; i < N; i++)
	{
		ll s, t, x; cin >> s >> t >> x;
		que.push({t-x, 1, x});  // 工事終了イベント
		que.push({s-x, 2, x});  // 工事開始イベント
	}
	for(i = 0; i < Q; i++)
	{
		ll d; cin >> d;
		que.push({d, 3, i});  // 人の移動開始イベント
	}

	set<ll> stop;  // 今スタートしたときに、ここで止まる
	vector<ll> ans(Q);
	while(!que.empty())
	{
		ll kind, x;
		// tm = que.top()[0];
		kind = que.top()[1];
		x = que.top()[2];
		que.pop();

		if(kind == 1) stop.erase(x);
		else if(kind == 2) stop.insert(x);
		else
		{
			if(stop.empty()) ans[x] = -1;
			else ans[x] = *stop.begin();
		}
	}

	for(auto &e : ans) cout << e << endl;

	return 0;
}
