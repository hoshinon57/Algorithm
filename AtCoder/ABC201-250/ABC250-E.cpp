#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC250 https://atcoder.jp/contests/abc250

/*
 * 尺取り法っぽい手法で解く。
 * (よく一発ACしたな…と思う。ソース見返して解読できん…)
 * 
 * Aの先頭x項の集合と一致するようなBの先頭y項の集合は、
 * 例えば [1,By], [1,By+1], [1,By+2], ..., [1,By+z] のように区間となる。
 * 
 * Aを先頭から見ていくときに、
 *   a_list, b_list：これまでのA,Bの集合
 *   b_start[i], b_end[i]：a[i]まで見たときに、bは1から [b_start[i], b_end[i]] の範囲で集合が一致する
 * とする。
 * 今回のa[i]がa_listに含まれていれば、b_start,endは前回と同じ。
 * 今回のa[i]がa_listに含まれていない場合：
 *   次のbがa_listに含まれていない場合、そのbは取らずに次へ。
 *   次のbがa_listに含まれている場合：
 *     含まれている間だけ、bを読み進める(b_idxを進める)。
 *     a_listとb_listのサイズが同じであれば、ここまでの処理により中身も一致しているので、b_start,endを更新していく。
 * 
 * ここまでが事前処理。
 * クエリ処理は、各クエリについてb_start[x]<=y<=b_end[x]か判定すればよい。
 */

int main(void)
{
	// 1-indexed
	int i;
	int N; cin >> N;
	vector<int> a(N+1), b(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
	}
	for(i = 1; i <= N; i++)
	{
		cin >> b[i];
	}
	set<int> a_list, b_list;
	vector<int> b_start(N+1), b_end(N+1);
	int b_idx = 1;
	for(i = 1; i <= N; i++)  // Aを先頭から見ていく
	{
		int a_tmp = a[i];
		// 今回のaiが、これまでのAの集合に含まれていれば、b_start,endは前回と同じ
		if(a_list.count(a_tmp) > 0)
		{
			b_start[i] = b_start[i-1];
			b_end[i] = b_end[i-1];
			continue;
		}
		a_list.insert(a_tmp);

		int b_tmp = b[b_idx];
		// 次のbがa_listに含まれていない場合、そのbは取らずに次へ
		if(a_list.count(b_tmp) == 0)
		{
			b_start[i] = INF32;  // INFや-1にしておく
			b_end[i] = -1;
			continue;
		}
		// 次のbがa_listに含まれている場合
		else
		{
			b_start[i] = INF32;
			b_end[i] = -1;
			// 含まれている間だけ、bを読み進める(b_idxを進める)
			while(b_idx <= N)
			{
				if(a_list.count(b[b_idx]) > 0)
				{
					b_list.insert(b[b_idx]);
					// a_listとb_listのサイズが同じであれば、
					// ここまでの処理により中身も一致しているので、b_start,endを更新していく
					if(a_list.size() == b_list.size())
					{
						// 事前にstartをINF, endを-1にしているので、min/maxで適切に更新できる
						b_start[i] = min(b_idx, b_start[i]);
						b_end[i] = max(b_idx, b_end[i]);
					}
					b_idx++;
				}
				else break;
			}
		}
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		int x, y; cin >> x >> y;
		string ans = "No";
		if(b_start[x] <= y && y <= b_end[x])
		{
			ans = "Yes";
		}
		cout << ans << endl;
		Q--;
	}

	return 0;
}
