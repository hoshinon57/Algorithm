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

// ABC019 https://atcoder.jp/contests/abc019

/*
 * 「木の直径は最短距離計算を2回やる」の気持ちになる。
 * 
 * 頂点1->x への距離をNパターン求めて、最長距離の頂点vを求め、
 * 頂点v->x への距離をNパターン求めて、その最長距離が答。
 * 
 * ジャッジシステムへの問い合わせ回数はN*2回であり、
 * 制約のmax(N)=50, 問い合わせ回数100回にぴったり合う。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	using pii = pair<int,int>;

	// 頂点stから各頂点への求め、最長距離の{頂点番号, 距離}を返す
	auto f = [&](int st) -> pii
	{
		int idx = 0;
		int md = -1;
		for(i = 1; i <= N; i++)
		{
			// ? a b
			string q = "? " + to_string(st) + " " + to_string(i);
			cout << q << endl;
			int d; cin >> d;
			if(chmax(md, d)) idx = i;
		}
		return {idx, md};
	};
	auto [idx1, d1] = f(1);
	auto [idx2, d2] = f(idx1);
	cout << "! " << d2 << endl;

	return 0;
}
