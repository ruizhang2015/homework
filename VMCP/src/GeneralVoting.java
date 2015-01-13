
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.SocketConnection;
import javax.microedition.io.StreamConnection;
import javax.microedition.lcdui.*;

public class GeneralVoting extends Form implements CommandListener {
	Command exitCommand = new Command("Back", Command.BACK, 2);
	Command sureCommand = new Command("OK", Command.OK, 1);
	ChoiceGroup cg;
	SocketMIDlet midlet;
	VoteTypeChoice vtc;
	String[] str = { "Agree", "Opposite", "Abandon" };
	Sender sender;
	InputStream is;

	// OutputStream os;

	public GeneralVoting(SocketMIDlet midlet, VoteTypeChoice vtc, Sender sender, InputStream is) {
		super("GeneralVoting");

		System.out.println("general voting start");

		Image[] img = new Image[3];
		img[0] = null;
		img[1] = null;
		img[2] = null;
		cg = new ChoiceGroup("1st vote", Choice.EXCLUSIVE, str, img);
		// this.os = os;
		this.sender = sender;
		/*
		 * try { os = sc.openOutputStream(); } catch (IOException ioe) {
		 * ioe.printStackTrace(); }
		 */

		// sender = new Sender(os);
        this.is=is;
		this.midlet = midlet;
		this.vtc = vtc;
		this.append(cg);
		this.addCommand(exitCommand);
		this.addCommand(sureCommand);
		this.setCommandListener(this);
		// TODO Auto-generated constructor stub

	}

	public void commandAction(Command arg0, Displayable arg1) {
		// System.out.println("in commandAction head");
		// System.out.println((arg0.getCommandType() == Command.BACK)?1:0);
		if (arg0.getCommandType() == Command.OK) {
			System.out.println(str[cg.getSelectedIndex()]);
			sender.send("VR"+str[cg.getSelectedIndex()]);

				//System.out.println("Error in Create!!");
				ResultDisplay rd = new ResultDisplay("General Voting", midlet, is);
				Display.getDisplay(midlet).setCurrent(rd);
				new Thread(rd).start();
	
		}
		if (arg0.getCommandType() == Command.BACK) {
			// System.out.println("Command.back in Digital Rating");
			Display.getDisplay(midlet).setCurrent(vtc);
		}
	}

}
