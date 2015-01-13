
import javax.microedition.io.ConnectionNotFoundException;
import javax.microedition.io.Connector;
import javax.microedition.io.SocketConnection;
import javax.microedition.lcdui.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Client implements Runnable, CommandListener {
	private SocketMIDlet parent;

	private Display display;

	private Form f;

	private StringItem si;

	// private TextField tf;

	private boolean stop;

	private Command sendCommand = new Command("Get VoteType", Command.OK, 1);

	private Command exitCommand = new Command("Cancel", Command.CANCEL, 2);

	InputStream is;

	OutputStream os;

	SocketConnection sc;

	Sender sender;

	SocketMIDlet midlet;

	private int port;

	String vt;

	boolean whiletoend = false;

	// StringBuffer sb;

	public Client(SocketMIDlet m, int p) {
		parent = m;
		midlet = m;
		port = p;
		display = Display.getDisplay(parent);
		f = new Form("Socket Client");
		si = new StringItem("Status:", " ");
		// tf = new TextField("Send:", "", 30, TextField.ANY);
		f.append(si);
		// f.append(tf);
		f.addCommand(exitCommand);
		f.addCommand(sendCommand);
		f.setCommandListener(this);
		display.setCurrent(f);
	}

	/**
	 * Start the client thread
	 */
	public void start() {
		Thread t = new Thread(this);
		t.start();
	}

	public void run() {
		String portString = String.valueOf(port);
		try {
			sc = (SocketConnection) Connector.open("socket://localhost:"
					+ portString);
			si.setText("Connected to server on port " + portString);
			is = sc.openInputStream();
			os = sc.openOutputStream();

			// Start the thread for sending messages - see Sender's main
			// comment for explanation
			sender = new Sender(os);

			// Loop forever, receiving data
			while (true) {
				StringBuffer sb = new StringBuffer();
				int c;

				while (((c = is.read()) != '\n') && (c != -1)) {
					sb.append((char) c);
				}

				System.out.println("Client c=" + c);

				if (c == -1) {
					break;
				}

				// Display message to user
				String sbstr = sb.toString();
				
				System.out.println("Client Message received - " + sbstr);
				if (sb.toString().startsWith("VoteType:"))
					si.setText(sbstr);
					vt = sbstr.substring(9);
				break;
			}
			System.out.print("(Client)votetype:" + vt);
			sender.send("votetype send");
			VoteTypeChoice vtc = new VoteTypeChoice(midlet, sender, is, vt);
			Display.getDisplay(midlet).setCurrent(vtc);

			// stop();
			// si.setText("Connection closed");
			// f.removeCommand(sendCommand);
		} catch (ConnectionNotFoundException cnfe) {
			Alert a = new Alert("Client",
					"Please run Server MIDlet first on port " + portString,
					null, AlertType.ERROR);
			a.setTimeout(Alert.FOREVER);
			a.setCommandListener(this);
			display.setCurrent(a);
		} catch (IOException ioe) {
			if (!stop) {
				ioe.printStackTrace();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void commandAction(Command c, Displayable s) {
		if ((c == sendCommand) && !parent.isPaused()) {
			sender.send("getVoteType");
			// whiletoend = true;
			// VoteTypeChoice vtc = new VoteTypeChoice(midlet, sender, is, vt);
			// Display.getDisplay(midlet).setCurrent(vtc);
		}

		if ((c == Alert.DISMISS_COMMAND) || (c == exitCommand)) {
			parent.notifyDestroyed();
			parent.destroyApp(true);
		}
	}

	/**
	 * Close all open streams
	 */
	public void stop() {
		try {
			stop = true;

			if (sender != null) {
				sender.stop();
			}

			if (is != null) {
				is.close();
			}

			if (os != null) {
				os.close();
			}

			if (sc != null) {
				sc.close();
			}
		} catch (IOException ioe) {
		}
	}
}
