import java.io.IOException;
import java.io.InputStream;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;


public class ResultDisplay extends Form implements CommandListener,Runnable{
	private SocketMIDlet parent;
	private StringItem si;
	InputStream is;
	boolean flag=false;

	private Command okCommand = new Command("OK", Command.OK, 1);

	//private Command exitCommand = new Command("Exit", Command.EXIT, 1);

	public ResultDisplay(String vt, SocketMIDlet m, InputStream is)  {
		super("Voting Rusults");
		
		this.is=is;
		parent = m;
		
		
		si = new StringItem("Voting Successfully!!\nWaiting for Voting Result ...", " ");
		this.append(si);
		
		this.setCommandListener(this);
		//System.out.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2");
		

	}
	
	public void commandAction(Command c, Displayable arg1) {
		if (c == okCommand) {
			parent.destroyApp(true);
			parent.notifyDestroyed();
		}
		
	}
	void showInfo(String s) {
		StringBuffer sb = new StringBuffer(si.getText());
		if (sb.length() > 0) {
			sb.append("\n");
		}
		sb.append(s);
		si.setText(sb.toString());
	}

	public void run() {
		while (true) {
			StringBuffer sb = new StringBuffer();
			int c = 0;

			try {
				while (((c = is.read()) != '\n') && (c != -1)) {
				    sb.append((char) c);
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			if (c == -1) {
			    break;
			}
			if (sb.toString().startsWith("[")&& !flag) { 
				flag=true;
				this.addCommand(okCommand);
				this.setCommandListener(this);

			}
			showInfo(sb.toString());
		    }
		
	}

}
