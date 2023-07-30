#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC166 https://atcoder.jp/contests/abc166

/*
 * 参加者i,j(ただしi<j)について考えると、
 *   j-i = Aj+Ai
 * を満たすペアを求めればよい。
 * ※問題文では「番号の差の絶対値」とあるが、上記は i<j と条件を加えているので、絶対値を外して考えられる。
 * 
 * 上記を整理すると
 *   j-Aj = Ai+i
 * となり、左辺は参加者ごとに共通の値となる。
 * よって各iごとに、j-Ajがある値(=Ai+i)である参加者が何人いるかをカウントしていく。
 * これは事前に連想配列を使ってj-Ajの部分をカウントすればよい。
 * 
 * i<jという条件を追加しているので、iについて探索する前に
 * i-Aiを上記の連想配列からデクリメントしておく必要がある。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	map<int,int> cnt;  // [(Ax-idx)]=その人数
	for(i = 1; i <= N; i++)  // i:番号, a[i]:身長
	{
		cnt[i-a[i]]++;
	}

	ll ans = 0;  // 組み合わせ数はN^2のためint型を超えるケースがある(1WA)
	for(i = 1; i <= N; i++)
	{
		cnt[i-a[i]]--;  // まず自分のcntを削除
		ans += cnt[a[i]+i];
	}
	cout << ans << endl;

	return 0;
}
