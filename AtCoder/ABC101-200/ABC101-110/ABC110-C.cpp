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

// ABC110 https://atcoder.jp/contests/abc110

/*
 * うおーこれ苦戦した…
 * 
 * 最初にACした実装は、s[i]->t[i]をグラフっぽくみなす感じ。
 * https://atcoder.jp/contests/abc110/submissions/63981877
 *   S:cki
 *   T:rcr
 * c->r, k->c, i->rへの辺を張った有向グラフで考えて、
 * 入次数および出次数が2以上のものがあったらNG.無ければOK.
 * というのを試行錯誤したら見えた。(未証明)
 * 以下が近いかな。
 *   https://emtubasa.hateblo.jp/entry/2018/09/25/000000_1
 * 
 * 解説で多いのは1体1対応を作る、だった。(本質は似てるかもしれないが)
 *   https://pyteyon.hatenablog.com/entry/2019/02/23/200000
 *   https://blog.hamayanhamayan.com/entry/2018/09/23/234811
 *   https://drken1215.hatenablog.com/entry/2018/09/23/232100
 * ・(Sの)ある文字種について、(Tの)異なる文字種に対応したらダメ
 * ・(Tの)ある文字種について、(Sの)異なる文字種に対応したらダメ
 * というルール。
**/

bool solve(void)
{
	int i;
	string ss, st; cin >> ss >> st;
	int len = (int)ss.size();
	vector<int> s(len), t(len);
	for(i = 0; i < len; i++)
	{
		s[i] = ss[i]-'a';
		t[i] = st[i]-'a';
	}
	vector<int> cnv(26, -1), rev(26, -1);  // cnv[2]=3: Sの文字cがTの文字dに対応  rev[0]=1 Tの文字aがSの文字bに対応
	for(i = 0; i < len; i++)
	{
		if(cnv[s[i]] == -1 && rev[t[i]] == -1)  // 対応表にまだ出てきていない
		{
			cnv[s[i]] = t[i];
			rev[t[i]] = s[i];
		}
		else
		{
			if(cnv[s[i]] != t[i]) return false;  // Sのある文字が、Tの複数の文字に対応してしまっている
			if(rev[t[i]] != s[i]) return false;
		}
	}
	return true;
}

int main(void)
{
	YesNo(solve());
	return 0;
}
