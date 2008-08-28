package nc2Asc;

import java.awt.ComponentOrientation;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;


/** 
 * NC2ASCII is a program for users to convert some variables (data) from a netcdf file to  
 * Ascii format data. So that users can perform edition in the readable formats.
 *  
 * @author dongl
 *
 */
public class NC2A extends JPanel implements ActionListener {

	
	/**
	 * Important ui element class instance to add UI items
	 */
	private static NC2AUI aui  = new NC2AUI();

	/**
	 * Create the GUI and show it. For thread safety, this method should be
	 * invoked from the event-dispatching thread.
	 */  
	private static void createAndShowGUI() {
		// Create and set up the window.
		JFrame frame = new JFrame("NetCDF To Ascii");
			
		frame.setDefaultLookAndFeelDecorated(true);
		JDialog.setDefaultLookAndFeelDecorated(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocation(250, 150);
		aui.createComponents(frame);

		// Display the window.
		frame.pack();
		frame.setVisible(true);
	}

		
	/**
	 * 
	 *  @param args
	 *  This program has two modes: 
	 *  1> Schedulc e a job for the event-dispatching thread:
	 *	2> Creating and showing this application's GUI.
	 *  
	 *  UI mode:    to provide user graphic interface
	 *  batch mode (-b option): to provide user to run multiple files with the same option set.
	 *  By default: the UI mode is applied. 
	 *  
	 */
	public static void main(String[] args) {
        
		if (args.length >0 && args[0].toString().equals("-b")) {
			//NC2Act.wrtMsg(args[0].toString());
		}	else {
			NC2Act.setMood(false);
			javax.swing.SwingUtilities.invokeLater(new Runnable() {
				public void run() {
					createAndShowGUI();
				}
			});
		}
	}

	/**
	 * This is a interface in order to inheritance ActionListener 
	 * the real action implementation is in NC2AUI class
	 */
	public void actionPerformed(ActionEvent e) {
	}

}//eof class NC2A



