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

// ABC283 https://atcoder.jp/contests/abc283

/*
 * 以下3つのデータを用意する。
 *   今見ている文字のカッコの深さ -> depth
 *   その深さごとに出現した文字を保持する配列 -> char_list[depth]
 *   英字に対応したボールが使われているか -> used[]
 * 
 * 文字列ごとに以下の操作をする。
 *   英字：used[]を使用済みに更新。もし既に使われていたら操作を完了できない。
 *   '('：depthを1増やす。
 *   ')'：今のchar_list[depth]に含まれる文字をすべて削除し、depthを1減らす。
 */

bool solve(void)
{
	string s;
	cin >> s;
	const int AL_KIND = 26;  // アルファベットの種類

	vector<bool> used(AL_KIND, false);  // used[i]:i番目のアルファベットが既に使われていたらtrue
	int depth = 0;  // カッコの深さ
	vector<vector<int>> char_list(300000);  // char_list[i]:depth=iのときの、出てきた文字
	
	for(int i = 0; i < (int)s.size(); i++)
	{
		auto e = s[i];
		switch(e)
		{
		case ')':
			// char_list[depth]を全削除して、depth--;
			for(auto &c : char_list[depth])
			{
				used[c] = false;
			}
			char_list[depth].clear();
			depth--;
			break;
		case '(':
			depth++;
			break;
		default:
			int num = (int)(e-'a');
			if(used[num]) return false;  // 既に使われている

			used[num] = true;
			char_list[depth].push_back(num);
			break;
		}
	}

	return true;
}
int main(void)
{
	YesNo(solve());

	return 0;
}
