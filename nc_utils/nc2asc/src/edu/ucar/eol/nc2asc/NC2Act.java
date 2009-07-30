package edu.ucar.eol.nc2asc;


import javax.swing.*;
import java.io.*;
import java.util.*;
import java.awt.*;
import edu.ucar.eol.nc2ascData.*;

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
	}
	
	
	/**
	 *  Display the messages to users, and Confirm with users
	 * @param s
	 */
	public static int confirmMsgBox (String s) {
		JFrame p = new JFrame();
		return JOptionPane.showConfirmDialog(p, s, "", JOptionPane.YES_NO_OPTION);
		//JOptionPane.showm (p, s, "", JOptionPane.YES_NO_OPTION);
	}

	

	/**
	 * Write the message to the nc2ascLog file.
	 * Display the same message to the users if the program is in UI mode
	 * 
	 * @param msg --text to display;  mboxFlag- flag to display in msgbox 
	 * 
	 */ 
	public static void wrtMsg (String msg, boolean mboxFlag) {
		if (mboxFlag && !batchMode) {
			prtMsgBox(msg);
		}
		wrtMsg(msg);
	}
	
	/**
	 * Write the message to the nc2ascLog file.
	 * Display the same message to the users if the program is in UI mode
	 * @param msg
	 * 
	 */ 
	public static void wrtMsg (String msg) {
		//add time stamp
		String t = Calendar.getInstance().getTime().toString();
		
		if (log ==null) {
			try {
				log=new FileWriter("nc2ascLog.txt");
			} catch (IOException e) {
				log=null;
				//System.out.println("Cannot create nc2ascLog");
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
	
	/**
	 * 
	 * @param f --file name
	 * @return  -- is file or dir
	 */
	public static boolean validataFile(String f) {
		File fv = new File(f);
		return fv.isFile();
	}

	/**
	 * 
	 * @param f  -- file name
	 * @return file-exist or not
	 */
	public static boolean existFile(String f) {
		File fv = new File(f);
		return fv.exists();
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


