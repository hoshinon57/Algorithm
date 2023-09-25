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

// ABC314 https://atcoder.jp/contests/abc314

/*
 * 色ごとに「色iで塗られた文字列」と「次に出力する文字番号」を管理する。
 * (以下の実装ではc_strとc_idx)
 * 
 * c_strについては2周りした文字列にしておき、
 * c_idxは (その文字列長/2 - 1) を設定しておく。
 * 後は先頭から1文字ずつ出力していけばよい。
 * ※2周させることで実装が楽になるテク
 */

int main(void)
{
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	string s; cin >> s;
	vector<int> c(N);
	vector<string> c_str(M);  // c_str[i]:色iの文字列
	for(i = 0; i < N; i++)
	{
		cin >> c[i];
		c[i]--;
		c_str[c[i]] += s[i];
	}
	vector<int> c_idx(M);  // c_idx[i]:色iについて次に出力するindex
	for(i = 0; i < M; i++)
	{
		c_str[i] += c_str[i];
		c_idx[i] = (int)c_str[i].size()/2 - 1;
	}

	for(i = 0; i < N; i++)
	{
		cout << (char)c_str[c[i]][c_idx[c[i]]];
		c_idx[c[i]]++;
	}
	cout << endl;

	return 0;
}
