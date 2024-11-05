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

// ABC368 https://atcoder.jp/contests/abc368

/*
 * Grundy数で解く。
 * 
 * Aに対する操作を、Aの(A自身を除く)約数への遷移になると考えると、
 * AのGrundy数は遷移先のGrundy数のmexとなる。
 * これをまずエラトステネスの篩っぽく1～10^5まで求めておく。計算量はAの最大値をTとしてO(TlogT)とかそんな感じ。
 * 
 * ここから各AのGrundy数のxorを取り、0以外なら先手勝ち。
 */

// a[]の要素に含まれない最小の非負整数を返す
// 呼び出し元で mex({0,2,3}); のようにinitializer_listを用いることを想定しているため、aは参照渡しにしていない
// 計算量はmex()と違いO(N)だが、Nが小さければmex()の方が早い (定数倍によるものと推測)
int mex2(vector<int> a)
{
	int i;
	int a_size = (int)a.size();
	vector<bool> exist(a_size, false);  // [i]:a[]にiがあればtrue
	for(i = 0; i < a_size; i++)
	{
		if(a[i] >= a_size) continue;
		exist[a[i]] = true;
	}
	// exist[]がfalseになる最小の値を求める
	// (入力が {0,1,2} のような場合、i=3でループを抜けるため正しく判定される)
	for(i = 0; i < a_size; i++)
	{
		if(!exist[i]) break;
	}
	return i;
}

int main(void)
{
	int i, j;
	const int MX = 1e5;
	vector<vector<int>> divi(MX+1);  // divi[val]:値valの約数一覧
	for(i = 1; i <= MX; i++)
	{
		for(j = i+i; j <= MX; j+=i)
		{
			divi[j].push_back(i);
		}
	}
	vector<int> grundy(MX+1, INF32);  // mexを求めたいのでINFにしておく
	for(i = 1; i <= MX; i++)
	{
		vector<int> g_divi;
		for(auto &e : divi[i]) g_divi.push_back(grundy[e]);  // iの約数のgrundy数、の一覧
		grundy[i] = mex2(g_divi);  // 上記からmexを求めたものが、iのGrundy数
	}

	int N; cin >> N;
	int totg = 0;
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		totg ^= grundy[a];
	}
	if(totg != 0) cout << "Anna" << endl;
	else cout << "Bruno" << endl;
	return 0;
}
