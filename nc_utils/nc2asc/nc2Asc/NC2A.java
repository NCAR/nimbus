package nc2Asc;

import java.awt.ComponentOrientation;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.List;
import javax.swing.*;

import nc2AscData.NCData;
import ucar.nc2.Variable;


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
	private static NC2AUI aui  =  null; 

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
	 *  1> Schedule a job for the event-dispatching thread:
	 *	2> Creating and showing this application's GUI.
	 *  
	 *  UI mode:    to provide user graphic interface
	 *  batch mode (-b option): to provide user to run multiple files with the same option set.
	 *  By default: the UI mode is applied. 
	 *  
	 */
	public static void main(String[] args) {
        
		if (args.length >0 && findBatch(args)) {
			NC2Act.setMode(true);
			setArgs(args);
		}	else {
			//NC2Act.setMode(false);
			aui = new NC2AUI();
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
	
	/**
	 * Search each input to find batch file. -b follows filename 
	 * @return -- contains batch file or not
	 */
	private static boolean findBatch(String[] args) {
		for (int i=0; i<args.length; i++ ) {
			if (args[i]== "-b") return true;
		}
		return false;
	}
	
	/**
	 * If the program is running in batch mode, the input arguments contains batch file, and/or input, and output.
	 * This method signs the files into the NC2AUI local file name strings, 
	 * start parsing the batch file to get i/o files, data format, and  
	 * @param args  -b batchfile, -i input -o output
	 */
	private static void setArgs(String[] args) {
		//batchArgs= args;

		BatchConfig bf = new BatchConfig(args);
		bf.start();  //read and interpret the inputs

		String[] fmt = bf.getDataFmt();
//		bf.showFmt(); //fordebug

		int[] range = bf.getTmRange();

		//get Vals from variable names
		NCData ncdata= new NCData();
		ncdata.setMode(true);
		try {
			ncdata.openFile(bf.getFiles()[1]);
			ncdata.openOutFile(bf.getFiles()[2]);
		} catch (IOException e ) {
			System.out.println("Batch-mode open i/o file fails..."+ e.getStackTrace());
			return;
		}
		List<Variable> sublvars= ncdata.getBatchSubVars(bf.getSelVars());
//		bf.showSelectedVars(); //fordebug

		ncdata.writeDataToFile(sublvars, range, fmt);
		
	}



}//eof class NC2A



