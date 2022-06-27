#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * イベントソートの簡易まとめ
 * 参考：
 *   https://atcoder.jp/contests/abc256/editorial/4121   (ABC256-D)
 *   https://qiita.com/pata_/items/e7cfdbbb4414cf13e715
 * 
 * [関連する問題]
 * ABC256-D https://atcoder.jp/contests/abc256/tasks/abc256_d
 * ABC257-C https://atcoder.jp/contests/abc257/tasks/abc257_c
 * 
 * [問題設定]
 * N人のユーザがいて、i番目のユーザは時刻Lにログインし、時刻Rにログアウトしました。
 * 1人以上がログインしていた時刻の区間を求めなさい。
 * (同時刻にユーザAがログアウト、ユーザBがログインした場合、「1人以上ログイン」は継続されているとする)
 * 
 * [考え方]
 * L,Rをそれぞれ別のイベントとして登録する。
 * {L/R, event}として、{L,0}, {R,1}のように設定する。(※)
 * そして全イベントをまとめて昇順ソートする。
 * 
 * ソート後に小さい方から順に見ていき、
 *   Lならログインユーザが増えるという考えで、カウンタをインクリメント
 *   Rならログインユーザが減る  という考えで、カウンタをデクリメント
 * とする。
 * カウンタが0->1以上 になったとき、区間の始まりになる。
 * カウンタが1以上->0 になったとき、区間の終了になる。
 * 
 * (※)0,1をLRで逆にすると正しく解を出せない。(昇順ソート時)
 *     同じタイミングでL,Rが来たときに、Rが先に処理されるとそこで途切れてしまうため。
 * 
 * [補足]
 * 区間設定を[L,R)としたときに。
 * 問題設定が、「同時にログインしていた人数の最大値を求めなさい」の場合、
 * 上記では正しく処理できない。
 * 同時刻に別ユーザがログアウト＆ログインした場合、
 * ログインが先に処理されるため想定より1人多くなってしまう。
 * {L,1},{R,0}のようにRを優先して処理すれば、正しく求められる…はず。
 * (Lが来たときのカウンタの最大値を保持しておけばよい)
 * 
 * 最大値を求めるなら、いもす法の方が良いかも。
 */
int main(void)
{
	// ABC256-D問題の形式で入力があると仮定する
	// https://atcoder.jp/contests/abc256/tasks/abc256_d
	// 自分が提出した ABC256-D.cpp も参照。処理は全く同じソース。
	int N;
	cin >> N;
	vector<pair<int,int>> LR_event;
	for(int i = 0; i < N; i++)
	{
		int l, r;
		cin >> l >> r;
		// Lのイベント番号は0, Rは1
		LR_event.push_back(pair<int,int>(l,0));
		LR_event.push_back(pair<int,int>(r,1));
	}
	// L,Rまとめてソート
	// L:0, R:1のため、L/Rが同じ値のときはLが優先される
	sort(LR_event.begin(), LR_event.end());

	int count = 0;
	int nowL, nowR = 0;
	vector<pair<int, int>> answer;  // {L,R}
	for(auto &e : LR_event)
	{
		if(e.second == 0)  // L
		{
			if(count == 0)
			{
				// 0->1以上 のとき、区間の始まり
				nowL = e.first;
			}
			count++;
		}
		else  // R
		{
			// 1以上->0 のとき、区間の終わり
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
