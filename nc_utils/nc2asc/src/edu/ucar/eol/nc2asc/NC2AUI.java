package edu.ucar.eol.nc2asc;

import java.awt.Color;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.AbstractButton;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.SwingWorker;
import javax.swing.border.EtchedBorder;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.DefaultTableModel;

import ucar.nc2.Variable;
import edu.ucar.eol.nc2ascData.DataFmt;
import edu.ucar.eol.nc2ascData.NCData;
import edu.ucar.eol.nc2ascData.NCDataException;

public class NC2AUI  implements ActionListener, PropertyChangeListener{

	//in class
	class Task extends SwingWorker<Void, Void> {
		/*
		 * Main task. Executed in background thread.
		 */
		@Override
		public Void doInBackground() {
			int progress = ncdata.getProgIdx();
			setProgress(0);
			if (taskLen < 1) {NC2Act.wrtMsg("Task Len = 0"); return null;}
			while (!ncdata.getFinish()) {
				//Sleep for up to one second.
				setProgress(Math.min(100*progress/taskLen, 100));
				try {
					Thread.sleep(1000);
				} catch (InterruptedException ignore) {}
				progress = ncdata.getProgIdx();
			}
			progress = ncdata.getProgIdx();
			setProgress(Math.min(100*progress/taskLen, 100));
			bnCancel.setVisible(false);
			bnProc.setEnabled(true);
			return null;
		}

		/*
		 * Executed in event dispatching thread
		 */
		@Override
		public void done() {
			//setProgress(100);
			Toolkit.getDefaultToolkit().beep();
			if (ncdata.getProgIdx()<taskLen) {
				statusBar.setText("Canceled. taskLen= "+ taskLen + " finished= "+ ncdata.getProgIdx());
			} else {
				statusBar.setText("  Done.    Elapsed time is " + ncdata.getTmPassed()/1000.0 + " seconds." );
			}
			//setCursor(Cursor.DEFAULT_CURSOR); //turn off the wait cursor
			pfrm.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

		}
	}
	//command name strings
	private final static String  INFILE = "INFILE";
	//private final static String  OUTFILE = "OUTFILE";    
	private final static String  RDBATCH = "RDBATCH";
	private final static String  SVBATCH = "SVBATCH"; 
	private final static String  QUIT= "QUIT";
	private final static String  DATAFORMAT = "DATAFORMAT";
	private final static String  PROCESS = "PROCESS";
	private final static String  TOGGLE  = "TOGGLE";
	private final static String  SELALL = "SELALL";
	private final static String  DESELALL = "DESELALL";	
	private final static String  N2AHELP = "N2AHELP";

	// component containers
	private JFrame     pfrm;
	private Task       task;

	private JTable tbl; 
	private JButton    bnProc, bnFmt, bnCancel, bnTog;
	private JCheckBox  cbSel, cbDesel; 
	private JTextField tfRt;
	private StatusBarMsg  statusBar;

	///class objs: data-fmt selection dialog, and netcdf-data, and data-format
	private NC2AUIDiag dialog; //NC2AUIProgBar dlgProgBar;	  
	private NCData     ncdata  = new NCData();
	private DataFmt    datafmt = new DataFmt();

	private List<String>   dataInf;   //dataInf contains all the variable data information = lvars-ncdata-- first one is the time var
	private List<Integer>  idxSearch; //search result list  -- skip time var
	private int            idxCount;  //search count		-- skip time var
	private List<String>   selStatus; //select status 		-- skip the time var 

	private String[]    fileName = new String[3];  // batchFile, inputFileName and outputFileName
	private int 		taskLen; //max len of tasks     

	public void setStatus(String msg) {
		statusBar.setText(msg);
	}
	/**
	 * Main method to create all the components:
	 * buttons, tbl, check, display field, etc.
	 * @param frm -- main frame where the UI components to be.
	 */
	public void createComponents ( JFrame frm) {
		pfrm=frm;
		//save parent container
		Container pane = frm.getContentPane(); 
		//layout and constraints		
		pane.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.weighty = 1.0; 
		c.weightx = 0.5;

		try {
			//add  menus
			c.ipadx=5;
			c.insets = new Insets(1, 0, 0, 0); // top padding
			c.gridx = 0;
			c.gridy = 0;
			c.gridheight=2;
			c.gridwidth=GridBagConstraints.REMAINDER;
			pane.add( createFileMenu(), c);

			//row-2 tbl-lable
			c.insets = new Insets(20, 0, 0, 0); // top padding
			c.gridx = 0;
			c.gridy = 2;
			c.gridheight=GridBagConstraints.REMAINDER;
			c.gridwidth=5;
			pane.add(createUITbl(), c);

			// row -3 add buttons
			c.ipadx=0;
			c.gridwidth=3;
			c.gridheight=1;
			c.anchor=GridBagConstraints.NORTH;
			c.gridx = 5; // aligned with col3
			c.gridy = 2;//2; // 2--4th row
			c.insets = new Insets(55, 0, 0, 0); // top paddingreadData
			pane.add(createUIBtns(), c);

			//c.insets = new Insets(60, 0, 0, 0); // top padding
			c.ipadx=5;
			c.gridwidth= GridBagConstraints.REMAINDER;
			c.gridheight=1;//GridBagConstraints.REMAINDER;
			//c.anchor=GridBagConstraints.SOUTH;
			c.gridx = 0; // aligned with col3
			c.gridy = 3;//2; // 2--4th row
			c.anchor = GridBagConstraints.PAGE_END; // bottom of space
			c.insets = new Insets(25, 0, 0, 0); // top paddingreadData
			statusBar = new StatusBarMsg(); 
			statusBar.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.RAISED));
			NC2Act.setStatusBar(statusBar);
			pane.add(statusBar, c);

		} catch (Exception e) {
			NC2Act.wrtMsg(e.getMessage());
		}
	}

	/**
	 * 
	 * @return -- menu bar that contains file menu and items
	 */
	private JPanel createFileMenu(){
		JMenuBar  mBar 		= new JMenuBar();
		mBar.add(Box.createHorizontalGlue());

		JMenu 	  mFile  	= new JMenu("File   ");
		Color c = new Color(200, 80, 0);
		mFile.setForeground(c);
		JMenuItem miInput 	= new JMenuItem("Select Input File"); 
		addButtonActCmd(miInput,  INFILE);
		JMenuItem miBatchSv	= new JMenuItem("Save Batch File");
		addButtonActCmd(miBatchSv, SVBATCH);
		JMenuItem miBatchRd = new JMenuItem("Read Batch File");
		addButtonActCmd(miBatchRd, RDBATCH);
		JMenuItem miQuit    = new JMenuItem("Quit"); 
		addButtonActCmd(miQuit, QUIT);

		JMenu mHelp = new JMenu("Help            ");
		mHelp.setForeground(c);
		addButtonActCmd(mHelp, N2AHELP);

		mFile.add(miInput);
		mFile.insertSeparator(1);
		mFile.add(miBatchSv);
		mFile.insertSeparator(3);
		mFile.add(miBatchRd);
		mFile.insertSeparator(5);
		mFile.add(miQuit);

		mBar.add(mFile);
		mBar.add(mHelp);
		mBar.setBackground(Color.LIGHT_GRAY);
		mBar.setBorderPainted(true);
		mBar.setForeground(Color.BLUE);
		JMenuBar m1 =new JMenuBar(); m1.setBackground(Color.LIGHT_GRAY);
		JMenuBar m2 =new JMenuBar(); m2.setBackground(Color.LIGHT_GRAY);
		JMenuBar m3 =new JMenuBar(); m3.setBackground(Color.LIGHT_GRAY);
		JMenuBar m4 =new JMenuBar(); m4.setBackground(Color.LIGHT_GRAY);
		JPanel jp  = new JPanel(); 
		jp.setLayout(new GridLayout(1, 5));
		jp.setBackground(Color.lightGray );
		jp.add(mBar,0);
		jp.add(m1,1);
		jp.add(m2,2);
		jp.add(m3,3);
		jp.add(m4,4);
		return jp;
	}


	/**
	 * Create a panel for a button and a table Put the two in the panel.
	 * 
	 * @return -- the panel... it will be added to the frame
	 */
	private JPanel createUITbl()  {
		JPanel jpTbl  = new JPanel();
		jpTbl.setLayout(new GridBagLayout());

		GridBagConstraints c2 = new GridBagConstraints();
		c2.fill = GridBagConstraints.HORIZONTAL;
		c2.weighty = 1.0; 
		c2.weightx = 0.5;
		JLabel lVar = new JLabel("                                              Variable List");
		c2.gridx = 0;
		c2.gridy = 0;
		c2.gridwidth = 2;//c2.REMAINDER;
		c2.gridheight = 1;
		c2.anchor = GridBagConstraints.CENTER;
		jpTbl.add(lVar, c2); 

		//createModelTbl();
		//tbl = new JTable(100,4);
		c2.insets = new Insets(1, 0, 0, 0); // top padding
		createTbl();
		JScrollPane tblScrlPane = new JScrollPane(tbl);
		c2.gridx = 0;
		c2.gridy = 1;
		c2.gridwidth = GridBagConstraints.REMAINDER;
		c2.gridheight = GridBagConstraints.REMAINDER;
		c2.anchor = GridBagConstraints.PAGE_END; // bottom of space
		jpTbl.add(tblScrlPane,c2);

		return jpTbl;  
	}


	/**
	 * Create a panel for format and process buttons, and sel-all dsel-all chkbox. Put all in the panel.
	 * 
	 * @return -- the panel... it will be added to the frame
	 */
	private JPanel createUIBtns()  {
		bnFmt = new JButton("Select Data Format");
		bnProc  = new JButton(" Process");
		addButtonActCmd(bnFmt, DATAFORMAT);
		addButtonActCmd(bnProc, PROCESS);

		//cancel and toggle
		bnCancel= new JButton("Cancel");
		bnCancel.setForeground(new Color( 150, 0,0));
		addButtonActCmd(bnCancel, "cancel");

		bnTog = new JButton("Toggle "); bnTog.setForeground(new Color(50, 100,255));
		cbSel = new JCheckBox("Select All");
		cbDesel = new JCheckBox("Clean All");
		addButtonActCmd(bnTog, TOGGLE);
		addButtonActCmd(cbSel, SELALL);
		addButtonActCmd(cbDesel, DESELALL);

		JPanel jp = new JPanel();
		jp.setLayout(new GridLayout(0, 2));
		//jp.add(new JLabel(""));
		jp.add(new JLabel(""));
		jp.add(bnCancel);  

		JPanel jpTog = new JPanel();
		jpTog.setLayout(new GridLayout(0, 2));
		bnTog.setSize(5,3);
		jpTog.add(bnTog);


		JPanel jpBt = new JPanel();
		jpBt.setLayout(new GridLayout(16, 1));
		int i=0;
		jpBt.add(bnFmt,i++);
		jpBt.add(bnProc,i++);
		jpBt.add(jp,i++);
		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""), i++);
		jpBt.add(jpTog,i++);
		jpBt.add(cbSel, i++);
		jpBt.add(cbDesel, i++);


		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""),i++);

		tfRt = new JTextField();
		jpBt.add(new JLabel("Search..."),i++);
		jpBt.add(tfRt, i++);

		tfRt.setVisible(false);
		bnFmt.setEnabled(false);
		bnProc.setEnabled(false);
		bnCancel.setVisible(false);
		cbSel.setEnabled(false);
		cbDesel.setEnabled(false);
		bnTog.setEnabled(false);

		tfRt.addKeyListener( new KeyListener() {
			public void keyPressed(KeyEvent e) {};
			public void keyReleased(KeyEvent e) {
				repopulateTbl(tfRt.getText());
			};
			public void keyTyped(KeyEvent e) {
				//repopulateTbl(tfRt.getText());
			};
		});
		return jpBt; 
	}


	/**
	 * This method provides the usage for all the components which have the abstractButton's characters
	 * to easily add the action-listener and its action in teh code
	 *   
	 * @param jb  -- an abstractor-component
	 * @param actStr -- action command
	 */

	private void addButtonActCmd(AbstractButton jb, String actStr){
		jb.setActionCommand(actStr);
		jb.addActionListener(this) ;

	}

	/**
	 * Invoked when the user presses the start button.
	 */
	public void actionPerformed(ActionEvent e) {
		selectInputFile(e);
		getDataVar(e);
		selectDataFormat(e);
		process(e);
		toggle(e);
		selAll(e);
		deselAll(e);
		saveBatch(e);
		readBatch(e);
		cancelDataProgress(e);
		quitApp(e);

	}

	/**
	 * Invoked when task's progress property changes.
	 */
	public void propertyChange(PropertyChangeEvent evt) {
		if ("progress" == evt.getPropertyName()) {
			int progress = (Integer) evt.getNewValue();
			statusBar.setText("    Data Progress is "+progress + "% done.");
		} 
	}


	private void cancelDataProgress(ActionEvent e) {
		if ("cancel".equals(e.getActionCommand())) {
			if (bnCancel.isVisible() && ncdata.getProgIdx() < taskLen ) {
				ncdata.setFinish(true);
			}
		} 
	}

	/**
	 * Implement the actions when the input-file is selected...
	 * @param e -- ActionEvent to verify if this is this button's command.
	 */
	private boolean selectInputFile(ActionEvent e) {
		if (INFILE.equals(e.getActionCommand())) {
			//filter
			String[] strs = {"nc"};
			FileNameExtensionFilter filter = new FileNameExtensionFilter(null, strs);
			//default file
			String txt =System.getenv("DATA_DIR"); 
			if ( txt==null || txt.isEmpty()){
				File dir = new File(".");
				txt =dir.getAbsolutePath();
			}
			//String txt = "/home/data/pac10.nc";
			JFileChooser fileChooser = new JFileChooser(txt);
			fileChooser.addChoosableFileFilter(filter);
			fileChooser.setDialogTitle("                  Select Input File");
			fileChooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);


			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				File selectedFile = fileChooser.getSelectedFile();
				if (selectedFile.isFile()) {
					fileName[1] =selectedFile.getAbsolutePath();//(selectedFile.getName());
					return true;
				}
				else {
					NC2Act.wrtMsg("Invalid Input File...");
				}
			}
		}
		return false;
	}  


	/**
	 *  it invokes a dialog box to select an output file for users
	 */
	private boolean selectOutputFile() {

		String txt =System.getenv("DATA_DIR"); 
		if (fileName[2]!=null && !fileName[2].isEmpty()) {
			txt = fileName[2];
		} else {
			if ( txt==null || txt.isEmpty()){
				File dir = new File(".");
				txt =dir.getAbsolutePath();
			}
		}
		JFileChooser fileChooser = new JFileChooser(txt);
		fileChooser.setDialogTitle("            Select Output File");
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);

		int returnValue = fileChooser.showOpenDialog(null);
		if (returnValue == JFileChooser.APPROVE_OPTION) {
			String out =fileChooser.getSelectedFile().getAbsolutePath();
			if (out == null || out.isEmpty()) {
				NC2Act.wrtMsg("Invalid Output File...");
				return false;
			}
			if (NC2Act.existFile(out)) {
				if (NC2Act.confirmMsgBox("File "+ out + " exists.         Overwrite? ")== JOptionPane.NO_OPTION) {
					selectOutputFile();
				}
			}

			fileName[2]= out;
			try {
				ncdata.openOutFile(fileName[2]); 
				return true;
			} catch (IOException e ) {
				NC2Act.wrtMsg("selectOutputFile_openOutFile_IOException...");
			}
		}
		return false;
	} 


	private void getDataVar(ActionEvent e) {
		if (INFILE.equals(e.getActionCommand())) {	
			if (fileName[1]==null || !validateInputFile(fileName[1]) ) {
				NC2Act.wrtMsg("Invalid input file: ");
				return;
			}

			//nc2asc.NC2Act.startWaitCursor(tbl);
			pfrm.setCursor(new Cursor(Cursor.WAIT_CURSOR));
			populateTbl();			
			datafmt.initDataFmt();
			pfrm.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

			enableButtons();

		} 
	}

	private void selectDataFormat(ActionEvent e) {

		if (DATAFORMAT.equals(e.getActionCommand())) {
			if (!validateInputFile(fileName[1]) ) {
				return;
			}
			JFrame frm = new JFrame("Select Data Foramt");
			JFrame.setDefaultLookAndFeelDecorated(true);
			JDialog.setDefaultLookAndFeelDecorated(true);
			//frm.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);// .DO_NOTHING_ON_CLOSE);// .EXIT_ON_CLOSE);

			frm.setLocation(250, 150);
			String[] demodata = new String[10];
			try {
				demodata =ncdata.getDemoData();
			} catch ( ArrayIndexOutOfBoundsException a){
				NC2Act.wrtMsg("getDemoData Array index out of bound"+a.getMessage());
			} catch (NCDataException ee) {
				NC2Act.wrtMsg("getDemoData NCData exception "+ee.getMessage());
			} catch (IOException nce){
				NC2Act.wrtMsg("getDemoData IO exception "+nce.getMessage());
			} catch (Exception ex) {
				NC2Act.wrtMsg("getDemoData exception "+ex.getMessage());
			}

			dialog = new NC2AUIDiag(frm, true, demodata, datafmt,  ncdata.getGlobalDataInf());
			dialog.setBounds(250, 150, 690, 450);
			dialog.setVisible(true);
		}
	} 

	private void readBatch (ActionEvent e) {
		if (RDBATCH.equals(e.getActionCommand())) {
			// get batch file 
			NC2Act.wrtMsg("Read batch file...");
			String txt =System.getenv("DATA_DIR"); 
			if (fileName[0]!=null && !fileName[0].isEmpty()) {
				txt = fileName[0];
			} else {
				if ( txt==null || txt.isEmpty()){
					File dir = new File(".");
					txt =dir.getAbsolutePath();
				}
			}
			JFileChooser fileChooser = new JFileChooser(txt);
			fileChooser.setDialogTitle("            Select Batch File");
			fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);

			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				String out =fileChooser.getSelectedFile().getAbsolutePath();

				if (out.isEmpty()) {
					NC2Act.wrtMsg("ReadBatch: Invalid input batch file. Discontinue.");
					return;
				} else {
					fileName[0]= out;
				}
			}

			//read batch
			String[] arg ={"-b", fileName[0]};
			BatchConfig bf = new BatchConfig(arg);
			bf.start();

			//bf.getDataFmt(), bf.getTmSetOrig()
			//String[] ff = bf.getDataFmt();
			datafmt.setDataFmt(bf.getDataFmt(), bf.getTmSetOrig());

			String[] files= bf.getFiles();
			if (files[1]==null || files[1].isEmpty()) {
				NC2Act.wrtMsg("No input file from the batch reading.");
				return;
			}
			fileName[1]= files[1];
			fileName[2]= files[2];
			populateTbl();
			enableButtons();
			List<String> vars= bf.getSelVars();

			if (vars==null || vars.size()==0) {
				NC2Act.wrtMsg("No input variables. ");
				return;
			}

			//set selStatus
			for (int i=1; i<dataInf.size(); i++) { //start at 1 to skip time variable
				//Variable v= vars.get(i);
				String line = dataInf.get(i);
				for (int j=0; j<vars.size(); j++) {
					int idx = vars.get(j).indexOf('=') + 1;
					String s1 = vars.get(j).substring(idx);
					String target = s1.trim();
                                        idx = line.indexOf(',');
                                        String str = line.substring(0, idx);
					if (str.equals(target)) {
						selStatus.set(i-1,"Y");
						tbl.setValueAt("Y", i-1, 4);
					}
				}
			} 

			if (ncdata.getGlobalDataInf()[0]>1 && datafmt.getDataFmt()[DataFmt.HEAD_IDX].equals(DataFmt.HEAD2)) {
				datafmt.setDataFmt(DataFmt.HEAD, DataFmt.HEAD_IDX);
			}
			ncdata.setMode(true);
			NC2Act.wrtMsg("  Reading batch file is done.");
		} //if
	}

	private void saveBatch (ActionEvent e) {
		if (SVBATCH.equals(e.getActionCommand())) {

			//check input
			if (fileName[1]==null ||fileName[1].isEmpty()) {
				NC2Act.wrtMsg("SaveBatch: No netcdf input file found.");
				return;
			}

			//  data format 
			if (datafmt==null) {
				NC2Act.wrtMsg("No data format selected...");
				return;
			}

			// get batch file 
			NC2Act.wrtMsg("Saving batch arguments...");
			String txt =System.getenv("DATA_DIR"); 
			if (fileName[0]!=null && !fileName[0].isEmpty()) {
				txt = fileName[0];
			} else {
				if ( txt==null || txt.isEmpty()){
					File dir = new File(".");
					txt =dir.getAbsolutePath();
				}
			}
			JFileChooser fileChooser = new JFileChooser(txt);
			fileChooser.setDialogTitle("            Select Batch File");
			fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);

			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				String out =fileChooser.getSelectedFile().getAbsolutePath();

				if (out == null || out.isEmpty()) {
					NC2Act.wrtMsg("SaveBatch: Invalid batch output file...");
					return;
				}
				if (NC2Act.existFile(out)) {
					if (NC2Act.confirmMsgBox("File "+ out + " exists.         Overwrite? ")== JOptionPane.NO_OPTION) {
						saveBatch(e);
					}
				}
				fileName[0]= out;
			}

			//NC2Act.prtMsgBox("new batch file="+fileName[0]); //fordebug
			FileWriter bout = null;
			try {
				bout = new FileWriter(fileName[0]);
			} catch (Exception ee ) {
				NC2Act.wrtMsg("saveBatch_openBatchFile_Exception...");
				return;
			}

			String line = "if= "+fileName[1]+ "\n";
			line +="of= "+fileName[2]+ "\n\n";
			String[] tmp= datafmt.getDataFmt();  //datafmt is not null
			line += "hd= "+ tmp[DataFmt.HEAD_IDX]+"\n";
			line += "avg= "+ tmp[DataFmt.AVG_IDX]+"\n";
			line += "dt= "+ tmp[DataFmt.DATE_IDX]+"\n";
			line += "tm= "+ tmp[DataFmt.TM_IDX]+"\n";
			if (tmp[DataFmt.DMTR_IDX].equals(DataFmt.COMMAVAL)) {
				line += "sp= "+ DataFmt.COMMA+"\n";
			} else {
				line += "sp= "+ DataFmt.SPACE+"\n";
			}

			if (tmp[DataFmt.MVAL_IDX].isEmpty()) {
				line += "fv= "+ DataFmt.LEAVEBLANK+"\n";
			} else if (tmp[DataFmt.MVAL_IDX].equals(DataFmt.MISSVAL )) {
				line += "fv= "+ DataFmt.FILLVALUE+"\n";
			} else  {
				line += "fv= "+ DataFmt.REPLICATE+"\n";
			}

			line +="ti= "+ datafmt.getTmSet()+"\n\n";

			//  variables 
			List<Variable> subvars = getSubVarList();
			for (int i=0; i<subvars.size(); i++) {
				line +="var= "+ subvars.get(i).getName()+ "\n";
			}
			//   write out
			try {
				bout.write(line);
				bout.close();
			} catch (IOException ioe) {
				NC2Act.wrtMsg("saveBatch_writeOut_exception...");
			}
			NC2Act.wrtMsg("  Done.");
		}
	}

	private void quitApp(ActionEvent e) {
		if (QUIT.equals(e.getActionCommand())) {
			System.runFinalization();
			System.exit(0);
		}
	}

	private void toggle(ActionEvent e) {

		if (TOGGLE.equals(e.getActionCommand())) {
			if (TOGGLE.equals(e.getActionCommand())) {
				statusBar.setText(" Ready");
				if (idxCount <1) {
					statusBar.setText("toggle_idxCount < 1");
				}

				for (int i=0; i<idxCount; i++) {
					if (!tbl.isRowSelected(i)) {continue;}
					String v = (String)tbl.getValueAt(i, 4); 
					if (v==null || !v.equals("Y")) {
						tbl.setValueAt("Y", i, 4);
						selStatus.set(idxSearch.get(i),"Y");
					} else { 
						tbl.setValueAt("", i, 4);
						selStatus.set(idxSearch.get(i), "");
					} 
				}
				return;
			} //if_cbTog
		}//if_cmd
	}
	private void selAll(ActionEvent e){
		if (SELALL.equals(e.getActionCommand())) {
			if (cbSel.isSelected()){
				statusBar.setText(" Ready");
				cbDesel.setSelected(false);
				//tbl.selectAll();
				int len = idxCount-1;
				if (idxCount<1) {
					len = dataInf.size()-1;
				}
				tbl.clearSelection();
				tbl.changeSelection(len, 4, false,false);
				tbl.changeSelection(0, 0, true, true);
			} else {
				tbl.clearSelection();
			}
		}
	}

	private void deselAll(ActionEvent e){
		if (DESELALL.equals(e.getActionCommand())) {
			if (cbDesel.isSelected()){
				statusBar.setText(" Ready");
				tbl.clearSelection();
				cbSel.setSelected(false);
				cbDesel.setSelected(false);
			} 
		}
	}

	private void process(ActionEvent e) {
		if (PROCESS.equals(e.getActionCommand())) {

			List<Variable> sublvars = getSubVarList();
			if (sublvars.size()==0) {
				NC2Act.wrtMsg("No variables selected.   Please select variables from the table...", true);
				return;
			}

			//chk out file
			NC2Act.wrtMsg("Start Processing...");
			if (!selectOutputFile()) {
				NC2Act.wrtMsg(" Ready");
				return;
			}
			if ( !validateOutFile(fileName[2])) {
				return;
			}

			bnProc.setEnabled(false);

			//write out data
			String[] fmt =datafmt.getDataFmt();
			if (fmt[DataFmt.HEAD_IDX].equals(DataFmt.HEAD2)) {
				ncdata.writeOut(ncdata.getAmesHead(sublvars)+"\n");
			}
			String out= ncdata.genVarName(sublvars, fmt); 
			ncdata.writeOut(out+"\n");
			int[] range = getTmRange();
			//long t1 = Calendar.getInstance().getTimeInMillis();

			//start thread one to write data
			DataThread dth = new DataThread(ncdata, sublvars, range, datafmt.getNewDataFmt()); 
			dth.start();

			//start thread two to display % of works
			taskLen = sublvars.size()+ range[1];
			task = new Task();
			task.addPropertyChangeListener(this);
			task.execute();
			bnCancel.setVisible(true); 
			pfrm.setCursor(new Cursor(Cursor.WAIT_CURSOR));

		}
	}



	private void createTbl() {
		MyTblModel model = new MyTblModel(800, 5); 
		tbl = new JTable(model);	

		String[] colHeaders = {"  VarName        ","Units","OR/Len"," LongName            ", " "};
		int i=0;
		for (i=0; i<5;i++){
			tbl.getColumnModel().getColumn(i).setHeaderValue(colHeaders[i]);
		}
		tbl.getColumnModel().getColumn(0).setMaxWidth(140);
		tbl.getColumnModel().getColumn(1).setMaxWidth(60);
		tbl.getColumnModel().getColumn(2).setMaxWidth(70);
		tbl.getColumnModel().getColumn(3).setMaxWidth(200);
		tbl.getColumnModel().getColumn(4).setMaxWidth(20);

		tbl.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
		tbl.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		tbl.setAutoscrolls(true);
		tbl.sizeColumnsToFit(4); 
		tbl.addMouseListener( new java.awt.event.MouseListener() {
			public void mousePressed(MouseEvent e) {};
			public void mouseReleased(MouseEvent e) {};
			public void mouseClicked(MouseEvent e) { cbSel.setSelected(false);};
			public void mouseEntered(MouseEvent e) {};
			public void mouseExited(MouseEvent e) {};
		});

	}

	private void populateTbl()  {
		if (fileName[1]==null || fileName[1].isEmpty()) {
			NC2Act.wrtMsg("Invalid Input File: "+ fileName[1]);
			return;
		}
		try {
			ncdata.openFile(fileName[1]);
			ncdata.readDataInf();

			//clean up tbl
			if (dataInf!=null){
				for (int i=1; i<dataInf.size(); i++){
					tbl.setValueAt("", i-1, 0);
					tbl.setValueAt("", i-1, 1);
					tbl.setValueAt("", i-1, 2);
					tbl.setValueAt("", i-1, 3);
					tbl.setValueAt("", i-1, 4);
				}
			}

			dataInf=ncdata.getDataInf();
			//NC2Act.wrtMsg("dataInf len="+dataInf.size(), true);
			idxSearch = new ArrayList<Integer>();
			selStatus = new ArrayList<String>();
			idxCount =dataInf.size()-1;  //skip the first time data
			for (int i=0; i<idxCount; i++){
				idxSearch.add(i, i);
				selStatus.add(i,"");
			}
			//statusBar.setText("datainf len "+ dataInf.size());
		} catch ( ArrayIndexOutOfBoundsException ae) {
			NC2Act.wrtMsg("populateTbl_Array index out of bound"+ae.getMessage());
		} catch (NCDataException n) {
			NC2Act.wrtMsg("populateTbl_NCdata exception "+n.getMessage());
		} catch (IOException ioe){
			NC2Act.wrtMsg("populateTbl_IO exception "+ioe.getMessage());
		} catch (Exception ex) {
			NC2Act.wrtMsg("populateTbl_Other exception "+ex.getMessage());
		}

		//check data
		if (dataInf==null || dataInf.size()<1 ) {
			NC2Act.wrtMsg("Information data is null... Use default data for format display");
			return;
		}
		// set data

		for (int i=1; i<dataInf.size(); i++){
			String data =dataInf.get(i);
			if (data==null || data.length()<1) {return;}
			String[] d = data.split(DataFmt.COMMAVAL);
			if (tbl.getRowCount()<i-1){
				NC2Act.wrtMsg("Table is not long enough: "+ i);
				break;
			}

			tbl.setValueAt((Object)d[0], i-1, 0);
			tbl.setValueAt((Object)d[1], i-1, 1);
			tbl.setValueAt((Object)d[2], i-1, 2);
			tbl.setValueAt((Object)d[3], i-1, 3);
		}
		//model.removeRow(row);
		tbl.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
		tbl.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		tbl.setAutoscrolls(true);

	}

	private boolean validateInputFile(String fn){
		try {
			new FileInputStream(fn);
		} catch (Exception e) {
			NC2Act.wrtMsg("validateInputFile: Invalid Input File: "+fn);
			return false;
		}
		return true;
	}

	private boolean validateOutFile(String   fn){
		try {
			new FileOutputStream(fn);
		} catch (Exception e) {
			NC2Act.wrtMsg("validateOutputFile: Invalid Output File: "+fn);
			return false;
		}
		return true;
	}


	private List<Variable>  getSubVarList(){
		List<Variable> slvars= new ArrayList<Variable>(); 
		List<Variable> allvars= ncdata.getVars();
		List<String>   varfmt = new ArrayList<String>();
		int len = allvars.size()-1; 
		for (int i=0; i<len; i++) {
			if (selStatus.get(i)==null || !selStatus.get(i).equals("Y")) {
			} else {
				slvars.add(allvars.get(i+1)); //[i]+1 to skip the time var on the first line
				varfmt.add("%f");
			}
		}
		ncdata.setVarDatFmt(varfmt);
		return slvars;
	}

	private void repopulateTbl(String pref){
		statusBar.setText(" Ready");
		idxCount =0; 
		tbl.clearSelection(); cbSel.setSelected(false);
		String st = pref.trim().toUpperCase();
		for (int i=1; i<dataInf.size(); i++){
			String data =dataInf.get(i);
			if (data==null || data.length()<1) {return;}
			String[] d = data.split(DataFmt.COMMAVAL);
			tbl.setValueAt(" ", i-1, 0);
			tbl.setValueAt(" ", i-1, 1);
			tbl.setValueAt(" ", i-1, 2);
			tbl.setValueAt(" ", i-1, 3);
			tbl.setValueAt(" ", i-1, 4);

			if (d[0].indexOf(st) >=0) {
				tbl.setValueAt((Object)d[0], idxCount, 0);
				tbl.setValueAt((Object)d[1], idxCount, 1);
				tbl.setValueAt((Object)d[2], idxCount, 2);
				tbl.setValueAt((Object)d[3], idxCount, 3);
				tbl.setValueAt((Object)selStatus.get(i-1), idxCount, 4);
				idxSearch.set(idxCount, i-1);
				idxCount++;
			}
		} //for
	}



	private int[] getTmRange()  {
		int[] ii = new int[2];
		String tmset = datafmt.getNewDataFmt()[DataFmt.TMSET_IDX];

		if (tmset.equals(DataFmt.FULLTM)) { //all the data
			ii[0]=0;
			ii[1]=(int)ncdata.getGlobalDataInf()[2]; // .getShape()[0];
			return ii;
		}
		try {
			String[] iiStr = tmset.split(DataFmt.TMSETDELIMIT);	
			ii[0] =  (int)(Long.parseLong(iiStr[0]) - ncdata.getTimeMilSec())/1000 ;
			ii[1] =  Integer.parseInt(iiStr[1]);
		} catch (NullPointerException e) {
			NC2Act.wrtMsg("getTmRange_nullPointerException"+ e.getStackTrace());
		} catch (NCDataException ee) {
			NC2Act.wrtMsg("getTmRange_NCDataException"+ ee.getStackTrace());
		}

		return ii;
	}

	private void enableButtons() {
		bnFmt.setEnabled(true);
		bnProc.setEnabled(true);
		tfRt.setVisible(true);
		cbSel.setEnabled(true);
		cbDesel.setEnabled(true);
		bnTog.setEnabled(true);
	}

}//eof class


/**
 * Make a Tbl class that extends DefaultModel, so that myTbl can have more desirable features
 * to handle data and control tbl.
 *  
 * @author dongl
 *
 */
class MyTblModel extends DefaultTableModel {
	static final long serialVersionUID =0;
	
	MyTblModel(int rows, int cols) {
		super(rows, cols);
	}

	public void addRow(Object[] rowData) {
		super.addRow(rowData);
	}

}


/**
 *   StatusBar extends JLabel and implements setMessage method 
 */
class StatusBarMsg extends JLabel   {
	/** Creates a new instance of StatusBar */
	static final long serialVersionUID =0;

	public StatusBarMsg() {
		super();
		super.setPreferredSize(new Dimension(100, 16));
		setMessage(" Ready");
	}

	public void setMessage(String message) {
		setText(" "+message);        
	}        
	public void setBackground(Color c) {
		super.setBackground(c);
	}
}



class DataThread extends Thread {
	DataThread(NCData obj, List<Variable> vrs, int[] rg, String[] ft) {
		ncdata=obj;
		vars = vrs;
		range = rg;
		fmt = ft;
	}
	public void run() {
		ncdata.writeDataToFile(vars, range, fmt);
	}

	private List<Variable> vars;
	private NCData ncdata;
	private int[] range;
	private String[] fmt;
}


