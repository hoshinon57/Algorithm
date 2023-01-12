#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題93
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/s8pc-3/tasks/s8pc_3_b

/*
 * 以下2つの処理を行う関数を用意する。
 * (1)石を消す
 * (2)石の落下
 * これを「これ以上消せない」まで繰り返す。
 * 
 * 最初に1つのマスを選んで消す処理について、全てのマスについて全探索する。
 * 各探索について上記(1)(2)を繰り返していく。
 * 
 * 石の配置は c_org[y][x] にて持ち、各探索にて別変数にコピーして処理する。
 * 「K個以上並んでいる」判定を簡易にするため、横方向の末尾に番兵を用意した。
 * 
 * ＞点数は1,000,000,000点を超えない
 * の制約より、これは約2^30であることから消滅の回数（連鎖）は最大30回程度と分かる。
 * 
 * 計算量は、上記の消滅の回数の最大をMとして、O(H^2*W^2*M)となる。
 * ⇒最初に1つのマスを選ぶ方法がO(HW).
 *   石の消滅や落下処理がO(HW).
 */

int H, W, K;
vector<vector<int>> c_org;  // c_org[y][x]

void input(void)
{
	cin >> H >> W >> K;
	c_org.resize(H, vector<int>(W+1));  // +1は横方向の番兵用
	for(int y = 0; y < H; y++)
	{
		for(int x = 0; x < W; x++)
		{
			char c;
			cin >> c;
			c_org[y][x] = (int)(c-'0');
		}
		c_org[y][W] = -1;  // 番兵
	}
}

void drop(vector<vector<int>> &c)
{
	for(int x = 0; x < W; x++)
	{
		for(int y = H-1; y >= 0; y--)
		{
			if(c[y][x] == 0)  // 消えてる
			{
				// 自分より上のセルから、次の石を探してswap
				for(int k = y-1; k >= 0; k--)
				{
					if(c[k][x] != 0)
					{
						swap(c[y][x], c[k][x]);
						break;
					}
				}
			}
		}
	}
}

// セルを消し、得られるスコアを返す
// ここでのスコアは「消えた数字の和」とする (つまり2^iは乗算しない)
ll erase(vector<vector<int>> &c)
{
	int score = 0;
	for(int y = 0; y < H; y++)
	{
		int prev = c[y][0];
		int count = 1;  // 同じ値のセルが連続する数
		for(int x = 1; x < W+1; x++)  // 番兵用に+1
		{
			if(c[y][x] == prev)  // 同じ値が連続している
			{
				count++;
			}
			else  // 異なる値
			{
				if(count >= K)  // K個以上並んでいたら、セルを消してスコア加算
				{
					// (x-count)から(x-1)までのcount個
					for(int k = x-count; k <= x-1; k++)
					{
						c[y][k] = 0;
					}
					score += prev*count;
				}

				// カウントしなおし
				prev = c[y][x];
				count = 1;
			}
		}
	}

	return score;
}

// デバッグ用
void print(vector<vector<int>> &c)
{
	for(int y = 0; y < H; y++)
	{
		for(int x = 0; x < W; x++)
		{
			cout << c[y][x] << " ";
		}
		cout << endl;
	}
	cout << "---" << endl;
}

int main(void)
{
	// 0-indexed
	input();

	ll max_score = 0;
	for(int del_y = 0; del_y < H; del_y++)  // 最初に手動で消すマスY
	{
		for(int del_x = 0; del_x < W; del_x++)  // 最初に手動で消すマスX
		{
			// (del_y,del_x)を最初に消す
			vector<vector<int>> c;
			c = c_org;
			c[del_y][del_x] = 0;
//			print(c);

			ll score = 0;
			ll phase = 1;  // 何回目の消滅か
			while(true)
			{
				drop(c);
//				print(c);

				int add_score = erase(c) * phase;
				if(add_score == 0) break;  // 消せるセルが無くなったら終了
				score += add_score;
				phase *= 2;  // 回が進むごとに加算されるスコアは2倍になっていく
			}
			max_score = max(max_score, score);
		}
	}
	cout << max_score << endl;

	return 0;
}
