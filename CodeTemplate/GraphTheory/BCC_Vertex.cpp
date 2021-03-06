//每个点双连通分量和割点分别作为树上的点，标记边的序号表示边所在的点双连通分量
#define maxV 20005
#define maxE 200005
#define th(x) this->x = x
int rmq[2*maxV];
struct ST {
    int mm[2*maxV];
    int best[20][2*maxV];
    void init(int n) {
        int i,j,a,b;
        mm[0]=-1;
        for(i=1; i<=n; i++) {
            mm[i]=((i&(i-1))==0)?mm[i-1]+1:mm[i-1];
            best[0][i]=i;
        }
        for(i=1; i<=mm[n]; i++) {
            for(j=1; j<=n+1-(1<<i); j++) {
                a=best[i-1][j];
                b=best[i-1][j+(1<<(i-1))];
                best[i][j]=rmq[a] < rmq[b]?a:b;
            }
        }
    }
    int query(int a, int b) {
        if(a > b)    swap(a, b);
        int t;
        t=mm[b-a+1];
        a=best[t][a];
        b=best[t][b-(1<<t)+1];
        return rmq[a] < rmq[b] ? a : b;
    }
};
//下面是LCA2RMQ开始
struct Nod {
    int b, nxt;
    void init(int b, int nxt) {
        th(b);    th(nxt);
    }
};
/**
    lca转化为rmq问题
    注意: 
        1.maxV为最大节点数, ST的数组和F、buf要设置为2*maxV!!!!!
        2.ST类为1开始，此类依然为0开始!
        3.F是欧拉序列,rmq是深度序列,P某点在欧拉序列中第一次出现的位置
*/
struct LCA2RMQ {
    int n;                            //节点个数
    Nod buf[2*maxV];    int len;    int E[maxV];    //Tree    资源
    int V[2*maxV], route[maxV];                //路径的权，根到某点的路径和
    char vis[maxV];                        //0没有访问，1正在访问
    
    ST st;                            //Spare-Table...
    int F[2*maxV], P[maxV], cnt;    //介绍如上,cnt为计数器,lev为dfs时层数(减少递归栈大小)
    
    void init(int n) {
        th(n);
        len = 0;
        memset(E, 255, sizeof(E));
    }
    void addEdge(int a, int b, int v) {
        //cout << a << " " << b << " " << v << endl;
        buf[len].init(b, E[a]);        V[len] = v;        E[a] = len ++;
        buf[len].init(a, E[b]);        V[len] = v;        E[b] = len ++;
    }
    int query(int a, int b) {                //传入两个节点，返回他们lca节点编号
        return F[st.query(P[a], P[b])];
    }
    //1.【递归版】
    void dfs(int a, int lev) {
        vis[a] = 1;
        
        ++ cnt;
        F[cnt] = a;
        rmq[cnt] = lev;
        P[a] = cnt;
        for(int i = E[a]; i != -1; i = buf[i].nxt) {
            if(vis[buf[i].b])    continue;
            route[buf[i].b] = route[a] + V[i];
            dfs(buf[i].b, lev+1);
            ++ cnt;
            F[cnt] = a;
            rmq[cnt] = lev;
        }
    }
    void solve(int root) {
        memset(vis, 0, sizeof(vis));
        route[root] = cnt = 0;
        dfs(root, 0);
        st.init(2*n-1);
    }
    
    //2.【非递归版】
/*    void solve(int root) {
        static Nod stk[maxV];
        memset(vis, 0, sizeof(vis));
        
        stk[0].init(root, E[root]);
        int len = 1;
        int lev = 0;
        cnt = 0;
        route[root] = 0;                                        //update route!
        
        while(1) {
            Nod & cur = stk[len-1];
//here, b means current Nod, nxt means nxt's pointer(as buf.nxt)!
            if(false == vis[cur.b]) {
                vis[cur.b] = 1;
                F[++ cnt] = cur.b;
                rmq[cnt] = lev;
                P[cur.b] = cnt;
            } else if(cur.nxt == -1) {
                if(--len == 0)    break;
                F[++ cnt] = stk[len-1].b;
                rmq[cnt] = -- lev;
            } else {
                int & i = cur.nxt;
                if(!vis[buf[i].b]) {
                    route[buf[i].b] = route[cur.b] + V[i];        
//update route!
                    lev ++;
                    stk[len ++].init(buf[i].b, E[buf[i].b]);
                }
                i = buf[i].nxt;
            }
        }
        st.init(2*n-1);
    }*/
} lca;

struct Graph {
    vector<pair<int, int> > adj[maxV];
    int dfn[maxV], low[maxV];
    bool vst[maxV];
    bool visit[maxE];
    int N, M, size, depth;
    int edgeId[maxE];
    stack<int> S;
    vector<int> block;
    vector<int> vertex_bcc[maxV];

    void init(int n) {
        N = n;
        memset(vst, 0, sizeof(vst));
        memset(visit, 0, sizeof(visit));
        for (int i = 0; i < N; i++)
            adj[i].clear();
        for (int i = 0; i < N; i++)
            vertex_bcc[i].clear();
    }

    void addEdge(int u, int v, int id) {
        adj[u].PB(MP(v, id));
        adj[v].PB(MP(u, id));
    }
    
    void CountBlock() {
        size++;
        for (int i = 0; i < block.size(); i++) {
            vertex_bcc[block[i]].PB(size);
        }
    }

    void Tarjan(int u) {
        dfn[u] = low[u] = depth++;
        S.push(u);
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i].first;
            int id = adj[u][i].second;
            if (dfn[v] == -1) {
                visit[id] = true;//It's an undirected graph
                Tarjan(v);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    block.clear();
                    int x;
                    do {
                        x = S.top(); S.pop();
                        block.PB(x);
                    }while (x != v);
                    block.PB(u);
                    CountBlock();    
                }
            } else if (!visit[id]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    int getBlock(int x, int y) {
        for (int i = 0, j = 0; i < vertex_bcc[x].size() && j < vertex_bcc[y].size(); ) {
            if (vertex_bcc[x][i] == vertex_bcc[y][j]) return vertex_bcc[x][i];
            else if (vertex_bcc[x][i] < vertex_bcc[y][j]) i++;
            else j++;
        }
        return -1;
    }

    void BCC() {
        depth = 0; size = 0;
        while (!S.empty()) S.pop();
        for (int i = 0; i < N; i++)
            dfn[i] = -1;
        for (int i = 0; i < N; i++)
            if (dfn[i] == -1) Tarjan(i);
    }

    void build() {
        lca.init(N + 1);
        int root = 0;
        for (int i = 0; i < N; i++) {
            sort(vertex_bcc[i].begin(), vertex_bcc[i].end());
            vertex_bcc[i].resize(unique(vertex_bcc[i].begin(), vertex_bcc[i].end()) - vertex_bcc[i].begin());
            if (vertex_bcc[i].size() > 1) {
                size++;
                for (int j = 0; j < vertex_bcc[i].size(); j++) {
                    int v = vertex_bcc[i][j];
                    lca.addEdge(v, size, 1);
                }
                lca.addEdge(root, size, 1);
            }
        }
        lca.n = size + 1;
    }

}g;

//Hdu3686
int n, m, q;
int edgeA[maxE], edgeB[maxE];
int main(int argc, char const *argv[])
{
    while (~scanf("%d%d", &n, &m) && n + m) {
        g.init(n);
        for (int i = 0; i < m; i++) {
            int a, b;
            scanf("%d%d", &a, &b);
            edgeA[i] = a - 1; edgeB[i] = b - 1;
            g.addEdge(a - 1, b - 1, i);
        }
        g.BCC();
        for (int i = 0; i < m; i++) {
            g.edgeId[i] = g.getBlock(edgeA[i], edgeB[i]);
        }
        g.build();
        lca.solve(0);
        scanf("%d", &q);
        while (q--) {
            int a, b;
            scanf("%d%d", &a, &b);
            a = g.edgeId[a - 1], b = g.edgeId[b - 1];
            int anc = lca.query(a, b);
            int ret = lca.route[a] + lca.route[b] - 2 * lca.route[anc];
            printf("%d\n", ret / 2);
        }
    }
    return 0;
}

//hdu3686
int n, m, q;
int main(int argc, char const *argv[])
{
	while (~scanf("%d%d", &n, &m) && n + m) {
		g.init(n);
		for (int i = 0; i < m; i++) {
			int a, b;
			scanf("%d%d", &a, &b);
			g.addEdge(a - 1, b - 1, i);
		}
		g.BCC();
		g.build();
		lca.solve(0);
		scanf("%d", &q);
		while (q--) {
			int a, b;
			scanf("%d%d", &a, &b);
			a--; b--;
			a = g.idx[a]; b = g.idx[b];
			if (a == b) {
				printf("0\n");
			} else {
				int anc = lca.query(a, b);
				if (anc == 0) puts("0");
				else {
					int ret = lca.route[a] + lca.route[b] - 2 * lca.route[anc];
					printf("%d\n", ret / 2);
				}
			}
		}
	}
	return 0;
}


#define MAXN 20005
#define MAXM 200005
struct Graph {
	vector<pair<int, int> > adj[MAXN];
	int dfn[MAXN], low[MAXN];
	bool vst[MAXN];
	bool visit[MAXM];
	int N, M, size, depth;
	int ret1, ret2;
	stack<int> S;
	vector<int> block;

	void init(int n) {
		N = n;
		memset(vst, 0, sizeof(vst));
		memset(visit, 0, sizeof(visit));
		for (int i = 0; i < n; i++)
			adj[i].clear();
	}

	void addEdge(int u, int v, int id) {
		adj[u].PB(MP(v, id));
		adj[v].PB(MP(u, id));
	}

	int CountBlock() {
		for (int i = 0; i < block.size(); i++)
			vst[block[i]] = true;
		int sum = 0;
		for (int i = 0; i < block.size(); i++) {
			int u = block[i];
			for (int j = 0; j < adj[u].size(); j++) {
				if (vst[adj[u][j].first])
					sum++;
			}
		}

		sum /= 2;
		if (sum > block.size())
			ret2 += sum;
		if (sum < block.size())
			ret1 += sum;
		for (int i = 0; i < block.size(); i++)
			vst[block[i]] = false;
	}

	void Tarjan(int u) {
	    dfn[u] = low[u] = depth++;
		S.push(u);
		for (int i = 0; i < adj[u].size(); i++) {
			int v = adj[u][i].first;
			int id = adj[u][i].second;
			if (dfn[v] == -1) {
				visit[id] = true;//It's an undirected graph
				Tarjan(v);
	            low[u] = min(low[u], low[v]);
				if (low[v] >= dfn[u]) {
					block.clear();
					int x;
					do {
						x = S.top();
						S.pop();
						block.PB(x);
					}while (x != v);
					block.PB(u);
					CountBlock();
				}
	        } else if (!visit[id]) {
	            low[u] = min(low[u], dfn[v]);
	        }
	    }
	}

	void BCC() {
		depth = 0; size = 0;
		ret1 = ret2 = 0;
		while (!S.empty())
			S.pop();
		for (int i = 0; i < N; i++)
			dfn[i] = -1;
	    for (int i = 0; i < N; i++)
		    if (dfn[i] == -1) Tarjan(i);
		printf("%d %d\n", ret1, ret2);
	}

}g;

int a[MAXM], b[MAXM];
int n, m;
/*
 Hdu3394
 求无向图中不属于任意一个环的边数和属于多个环的边数
 求点双连通分量
 如果一个块的边数大于顶点数，那么这些边都是属于多个环的
 如果一个块的边数小于顶点数，那么这些边都是不属于任意环的
 邻接链表实现
 */
int main() {
	while (scanf("%d%d", &n, &m) && n + m) {
		g.init(n);
		for (int i = 0; i < m; i++) {
			scanf("%d%d", &a[i], &b[i]);
			g.addEdge(a[i], b[i], i);
		}
		g.BCC();
	}
	return 0;
}

#define MAXN 1005
struct Graph {
	bool adj[MAXN][MAXN];
	int dfn[MAXN], low[MAXN];
	bool vst[MAXN];
	int N, M, size, depth;
	stack<int> S;
	int block[MAXN], numB;
	int col[MAXN];
	bool odd;
	int que[MAXN];

	void init(int n) {
		N = n;
		//memset(adj, 0, sizeof(adj));
	}

	void dfs(int u, int c) {
		if (odd) return;
		if (col[u] != -1) {
			if (col[u] != c) {
				odd = true;
			}
			return;
		}
		col[u] = c;
		for (int i = 0; i < numB; i++)
			if (adj[u][block[i]]) {
				dfs(block[i], 1 - c);
			}
	}

	void CheckBlock() {
		if (numB == 1) return;
		for (int i = 0; i < numB; i++) {
			col[block[i]] = -1;
			//printf("%d ", block[i]);
		}
		//printf("\n");
		odd = false;
		dfs(block[0], 0);
		if (odd) {
			for (int i = 0; i < numB; i++)
				vst[block[i]] = true;
		}
	}

	void Tarjan(int u) {
	    dfn[u] = low[u] = depth++;
		S.push(u);
		for (int v = 0; v < N; v++) {
			if (adj[u][v]) {
				adj[v][u] = 0;
				if (dfn[v] == -1) {
					Tarjan(v);
					low[u] = min(low[u], low[v]);
					if (low[v] >= dfn[u]) {
						numB = 0;
						int x;
						do {
							x = S.top(); S.pop();
							block[numB++] = x;
						} while (x != v);
						block[numB++] = u;
						CheckBlock();
					}
				} else {
					low[u] = min(low[u], dfn[v]);
				}
			}
		}
	}

	void BCC() {
		depth = 0; size = 0;
		while (!S.empty()) S.pop();
		for (int i = 0; i < N; i++)
			dfn[i] = -1, vst[i] = false;
	    for (int i = 0; i < N; i++)
		    if (dfn[i] == -1) Tarjan(i);
		int ret = 0;
		for (int i = 0; i < N; i++)
			ret += vst[i];
		printf("%d\n", N - ret);
	}

}g;

/*
 Poj2942
 求点双连通分量+判奇圈
 邻接矩阵实现
 */
int main() {
	freopen("in.txt", "r", stdin);
	int n, m;
	while (~scanf("%d%d", &n, &m)) {
		if (n == 0 && m == 0) break;
		g.init(n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				g.adj[i][j] = i != j;
		while (m--) {
			int a, b;
			scanf("%d%d", &a, &b);
			a--; b--;
			g.adj[a][b] = g.adj[b][a] = 0;
		}
		g.BCC();
	}
	return 0;
}