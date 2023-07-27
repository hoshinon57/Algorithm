#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC250 https://atcoder.jp/contests/abc250

/*
 * hamamuさんの解説を元に実装した。
 *   https://atcoder.jp/contests/abc250/editorial/3948
 *   https://blog.bouzuya.net/2022/05/09/
 *   https://qiita.com/u2dayo/items/5c021016d11f125e3c0b
 * 
 * 入力A,Bの値を、登場順に小さい数値から割り当てる。(実装ではmapを用いた)
 * するとAについて、種類数がkであれば{1,2,3, ..., k}の集合となり、種類数と最大値が一致する。
 * (最小値は必ず1になる)
 * これを先頭i個についてN通り求めておく。
 * 
 * Bについても似たような感じで
 *   先頭i個に含まれる要素の種類数
 *   先頭i個に含まれる要素の最大値
 * をN通り求めておく。
 * 
 * するとAの先頭i項とBの先頭j項の集合が一致するとは、
 *   A側の種類数とB側の種類数が同じ かつ
 *   A側の種類数とB側の最大値が同じ
 * で判定できる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	map<int,int> mp;

	// それぞれ、
	// Aの先頭i個に含まれる要素の種類数
	// Bの先頭i個に含まれる要素の種類数
	// Bの先頭i個に含まれる要素の最大値
	vector<int> a_kind(N), b_kind(N), b_max(N);

	int cnt = 0;
	for(i = 0; i < N; i++)
	{
		int tmp; cin >> tmp;
		if(mp.count(tmp) == 0)
		{
			cnt++;
			mp[tmp] = cnt;
		}
		a_kind[i] = cnt;  // 登場順に番号を振りなおす
	}
	set<int> b_set;
	int b_m = -1;
	for(i = 0; i < N; i++)
	{
		int tmp; cin >> tmp;
		if(mp.count(tmp) == 0)
		{
			cnt++;
			mp[tmp] = cnt;
		}
		b_set.insert(mp[tmp]);
		b_m = max(b_m, mp[tmp]);
		b_kind[i] = b_set.size();
		b_max[i] = b_m;
	}
	
	int Q; cin >> Q;
	while(Q > 0)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		string ans = "No";
		// A,Bの種類数が同じ、かつAの種類数(最大値)とBの最大値が同じ
		if(a_kind[x] == b_kind[y] && a_kind[x] == b_max[y])
		{
			ans = "Yes";
		}
		cout << ans << endl;
		Q--;
	}

	return 0;
}
