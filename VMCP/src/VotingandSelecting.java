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

public class VotingandSelecting extends Form implements CommandListener {
	Command exitCommand = new Command("Back", Command.BACK, 2);
	Command sureCommand = new Command("OK", Command.OK, 1);
	ChoiceGroup cg;
	SocketMIDlet midlet;
	VoteTypeChoice vtc;
	String[] str = { "1", "2", "3", "4", "5", "6", "7", "8" };
	Sender sender;
	InputStream is;

	public VotingandSelecting(SocketMIDlet midlet, VoteTypeChoice vtc,
			Sender sender, InputStream is) {
		super("VotingandSelecting");

		Image[] img = new Image[8];
		img[0] = null;
		img[1] = null;
		img[2] = null;
		img[3] = null;
		img[4] = null;
		img[5] = null;
		img[6] = null;
		img[7] = null;
		cg = new ChoiceGroup("1st vote", Choice.EXCLUSIVE, str, img);
        this.is=is;
		this.sender = sender;
		this.midlet = midlet;
		this.vtc = vtc;
		this.append(cg);
		this.addCommand(exitCommand);
		this.addCommand(sureCommand);
		this.setCommandListener(this);
		// TODO Auto-generated constructor stub

	}

	public void commandAction(Command arg0, Displayable arg1) {
		if (arg0.getCommandType() == Command.OK) {
			System.out.println(str[cg.getSelectedIndex()]);
			sender.send("VR"+str[cg.getSelectedIndex()]);
			Display.getDisplay(midlet).setCurrent(vtc);
		}
		if (arg0.getCommandType() == Command.BACK) {
			Display.getDisplay(midlet).setCurrent(vtc);
		}
	}
}
