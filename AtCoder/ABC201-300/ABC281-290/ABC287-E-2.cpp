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

// ABC287 https://atcoder.jp/contests/abc287

/*
 * 解説を元に、Trie木にて解いた。
 *   https://twitter.com/kyopro_friends/status/1619332267779964929
 *   https://atcoder.jp/contests/abc287/editorial/5609
 * 
 * まだまだ理解は浅いが…、
 * 木の根から文字列Siを1文字ずつ追加していくイメージ。
 * 各辺は'a'～'z'の文字を、各頂点には「そこまでの文字列を使うSiの一覧」を保持しておく…感じか。
 */

int N;
vector<string> s;
vector<int> ans;

// prefix：次に見る文字列の要素番号(0-indexed)
// group：見ようとしている文字列Siの一覧 S2,S5を見るなら{2,5}
void dfs(int prefix, vector<int> group)
{
	vector<vector<int>> group_new(26);  // group_new[26][*]  group_new['a']ならば、次の文字が'a'となる文字列Siの一覧
	for(auto &e : group)
	{
		if(prefix < (int)s[e].size())  // 文字列Seの終端には行っていない
		{
			group_new[(int)(s[e][prefix] - 'a')].push_back(e);
		}
		ans[e] = prefix;  // 再帰につれ最大値に更新されていく
	}
	for(auto &e : group_new)
	{
		if((int)e.size() >= 2)  // 被る文字列が存在する場合のみ
		{
			dfs(prefix+1, e);
		}
	}
}

int main(void)
{
	int i;
	cin >> N;
	s.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> s[i];
	}

	vector<int> group(N);
	for(i = 0; i < N; i++) group[i] = i;  // 最初はS1,S2,...,Snまで全て見る
	ans.resize(N);
	dfs(0, group);

	for(auto &e : ans) cout << e << endl;

	return 0;
}
