
import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDlet;

public class SocketMIDlet extends MIDlet implements CommandListener {
	private static final int DEFAULT_PORT = 5000;

	private static final String SERVER = "Server";

	private static final String CLIENT = "Client";

	private static final String[] names = { SERVER, CLIENT };

	private ChoiceGroup cg;

	private ChoiceGroup vtc;

	private boolean isPaused;

	private TextField portField;

	private Server server;

	private Client client;

	private Command exitCommand = new Command("Exit", Command.EXIT, 3);

	private Command startCommand = new Command("Start", Command.OK, 2);

	private Form f = new Form("Socket Demo");

	private Display display = Display.getDisplay(this);

	String[] votetype = { "General Voting", "Digital Rating",
			"Select and Vote", "Democracy Review" };

	public SocketMIDlet() {
		cg = new ChoiceGroup("Please select peer", Choice.EXCLUSIVE, names,
				null);
		f.append(cg);
		vtc = new ChoiceGroup("Select vote type(If you are server)",
				Choice.EXCLUSIVE, votetype, null);
		f.append(vtc);
		portField = new TextField("Port number:", String.valueOf(DEFAULT_PORT),
				6, TextField.NUMERIC);
		f.append(portField);
		f.addCommand(exitCommand);
		f.addCommand(startCommand);
		f.setCommandListener(this);

		display.setCurrent(f);
	}

	public void showMainMenu() {
		display.setCurrent(f);
	}

	public boolean isPaused() {
		return isPaused;
	}

	public void startApp() {
		isPaused = false;
	}

	public void pauseApp() {
		isPaused = true;
	}

	public void destroyApp(boolean unconditional) {
		if (server != null) {
			server.stop();
		}

		if (client != null) {
			client.stop();
		}
	}

	public void commandAction(Command c, Displayable s) {
		if (c == exitCommand) {
			destroyApp(true);
			notifyDestroyed();
		} else if (c == startCommand) {
			String name = cg.getString(cg.getSelectedIndex());
			int port = Integer.parseInt(portField.getString());
			String vtname = vtc.getString(vtc.getSelectedIndex());

			if (name.equals(SERVER)) {
				server = new Server(this, port, vtname);
				server.start();
			} else {
				// System.out.println("client go");
				client = new Client(this, port);
				// display.setCurrent(client);
				client.start();
			}
		}
	}
}
