#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 三井住友信託銀行プログラミングコンテスト2019 https://atcoder.jp/contests/sumitrust2019/

int main(void)
{
	int N;
	string S;
	cin >> N >> S;

	// "000"を暗証番号に選べるか否か、"001"を暗証番号に選べるか否か、… で探していく
	int answer = 0;
	for(int i = 0; i < 1000; i++)
	{
		int figure = 100;  // 今どの桁を探しているか 100 or 10 or 1
		int num = (i / figure)%10;  // 今探している数
		for(int j = 0; (j < S.size()) && (figure > 0); j++)
		{
			if(S[j]-'0' == num)  // 探している数が見つかった
			{
				figure /= 10;  // 次の桁へ
				if(figure!=0) num = (i / figure)%10;
			}
		}
		if(figure == 0)  // 数値iを暗証番号に選べる
		{
			answer++;
		}
	}

	cout << answer << endl;

	return 0;
}
