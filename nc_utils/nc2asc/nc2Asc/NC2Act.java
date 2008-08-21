

package nc2Asc;


import javax.swing.*;
import java.io.*;
import java.util.*;
import java.awt.*;
import nc2AscData.*;

public class NC2Act {

	NC2Act() {};
	NC2Act(boolean bm) {batchMood = bm;}
	
	private static FileWriter log=null;
	private static boolean    batchMood= true;
	private static StatusBar  sBar= null;

	/**
	 * close the log file, when the program exists 
	 */
	protected void finalize() {
		if (log !=null) {
			try {
				log.close();
			} catch (IOException e) {}
		} 
	}
	
	/**
	 * allow user to setup JComponent that is used to control main UI display, cursor, etc.
	 * @param c
	 */
	public static void setStatusBar(StatusBar c) {sBar=c;}

	/**
	 * Allow users to setup the program mood, either batch mood or UI mood
	 * 
	 * @param bm
	 */
	public static void setMood(boolean bm) {batchMood = bm;}

	/**
	 *  Display the messages to users
	 * @param s
	 */
	public static  void prtMsgBox (String s) {

		JFrame p = new JFrame();
		JOptionPane.showMessageDialog(p, s);
		//JOptionPane.showConfirmDialog(p,s);
	}

	/**
	 * Write the message to the nc2AscLog file.
	 * Display the same message to the users if the program is in UI mood
	 * @param msg
	 * 
	 */ 
	public static void wrtMsg (String msg) {
		//add time stamp
		String t = Calendar.getInstance().getTime().toString();
		
		if (log ==null) {
			try {
				log=new FileWriter("nc2AscLog.txt");
			} catch (IOException e) {
				if (!batchMood ) 	prtMsgBox("Cannot create nc2AscLog");
				return;
			}			   
		}

		//write msg to the log
		try {
			String mg ="\n\n"+ t +"\n" + msg;
			log.write(mg);
			log.flush();
		} catch (IOException ie) {
			if (!batchMood) 	prtMsgBox("IOException: Cannot write to log "+ie.toString());
			return;
		} catch (NullPointerException ne) {
			if (!batchMood) 	prtMsgBox("NullPointerException: Cannot write to log "+ne.toString());
			return; 
		}
		
		if (!batchMood ) {
			if (sBar!=null){
				sBar.setText(msg);
			} else {
				prtMsgBox(msg);
			}
		}
		
	}
	public boolean validataFile(String f) {
		File fv = new File(f);
		return fv.isFile();

	}

	/**
	 * enable the wait cursor in the passed component's view
	 * @param cmpt
	 */
	public static void startWaitCursor(JComponent cmpt) {
		if (cmpt == null) {wrtMsg("startWaitCursor_cmpt  is null..."); return;}
		cmpt.getTopLevelAncestor().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
	}

	/**
	 * disable the wait cursor in the passed component's view
	 * @param cmpt
	 */
	public static void stopWaitCursor(JComponent cmpt) {
		if (cmpt == null) {wrtMsg("stopWaitCursor_cmpt  is null..."); return;}
		cmpt.getTopLevelAncestor().setCursor(Cursor.getDefaultCursor());
	}




} // eo class


