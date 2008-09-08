

package nc2Asc;


import javax.swing.*;
import java.io.*;
import java.util.*;
import java.awt.*;
import nc2AscData.*;

public class NC2Act {

	NC2Act() {};
	NC2Act(boolean bm) {batchMode = bm;}
	
	private static FileWriter log=null;
	private static boolean    batchMode= false;
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
	 * Allow users to setup the program mode, either batch mode or UI mode
	 * 
	 * @param bm
	 */
	public static void setMode(boolean bm) {batchMode = bm;}

	/**
	 * Allow users to retrieve the program mode, either batch mode or UI mode
	 * 
	 * @return batchMode
	 */
	public static boolean getMode() {return batchMode;}

	
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
	 * Display the same message to the users if the program is in UI mode
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
				if (!batchMode ) 	prtMsgBox("Cannot create nc2AscLog");
				return;
			}			   
		}

		//write msg to the log
		try {
			String mg ="\n\n"+ t +"\n" + msg;
			log.write(mg);
			log.flush();
			System.out.println(msg);
		} catch (IOException ie) {
			if (!batchMode) 	prtMsgBox("IOException: Cannot write to log "+ie.toString());
			return;
		} catch (NullPointerException ne) {
			if (!batchMode) 	prtMsgBox("NullPointerException: Cannot write to log "+ne.toString());
			return; 
		}
		
		if (!batchMode ) {
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
		cmpt.getTopLevelAncestor().setCursor(new Cursor(Cursor.WAIT_CURSOR));
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


