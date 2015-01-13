import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public class Utilities {

	static int MAX_SIZE = 50000;
	
	public static int[] generateO(String text) {
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

	public static Map<Character, Integer> wordtableProcessing(String fname) {

		char[] cbuf = new char[MAX_SIZE];
		int hasRead = 0;
		try {
			FileReader fr = new FileReader(fname);
			hasRead = fr.read(cbuf);
			fr.close();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		String data = new String(cbuf, 0, hasRead);
		String[] Hanzi = data.split(" ");
		//System.out.println(Hanzi.length);

		Map<Character, Integer> hmap = new HashMap<Character, Integer>();
		for (int i = 0; i < Hanzi.length; i++) {
			hmap.put(Hanzi[i].charAt(0), i);
		}
		//System.out.println("utilities.word... "+Hanzi[3].charAt(0));
		return hmap;
	}

	public static void randomP(double[] p, int l, int h) { // ��ͷ����β
		Random rand = new Random();
		double sum = 0.0;
		for (int i = l; i < h; i++) {
			p[i] = (double) rand.nextInt(100);
			sum += p[i];
		}
		double cursum = 0.0;
		for (int i = l; i < h - 1; i++) {
			p[i] /= sum;
			cursum += p[i];
		}
		p[h - 1] = 1 - cursum;
	}

	public static void main(String[] args) {

		double[] test = new double[4];
		randomP(test, 0, 3);
		for (int i = 0; i < test.length; i++)
			System.out.println(test[i]);
	}

}
