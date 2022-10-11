#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題12より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc002

/*
 * bit全探索で解く。
 * 
 * know[i][j]:人i,jが知り合いならtrue
 * とし、入力時に知り合いはtrueに設定しておく。
 * 
 * 数値の各ビットを人に割り当て、
 * 1になっているビットの人どうしが全て知り合いかを判定する。
 * (例えば b'0001 1101 であれば、0,2,3,4番目の人が知り合いかを判定する)
 * これは2重ループでビットが1の組み合わせを全て判定すればよい。
 * 
 * 全て知り合いであれば、ビットが1の数が派閥の人数となり、
 * 0～(2^N)-1 の範囲で全探索したときにこの最大値が答となる。
 * 
 * 計算量はO(2^N*N^2).
 * 最大でN=12のため間に合う。
 * 
 * 余談だが、今回初めてラムダ式を使ってみた。
 * true/falseの判定を返す部分が楽に実装できた。
 */
int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	vector<vector<bool>> know(N, vector<bool>(N, false));  // know[i][j]:人i,jが知り合いならtrue
	for(i = 0; i < M; i++)
	{
		int x, y;
		cin >> x >> y;
		x--;  // 0-indexedに変換
		y--;
		know[x][y] = know[y][x] = true;
	}
	for(i = 0; i < N; i++) know[i][i] = true;  // 自分自身どうしは知り合い、とする

	// iで各ビットを人に割り当て、1になっている人たちがそれぞれ知り合いか判定する
	auto check = [&](int i) -> bool {
		for(int j = 0; j < N; j++)  // jビット目について判定
		{
			for(int k = j+1; k < N; k++)  // kビット目について判定
			{
				if( ((i>>j)&1) == 1 && ((i>>k)&1) == 1 )  // j,kビット目がともに1なら、j,k番目の人が知り合いか判定する
				{
					if(!know[j][k])
					{
						return false;
					}
					// ここに来たならば現時点で皆知り合い 処理継続
				}
			}
		}
		// 最後まで来たら、皆知り合い
		return true;
	};

	int answer = 0;
	for(i = 0; i < (1<<N); i++)  // 各bitを人に割り当て、bitが1の人たちがそれぞれ知り合いかを判定する
	{
		if(check(i))
		{
			answer = max(answer, __builtin_popcount(i));
		}
	}
	cout << answer << endl;

	return 0;
}
