#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC386 https://atcoder.jp/contests/abc386

/*
 * 平面走査的な考え方…のはず。
 * ※以下、入力のX,Yを反転させている。(Y行目、X列)
 * 
 * 入力に対し塗り分け結果(の一例)がどうなるかを考えると、
 * (y,x)に白の入力があったとき、そこから右下方向は全て白である必要がある。
 * 
 * それまで見た白のY座標の最小値をyminとする。
 * よって入力をXの昇順でソートしておいて、入力を1つずつ見ていったときに、
 *   白：yminをchmin()で更新
 *   黒：y>=yminならNo.
 * となる。
 * 入力の最後までNoにならなければYes.
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・入力に対してマスを塗るといった操作を行うとき、塗り方にどのような規則性があるかを考える。
 */

bool solve(void)
{
	// 問題文のX,Yを反転させている
	int i;
	int N, M; cin >> N >> M;
	vector<vector<int>> d(M, vector<int>(3, 0));  // {X,Y,C}  C:0=Black,1=White  ※入力のX､Y反転と、ソート用にXを先に置いているのでややこしい
	for(i = 0; i < M; i++)
	{
		cin >> d[i][1] >> d[i][0];
		string s; cin >> s;
		if(s == "W") d[i][2] = 1;
	}
	sort(d.begin(), d.end());

	int ymin = INF32;
	for(auto &e : d)
	{
		int y = e[1], c = e[2];
		if(c == 0)  // black
		{
			if(y >= ymin) return false;
		}
		else  // white
		{
			chmin(ymin, y);
		}
	}
	return true;
}

int main(void)
{
	YesNo(solve());
	return 0;
}
