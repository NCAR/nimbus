

package nc2Asc;

import javax.swing.*;
import java.io.*;
import java.util.*;
import java.awt.*;

public class NC2Act {

	NC2Act() {};
	NC2Act(boolean bm) {_batchMood = bm;}
	protected void finalize() {
		if (_log !=null) {
			try {
				_log.close();
			} catch (IOException e) {}
		} 
	}

	private static FileWriter _log=null;
	private static boolean    _batchMood= true;
	private JComponent cmpt= null;

	/**
	 * allow user to setup JComponent that is used to control main UI display, cursor, etc.
	 * @param c
	 */
	public void setComponent(JComponent c) {cmpt=c;}
	
	/**
	 * Allow users to setup the program mood, either batch mood or UI mood
	 * 
	 * @param bm
	 */
	public static void setMood(boolean bm) {_batchMood = bm;}
	
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
		//msg = "\n\n"+t+"\n" + msg;
		
		//check log file
		if (_log ==null) {
			try {
				_log=new FileWriter("nc2AscLog.txt");
			} catch (IOException e) {
				prtMsgBox("Cannot create nc2AscLog");
			}			   
		}

		//write msg to the log
		try {
			String mg ="\n\n"+ t +"\n" + msg;
			_log.write(mg);
			_log.flush();
		} catch (IOException ie) { 	 
			prtMsgBox("IOException: Cannot write to log "+ie.toString());
		} catch (NullPointerException ne) {
		   prtMsgBox("NullPointerException: Cannot write to log "+ne.toString());	
		}

		//display for user if UI mood
		if (!_batchMood) {
			prtMsgBox(msg);
		}
	}
	
	public boolean validataFile(String f) {
		File fv = new File(f);
		return fv.isFile();
			
	}
	
	public static void startWaitCursor(JComponent cmpt) {
		//if (cmpt == null) {wrtMsg("JCmpt in Util is null..."); return;}
	    //RootPaneContainer root =
	    //  (RootPaneContainer)cmpt.getTopLevelAncestor();
	    cmpt.getTopLevelAncestor().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
	    //root.getGlassPane().setVisible(true);
	  }

	  /** Sets cursor for specified component to normal cursor */
	  public static void stopWaitCursor(JComponent cmpt) {
		//if (cmpt == null) {wrtMsg("JCmpt in Util is null..."); return;}
	    //RootPaneContainer root =
	   //   (RootPaneContainer)cmpt.getTopLevelAncestor();
		  cmpt.getTopLevelAncestor().setCursor(Cursor.getDefaultCursor());
	   // root.getGlassPane().setVisible(false);
	  }

	
	
	
} // eo class


