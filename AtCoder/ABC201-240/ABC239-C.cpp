#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC239 https://atcoder.jp/contests/abc239

/*
 * まず、移動量が√5になるようなx,yの組を全て求める。
 * 次に(x1,y1)からの移動先と(x2,y2)の移動先を全探索し、重なるものがあればYesとなる。
 */

bool solve(void)
{
	int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;

	// 移動量が√5になるx,yの組を求める
	vector<pair<int,int>> root_5;
	for(int i = -2; i <= 2; i++)
	{
		for(int j = -2; j <= 2; j++)
		{
			if(i*i+j*j == 5) root_5.push_back({i, j});
		}
	}

	for(auto &e1 : root_5)  // {x1,y1}
	{
		for(auto &e2 : root_5)  // {x2,y2}
		{
			if( (x1+e1.first == x2+e2.first) && (y1+e1.second == y2+e2.second) ) return true;
		}
	}
	return false;
}
int main(void)
{
	YesNo(solve());

	return 0;
}
