#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC237 https://atcoder.jp/contests/abc237

/*
 * 文字列の先頭と末尾を指す変数head/tailを用意し、1文字ずつ判定していく。
 * 
 * S[head]==S[tail]の場合、headを1文字後ろに、tailを1文字手前にずらす。
 * 
 * S[head]!=S[tail]の場合、条件分けする
 * ・S[tail]が'a'の場合、文字列先頭にaを付与することでS[head]==S[tail]を満たせる。
 *   ただし付与できるのは文字列先頭のみのため、
 *   これまで末尾にa以外があったら付与NG. (文字列の途中にaを付与することになってしまうため)
 *   a以外が無かったら付与OK.
 * ・S[tail]が'a'以外の場合、回文にはできない。
 */
int main(void)
{
	string S;
	cin >> S;
	bool canAddA = true;  // 先頭にaを付与しても良いか
	bool bOK = true;

	int head = 0;
	int tail = S.size()-1;

	while(head <= tail)
	{
//		cout << "head:" << S[head] << " tail:" << S[tail] << endl;
		// 一度でも末尾にa以外が来たら、以降はaを付与できない
		if(S[tail] != 'a') canAddA = false;

		if(S[head] == S[tail])  // 一致する場合、1文字ずつずらす
		{
			head++;
			tail--;
		}
		else
		{
			if(S[tail] == 'a' && canAddA)  // 一致しなくても、末尾がaかつa付与できるなら、処理継続
			{
				// 先頭にaを付与する  この場合、headの位置はそのまま
				tail--;
			}
			else  // 回文にはできない
			{
				bOK = false;
				break;
			}
		}
	}

	if(bOK)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
