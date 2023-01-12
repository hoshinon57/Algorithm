#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題92
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1193&lang=jp

/*
 * 1つのデータセットに対し、以下のフェーズに分ける。
 * (1)入力
 * (2)石を消す
 * (3)石の落下
 * (2)(3)は「これ以上消せない」まで繰り返す。
 * 上記をそれぞれ input(), erase(), drop() にて実装した。
 * 
 * 石の配置は pzl[y][x] にて持つ。
 * (2)にて「3個以上並んでいる」を簡単に実装するため、横方向の末尾に番兵を用意した。
 * 
 * あとはこれをゴリゴリと実装していけばよい。
 */

int H;
const int W = 5;
vector<vector<int>> pzl;  // pzl[y][x]

void input(void)
{
	pzl.resize(H, vector<int>(W+1));  // pzl[H][W]  W方向には+1の番兵を用意

	for(int y = 0; y < H; y++)
	{
		for(int x = 0; x < W; x++)
		{
			cin >> pzl[y][x];
		}
		pzl[y][W] = -1;  // 番兵
	}
}

// 石を消去し、得られるスコアを返す
int erase(void)
{
	int score = 0;
	for(int y = 0; y < H; y++)
	{
		// 横方向に同じ値が連続する個数を調べる
		int prev = pzl[y][0];
		int count = 1;  // 同じ値の石が連続する数
		for(int x = 1; x < W+1; x++)  // +1は番兵
		{
			if(prev == pzl[y][x])  // 同じ値が連続している
			{
				count++;
			}
			else  // 異なる値
			{
				if(count >= 3)  // 3個以上並んでいるので、消してスコア加算
				{
					// 石を消す  (x-count)から(x-1)までのcount個
					for(int k = x-count; k <= x-1; k++)
					{
						pzl[y][k] = 0;
					}
					score += prev*count;
				}
				// カウントしなおし
				prev = pzl[y][x];
				count = 1;
			}
		}
	}

	return score;
}

// 石を落とす処理
void drop(void)
{
	for(int x = 0; x < W; x++)
	{
		for(int y = H-1; y >= 0; y--)
		{
			if(pzl[y][x] == 0)  // 消えてる
			{
				// 自分より上のセルから、次の石を探してswap
				for(int k = y-1; k >= 0; k--)
				{
					if(pzl[k][x] != 0)
					{
						swap(pzl[y][x], pzl[k][x]);
						break;
					}
				}
			}
		}
	}
}

// デバッグ用
void print(void)
{
	for(int y = 0; y < H; y++)
	{
		for(int x = 0; x < W; x++)
		{
			cout << pzl[y][x] << " ";
		}
		cout << endl;
	}
	cout << "---" << endl;
}

int main(void)
{
	while(true)
	{
		cin >> H;
		if(H == 0) break;
		
		input();

		int score = 0;
		// print();
		while(true)
		{
			int add_score = erase();
			if(add_score == 0) break;  // 1つも消せなかったら終了
			score += add_score;

			drop();
			// print();
		}
		cout << score << endl;
	}

	return 0;
}
