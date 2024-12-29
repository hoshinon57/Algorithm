#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// next_permutationを用いた、辞書順での順列生成の練習
// 参考 https://qiita.com/drken/items/872ebc3a2b5caaa4a0d0  の順列全探索
//      https://qiita.com/siser/items/a91022071b24952d27d9
//      https://qiita.com/Nikkely/items/0ddca51b3c0e60afbaab

/*
 * [関連する問題]
 * ABC232-C 
 * ABC268-D D問題にしては難易度高めだが、良問だった
 * ABC276-C prev_permutation
 * ABC302-C 
 * ABC328-E M個の辺からN-1個を選ぶ
 * ABC345-D
 * ABC369-E ワーシャルフロイド, bit全探索と絡めた良問
 * ABC371-C 難しかったが良問
 * ABC374-D
 */

/*
 * [memo]
 * {1,2,3}という順列だと、
 * {1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1} の順で6つ生成される。
 * 一方で{1,1,2}という順列だと、
 * {1,1,2},{1,2,1},{2,1,1} の3つが生成される。
 * 
 * 辞書順と逆方向で生成していく prev_permutation() という関数もある。
 */
/*
 * string型や、vector<string>型でもnext_permutation()に渡すことができる。
 * ただし事前に昇順ソートしておく必要がある。
 * 参考：ABC215-C, ABC302-C
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> a(N);
#if 0
	for(i = 0; i < N; i++)  // 初期値設定 昇順に並べる
	{
		a[i] = i+1;
	}
#else
	iota(a.begin(), a.end(), 0);  // 初期値設定  0から昇順に並べる  #include <numeric>が必要
#endif
	
	int count = 0;  // 生成した順列の数
	do
	{
		for(i = 0; i < N; i++)  // a[i]:i番目に見るべき要素の番号
		{
			cout << a[i] << " ";
		}
		cout << endl;
		count++;
	} while (next_permutation(a.begin(), a.end()));

	cout << "permutation num:" << count << endl;

	return 0;
}
