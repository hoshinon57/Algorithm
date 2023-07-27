#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC264 https://atcoder.jp/contests/abc264

/*
 * "atcoder"の文字をそれぞれ0～6に割り当てて、入力文字列を変換する。
 * これをバブルソートで昇順にする処理を実装し、
 * その際の交換回数が答となる。
 */

const int str_len = 7;  // "atcoder"の文字数

int main(void)
{
	int i, j;
	string S;
	cin >> S;
	vector<int> array(str_len);
#if 0
	for(i = 0; i < str_len; i++)
	{
		switch(S[i])
		{
		case 'a':
			array[i] = 0;
			break;
		case 't':
			array[i] = 1;
			break;
		case 'c':
			array[i] = 2;
			break;
		case 'o':
			array[i] = 3;
			break;
		case 'd':
			array[i] = 4;
			break;
		case 'e':
			array[i] = 5;
			break;
		case 'r':
			array[i] = 6;
			break;
		}
	}
#else
	string atcoder = "atcoder";
	for(i = 0; i < str_len; i++)
	{
		array[i] = S.find(atcoder[i]);
	}
#endif

	int answer = 0;
	for(i = 0; i < str_len-1; i++)
	{
		for(j = 0; j < str_len-1-i; j++)
		{
			if(array[j] > array[j+1])
			{
				swap(array[j], array[j+1]);
				answer++;
			}
		}
	}
	cout << answer << endl;

	return 0;
}
