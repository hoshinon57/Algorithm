#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC264 https://atcoder.jp/contests/abc264

/*
 * bit全探索で解く。
 * 
 * bitを行（または列）とみなして、1になっている部分を残すことを考える。
 * 例としてH1=5,H2=2のとき、"00110"であれば3,4行目を残す。
 * 
 * これを行であればH2の数だけ、列であればW2の数だけbitを1にして、
 * そこから切り出したものが行列Bと一致するかを判定すればよい。
 */

int H1, W1;
int H2, W2;
vector<vector<int>> a;
vector<vector<int>> b;

// aのうちbitが1の数を返す
int bitcount(int a)
{
	int ret = 0;
	while(a > 0)
	{
		if((a&1) == 1)  ret++;
		a = a>>1;
	}
	return ret;
}

// 行列Aについて以下の操作を行う
//   h_bitを行とみなして、1になっている部分を切り出す
//   w_bitを列とみなして、1になっている部分を切り出す
// これが行列Bと一致していればtrueを返す
bool check(int h_bit, int w_bit)
{
	int i, j;
	int i2, j2;

	i2 = 0;
	for(i = 0; i < H1; i++)
	{
		if(((h_bit>>i)&1) != 1) continue;  // h_bitが1になっている行を切り出す

		j2 = 0;
		for(j = 0; j < W1; j++)
		{
			if(((w_bit>>j)&1) != 1) continue;
			if(a[i][j] != b[i2][j2])
			{
				// 行列Aから切り出したものは、行列Bと不一致
				return false;
			}

			// ここまでは矛盾なし
			j2++;
		}
		i2++;
	}

	// 最後まで矛盾がなければ、切り出した行列が行列Bと一致するとみなせる
	return true;
}

int main(void)
{
	// 0-indexed
	int i, j;
	cin >> H1 >> W1;
	a.resize(H1, vector<int>(W1));  // a[H1][W1]
	for(i = 0; i < H1; i++)
	{
		for(j = 0; j < W1; j++)
		{
			cin >> a[i][j];
		}
	}
	cin >> H2 >> W2;
	b.resize(H2, vector<int>(W2));  // b[H2][W2]
	for(i = 0; i < H2; i++)
	{
		for(j = 0; j < W2; j++)
		{
			cin>> b[i][j];
		}
	}
	// ここまで入力

	bool answer = false;
	for(i = 0; i < (1<<H1); i++)
	{
		// iのbitを行とみなして、1になっている部分を残して行列Bと比較する
		if(__builtin_popcount(i) != H2) continue;  // H2の数だけbitが1になっている必要がある
//		if(bitcount(i) != H2) continue;  // H2の数だけbitが1になっている必要がある

		for(j = 0; j < (1<<W1); j++)
		{
			if(__builtin_popcount(j) != W2) continue;
//			if(bitcount(j) != W2) continue;

			// 選んだi,jで行列Aから切り出して、行列Bに一致するか
			if(check(i, j)) answer = true;
		}
	}

	if(answer)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
