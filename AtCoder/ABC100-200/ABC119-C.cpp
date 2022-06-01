#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC119 https://atcoder.jp/contests/abc119

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * それぞれの竹を、「0:Aに使う」「1:Bに使う」「2:Cに使う」「3:使わない」の4パターンに分ける。
 * 再帰を使い、長さがNで各値が0～3の数列を全作成し、
 * それぞれの数列ごとに消費MPを計算する。
 * これの最小値が答となる。
 * 
 * 数列を作ったら、まずは竹ごとに合成魔法を使う。
 * 次に目標であるA,B,Cとの長さの差の絶対値が、延長or短縮魔法にて必要なMPとなる。
 */

const int use_A = 0;
const int use_B = 1;
const int use_C = 2;
const int noUse = 3;

int N, A, B, C;
vector<int> l;
int minMP = INF32;

// 作成した数列arrを元に、消費MPを計算する
void calc(vector<int> &arr)
{
	int length[3] = {0};
	int mp = 0;

	for(int i = 0; i < N; i++)
	{
		if(arr[i] == noUse) continue;
		if(length[arr[i]] != 0)
		{
			length[arr[i]] += l[i];
			mp += 10;
		}
		else  // 1本目なので合成ではない、MP消費しない
		{
			length[arr[i]] += l[i];
		}
	}
	// 必ず1本は竹を選ぶ必要がある（0から竹を延長はできない）
	if(length[0] == 0 || length[1] == 0 || length[2] == 0) return;

	// length[0]をAに、同じく[1]をB、[2]をCに長さ調節する
	mp += abs(length[0] - A) + abs(length[1] - B) + abs(length[2] - C);
	minMP = min(minMP, mp);

}

// 再帰を使い、長さがNで各値が0～3の数列を全作成する。
void func(vector<int> &arr)
{
	if(arr.size() == N)  // 数列完成
	{
		calc(arr);
		return;
	}

	for(int i = 0; i < 4; i++)
	{
		arr.push_back(i);
		func(arr);
		arr.pop_back();
	}
}

int main(void)
{
	cin >> N >> A >> B >> C;
	l.resize(N);
	for(int i = 0; i < N; i++)  cin >> l[i];

	vector<int> arr;  // arr[i]:l[i]を竹A,B,C,(使わない)のどれに採用するか
	func(arr);

	cout << minMP << endl;

	return 0;
}
