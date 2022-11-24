#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題78
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2011ho/tasks/joi2011ho1

/*
 * 2次元累積和で解く。
 * 以下記事を参考に、2次元累積和を構築する。
 *   https://qiita.com/drken/items/56a6b68edef8fc605821
 * 
 * まず、クエリごとに対象領域を数え上げる手法は、計算量がO(KMN)となりTLE.
 * 
 * 地形がJ/O/Iの3通りあるため、sum[3]として累積和を3つ構築する。
 * クエリについて、入力a,b,c,dをy1,x1,y2,x2と置き換えて書くと、[y1,y2] x [x1,y2] の閉区間であるため、
 * y2,x2をインクリメントして [y1,y2) x [x1,y2) のように半開区間にする。
 * すると以下の式で区間の和を求められる。
 *   answer = sum[y2][x2] - sum[y2][x1] - sum[y1][x2] + sum[y1][x1]
 * 
 * 計算量はO(MN+K).
 *   累積和の構築部分がO(MN), 
 *   クエリ部分が、各クエリに対してO(1)で答えられるのでO(K).
 *   合わせてO(MN+K).
 */

using vii = vector<vector<int>>;

int main(void)
{
	// 0-indexed
	int i, j, k;
	int M, N;
	int K;  // クエリ数
	cin >> M >> N >> K;
	vii sum[3];  // 0:J, 1:O, 2:I
	for(auto &e : sum) e.resize(M+1, vector<int>(N+1));

	// 地図入力
	for(i = 0; i < M; i++)  // y
	{
		string str;
		cin >> str;
		for(j = 0; j < N; j++)  // x
		{
			// 2次元累積和の更新式
			// sum[y+1][x+1] = sum[y+1][x] + sum[y][x+1] - sum[y][x] + a[y][x]

			// まずはa[y][x]=0として、これまでの要素から[i+1][j+1]を計算
			for(k = 0; k < 3; k++)  // J or O or I
			{
				sum[k][i+1][j+1] = sum[k][i+1][j] + sum[k][i][j+1] - sum[k][i][j] + 0;
			}

			// 次に、現在の文字に対応するsum[*]をインクリメント (a[y][x]=1 のイメージ)
			switch(str[j])
			{
			case 'J':
				sum[0][i+1][j+1]++;
				break;
			case 'O':
				sum[1][i+1][j+1]++;
				break;
			case 'I':
				sum[2][i+1][j+1]++;
				break;
			}
		}
	}

	// クエリごとの処理
	for(i = 0; i < K; i++)
	{
		int y1, x1, y2, x2;
		cin >> y1 >> x1 >> y2 >> x2;
		y1--; x1--; y2--; x2--;  // 0-indexedに変換
		y2++; x2++;  // 半開区間にするため+1
		// [x1,x2) x [y1,y2)  の半開区間の累積和
		// sum[y2][x2] - sum[y2][x1] - sum[y1][x2] + sum[y1][x1]
		for(j = 0; j < 3; j++)  // J or O or I
		{
			int ans;
			ans = sum[j][y2][x2] - sum[j][y2][x1] - sum[j][y1][x2] + sum[j][y1][x1];
			cout << ans;
			if(j != 2) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
