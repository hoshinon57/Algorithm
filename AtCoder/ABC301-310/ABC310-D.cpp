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

// ABC310 https://atcoder.jp/contests/abc310

/*
 * コンテスト中に解けず、解説を見た。
 * 深さ優先探索で解く。
 * 
 * 人1から順番に
 *   新たなチームを作成して、そこに入れる
 *   既に作成しているチームに入れる
 * を繰り返していく。
 * 全員をチームに加えた後、
 *   チーム数がTと一致しているか
 *   各チームについて、相性の悪いペアが存在しないか
 * をチェックすればよい。
 * 
 * N人をTチームに分けることから、計算量がO(T^N)とかになりそうと当初思ったが、
 * 「新たなチームに入れる」「既存のチームに入れる」
 * という処理にすることでO(N!)になる。
 * ※解説のo(N*N!)になる理由はちょっとわからず…
 */

int N, T, M;
vector<vector<bool>> hate;  // [i][j]:人i,jの相性が悪いならtrue
vector<vector<int>> team;  // [i]:i番目のチームの人たち 空ならチーム未作成
int team_num = 0;  // 現在作成したチーム数
int ans = 0;

// 人noを新たにチームに加える
void dfs(int no)
{
	int i, j, k;

	// 全N人をチームに加えた
	if(no == N)
	{
		if(team_num != T) return;  // チーム数がTと不一致
		// 各チームに属する選手を見ていって、相性が悪いペアがいるかチェック
		for(i = 0; i < team_num; i++)
		{
			bool ng = false;
			for(j = 0; j < (int)team[i].size(); j++)
			{
				for(k = j+1; k < (int)team[i].size(); k++)
				{
					if(hate[team[i][j]][team[i][k]]) ng = true;
				}
			}
			if(ng) return;
		}
		// チーム数がT, 相性悪いペア無し、よってチーム作成OK
		ans++;
		return;
	}

	// 新規チームに入れて再帰
	team[team_num].push_back(no);
	team_num++;
	dfs(no+1);
	team_num--;
	team[team_num].pop_back();

	// 今あるチームに入れて再帰
	for(i = 0; i < team_num; i++)
	{
		team[i].push_back(no);
		dfs(no+1);
		team[i].pop_back();
	}

}

int main(void)
{
	int i;
	cin >> N >> T >> M;
	hate.resize(N, vector<bool>(N, false));
	team.resize(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--;
		b--;
		hate[a][b] = true;
		hate[b][a] = true;
	}

	dfs(0);
	cout << ans << endl;

	return 0;
}
