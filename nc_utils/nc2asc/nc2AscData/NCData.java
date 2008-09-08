package nc2AscData;

import java.lang.*;
import java.lang.Exception.*;
import java.io.*;
import java.io.FileWriter;
import java.util.*;

import ucar.nc2.*;
import ucar.ma2.*;

/**
 * This class is to use netCDF APIs to read and write 
 * netCDF data from files and format the data into 
 * desirable ASCII formats
 * @author dongl
 *
 */

public class NCData {

	//constructor
	public NCData(String ifn, String ofn) {infn = ifn; outfn=ofn;}
	public NCData(String ifn) {infn = ifn;}
	public NCData() {}

	/**
	 * Input file name and output file name
	 */
	private String infn, outfn;

	/**
	 *  Netcdf file pointer
	 */
	private NetcdfFile  fin=null;

	/**
	 * Ascii output file pointer
	 */
	private FileWriter  fout=null;

	/**
	 * List of all the variable from the input netcdf file
	 */
	private List<Variable> lvars=null;

	/**
	 * All the variables' name, units, OR/Len displayed in the table for users--include time-var
	 */	
	private List<String> dataInf;

	/**
	 * Status of the process to retrieve and write data to a file
	 */
	private boolean bfinish;

	/**
	 * Time needed to process the data (in mili seconds)
	 */ 
	private long tmPassed;

	/**
	 * Progress status displayed on the status bar for end users 
	 */
	private int progIdx =0; 

	/**
	 * All the selected variables' length. These are the variables on the output file 
	 */ 
	private int totVarLen;

	/**
	 * the highest output rate in mixed high rate data
	 */
	//private int topRate;

	/**
	 * All selected variables' 1D array float data
	 */
	private float[][] data;
	
	/**
	 * The represent of each variable's miss value
	 */
	private float[] missVal;
	
	/**
	 * Each variable's length and rate from netcdf file.
	 */
	private int[] oneDLen, hRate;

	/**
	 * 	Global information about the netcdf file
	 *  idx 0--high rate data
	 *  idx 1--start time in milli second
	 *  idx 2--size of the records
	 */
	private long[] gDataInf = {1,0,0};  
	
	/**
	 * batch mode 
	 */
	private boolean bMode = false;
	
	public void setMode (boolean b) { bMode = b;}

	/**
	 * The index out of the total counts
	 * The total counts are made of the number of all selected variables and total time range in seconds 
	 * @return - The current index of total counts
	 */
	public int getProgIdx() {return progIdx;}

	
	/**
	 * The status of the data retrieving and writing to an ascii file.
	 * @return - finished or not
	 */
	public boolean  getFinish(){return bfinish;}
	
	
	/**
	 * 
	 * @param finished - Set "true" to stop the data process, if a user chooses to cancel the data process from UI 
	 */
	public void  setFinish(boolean finished){ bfinish = finished;}

	/**
	 * This is the time used to retrieve and write to a file 
	 * @return -- the total time passed in milseconds
	 */
	public long getTmPassed() {	return tmPassed;}

	/**
	 * Get the variables' list from the netcdf file
	 * @return
	 */
	public List<Variable> getVars(){return lvars;}



	/**
	 * 
	 * Get the data information such as variable name, units, OR, etc.
	 * Usually, it is the all the variable in a netcdf file.
	 * @return -- pointer to data information  
	 *  
	 */
	public List<String> getDataInf() {
		return dataInf;
	}

	/**
	 *  Get the global information of the netcdf file
	 */
	public long[] getGlobalDataInf() {
		return gDataInf;
	}

	/**
	 * program calls this method to clean up 
	 */
	protected void finalize() {
		if (fin!=null)  {try {fin.close();} catch (IOException e) {}}
		if (fout!=null) {try {fout.close();} catch (IOException e) {}}
	}

	/**
	 * 
	 * @return -- file pointer to access the netcdf file
	 */
	public NetcdfFile getInFile() {
		return fin;
	}

	/**
	 * 
	 * @return -- file pointer to write output stream to the output file
	 */
	public FileWriter getOutFile() {
		return fout;
	}


	/**
	 * setFileName provides the users with ability to set up netcdf read-only input file name 
	 * 
	 */
	public void setInputFileName(String ifn){infn=ifn;}



	/**
	 * openFile is to open a netcdf read-only file and a writable output file.
	 * Since we need both files to operate, we open them at the same time.
	 */	
	public void openFile(String fn) throws IOException {
		//open netcdf readonly
		infn = fn;
		try{
			fin = NetcdfFile.open(infn);
		} catch (IOException e){
			if (fin !=null){ 
				fin.close();
				fin = null;
			}
			throw e;
		} 

	}

	/**
	 * Open a output file to write data
	 * @param fn -- output file name 
	 * @throws IOException
	 */
	public void openOutFile(String fn) throws IOException {
		outfn = fn;

		//check file exits
		fout = new FileWriter(fn);

	}

	/**
	 * Retrieve attribute information such as name, unit, OR/len, etc for the variables-time var is the first one
	 * 
	 * @throws NCDataException ArrayIndexOutOfBoundsException InvalidRangeException IOException
	 * @throws 
	 */
	public void readDataInf() throws NCDataException, ArrayIndexOutOfBoundsException, InvalidRangeException, IOException{			

		if (fin == null) { 
			throw new NCDataException("readDataInf: Empty input file exception:");
		}

		lvars = new ArrayList<Variable>();
		List<Variable> tmp =  new ArrayList<Variable>();
		tmp = fin.getVariables();
		checklvars(tmp, lvars); //check the time-base and time_offset && sign valid vars to lvars
		int len = lvars.size();
		gDataInf[0] =1;   
		dataInf = new ArrayList<String>();	
		for (int i=0; i<len ; i++){ 
			String dat = "";
			Variable v = lvars.get(i);
			dat += v.getShortName() + DataFmt.COMMAVAL;
			dat += v.getUnitsString() + DataFmt.COMMAVAL;

			int or = getOR(v); if (or>gDataInf[0]) { gDataInf[0]= or;}
			dat += or+"/"+getLen(v) + DataFmt.COMMAVAL ;
			
			String lname = ""+v.findAttribute("long_name");  //getStringValue()  -bugs
			lname = lname.substring(lname.indexOf('\"')+1);  //take off the first "
			int idx = lname.indexOf('\"');  //takeoff the second "
			if (idx == 0 && lname.length()==1) {
				dat += " ";
			}
			if (idx >0 ) {
				dat += lname = lname.substring(0, idx);
			}
			dataInf.add(i,dat);

		}
		//init global data info
		gDataInf[1] = getTimeMilSec(); 
		gDataInf[2] = fin.getUnlimitedDimension().getLength();
	}


	/**
	 * Retrieve all the data in the given range for a variable
	 * @param v 	-- the variable to read data
	 * @param start -- starting index to retrieve data
	 * @param len	-- how many data records (representing time in seconds)  
	 * @throws InvalidRangeException
	 * @throws IOException
	 */
	public float[] read1DData (Variable v, int start, int len) throws InvalidRangeException, IOException {
		//set orig and size
		int[] shape = v.getShape();
		int[] origin=new int[shape.length], size=new int[shape.length];

		origin[0] =  start;
		size[0] = len;
		for (int i=1; i<shape.length; i++) {
			origin[i]=0;
			size[i]  =shape[i];
		}		

		Array data = v.read(origin, size);
		data=data.reduce();
		return  (float [])data.copyTo1DJavaArray();
	}

	/**
	 * 
	 * @param variable
	 * @return
	 */
	private int getOR(Variable v) {
		int[] dims = v.getShape();
		if (dims.length==1){
			return 1;
		}
		return dims[1];
	}

	public int getLen(Variable v) {
		int[] dims = v.getShape();
		if (dims.length<3){
			return 1;
		}
		return dims[2];
	}

	public String[] getDemoData() throws  NCDataException, InvalidRangeException, IOException {
		String[] demoData = new String[10];

		if (lvars==null || lvars.size()==0) {
			throw new NCDataException("getDemoData encounts empty variable list.");
		}
		// get 10 example data
		Variable v1 = lvars.get(1);
		Variable v2 = lvars.get(2);
		demoData[0] ="Date,UTC,"+v1.getShortName() +","+v2.getShortName();
		long milSec;
		String[] vdata1, vdata2;
		milSec = getTimeMilSec();    

		int len =10;
		for (int i=1; i<len; i++) {
			demoData[i]= getNewTm(milSec, i)+ DataFmt.COMMAVAL 
			+ DataFmt.MISSVAL + DataFmt.COMMAVAL
			+ "0.0";
		}
		return demoData;
	}

	public long getTimeMilSec() throws NCDataException {
		if (dataInf.get(0)== null) {
			throw new  NCDataException("getTimeMilSec: Variables are not read... Please get variables from the netcdf file.");
		}
		//nc2Asc.NC2Act.wrtMsg(dataInf[0]);
		String tmVar= dataInf.get(0);
		String date = tmVar.split(DataFmt.COMMAVAL)[1].split(" ")[2];
		String tm   = tmVar.split(DataFmt.COMMAVAL)[1].split(" ")[3];
		String[] dInf = date.split("-");
		String[] tmInf   = tm.split(":");

		Calendar cl = Calendar.getInstance();
		int y= Integer.parseInt(dInf[0]);
		int mm= Integer.parseInt(dInf[1]);
		int d= Integer.parseInt(dInf[2]);

		int h= Integer.parseInt(tmInf[0]);
		int m= Integer.parseInt(tmInf[1]);
		int s= Integer.parseInt(tmInf[2]);

		cl.set(y,mm,d,h,m,s);
		return cl.getTimeInMillis();
	}


	public long getNewTimeMilSec(String hms) throws NCDataException {
		//long ms=0;
		String[] tmInf = hms.split(":");

		Calendar cl = Calendar.getInstance();
		cl.setTimeInMillis(getTimeMilSec());
		int y =cl.get(Calendar.YEAR);
		int mm =cl.get(Calendar.MONTH);
		int d =cl.get(Calendar.DAY_OF_MONTH);

		int h= Integer.parseInt(tmInf[0]);
		int m= Integer.parseInt(tmInf[1]);
		int s= Integer.parseInt(tmInf[2]);

		Calendar c2 = Calendar.getInstance();
		c2.set(y,mm,d,h,m,s);

		return c2.getTimeInMillis();
	}

	public String  getNewTm( long milSec, int sec) {
		String tm;
		Calendar cl = Calendar.getInstance();
		cl.setTimeInMillis(milSec);
		cl.add(Calendar.SECOND, sec);
		tm = cl.get(Calendar.YEAR) + "-"+cl.get(Calendar.MONTH)+ "-"+ cl.get(Calendar.DAY_OF_MONTH) + DataFmt.COMMAVAL+ cl.get(Calendar.HOUR_OF_DAY)+ ":"+cl.get(Calendar.MINUTE)+":"+cl.get(Calendar.SECOND);
		return tm;
	}

	public String getNewTm(long milSec, int sec, String[] fmt) {
		String tm;
		Calendar cl = Calendar.getInstance();
		cl.setTimeInMillis(milSec);
		cl.add(Calendar.SECOND, sec);

		//check Date
		String dtm = "", dateFmt = fmt[DataFmt.DATE_IDX], tmFmt = fmt[DataFmt.TM_IDX];
		if (!dateFmt.equals(DataFmt.NODATE)) {
			dtm = cl.get(Calendar.YEAR) + dateFmt +cl.get(Calendar.MONTH)+ dateFmt+ cl.get(Calendar.DAY_OF_MONTH)+ fmt[DataFmt.DMTR_IDX];
		} 

		//check tm
		if (tmFmt.equals(DataFmt.TIMESEC)) {
			dtm += cl.get(Calendar.HOUR_OF_DAY)* 3600 + cl.get(Calendar.MINUTE)*60 +cl.get(Calendar.SECOND);
		} else {
			dtm +=cl.get(Calendar.HOUR_OF_DAY)+ tmFmt +cl.get(Calendar.MINUTE)+ tmFmt +cl.get(Calendar.SECOND)+"."+cl.get(Calendar.MILLISECOND);
		}

		return dtm;
	}


	public void writeDataToFile(List<Variable> sublvars, int[] range, String[] fmt){
		// all the time-range data length-should be the seconds in the time range
		tmPassed=0; bfinish =false; progIdx =0; 

		int size = sublvars.size(); ///variable name to the first line of out file
		data = new float [size][];
		oneDLen= new int[size];
		missVal = new float[size];
		hRate = new int[size];
		long milSec= 0;
		Variable tmp=null;

		long t1 = System.currentTimeMillis();
		try {
			milSec = getTimeMilSec(); 
			for (int i =0; i<size; i++){	
				if (bfinish) return;
				tmp = sublvars.get(i);
				oneDLen[i]= getLen(tmp);
				totVarLen +=oneDLen[i];
				missVal[i]= tmp.findAttribute("_FillValue").getNumericValue().floatValue();
				if (gDataInf[0]>1)  {
					hRate[i]=getOR(tmp);
				}
				data[i] = read1DData(tmp , range[0], range[1]);
				progIdx ++;
				if (bMode) { System.out.println("Reading "+ progIdx);}
			}
		} catch (NCDataException e) {
			nc2Asc.NC2Act.wrtMsg("wrtieDataToFile_NCDataException "+ tmp.getName());
		} catch (InvalidRangeException ee) {
			nc2Asc.NC2Act.wrtMsg("wrtieDataToFile_InvalidRangeException "+ tmp.getName());
		} catch (IOException eee) {
			nc2Asc.NC2Act.wrtMsg("wrtieDataToFile_IOException "+ tmp.getName());
		}

		if (gDataInf[0]>1) {
			writeHighRateData(range, fmt, milSec, t1, size);
		} else if (Integer.parseInt(fmt[DataFmt.AVG_IDX])>1) {
			writeLowRateAvgData(range, fmt, milSec, t1, size);
		} else {
			writeNormalData (range, fmt, milSec, t1, size) ;
		}

		tmPassed =System.currentTimeMillis() - t1;
		bfinish = true;
	}

	private void writeNormalData(int[] range, String[] fmt, long milSec, long t1, int size) {
		//regular data - no average& no high rate
		String dmtr = fmt[DataFmt.DMTR_IDX], mval = fmt[DataFmt.MVAL_IDX];
		for (int i =0; i<range[1]; i++) {
			if (bfinish) return;
			String line = getNewTm(milSec,i+range[0], fmt);
			progIdx++; 
			for (int j =0; j<size; j++) {
				int count =0;
				while (count<oneDLen[j]) {
					float f = data[j][oneDLen[j]*i+count];
					if (f == missVal[j]) {
						line += dmtr + mval;
					} else {
						line += dmtr + f;
					}
					count++;			
				}
			}
			writeOut(line+"\n");
		}
	}
	
	private void writeLowRateAvgData(int[] range, String[] fmt, long milSec, long t1, int size) {
		int avg = Integer.parseInt(fmt[DataFmt.AVG_IDX]); 
		long tot = 0;
		for (int j=0; j<avg; j++) {
			tot +=  j;
		}
		tot = (long) 1000.0 * tot/avg;
		String dmtr = fmt[DataFmt.DMTR_IDX], mval = fmt[DataFmt.MVAL_IDX]; 
		float[] totVal= new float[totVarLen]; int totValIdx=0, avgCount =0; float mVal = -99999;

		for (int i =0; i<range[1]; i++) { //data-in the time range
			if (bfinish) return;
			progIdx ++; totValIdx=0; String lineData="";
			for (int j =0; j<size; j++) {
				int count =0;
				while (count<oneDLen[j]) {
					float f = data[j][oneDLen[j]*i+count];
					if (f == missVal[j] || totVal[totValIdx]==mVal) {
						totVal[totValIdx]= mVal;
					} else {
						totVal[totValIdx]+= f/avg;
					}

					if (avgCount == avg-1) {
						if (totVal[totValIdx] == mVal) {
							lineData += dmtr + mval;;
						} else {
							lineData += dmtr + totVal[totValIdx];
						}
						totVal[totValIdx]=0;
					}
					count ++;		//one-var-len
					totValIdx++;  	//total-varl-len
				}
			}
			//write one averaged-line
			if (avgCount<(avg-1)) {
				avgCount++; 		// time-D
			} else {  				//write avg-data-val to file and reset
				String line = getNewTm(milSec +(range[0]+i-avg+1)*1000 +tot,0, fmt)+lineData;
				writeOut(line+"\n");
				avgCount=0;
			}
		}
		
	}


	private void writeHighRateData(int[] range, String[] fmt, long milSec, long t1, int size) {
		String dmtr = fmt[DataFmt.DMTR_IDX], mval = fmt[DataFmt.MVAL_IDX]; 
		float[] valKp= new float[totVarLen];
		
		for (int i =0; i<range[1]; i++) { //data-in the time range
			if (bfinish) return;
			progIdx ++; 

			int topRate = (int)gDataInf[0];
			float tmInt = (float)1.0/topRate;
			for (int k=0; k<topRate; k++) {  //highest rate
				int varIdx =0;
				String line  = getNewTm(milSec + (long)(range[0]+i+k*tmInt)*1000, 0, fmt);
				for (int j =0; j<size; j++) { //variables
					int count =0; int dataInterval = topRate/hRate[j]; 
					while (count<oneDLen[j]) { //length-of-each-variable
						if (k % dataInterval==0) {
							int idx = k/dataInterval;
							valKp[varIdx] = data[j][oneDLen[j]*i + count + idx];
							line += dmtr + valKp[varIdx];
						} else {
							if (mval.equals(DataFmt.REPLICATE)) {
								line += dmtr + valKp[varIdx];
							} else {
								line += dmtr + mval;
							}
						}
						count ++;		//one-var-len
						varIdx++;        //tot-var-len
					}
				} //variable
				writeOut(line+"\n");
			} //k-highest rate
		} //time-for
	}

	public void writeOut(String str) {
		try {
			fout.write(str);
			fout.flush();
			if (bMode) { System.out.println("Writing "+ progIdx);}
		} catch (IOException e ) {
			nc2Asc.NC2Act.wrtMsg("writeOut_err:"+e.getMessage());
		}
	}


	private void checklvars(List<Variable> tmp, List<Variable> vars) {
		int idx =0;
		for (int i =0; i<tmp.size(); i++) {
			Variable v = tmp.get(i);
			if (v.getName().equals("base_time")||v.getName().equals("time_offset")) {
				continue;
			}
			vars.add(v); idx ++;
		}
	}
	
	/**
	 * Open the netcdf file, find each variable based on the variable names from the batch file, and
	 * return the list of Variables
	 * @return -- selected variables
	 */
	public List<Variable> getBatchSubVars(List<String> selVars){
		
		if (fin==null) {System.out.println("The netcdf file is not opened.");
			System.exit(-1);
		}
		List<Variable> lvars= new ArrayList();
		for (int i =0; i<selVars.size(); i++) {
			lvars.add(fin.findVariable(selVars.get(i)));
		}
		return lvars;
	}
	
	
	
} //eofclass



