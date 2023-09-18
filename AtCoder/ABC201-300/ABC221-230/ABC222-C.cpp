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

// 実際にシミュレーションして解く

// {勝ち数, 人の番号}のpairについて、問題文に合わせてソート
bool myCompare(pair<int,int> &a, pair<int,int> &b)
{
	if(a.first != b.first) return a.first > b.first;
	return a.second < b.second;
}

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<string> hand(N*2);  // hand[i]：人iのじゃんけんの手
	for(i = 0; i < N*2; i++)
	{
		cin >> hand[i];
	}

	vector<pair<int,int>> data;  // {勝ち数, 人の番号}
	for(i = 0; i < N*2; i++)
	{
		data.push_back({0, i});
	}

	// じゃんけんをしてh1の手が勝つならtrue, 負けやあいこならfalse
	auto jan = [](char h1, char h2) -> bool
	{
		if(h1 == 'G' && h2 == 'C') return true;
		if(h1 == 'C' && h2 == 'P') return true;
		if(h1 == 'P' && h2 == 'G') return true;
		return false;
	};

	for(i = 0; i < M; i++)  // iラウンド目
	{
		for(j = 0; j < N*2; j+=2)  // 人jとj+1がじゃんけん
		{
			char h1 = hand[data[j].second][i];
			char h2 = hand[data[j+1].second][i];
			if(jan(h1, h2)) data[j].first++;
			if(jan(h2, h1)) data[j+1].first++;
		}
		sort(data.begin(), data.end(), myCompare);  // 勝ち数、人の番号の順にソート
	}
	for(auto &e : data)
	{
		cout << e.second+1 << endl;
	}

	return 0;
}
