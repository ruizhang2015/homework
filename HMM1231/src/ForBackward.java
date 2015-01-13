import java.lang.Math;

public class ForBackward {

	public static double[][] ForwardWithScale(wordHMM phmm, int T, int[] O,
			double[] logre) {

		double[][] alpha = new double[T][phmm.N];
		for (int i = 0; i < phmm.N; i++) {
			alpha[0][i] = phmm.pi[i] * phmm.B[i][O[0]];
		}
		for (int i = 1; i < T; i++) {
			for (int j = 0; j < phmm.N; j++) {
				alpha[i][j] = 0;
				for (int k = 0; k < phmm.N; k++) {
					alpha[i][j] += alpha[i - 1][k] * phmm.A[k][j];
				}
				alpha[i][j] *= phmm.B[j][O[i]];
			}
		}

		double re = 0.0;
		for (int i = 0; i < phmm.N; i++) {
			re += alpha[T - 1][i];
		}
		logre[0] = Math.log(re) / Math.log((double) 2);
		return alpha;
	};

	public static double[][] BackwardWithScale(wordHMM phmm, int T, int[] O,
			double[] logre) {
		double[][] beta = new double[T][phmm.N];
		int i, j; /* ״ָ̬ʾ */
		int t; /* ʱ���±� */
		double sum;

		/* 1. ��ʼ�� */
		for (i = 0; i < phmm.N; i++)
			beta[T - 1][i] = 1.0;

		/* 2. �ݹ� */
		for (t = T - 2; t >= 0; t--) {
			for (i = 0; i < phmm.N; i++) {
				sum = 0.0;
				for (j = 0; j < phmm.N; j++)
					sum += phmm.A[i][j] * (phmm.B[j][O[t + 1]])
							* beta[t + 1][j];
				beta[t][i] = sum;
			}
		}

		double re = 0.0;
		for (i = 0; i < phmm.N; i++)
			re += beta[0][i] * phmm.B[i][O[0]] * phmm.pi[i];

		logre[0] = Math.log(re) / Math.log((double) 2);
		return beta;
	};

}
