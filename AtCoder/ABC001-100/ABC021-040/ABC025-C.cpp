#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC025 https://atcoder.jp/contests/abc025

/*
 * メモ化再帰でゲームDPを解く。
 * 解説としては以下が最も分かりやすかった。
 *   https://drken1215.hatenablog.com/entry/2023/07/22/192817
 * 
 * 盤面の状態をmp[3][3]にて管理し、このときに互いが最善手を尽くしたときの、先攻側のスコアを考える。
 * 先攻はこれの最大化を、後攻はこれの最小化を目指す。
 * ※後攻のスコアは、入力b[],c[]と先攻のスコアから求められるので、管理不要。
 * 
 * mp[]はそのマスが未入力なら-1, oなら0, xなら+1として実装している。
 * 全マス入力していたら、スコア計算して終了。
 * 
 * 空きマスがあれば、それら全てを試して
 *   先攻：スコア最大化
 *   後攻：スコア最小化
 * を目指す。
 */

map<vector<vector<int>>, int> sc;  // 盤面と、そこから始めたときのスコア
int b[2][3], c[3][2];

// 盤面の状態がmpから始めて、互いが最善手を尽くしたときに、先攻のスコアを返す
int func(vector<vector<int>> &mp)
{
	if(sc.count(mp) > 0) return sc[mp];  // メモ済み

	int i, j;
	int t = 0;  // 現在のターン  0-indexed	
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(mp[i][j] != -1) t++;  // 埋まったマス
		}
	}
	if(t == 9)  // 全マス埋まったらスコア計算
	{
		int s = 0;
		for(i = 0; i < 3; i++)
		{
			for(j = 0; j < 3; j++)
			{
				if(i+1 < 3 && mp[i][j] == mp[i+1][j]) s += b[i][j];
				if(j+1 < 3 && mp[i][j] == mp[i][j+1]) s += c[i][j];
			}
		}
		return (sc[mp] = s);
	}
	
	int s;
	if(t%2 == 0) s = -1;  // tの偶奇で、次にどちらのターンかが分かる
	else s = INF32;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(mp[i][j] != -1) continue;
			mp[i][j] = t%2;
			int tmp = func(mp);
			if(t%2 == 0) chmax(s, tmp);  // 先攻なら最大化、後攻なら最小化を目指す
			else chmin(s, tmp);
			mp[i][j] = -1;
		}
	}
	return (sc[mp] = s);
}

int main(void)
{
	int i, j;
	int tot = 0;
	for(i = 0; i < 3-1; i++)
	{
		for(j = 0; j < 3; j++)
		{
			cin >> b[i][j];
			tot += b[i][j];
		}
	}
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3-1; j++)
		{
			cin >> c[i][j];
			tot += c[i][j];
		}
	}
	vector<vector<int>> mp(3, vector<int>(3, -1));
	int p1 = func(mp);
	int p2 = tot-p1;
	cout << p1 << endl;
	cout << p2 << endl;

	return 0;
}
