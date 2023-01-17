#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題3より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc122/tasks/abc122_b

/*
 * 入力文字列の先頭から見ていって、i番目の文字が
 *   ACGT文字列に含まれるなら、countをインクリメント
 *   ACGT文字列に含まれないなら、countを0にリセット
 * する。
 * countの最大値が答となる。
 * 
 * 計算量は、入力Sの長さをNとしてO(N).
 * ※もしくは、ACGT文字列の長さをMとしてO(MN).
 */

int main(void)
{
	// 0-indexed
	string s;
	cin >> s;

	string acgt = "ACGT";
	int answer = 0;
	int count = 0;
	for(int i = 0; i < (int)s.size(); i++)
	{
		if(acgt.find(s[i]) != string::npos)  // 入力のi番目の文字が、"ACGT文字列"に含まれる文字か
		{
			count++;
			answer = max(answer, count);
		}
		else
		{
			count = 0;
		}
	}
	cout << answer << endl;

	return 0;
}
