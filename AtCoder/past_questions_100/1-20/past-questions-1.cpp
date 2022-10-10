#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題1
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_B&lang=ja

/*
 * 全探索で解く。
 * 3重ループで3つの数を選び、それらの和がxかどうかを判定していけばよい。
 * 計算量はO(n^3).
 */
int main(void)
{
	int i, j, k;
	int n, x;
	while(true)
	{
		cin >> n >> x;
		if(n==0 && x==0) break;

		int answer = 0;
		for(i = 1; i <= n; i++)
		{
			for(j = i+1; j <= n; j++)  // i+1スタートにより「重複無し」を実現する
			{
				for(k = j+1; k <= n; k++)  // j+1スタートにより「重複無し」を実現する
				{
					if(i+j+k == x) answer++;
				}
			}
		}
		cout << answer << endl;
	}

	return 0;
}
