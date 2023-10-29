#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC326 https://atcoder.jp/contests/abc326

/*
 * 公式解説2の手法で解く。
 *   https://atcoder.jp/contests/abc326/editorial/7540
 * 
 * 文字Aを各行各列に1個ずつあるような置き方は、N=5のとき5!=120通りとなる。
 * A～Cまでそれぞれ行っても120^3=1.7*10^6程度であり、それぞれ全探索しても十分に間に合う。
 * 
 * DFSを用いて、ある文字を各行各列でかぶらないよう置く処理を実装する。
 * 置き方はnext_permutation()を使った。
 * A～Cまで置いたら判定処理。
 * 
 * 判定処理では、R,Cに関する制約をチェックするだけで良い。
 * 各行各列で各文字が1回ずつ登場する、というのはここまでで満たしているため。
 */

int N;
string r, c;
vector<string> mp;

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
	// R,Cに関する制約を満たしているかをチェックしていく
	int i, j;
	for(i = 0; i < N; i++)
	{
		if(top(s[i]) != r[i]) return false;
	}

	for(j = 0; j < N; j++)  // 列j
	{
		string t;
		for(i = 0; i < N; i++)
		{
			t += s[i][j];
		}
		if(top(t) != c[j]) return false;
	}
	
	return true;
}

// 文字aを各行に置き、再帰
bool dfs(char a)
{
	int i;
	if(a == 'D')  // A～Cまで置いた
	{
		return check(mp);
	}

	vector<int> idx(N);
	iota(idx.begin(), idx.end(), 0);
	auto mp_bk = mp;
	do
	{
		mp = mp_bk;

		bool ok = true;
		for(i = 0; i < N; i++)  // i行目 idx列 に文字を置く
		{
			if(mp[i][idx[i]] != '.') { ok = false; break; }  // すでに置かれている場合はNG
			mp[i][idx[i]] = a;
		}
		if(!ok) continue;

		if(dfs((char)(a+1))) return true;  // trueで返ってきたら、そのまま返していく
	} while (next_permutation(idx.begin(), idx.end()));

	return false;
}

int main(void)
{
	int i;
	cin >> N >> r >> c;

	// 盤面を'.'で埋める
	string d;
	for(i = 0; i < N; i++) { d += '.'; }
	mp.resize(N, d);

	if(dfs('A'))
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
