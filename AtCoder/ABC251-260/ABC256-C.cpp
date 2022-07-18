#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC256 https://atcoder.jp/contests/abc256

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * 長さ9の数列で、各値が1～30となるものを列挙する。
 * ただしそのままでは 30^9 で到底終わらないので、枝刈りを行う。
 * 
 * 以下、配列a[]およびcalc[]の添字は、3x3のマスにおいて
 *  0|1|2
 * -------
 *  3|4|5
 * -------
 *  6|7|8
 * を表す。
 */

vector<int> h(3), w(3);
ll answer = 0;

// 枝刈り
// ここまでのa[]で条件を満たさないと判断できれば、trueを返す
bool edagari_NG(vector<int> &a)
{
	int i;
	vector<int> calc(9);
	for(i = 0; i < a.size(); i++)
	{
		calc[i] = a[i];
	}

	// h[], w[]を超えていれば、この時点で条件を満たさない
	if(calc[0]+calc[1]+calc[2] > h[0]) return true;
	if(calc[3]+calc[4]+calc[5] > h[1]) return true;
	if(calc[6]+calc[7]+calc[8] > h[2]) return true;

	if(calc[0]+calc[3]+calc[6] > w[0]) return true;
	if(calc[1]+calc[4]+calc[7] > w[1]) return true;
	if(calc[2]+calc[5]+calc[8] > w[2]) return true;

	// 例えば1行目の3つを決めた段階で、h1と比較してOK/NGを判断する
	// それを1行目, 2行目, 1列目, 2列目で判定する
	if(a.size() >= 3)
	{
		if(calc[0]+calc[1]+calc[2] != h[0])  return true;
	}
	if(a.size() >= 6)
	{
		if(calc[3]+calc[4]+calc[5] != h[1])  return true;
	}
	if(a.size() >= 7)
	{
		if(calc[0]+calc[3]+calc[6] != w[0]) return true;
	}
	if(a.size() >= 8)
	{
		if(calc[1]+calc[4]+calc[7] != w[1]) return true;
	}

	return false;  // 現時点では矛盾なし
}

// 長さ9の数列を作り切ったので、判定
bool check(vector<int> &a)
{
	if(a[0]+a[1]+a[2] != h[0]) return false;
	if(a[3]+a[4]+a[5] != h[1]) return false;
	if(a[6]+a[7]+a[8] != h[2]) return false;

	if(a[0]+a[3]+a[6] != w[0]) return false;
	if(a[1]+a[4]+a[7] != w[1]) return false;
	if(a[2]+a[5]+a[8] != w[2]) return false;

	return true;
}

/*
 * 長さ9の数列で、各値が1～30となるものを、
 * 再帰を用いた深さ優先探索にて列挙する(30^9)。
 * そのままでは到底終わらないので、枝刈りを行う。
 */
void func(vector<int> &a)
{
	if(edagari_NG(a)) return;  // 枝刈り

	if(a.size() == 9)  // 必要な桁ぶん積んだので処理
	{
		if(check(a)) answer++;
		return;
	}

	// 再帰の処理
	for(int i = 1; i <= 30; i++)
	{
		a.push_back(i);
		func(a);
		a.pop_back();
	}

	return;
}

int main(void)
{
	cin >> h[0] >> h[1] >> h[2] >> w[0] >> w[1] >> w[2];

	vector<int> a;
	func(a);

	cout << answer << endl;
	return 0;
}
