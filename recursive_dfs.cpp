#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 再帰関数を用いた深さ優先探索の練習
// 参考 https://drken1215.hatenablog.com/entry/2020/05/04/190252
//      https://qiita.com/drken/items/872ebc3a2b5caaa4a0d0  の順列全探索

/*
 * [関連する問題]
 * ABC232-C https://atcoder.jp/contests/abc232/tasks/abc232_c
 * ABC268-D https://atcoder.jp/contests/abc268/tasks/abc268_d (D問題にしては難易度高めだが、良問だった)
 * ABC310-D https://atcoder.jp/contests/abc310/tasks/abc310_d (これ、コンテスト中に解けなかったのは反省)
 */

const int NUM = 3;
const int DIGIT = 5;
/*
 * 5桁表示で、各桁0～2を取る数値を列挙する(3^5通り)方法を、
 * 再帰を用いた深さ優先探索で実装する。
 * 最上位の桁から処理していくイメージ
 */
void recursive_dfs(vector<int> &a)
{
	int i;
	if(a.size() == DIGIT)  // 必要な桁ぶん積んだので処理  ★ここを問題に合わせて実装する
	{
		for(i = 0; i < DIGIT; i++) cout << a[i];
		cout << endl;
		return;
	}

	// 再帰の処理  ★問題に合わせて再帰の条件を追加する
	for(i = 0; i < NUM; i++)
	{
		a.push_back(i);
		recursive_dfs(a);
		a.pop_back();
	}

	return;
}

const int NUM2 = 5;
const int dispNum[NUM2] = {1, 2, 3, 4, 5};
/*
 * 1,2,3,4,5 を用いた順列を全て列挙する。
 * a[]に要素番号を積み、順列の表示はdispNum[]を使う。
 * next_permutation()というライブラリもあるようだが、使わないで実装してみた。
 */
void recursive_dfs_2(vector<int> &a)
{
	int i;
	if(a.size() == NUM2)  // 順列を生成したので処理  ★ここを問題に合わせて実装する
	{
		for(i = 0; i < NUM2; i++)
		{
			cout << dispNum[a[i]] << ",";
		}
		cout << endl;
		return;
	}

	// まだa[]に積んでいないものを探す
	vector<bool> b(NUM2, false);
	for(i = 0; i < a.size(); i++)
	{
		b[a[i]] = true;
	}

	// 再帰の処理  ★問題に合わせて再帰の条件を実装する
	for(i = 0; i < NUM2; i++)
	{
		if(b[i]) continue;  // iはa[]に積み済
		a.push_back(i);
		recursive_dfs_2(a);
		a.pop_back();
	}

	return;
}

int main(void)
{
	vector<int> a;
	recursive_dfs(a);

	vector<int> a2;
	recursive_dfs_2(a);

	return 0;
}
