#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 再帰関数を用いた深さ優先探索の練習
// 参考 https://drken1215.hatenablog.com/entry/2020/05/04/190252
//      https://qiita.com/drken/items/872ebc3a2b5caaa4a0d0  の順列全探索

/*
 * [関連する問題]
 * ABC015-C https://atcoder.jp/contests/abc015/tasks/abc015_3
 * ABC232-C https://atcoder.jp/contests/abc232/tasks/abc232_c
 * ABC236-D https://atcoder.jp/contests/abc236/tasks/abc236_d (ペアを作る)
 * ABC268-D https://atcoder.jp/contests/abc268/tasks/abc268_d (D問題にしては難易度高めだが、良問だった)
 * ABC310-D https://atcoder.jp/contests/abc310/tasks/abc310_d (これ、コンテスト中に解けなかったのは反省)
 * ABC318-D https://atcoder.jp/contests/abc318/tasks/abc318_d (ペアを作る＆Nが奇数のケース有, 解けなかったが良問)
 * ABC328-E https://atcoder.jp/contests/abc328/tasks/abc328_e (M個の辺からN-1個を選ぶ recursive_dfs_3()が参考)
 * ABC367-C https://atcoder.jp/contests/abc367/tasks/abc367_c
 * ABC382-D https://atcoder.jp/contests/abc382/tasks/abc382_d
 * ABC390-D https://atcoder.jp/contests/abc390/tasks/abc390_d (ベル数)
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
 * used[i]:要素iをa[]に積んだらtrue. 呼び出し元にて全要素をfalseに初期化しておくこと。
 * next_permutation()というライブラリもあるようだが、使わないで実装してみた。
 */
void recursive_dfs_2(vector<int> &a, vector<bool> &used)
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

	// 再帰の処理  ★問題に合わせて再帰の条件を実装する
	for(i = 0; i < NUM2; i++)
	{
		if(used[i]) continue;  // iはa[]に積み済
		used[i] = true;
		a.push_back(i);
		recursive_dfs_2(a, used);
		a.pop_back();
		used[i] = false;
	}

	return;
}

const int M3 = 5;
const int N3 = 3;
// {0,1,2,...,M}から重複無しでN個選ぶ  CombinationのMCN
// nxt:次に選ぶ/選ばないを決める番号
// use:これまでに選んだ番号の一覧
// cnt:これまでに選んだ番号の個数
void recursive_dfs_3(int nxt, vector<int> &use, int cnt)
{
	if(cnt == N3)  // N個選んだので処理
	{
		for(auto &e : use) { cout << e << " "; }
		cout << endl;
		return;
	}
	if(nxt == M3) return;  // 最後まで見た
	// 枝刈り 今後全ての要素を選んでもN個に足りない場合、return
	// ただしABC328-Eではこの枝刈りの有無で実行時間はほとんど変わらなかった
	if(cnt + (M3-nxt) < N3) return;

	// nxtを選ばない場合
	recursive_dfs_3(nxt+1, use, cnt);
	// nxtを選ぶ場合
	use.push_back(nxt);
	recursive_dfs_3(nxt+1, use, cnt+1);
	use.pop_back();
}

const int N4 = 5;
const vector<int> a4 = {1, 2, 3, 4, 5};
// a[0],a[1],...a[N-1]のN要素を、重複無しで、グループ分けする
// グループ数は1～N通りとなる
// ベル数と呼ぶらしい (N=12で4*10^6ほど)
// 
// li[m]:現在のm番目のグループに属する要素のリスト
// idx:次に見るa[idx]
void recursive_dfs_4_BellNumber(vector<vector<int>> &li, int idx)
{
	if(idx == N4) {  // 最後まで見たので出力
		cout << "{";
		for(int i = 0; i < (int)li.size(); i++)
		{
			cout << "{";
			for(int j = 0; j < (int)li[i].size(); j++)
			{
				cout << li[i][j];
				if(j != (int)li[i].size()-1) cout << ",";
			}
			cout << "}";
			if(i != (int)li.size()-1) cout << ",";
		}
		cout << "}" << endl;
		return;
	}
	// 既存のグループに入れるパターン
	for(int i = 0; i < (int)li.size(); i++)
	{
		li[i].push_back(a4[idx]);
		recursive_dfs_4_BellNumber(li, idx+1);
		li[i].pop_back();
	}
	// 新しいグループを作って入れるパターン
	li.push_back({a4[idx]});
	recursive_dfs_4_BellNumber(li, idx+1);
	li.pop_back();
}

int main(void)
{
	vector<int> a;
	recursive_dfs(a);

	cout << "-----" << endl;
	vector<int> a2;
	vector<bool> used(NUM2, false);
	recursive_dfs_2(a, used);

	cout << "-----" << endl;
	vector<int> use;
	recursive_dfs_3(0, use, 0);

	cout << "-----" << endl;
	vector<vector<int>> li;
	recursive_dfs_4_BellNumber(li, 0);
	return 0;
}
