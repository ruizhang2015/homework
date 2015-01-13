/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Administrator
 */

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;

import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.lcdui.*;

public class DemocracyReview extends Form implements CommandListener {
	Command exitCommand = new Command("Back", Command.BACK, 2);
	Command sureCommand = new Command("OK", Command.OK, 1);
	ChoiceGroup cg;
	String[] str = { "very good", "good", "ordinary", "bad", "very bad" };
	SocketMIDlet midlet;
	VoteTypeChoice vtc;
	Sender sender;
	InputStream is;

	public DemocracyReview(SocketMIDlet midlet, VoteTypeChoice vtc,
			Sender sender, InputStream is) {
		super("Democracy Review");
        this.is=is;
		this.sender = sender;
		this.vtc = vtc;
		this.midlet = midlet;
		Image[] img = new Image[5];
		img[0] = null;
		img[1] = null;
		img[2] = null;
		img[3] = null;
		img[4] = null;
		cg = new ChoiceGroup("1st vote", Choice.EXCLUSIVE, str, img);

		this.append(cg);
		this.addCommand(exitCommand);
		this.addCommand(sureCommand);
		this.setCommandListener(this);
		// TODO Auto-generated constructor stub

	}

	public void commandAction(Command com, Displayable arg1) {
		System.out.println("in commandAction head");
		if (com.getCommandType() == Command.OK) {
			System.out.println(str[cg.getSelectedIndex()]);
			sender.send("VR"+cg.getSelectedIndex()+str[cg.getSelectedIndex()]);
			ResultDisplay rd = new ResultDisplay("Democracy Review", midlet, is);
			Display.getDisplay(midlet).setCurrent(rd);
			new Thread(rd).start();
		}

		if (com.getCommandType() == Command.BACK) {
			System.out.println("Command.back in VoteTypeChoice");
			Display.getDisplay(midlet).setCurrent(vtc);
		}
	}

}
