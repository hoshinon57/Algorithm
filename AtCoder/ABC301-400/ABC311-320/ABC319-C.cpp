#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC319 https://atcoder.jp/contests/abc319

/*
 * 解説を元に実装しなおした。
 * ※コンテスト時はだいぶシッチャカメッチャカな実装になった…
 *   https://atcoder.jp/contests/abc319/editorial/7114
 *   https://www.youtube.com/watch?v=26jzAnxw-S4
 * 
 * 「iマス目を何番目に開けるか」について全探索する。9!=362880であり、十分に間に合う。
 * 全探索にはnext_permutation()を用いる。
 * 
 * ある「全9マスについて、何番目に開けるか」を保持したarrについて、「がっかり」するかどうかの判定を考える。
 * マスi,j,kの3マスについて考えると、
 *   {arr[i],c[i]}, {arr[j],c[j]}, {arr[k],c[k]}
 * の「何番目に開けるか, その値」の組み合わせを作ってソートすると、
 * [0]と[1]のsecondが一致するかどうかでがっかり判定ができる。
 * これを関数fとして実装すると、各列/行/斜めそれぞれについて呼び出せばよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・ある列について、1番目と2番目に開く値が一致するかどうかで判定すると楽だった。
 * ・そのためには、next_permutaion()に使う配列を
 *   「各マスを、何番目に開けるか」として定義するのが良さそう。
 *   ※「i番目に開けるマスは何か」で定義すると、不可能ではないが大変そう。
 */

int c[9];

// 「全9マスについて、何番目に開けるか」を保持したarrを元に「がっかり」判定
// がっかりしないならtrueを返す
bool check(vector<int> &arr)
{
	bool ok = true;
	// マスi,j,kからなる列/行/斜めについて、「がっかり」するかどうかを判定
	auto f = [&](int i, int j, int k) -> void
	{
		vector<pair<int,int>> p;  // {何番目に開けるか, その値}
		p.push_back({arr[i],c[i]});
		p.push_back({arr[j],c[j]});
		p.push_back({arr[k],c[k]});
		sort(p.begin(), p.end());
		if(p[0].second == p[1].second) ok = false;
	};
	f(0, 1, 2);
	f(3, 4, 5);
	f(6, 7, 8);
	f(0, 3, 6);
	f(1, 4, 7);
	f(2, 5, 8);
	f(0, 4, 8);
	f(2, 4, 6);
	return ok;
}

int main(void)
{
	int i;
	for(i = 0; i < 9; i++) cin >> c[i];
	vector<int> arr(9);  // arr[i]:iマス目を何番目に開けるか
	iota(arr.begin(), arr.end(), 0);
	int count = 0, tot = 0;
	do
	{
		if(check(arr)) count++;
		tot++;
	} while (next_permutation(arr.begin(), arr.end()));
	cout << std::fixed << std::setprecision(20);
	cout << (double)count / tot << endl;

	return 0;
}
