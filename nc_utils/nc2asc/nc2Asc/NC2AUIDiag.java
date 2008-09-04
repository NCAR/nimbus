
package nc2Asc;


import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.Border;
import java.io.*;
import java.util.zip.DataFormatException;
import java.util.*;

import nc2AscData.*;


class NC2AUIDiag extends JDialog {

	//assumed MISSVAL for all variables is -32768;
	//assumed the time variable contains the start-time in seconds since1970
	//and each record is 1 second apart from previous one

	private final static String CBDATE = "CBDATE";
	private final static String CBTM = "CBTM";
	private final static String RDMTR = "RDMTR";
	private final static String RDMTR2 = "RDMTR2";
	private final static String RVAL = "RVAL";
	private final static String RVAL2 = "RVAL2";
	private final static String RVAL3 = "RVAL3";
	private final static String RTMSET = "RTMSET";
	private final static String RTMSET2 = "RTMSET2";
	private final static String RHEAD = "RHEAD";
	private final static String RHEAD2 = "RHEAD2";
	private final static String RHEAD3 = "RHEAD3";

	private JRadioButton 	rDmtr, rDmtr2, rVal, rVal2, rVal3, rTmSet, rTmSet2, rHead, rHead2, rHead3;
	private JComboBox    	cbDate, cbTm;
	private JTextField 		tfTmSet, tfTmSet2, tfDateSet, tfDateSet2, tfSmpRate; 
	private JTextArea 		tfDisp; 

	private DataFmt  dFormat = new DataFmt(); //data format chosen by author
	private String[] cbDateTxt, cbTmTxt;
	private String[] dDisp   = new String[10]; //make up data for display

	private int  highRate;
	private int  size;
	private long tmStart;
	private String[] origTm= new String[4];


	public NC2AUIDiag(Frame owner, boolean modal) {
		super(owner, modal);
		JDialog.setDefaultLookAndFeelDecorated(true);

		init();
	}

	public NC2AUIDiag(Frame owner, boolean modal, String[] initData, long[] gDataInf) {
		super(owner, modal);
		JDialog.setDefaultLookAndFeelDecorated(true);
		this.setDefaultCloseOperation(this.DO_NOTHING_ON_CLOSE);

		dDisp=initData;
		highRate = (int)gDataInf[0];
		tmStart =   gDataInf[1];
		size   = (int)gDataInf[2];
		initTmSet();
		init();
	}
	public void setDispData(String[] dd){
		dDisp=dd;

		if (tfDisp==null){
			nc2Asc.NC2Act.wrtMsg("Text field in the format dialog is not created");
			return;
		}
		int i=0;
		for (i=0; i< dd.length; i++){
			//nc2Asc.NC2Act.wrtMsg("tfdisp data:"+dd[i]);
			tfDisp.insert(dd[i], i++);
		}
	}

	public DataFmt getDFormat() {
		return dFormat;
	}


	public JPanel createPaneTm() {

		JPanel jpTm  = new JPanel();
		jpTm.setLayout(new GridLayout(0,1));

		Border bdTm = BorderFactory.createTitledBorder(" Date-Time Format  ");   
		jpTm.setBorder(bdTm);
		cbDateTxt = new String[3];
		cbDateTxt[0]=DataFmt.DATEDASH.toString();
		cbDateTxt[1]=DataFmt.DATESPACE.toString();
		cbDateTxt[2]=DataFmt.NODATE.toString();
		cbDate = new JComboBox(cbDateTxt);
		cbDate.setActionCommand(CBDATE.toString());
		cbDate.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
				selectDateFrmt(e);
			}
		});
		jpTm.add(cbDate);

		cbTmTxt = new String[4];
		cbTmTxt[0]=DataFmt.TIMECOLON.toString();
		cbTmTxt[1]=DataFmt.TIMESPACE.toString();
		cbTmTxt[2]=DataFmt.TIMENOSPACE.toString();
		cbTmTxt[3]=DataFmt.TIMESEC.toString();
		cbTm = new JComboBox(cbTmTxt);
		jpTm.add(cbTm);
		cbTm.setActionCommand(CBTM.toString());
		cbTm.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
				selectTmFrmt(e);
			}
		});

		return jpTm;		
	}

	public JPanel createPaneHead() {
		JPanel jpHead  = new JPanel();
		//	jpHead.setFont(Systems, 7);
		jpHead.setLayout(new GridLayout(0,1));

		Border bdHead = BorderFactory.createTitledBorder("  Head ");
		jpHead.setBorder(bdHead);

		rHead = new JRadioButton(DataFmt.HEAD);
		jpHead.add(rHead);
		rHead.setSelected(true);
		addButtonActCmd(rHead, RHEAD.toString());

		rHead2 = new JRadioButton(DataFmt.HEAD2); 
		jpHead.add(rHead2);
		addButtonActCmd(rHead2, RHEAD2.toString());

		rHead3 = new JRadioButton(DataFmt.HEAD3); 
		jpHead.add(rHead3);
		addButtonActCmd(rHead3, RHEAD3.toString());

		jpHead.add(createPaneSR());
		return jpHead;
	}

	public JPanel createPaneSR() {
		JPanel jpSmpRate = new JPanel();
		jpSmpRate.setLayout(new GridLayout(0,2));
		tfSmpRate = new JTextField("1");
		JLabel l = new JLabel("Avg: ");
		jpSmpRate.add(l);
		jpSmpRate.add(tfSmpRate);
		tfSmpRate.addKeyListener( new KeyListener() {
			public void keyPressed(KeyEvent e) {};
			public void keyReleased(KeyEvent e) {
				String srStr= tfSmpRate.getText().trim();
				DataFmt.setDataFmt(srStr, DataFmt.AVG_IDX);
				tfDisp.setText(reFmtDisp());
			};
			public void keyTyped(KeyEvent e) {};
		});
		//addSmpRateFocusListener();
		return jpSmpRate;
	}

	public JPanel createPaneDmtr() {
		JPanel jpDmtr  = new JPanel();
		jpDmtr.setLayout(new GridLayout(0,1));
		//tfDisp.insert(dDisp[i], i);

		Border bdDmtr = BorderFactory.createTitledBorder("  Delimiter ");
		jpDmtr.setBorder(bdDmtr);

		rDmtr = new JRadioButton("Comma");
		jpDmtr.add(rDmtr);
		rDmtr.setSelected(true);
		addButtonActCmd(rDmtr, RDMTR.toString());

		rDmtr2 = new JRadioButton("Space"); 
		jpDmtr.add(rDmtr2);
		addButtonActCmd(rDmtr2, RDMTR2.toString());

		return jpDmtr;
	}

	public JPanel createPaneVal() {
		JPanel jpVal  = new JPanel();
		jpVal.setLayout(new GridLayout(0,1));

		Border bdVal = BorderFactory.createTitledBorder("Fill Value");
		jpVal.setBorder(bdVal);

		rVal = new JRadioButton(DataFmt.FILLVALUE.toString());
		jpVal.add(rVal);
		rVal.setSelected(true);
		addButtonActCmd(rVal, RVAL.toString());

		rVal2 = new JRadioButton(DataFmt.LEAVEBLANK.toString()); 
		jpVal.add(rVal2);
		addButtonActCmd(rVal2, RVAL2.toString());

		rVal3 = new JRadioButton(DataFmt.REPLICATE); 
		jpVal.add(rVal3);
		addButtonActCmd(rVal3, RVAL3.toString());

		return jpVal;
	}

	public JPanel createPaneTmSet() {

		JPanel jpTmSet  = new JPanel();
		jpTmSet.setLayout(new GridLayout(0,1));

		rTmSet = new JRadioButton("Full"); 
		rTmSet.setSelected(true);
		jpTmSet.add(rTmSet);

		rTmSet2 = new JRadioButton("Partial Scale"); 
		jpTmSet.add(rTmSet2);

		JPanel jp = new JPanel();
		jp.setLayout(new GridLayout(0,2));
		tfDateSet = new JTextField(origTm[0].toString());
		tfDateSet.setSize(12,8);
		tfDateSet.setEnabled(false); 
		tfTmSet = new JTextField(origTm[1].toString());
		tfTmSet.setSize(12,8);
		tfTmSet.setEnabled(false); 
		jp.add(tfDateSet);
		jp.add(tfTmSet);
		jpTmSet.add(jp);

		jp = new JPanel();
		jp.setLayout(new GridLayout(0,2));
		tfDateSet2 = new JTextField(origTm[2].toString());
		tfDateSet2.setSize(12,8);
		tfDateSet2.setEnabled(false); 
		tfTmSet2 = new JTextField(origTm[3].toString());		
		tfTmSet2.setSize(12,8);
		tfTmSet2.setEnabled(false);
		//lblEnd = new JLabel("End"); lblEnd.setEnabled(false);
		//jp.add(lblEnd); 
		//jp.add(tfTmSet2);
		jp.add(tfDateSet2);
		jp.add(tfTmSet2);
		jpTmSet.add(jp);
		Border bdDmtr = BorderFactory.createTitledBorder(" Time Scope ");
		jpTmSet.setBorder(bdDmtr);

		addButtonActCmd(rTmSet, RTMSET.toString());
		addButtonActCmd(rTmSet2, RTMSET2.toString());
		addTmSetFocusListener();
		addTmSet2FocusListener();
		return jpTmSet;
	}


	public JTextArea createPaneTxtArea() {

		String disp="";

		//if the demo data is empty, set defaults.
		if (dDisp[0]==null || dDisp[0].length()==0) {
			nc2Asc.NC2Act.wrtMsg("Input demo data is empty...");
			dDisp[0]="Date,UTC,A1DC_LWO,A2DC_LWO";
			String s1 = "2007-5-16,23:44:";
			String s2 = DataFmt.COMMAVAL.toString() + DataFmt.MISSVAL + DataFmt.COMMAVAL.toString()+",0.0";
			int sec = 17;
			for (int i=1; i<10; i++) {
				sec++;
				dDisp[i]=s1+sec+s2;
			}
		}

		disp= reFmtDisp();
		tfDisp = new JTextArea(disp);
		tfDisp.setPreferredSize(new Dimension(500, 600));
		tfDisp.setLineWrap(true);
		tfDisp.setEditable(false);
		tfDisp.setFont(new Font("System", Font.ITALIC,  16));
		tfDisp.setVisible(true);
		tfDisp.setBorder(BorderFactory.createLineBorder(Color.blue));
		return tfDisp;
	}


	private JPanel createButtons() {
		JButton bnOk = new JButton("Ok");
		JButton bnCancel = new JButton("Cancel");
		JPanel jp = new JPanel();
		jp.setLayout(new GridLayout(0,7));
		jp.add(new JLabel());
		jp.add(new JLabel());
		jp.add(new JLabel());
		jp.add(new JLabel());
		jp.add(new JLabel());
		jp.add(bnCancel);
		jp.add(bnOk);

		addButtonActCmd(bnOk, "ok");
		addButtonActCmd(bnCancel, "cancel");

		return jp;
	}

	private void init() {

		try {
			this.setTitle("         Data Format Dialog          ");
			this.setLayout(new GridBagLayout());
			GridBagConstraints c = new GridBagConstraints();
			c.fill = GridBagConstraints.HORIZONTAL;
			c.weighty = 1.0; 
			c.weightx = 0.5;
			c.insets= new Insets(15,0,0,0);
			c.anchor=c.NORTH;
			int i=0;

			//add head
			c.gridx = i++;
			c.gridy = 0;
			this.getContentPane().add(createPaneHead(), c); 

			//add date-time
			c.gridx = i++;
			c.gridy = 0;
			this.add(createPaneTm(), c); 

			//add dlimiter
			c.gridx = i++;
			this.add(createPaneDmtr(), c);

			//add missing val
			c.gridx = i++; 
			this.add(createPaneVal(), c);

			//add time scope
			c.gridx = i++;
			this.add(createPaneTmSet(), c);

			//add txt display
			c.gridx = 0;
			c.gridy = 1;
			c.gridheight= 1;
			c.gridwidth= c.REMAINDER;
			this.add(createPaneTxtArea(), c);

			//add cancel and ok buttons
			c.gridy = 2;
			c.gridwidth= c.REMAINDER;
			c.anchor = c.SOUTHEAST;
			this.add(createButtons(),c);

		} catch (Exception e) {
			nc2Asc.NC2Act.wrtMsg("Err in init_method: "+e.getMessage());
		}

		//chk high rate or low rate data to control options
		if (highRate>1) {
			tfSmpRate.setEnabled(false);
		} else {
			rVal3.setEnabled(false);
		}
	}


	/**
	 *  actions listener
	 */
	void addButtonActCmd(AbstractButton jb, String actStr){
		jb.setActionCommand(actStr);
		jb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e){
				selectDmtrFrmt(e);
				selectDmtrFrmt2(e);
				selectValFrmt(e);
				selectValFrmt2(e);
				selectValFrmt3(e);
				selectTmSetFrmt(e);
				selectTmSetFrmt2(e);
				selectHead(e);
				selectHead2(e);
				selectHead3(e);
				selectCancel(e);
				selectOk(e);

			}
		});
	}


	void addTmSetFocusListener() {
		tfTmSet.addFocusListener ( new FocusListener() {
			public void focusGained(FocusEvent e) {};
			public void focusLost(FocusEvent e) {
				chkStartTmField();
				tfDisp.setText(reFmtDisp());	
			}
		});
	}

	void addTmSet2FocusListener() {
		tfTmSet2.addFocusListener ( new FocusListener() {
			public void focusGained(FocusEvent e) {};
			public void focusLost(FocusEvent e) {
				chkEndTmField(); 
			}
		});
	}


	private void chkStartTmField() {
		String dt = tfDateSet.getText();
		String tm = tfTmSet.getText();
		long tmms= getNewTmMilSec(dt,tm, 0);

		if (tmms+1000 < tmStart){
			nc2Asc.NC2Act.wrtMsg("This time is earlier than the start-time: " + origTm[0]+ " "+ origTm[1]);
			tfDateSet.setText(origTm[0]);
			tfTmSet.setText(origTm[1]);
			return;
		}
	}


	private void chkEndTmField() {
		//new end-time
		String dt = tfDateSet2.getText();
		String tm = tfTmSet2.getText();

		long tmEnd = getNewTmMilSec(dt,tm, 1);
		if ((int)tmEnd/1000 > (int)(tmStart+size*1000)/1000){
			nc2Asc.NC2Act.wrtMsg("The time is later than the end-time: " + origTm[2]+ " "+ origTm[3]);
			tfDateSet2.setText(origTm[2]);
			tfTmSet2.setText(origTm[3]);
			return;
		}

		//compare with new-start 
		String dt0 = tfDateSet.getText();
		String tm0 = tfTmSet.getText();
		long tmStart= getNewTmMilSec(dt0,tm0, 0);
		if (tmStart > tmEnd){
			nc2Asc.NC2Act.wrtMsg("The end-time is less than the start-time: " );
			tfDateSet.setText(origTm[0]);
			tfTmSet.setText(origTm[1]);
			tfDateSet2.setText(origTm[2]);
			tfTmSet2.setText(origTm[3]);
			return;
		}
		//start-time(milsec) and range-seconds
		DataFmt.setDataFmt(tmStart+ DataFmt.TMSETDELIMIT+(int)((tmEnd-tmStart)/1000), DataFmt.TMSET_IDX);
	}


	private long getNewTmMilSec(String dt, String tm, int idx)  {

		String[] dts = dt.split("-");
		String[] tms = tm.split(":");
		if (dts.length!=3 || tms.length !=3) {
			nc2Asc.NC2Act.wrtMsg("Invalid Tm Data format...");
			if (idx ==0){
				tfDateSet.setText(origTm[0]);
				tfTmSet.setText(origTm[1]);
			} else {
				tfDateSet2.setText(origTm[2]);
				tfTmSet2.setText(origTm[3]);
			}
		}

		Calendar cl = Calendar.getInstance();
		int yy = Integer.parseInt(dts[0]);
		int mm = Integer.parseInt(dts[1]);
		int dd = Integer.parseInt(dts[2]);
		int h = Integer.parseInt(tms[0]);
		int m = Integer.parseInt(tms[1]);
		int s = Integer.parseInt(tms[2]);

		yy = 1900+yy;
		if (yy< 1950) {yy = 100 +yy;} //1950 -1999  or 2000- now

		cl.set(yy,mm,dd,h,m,s);
		return cl.getTimeInMillis();
	}

	void selectCancel(ActionEvent e){
		if ("cancel".equals(e.getActionCommand())) {
			dFormat.initDataFmt();
			//dFormat.showDataFmt();
			dispose();
		}
	}

	void selectOk(ActionEvent e){
		if ("ok".equals(e.getActionCommand())) {
			chkEndTmField();
			dispose();
		}
	}

	void selectDateFrmt(ActionEvent e) {
		if (CBDATE.equals(e.getActionCommand())) {
			DataFmt.setDataFmt(cbDateTxt[cbDate.getSelectedIndex()],DataFmt.DATE_IDX);
			tfDisp.setText(reFmtDisp());	
		}
	}

	void selectTmFrmt(ActionEvent e) {
		if (CBTM.equals(e.getActionCommand())) {
			DataFmt.setDataFmt(cbTmTxt[cbTm.getSelectedIndex()], DataFmt.TM_IDX);
			tfDisp.setText(reFmtDisp());	
		}
	}

	void selectDmtrFrmt(ActionEvent e) {
		if (RDMTR.equals(e.getActionCommand())) {	
			if (rDmtr.isSelected()){
				rDmtr2.setSelected(false);
				rVal2.setEnabled(true);
				DataFmt.setDataFmt(DataFmt.COMMAVAL.toString(), DataFmt.DMTR_IDX);
				tfDisp.setText(reFmtDisp());
			} else if (!rDmtr2.isSelected()) {
				DataFmt.setDataFmt(" ", DataFmt.DMTR_IDX);
				rDmtr.setSelected(true);
			}
		}
	}

	void selectDmtrFrmt2(ActionEvent e) {
		if (RDMTR2.equals(e.getActionCommand())) {	
			if (rDmtr2.isSelected()){
				rDmtr.setSelected(false);
				rVal2.setEnabled(false);
				rVal.setSelected(true);
				rVal2.setSelected(false);
				DataFmt.setDataFmt(" ", DataFmt.DMTR_IDX);
				DataFmt.setDataFmt(DataFmt.MISSVAL.toString(), DataFmt.MVAL_IDX);
				tfDisp.setText(reFmtDisp());
			}  else if (!rDmtr.isSelected()) {
				rDmtr2.setSelected(true);
			}
		}
	}


	void selectValFrmt(ActionEvent e) {
		if (RVAL.equals(e.getActionCommand())) {
			if (rVal.isSelected()){
				rVal2.setSelected(false);
				rVal3.setSelected(false);
				DataFmt.setDataFmt(DataFmt.MISSVAL, DataFmt.MVAL_IDX);
				tfDisp.setText(reFmtDisp());
			}  else if (!rVal2.isSelected()&& (!rVal3.isSelected())) {
				rVal.setSelected(true);
			}

		}
	}


	void selectValFrmt2(ActionEvent e) {
		if (RVAL2.equals(e.getActionCommand())) {	
			if (rVal2.isSelected()){
				rVal.setSelected(false);
				rVal3.setSelected(false);
				DataFmt.setDataFmt("", DataFmt.MVAL_IDX);
				tfDisp.setText(reFmtDisp());
			}	else if (!rVal.isSelected() && (!rVal3.isSelected())) {
				rVal2.setSelected(true);
			}	
		}
	}


	void selectValFrmt3(ActionEvent e) {
		if (RVAL3.equals(e.getActionCommand())) {	
			if (rVal3.isSelected()){
				rVal.setSelected(false);
				rVal2.setSelected(false);
				dFormat.setDataFmt(DataFmt.REPLICATE, DataFmt.MVAL_IDX);
				tfDisp.setText(reFmtDisp());
			}	else if (!rVal.isSelected() && (!rVal2.isSelected())) {
				rVal3.setSelected(true);
			}	
		}
	}

	void selectTmSetFrmt(ActionEvent e) {
		if (RTMSET.equals(e.getActionCommand())) {	
			if (rTmSet.isSelected()){
				rTmSet2.setSelected(false);
				tfTmSet.setEnabled(false);
				tfTmSet2.setEnabled(false);
				tfDateSet.setEnabled(false);
				tfDateSet2.setEnabled(false);
				//lblStart.setEnabled(false);
				//lblEnd.setEnabled(false);
				DataFmt.setDataFmt(DataFmt.FULLTM, DataFmt.TMSET_IDX);
			} else if (!rTmSet2.isSelected()) {
				rTmSet.setSelected(true);
			}
		}
	}

	void selectTmSetFrmt2(ActionEvent e) {
		if (RTMSET2.equals(e.getActionCommand())) {	
			if (rTmSet2.isSelected()){
				rTmSet.setSelected(false);
				tfTmSet.setEnabled(true);
				tfTmSet2.setEnabled(true);
				tfDateSet.setEnabled(true);
				tfDateSet2.setEnabled(true);
				//lblStart.setEnabled(true);
				//lblEnd.setEnabled(true);
				//DataFmt.setDataFmt(tfTmSet.getText()+DataFmt.TMSETDELIMIT+tfTmSet2.getText(), DataFmt.TMSET_IDX);
			} else if (!rTmSet.isSelected()) {
				rTmSet2.setSelected(true);
			}
		}
	}

	void selectHead(ActionEvent e){
		if (RHEAD.equals(e.getActionCommand())) {	
			if (rHead.isSelected()){
				rHead2.setSelected(false);
				rHead3.setSelected(false);
				//dForamt[5]=RHEAD.toString();
				DataFmt.setDataFmt(DataFmt.HEAD.toString(), DataFmt.HEAD_IDX);
			} else if (!rHead2.isSelected() && !rHead3.isSelected()){
				rHead.setSelected(true);
			}

		}
	}

	void selectHead2(ActionEvent e){
		if (RHEAD2.equals(e.getActionCommand())) {	
			if (rHead2.isSelected()){
				rHead.setSelected(false);
				rHead3.setSelected(false);
				//dForamt[5]=RHEAD.toString();
				DataFmt.setDataFmt(DataFmt.HEAD2.toString(), DataFmt.HEAD_IDX);
			} else if (!rHead.isSelected() && !rHead3.isSelected()){
				rHead2.setSelected(true);
			}
		}
	}

	void selectHead3(ActionEvent e){
		if (RHEAD3.equals(e.getActionCommand())) {	
			if (rHead3.isSelected()){
				rHead.setSelected(false);
				rHead2.setSelected(false);
				DataFmt.setDataFmt(DataFmt.HEAD3.toString(), DataFmt.HEAD_IDX);
			} else if (!rHead.isSelected() && !rHead2.isSelected()){
				rHead3.setSelected(true);
			}
		}
	}

	void addSmpRateFocusListener()  {
		tfSmpRate.addFocusListener ( new FocusListener() {
			public void focusGained(FocusEvent e) {};
			public void focusLost(FocusEvent e) {
				String srStr="";
				try {
					srStr= tfSmpRate.getText().trim();
				} catch (Exception en) {
					tfSmpRate.setText("1");
				}
				DataFmt.setDataFmt(srStr, DataFmt.AVG_IDX);
				//if (Integer.parseInt(srStr)>1) {
				tfDisp.setText(reFmtDisp());
				//}
			}
		});
	}



	private String  reFmtDisp() {
		String[] ddata= new String[10];

		//check new start time
		if (tfDateSet.isEnabled()) {
			ddata[0]="Date,UTC,A1DC_LWO,A2DC_LWO";
			String[] tt = tfTmSet.getText().split(":");
			String s1 = tfDateSet.getText()+DataFmt.COMMAVAL.toString()+ tt[0]+":"+tt[1]+":";
			//add 19 0r 20
			if (Integer.parseInt(s1.split("-")[0])< 50) {
				s1="20"+s1;
			} else {
				s1 +="19"+s1;
			}
			String s2 = DataFmt.COMMAVAL.toString() + ",0.0";
			int sec = Integer.parseInt(tt[2]);
			for (int i=1; i<10; i++) {
				ddata[i]=s1+sec+s2;
				sec++;
			}
		} else {
			ddata = dDisp;
		}

		//check avg
		if ( tfSmpRate.isEnabled() && (Integer.parseInt(tfSmpRate.getText().trim())>1) ){
			return resetAvgDisp(ddata);
		}

		//handle date/time
		String tmp = ddata[0];
		tmp = dFormat.fmtDmtr(tmp) + "\n";

		// handle date/time and two demo data-items
		for (int i =1; i< ddata.length; i++){

			String[] d = ddata[i].split(DataFmt.COMMAVAL.toString());
			try { 
				d[0]=dFormat.fmtDate(d[0].split("-"));
				d[1]=dFormat.fmtTm(d[1].split(":"));
			} catch (DataFormatException de) {
				return "Data Format Exception"+de.getMessage();
			}

			String onedata = dFormat.fmtDmtr(d);
			tmp +=onedata+ "\n";
		}
		return tmp;
	}

	private String resetAvgDisp(String[] ddata) {
		int avg = Integer.parseInt(tfSmpRate.getText().trim());
		int loop = ddata.length;
		int len =  (int)loop/avg;

		String ret =   dFormat.fmtDmtr(ddata[0]);
		for (int i =0; i<len; i++) {
			String[] one = ddata[i*avg+1].split(DataFmt.COMMAVAL);

			//adjust time
			long ms = getNewTmMilSec(one[0], one[1],0);
			long tot = 0;
			for (int j=0; j<avg; j++) {
				tot = tot+ ms + j*1000;
			}
			tot = tot/avg;
			Calendar cl = Calendar.getInstance();
			cl.setTimeInMillis(tot);

			//get new time 
			one[0]=  cl.get(Calendar.YEAR)+ "-"+ cl.get(Calendar.MONTH) + "-"+cl.get(Calendar.DAY_OF_MONTH);
			one[1]=  cl.get(Calendar.HOUR_OF_DAY)+ ":"+ cl.get(Calendar.MINUTE) + ":"+cl.get(Calendar.SECOND)+"."+cl.get(Calendar.MILLISECOND);

			try {
				one[0] = dFormat.fmtDate(one[0]);
				one[1] = dFormat.fmtTm(one[1]);
			} catch (DataFormatException e) {
				nc2Asc.NC2Act.wrtMsg("resetAvgDisp_fmtDate_fmtTm exception...");
			}
			ret += "\n" + dFormat.fmtDmtr(one);
		}
		
		//add extra empty lines
		for (int i=0; i<loop-len; i++){
			ret += " \n";
		}
		return ret;
	}



	void initTmSet() {
		Calendar cl = Calendar.getInstance();
		cl.setTimeInMillis(tmStart);

		origTm[0] = cl.get(Calendar.YEAR)+ "-"+ cl.get(Calendar.MONTH)+"-"+cl.get(Calendar.DAY_OF_MONTH);
		origTm[0] = origTm[0].substring(2);//chop off the first 2 chars 19 or 20
		origTm[1] = cl.get(Calendar.HOUR_OF_DAY )+ ":"+ cl.get(Calendar.MINUTE)+":"+cl.get(Calendar.SECOND);

		cl.add(Calendar.SECOND, size);
		origTm[2] = cl.get(Calendar.YEAR)+ "-"+ cl.get(Calendar.MONTH)+"-"+cl.get(Calendar.DAY_OF_MONTH);
		origTm[2] = origTm[2].substring(2);//chop off the first 2 chars 19 or 20
		origTm[3] = cl.get(Calendar.HOUR_OF_DAY )+ ":"+ cl.get(Calendar.MINUTE)+":"+cl.get(Calendar.SECOND);
	}

} //eof class



