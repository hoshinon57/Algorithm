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
 * past-questions-1の計算量改善版。
 * 
 * 重複しない3つの数をa,b,c(a<b<c)とする。
 * a,bについて全探索すると、c=x-a-bとなる。
 * このcが、cとしての条件(※)を満たせば組み合わせOKと判断できる。
 * (※)b<c, かつc<=n
 * 
 * 計算量はO(n^2).
 */
int main(void)
{
	int a, b, c;
	int n, x;
	while(true)
	{
		cin >> n >> x;
		if(n==0 && x==0) break;

		int answer = 0;
		// a<b<cとする
		for(a = 1; a <= n-2; a++)
		{
			for(b = a+1; b <= n-1; b++)
			{
				// a+b+c=xとしたときに、cとしての条件を満たすか
				c = x - a - b;
				if(b < c && c <= n) answer++;
			}
		}
		cout << answer << endl;
	}

	return 0;
}
