#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC216 https://atcoder.jp/contests/abc216

/*
 * [考え方]
 * ある状態にて取り除けるボールが複数種類あるとき、どの手順で取り除いても結果は同じ。
 * ⇒シミュレーションで解ける。
 * 
 * queue tube[i]:筒iに入っているボールを、取り出せる順からqueueにて
 * ball[i][*]:色iのボールが入っている筒の番号 制約より[0][1]の2つ
 * queue ev:次にこの筒についてボールを取り除けるかチェックする
 * のように定義する。
 * 最初、evには全ての筒番号を登録しておく。
 * 
 * ・evから1つ取り出し、一番上のボールの色を取得する。
 * ・その色について、もう片方のボールがある筒をball[]を使って探す。
 * ・どちらも一番上にあるならば、tube[]からボールを取り除く
 * ・evが空になった後、全てのtube[]が空ならOK.
 * の手順で解ける。
 * 
 * [ACまでの思考の流れ]
 * ・シミュレーションっぽい。
 * ・愚直にやるとTLEのはずなので、どうやって最適化しようか。
 * ・次にチェックする筒は、ボールを取り出した筒2つで良さそう。
 *   それ以外はボールが減ってないので、わざわざチェックしなおす必要は無い。
 * ・「各筒のボール一覧」と「各色のボールが、どの筒に入っているか」のデータを持てば、解けるかな。
 */

bool solve(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<queue<int>> tube(M);  // tube[i]:筒iに入っているボール
	vector<vector<int>> ball(N);  // ball[i][*] 色iのボールが入っている筒の番号 [0][1]
	for(i = 0; i < M; i++)
	{
		int k; cin >> k;
		for(j = 0; j < k; j++)
		{
			int a; cin >> a;
			a--;
			tube[i].push(a);
			ball[a].push_back(i);
		}
	}
	queue<int> ev;  // 次にこの筒をチェックする
	for(i = 0; i < M; i++) ev.push(i);

	while(!ev.empty())
	{
		int t1 = ev.front();  // 筒t1についてチェック
		ev.pop();
		if(tube[t1].empty()) continue;

		int col = tube[t1].front();
		int t2;  // 同じ色のボールがある、もう片方の筒
		if(t1 == ball[col][0]) t2 = ball[col][1];
		else t2 = ball[col][0];

		if(t1 != t2 && tube[t2].front() == col)  // ある色のボールが別の筒に入っていて、どちらも一番上に置かれている場合
		{
			// ボールを取り出してチェック用キューに追加
			tube[t1].pop();
			tube[t2].pop();
			ev.push(t1);
			ev.push(t2);
		}
	}
	
	for(i = 0; i < M; i++)
	{
		if(!tube[i].empty()) return false;  // 空ではない筒が残っていればNG
	}
	return true;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
