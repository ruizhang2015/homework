public class Vterbi {
	/**
	 * hmm model paremeter int n, m;// n��״̬��m���ַ�Ĵ�С double a[][];// ״̬ת�Ƹ���
	 * double b[][];// ������� double p[];// ��ʼ״̬���� double det[][];//
	 * dp�м���� int log[][];// �������¼����
	 */

	public static void vterbi(wordHMM whmm, int T, int[] O, int[] re, int rei) {
		/*
		 * ��δ���Ǿ�������
		 */
		int n = whmm.N;
		double det[][] = new double[T][n];
		int log[][] = new int[T][n];

		// 1. ��ʼ��
		for (int i = 0; i < n; i++) {
			det[0][i] = whmm.pi[i] * (whmm.B[i][O[0]]);
			log[0][i] = 0;
		}
		for (int i = 1; i < T; i++) {// ʱ��
			for (int j = 0; j < n; j++) {// ��ǰ״̬
				det[i][j] = 0;
				for (int k = 0; k < n; k++) {// ǰһ��ʱ���״̬
					double tmp = det[i][j];
					det[i][j] = Math.max(det[i][j], det[i - 1][k]
							* whmm.A[k][j]);
					if (tmp != det[i][j]) {
						log[i][j] = k;// ��¼�м�״̬
					}
				}
				det[i][j] *= whmm.B[j][O[i]];
			}
		}
		// int re[] = new int[t];
		double now = 0;
		for (int i = 0; i < n; i++) {
			if (det[T - 1][i] > now) {
				now = det[T - 1][i];
				re[rei + T - 1] = i;
			}
		}
		for (int i = T - 2; i >= 0; i--) {
			re[rei + i] = log[i + 1][re[rei + i + 1]];
		}
	}

}
