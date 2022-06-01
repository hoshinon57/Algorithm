#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// パナソニックプログラミングコンテスト2020 https://atcoder.jp/contests/panasonic2020

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * 同型ではない文字列、を全列挙していく。そのためには
 *   1文字目：0のみ選べる
 *   n文字目：0～(n-1)文字目までの最大値+1 が選べる
 * として数列を作成すればよい。
 * 
 * 数列を作成したら、それを標準形にするには
 * 0をa, 1をb, 2をc, ... に対応させればよい。
 */

int N;

void func(vector<int> &arr)
{
	if(arr.size() == N)  // 数列完成
	{
		// 0をa, 1をb, ... に対応させて出力
		for(auto e : arr) cout << (char)(e+'a');
		cout << endl;
		return;
	}

	// arr[]の中の最大値を求める (N==0にも対応済)
	int arrMax = -1;
	for(auto e : arr)  arrMax = max(arrMax, e);

	// 0～(arr[]の中の最大値+1) までを付与して再帰
	for(int i = 0; i <= arrMax+1; i++)
	{
		arr.push_back(i);
		func(arr);
		arr.pop_back();
	}
}

int main(void)
{
	cin >> N;

	vector<int> arr;
	func(arr);

	return 0;
}
