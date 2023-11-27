#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

/*
 * その他のライブラリ
 * 
 */

// 要素がN*Nであるaについて、右に90度回転させる
void rotate(vector<vector<int>> &a)
{
	int n = a.size();
	auto a_bk = a;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			a[i][j] = a_bk[n-1-j][i];
		}
	}
}

int main(void)
{
	vector<vector<int>> a(4, vector<int>(4)), b(4, vector<int>(4));
	a = {{1,0,0,0},
	     {1,1,0,0},
		 {1,0,2,0},
		 {1,2,2,2}};
	b = {{1,1,1,1},
	     {2,0,1,0},
		 {2,2,0,0},
		 {2,0,0,0}};
	rotate(a);
	assert(a == b);

	return 0;
}
