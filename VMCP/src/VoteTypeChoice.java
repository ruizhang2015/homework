
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;
import javax.microedition.lcdui.TextField;
import javax.microedition.midlet.MIDletStateChangeException;

import java.io.*;

public class VoteTypeChoice extends Form implements CommandListener {

	//static int n = 2;
	Command sureCommand = new Command("OK", Command.OK, 4);
	Command exitCommand = new Command("Back", Command.BACK, 2);

	StringItem getVTC;

	SocketMIDlet midlet;
	Sender sender;
	InputStream is;
	String vt = "none";

	//ChoiceGroup votetypechoice;
	String[] votetype = { "General Voting", "Digital Rating",
			"Select and Vote", "Democracy Review" };

	public VoteTypeChoice(SocketMIDlet midlet, Sender sender, InputStream is,
			String vt1) {
		super("VoteType");
		this.midlet = midlet;
		this.sender = sender;
		this.is = is;
		this.vt = vt1;
		getVTC = new StringItem(vt,"");

		//sender.send("(VTC)VTC Start");
		//StringBuffer sb = new StringBuffer();

		//System.out.println("(VTC)Get VoteType");
		//System.out.println("(VTC)vt1=" + vt1);

		// getVTC.setText("get VTC");
		// int c = 0;

		/*
		 * try { while (((c = this.is.read()) != '\n') && (c != -1)) {
		 * sb.append((char) c); } } catch (IOException ioe) {
		 * ioe.printStackTrace(); }
		 * 
		 * String sbstr = sb.toString(); System.out.println("(VTC)sb.toString="
		 * + sb.toString()); if (sbstr.startsWith("VoteType:")) {
		 * System.out.println("VoteType requirement received"); }
		 */

		// sender = new Sender(osq[sum]);
		// sender.send(tf.getString()); // break; }

		// Image[] img = new Image[4];
		// img[0] = null;
		// img[1] = null;
		// img[2] = null;
		// img[3] = null;
		//Image[] img = new Image[1];
		//img[0] = null;
		//String[] temp = { "Vote Type received" };
		//votetypechoice = new ChoiceGroup("Vote Type", ChoiceGroup.EXCLUSIVE,
				//temp, img);
		//this.append(votetypechoice);
		this.append(getVTC);
		// getVTC.setText("get VTC");
		this.addCommand(exitCommand);
		this.addCommand(sureCommand);
		this.setCommandListener(this);
	}


	public void commandAction(Command com, Displayable disp) {
		if (com.getCommandType() == Command.OK) {

			sender.send("ok in VTC");
			System.out.print("(VTC)vt=" + vt);
			if (vt.startsWith(votetype[0])) {
				GeneralVoting gv = new GeneralVoting(midlet, this, sender, is);
				Display.getDisplay(midlet).setCurrent(gv);
			}
			if (vt.startsWith(votetype[1])) {
				DigitalRating dr = new DigitalRating(midlet, this, sender, is);
				Display.getDisplay(midlet).setCurrent(dr);
			}
			if (vt.startsWith(votetype[2])) {
				VotingandSelecting vs = new VotingandSelecting(midlet, this,
						sender, is);
				Display.getDisplay(midlet).setCurrent(vs);
			}
			if (vt.startsWith(votetype[3])) {
				DemocracyReview dr = new DemocracyReview(midlet, this, sender, is);
				Display.getDisplay(midlet).setCurrent(dr);
			}
		}

		if (com.getCommandType() == Command.BACK) {
			midlet.showMainMenu();
		}
	}
}