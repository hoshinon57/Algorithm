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
 * 各{L,R}をpairに保存し、Lの小さい順に見ていく。
 * 参考：
 *   https://note.com/syamashi/n/nab622777a941
 *   https://twitter.com/mplpplt/status/1538156867654144000
 * 
 * まず各{L,R}をpairのvectorに保存してソートすることで、Lの小さい順に並べる。
 * 次に確定させたい区間をnowL, nowRとし、Lの小さい順に{L,R}のペアを見ていく。
 * (1)次のL <= nowR
 *   このとき、次のL,R区間と連結できる。
 *   nowLはそのままで、nowRをmax(nowR, 次のR)で更新する。
 * (2)次のL > nowR
 *   次のL,R区間とは分離されるため、現在のnowL,nowRで区間が確定する。
 *   これをanswerに保持した上で、次のL,RをnowL,nowRに設定する。
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<pair<int,int>> LR(N);
	for(i = 0; i < N; i++)
	{
		int l, r;
		cin >> l >> r;
		LR[i] = make_pair(l, r);
	}
	sort(LR.begin(), LR.end());  // Lで昇順ソートする

	int nowL = 0, nowR = 0;
	vector<pair<int,int>> answer;
	nowL = LR[0].first;
	nowR = LR[0].second;
	for(auto &e : LR)  // 以降の処理より、LR[0]に値を入れたうえでLR全体でループしても問題ない
	{
		if(e.first <= nowR)  // 次のL <= 現在のR なら連結
		{
			nowR = max(nowR, e.second);  // 連結により、より右にある方のRを採用
		}
		else  // 次のL > 現在のR ならnowL,nowRで確定
		{
			answer.push_back({nowL, nowR});
			// 次のL,R
			nowL = e.first;
			nowR = e.second;
		}
	}
	answer.push_back({nowL, nowR});  // 最後の分

	for(auto &e : answer)
	{
		cout << e.first << " " << e.second << endl;
	}

	return 0;
}
