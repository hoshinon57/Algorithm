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

// ABC320 https://atcoder.jp/contests/abc320

/*
 * 「どの数値で揃えるか」と「どの順にスロットを止めるか」を合わせて全探索する。
 * 後者はnext_permutation()を使う。
 * 
 * リールは3つなので、揃えられるなら3ループ目までには確実に揃えられる。
 * よって入力文字列を3倍にしておくと処理が楽。
 */

int main(void)
{
	int i;
	int M; cin >> M;
	vector<string> s(3);
	for(i = 0; i < 3; i++)
	{
		cin >> s[i];
		s[i] = s[i] + s[i] + s[i];  // リールは3つなので、揃えられるなら3ループ目までには確実に揃えられる
	}

	vector<int> jun{0, 1, 2};  // この順番でスロットを止める
	int ans = INF32;
	for(char c = '0'; c <= '9'; c++)  // 揃える数値
	{
		// 数値cで揃えられるか判定
		bool ok = true;
		for(i = 0; i < 3; i++)
		{
			if(s[i].find(c) == string::npos) ok = false;
		}
		if(!ok) continue;

		// 数値cで揃えるときの時間を計算する
		// これをスロットを止める順番について全探索する
		int pos = 0;
		do {
			pos = s[jun[0]].find(c);
			pos = s[jun[1]].find(c, pos+1);
			pos = s[jun[2]].find(c, pos+1);
			chmin(ans, pos);
		} while (next_permutation(jun.begin(), jun.end()));
	}
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
