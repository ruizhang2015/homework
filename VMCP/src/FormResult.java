import java.io.OutputStream;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.StringItem;



public class FormResult extends Form implements CommandListener{

	private SocketMIDlet parent;
	String vtype;
	String vdata;
	Server server;
	private StringItem si;


	private Command okCommand = new Command("Display", Command.OK, 1);

	private Command exitCommand = new Command("Exit", Command.EXIT, 1);

	FormResult(String vt, String vd, Server server, SocketMIDlet m) {
		super("Voting Result");
		//System.out.println("Result forming...");
		parent = m;
		vtype = vt;
		vdata = vd;
		this.server = server;
		si = new StringItem(vtype+" Result forming...\n", "");
		this.append(si);
		this.addCommand(okCommand);
		this.addCommand(exitCommand);
		this.setCommandListener(this);
		
		this.Calculate();
	}
	
	public void Calculate(){
		//System.out.println(vtype+" Result Calculating...");
		
		int i=0;
		if(vtype.equals("General Voting")){
			//server.showInfo(vtype+" Result Calculating...");
			int ag=0;
			int op=0;
			int ab=0;
			while(i<vdata.length()){
				if(vdata.charAt(i)=='V' && vdata.charAt(i+1)=='R'){
					if(vdata.charAt(i+2)=='A' && vdata.charAt(i+3)=='g') {ag+=1;i+=7;}
					else{
						if(vdata.charAt(i+2)=='A' && vdata.charAt(i+3)=='b'){ab+=1;i+=9;}
						else {op+=1;i+=10;}						
					}
				}
				else i++;
			}
			showInfo("[Gerneral Voting Result]");
			showInfo("    Agree    : "+ag);
			showInfo("    Opposite : "+op);
			showInfo("    Abandon  : "+ab);
			return;
		}
		if(vtype.equals("Digital Rating")){
			int sum=0;
			int n=0;
			String sco;
			while(i<vdata.length()){
				if(vdata.charAt(i)=='V' && vdata.charAt(i+1)=='R'){
					if(vdata.charAt(i+3)<='9' && vdata.charAt(i+3)>='0'){sco = vdata.substring(i+2, i+4);i+=4;}
					else {sco = vdata.substring(i+2, i+3);i+=3;}
					sum+=Integer.parseInt(sco);
					n+=1;
				}
				else i++;
			}
			showInfo("[Digital Rating Result]");
			showInfo("    Average Score  : "+sum/n);
			return;
		}
		if(vtype.equals("Democracy Review")){
			int sum=0;
			int n=0;
			int[] nu = {0,0,0,0,0}; 
			String sco;
			while(i<vdata.length()){
				if(vdata.charAt(i)=='V' && vdata.charAt(i+1)=='R'){
					//System.out.println(vdata.substring(i+2, i+3));
					nu[Integer.parseInt(vdata.substring(i+2, i+3))]+=1;
					sum+=Integer.parseInt(vdata.substring(i+2, i+3));
					n+=1;
					i+=6;
				}
				else i++;
				
			}
			sum=sum/n;
			switch(sum){
			case 0:sco="very good";break;
			case 1:sco="good";break;
			case 2:sco="ordinary";break;
			case 3:sco="bad";break;
			case 4:sco="very bad";break;
			default:sco="very bad";
			}
			showInfo("[Democracy Review Result]");
			showInfo("very good : "+nu[0]);
			showInfo("good      : "+nu[1]);
			showInfo("ordinary  : "+nu[2]);
			showInfo("bad       : "+nu[3]);
			showInfo("very bad  : "+nu[4]);
			showInfo("Average   : "+sco);
			return;
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

	public void commandAction(Command c, Displayable arg1) {
		// TODO Auto-generated method stub
		if (c == exitCommand) {
			parent.destroyApp(true);
			parent.notifyDestroyed();
		} else if (c == okCommand) {
			this.removeCommand(okCommand);
			for (int i = 0; i < server.sum; i++) {
				Sender sender = new Sender(server.osq[i]);
				sender.send(si.getText());
			}
		}
		
	}
}


