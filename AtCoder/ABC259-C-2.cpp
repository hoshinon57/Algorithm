#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC259 https://atcoder.jp/contests/abc259

/*
 * 解説を参考に、ランレングス圧縮で解く。
 * 
 * "aabccca"という文字列を、(a,2), (b,1), (c,3), (a,1)のように (文字,連続する個数) の形で保持する。
 * S,Tそれぞれ上記のように保持したとき、
 * (1)文字の種類の数が一致していること (上記例ではa,b,c,aの4)
 * Sから(char, num)を、Tから(char', num')を順に抽出したときに、
 * (2)char=char'であること
 * (3)num=num' または (num<num'かつnum>=2) であること
 * 上記(1)～(3)が全て成立すれば、SをTに一致させることが可能となる。
 */

// ランレングス圧縮
void RLE(string &str, vector<pair<char, int>> &rle)
{
	char now = str[0];
	int count = 1;
	for(int i = 1; i < str.size(); i++)
	{
		if(str[i] == now)
		{
			count++;
		}
		else
		{
			rle.push_back(make_pair(now, count));  // 1文字前までの分を積む
			now = str[i];
			count = 1;
		}
	}
	// 最後の分を積む
	rle.push_back(make_pair(now, count));
}

bool solve(vector<pair<char, int>> &s_rle, vector<pair<char, int>> &t_rle)
{
	if(s_rle.size() != t_rle.size())  // そもそもサイズが異なっていたらNG
	{
		return false;
	}

	for(int i = 0; i < s_rle.size(); i++)
	{
		if(s_rle[i].first != t_rle[i].first) return false;  // 文字が異なっていたらNG

		int s_count, t_count;
		s_count = s_rle[i].second;
		t_count = t_rle[i].second;
		if(s_count == t_count)
		{
			continue;  // ここまで一致にできる、次へ
		}
		else if(s_count < t_count)
		{
			if(s_count == 1) return false;  // 1文字なら挿入できないためNG
			else continue;  // Sに文字を挿入して一致できる、次へ
		}
		else  // s_count > t_count
		{
			return false;  // Tに文字は挿入できないためNG
		}
	}

	return true;
}

int main(void)
{
	int i, j;
	string s, t;
	cin >> s >> t;
	vector<pair<char, int>> s_rle, t_rle;
	RLE(s, s_rle);
	RLE(t, t_rle);

	if(solve(s_rle, t_rle))
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
