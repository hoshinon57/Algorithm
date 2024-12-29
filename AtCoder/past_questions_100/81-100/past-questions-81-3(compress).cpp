#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC014 https://atcoder.jp/contests/abc014

/*
 * 解説にて「座圧すると～～」という記載があり、実装してみた版。
 * ※解説にはO(N)とあったが、座圧にはO(NlogN)かかると思うので、結局はO(NlogN)な気がする。(O(N)手法あるのかな？)
 * 
 * a,bの最大値をMとして、単純にいもす法を使うとO(N+M).
 * これでも今回の制約では十分に間に合うが、N<MなのでMに依存しない方法を…というのが趣旨。
 * ※そういえば、イベントソートだったらMに寄らないね
 * 
 * 入力のa,bを1つの配列にまとめ、座標圧縮をする。
 * 座標圧縮したデータをa,bに戻し、いもす法を実施すればよい。
 */

// 1次元の座標圧縮
// a：座標圧縮したい元データ 処理途中で要素が書き換えられる点に注意(保持したい場合は参照を外す)
// 返り値：圧縮後のデータ
// 計算量はO(NlogN)
template <typename T>
vector<T> compression_one(vector<T> &a)
{
	vector<T> a_comp = a;

	// 元データをソートし、重複を削除する
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する

	// それぞれの元データが「何番目に小さいか」をlower_bound()で求める
	for(int i = 0; i < (int)a_comp.size(); i++)
	{
		a_comp[i] = lower_bound(a.begin(), a.end(), a_comp[i]) - a.begin();
	}

	return a_comp;
}

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> d;
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		d.push_back(a);
		d.push_back(b);
	}
	vector<int> d_comp = compression_one<int>(d);
	vector<int> a, b;
	for(i = 0; i < N*2; i+=2)
	{
		a.push_back(d_comp[i]);
		b.push_back(d_comp[i+1]);
	}

	vector<int> imos(N*2+10);  // a,b2種の要素があるのでワーストでN*2
	for(i = 0; i < N; i++)
	{
		imos[a[i]]++;
		imos[b[i]+1]--;  // [a,b]の閉区間なので+1
	}
	int ans = imos[0];
	for(i = 1; i <= N*2+5; i++)
	{
		imos[i] += imos[i-1];
		chmax(ans, imos[i]);
	}
	cout << ans << endl;

	return 0;
}
