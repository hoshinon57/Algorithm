#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:63 https://atcoder.jp/contests/typical90/tasks/typical90_bk

/*
 * bit全探索で解く。ほぼ公式解説と同じ実装。(mapの使用有無だけ異なるぐらい)
 *   https://github.com/E869120/kyopro_educational_90/blob/main/sol/063.cpp
 * 
 * H=8なので、行について選ぶ/選ばないをbit全探索する。
 * W側についてそれぞれ見ていって、
 *   ある列について、選んでいる行の値がすべて同一なら、cnt[val]に加算
 *   Wの最後まで見たら、cnt[val]が最大になるようなvalを選ぶように、列を選ぶ
 * とすればよい。
 * 計算量はO(2^H*HW)かな。
 */

int main(void)
{
	int i, j, k;
	int H, W; cin >> H >> W;
	vector<vector<int>> p(H, vector<int>(W, 0));  // p[H][W]
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> p[i][j];
	}

	int ans = 0;
	for(i = 1; i < 1<<H; i++)  // 立っているbitの行のみを使う 1つ以上
	{
		vector<int> cnt(H*W+1);  // cnt[i]:選んだ行/列について、値iが書かれているマス数
		int mx = 0;
		for(j = 0; j < W; j++)
		{
			int val = -1;
			bool ok = true;
			// 列jが全て同じ値かをチェック
			for(k = 0; k < H; k++)
			{
				if( ((i>>k)&1) == 0 ) continue;
				if(val == -1) val = p[k][j];  // 初回
				else
				{
					if(val != p[k][j]) ok = false;
				}
			}
			// 列jが全て同じ値なら、その値をカウント加算
			if(ok)
			{
				cnt[val] += __builtin_popcount(i);
				chmax(mx, cnt[val]);  // 最大値を保持 後にansへ展開
			}
		}
		chmax(ans, mx);
	}
	cout << ans << endl;

	return 0;
}
