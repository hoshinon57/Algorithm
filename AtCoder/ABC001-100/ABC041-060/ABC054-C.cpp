#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC054 https://atcoder.jp/contests/abc054/
// 参考：再帰と深さ優先探索を用いた順列の生成
//   https://drken1215.hatenablog.com/entry/2020/05/04/190252

/*
 * 再帰と深さ優先探索を用いて、{0, 1, ..., N-1}までを使った順列を作成していく。
 * 再帰呼び出しの前にa[]の中身をチェックし、
 * 同じ数値がa[]に積まれないようにしておく。
 * 作成した順列を頂点の並びとみなしたときに、各頂点が繋がっていれば問題の条件を満たす。
 */
int N, M;
vector<vector<bool>> path;  // 0-indexed  path[0][1]=trueなら、頂点(1)と(2)は繋がっている
int count_ = 0;  // 条件を満たすパスの数
// 0, 1, 2, ..., N-1までの順列を再帰で作成する
void func(vector<int> &a)
{
	int i;
	if(a.size() == N)  // 順列を生成したので、これが条件を満たすか確認
	{
		bool check = true;
		for(i = 0; i < N-1; i++)
		{
			// 2つの頂点 a[i],a[i+1] が繋がっているか
			if(! (path[a[i]][a[i+1]]) )
			{
				check = false;  // 繋がっていない
				break;
			}
		}
		if(check) count_++;
		return;
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

	/*
	int j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(path[i][j]) cout << "1";
			else cout << "0";
		}
		cout << endl;
	}
	*/

	vector<int> a;  // 作成した順列
	a.push_back(0);  // 始点は必ず頂点(1)なので、積んでおく
	func(a);

	cout << count_ << endl;

	return 0;
}
