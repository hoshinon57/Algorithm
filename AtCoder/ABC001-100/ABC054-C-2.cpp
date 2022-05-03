#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC054 https://atcoder.jp/contests/abc054/
// 順列全探索ではなく、繋がっている頂点の一覧を深さ優先探索で進んでいく版

/*
 * 現状のa[]の末尾の頂点から繋がっている頂点のうち、まだ行っていない頂点をリストアップして
 * 深さ優先探索で進んでいく。
 * a[]の長さがNになれば、全ての頂点を1度だけ訪れるパスとみなせる。
 */
int N, M;
vector<vector<bool>> path;  // 0-indexed  path[0][1]=trueなら、頂点(1)と(2)は繋がっている
int count_ = 0;  // 条件を満たすパスの数
void func(vector<int> &a)
{
	int i;
	if(a.size() == N)  // a[]の長さがNになれば、全ての頂点を1度だけ訪れるパスとみなせる
	{
		count_++;
		return;
	}

	// 現状のa[]の末尾の頂点から繋がっている頂点のうち、まだ行っていない頂点をリストアップする
	vector<int> next;
	for(i = 0; i < N; i++)
	{
		if(path[a[a.size()-1]][i])  // a[]末尾の頂点と、頂点iが繋がっているか
		{
			if(find(a.begin(), a.end(), i) == a.end())  // 頂点iはまだ行っていないか
			{
				// 頂点iに進める
				a.push_back(i);
				func(a);
				a.pop_back();
			}
		}
	}

	return;
}

int main(void)
{
	int i;
	cin >> N >> M;
	path.resize(N);
	for(i = 0; i < N; i++) path[i].resize(N, false);

	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		path[a-1][b-1] = true;
		path[b-1][a-1] = true;  // a<bのため、反対側も設定しておく
	}

	vector<int> a;  // 作成した順列
	a.push_back(0);  // 始点は必ず頂点(1)なので、積んでおく
	func(a);

	cout << count_ << endl;

	return 0;
}
