#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 「上級者が解くべき過去問精選 100 + 50 問」の問題135
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0365&lang=ja

/*
 * BITを用いて解く。
 * 
 * まず、各アルファベットについて「文字列sの何文字目に登場するか」をキューに登録する。
 * sが"cbacba"なら
 *   a:3,6
 *   b:2,5
 *   c:1,4
 * のような感じで。
 * 
 * 辞書順なので「前から貪欲」がセオリー。
 * sのうち「何番目の文字を先頭に移動させるか」を考える。
 * この際、「i番目の文字が未使用か、使用済み（先頭に移動済み）か」を管理したい。
 * これをBITで管理する。
 * 最初は全て未使用なので、bit.Add(1,1), Add(2,1), ..., (n,1) としておく。
 * 
 * kは「あと何回交換できるか」を示すので、
 * 各アルファベットについて先頭に持ってこれるかを'a'～'z'それぞれ判定する。
 * 持ってこれるものがあれば、
 *   BITでその文字を「使用済み」にする
 *   キューから取り除く
 *   先頭に移動する文字数ぶん、kを減算
 * していく。
 * 
 * 計算量は、sの長さをS, アルファベットの種類をT(=26)とすると、
 * O(STlogS)かと考える。
 */

// Binary Indexed Treeを用いて以下(1)(2)をO(logN)で計算する
//   (1)iが与えられたとき、累積和A1+A2+...+Aiを計算
//   (2)iとxが与えられたとき、Aiにxを加算する
// [注意]
//   1-indexed
//   Sum()は閉区間で処理する
//   要素数nは2のべき乗でなくても良い
// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
template <typename T>
struct BIT
{
	vector<T> array;  
	const int n;  // 1-indexedでA1～Anまでの数列を扱う

	BIT(int n_) : array(n_+1, 0), n(n_) {}  // 1-indxedのため配列の要素数は+1して確保
	
	// 1番目からi番目までの累積和を求める
	T Sum(int i)
	{
		T s = 0;
		while(i > 0)
		{
			s += array[i];
			i -= i & (-i);  // LSBを減算
		}
		return s;
	}

	// [i,j]の要素の累積和を求める
	T Sum(int i, int j)
	{
		T s1 = Sum(i-1);
		T s2 = Sum(j);
		return s2 - s1;
	}

	// i番目の要素にxを加算
	void Add(int i, T x)
	{
		while(i <= n)
		{
			array[i] += x;
			i += i & (-i);  // LSBを加算
		}
	}
};

int main(void)
{
	// 1-indexed
	int i, j;
	string s;
	int k;
	cin >> s >> k;

	queue<int> alp_que[26];  // 各アルファベットが文字列sの何文字目に出てくるかをキューで管理
	for(i = 1; i <= (int)s.size(); i++)  // 1-indexed
	{
		alp_que[(int)(s[i-1] - 'a')].push(i);
	}

	BIT<int> bit(s.size());  // 文字列sのi文字目が未使用なら、[i]=1
	// まずは全文字を「未使用」にする
	for(i = 1; i <= (int)s.size(); i++)  // 1-indexed
	{
		bit.Add(i,1);
	}

	string answer = "";
	for(i = 1; i <= (int)s.size(); i++)
	{
		for(j = 0; j < 26; j++)
		{
			if(alp_que[j].empty()) continue;  // j番目のアルファベットはもう残っていない
			auto idx = alp_que[j].front();  // j番目のアルファベットが、何文字目に出てくるか
			if(bit.Sum(idx)-1 <= k)  // idx番目の文字を先頭に持ってこれる？
			{
				// 答の文字列に追加して、kを減算
				answer += (char)(j + 'a');
				k -= bit.Sum(idx)-1;
				bit.Add(idx, -1);  // idx文字目は「使用済み」として0に

				alp_que[j].pop();
				break;  // 次の文字へ
			}
		}
	}
	cout << answer << endl;

	return 0;
}
