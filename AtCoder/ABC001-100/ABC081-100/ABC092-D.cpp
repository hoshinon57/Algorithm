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

// ABC092 https://atcoder.jp/contests/abc092

/*
 * 構築問題。
 * 自分の実装はhamayanhamayan氏と近かったと思う。
 *   https://blog.hamayanhamayan.com/entry/2018/03/25/225328
 * 
 * 縦40,横100で固定の盤面を用意し、上半分を白、下半分を黒に塗っておく。
 * まずこれで連結成分数が1ずつになる。
 * 
 * 残り必要なだけ、黒は上半分に点を打っていき、白は下半分に同様。
 * 
 * サイズは異なるが、以下になるイメージ。
 * .#.#.#.#
 * ........
 * .#.#.#.#
 * ........
 * .#.#.#.#
 * ........
 * ########
 * #.#.#.#.
 * ########
 * #.#.#.#.
 * ########
 * #.#.#.#.
 */

int main(void)
{
	int i;
	int a, b; cin >> a >> b;
	vector<string> s(40, string(100, '#'));  // 40行、100列
	for(i = 0; i < 20; i++)
	{
		s[i].assign(100, '.');
	}

	// 黒を順に
	for(i = 0; i < b-1; i++)
	{
		int x = i%50 * 2 + 1;
		int y = i/50 * 2;
		s[y][x] = '#';
	}

	// 白を順に
	for(i = 0; i < a-1; i++)
	{
		int x = i%50 * 2 + 1;
		int y = i/50 * 2 + 21;
		s[y][x] = '.';
	}

	cout << "40 100" << endl;
	for(auto &e : s) cout << e << endl;

	return 0;
}
