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

// ABC161 https://atcoder.jp/contests/abc161

/*
 * 貪欲法で解く。
 * ※公式解説と同じ考え方。
 *   https://img.atcoder.jp/abc161/editorial.pdf
 * 
 * まず、「なるべく早い日程で働く」ようにした場合に働ける日を計算する。
 * これは先頭から貪欲に求めればよい(前から貪欲)。
 * ここで得られたL[i]は、「i回目に働く日はL[i]日目以降」と言うことができる。
 * ※得られた働ける日がK日より多い場合、ここからK日を選ぶことで「必ず働く日」は設定不要となる。
 *   この場合は出力無しで終了させる。
 * 
 * 次は後ろから貪欲で計算することで、「なるべく遅い日程で働く」を求めることができる。
 * ここで得られたR[i]は「i回目に働く日はR[i]日目以前」と言える。
 * 
 * 例えばL[2]=4, R[2]=7の場合、2回目に働く日は4日目or7日目が候補となる。(5,6日目も働けるかもしれない)
 * L[2]=4, R[2]=4の場合、2回目に働く日は4日目のみとなる。
 * つまりL[i]=R[i]となるような要素が答になる。
 */

int main(void)
{
	// 0-indexed
	int i;
	int N, K, C; cin >> N >> K >> C;
	string s; cin >> s;

	// 前から貪欲
	vector<int> L;
	for(i = 0; i < N; i++)
	{
		if(s[i] == 'o')
		{
			L.push_back(i+1);
			i += C;
		}
	}

	// 候補がK個より多いのであれば、出力無しで終了
	if((int)L.size() > K)
	{
		return 0;	
	}

	// 後ろから貪欲
	vector<int> R;
	for(i = N-1; i >= 0; i--)
	{
		if(s[i] == 'o')
		{
			R.push_back(i+1);
			i -= C;
		}
	}
	reverse(R.begin(), R.end());

	vector<int> ans;
	for(i = 0; i < (int)L.size(); i++)
	{
		if(L[i] == R[i]) ans.push_back(L[i]);
	}
	for(auto &e : ans) cout << e << endl;

	return 0;
}
