package nc2Asc;

import java.lang.*;
import java.lang.Exception.*;
import java.util.ArrayList;
import java.util.*;
import java.util.List;
import java.util.zip.DataFormatException;
import java.awt.ComponentOrientation;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.*;
import java.awt.*; 
import java.io.*;

import javax.swing.*;
import javax.swing.filechooser.*;
import javax.swing.table.*; 

import ucar.ma2.InvalidRangeException;
import ucar.nc2.Variable;

import nc2AscData.*;

public class NC2AUI {

	//command name strings
	private final static String  INFILE = "INFILE";
	private final static String  OUTFILE = "OUTFILE";
	private final static String  DATAFORMAT = "DATAFORMAT";
	private final static String  PROCESS = "PROCESS";
	private final static String  VARDATA = "VARDATA";
	private final static String  SELALL = "SELALL";
	private final static String  DESELALL = "DESELALL";	

	// component containers
	private JPanel jp, jpTbl, jpBt; GridBagConstraints c1, c2;
	private JScrollPane tblScrlPane;

	//tbl
	private JTable tbl;  MyTblModel model;

	//button, textfield, chkbox. etc
	private JButton bnFileIn, outButton, bnData, bnFmt, bnProc;
	private JTextField tfFileIn, tfFileOut;
	private JCheckBox  cbSel, cbDesel;

	///class objs: data-fmt selection dialog, and netcdf-data, and data-format
	private NC2AUIDiag dialog;	  
	private NCData     ncdata;
	private DataFmt    datafmt = new DataFmt();
	String[] dataInf; 


	/**
	 * Main method to create all the components:
	 * buttons, tbl, check, display field, etc.
	 * @param frm -- main frame where the UI components to be.
	 */
	public void createComponents ( JFrame frm) {

		//save parent container
		Container pane = frm.getContentPane();

		//layout and constraints		
		pane.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.fill = GridBagConstraints.HORIZONTAL;
		c.weighty = 1.0; 
		c.weightx = 0.5;

		try {
			//add io fields
			c.insets = new Insets(20, 0, 0, 0); // top padding
			c.gridx = 0;
			c.gridy = 0;
			c.gridheight=2;
			c.gridwidth=c.REMAINDER;
			pane.add(createUIIOFields(), c);


			//row-3 tbl-lable
			c.insets = new Insets(30, 0, 0, 0); // top padding
			c.gridx = 0;
			c.gridy = 2;
			c.gridheight=c.REMAINDER;
			c.gridwidth=3;
			c.anchor = GridBagConstraints.PAGE_END; // bottom of space
			pane.add(createUITbl(), c);

			// row -3 add buttons
			c.ipadx=0;
			c.gridwidth=1;//c.REMAINDER;
			c.gridheight=1;
			c.anchor=c.NORTH;
			c.gridx = 3; // aligned with col3
			c.gridy = 2; // 2--4th row
			c.insets = new Insets(55, 0, 0, 0); // top paddingreadData
			pane.add(createUIBtns(), c);
		} catch (Exception e) {
			NC2Act.prtMsgBox(e.getMessage());
		}
	}

	/**
	 * Create a panel for input and output buttons. Put the tow buttons in the panel.
	 * 
	 * @return -- the panel... it will be added to the frame
	 */
	private JPanel createUIIOFields() {

		jp  = new JPanel();
		jp.setLayout(new GridBagLayout());

		bnFileIn  = new JButton("Select Input File");
		outButton = new JButton("Select Output Path");
		addButtonActCmd(bnFileIn, INFILE.toString());
		addButtonActCmd(outButton, OUTFILE.toString());

		tfFileIn = new JTextField("/home/data/pac10.nc");
		tfFileIn.setBackground(Color.LIGHT_GRAY);
		tfFileIn.setEditable(false);
		tfFileIn.setFont(new Font("System", Font.ITALIC,  16));
		tfFileOut =new JTextField("/home/data/default.txt");
		tfFileOut.setFont(new Font("System", Font.ITALIC,  16));

		c1 = new GridBagConstraints();
		c1.fill = GridBagConstraints.HORIZONTAL;
		c1.weighty = 1.0; 
		c1.weightx = 0.5;

		//row-one   input-file
		c1.gridx = 0;
		c1.gridy = 0;
		jp.add(bnFileIn,c1);

		c1.gridx = 1;
		c1.gridy = 0;
		c1.ipadx = 400;
		c1.gridwidth = c1.REMAINDER;
		jp.add(tfFileIn,c1);

		c1.gridx =0;
		c1.gridy =1; 	
		c1.ipadx=0;
		c1.gridwidth=1;
		jp.add(outButton,c1);

		c1.gridx = 1;
		c1.gridy = 1;
		c1.gridwidth = c1.REMAINDER;
		jp.add(tfFileOut, c1);

		return jp;

	}

	/**
	 * Create a panel for a button and a table Put the two in the panel.
	 * 
	 * @return -- the panel... it will be added to the frame
	 */
	private JPanel createUITbl()  {
		jpTbl  = new JPanel();
		jpTbl.setLayout(new GridBagLayout());

		c2 = new GridBagConstraints();
		c2.fill = GridBagConstraints.HORIZONTAL;
		c2.weighty = 1.0; 
		c2.weightx = 0.5;

		bnData = new JButton("Data Variables");
		addButtonActCmd(bnData, VARDATA.toString());
		c2.gridx = 1;
		c2.gridy = 0;
		c2.gridwidth = 2;//c2.REMAINDER;
		c2.gridheight = 1;
		c2.anchor = c2.CENTER;
		jpTbl.add(bnData, c2); 

		//createModelTbl();
		//tbl = new JTable(100,4);
		c2.insets = new Insets(1, 0, 0, 0); // top padding
		createTbl();
		tblScrlPane = new JScrollPane(tbl);
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
		addButtonActCmd(bnFmt, DATAFORMAT.toString());
		addButtonActCmd(bnProc, PROCESS.toString());

		cbSel = new JCheckBox("Select All");
		cbDesel = new JCheckBox("Clean All");
		addButtonActCmd(cbSel, SELALL.toString());
		addButtonActCmd(cbDesel, DESELALL.toString());

		jpBt = new JPanel();
		jpBt.setLayout(new GridLayout(8, 1));
		jpBt.add(bnFmt,0);
		jpBt.add(bnProc,1);
		jpBt.add(new JLabel(""), 2);
		jpBt.add(new JLabel(""),3);
		jpBt.add(new JLabel(""),4);
		jpBt.add(cbSel, 5);
		jpBt.add(cbDesel, 6);
		bnFmt.setEnabled(false);
		bnProc.setEnabled(false);
		cbSel.setEnabled(false);
		cbDesel.setEnabled(false);
		return jpBt;
	}


	/**
	 * This method provides the usage for all the components which have the abstractButton's characters
	 * to easily add the action-listener and its action in teh code
	 *   
	 * @param jb  -- an abstractor-component
	 * @param actStr -- action command
	 */
	void addButtonActCmd(AbstractButton jb, String actStr){
		jb.setActionCommand(actStr);
		jb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
				selectInputFile(e);
				selectOutputFile(e);
				getDataVar(e);
				selectDataFormat(e);
				process(e);
				selAll(e);
				deselAll(e);
			}
		});
	}

	/**
	 * Implement the actions when the input-file is selected...
	 * @param e -- ActionEvent to verify if this is this button's command.
	 */
	void selectInputFile(ActionEvent e) {
		if (INFILE.equals(e.getActionCommand())) {
			String[] strs = {"nc"};
			FileNameExtensionFilter filter = new FileNameExtensionFilter(null, strs);
			JFileChooser fileChooser = new JFileChooser(tfFileIn.getText().trim().toString());
			fileChooser.addChoosableFileFilter(filter);

			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				File selectedFile = fileChooser.getSelectedFile();
				if (selectedFile.isFile()) {
					tfFileIn.setText(selectedFile.getAbsolutePath());//(selectedFile.getName());
				}
				else {
					tfFileIn.setText("Invalid file");
				}
			}
		}
	}  


	/**
	 * Implement the actions when the input-file is selected...
	 * @param e -- ActionEvent to verify if this is this button's command.
	 */
	void selectOutputFile(ActionEvent e) {
		if (OUTFILE.equals(e.getActionCommand())) {	
			JFileChooser fileChooser = new JFileChooser(tfFileOut.getText().trim().toString());
			fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				String selectedPath=fileChooser.getSelectedFile().getAbsolutePath();

				if (!selectedPath.isEmpty()) {
					tfFileOut.setText(selectedPath+"/");
				}
				else {
					tfFileOut.setText("Empty file path");
				}
			}
		}
	} 


	void getDataVar(ActionEvent e) {
		if (INFILE.equals(e.getActionCommand()) || VARDATA.equals(e.getActionCommand())) {	
			if (!validateInputFile(tfFileIn.getText()) ) {
				nc2Asc.NC2Act.wrtMsg("Invalid input file...");
				return;
			}
			nc2Asc.NC2Act.startWaitCursor(bnFileIn);
			populateTbl();					
			nc2Asc.NC2Act.stopWaitCursor(bnFileIn);
			bnFmt.setEnabled(true);
			bnProc.setEnabled(true);
			cbSel.setEnabled(true);
			cbDesel.setEnabled(true);
		} 
	}

	void selectDataFormat(ActionEvent e) {

		if (DATAFORMAT.equals(e.getActionCommand())) {
			if (!validateInputFile(tfFileIn.getText()) || (!validateOutFile(tfFileOut.getText()))) {
				return;
			}
			JFrame frm = new JFrame("Select Data Foramt");
			JFrame.setDefaultLookAndFeelDecorated(true);
			JDialog.setDefaultLookAndFeelDecorated(true);
			//frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

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
			dialog.setBounds(250, 150, 690, 400);
			dialog.setVisible(true);
		}
	} 

	void selAll(ActionEvent e){
		if (SELALL.equals(e.getActionCommand())) {
			if (cbSel.isSelected()){
				tbl.selectAll();
				cbDesel.setSelected(false);
			} else {
				tbl.clearSelection();
			}
		}
	}

	void deselAll(ActionEvent e){
		if (DESELALL.equals(e.getActionCommand())) {
			if (cbDesel.isSelected()){
				tbl.clearSelection();
				cbSel.setSelected(false);
				cbDesel.setSelected(false);
			} 
		}
	}


	void process(ActionEvent e) {
		if (PROCESS.equals(e.getActionCommand())) {
			//chk out file
			if (!validateOutFile(tfFileOut.getText())) {
				NC2Act.wrtMsg("Invalid output file...");
				return;
			}

			//get selected rows from the tbl
			int[] idxs = tbl.getSelectedRows();
			String idxstr="";
			for (int i=0; i<idxs.length; i++) {
				idxstr += idxs[i] + " ";
			}
			if (idxs.length==0) {
				NC2Act.wrtMsg("Please select variables from the table...");
				return;
			}

			nc2Asc.NC2Act.startWaitCursor(bnFileIn);
			writeOutVarData(idxs);
			nc2Asc.NC2Act.stopWaitCursor(bnFileIn);
		}  
	}

	void createTbl() {
		model = new MyTblModel(3000, 4);
		tbl = new JTable(model);	
		tbl.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
		tbl.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		tbl.setAutoscrolls(true);
		tbl.sizeColumnsToFit(4);
		String[] colHeaders = {"VarName","Units","OR/Len","LongName"};
		int i=0;
		for (i=0; i<4;i++){
			tbl.getColumnModel().getColumn(i).setHeaderValue(colHeaders[i]);
		}
	}

	void populateTbl()  {
		if (ncdata==null) {
			ncdata = new NCData(tfFileIn.getText(), tfFileOut.getText());
		}
		try {
			ncdata.openFile();
			ncdata.readDataInf();
			dataInf = ncdata.getDataInf();

		} catch ( ArrayIndexOutOfBoundsException ae) {
			nc2Asc.NC2Act.wrtMsg("populateTbl_Array index out of bound"+ae.getMessage());
		} catch (NCDataException n) {
			nc2Asc.NC2Act.wrtMsg("populateTbl_NCdata exception "+n.getMessage());
		} catch (IOException ioe){
			nc2Asc.NC2Act.wrtMsg("populateTbl_IO exception "+ioe.getMessage());
		} catch (Exception ex) {
			nc2Asc.NC2Act.wrtMsg("populateTbl_Other exception "+ex.getMessage());
		}

		//check data
		if (dataInf==null || dataInf.length<1 ) {
			nc2Asc.NC2Act.wrtMsg("Information data is null... Use default data for format display");
			return;
		}
		// set data

		for (int i=1; i<dataInf.length; i++){
			String data =dataInf[i];
			if (data==null || data.length()<1) {return;}
			String[] d = data.split(DataFmt.SEPDELIMIT.toString());
			if (tbl.getRowCount()<i-1){
				nc2Asc.NC2Act.wrtMsg("Table is not long enough: "+ i);
				break;
			}
			model.setValueAt((Object)d[0], i-1, 0);
			model.setValueAt((Object)d[1], i-1, 1);
			model.setValueAt((Object)d[2], i-1, 2);
			model.setValueAt((Object)d[3], i-1, 3);
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
			nc2Asc.NC2Act.wrtMsg("Invalid Input File: "+fn);
			return false;
		}
		return true;
	}

	private boolean validateOutFile(String fn){
		try {
			FileOutputStream f = new FileOutputStream(fn);
		} catch (Exception e) {
			nc2Asc.NC2Act.wrtMsg("Invalid Output File: "+fn+ "\n" + e);
			return false;
		}
		return true;
	}


	private List<Variable>  getSubVarList(int[] idxs){
		List<Variable> slvars= new ArrayList<Variable>();
		List<Variable> allvars= ncdata.getVars();
		int len = idxs.length;
		if (len > (allvars.size()-1)) {len = allvars.size()-1;} //in case select all includes  extra empty rows
		//allvars.size()-1, because the first one is time-var
		for (int i=0; i<len; i++) {
			slvars.add(i,allvars.get(idxs[i]+1)); //idxs[i]+1 to count the time var on the first line
		}
		return slvars;
	}

	private String genVarName(List<Variable> sublvars ){
		String varname = "Date,UTC";

		for (int i =0; i<sublvars.size(); i++) {
			Variable v =sublvars.get(i);
			varname += DataFmt.SEPDELIMIT.toString()+v.getName();

			//check if the it has multi-data
			int[] shape = v.getShape();
			if (shape.length <3 || shape[2]<=1) {
				continue;
			}
			// the var has multi-data. we need to add numbers as the varnames for the rest of the values
			for (int j=1; j<shape[2]; j++) {
				varname += DataFmt.SEPDELIMIT.toString()+j ;
			}
		}
		//nc2Asc.NC2Act.wrtMsg("varname_len:"+varname.split(",").length+ " "+varname);
		return varname;
	}


	private void writeOut(String msg) {
		try {
			ncdata.getOutFile().write(msg);
		} catch (IOException e ) {
			nc2Asc.NC2Act.wrtMsg("writeOut_err:"+e.getMessage());
		}
	}


	public void writeOutVarData(int[] idxs){
		//get and write the data out to the outfile
		try {
			List<Variable> sublvars = getSubVarList(idxs);
			java.io.FileWriter outFile = ncdata.getOutFile();

			//write le();
			int size = sublvars.size(); //.varname to the first line of out file
			String out= genVarName(sublvars); 
			out = datafmt.fmtDmtr(out)+"\n";
			writeOut(out); 
			long milSec = ncdata.getTimeMilSec();
			
			// all the time-range data  len-should be the seconds in the time range
			int[] range = getTmRange();
			nc2Asc.NC2Act.wrtMsg("tm_range:" +range[0]+ " size:"+range[1]);
			long t1 = Calendar.getInstance().getTimeInMillis();
			for (int i=0; i<sublvars.size(); i++) {
				ncdata.read1DData(sublvars.get(i) , 0, range[1]);
			}
			
			nc2Asc.NC2Act.wrtMsg("Done.  Tm (milliSeconds) :"+ (Calendar.getInstance().getTimeInMillis()-t1));
			
/*			for (int t=range[0] ; t <(range[1]-10); t++) {  //todo: cal time in seconds
				String dt = ncdata.getNewTm(milSec, t);

				//write one-second data of ALL vars to the out file
				for (int i=0; i<size; i++){
					String data = ncdata.readOneVarData(sublvars.get(i), t);
					data = DataFmt.SEPDELIMIT.toString()+ data;
					dt += data; //out += data;
				}

				dt =datafmt.fmtOneLineData(dt)+ "\n";
				//writeOut(dt);
				outFile.flush();
				out += dt;/
			}
			nc2Asc.NC2Act.wrtMsg("tmdiffff :"+ (Calendar.getInstance().getTimeInMillis()-t1));
			writeOut(out);
			//nc2Asc.NC2Act.wrtMsg(out);
	*/		
		} catch (NullPointerException ne){
			nc2Asc.NC2Act.wrtMsg("writeOutVarData_NullPointerException "+ne.getMessage());
		//} catch (DataFormatException de) {
			//nc2Asc.NC2Act.wrtMsg("writeOutVarData_DataFormatException "+de.getMessage());
		} catch (InvalidRangeException ie) {
			nc2Asc.NC2Act.wrtMsg("writeOutVarData_InvalidRangeException..."+ ie.getMessage());
		} catch (IOException ee) {
			nc2Asc.NC2Act.wrtMsg("writeOutVarData_IOException..."+ee.getMessage());
		}
	} 

	private int[] getTmRange() {
		int[] ii = new int[2];
		try {
			if (datafmt.getDataFmt()[DataFmt.TMSET_IDX]==DataFmt.FULLTM.toString()) { //all the data
				ii[0]=0;
				ii[1]=(int)ncdata.getVars().get(0).getSize(); // .getShape()[0];
				return ii;
			}
			
			//get selected  range---
			//get start
			String[] tm= datafmt.getDataFmt()[DataFmt.TM_IDX].split(DataFmt.TMSETDELIMIT); 
			ii[0] = (int)(ncdata.getNewTimeMilSec(tm[0]) - ncdata.getTimeMilSec())/1000;
			//get length
			String[] t1 = tm[0].split(DataFmt.TMSETCOLON.toString());
			String[] t2 = tm[1].split(DataFmt.TMSETCOLON.toString());
			int h = Integer.parseInt(t1[0]);
			int m = Integer.parseInt(t1[1]);
			int s = Integer.parseInt(t1[2]);
			int h2 = Integer.parseInt(t2[0]);
			int m2 = Integer.parseInt(t2[0]);
			int s2 = Integer.parseInt(t2[0]);
			ii[1] = h2*3600+m2*60+s2 - h*3600+m*60+s ;

		} catch (NCDataException ne) {
			nc2Asc.NC2Act.wrtMsg("NC2AUI_getTmRange_NCDataException"+ne.getMessage());
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