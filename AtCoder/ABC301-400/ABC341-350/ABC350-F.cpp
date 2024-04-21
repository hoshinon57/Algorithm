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

// ABC350 https://atcoder.jp/contests/abc350

/*
 * コンテスト中に解けず、解説を見た。
 * ほぼ公式解説と同じ実装となったため、基本はそちらを参照。
 *   https://atcoder.jp/contests/abc350/editorial/9820
 * その他参考：
 *   https://twitter.com/kyopro_friends/status/1781681418638381141
 *   https://twitter.com/ryuusagi/status/1781683296168296515
 *   https://twitter.com/scrblbug/status/1781679316075794875
 * 
 * 以下の再帰関数dfs(l,r,dir)を用意する。
 *   文字列Sの[l,r]の範囲について、dirが0なら右方向、1なら左方向に走査する。
 *   右方向のとき、
 *     '('を見つけたらそのカッコの位置をll,rrとして dfs(ll+1,rr-1,1) で再帰し、走査位置はrr+1へ移動。
 *     文字の場合はそのまま出力。
 *   左方向のときも同様だが、大文字小文字を反転して出力する。
 * 大文字小文字の反転は、「左方向に見ているときに反転する」で良い。
 * 
 * main()からは dfs(0,sz-1,0) で呼び出す。
 * カッコの位置は事前に求めておく。実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・うーん…、これ構文解析だと気づくにはどうしたら良いんだろうか。
 * ・紙に書きながら規則性というか、走査順序を見つけ出すことになるのかなあ。
 * ・「文字列全体をシフトする場合、基準位置をずらすことでO(1)で対応」という典型があるが、
 *   文字列の反転は見る順番や方向を反転する、というのが典型になるのかな。
 */

string s;
vector<int> lrpos;  // pos[i]:i文字目が'(' or ')'のとき、それに対応するカッコの位置

// 大文字小文字を反転して返す
// cが英字以外の挙動は未確認のため注意
char chg(char c)
{
	if(islower(c)) return toupper(c);
	else return tolower(c);
}

// [l,r]
// dir=0:右方向, 1:左方向
void dfs(int l, int r, int dir)
{
	int idx = (dir==0) ? l : r;  // スタート位置はdirで決まる
	while(idx >= l && idx <= r)
	{
		auto c = s[idx];
		if(dir == 0)  // 右方向
		{
			if(c == '(')
			{
				int tmp = lrpos[idx];  // 左:idx, 右:tmp
				dfs(idx+1, tmp-1, 1);
				idx = tmp;
			}
			else
			{
				cout << c;
			}
			idx++;
		}
		else  // 左方向
		{
			if(c == ')')
			{
				int tmp = lrpos[idx];  // 左:tmp, 右:idx
				dfs(tmp+1, idx-1, 0);
				idx = tmp;
			}
			else
			{
				cout << chg(c);
			}
			idx--;
		}
	}
}

int main(void)
{
	int i;
	cin >> s;
	int sz = (int)s.size();
	lrpos.resize(sz, -1);
	vector<int> stk;
	for(i = 0; i < sz; i++)
	{
		if(s[i] == '(')
		{
			stk.push_back(i);
			lrpos.push_back(i);
		}
		else if(s[i] == ')')
		{
			int l = stk.back();  // lとi
			lrpos[i] = l;
			lrpos[l] = i;
			stk.pop_back();
		}
	}
	dfs(0, sz-1, 0);
	cout << endl;

	return 0;
}
