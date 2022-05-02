#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC145 https://atcoder.jp/contests/abc145/
// 参考：再帰と深さ優先探索を用いた順列の生成
//   https://drken1215.hatenablog.com/entry/2020/05/04/190252

/*
 * 再帰と深さ優先探索を用いて、{0, 1, ..., N-1}までを使った順列を作成していく。
 * 再帰呼び出しの前にa[]の中身をチェックし、
 * 同じ数値がa[]に積まれないようにしておく。
 * 各順列ごとに距離とカウントを計算しておき、最後に距離/カウントを出力すればよい。
 */
int N;
vector<int> x, y;
double distSum = 0;
ll distCount = 0;
// 0, 1, ..., N-1 までの順列を再帰で作り、距離を計算する
void func(vector<int> &a)
{
	int i;
	if(a.size() == N)  // 順列を生成したので距離計算
	{
		for(i = 0; i < N-1; i++)
		{
			ll tmp1, tmp2;
			tmp1 = x[a[i]] - x[a[i+1]];
			tmp2 = y[a[i]] - y[a[i+1]];
			distSum += sqrt(tmp1*tmp1 + tmp2*tmp2);
		}
		distCount++;
	}

	// まだa[]に積んでいないものを探す
	vector<bool> b(N, false);
	for(i = 0; i < a.size(); i++)
	{
		b[a[i]] = true;
	}

	// 値を1つ追加し、再帰
	for(i = 0; i < N; i++)
	{
		if(b[i]) continue;  // iはa[]に積み済
		a.push_back(i);
		func(a);
		a.pop_back();
	}

	return;
}

int main(void)
{
	int i;
	cin >> N;
	x.resize(N);
	y.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
	}
	vector<int> a;
	func(a);

	cout << std::fixed << std::setprecision(10);
	cout << distSum/distCount << endl;
	return 0;
}
