import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.ServerSocketConnection;
import javax.microedition.io.SocketConnection;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;

public class Server implements CommandListener {
	private SocketMIDlet parent;

	private Display display;

	private Form f;

	private StringItem si;

	private TextField tf;

	private Command resultCommand = new Command("FormResult", Command.ITEM, 1);

	private Command exitCommand = new Command("Exit", Command.EXIT, 1);

	int sum;

	Sender sender;

	ServerSocketConnection scn;

	OutputStream[] osq = new OutputStream[15];

	private int port;

	String vt;

    boolean flag=false;

	public Server(SocketMIDlet m, int p, String vtname) {
		parent = m;
		port = p;
		vt = vtname;
		display = Display.getDisplay(parent);
		f = new Form("Socket Server");
		si = new StringItem("Status:", " ");
		//tf = new TextField("Send:", "", 30, TextField.ANY);

		//f.append(tf);
		f.append(si);
		f.addCommand(exitCommand);
		f.setCommandListener(this);
		display.setCurrent(f);
	}

	public void start() {
		final String portString = String.valueOf(port);
		si.setText("Waiting for connection on port " + portString);
		Runnable r = new Runnable() {
			public void run() {
				try {
					sum = 0;
					scn = (ServerSocketConnection) Connector.open("socket://:"
							+ portString);
					while (true) {
						SocketConnection sc = (SocketConnection) scn
								.acceptAndOpen();// 搜索到一个client设备
						// ////////////////////////////////////////
						// sender = new Sender(osq[sum]);
						sum++;
						showInfo("Connection" + sum + " accepted");
						//f.addCommand(resultCommand);
						new CreateServerThread(sc, sum); // 调用多线程
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		};
		new Thread(r).start();
	}

	public class CreateServerThread extends Thread {// 内部类，多线程
		private SocketConnection client;
		private InputStream is;
		private OutputStream os;
		private boolean stop;
		private int no;

		public CreateServerThread(SocketConnection s, int n) throws IOException {
			no = n;
			client = s;
			is = client.openInputStream();
			os = client.openOutputStream();
			osq[no - 1] = os;
			this.start();
		}

		public void run() {
			try {
				while (true) {
					StringBuffer sb = new StringBuffer();
					int c = 0;

					while (((c = is.read()) != '\n') && (c != -1)) {
						sb.append((char) c);
					}

					System.out.println("(S)c=" + c);
					if (c == -1) {
						break;
					}
					String sbstr = sb.toString();
					System.out.print("(S): " + sbstr);
					showInfo(" S:" + sbstr);

					if (sbstr.startsWith("getVoteType")) {
						//System.out.println("getVoteType requirement received");
						sender = new Sender(os);
						sender.send("VoteType:" + vt);
						continue;
					}
					if (sbstr.startsWith("VR")&&!flag) { //when receiving the voting data for the first time,add the "FormResult"command
						flag=true;
						f.addCommand(resultCommand);
					}
				}

			} catch (IOException ioe) {
				if (ioe.getMessage().equals("ServerSocket Open")) {
					Alert a = new Alert("Server", "Port 5000"
							+ " is already taken.", null, AlertType.ERROR);
					a.setTimeout(Alert.FOREVER);
					// a.setCommandListener(this);
					display.setCurrent(a);
				} else {
					if (!stop) {
						ioe.printStackTrace();
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		public void stop() {
			try {
				stop = true;

				if (is != null) {
					is.close();
				}

				if (os != null) {
					os.close();
				}

				if (client != null) {
					client.close();
				}

			} catch (IOException ioe) {
			}
		}
	}

	public void commandAction(Command c, Displayable s) {
		if ((c == resultCommand) && !parent.isPaused()) {
			Display.getDisplay(parent).setCurrent(new FormResult(vt, si.getText(), this,parent));
			
			/*for (int i = 0; i < sum; i++) {
				sender = new Sender(osq[i]);
				sender.send(tf.getString());
			}*/

		}
		if ((c == Alert.DISMISS_COMMAND) || (c == exitCommand)) {
			parent.notifyDestroyed();
			parent.destroyApp(true);
		}
	}

	public void stop() {
		try {
			if (scn != null) {
				scn.close();
			}
		} catch (IOException ioe) {
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

}
