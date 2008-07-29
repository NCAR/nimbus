package nc2Asc;

import java.lang.*;
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

import nc2AscData.*;

public class NC2AUI {

	private final static String  INFILE = "INFILE";
	private final static String  OUTFILE = "OUTFILE";
	private final static String  DATAFORMAT = "DATAFORMAT";
	private final static String  PROCESS = "PROCESS";

	private JPanel jp, jpTbl, jpBt; GridBagConstraints c1, c2;
	private JTable tbl;  MyTblModel model;

	private JScrollPane tblScrlPane;
	private JButton inButton, outButton, dataButton, fmtButton, procButton;
	private JTextField inFile, outFile;

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
			c.insets = new Insets(45, 0, 0, 0); // top paddingreadData
			pane.add(createUIBtns(), c);
		} catch (Exception e) {
			NC2Act.prtMsgBox(e.getMessage());
		}

	}

	public JPanel createUIIOFields() throws Exception {
		try{
			jp  = new JPanel();
			jp.setLayout(new GridBagLayout());

			inButton  = new JButton("Select Input File");
			outButton = new JButton("Select Output Path");
			addButtonActCmd(inButton, INFILE.toString());
			addButtonActCmd(outButton, OUTFILE.toString());

			inFile = new JTextField("/home/data/pac10.nc");
			inFile.setBackground(Color.LIGHT_GRAY);
			inFile.setEditable(false);
			inFile.setFont(new Font("System", Font.ITALIC,  16));
			outFile =new JTextField("/home/data/default.txt");
			outFile.setFont(new Font("System", Font.ITALIC,  16));

			c1 = new GridBagConstraints();
			c1.fill = GridBagConstraints.HORIZONTAL;
			c1.weighty = 1.0; 
			c1.weightx = 0.5;

			//row-one   input-file
			c1.gridx = 0;
			c1.gridy = 0;
			jp.add(inButton,c1);

			c1.gridx = 1;
			c1.gridy = 0;
			c1.ipadx = 400;
			c1.gridwidth = c1.REMAINDER;
			jp.add(inFile,c1);

			c1.gridx =0;
			c1.gridy =1; 	
			c1.ipadx=0;
			c1.gridwidth=1;
			jp.add(outButton,c1);

			c1.gridx = 1;
			c1.gridy = 1;
			c1.gridwidth = c1.REMAINDER;
			jp.add(outFile, c1);

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

			dataButton = new JButton("Data Variables");
			c2.gridx = 1;
			c2.gridy = 0;
			c2.gridwidth = 2;//c2.REMAINDER;
			c2.gridheight = 1;
			c2.anchor = c2.CENTER;
			jpTbl.add(dataButton, c2); 

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
		fmtButton = new JButton("Select Data Format");
		procButton  = new JButton(" Process");
		addButtonActCmd(fmtButton, DATAFORMAT.toString());
		addButtonActCmd(procButton, PROCESS.toString());

		try{
			jpBt = new JPanel();
			jpBt.setLayout(new GridLayout(4, 1));
			jpBt.add(fmtButton,0);
			jpBt.add(procButton,1);

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
				selectDataFormat(e);
				process(e);
			}
		});
	}

	void selectInputFile(ActionEvent e) {
		if (INFILE.equals(e.getActionCommand())) {	
			String[] strs = {"nc"};
			FileNameExtensionFilter filter = new FileNameExtensionFilter(null, strs);
			JFileChooser fileChooser = new JFileChooser(inFile.getText().trim().toString());
			fileChooser.addChoosableFileFilter(filter);

			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				File selectedFile = fileChooser.getSelectedFile();
				if (selectedFile.isFile()) {
					inFile.setText(selectedFile.getAbsolutePath());//(selectedFile.getName());
				}
				else {
					inFile.setText("Invalid file");
				}
			}
		}
	}  

	void selectOutputFile(ActionEvent e) {
		if (OUTFILE.equals(e.getActionCommand())) {	
			JFileChooser fileChooser = new JFileChooser(outFile.getText().trim().toString());
			fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

			int returnValue = fileChooser.showOpenDialog(null);
			if (returnValue == JFileChooser.APPROVE_OPTION) {
				String selectedPath=fileChooser.getSelectedFile().getAbsolutePath();

				if (!selectedPath.isEmpty()) {
					outFile.setText(selectedPath+"/");
				}
				else {
					outFile.setText("Empty file path");
				}
			}
		}
	} 

	void selectDataFormat(ActionEvent e) {

		if (DATAFORMAT.equals(e.getActionCommand())) {
			if (!validateInputFile(inFile.getText()) || (!validateOutFile(outFile.getText()))) {
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
				nc2Asc.NC2Act.wrtMsg("getDemotData Array index out of bound"+a.getMessage());
			} catch (NCDataException ee) {
				nc2Asc.NC2Act.wrtMsg("getDemoData NCData exception "+ee.getMessage());
			} catch (IOException nce){
				nc2Asc.NC2Act.wrtMsg("getDemoData IO exception "+nce.getMessage());
			} catch (Exception ex) {
				nc2Asc.NC2Act.wrtMsg("getDemoData exception "+ex.getMessage());
			}
				
			
			dialog = new NC2AUIDiag(frm, true, demodata);

			//dialog.setLocation(250, 150);
			dialog.setBounds(250, 150, 650, 400);
			dialog.setVisible(true);

			NC2Act.wrtMsg(dialog.getDFormat().getDataFmt()[0]);
		}
	} 

	void process(ActionEvent e) {
		if (PROCESS.equals(e.getActionCommand())) {
			if (!validateInputFile(inFile.getText()) || (!validateOutFile(outFile.getText()))) {
				return;
			}
			nc2Asc.NC2Act.startWaitCursor(dataButton);
			//createTbl();
			populateTbl();
			nc2Asc.NC2Act.stopWaitCursor(dataButton);
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
			ncdata = new NCData(inFile.getText(), outFile.getText());
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
			
		//get time data
		//timeData =dataInf[0];

		// set data
		for (int i=1; i<dataInf.length; i++){
			String data =dataInf[i];

			String[] d = data.split("~");
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