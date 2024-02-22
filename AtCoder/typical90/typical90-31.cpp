#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:31 https://atcoder.jp/contests/typical90/tasks/typical90_ae

/*
 * 自力で解けず、解説を見た。
 * Grundy数を用いて解く。
 *   https://yang33-kassa.hatenablog.com/entry/2017/12/21/202812
 *   https://augusuto04.hatenablog.com/entry/2015/05/03/233606  その2,3もある
 * 
 * まず、白石と青石を指定したときにGrundy数を返す grundy(w,b) を用意する。
 * 自分は再帰関数で実装した。
 * {w,b}={0,0},{0,1}は0.
 * それ以外のとき、白石および青石を取る場合の遷移先のGrundy数をそれぞれ求め、その集合のmexが求める値。
 * 
 * 後は各山ごとのGrundy数のxorを取ったものが、0以外なら先手勝ち、0なら後手勝ち。
 * ※各山ごとのxorで判定できる理由は理解していない。
 *   https://kyopro-friends.hatenablog.com/entry/2023/04/12/195012
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・Grundy数を学ぶ。
 */

vector<vector<int>> gr;
const int WMAX = 50;
const int BMAX = 1325;  // W=50,B=50のとき、(1+2+...+50)+50

int grundy(int w, int b)
{
	int &g = gr[w][b];
	if(g != -1) return g;
	if(w == 0 && b == 0) return 0;
	if(w == 0 && b == 1) return 0;

	vector<int> list(BMAX/2+10);  // 遷移先は (b/2)+1 個ぐらいなので、grundy数も最大でこれぐらいのはず
	int i;
	int n;
	if(w > 0)  // 白石を取る
	{
		n = grundy(w-1, b+w);
		list[n] = 1;
	}
	for(i = 1; i <= b/2; i++)  // 青石を取る
	{
		n = grundy(w, b-i);
		list[n] = 1;
	}
	// mexを求める
	for(i = 0; i < (int)list.size(); i++)
	{
		if(list[i] == 0)
		{
			g = i;
			break;
		}
	}

	return g;
}

int main(void)
{
	int i;
	int N; cin >> N;
	gr.assign(WMAX+1, vector<int>(BMAX+1, -1));
	vector<int> w(N); for(i = 0; i < N; i++) {cin >> w[i];}
	vector<int> b(N); for(i = 0; i < N; i++) {cin >> b[i];}

	ll cal = 0;
	for(i = 0; i < N; i++)
	{
		cal ^= grundy(w[i], b[i]);
	}
	if(cal != 0) cout << "First" << endl;
	else cout << "Second" << endl;

	return 0;
}
