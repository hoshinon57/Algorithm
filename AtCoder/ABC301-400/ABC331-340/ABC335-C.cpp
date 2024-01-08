#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <deque>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC335 https://atcoder.jp/contests/abc335

// コンテスト中は使わなかったdequeで実装した版。

int main(void)
{
	int i;
	deque<pair<int,int>> de;
	int N, Q; cin >> N >> Q;
	for(i = 0; i < N; i++) de.push_back({i+1, 0});
	int x = 1, y = 0;

	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			char c; cin >> c;
			switch(c)
			{
			case 'R':
				x++;
				break;
			case 'L':
				x--;
				break;
			case 'U':
				y++;
				break;
			case 'D':
				y--;
				break;
			}
			de.push_front({x, y});
			de.pop_back();  // popしても計算量は変わらないと思うが、念のため
		}
		else
		{
			int p; cin >> p;
			p--;
			cout << de[p].first << " " << de[p].second << endl;
		}
	}

	return 0;
}
