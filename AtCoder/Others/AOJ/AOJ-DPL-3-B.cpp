#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <stack>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_3_B

/*
 * 最大長方形問題。
 *   https://lets-csharp.com/max-rectangle/
 *   https://algorithmbeginner.blogspot.com/2020/06/blog-post.html
 * 
 * 各マスにて、上方向にきれいなタイルが何マス連続しているかを事前計算する。
 * 実装ではcs[i][j]としている。
 * 
 * 各行について「ヒストグラム中の最大正方形問題」を解けばよい。
 * これはAOJ-DPL-3-C.cppも参照。
 */

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	vector<vector<int>> c(H, vector<int>(W, 0));  // c[H][W]
	auto cs = c;  // 上方向に、きれいなタイルが何マス連続しているか
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			cin >> c[i][j];
			if(i-1 >= 0) cs[i][j] = cs[i-1][j];
			if(c[i][j] == 0) cs[i][j]++;
			else cs[i][j] = 0;
		}
		cs[i].push_back(0);  // 末尾番兵
	}

	// [実装方針]
	// stack内は単調増加、先頭から見ていく、stackには高さに加えx座標の情報が必要
	
	int ans = 0;
	for(i = 0; i < H; i++)  // 行ごと
	{
		stack<pair<int,int>> st;  // {h, x}
		st.push({0, -1});  // 先頭番兵
		for(j = 0; j < (int)cs[i].size(); j++)
		{
			int x = j;
			while(st.top().first > cs[i][j])
			{
				auto [hh, xx] = st.top();
				st.pop();
				chmax(ans, hh*(j-xx));
				x = xx;
			}
			st.push({cs[i][j], x});
		}
	}
	cout << ans << endl;

	return 0;
}
