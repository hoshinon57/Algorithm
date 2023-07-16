#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC240 https://atcoder.jp/contests/abc240

/*
 * 仕組み上、ボールを落としたときにいわゆる「連鎖」は発生しない。
 * ボールが消えるのは、今落としたボールに関してのみ。
 * 
 * {ボールの値, それが連続した個数} の形で配列を持つ。
 * ボールを落としたときに、1つ前のボールの値と比較して、
 * ・異なる値の場合
 *   新たに配列の要素を追加する。
 * ・同じ値の場合
 *   配列は追加せず、末尾の要素の"連続した個数"を+1する。
 *   ボールが消える条件になれば、要素を削除する。
 * を行っていけばよい。
 * これにより、ボールを落とすごとの計算量がO(1)になる。
 */
int main(void)
{
	int N;
	cin >> N;
	vector<pair<int,int>> ball;  // ball[i]={ボールの値, それが連続した個数}
	ball.push_back({-1, 0});  // 番兵
	int ball_num = 0;
	for(int i = 0; i < N; i++)
	{
		int a;
		cin >> a;
		int idx = ball.size()-1;  // 末尾の要素番号
		ball_num++;
		if(a != ball[idx].first)  // 1つ下のボールと異なる値
		{
			ball.push_back({a, 1});  // 新たに積む
		}
		else  // 1つ下のボールと同じ値
		{
			ball[idx].second++;  // 新たに積まず、末尾の要素の"連続した個数"を+1する
			if(ball[idx].second == ball[idx].first)  // kが書かれたボールがk個連続したら
			{
				// ボールの数を減らして、配列の最後尾を削除
				ball_num -= ball[idx].second;
				ball.pop_back();
			}
		}
		cout << ball_num << endl;
	}

	return 0;
}
