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

// ABC048 https://atcoder.jp/contests/abc048

/*
 * ゲームの問題だけどDPじゃないやつ。試行錯誤して法則を見つけ出して解けた。
 * AC後にWEBを見て参考にした記事は以下。
 *   公式解説
 *   https://drken1215.hatenablog.com/entry/2019/02/02/181600
 *   https://scrapbox.io/hkurokawa-cp/ABC_048_D_-_An_Ordinary_Game
 * 
 * 色々とガチャガチャ試してみると、どうも以下4パターンっぽい。
 * 1)a(文字数odd)a
 *   偶数回しか取れない、後手勝ち。
 *   先手が勝てる条件無さそう。
 * 2)a(文字数even)a
 *   奇数回しか取れない、先手勝ち。
 *   先手が勝てる条件無さそう。
 * 3)a(文字数odd)b
 *   奇数回しか取れない、先手勝ち。
 * 4)a(文字数even)b
 *   偶数回しか取れない、後手勝ち。
 * なので先頭末尾の文字の一致不一致と、文字列長からO(1)で求まりそう、と試したらAC.
 * 解説を見たら方針も合ってた。
 * 
 * ちゃんとした証明としては、最終形は
 *   ababa  (abaを含む)
 *   ababab  (abを含む)
 * の2パターンとなる。
 * 先頭末尾が同じなら最終形は奇数になる(偶数では終わらない)ので、入力の文字列長で決まる。
 * 先頭末尾が異なるなら最終形は偶数位なるので、同様。
 * となる。
 * 
 * [ACまでの思考の流れ]
 * ・ゲームDPかなあと考えたが、文字が減っていくのでDPで上手く表現できなさそう。
 * ・初手、もしくは1,2手実施ぐらいで、まあつまりO(1)で計算できたりしないかな？
 *   ⇒色々とがちゃがちゃやってたら法則が見えてきた
**/

// 先手勝つならtrue
bool solve(void)
{
	string s; cin >> s;
	if(s[0] == s.back())
	{
		return ((int)s.size()%2 == 0);
	}
	else
	{
		return ((int)s.size()%2 == 1);
	}
}

int main(void)
{
	string ans;
	if(solve()) ans = "First";
	else ans = "Second";
	cout << ans << endl;

	return 0;
}
