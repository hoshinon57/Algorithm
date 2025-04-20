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

// ABC106 https://atcoder.jp/contests/abc106

/*
 * "5000兆日後"とか10^10^10とか、たまに出てくる。
 * こういうときは操作は「やり切る」というか、「中途半端な状態では終わらない」とか、そういうイメージを持つ。
 * 
 * 各種解説の通りで、
 *   Sを先頭から見ていったときに1以外の値が出てきたら、それ。
 *   先頭に連続する1は操作後も保持されるので、K日後にそこを指すかどうかを判定
 * な感じ。
**/

int main(void)
{
	string s; cin >> s;
	ll K; cin >> K;
	int cnt = 0;  // 先頭からの1の連続個数
	for(auto &e : s)
	{
		if(e != '1') break;
		cnt++;
	}
	if(cnt >= K) cout << 1 << endl;
	else cout << s[cnt] << endl;  // 連続する1の次の文字

	return 0;
}
