/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Administrator
 */

import java.io.InputStream;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.MIDletStateChangeException;

public class DigitalRating extends Form implements CommandListener {

	static int n = 2;
	Command sureCommand = new Command("OK", Command.OK, 4);
	Command exitCommand = new Command("Back", Command.BACK, 2);
	TextField tf;
	TextField one;
	TextField two;
	SocketMIDlet midlet;
	VoteTypeChoice vtc;
	Sender sender;
	InputStream is;

	public DigitalRating(SocketMIDlet midlet, VoteTypeChoice vtc, Sender sender, InputStream is) {
		super("DigitalRating");
        this.is=is;
		this.sender = sender;
		this.vtc = vtc;
		this.midlet = midlet;
		tf = new TextField("1st candidate ", "", 3, TextField.NUMERIC);
		// tf[1] = new TextField("2∫≈—° ÷   ", "", 3, TextField.NUMERIC);
		this.append(tf);
		// this.append(tf[1]);
		this.addCommand(exitCommand);
		this.addCommand(sureCommand);
		this.setCommandListener(this);
	}

	protected void startApp() throws MIDletStateChangeException {
		// System.out.println("startApp in DigitalRating");
	}

	protected void pauseApp() {
	}

	protected void destroyApp() throws MIDletStateChangeException {
		// System.out.println("destroyApp in DigitalRating");
	}

	public void commandAction(Command com, Displayable disp) {
		if (com.getCommandType() == Command.OK) {
			/*
			 * String[] str = new String[n]; for (int i = 0; i < n; i++) {
			 * str[i] = (tf[i].getString()); System.out.println(str[i]); }
			 */
			String str = new String("");
			str = tf.getString();
			System.out.println(str);
			sender.send("VR"+str);
			// System.out.println("midlet.showMainMenu()");
			// midlet = new SocketMIDlet();
			// Display.getDisplay(midlet).setCurrent(midlet.list);
			// midlet.showMainMenu();
			ResultDisplay rd = new ResultDisplay("Digital Rating", midlet, is);
			Display.getDisplay(midlet).setCurrent(rd);
			new Thread(rd).start();
		}

		if (com.getCommandType() == Command.BACK) {
			// System.out.println("Command.back in Digital Rating");
			Display.getDisplay(midlet).setCurrent(vtc);
		}
	}
}