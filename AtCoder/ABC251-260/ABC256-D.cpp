#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC256 https://atcoder.jp/contests/abc256

/*
 * コンテスト中に解けず、解説を見て実装した。
 * イベントソートで解く。
 * 参考：
 *   https://atcoder.jp/contests/abc256/editorial/4121
 *   https://qiita.com/pata_/items/e7cfdbbb4414cf13e715
 * 
 * [L,R)についてLとRをそれぞれ別のイベントとして登録する。
 * このとき、L,Rを区別するために{L,0} {R,1}としておく。
 * 
 * Lのイベント番号(=0)をRより小さくすることで、
 * L,Rをまとめて昇順ソートするとL=RのときにLが先に来る。
 * (Rを先にすると正しく解を出せない)
 * 
 * ソート後に小さい方から順に見ていき、
 *   Lなら区間内のものが増えるという考えで、カウントをインクリメント
 *   Rなら区間内のものが減る  という考えで、カウントをデクリメント
 * カウントが0->1のとき、Lが確定。
 * カウントが1->0のとき、Rが確定。
 * 最後にL,Rのペアを出力すればよい。
 */
int main(void)
{
	int N;
	cin >> N;
	vector<pair<int, int>> LR_event;
	for(int i = 0; i < N; i++)
	{
		int l, r;
		cin >> l >> r;
		// Lのイベント番号は0, Rは1
		LR_event.push_back(pair<int,int>(l, 0));
		LR_event.push_back(pair<int,int>(r, 1));
	}
	// L,Rまとめてソート
	// L:0, R:1のため、ソートするとLが優先される
	sort(LR_event.begin(), LR_event.end());  

	int count = 0;
	int nowL, nowR = 0;
	vector<pair<int, int>> answer;  // {L,R}
	for(auto &e : LR_event)
	{
		if(e.second == 0)  // L
		{
			// 0->1のとき、Lが確定する
			if(count == 0)
			{
				nowL = e.first;
			}
			count++;
		}
		else  // R
		{
			// 1->0のとき、Rが確定する
			count--;
			if(count == 0)
			{
				nowR = e.first;
				answer.push_back(pair<int,int>(nowL, nowR));
			}
		}
	}

	for(auto &e : answer)
	{
		cout << e.first << " " << e.second << endl;
	}

	return 0;
}
