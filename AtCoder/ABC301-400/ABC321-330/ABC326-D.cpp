#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC326 https://atcoder.jp/contests/abc326

/*
 * コンテスト中に解けず、解説を見た。
 * 公式解説1の手法で解く。
 *   https://atcoder.jp/contests/abc326/editorial/7539
 * 
 * 完成した状態を見ると、各行に"ABC"が1回ずつ登場する。
 * N=5のとき、ある行についてこれを満たす組み合わせは
 *   (5*4*3)/3=20通り
 * となる。Rの制約により最左の文字は固定されるため。
 * 20^5=3.2*10^7 より、5行それぞれ全探索しても間に合う。
 * この方針で解く。
 * 
 * DFSを用いて、上の行から順に "ABC.." の文字列を置いていく。
 * これにはnext_permutation()を使っていく。
 * N行作成したら判定処理。
 * 
 * 判定処理では各列について
 * ・A,B,Cが1個ずつであること
 * ・最も上のマスの文字がCと一致すること
 * を判定する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * コンテスト時は
 * ・まずR,Cの制約を満たすよう各行/各列に置いていき、
 * ・"ABCが1個ずつ"という制約を満たすよう、残りのマスを埋めていく
 * という方針で実装していた。
 * たぶんこれでもACは可能とは思うが、全然実装しきれなかった。
 * セオリーは
 * ・各行について"ABC"が1文字ずつ登場するので、まずは各行を埋めてしまう
 * だと思う。その後、各列およびR,Cの制約を満たすかチェックすればよい。
 * 今後は、まずこのセオリーを思い出して実装方針を考える。
 */

int N;
string r, c;
vector<string> mp;
string abc_s;

// 空きマスを除いて最初の文字を返す(A or B or C)
char top(string s)
{
	int i = 0;
	while(s[i] == '.') { i++; }
	return s[i];
}

void print(vector<string> &s)
{
	for(int i = 0; i < N; i++)
	{
		cout << s[i] << endl;
	}
}

// N行作った文字列について、条件を満たすかチェック
bool check(vector<string> &s)
{
	int i, j;

	// 列jについて見ていき、「A,B,Cが1個ずつであること」「最も上のマスの文字がCと一致すること」を判定する
	for(j = 0; j < N; j++)  // j列目
	{
		string t;
		map<char,int> cnt;
		for(i = 0; i < N; i++)  // i行目
		{
			t += s[i][j];
			if(s[i][j] != '.') { cnt[s[i][j]]++; }
		}
		// 「A,B,Cが1個ずつであること」「最も上のマスの文字がCと一致すること」
		if(top(t) != c[j]) return false;
		if(cnt['A'] == 1 && cnt['B'] == 1 && cnt['C'] == 1) {;}
		else return false;
	}
	return true;
}

// y行目を作成して再帰
bool dfs(int y)
{
	if(y == N)  // N行作成したのでチェック
	{
		return check(mp);
	}

	string s = abc_s;
	do
	{
		if(top(s) != r[y]) continue;  // 最左の文字がRと不一致であればNG
		mp[y] = s;
		if(dfs(y+1)) return true;  // trueで返ってきたら、そのまま返していく
	} while(next_permutation(s.begin(), s.end()));

	return false;
}

int main(void)
{
	int i;
	cin >> N >> r >> c;
	mp.resize(N);

	// "ABC.."といった文字列を作成する
	abc_s = "ABC";
	for(i = 3; i < N; i++) {abc_s += '.';}
	sort(abc_s.begin(), abc_s.end());  // next_permutation()用

	if(dfs(0))
	{
		cout << "Yes" << endl;
		print(mp);
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
