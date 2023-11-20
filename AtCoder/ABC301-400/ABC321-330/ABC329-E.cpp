#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC329 https://atcoder.jp/contests/abc329

/*
 * 基本は公式解説と同じ考え方。(自分の最初のACよりもずっとスマートであり、実装しなおした)
 *   https://atcoder.jp/contests/abc329/editorial/7724
 * 
 * 逆方向に見ていって、文字列Sを"*"に置き換えていく、で考えてみる。
 * Sの部分文字列がTと一致したら、そこは"*"で置き換えることができる。
 * このとき置き換えた部分の前後について(1文字以上重なる部分)、以下の方針でさらにTと比較する。
 *   SとTで文字が一致するか、Sが"*"ならその文字はOK.
 *   M文字ぶんOKか？
 * これをキューを使って比較を繰り返していく。
 * 
 * 最終的にSが"***...***"と一致すればYes.
 * 
 * [ACまでの思考の流れ]
 * ・入力例に対し、どう操作すれば目的の文字列になるか、
 *   できない場合はなぜできないか(何がネックか)を考えた。
 *   それを元にアルゴリズムを検討していった。
 * ・Tに置き換えた部分があれば、そこは比較対象から外してよい。
 *   なぜなら後から置き換えることができるため。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	string s, t; cin >> s >> t;
	queue<int> que;

	// s[p]とt[0]を起点に、「文字一致 or sが"*"」ならOKとして比較していく
	// M文字ぶんOKであれば、sを"*"に置き換えてキューに積む
	auto check = [&](int p)
	{
		if(s.substr(p, M) == string(M, '*')) return;  // 全て"*"なら何もしない
		int ii;
		for(ii = 0; ii < M; ii++)
		{
			if(s[p+ii] == '*') continue;
			if(s[p+ii] != t[ii]) return;  // NG
		}
		for(ii = 0; ii < M; ii++) { s[p+ii] = '*'; }
		que.push(p);
	};

	for(i = 0; i < N-M+1; i++) { check(i); }  // 最初は全箇所をチェック
	while(!que.empty())
	{
		int pos = que.front();
		que.pop();
		for(i = pos-M+1; i < pos+M-1; i++)
		{
			if(i < 0 || i+M >= N) continue;  // 範囲外
			check(i);
		}
	}

	YesNo(s == string(N, '*'));

	return 0;
}
