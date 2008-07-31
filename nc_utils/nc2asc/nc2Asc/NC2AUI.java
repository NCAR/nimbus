package nc2Asc;

import java.lang.*;
import java.lang.Exception.*;
import java.util.ArrayList;
import java.util.List;
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

import ucar.nc2.Variable;

import nc2AscData.*;

public class NC2AUI {

	private final static String  INFILE = "INFILE";
	private final static String  OUTFILE = "OUTFILE";
	private final static String  DATAFORMAT = "DATAFORMAT";
	private final static String  PROCESS = "PROCESS";
	private final static String  VARDATA = "VARDATA";
	private final static String  SELALL = "SELALL";
	//private final static String  DESELALL = "DESELALL";	

	private JPanel jp, jpTbl, jpBt; GridBagConstraints c1, c2;
	private JTable tbl;  MyTblModel model;

	private JScrollPane tblScrlPane;
	private JButton bnFileIn, outButton, bnData, bnFmt, bnProc;
	private JTextField tfFileIn, tfFileOut;
	private JCheckBox  cbSel; //cbDesel;

//	private JFrame     pfrm;
	private NC2AUIDiag dialog;	  
	private NCData     ncdata;
	String[] dataInf; 


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

	public JPanel createUIIOFields() throws Exception {
		try{
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

		} catch (Exception e) {
			throw e;
		}

		return jp;

	}

	JPanel createUITbl() throws Exception  {
		try {
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

		} catch (Exception e) {
			throw e;
		}
		return jpTbl;  
	}


	JPanel createUIBtns() throws Exception  {
		bnFmt = new JButton("Select Data Format");
		bnProc  = new JButton(" Process");
		addButtonActCmd(bnFmt, DATAFORMAT.toString());
		addButtonActCmd(bnProc, PROCESS.toString());

		cbSel = new JCheckBox("Select All");
		//cbDesel = new JCheckBox("Deselect All");
		addButtonActCmd(cbSel, SELALL.toString());
		//addButtonActCmd(cbDesel, DESELALL.toString());

		try{
			jpBt = new JPanel();
			jpBt.setLayout(new GridLayout(8, 1));
			jpBt.add(bnFmt,0);
			jpBt.add(bnProc,1);
			jpBt.add(new JLabel(""), 2);
			jpBt.add(new JLabel(""),3);
			jpBt.add(new JLabel(""),4);
			jpBt.add(cbSel, 5);
			//jpBt.add(cbDesel, 3);
			bnFmt.setEnabled(false);
			bnProc.setEnabled(false);
			cbSel.setEnabled(false);
			//cbDesel.setEnabled(false);
		} catch (Exception e) {
			throw e;
		}
		return jpBt;
	}


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
				//deselAll(e);
			}
		});
	}

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
				//nc2Asc.NC2Act.wrtMsg("getDemoData Array index out of bound"+a.getMessage());
			} catch (NCDataException ee) {
				nc2Asc.NC2Act.wrtMsg("getDemoData NCData exception "+ee.getMessage());
			} catch (IOException nce){
				nc2Asc.NC2Act.wrtMsg("getDemoData IO exception "+nce.getMessage());
			} catch (Exception ex) {
				nc2Asc.NC2Act.wrtMsg("getDemoData exception "+ex.getMessage());
			}

			dialog = new NC2AUIDiag(frm, true, demodata);
			dialog.setBounds(250, 150, 650, 400);
			dialog.setVisible(true);
		}
	} 

	void selAll(ActionEvent e){
		if (SELALL.equals(e.getActionCommand())) {
			if (cbSel.isSelected()){
				tbl.selectAll();
				//cbDesel.setSelected(false);
			}
		}
	}


	void process(ActionEvent e) {
		if (PROCESS.equals(e.getActionCommand())) {
			if (!validateOutFile(tfFileOut.getText())) {
				NC2Act.wrtMsg("Invalid output file...");
				return;
			}

			try {

				List<Variable>  lvars = ncdata.getVars();
				int size = 20;
				String[] alldata = new String[size];
				for (int i=0; i<10; i++){
					String[] vdata = ncdata.readVariableData( lvars.get(i), 0, size );
					for (int j=0; j<size; j++) {
						alldata[j] += vdata[j];
						if (j<(size-1)) {
							alldata[j] += "~";
						}
					}
				} 

				//wrt to file
				for (int i = 0; i<size; i++) {
					ncdata.getOutFile().write(alldata[i]);
				}

			} catch (IOException ioe) {
				NC2Act.wrtMsg(ioe.getMessage() );
			}
			catch (Exception ee) {
				NC2Act.wrtMsg(ee.getMessage() );
			}
		}
	}

	void createTbl() {
		model = new MyTblModel(3000, 4);
		tbl = new JTable(model);	
		tbl.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
		tbl.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		tbl.setAutoscrolls(true);
		String[] colHeaders = {"VarName","Units","OR","LongName"};
		int i=0;
		for (i=0; i<4;i++){
			tbl.getColumnModel().getColumn(i).setHeaderValue(colHeaders[i]);
		}
	}

	void populateTbl()  {
		if (ncdata==null) {
			ncdata = new NCData(tfFileIn.getText(), tfFileOut.getText());
			//	ncdata.openFile();
		}
		try {
			ncdata.openFile();
			ncdata.readDataInf();
			dataInf = ncdata.getDataInf();

		} catch ( ArrayIndexOutOfBoundsException a){
			nc2Asc.NC2Act.wrtMsg("Array index out of bound"+a.getMessage());
		} catch (NCDataException ee) {
			nc2Asc.NC2Act.wrtMsg("NCdata exception "+ee.getMessage());
		} catch (IOException nce){
			nc2Asc.NC2Act.wrtMsg("IO exception "+nce.getMessage());
		} catch (Exception ex) {
			nc2Asc.NC2Act.wrtMsg("Other exception "+ex.getMessage());
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
			String[] d = data.split(",");
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

	boolean validateInputFile(String fn){
		try {
			FileInputStream f = new FileInputStream(fn);
		} catch (Exception e) {
			nc2Asc.NC2Act.wrtMsg("Invalid Input File: "+fn);
			return false;
		}
		return true;
	}

	boolean validateOutFile(String fn){
		try {
			FileOutputStream f = new FileOutputStream(fn);
		} catch (Exception e) {
			nc2Asc.NC2Act.wrtMsg("Invalid Output File: "+fn+ "\n" + e);
			return false;
		}
		return true;
	}



}//eof class


class MyTblModel extends DefaultTableModel {
	MyTblModel(int rows, int cols) {
		super(rows, cols);
	}

	public void addRow(Object[] rowData) {
		super.addRow(rowData);
	}
}