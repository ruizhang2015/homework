import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;

public class wordHMM {

	static char[] S = { 'b', 'e', 'm', 's' };
	static int MAX_SIZE = 10000000;
	int N = 4; /* Q={b, e, m, s} */
	int M = 21000; /* V={1,2,...,M} */
	double[][] A = { { 0.0, 0.99, 0.01, 0.0 }, { 0.7, 0.0, 0.0, 0.3 },
			{ 0.0, 0.99, 0.01, 0.0 }, { 0.8, 0.0, 0.0, 0.2 } };
	double[][] B = new double[N][M];
	double[] pi = { 0.95, 0.0, 0.0, 0.05 };

	public wordHMM() {

		PreProcessing pp = new PreProcessing();
		pp.textSplit("pku_training.txt");
		double[] sum = new double[4];
		for (int j = 0; j < 4; j++) {
			sum[j] = pp.count[j][9675];// ling
			for (int i = 19968; i < 40869; i++)
				// \u4e00-\u9fa5
				sum[j] += pp.count[j][i];
		}
		for (int j = 0; j < 4; j++) {
			B[j][0] = pp.count[j][9675] / sum[j];
			for (int i = 19968; i < 40869; i++)
				B[j][i - 19967] = pp.count[j][i] / sum[j];
		}

		System.out.println("wordHMM Done!");
	}

	public void printHMM(String fname) {

		int i, j;
		String result = new String("");
		result += this.N + " " + M + " ";
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
				result += A[i][j] + " ";
		result += "\n";
		for (i = 0; i < N; i++)
			for (j = 0; j < M; j++)
				result += B[i][j] + " ";
		result += "\n";
		for (i = 0; i < N; i++)
			result += pi[i] + " ";
		result += "\n";
		try {
			FileOutputStream fw = new FileOutputStream(fname);
			fw.write(result.getBytes());
			fw.close();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		System.out.println("print Done!");
	}

	int[] generateO(String text) {
		int[] O = new int[text.length()];
		int index;
		for (int i = 0; i < O.length; i++) {
			index = text.charAt(i) - 19967;
			if (index < 0)
				index = 0;
			O[i] = index;
		}
		return O;
	}

	public static void main(String[] args) {

		String data;
		String[] sentences;
		int[] O;
		int T;
		int[] pniter;
		double[] plogprobinit, plogprobfinal;
		wordHMM whmm = new wordHMM();

		// /////////////////////////////////////////////////////// training
		char[] cbuf = new char[MAX_SIZE];
		int hasRead = 0;
		try {
			FileReader fr = new FileReader("test_training.txt");
			hasRead = fr.read(cbuf);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		data = new String(cbuf, 0, hasRead);
		sentences = data.split("[^a-zA-Z0-9\u4e00-\u9fa5\u25cb]");
		pniter = new int[1];
		plogprobinit = new double[1];
		plogprobfinal = new double[1];
		BaumWelch.baumWelch(whmm, sentences, pniter, plogprobinit,
				plogprobfinal);

		System.out.println("BW Done!");

		// /////////////////////////////////////////////////////// segmentation
		hasRead = 0;
		try {
			FileReader fr = new FileReader("test.txt");
			hasRead = fr.read(cbuf);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		data = new String(cbuf, 0, hasRead);
		sentences = data.split("[^a-zA-Z0-9\u4e00-\u9fa5\u25cb]");
		String text = data.replaceAll("[^a-zA-Z0-9\u4e00-\u9fa5\u25cb]", "");
		int[] re = new int[text.length()];
		int rei = 0;
		for (String sentence : sentences) {
			if (sentence.length() == 0)
				continue;
			O = whmm.generateO(sentence);
			T = O.length;
			Vterbi.vterbi(whmm, T, O, re, rei);
			rei += T;
		}

		// /////////////////////////////////////////////////////// writefile
		String result = wordRecognization.getResult(data, sentences, re);
		// System.out.println(result);
	}
}
