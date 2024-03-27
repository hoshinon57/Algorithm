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
#define YesNo(T) std::cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC345 https://atcoder.jp/contests/abc345

/*
 * bit全探索＆next_permutaionによる全探索で解く。
 * 
 * ・N枚のタイルのうちどれを使うかをbit全探索
 * ・どの順番に置くかをnext_permutaion()で全探索
 * ・タイルは盤面左上側の空きマスから置く
 * という手順でDFSで解ける。
 * ※「回転や裏返しOK」⇒結局は縦横反転の2通り
 * 
 * main()にてarr[]に使うタイルの番号のみを入れている。
 * これをやらずに常にarr={0,1,2,...N-1}としてdfs()内で使う/使わないを判定する場合、TLEだった。
 * main()にて無駄な判定が増えるからかな。(O記法の計算量としては同じだが、定数倍が悪い)
 * 
 * [ACまでの思考の流れ]
 * ・ABC322-D(Polyomino)やABC307-C(Ideal Sheet)を思い出した。
 *   全探索することを考える。
 * ・コンテスト当初、各タイルについて「どこに置くか」を全探索したらTLE.
 *   各置き方についてそれぞれDFSしていたので、べき乗のオーダーになってしまってた模様。反省。
 *   タイルを置く順番も全探索し、次に置くマスは「左上側で空いているマス」として固定することでAC.
 */

int N, H, W;
vector<int> a, b;

// idx:次に置くタイルのidx
// arr:使うタイルの番号
bool dfs(int idx, vector<int> &arr, vector<vector<int>> &mp)
{
	if(idx == (int)arr.size())
	{
		// 全てのマスが埋まっていたらOK
		for(int y = 0; y < H; y++)
		{
			for(int x = 0; x < W; x++)
			{
				if(mp[y][x] == 0) return false;
			}
		}
		return true;
	}

	// 左上側の空きマスを探す
	int sy, sx;
	auto search = [&](void) -> bool
	{
		for(sy = 0; sy < H; sy++)
		{
			for(sx = 0; sx < W; sx++)
			{
				if(mp[sy][sx] == 0) return true;
			}
		}
		return false;
	};
	if(!search()) return false;  // 全部埋まってたらNGとしておく(idx==末尾のベースケースで判定するので)

	auto put = [&](void) -> bool
	{
		int y, x;
		for(y = sy; y < sy+a[arr[idx]]; y++)
		{	
			for(x = sx; x < sx+b[arr[idx]]; x++)
			{
				if(y >= H || x >= W) return false;
				if(mp[y][x] == 1) return false;
				mp[y][x] = 1;
			}
		}
		return true;
	};
	auto mp_org = mp;
	for(int i = 0; i < 2; i++)
	{
		swap(a[arr[idx]], b[arr[idx]]);
		mp = mp_org;
		if(put())
		{
			// 置けたら再帰
			if(dfs(idx+1, arr, mp)) return true;
		}
	}

	return false;
}

int main(void)
{
	// 0-indexed
	int i, j;
	cin >> N >> H >> W;
	a.resize(N);
	b.resize(N);
	for(i = 0; i < N; i++) cin >> a[i] >> b[i];

	bool ans = false;
	for(i = 0; i < 1<<N; i++)
	{
		vector<int> arr;  // 使うタイルの番号
		for(j = 0; j < N; j++)
		{
			if((i>>j)&1) arr.push_back(j);
		}  // arrはソート済みになる

		do
		{
			vector<vector<int>> mp(H, vector<int>(W, 0));  // mp[H][W]
			if(dfs(0, arr, mp)) ans = true;
		} while (next_permutation(arr.begin(), arr.end()));
	}
	YesNo(ans);

	return 0;
}
