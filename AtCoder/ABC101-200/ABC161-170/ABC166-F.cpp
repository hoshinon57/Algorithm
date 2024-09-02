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

// ABC166 https://atcoder.jp/contests/abc166

/*
 * 正直なところ乗り気になれず…hamayanhamayan氏の解説を見てACしてしまった。
 *   https://blog.hamayanhamayan.com/entry/2020/05/03/224516
 * 
 * 再帰で全探索していく方針。実行時間104msと、十分に間に合った。
 * ----------------
 *   OKなときは割と適当な手順にしてもOKだし、
 *   NGなときは間違った方向へ進むと1,2手でNGになるし(間違った選択をしてから早めに結果が分かる)、
 *   再帰で全探索しても、ダメなルートに行ってもすぐ戻ってくるので、全体としてほぼO(N)で処理できそう。
 * ----------------
 * こんな感じがする。
 * 
 * ちゃんとやるなら以下解説のような方針か。
 * -------------------
 *   A+B+C=0はNG
 *   A+B+C=1は現在のクエリを愚直にやるしかない
 *   A+B+C>=2の場合、
 *     初手で(0,0)のNGは仕方ない
 *     (1,1)のとき、1つ先のクエリを読んで足し引きする方に加算
 *     それ以外は大小関係で決めればうまくいく
 * -------------------
 * https://img.atcoder.jp/abc166/editorial.pdf
 * https://kmjp.hatenablog.jp/entry/2020/05/03/1100
 * https://atcoder.jp/contests/abc166/submissions/12738481 hitonanode氏の実装 解説に近そうだった
 * https://at274.hatenablog.com/entry/2020/05/21/222634
 * https://scrapbox.io/procon-kirokuyou/ABC166_F_-_Three_Variables_Game_(600)
 */

vector<string> s;
int N;

bool dfs(int cnt, int a, int b, int c, string &ans)
{
	if(cnt == N) return true;  // ベースケース
	if(s[cnt] == "AB")
	{
		if(a == 0 && b == 0) return false;
		if(a != 0)  // Aを引いてBを足す
		{
			ans.push_back('B');
			if(dfs(cnt+1, a-1, b+1, c, ans)) return true;
			else ans.pop_back();
		}
		// elseではなく、別個に試す
		if(b != 0) // Aを足してBを引く
		{
			ans.push_back('A');
			if(dfs(cnt+1, a+1, b-1, c, ans)) return true;
			else ans.pop_back();
		}
	}
	else if(s[cnt] == "BC")
	{
		if(b == 0 && c == 0) return false;
		if(b != 0)  // Bを引いてCを足す
		{
			ans.push_back('C');
			if(dfs(cnt+1, a, b-1, c+1, ans)) return true;
			else ans.pop_back();
		}
		if(c != 0) // Bを足してCを引く
		{
			ans.push_back('B');
			if(dfs(cnt+1, a, b+1, c-1, ans)) return true;
			else ans.pop_back();
		}
	}
	else  // AC
	{
		if(a == 0 && c == 0) return false;
		if(a != 0)  // Aを引いてCを足す
		{
			ans.push_back('C');
			if(dfs(cnt+1, a-1, b, c+1, ans)) return true;
			else ans.pop_back();
		}
		if(c != 0) // Aを足してCを引く
		{
			ans.push_back('A');
			if(dfs(cnt+1, a+1, b, c-1, ans)) return true;
			else ans.pop_back();
		}
	}
	return false;
}

int main(void)
{
	int i;
	int A, B, C;  cin >> N >> A >> B >> C;
	s.resize(N);
	for(i = 0; i < N; i++) cin >> s[i];
	string ans;
	if(dfs(0, A, B, C, ans))
	{
		cout << "Yes" << endl;
		for(char c : ans) cout << c << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
