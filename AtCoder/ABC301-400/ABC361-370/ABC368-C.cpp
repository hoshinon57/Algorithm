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

// ABC368 https://atcoder.jp/contests/abc368

/*
 * 周期性に着目して解く。コンテスト時に苦戦してしまった。
 * 
 * 与ダメージは1+1+3=5でループするので、5で割った余りの部分だけシミュレーションするという考え方。
 * シミュレーション部分は各敵ごとに高々3ループで済むので、間に合う。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}
	ll T = 0;
	for(auto &e : h)
	{
		T += (e/5)*3;  // 3回で5ダメージ で1ループ
		e %= 5;
		while(e > 0)
		{
			T++;
			if(T%3 != 0) e--;
			else e -= 3;
		}
	}
	cout << T << endl;

	return 0;
}
