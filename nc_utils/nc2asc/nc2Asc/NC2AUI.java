package nc2Asc;

import java.lang.*;
import java.lang.Exception.*;
import java.util.*;
import java.util.List;
import java.util.zip.DataFormatException;
import java.awt.ComponentOrientation;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.*;
import java.awt.*; 
import java.io.*;
import java.beans.*;

import javax.swing.*;
import javax.swing.filechooser.*;
import javax.swing.table.*; 

import ucar.ma2.InvalidRangeException;
import ucar.nc2.Variable;

import nc2Asc.NC2AUIProgBar.Task;
import nc2AscData.*;

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
			if (taskLen < 1) {nc2Asc.NC2Act.wrtMsg("Task Len = 0"); return null;}
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
				statusBar.setText("  Done.    Time is "+ ncdata.getTmPassed()+ " mil seconds." );
			}
			//setCursor(Cursor.DEFAULT_CURSOR); //turn off the wait cursor
			pfrm.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));

		}
	}
	//command name strings
	private final static String  INFILE = "INFILE";
	private final static String  OUTFILE = "OUTFILE";    
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
	private JButton    bnProc, bnFmt, bnCancel;
	private JCheckBox  cbTog, cbSel, cbDesel; 
	private JTextField tfRt;
	private StatusBar  statusBar;

	///class objs: data-fmt selection dialog, and netcdf-data, and data-format
	private NC2AUIDiag dialog; //NC2AUIProgBar dlgProgBar;	  
	private NCData     ncdata;
	private DataFmt    datafmt = new DataFmt();

	private List<String>   dataInf;   //dataInf contains all the variable data information = lvars-ncdata-- first one is the time var
	private List<Integer>  idxSearch; //search result list  -- skip time var
	private int            idxCount;  //search count		-- skip time var
	private List<String>   selStatus; //select status 		-- skip the time var 

	private String[]    fileName = new String[2];  // inputFileName and outputFileName
	private int 		taskLen; //max len of tasks     


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
			c.gridwidth=c.REMAINDER;
			pane.add( createFileMenu(), c);

			//row-2 tbl-lable
			c.insets = new Insets(20, 0, 0, 0); // top padding
			c.gridx = 0;
			c.gridy = 2;
			c.gridheight=c.REMAINDER;
			c.gridwidth=3;
			//c.anchor = GridBagConstraints.PAGE_END; // bottom of space
			pane.add(createUITbl(), c);

			// row -3 add buttons
			c.ipadx=0;
			c.gridwidth=1;//c.REMAINDER;
			c.gridheight=1;
			c.anchor=c.NORTH;
			c.gridx = 3; // aligned with col3
			c.gridy = 2;//2; // 2--4th row
			c.insets = new Insets(55, 0, 0, 0); // top paddingreadData
			pane.add(createUIBtns(), c);

			c.ipadx=5;
			c.gridwidth= 4;
			c.gridheight=1;
			//c.anchor=GridBagConstraints.SOUTH;
			c.gridx = 0; // aligned with col3
			c.gridy = 3;//2; // 2--4th row
			c.insets = new Insets(15, 0, 0, 0); // top paddingreadData
			statusBar = new StatusBar(); 
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
		JMenuItem miInput 	= new JMenuItem("Select Input File"); 
		addButtonActCmd(miInput,  INFILE);
		JMenuItem miBatchSv	= new JMenuItem("Save Batch File");
		addButtonActCmd(miBatchSv, SVBATCH);
		JMenuItem miBatchRd = new JMenuItem("Read Batch File");
		addButtonActCmd(miBatchRd, RDBATCH);
		JMenuItem miQuit    = new JMenuItem("Quit"); 
		addButtonActCmd(miQuit, QUIT);

		JMenu mHelp = new JMenu("Help         ");
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
		c2.anchor = c2.CENTER;
		jpTbl.add(lVar, c2); 

		//createModelTbl();
		//tbl = new JTable(100,4);
		c2.insets = new Insets(1, 0, 0, 0); // top padding
		createTbl();
		JScrollPane tblScrlPane = new JScrollPane(tbl);
		c2.gridx = 0;
		c2.gridy = 1;
		c2.gridwidth = c2.REMAINDER;
		c2.gridheight = c2.REMAINDER;
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

		cbTog = new JCheckBox("Toggle ");
		cbSel = new JCheckBox("Select All");
		cbDesel = new JCheckBox("Clean All");
		addButtonActCmd(cbTog, TOGGLE);
		addButtonActCmd(cbSel, SELALL);
		addButtonActCmd(cbDesel, DESELALL);

		JPanel jpBt = new JPanel();
		jpBt.setLayout(new GridLayout(16, 1));
		int i=0;
		jpBt.add(bnFmt,i++);
		jpBt.add(bnProc,i++);

		JPanel jp = new JPanel();
		jp.setLayout(new GridLayout(0, 3));
		bnCancel= new JButton("Cancel");
		bnCancel.setForeground(new Color( 150, 0,0));
		addButtonActCmd(bnCancel, "cancel");
		jp.add(new JLabel(""));
		jp.add(new JLabel(""));
		jp.add(bnCancel);
		jpBt.add(jp, i++);

		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""),i++);
		jpBt.add(new JLabel(""),i++);
		jpBt.add(cbTog, i++);
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
		cbTog.setEnabled(false);

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
			/*String txt =System.getenv("DATA_DIR"); 
			if ( txt==null || txt.isEmpty()){
				File dir = new File(".");
				txt =dir.getAbsolutePath();
			}*/
			String txt = "/home/data/pac10.nc";
			JFileChooser fileChooser = new JFileChooser(txt);
			fileChooser.addChoosableFileFilter(filter);
			fileChooser.setDialogTitle("                  Select Input File");
			fileChooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);


			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				File selectedFile = fileChooser.getSelectedFile();
				if (selectedFile.isFile()) {
					fileName[0] =selectedFile.getAbsolutePath();//(selectedFile.getName());
					return true;
				}
				else {
					nc2Asc.NC2Act.wrtMsg("Invalid Input File...");
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
		if (fileName[1]!=null && !fileName[1].isEmpty()) {
			txt = fileName[1];
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

			if (out.isEmpty()) {
				nc2Asc.NC2Act.wrtMsg("Invalid Output File...");
			} else {
				fileName[1]= out;
				try {
					ncdata.openOutFile(fileName[1]);
					return true;
				} catch (IOException e ) {
					nc2Asc.NC2Act.wrtMsg("selectOutputFile_openOutFile_IOException...");
				}
			}
		}
		return false;
	} 


	private void getDataVar(ActionEvent e) {
		if (INFILE.equals(e.getActionCommand())) {	
			if (fileName[0]==null || !validateInputFile(fileName[0]) ) {
				//nc2Asc.NC2Act.wrtMsg("Invalid input file: ");
				return;
			}

			//nc2Asc.NC2Act.startWaitCursor(tbl);
			pfrm.setCursor(new Cursor(Cursor.WAIT_CURSOR));
			populateTbl();					
			pfrm.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
			//nc2Asc.NC2Act.stopWaitCursor(tbl);
			bnFmt.setEnabled(true);
			bnProc.setEnabled(true);
			tfRt.setVisible(true);
			cbSel.setEnabled(true);
			cbDesel.setEnabled(true);
			cbTog.setEnabled(true);
		} 
	}

	private void selectDataFormat(ActionEvent e) {

		if (DATAFORMAT.equals(e.getActionCommand())) {
			if (!validateInputFile(fileName[0]) ) {
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
				nc2Asc.NC2Act.wrtMsg("getDemoData Array index out of bound"+a.getMessage());
			} catch (NCDataException ee) {
				nc2Asc.NC2Act.wrtMsg("getDemoData NCData exception "+ee.getMessage());
			} catch (IOException nce){
				nc2Asc.NC2Act.wrtMsg("getDemoData IO exception "+nce.getMessage());
			} catch (Exception ex) {
				nc2Asc.NC2Act.wrtMsg("getDemoData exception "+ex.getMessage());
			}

			dialog = new NC2AUIDiag(frm, true, demodata, ncdata.getGlobalDataInf());
			dialog.setBounds(250, 150, 690, 450);
			dialog.setVisible(true);
		}
	} 

	private void readBatch (ActionEvent e) {
		if (RDBATCH.equals(e.getActionCommand())) {

		}
	}

	private void saveBatch (ActionEvent e) {
		if (SVBATCH.equals(e.getActionCommand())) {

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
			if (cbTog.isSelected()){
				cbTog.setSelected(false);
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
				NC2Act.wrtMsg("No variables selected.   Please select variables from the table...");
				return;
			}

			//chk out file
			NC2Act.wrtMsg("Start Processing...");
			if (!selectOutputFile()) {
				NC2Act.wrtMsg(" Ready");
				return;
			}
			if ( !validateOutFile(fileName[1])) {
				return;
			}

			//bnCancel.setVisible(true); 
			bnProc.setEnabled(false);
			//write out data
			String out= genVarName(sublvars); 
			ncdata.writeOut(datafmt.fmtDmtr(out)+"\n");
			int[] range = getTmRange();
			long t1 = Calendar.getInstance().getTimeInMillis();

			DataThread dth = new DataThread(ncdata, sublvars, range, datafmt.getNewDataFmt()); 
			dth.start();

			taskLen = sublvars.size()+ range[1];
			task = new Task();
			task.addPropertyChangeListener(this);
			task.execute();
			bnCancel.setVisible(true); 
			pfrm.setCursor(new Cursor(Cursor.WAIT_CURSOR));

		}
	}



	private void createTbl() {
		MyTblModel model = new MyTblModel(3000, 5); 
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
		if (fileName[0]==null || fileName[0].isEmpty()) {
			NC2Act.wrtMsg("Invalid Input File: "+ fileName[0]);
			return;
		}
		if (ncdata==null) {
			ncdata = new NCData();
		}
		try {
			ncdata.openFile(fileName[0]);
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
			nc2Asc.NC2Act.wrtMsg("Information data is null... Use default data for format display");
			return;
		}
		// set data

		for (int i=1; i<dataInf.size(); i++){
			String data =dataInf.get(i);
			if (data==null || data.length()<1) {return;}
			String[] d = data.split(DataFmt.SEPDELIMIT);
			if (tbl.getRowCount()<i-1){
				nc2Asc.NC2Act.wrtMsg("Table is not long enough: "+ i);
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
			FileInputStream f = new FileInputStream(fn);
		} catch (Exception e) {
			nc2Asc.NC2Act.wrtMsg("validateInputFile: Invalid Input File: "+fn);
			return false;
		}
		return true;
	}

	private boolean validateOutFile(String   fn){
		try {
			FileOutputStream f = new FileOutputStream(fn);
		} catch (Exception e) {
			nc2Asc.NC2Act.wrtMsg("validateOutputFile: Invalid Output File: "+fn);
			return false;
		}
		return true;
	}


	private List<Variable>  getSubVarList(){
		List<Variable> slvars= new ArrayList<Variable>(); 
		List<Variable> allvars= ncdata.getVars();
		int len = allvars.size()-1; 
		for (int i=0; i<len; i++) {
			if (selStatus.get(i)==null || !selStatus.get(i).equals("Y")) {
			} else {
				slvars.add(allvars.get(i+1)); //[i]+1 to skip the time var on the first line
			}
		}
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
			String[] d = data.split(DataFmt.SEPDELIMIT);
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

	private String genVarName(List<Variable> sublvars ){
		String varname = "Date,UTC";

		for (int i =0; i<sublvars.size(); i++) {
			Variable v =sublvars.get(i);
			varname += DataFmt.SEPDELIMIT+v.getName();

			//check if the it has multi-data
			int[] shape = v.getShape();
			if (shape.length <3 || shape[2]<=1) {
				continue;
			}
			// the var has multi-data. we need to add numbers as the varnames for the rest of the values
			for (int j=1; j<shape[2]; j++) {
				varname += DataFmt.SEPDELIMIT+j ;
			}
		}
		//nc2Asc.NC2Act.wrtMsg("varname_len:"+varname.split(",").length+ " "+varname);
		return varname;
	}



	/*public long  writeOutVarData(List<Variable> sublvars){
		String line = "";
		//get and write the data out 
		try{
			String out= genVarName(sublvars); 
			ncdata.writeOut(datafmt.fmtDmtr(out)+"\n");
			long milSec = ncdata.getTimeMilSec();

			// all the time-range data len-should be the seconds in the time range
			int[] range = getTmRange();
			int size = sublvars.size(); //.varname to the first line of out file
			float[][] data = new float [size][];
			long t1 = Calendar.getInstance().getTimeInMillis();
			int[] oneDLen= new int[size];

			for (int i =0; i<size; i++){	
				Variable tmp = sublvars.get(i); 
				data[i] = ncdata.read1DData(tmp , range[0], range[1]);
				oneDLen[i] =ncdata.getLen(tmp);
				line = tmp.getName();
				//statusBar.setText(line);
			}
			//write out
			String del = datafmt.getDataFmt()[DataFmt.DMTR_IDX];;
			for (int i =range[0]; i<range[1]; i++) {
				line = ncdata.getNewTm(milSec,i);

				for (int j =0; j<size; j++) {
					int count =0;
					while (count<oneDLen[j]) {
						line += del + data[j][oneDLen[j]*i+count];
						count++;			
					}
				}
				ncdata.writeOut(line+"\n");
			}

			long ret = Calendar.getInstance().getTimeInMillis()-t1;
			NC2Act.wrtMsg("Done.     Total " +ret+ " miliSeconds");
			return ret;

		} catch (NullPointerException ne){
			NC2Act.wrtMsg("writeOutVarData_NullPointerException "+line);
		} catch (InvalidRangeException ie) {
			NC2Act.wrtMsg("writeOutVarData_InvalidRangeException..."+ line);
		} catch (IOException ee) {
			NC2Act.wrtMsg("writeOutVarData_IOException..."+ee.getMessage()+line);
		} catch (Exception e ) {
			NC2Act.wrtMsg("writeOutVarData_Exception"+ line);
		} 

		return 0;
	} 
*/

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

}//eof class


/**
 * Make a Tbl class that extends DefaultModel, so that myTbl can have more desirable features
 * to handle data and control tbl.
 *  
 * @author dongl
 *
 */
class MyTblModel extends DefaultTableModel {
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
class StatusBar extends JLabel   {
	/** Creates a new instance of StatusBar */
	public StatusBar() {
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


