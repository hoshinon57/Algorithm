#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC049 https://atcoder.jp/contests/abc049/
// AtCoder Beginners Selectionより

/*
 * 先頭から見ていくと dream/dreamer のように選択肢が複数あり得るが、
 * 末尾から見ていくと一意に決まる。
 * [memo]深さ優先探索で解く方法も試したが、「メモリ制限超過」により不正解となった。
 */
int main(void)
{
	string S, T;
	cin >> S;

	int last = S.size();
	bool result = true;
	while(last > 0)  // 末尾から4パターンそれぞれ判定していく
	{
		if(S.substr(max(last-5,0), 5) == "dream")  // last-xが負の可能性があるため、max()で抑える
		{
			last -= 5;
		}
		else if(S.substr(max(last-7,0), 7) == "dreamer")
		{
			last -= 7;
		}
		else if(S.substr(max(last-5,0), 5) == "erase")
		{
			last -= 5;
		}
		else if(S.substr(max(last-6,0), 6) == "eraser")
		{
			last -= 6;
		}
		else
		{
			result = false;
			break;
		}
	}

	if(result)
	{
		cout << "YES" << endl;
	}
	else{
		cout << "NO" << endl;
	}

	return 0;
}
