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

// ABC138 https://atcoder.jp/contests/abc138

/*
 * まず、s'は無限に長いとみなしてよい。
 * 'a'～'z'の各文字について、sの何番目に登場するかを昇順で持っておく。
 *   例：s=contest なら、ch_idx['t']={3,6}
 * tについて次に見つける文字が最寄りでどこかを、このidxを用いて二分探索で探す。
 * (二分探索で見つからない場合は-1を出力して終了)
 * 
 * sを2周させておくと実装が楽。
 * この場合、次の文字の探索前に、今見ている文字をsの前半側に移しておく。
 */

int main(void)
{
	int i;
	string s, t; cin >> s >> t;
	s = s+s;  // 2周させる
	vector<int> ch_idx[26];  // idx[0]:2重になったsにて、aが何番目に登場するか
	for(i = 0; i < (int)s.size(); i++)
	{
		ch_idx[s[i]-'a'].push_back(i);  // 各文字について、昇順に並ぶ
	}

	ll ans = -1;
	ll idx = -1;  // 今見ている場所 0-indexedのため初期値は-1にしておく
	for(i = 0; i < (int)t.size(); i++)
	{
		// 文字t[i]が、今見てるidx番目より後に登場するのはどこか
		// lower_bound()だとNG. tに同じ文字が続く場合に移動しなくなってしまう
		int c = t[i]-'a';
		auto itr = upper_bound(ch_idx[c].begin(), ch_idx[c].end(), idx);
		if(itr == ch_idx[c].end())
		{
			cout << -1 << endl;
			return 0;
		}
		ans += *itr-idx;
		idx = *itr % ((int)s.size()/2);  // idxの場所更新 sを2重にしているので、次のために前半側に移しておく
	}
	cout << ans+1 << endl;  // 1-indexedに変換

	return 0;
}
