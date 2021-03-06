public class SmallBricks31 {

	int mod = 1000000007;
	int w, h, up_sta;
	int[] mask = new int[] { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048,
			4096 };
	int[][] ways;
	int[][] dp;

	public int countWays(int w, int h) {
		this.w = w;
		this.h = h;
		up_sta = 1 << w;
		ways = new int[up_sta][up_sta];
		dp = new int[h + 1][up_sta];
		for (int i = 1; i < up_sta; i++)
			dfs(0, i, 0);
		// print();
		dp[0][up_sta - 1] = 1;
		int ret = 0;
		for (int i = 0, s, t; i < h; i++)
			for (s = 1; s < up_sta; s++)
				if (dp[i][s] >= 0) {
					for (t = 0; t < up_sta; t++) {
						dp[i + 1][t] += (long) dp[i][s] * ways[s][t] % mod;
						if (dp[i + 1][t] >= mod)
							dp[i + 1][t] -= mod;
					}
					ret += dp[i][s];
					if (ret >= mod)
						ret += mod;
				}
		for (int s = 1; s < up_sta; s++) {
			ret += dp[h][s];
			if (ret >= mod)
				ret -= mod;
		}
		return ret;
	}

	private void dfs(int r, int f, int t) {
		if (r == w) {
			ways[f][t]++;
			return;
		}
		int p1 = f & mask[r], p2 = f & mask[r + 1], p3 = f & mask[r + 2];
		dfs(r + 1, f, t);
		if (p1 > 0)
			dfs(r + 1, f, t | mask[r]);
		if (p1 > 0 && p2 > 0)
			dfs(r + 2, f, t | mask[r] | mask[r + 1]);
		if (p1 > 0 && p3 > 0)
			dfs(r + 3, f, t | mask[r] | mask[r + 1] | mask[r + 2]);
	}
}

