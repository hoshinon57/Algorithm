#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ARC037 https://atcoder.jp/contests/arc037

/*
 * 二分探索で解く。
 * 
 * a[]とb[]から1つずつ選んで積を取ってできる一覧をa_b[]とする。
 *   r:a_b[]においてr未満の数がK個以上
 *   l:a_b[]においてl未満の数がK個未満
 * と定義して二分探索を行うと、
 * 最終的にl+1=rになったときにlの値が「小さい方からK番目に位置する値」となる。
 * 
 * 二分探索にあたり、a_b[]を実際に作るとTLEとなる。
 * そのため、a[]の各要素において
 *   mid/a[i]の余りが0    ：lower_bound(mid/a[i])
 *   mid/a[i]の余りが0以外：lower_bound(mid/a[i]+1)   ※mid/a[i](小数点以下切り捨て)=b となるbも使えるため、+1する
 * の値が組み合わせに使えるb[]の数になる。
 *   ->小数点以下を切り上げすればよいので、(mid+e-1)/e で1つにまとめられる
 * (bは事前に昇順ソートしておく)
 * これをカウントしていけばよい。
 * 
 * [補足]
 * 公式解説などを見て思ったが、
 *   r未満の数がK個以上
 * ではなく
 *   r以下の数がK個以上
 * にした方が解きやすかったかもしれない。
 * 判定問題では、 <, > より <=, >= の方が解きやすいかもしれない。
 */
int main(void)
{
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N), b(N);
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < N; i++) cin >> b[i];
	// ここまで入力

	// bは昇順ソートしておく
	sort(b.begin(), b.end());

	ll l = 1;  // aとbの積の一覧において、1未満となる数は確実にK個未満である(a[],b[]の各要素は1以上のため)
	ll r = INF64;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		ll count = 0;
		// a*bがmid未満となる個数を数える
		for(auto &e : a)
		{
#if 0
			if(mid%e == 0)
			{
				// mid/a[]の余りが0の場合、mid/a[]=bとなるbは使えない
				// よってmid/a[]でlower_bound()を呼び出す
				auto itr = lower_bound(b.begin(), b.end(), mid/e);
				count += itr - b.begin();
			}
			else
			{
				// mid/a[]の余りが0以外の場合、
				// mid/a[](小数点以下切り捨て)=b となるbも使える
				// よってmid/a[]+1でlower_boundを呼び出す
				auto itr = lower_bound(b.begin(), b.end(), mid/e+1);
				count += itr - b.begin();
			}
#else
			// 小数点以下を切り上げすればよいので、
			// (mid+e-1)/e で1つにまとめられる
			auto itr = lower_bound(b.begin(), b.end(), (mid+e-1)/e);
			count += itr - b.begin();
#endif
		}
		if(count >= K) r = mid;
		else l = mid;
	}
	cout << l << endl;

	return 0;
}
