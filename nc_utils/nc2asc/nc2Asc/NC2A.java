package edu.ucar.eol.nc2Asc;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.List;

import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;

import ucar.nc2.Variable;
import edu.ucar.eol.nc2AscData.*;



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
	 * overMidNight is a boolean to remember it happened, for AmesDEF format 
	 */
	static public boolean overMidNight = false;
		
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
			if (args[i].equals("-b")) return true; 
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
		
		BatchConfig bf = new BatchConfig(args);
		bf.start();  //read and interpret the inputs
        
		String[] fs = bf.getFiles();
		System.out.println("\nFiles= " +fs[0]+ " "+ fs[1]+ " "+fs[2]);
		
		String[] fmt = bf.getDataFmt();
				
		//get Vals from variable names
		NCData ncdata= new NCData();
		ncdata.setMode(true);
		
		try {
			ncdata.openFile(fs[1]);
			ncdata.openOutFile(fs[2]);
		} catch (Exception e ) {
			System.out.println("Batch-mode open i/o file fails..."+ e.getStackTrace());
			return;
		}
		
		int[] range = new int[2];
		try {
			range = ncdata.calBatchTmRange(fmt);
		} catch (Exception ee) {
			System.out.println("Batch-mode calBatchTmRange fails..."+ ee.getStackTrace());
			return;
		}
		System.out.println("bf-selectedVars= \n" +bf.showSelectedVars()+ "\nrange "+ range[0]+ "  "+ range[1]); 
		
		List<Variable> sublvars= ncdata.getBatchSubVars(bf.getSelVars());
		if (sublvars.size()<=0) {
			System.out.println("NO variables are selected...");
			return;
		}
		if (ncdata.getGlobalDataInf()[0]>1 && fmt[DataFmt.HEAD_IDX].equals(DataFmt.HEAD2)) { //high rate data NO Ames
			fmt[DataFmt.HEAD_IDX]=(DataFmt.HEAD);
			System.out.println(" Warning: High rate data netcdf file. AmesDEF is not supported.  ");
		}
		if (fmt[DataFmt.HEAD_IDX].equals(DataFmt.HEAD2)) {ncdata.writeOut( ncdata.getAmesHead(sublvars)+ "\n"); }
	
		System.out.println("bf_fmt_at_printout= \n"+ fmt[0]+ "\n"+ fmt[1]+ "\n"+ fmt[2]+ "\n"+ fmt[3]+ "\n"+ fmt[4]+ "\n"+ fmt[5]+ "\n"+ fmt[6]+ "\n"); 
		String out= ncdata.genVarName(sublvars, fmt);
		ncdata.writeOut(out+"\n"); 
		ncdata.writeDataToFile(sublvars, range, fmt);
		System.out.println("Writing is completed.");
		
	}



}//eof class NC2A



