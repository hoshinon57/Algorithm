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

// ABC039 https://atcoder.jp/contests/abc039

/*
 * 処理前の画像がどう決まるかを考えてみる。
 * 
 * 処理後にて「周りが全て黒」があれば、処理前でも黒にして矛盾しない。
 * ⇒逆に言えば、処理後で「周りに1つでも白がある」部分は、処理前は白でないとダメ。 ★
 * 
 * 上記★を元に処理前の画像を求めてみて(※)、そこから再度処理する。
 * (※)実装では処理後で白画素があれば、周囲8マスの処理前は白にした。
 * それが入力データと一致すればOK.
 * 「元の画像として考えられるものが複数ある」とのことなので、再度処理して比較することが必要。
**/

int main(void)
{
	int i, j, y, x;
	int H, W; cin >> H >> W;
	vector<string> s(H);
	for(i = 0; i < H; i++) cin >> s[i];

	vector<string> be(H, string(W, '#'));  // 処理前を復元
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			if(s[y][x] == '#') continue;
			// 
			for(i = y-1; i <= y+1; i++)
			{
				if(i < 0 || i >= H) continue;
				for(j = x-1; j <= x+1; j++)
				{
					if(j < 0 || j >= W) continue;
					be[i][j] = '.';
				}
			}
		}
	}

	// 再度処理
	vector<string> af(H, string(W, '.'));
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			if(be[y][x] == '.') continue;
			for(i = y-1; i <= y+1; i++)
			{
				if(i < 0 || i >= H) continue;
				for(j = x-1; j <= x+1; j++)
				{
					if(j < 0 || j >= W) continue;
					af[i][j] = '#';
				}
			}
		}
	}

	for(i = 0; i < H; i++)
	{
		if(s[i] != af[i])
		{
			cout << "impossible" << endl;
			return 0;
		}
	}
	cout << "possible" << endl;
	for(i = 0; i < H; i++) cout << be[i] << endl;

	return 0;
}
