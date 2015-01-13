import java.io.FileReader;
import java.io.IOException;

public class PreProcessing {

	static int MAX_SIZE = 5000000;
	int[][] count = new int[4][50000]; // b,e,m,s

	public void textSplit(String fname) {
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
		String[] sentences = data.split("[^a-zA-Z0-9\u4e00-\u9fa5\u25cb]");
		for (String sentence : sentences) {
			// System.out.println(sentence);
			int i = 0;
			for (char c : sentence.toCharArray()) {
				if (sentence.length() == 1) {
					count[3][c]++;
					// System.out.print(c);
				} else if (i == 0)
					count[0][c]++;
				else if (i == sentence.length() - 1)
					count[1][c]++;
				else
					count[2][c]++;
				i++;
			}
		}
		//System.out.println((char) 20108);
		System.out.println("textSplit Done!");
	}

	public static void main(String[] args) {

		PreProcessing pp = new PreProcessing();
		pp.textSplit("test.txt");
	}

}
