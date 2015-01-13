import java.util.Arrays;

public class BaumWelch {

	/**
	 * @param args
	 */
	static double DELTA = 0.001;

	public static void baumWelch(wordHMM phmm, String[] sentences,
			int[] pniter, double[] plogprobinit, double[] plogprobfinal) {

		int T;
		int[] O;
		double[][] alpha, beta;
		double[][][] gammas = new double[sentences.length][][];

		int i, j, k;
		int t, l = 0;

		double[] logprobf = new double[1];
		double[] logprobb = new double[1];

		double[][][] xi;

		double delta, logprobprev = 0.;

		do {

			int s = 0;
			double[][] numeratorA = new double[phmm.N][phmm.N];
			double[] denominatorA = new double[phmm.N];
			double[][] numeratorB = new double[phmm.N][phmm.M];
			double[] denominatorB = new double[phmm.N];
			/* ���¹��� t=1 ʱ��״̬Ϊi ��Ƶ�� */
			for (String sentence : sentences) {
				// System.out.println("NaN! " + s);
				O = Utilities.generateO(sentence);
				T = O.length;
				if (T == 0) {
					continue;
				}
				alpha = ForBackward.ForwardWithScale(phmm, T, O, logprobf);
				plogprobinit[0] = logprobf[0];
				beta = ForBackward.BackwardWithScale(phmm, T, O, logprobb);

				xi = ComputeXi(phmm, T, O, alpha, beta);
				gammas[s] = ComputeGamma(phmm, T, alpha, beta);
				if (logprobprev < logprobf[0])
					logprobprev = logprobf[0];

				/* ���¹���ת�ƾ���͹۲���� */
				for (i = 0; i < phmm.N; i++) {
					for (t = 0; t < T - 1; t++)
						denominatorA[i] += gammas[s][t][i];
					if (Double.isNaN(denominatorA[i])) {
						System.out.println(s);
						while (true)
							;
					}
					for (j = 0; j < phmm.N; j++) {
						for (t = 0; t < T - 1; t++)
							numeratorA[i][j] += xi[t][i][j];
					}

					denominatorB[i] = denominatorA[i] + gammas[s][T - 1][i];
					for (k = 0; k < phmm.M; k++) {
						for (t = 0; t < T; t++) {
							if (O[t] == k)
								numeratorB[i][k] += gammas[s][t][i];
						}
					}
				}
				s++;
				// System.out.println(s);
			}// end for

			for (i = 0; i < phmm.N; i++) {
				if (denominatorA[i] != 0)
					for (j = 0; j < phmm.N; j++)
						phmm.A[i][j] = numeratorA[i][j] / denominatorA[i];
				if (denominatorB[i] != 0)
					for (k = 0; k < phmm.M; k++)
						phmm.B[i][k] = numeratorB[i][k] / denominatorB[i];
			}
			Arrays.fill(phmm.pi, 0.);
			for (i = 0; i < phmm.N; i++) {
				for (j = 0; j < s; j++)
					phmm.pi[i] += gammas[j][0][i];
				phmm.pi[i] /= s;
			}

			delta = logprobf[0] - logprobprev;
			logprobprev = logprobf[0];
			l++;
		} while (delta > DELTA); /* ����Ĳ�̫�󣬱����������˳� */

		pniter[0] = l;
		plogprobfinal[0] = logprobf[0]; /* log P(O|estimated model) */

	}

	static double[][] ComputeGamma(wordHMM phmm, int T, double[][] alpha,
			double[][] beta) {

		int i, j;
		int t;
		double denominator;
		double[][] gamma = new double[T][phmm.N];

		for (t = 0; t < T; t++) {
			denominator = 0.0;
			for (j = 0; j < phmm.N; j++) {
				gamma[t][j] = alpha[t][j] * beta[t][j];
				denominator += gamma[t][j];
			}
			if (denominator != 0)
				for (i = 0; i < phmm.N; i++)
					gamma[t][i] = gamma[t][i] / denominator;
		}
		return gamma;
	}

	static double[][][] ComputeXi(wordHMM phmm, int T, int[] O,
			double[][] alpha, double[][] beta) {
		int i, j;
		int t;
		double sum;
		double[][][] xi = new double[T][phmm.N][phmm.N];

		for (t = 0; t <= T - 2; t++) {
			sum = 0.0;
			for (i = 0; i < phmm.N; i++)
				for (j = 0; j < phmm.N; j++) {
					xi[t][i][j] = alpha[t][i] * beta[t + 1][j] * (phmm.A[i][j])
							* (phmm.B[j][O[t + 1]]);
					//
					// System.out.println("NaN! " + t + " " + xi[t][i][j]);
					if (Double.isNaN(xi[t][i][j])) {
						System.out.println("NaN! " + alpha[t][i] + " "
								+ beta[t + 1][j] + " " + phmm.A[i][j] + " "
								+ phmm.B[j][O[t + 1]]);
						while (true)
							;
					}
					sum += xi[t][i][j];
				}

			if (sum != 0)
				for (i = 0; i < phmm.N; i++)
					for (j = 0; j < phmm.N; j++)
						xi[t][i][j] /= sum;
		}
		return xi;
	}

}
