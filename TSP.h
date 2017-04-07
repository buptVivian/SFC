#include<algorithm>
#include<cstdio>
#include<queue>
using namespace std;

static const int MAX_N = 22;
const int INF = 100000;
static int n;//节点个数

vector <vector<int>> cost;

typedef struct Node
{
	bool visited[MAX_N];//标记哪些点走了
	int path[MAX_N]; //标记访问路径 -1为空
	//int s;//第一个点
	//int s_p;//第一个点的邻接点
	//int e;//最后一个点
	//int e_p;//最后一个点的邻接点
	int k;//走过的点数
	int sumv;//经过路径的距离
	int lb;//目标函数的值（目标结果）
	
	bool operator <(const Node &p)const
	{
		return p.lb < lb;//目标函数值小的先出队列
	}
};
priority_queue<Node> pq;//创建一个优先队列
int low, up;//下界和上界
bool dfs_visited[MAX_N];//在dfs过程中搜索过

int dfs(int u, int k, int l);
void get_up();
void get_low();
int get_lb(Node p);

int* calculateOrder(vector<vector<int>> cost);


