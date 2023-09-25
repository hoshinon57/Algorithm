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
 * 操作2,3について、文字全てをいちいち変換していると間に合わない。
 * そのため、操作はその時刻も保持しておき、操作2,3の処理は最後の回答出力時にのみ行う。
 * 
 * chr[i]：{xi文字目に最後に操作1にて設定した時刻, その文字}
 * ev_time, ev_kind：操作2,3について、{最後に設定した時刻, その種類}
 * と定義する。
 * chr[i].firstとev_timeを比較し、前者の方が後であればchr[i].secondをそのまま出力する。
 * そうでない場合、小文字化 or 大文字化して出力する。
 * 
 * 計算量はO(N+Q).
 */

int main(void)
{
	int i;
	int N; cin >> N;
	string S; cin >> S;
	int Q; cin >> Q;

	vector<pair<int,char>> chr(N);  // chr[i]={xi文字目に最後に設定した時刻, 文字}
	for(i = 0; i < N; i++)
	{
		chr[i] = {-1, S[i]};
	}
	int ev_time = -2, ev_kind = 2;  // t=2,3について、{最後に設定した時刻, その種類}

	for(int tm = 0; tm < Q; tm++)  // 時刻tm
	{
		int t, x;
		char c;
		cin >> t >> x >> c;
		if(t == 1)
		{
			x--;
			chr[x] = {tm, c};
		}
		else if(t == 2)
		{
			ev_time = tm;
			ev_kind = 2;
		}
		else
		{
			ev_time = tm;
			ev_kind = 3;
		}
	}

	for(i = 0; i < N; i++)
	{
		char ch;
		if(chr[i].first > ev_time)  // 操作2,3よりも後に操作1があれば、操作1での文字をそのまま出力
		{
			ch = chr[i].second;
		}
		else  // そうでなければ、小文字化 or 大文字化して出力
		{
			if(ev_kind == 2)
			{
				ch = tolower(chr[i].second);
			}
			else
			{
				ch = toupper(chr[i].second);
			}
		}
		cout << (char)ch;
	}
	cout << endl;

	return 0;
}
