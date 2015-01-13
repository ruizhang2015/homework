import java.io.FileOutputStream;
import java.io.IOException;

public class wordRecognization {

	public static String getResult(String text, String[] sentences, int[] re) {

		String result = "";
		int ti = 0;
		int rei = 0;
		String sentence;
		for (int i = 0; i < sentences.length; i++) {
			sentence = sentences[i];
			for (char c : sentence.toCharArray()) {
				result += c;
				// System.out.print(" re "+re[rei]);
				if (re[rei] == 1 || re[rei] == 3) {
					result += "  ";
				}
				rei++;
			}
			ti += sentence.length();
			if (result.charAt(result.length() - 1) != ' '
					&& result.charAt(result.length() - 1) != '—')
				result += "  ";
			if (ti >= text.length())
				break;
			result += text.charAt(ti);
			if (i + 1 < sentences.length)
				if (sentences[i + 1].length() != 0)
					result += "  ";

			ti++;
		}
		try {
			FileOutputStream fw = new FileOutputStream("result.txt");
			fw.write(result.getBytes());
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		return result;
	}

}
