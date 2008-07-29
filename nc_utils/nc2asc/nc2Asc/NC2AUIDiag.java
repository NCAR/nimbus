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

	private final static String MISSVAL = "-32768";
	private final static String CBDATE = "CBDATE";
	private final static String CBTM = "CBTM";
	private final static String RDMTR = "RDMTR";
	private final static String RDMTR2 = "RDMTR2";
	private final static String RVAL = "RVAL";
	private final static String RVAL2 = "RVAL2";
	private final static String RTMSET = "RTMSET";
	private final static String RTMSET2 = "RTMSET2";
	private final static String RHEAD = "RHEAD";
	private final static String RHEAD2 = "RHEAD2";
	private final static String RHEAD3 = "RHEAD3";
	private final static String TMSETVAL = "99:99:99";
//	private final static int HMSMAX = 485959;

	private DataFmt  dFormat = new DataFmt(); //data format chosen by author
	private String[] cbDateTxt, cbTmTxt;
	private String[] dDisp   = new String[10]; //make up data for display
	private String[] dCp     = new String[10]; // a copy of display data during the format selection
	private JPanel 			jpTm, jpDmtr, jpVal, jpTmSet, jpHead, jpSmpRate;
	private JRadioButton 	rDmtr, rDmtr2, rVal, rVal2, rTmSet, rTmSet2, rHead, rHead2, rHead3;
	private JComboBox    	cbDate, cbTm;
	private JTextField 		tfTmSet, tfTmSet2, tfSmpRate; 
	private JTextArea 		tfDisp; //JScrollPane sp;	
	private JLabel       	lblStart, lblEnd;

	public NC2AUIDiag(Frame owner, boolean modal) {
		super(owner, modal);
		JDialog.setDefaultLookAndFeelDecorated(true);

		init();
	}

	public NC2AUIDiag(Frame owner, boolean modal, String[] initData) {
		super(owner, modal);
		JDialog.setDefaultLookAndFeelDecorated(true);
		dDisp=initData;
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

		jpTm  = new JPanel();
		jpTm.setLayout(new GridLayout(0,1));

		Border bdTm = BorderFactory.createTitledBorder(" Date-Time Format  ");   
		jpTm.setBorder(bdTm);
		cbDateTxt = new String[3];
		cbDateTxt[0]=DataFmt.DATEDASH.toString();
		cbDateTxt[1]=DataFmt.DATESPACE.toString();
		//cbDateTxt[2]=DataFmt.DATESPACEMON.toString();
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
		jpHead  = new JPanel();
		//	jpHead.setFont(Systems, 7);
		jpHead.setLayout(new GridLayout(0,1));

		Border bdHead = BorderFactory.createTitledBorder("  Head ");
		jpHead.setBorder(bdHead);

		rHead = new JRadioButton(" None");
		jpHead.add(rHead);
		rHead.setSelected(true);
		addButtonActCmd(rHead, RHEAD.toString());

		rHead2 = new JRadioButton(" Icartt    "); 
		jpHead.add(rHead2);
		addButtonActCmd(rHead2, RHEAD2.toString());

		rHead3 = new JRadioButton(" Xml "); 
		jpHead.add(rHead3);
		addButtonActCmd(rHead3, RHEAD3.toString());

		jpHead.add(createPaneSR());
		return jpHead;
	}

	public JPanel createPaneSR() {
		jpSmpRate = new JPanel();
		jpSmpRate.setLayout(new GridLayout(0,2));
		tfSmpRate = new JTextField("1");
		JLabel l = new JLabel("Avg: ");
		jpSmpRate.add(l);
		jpSmpRate.add(tfSmpRate);
		addSmpRateFocusListener();

		return jpSmpRate;
	}

	public JPanel createPaneDmtr() {
		jpDmtr  = new JPanel();
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
		jpVal  = new JPanel();
		jpVal.setLayout(new GridLayout(0,1));

		Border bdVal = BorderFactory.createTitledBorder("  Missing Value ");
		jpVal.setBorder(bdVal);

		rVal = new JRadioButton("Fill Value");
		jpVal.add(rVal);
		rVal.setSelected(true);
		addButtonActCmd(rVal, RVAL.toString());

		rVal2 = new JRadioButton("Leave Blank"); 
		jpVal.add(rVal2);
		addButtonActCmd(rVal2, RVAL2.toString());

		return jpVal;
	}

	public JPanel createPaneTmSet() {

		jpTmSet  = new JPanel();
		jpTmSet.setLayout(new GridLayout(0,1));

		rTmSet = new JRadioButton("Full"); 
		rTmSet.setSelected(true);
		jpTmSet.add(rTmSet);

		rTmSet2 = new JRadioButton("Partial Scale"); 
		jpTmSet.add(rTmSet2);

		tfTmSet = new JTextField(TMSETVAL.toString());
		tfTmSet.setSize(12,8);
		tfTmSet.setEnabled(false); 
		JPanel jp = new JPanel();
		jp.setLayout(new GridLayout(0,2));
		lblStart = new JLabel("Start"); lblStart.setEnabled(false);
		jp.add(lblStart);
		jp.add(tfTmSet);
		jpTmSet.add(jp);

		tfTmSet2 = new JTextField(TMSETVAL.toString());
		tfTmSet2.setSize(12,8);
		tfTmSet2.setEnabled(false);
		jp = new JPanel();
		jp.setLayout(new GridLayout(0,2));
		lblEnd = new JLabel("End"); lblEnd.setEnabled(false);
		jp.add(lblEnd); 
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
			dDisp[0]="Date,UTC,A1DC_LWO,A2DC_LWO";
			String s1 = "2007-5-16,23:44:";
			String s2 = ","+""+",0.0";
			int sec = 17;
			for (int i=1; i<10; i++) {
				sec++;
				dDisp[i]=s1+sec+s2;
			}
		}


		//for (int i=0; i<dDisp.length; i++){
		//disp += dDisp[i]+ "\n";
		//dCp[i]= dDisp[i];
		//}
		disp= reFmtDisp(dDisp);
		tfDisp = new JTextArea(disp);
		tfDisp.setPreferredSize(new Dimension(500, 600));
		tfDisp.setLineWrap(true);
		tfDisp.setEditable(false);
		tfDisp.setFont(new Font("System", Font.ITALIC,  16));
		tfDisp.setVisible(true);
		return tfDisp;
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

			//add head
			c.gridx = 0;
			c.gridy = 0;
			this.add(createPaneHead(), c); 

			//add date-time
			c.gridx = 1;
			c.gridy = 0;
			this.add(createPaneTm(), c); 


			//add dlimiter
			c.gridx = 2;
			this.add(createPaneDmtr(), c);

			//add missing val
			c.gridx = 3; 
			this.add(createPaneVal(), c);

			//add time scope
			c.gridx = 4;
			this.add(createPaneTmSet(), c);

			//add txt display
			c.gridx = 0;
			c.gridy = 1;
			c.gridheight= 20;
			c.gridwidth= c.REMAINDER;
			this.add(createPaneTxtArea(), c);
		} catch (Exception e) {
			nc2Asc.NC2Act.wrtMsg(e.getMessage());
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
				selectTmSetFrmt(e);
				selectTmSetFrmt2(e);
				selectHead(e);
				selectHead2(e);
				selectHead3(e);
			}
		});
	}

	void addTmSetFocusListener() {
		tfTmSet.addFocusListener ( new FocusListener() {
			public void focusGained(FocusEvent e) {};
			public void focusLost(FocusEvent e) {
				chkTxtField(tfTmSet);
			}
		});
	}

	void addTmSet2FocusListener() {
		tfTmSet2.addFocusListener ( new FocusListener() {
			public void focusGained(FocusEvent e) {};
			public void focusLost(FocusEvent e) {

				//chk field
				if (chkTxtField(tfTmSet2)<0) {
					return;
				}
				//chk end and start
				if (tfTmSet2.getText().compareTo(tfTmSet.getText())<= 0) {
					NC2Act.wrtMsg("TimeEnd should be bigger than TimeStart...");
					tfTmSet2.setText(TMSETVAL.toString()); 	
					return;
				}
				// chk display data
				resetDisplayTm(dDisp);
			}
		});
	}


	private int chkTxtField(JTextField tf) {
		String[] tmset = tf.getText().split(":");
		try {
			String str = tmset[0]+tmset[1]+tmset[2];
			int in = (new  Integer(str)).intValue();
			if ((new  Integer(tmset[0])).intValue()>48){
				return resetTmSetTf(tf, " Max hh= 48");
			}
			if ((new  Integer(tmset[1])).intValue()>59){
				return resetTmSetTf(tf," Max mm= 59");
			}
			if ((new  Integer(tmset[2])).intValue()>59){
				NC2Act.wrtMsg(" Max ss= 59");
			}
			return in;
		} catch (Exception ne) {
			tf.setText(TMSETVAL.toString()); 
			return -1;
		}
	}

	private int resetTmSetTf(JTextField tf, String msg){
		NC2Act.wrtMsg(msg);
		tf.setText(TMSETVAL.toString());
		return -1;
	}

	void selectDateFrmt(ActionEvent e) {
		if (CBDATE.equals(e.getActionCommand())) {
			dFormat.setDataFmt(cbDateTxt[cbDate.getSelectedIndex()],DataFmt.DATE_IDX);
			tfDisp.setText(reFmtDisp(dDisp));	
		}
	}

	void selectTmFrmt(ActionEvent e) {
		if (CBTM.equals(e.getActionCommand())) {
			dFormat.setDataFmt(cbTmTxt[cbTm.getSelectedIndex()], DataFmt.TM_IDX);
			tfDisp.setText(reFmtDisp(dDisp));	
		}
	}

	void selectDmtrFrmt(ActionEvent e) {
		if (RDMTR.equals(e.getActionCommand())) {	
			if (rDmtr.isSelected()){
				rDmtr2.setSelected(false);
				dFormat.setDataFmt(",", DataFmt.DMTR_IDX);
				tfDisp.setText(reFmtDisp(dDisp));
			} else if (!rDmtr2.isSelected()) {
				rDmtr.setSelected(true);
			}
		}
	}

	void selectDmtrFrmt2(ActionEvent e) {
		if (RDMTR2.equals(e.getActionCommand())) {	
			if (rDmtr2.isSelected()){
				rDmtr.setSelected(false);
				dFormat.setDataFmt(" ", DataFmt.DMTR_IDX);
				tfDisp.setText(reFmtDisp(dDisp));
			}  else if (!rDmtr.isSelected()) {
				rDmtr2.setSelected(true);
			}
		}
	}


	void selectValFrmt(ActionEvent e) {
		if (RVAL.equals(e.getActionCommand())) {
			if (rVal.isSelected()){
				rVal2.setSelected(false);
				dFormat.setDataFmt(MISSVAL.toString(), DataFmt.MVAL_IDX);
				//nc2Asc.NC2Act.wrtMsg("miss value:"+ MISSVAL.toString());
				tfDisp.setText(reFmtDisp(dDisp));
			}  else if (!rVal2.isSelected()) {
				rVal.setSelected(true);
			}

		}
	}


	void selectValFrmt2(ActionEvent e) {
		if (RVAL2.equals(e.getActionCommand())) {	
			if (rVal2.isSelected()){
				rVal.setSelected(false);
				dFormat.setDataFmt("", DataFmt.MVAL_IDX);
				//nc2Asc.NC2Act.wrtMsg("miss value:"+ "");
				tfDisp.setText(reFmtDisp(dDisp));
			}	else if (!rVal.isSelected()) {
				rVal2.setSelected(true);
			}	
		}
	}

	void selectTmSetFrmt(ActionEvent e) {
		if (RTMSET.equals(e.getActionCommand())) {	
			if (rTmSet.isSelected()){
				rTmSet2.setSelected(false);
				tfTmSet.setEnabled(false);
				tfTmSet2.setEnabled(false);
				lblStart.setEnabled(false);
				lblEnd.setEnabled(false);
				dFormat.setDataFmt(DataFmt.FULLTM.toString(), DataFmt.TMSET_IDX);
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
				lblStart.setEnabled(true);
				lblEnd.setEnabled(true);
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
				dFormat.setDataFmt(DataFmt.HEAD.toString(), DataFmt.HEAD_IDX);
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
				dFormat.setDataFmt(DataFmt.HEAD2.toString(), DataFmt.HEAD_IDX);
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
				dFormat.setDataFmt(DataFmt.HEAD3.toString(), DataFmt.HEAD_IDX);
			} else if (!rHead.isSelected() && !rHead2.isSelected()){
				rHead3.setSelected(true);
			}
		}
	}

	void addSmpRateFocusListener()  {
		tfSmpRate.addFocusListener ( new FocusListener() {
			public void focusGained(FocusEvent e) {};
			public void focusLost(FocusEvent e) {
				try {
					String str= tfSmpRate.getText().trim();
					int in = (new  Integer(str)).intValue();
				} catch (Exception en) {
					tfSmpRate.setText("1");
				}

				dFormat.setDataFmt(tfSmpRate.getText(), DataFmt.AVG_IDX);
				//tfDisp.setText(resetAvgDisp(dDisp));
			}
		});
	}

	private String  reFmtDisp(String[] ddata) {
		String tmp = ddata[0]; boolean hasDate=true;
		for (int i =1; i< ddata.length; i++){
			//nc2Asc.NC2Act.wrtMsg(dDisp[i]);
			String[] d = ddata[i].split(",");
			try { 
				d[0]=dFormat.fmtDate(d[0].split("-"));
				d[1]=dFormat.fmtTm(d[1].split(":"));
			} catch (DataFormatException de) {
				return "Data Format Exception"+de.getMessage();
			}

			//check if the date=no-date
			if (d[0]=="") { 
				int len = d.length-1; // take d[0] out of the list
				String[] s = new String[len];
				for (int j=0; j<len; j++){
					s[j]=d[j+1];
				}
				tmp =tmp+"\n"+ dFormat.fmtDmtr(s);
				hasDate = false;
			} else {
				tmp =tmp+"\n"+ dFormat.fmtDmtr(d);
			}
		}

		if (!hasDate) {	tmp=tmp.substring(5); }
		return tmp;
	}

	private String resetAvgDisp(String[] ddata) {
		String ret = ddata[0]+ "\n";
		int startIdx =2;
		if (ddata[0].split(",")[0].equals("UTC") ){
			startIdx =1; 
			nc2Asc.NC2Act.wrtMsg("noDate:");
		}
		//nc2Asc.NC2Act.wrtMsg("noDate:"+noDate);
		int avg = new Integer(tfSmpRate.getText()).intValue();
		int rem = ddata.length%avg;
		int div = ddata.length/avg;

		//get total for dateach var
		double[] tot = new double [2];//only vars in our demo
		int count =0;
		for (int i=1; i<ddata.length; i++ ) {
			String[] data = ddata[i].split(",");
			if (count <div) {
				//nc2Asc.NC2Act.wrtMsg("data len:"+ data.length+ data[0] + " " + data[1]+ " " +data[2]+ " "+data[3]+ " first: "+ data[data.length-2] + " second: "+ data[data.length-1]);
				if (data[data.length-2]!=null && data[data.length-2].length()>0) {
					tot[0]+= new Double(data[data.length-2]).doubleValue();
				} else {
					tot[0] = -1;
				}
				if (data[data.length-1]!=null && data[data.length-1].length()>0) {
				    tot[1]+= new Double(data[data.length-1]).doubleValue();
				}  else {
					tot[1] = -1;
				}
				count ++;
			} else {
				//reset count
				count =0; 
				String tmp = new String();
				
				//get time
				try {
					if (startIdx ==2){
						tmp +=  dFormat.fmtDate(data[0].split("-")) + ",";
						tmp += dFormat.fmtTm(data[1].split(":")) + ",";
					} else {
						tmp += dFormat.fmtTm(data[0].split(":"))+ ",";
					}
				} catch (DataFormatException e) {
					nc2Asc.NC2Act.wrtMsg("data format catches exception...");
				}
				
				//add data 1
				if (tot[0]!=-1){
					tot[0] /=tot[0]/div;
					tmp += tot[0] + ",";
				} else {
					tmp +=",";
				}
				//add data 2
				if (tot[1]!=-1){
					tot[1] /=tot[1]/div;
					tmp += tot[1];
				} else {
					tmp+= "";
				}
				ret += dFormat.fmtDmtr(tmp.split(",")) + "\n";
			}
		}
		return ret;
	}

	private void resetDisplayTm(String[] ddata) {
		Calendar cl1 = Calendar.getInstance();
		Calendar cl2 = Calendar.getInstance();

		String[] dat = ddata[1].split(",")[0].split("-");
		String[] tm1 = ddata[1].split(",")[1].split(":");
		String[] tm2 = tfTmSet.getText().split(":");

		int y = new Integer(dat[0]).intValue();
		int m = new Integer(dat[1]).intValue();
		int d = new Integer(dat[2]).intValue();

		int h1 = new Integer(tm1[0]).intValue();
		int m1 = new Integer(tm1[1]).intValue();
		int s1 = new Integer(tm1[2]).intValue();

		int h2 = new Integer(tm2[0]).intValue();
		int m2 = new Integer(tm2[1]).intValue();
		int s2 = new Integer(tm2[2]).intValue();

		cl1.set(y,m,d,h1,m1,s1);
		cl2.set(y,m,d,h2,m2,s2);
		if (cl2.after(cl1)){
			for (int i=1;i<ddata.length; i++){
				String[] tmp = ddata[i].split(",");
				cl2.add(Calendar.SECOND, 1);
				tmp[1]= cl2.get(Calendar.HOUR_OF_DAY)+":"+cl2.get(Calendar.MINUTE)+":"+cl2.get(Calendar.SECOND);
				ddata[i]= tmp[0] +","+ tmp[1];
				for (int j =2; j<tmp.length; j++) {
					ddata[i] = ddata[i]+ ",";
					ddata[i] = ddata[i]+ tmp[j]; 
				}
			}
		}
		tfDisp.setText(reFmtDisp(ddata));
	}

} //eof class



