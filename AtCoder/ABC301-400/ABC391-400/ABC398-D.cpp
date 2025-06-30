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

// ABC398 https://atcoder.jp/contests/abc398

/*
 * 煙は毎ターン増えていくので、これを毎回動かす処理を愚直にやるとO(N^2)となりTLE.
 * 煙ではなく、個数が定数である火と人を動かす。
 * 煙が存在する位置はsetで管理。
**/

int main(void)
{
	int N, r, c; cin >> N >> r >> c;
	string s; cin >> s;
	int fy = 0, fx = 0;  // 火の{y,x}

	set<pair<int,int>> ke;  // 煙がある{y,x}
	ke.insert({fy, fx});
	string ans;
	for(auto &e : s)
	{
		if(e == 'N')  // 煙は北へ動くので、火と人は南へ
		{
			r++;
			fy++;
		}
		if(e == 'W')
		{
			c++;
			fx++;
		}
		if(e == 'S')
		{
			r--;
			fy--;
		}
		if(e == 'E')
		{
			c--;
			fx--;
		}
		if(ke.count({r, c}) > 0) ans.push_back('1');
		else ans.push_back('0');
		ke.insert({fy, fx});
	}
	cout << ans << endl;

	return 0;
}
