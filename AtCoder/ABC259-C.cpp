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
 * 先頭から1文字ずつ見ていく。貪欲法(と思われる)。
 * 
 * 「Sに文字を挿入」について、S="abbc"のときに
 *   ・bbの間にbを入れる(2,3文字目の間)
 *   ・bbの次にbを入れる(3,4文字目の間)
 * は同じ操作となる。
 * 本回答では後者を行うイメージ。
 * Sの直近2文字が同一である場合、その文字を挿入できるのでbufに入れておく。
 * 
 * S,Tの先頭から1文字ずつ進めていく。
 * ・Sの文字＝Tの文字のとき
 *   S,Tともに1文字進める。
 * ・Sの文字≠Tの文字のとき
 *   ・Tがbufと一致すれば、Sに文字挿入することで一致を保てる。
 *   ・Tがbufと不一致であれば、一致できないためNG.
 * 
 * S,Tどちらかが先に終端に来るケースもある。
 * Tが先に終端に来た場合（＝Sが終端に到達していない）はNG.
 * Sが先に終端に来た場合（＝Tが終端に到達していない）は、
 * Tの残りが全てbufと一致すればOK.それ以外はNG.
 */
int main(void)
{
	string s, t;
	cin >> s >> t;
	int s_index = 0, t_index = 0;
	char buf = 'Z';  // S,Tは小文字のため、適当な大文字にしておく

	while(s_index < s.size() && t_index < t.size())
	{
		if(s[s_index] == t[t_index])
		{
			s_index++;
			t_index++;
		}
		else
		{
			if(t[t_index] == buf)
			{
				// Sに文字を挿入することで一致を保てる
				t_index++;
			}
			else
			{
				cout << "No" << endl;
				return 0;
			}
		}
		if(s_index >= 2)
		{
			if(s[s_index-2] == s[s_index-1])  // Sの直近2文字が同じ
			{
				buf = s[s_index-1];
			}
			else
			{
				// コンテスト時はここの処理を忘れていた
				// よってS="aabab", T="aabaab"といったケースでYesとなってしまっていた(Noが正しい)
				buf = 'Z';  // 適当な大文字に
			}
		}
	}
	// S,Tのどちらかが終端に来たらループ終了

	// Sが終端まで到達していなければNG.
	if(s_index < s.size())
	{
		cout << "No" << endl;
		return 0;
	}

	// Tが終端まで到達していない場合、残りが全てbufと一致するか
	while(t_index < t.size())
	{
		if(t[t_index] != buf)
		{
			cout << "No" << endl;
			return 0;
		}
		t_index++;
	}

	cout << "Yes" << endl;
	return 0;
}
