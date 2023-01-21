#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題6
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/sumitrust2019/tasks/sumitb2019_d

/*
 * 文字列Sから3桁を選ぶ手順を全探索するのは、選び方が (N)C(3) でO(N^3)となりTLE.
 * 違う考え方で、3桁の数値について全探索し「番号XXXは暗証番号にできるか？」を判定する。
 * 
 * 計算量はO(N).
 * もしくは、探索する数値の種類をM(=1000)としてO(MN).
 */

int main(void)
{
	int i, j;
	int N;
	string S;
	cin >> N;
	cin >> S;

	int answer = 0;
	for(i = 0; i < 1000; i++)  // iを暗証番号として設定できるか判定する
	{
		// iをpw[0]～[2]に1文字ずつ設定する
		int pw[3];
		int tmp_i = i;
		for(j = 2; j >= 0; j--)
		{
			pw[j] = tmp_i%10;
			tmp_i /= 10;
		}

		// 入力の先頭から見て行って、no[0]～[2]までその順で一致するものがあるか
		int no = 0;
		for(auto &e : S)
		{
			if(e-'0' == pw[no]) no++;
		}
		if(no == 3) answer++;  // 暗証番号として設定可能
	}
	cout << answer << endl;

	return 0;
}
