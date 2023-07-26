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

// ABC122 https://atcoder.jp/contests/abc122

/*
 * 累積和で解く。
 * 
 * i番目のが'C', 1つ手前が'A'となるような箇所に+1
 * として累積和を構築する。
 * "AC"の右側に+1するイメージ。
 * 
 * クエリ計算[l,r]では、lが"AC"の'C'部分だと正しく求められないため(※)、lを+1して考える。
 * (※)A|C のようにACが切られるようなlが設定されるイメージ
 * 後は通常の累積和のように、半開区間 [l,r+1) で解ける。
 */

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	string s; cin >> s;
	vector<int> s_sum(N+1);
	for(i = 0; i < N; i++)
	{
		s_sum[i+1] = s_sum[i];
		if(i > 0 && s[i-1] == 'A' && s[i] == 'C') s_sum[i+1]++;
	}

	while(Q > 0)
	{
		int l, r; cin >> l >> r;
		l--; r--;
		
		// "AC"の'C'部分で累積和を+1しているので、左端は+1する。
		// 最初、s[l]が'C'なら+1 としていたが、無条件で+1で良い。
		// if(s[l] == 'C') l++;
		l++;

		cout << s_sum[r+1] - s_sum[l] << endl;
		Q--;
	}

	return 0;
}
