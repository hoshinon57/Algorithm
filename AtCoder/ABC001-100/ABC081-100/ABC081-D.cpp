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

// ABC081 https://atcoder.jp/contests/abc081

/*
 * 各種解説と同じ方針だった。
 *   https://scrapbox.io/Example0911/ABC081_D_Non-decreasing
 *   公式解説
 * 
 * まず全てが正の値のとき、左から累積和の要領で操作して条件を満たせる。
 * 全てが負の値のときも、右からやることでOK.
 * 
 * 問題は正負どちらも存在するとき。そのまま累積和の要領でやってもうまくいかない。
 * ここで絶対値が最大の要素に着目して、それが
 *   1)正の値のとき：全要素にその要素を加算すると、全て正の値になる
 *   2)負の値のとき：同じく、全て負の値になる
 * となり、前述の累積和で処理できるようになる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int absmx = 0;
	int absidx = 0;
	for(i = 0; i < N; i++)
	{
		if(abs(a[i]) > absmx)
		{
			absmx = abs(a[i]);
			absidx = i;
		}
	}

	vector<pair<int,int>> ans;
	if(a[absidx] >= 0)
	{
		// まずは全て正へ
		for(i = 0; i < N; i++)
		{
			ans.push_back({absidx, i});
			a[i] += a[absidx];
		}
		// 先頭から順に
		for(i = 0; i < N-1; i++)
		{
			ans.push_back({i, i+1});
			a[i+1] += a[i];
		}
	}
	else
	{
		// まずは全て負へ
		for(i = 0; i < N; i++)
		{
			ans.push_back({absidx, i});
			a[i] += a[absidx];
		}
		// 末尾から順に
		for(i = N-1; i > 0; i--)
		{
			ans.push_back({i, i-1});
			a[i-1] += a[i];
		}
	}

	cout << ans.size() << endl;
	for(auto [fi,se] : ans)
	{
		cout << fi+1 << " " << se+1 << endl;
	}

	return 0;
}
