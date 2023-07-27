#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC268 https://atcoder.jp/contests/abc268

/*
 * 順列全探索と深さ優先探索(再帰)を用いて解く。
 * 
 * まず、文字列S1,S2,...,SNの並び替えは、next_permutation()にて行う。
 * 
 * 次に、アンダーバーの付与は再帰を用いた深さ優先探索にてパターンを列挙する。
 * 再帰関数には、
 *   各文字列の間に付与するアンダーバーの数(und_sum[])
 *   本関数の呼び出し直前に、どの文字列の直後にアンダーバーを付与したかの情報(und_idx)
 * を引数として渡す。
 * 
 * 再帰関数内にて、
 *   先ほど追加した位置に、さらにアンダーバーを付与する再帰(und_idx)
 *   先ほど追加した位置の次に、アンダーバーを付与する再帰(und_idx+1)
 * それぞれの再帰を行う。
 * 最後の文字列間にアンダーバーを付与したら文字列を作ってみて、
 * それがT1,T2,...,TMに無ければ正解となる。
 * 
 * N=1の場合はアンダーバーの付与が無いため、いくつか例外処理が必要になる。
 * また、「文字列は3文字以上」という制約に注意。
 */

int N, M;
int s_total_length;  // 文字列S1,S2,...,SNの長さの和
vector<string> s;
vector<int> s_idx;  // Sを並び替えたものを番号順に並べる
set<string> NG_set;  // T1,T2,...,TMの集合 これらがNGリストとなる
string answer;

// 再帰を用いた深さ優先探索にて、条件を満たす文字列を探す。
// und_num:
//   und_num[i]は、S(i)とS(i+1)の間に付与するアンダーバーの数を表す。
// und_idx:
//   本関数の呼び出し直前に、S(und_idx)の直後にアンダーバーを追加した、ということを指す。
//   次にアンダーバーを付与するのは、S(und_idx)の直後か、S(und_idx+1)の直後になる。
// 戻り値：条件を満たす文字列を見つけたらtrue, 見つからなかったらfalse.
bool dfs(vector<int> &und_num, int und_idx)
{
	int i, j;

	// 文字列Sとアンダーバーの長さの和を計算する
	// ここまでで16文字を越えたら終了
	int count = s_total_length;
	for(auto &e : und_num)
	{
		count += e;
	}
	if(count > 16) return false;
	
	// 最後の文字列間(S'(N-1)とS'(N)の間)にアンダーバーを付与したら、文字列を作ってみる
	// または、N=1の場合は文字列を作る
	if(und_idx == N-2 || N == 1)
	{
		string X;
		for(i = 0; i < N; i++)
		{
			X += s[s_idx[i]];  // S'(i)
			for(j = 0; j < und_num[i]; j++)
			{
				X += '_';
			}
		}

		// 文字列は3文字以上という制約
		if(X.size() < 3) return false;

		// 作成した文字列がNG文字列の中にあるか
		if(NG_set.find(X) == NG_set.end())
		{
			answer = X;
			return true;
		}
	}

	// N=1は少し例外で、アンダーバー付与が無い
	// 前述の処理で条件を満たす文字列が見つからなかったなら、ここで終了
	if(N == 1) return false;

	// 今見ている文字列の間に、アンダーバーを追加する
	und_num[und_idx]++;
	if(dfs(und_num, und_idx)) return true;  // trueで返ってきたらそのままtrueを返す
	und_num[und_idx]--;

	// 次の文字列の間に、アンダーバーを追加する
	if(und_idx != N-1)  // 最後の文字列の次にはアンダーバーを付与できない
	{
		und_num[und_idx+1]++;
		if(dfs(und_num, und_idx+1)) return true;
		und_num[und_idx+1]--;
	}

	return false;
}

int main(void)
{
	// 0-indexed
	int i;
	cin >> N >> M;
	s.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> s[i];
		s_total_length += s[i].length();  // 文字列S1,S2,...,SN の長さの和を計算しておく
	}
	for(i = 0; i < M; i++)
	{
		string tmp;
		cin >> tmp;
		NG_set.insert(tmp);
	}
	// ここまで入力
	
	s_idx.resize(N);
	for(i = 0; i < N; i++)  // 初期値設定 昇順に並べる
	{
		s_idx[i] = i;
	}

	answer = "";
	// S1, S2, ..., SNの並べ替えをnext_permutation()にて全探索
	do
	{
		// s_idx[]には並べ替えた順番が入る
		// 並べ替えた文字列に対して、アンダーバーの付与を深さ優先探索を用いて列挙する

		vector<int> und_num(N);  // アンダーバーの数
		// 再帰の処理の都合上、S'1とS'2の間には1つアンダーバーを入れておく
		// ただしN=1のときはアンダーバー付与の処理が無いので、除外する
		if(N != 1)
		{
			und_num[0] = 1;
		}

		if(dfs(und_num, 0)) break;
	} while (next_permutation(s_idx.begin(), s_idx.end()));

	if(answer != "")
	{
		cout << answer << endl;
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
