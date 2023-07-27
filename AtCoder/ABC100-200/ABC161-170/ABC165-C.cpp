#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC165 https://atcoder.jp/contests/abc165

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * 条件を満たす数列を全て作成し、それぞれについて得点を計算する。
 * その最高点が答となる。
 */

int N, M, Q;
vector<int> a, b, c, d;
ll maxScore = 0;

// 長さNの数列arrについて得点計算する
void calc(vector<int> &arr)
{
	ll score = 0;
	for(int i = 0; i < Q; i++)
	{
		if(arr[b[i]] - arr[a[i]] == c[i]) score += d[i];
	}
	maxScore = max(maxScore, score);
}

// 再帰を用いて数列を作成し、数列が完成したら得点計算する
void func(vector<int> &arr)
{
	if(arr.size() == N)  // 数列が完成したら得点計算
	{
		calc(arr);
		return;
	}

	// 現状の作成途中の数列に対して、
	// 末尾以上の値を追加して再帰呼び出し
	int arrLast;
	if(arr.size() != 0)  arrLast = arr[arr.size()-1];
	else  arrLast = 1;  // Aが空の場合、1以上を追加できる
	for(int i = arrLast; i <= M; i++)
	{
		arr.push_back(i);
		func(arr);
		arr.pop_back();
	}
}

int main(void)
{
	cin >> N >> M >> Q;
	a.resize(Q);
	b.resize(Q);
	c.resize(Q);
	d.resize(Q);
	for(int i = 0; i < Q; i++)
	{
		cin >> a[i] >> b[i] >> c[i] >> d[i];
		// a[]とb[]は0-indexedに変換する
		a[i]--;
		b[i]--;
	}

	vector<int> arr;  // 添え字は0-indexed
	func(arr);
	// arrayのサイズがNになったら数列完成、スコア計算する

	cout << maxScore << endl;

	return 0;
}
