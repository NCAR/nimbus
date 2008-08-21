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
	 *   All the data for a variable in the given range in 1D foramt
	 */
	private float[] oneDData;
	
	/**
	 *  All the ascii data that go to the output file. the index means the line order
	 */
	private String[] data;
	
	/**
	 *  All the variables' name, units, OR/Len displayed in the table for users
	 */	
	private String[] dataInf;

	/**
	 * 	Global information about the netcdf file
	 *  idx 0--high rate data
	 *  idx 1--start time in milli second
	 *  idx 2--size of the records
	 */
	private String[] gDataInf = {String.valueOf(false),"0","0"};
	
	/**
	 * Get the variables' list from the netcdf file
	 * @return
	 */
	public List<Variable> getVars(){return lvars;}
	
		
	/**
	 * Get all the data from the selected variable names 
	 * @return
	 */
	public String[] getData(String vn) {
		return data;
	}

	/**
	 * 
	 * Get the data information such as variable name, units, OR, etc.
	 * Usually, it is the all the variable in a netcdf file.
	 * @return -- pointer to data information  
	 *  
	 */
	public String[] getDataInf() {
		return dataInf;
	}

	/**
	 *  Get the global information of the netcdf file
	 */
	public String[] getGlobalDataInf() {
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
	public void openFile() throws IOException {
		//open netcdf readonly
		try{
			fin = NetcdfFile.open(infn);
		} catch (IOException e){
			if (fin !=null){ 
				fin.close();
				fin = null;
			}
			throw e;
		} 
		
		if (outfn!=null){
			openOutFile(outfn);
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
	 * Retrieve attribute information such as name, unit, OR/len, etc for the variables
	 * 
	 * @throws NCDataException
	 * @throws ArrayIndexOutOfBoundsException
	 */
	public void readDataInf() throws NCDataException, ArrayIndexOutOfBoundsException, InvalidRangeException, IOException{			

		if (fin == null) { 
			throw new NCDataException("readDataInf: Empty input file exception:");
		}

		if (lvars==null) { lvars =  new ArrayList<Variable>(); }
		lvars = fin.getVariables();

		int len = lvars.size();
		dataInf = new String[len];		
		for (int i=0; i<len-1 ; i++){ 
			String dat = "";
			Variable v = lvars.get(i);
			dat += v.getShortName() + DataFmt.SEPDELIMIT.toString();
			dat += v.getUnitsString() + DataFmt.SEPDELIMIT.toString();
			int or= getOR(v);
			dat += or+"/"+getLen(v) + DataFmt.SEPDELIMIT.toString() ;
			dat += v.getName();
			dataInf[i]=dat;
			
			if (or>1){ gDataInf[0] = String.valueOf(true);}
		}
		
		//init global data info
		gDataInf[1] = String.valueOf(getTimeMilSec()); 
		gDataInf[2] = String.valueOf(lvars.get(0).getSize());
	}

	

	public String readOneVarData (Variable v, int start) throws InvalidRangeException, IOException {

		String dat =null; // we handle 1-d to d-3 for now

		//set orig and size
		int[] shape = v.getShape();
		int[] origin=new int[shape.length], size=new int[shape.length];

		origin[0] =  start;
		size[0] = 1;
		for (int i=1; i<shape.length; i++) {
			origin[i]=0;
			size[i]  =shape[i];
		}		

		Array data = v.read(origin, size);
		data=data.reduce();
		
		IndexIterator ii = data.getIndexIterator();
		int count=0;
		while (ii.hasNext()){
			float f =ii.getFloatNext();
			count++;
			if (dat==null) {
				dat = String.valueOf(f);
			} else {
				dat += DataFmt.SEPDELIMIT.toString()+f;
			}
		}
		return dat;
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
			demoData[i]= getNewTm(milSec, i)+ DataFmt.SEPDELIMIT.toString() 
			+ DataFmt.MISSVAL + DataFmt.SEPDELIMIT.toString()
			+ "0.0";
		}
		return demoData;
	}

	public String[] mkDemoData() throws  NCDataException, InvalidRangeException, IOException {

		String[] demoData = new String[10];

		if (lvars==null || lvars.size()==0) {
			throw new NCDataException("getDemoData encounts empty variable list.");
		}

		// get two vars which have single data value
		Variable v1=null, v2=null;
		for (int i=0; i< lvars.size(); i++) {
			if (getLen(lvars.get(i))==1){
				if (v1==null) {
					v1=lvars.get(i);
					continue;
				} else if (v2==null) {
					v1=lvars.get(i);
					break;
				}
			}
		}
		
		demoData[0] ="Date,UTC,"+v1.getShortName() +","+v2.getShortName();

		//nc2Asc.NC2Act.wrtMsg(demoData[0]);
		long milSec;
		milSec = getTimeMilSec();
		
		//get 10 secons data
		int len =10;
		for (int i=1; i<len; i++) {
			demoData[i]= getNewTm(milSec, i)+ DataFmt.SEPDELIMIT.toString(); 
			demoData[i] += readOneVarData(v1, i) + DataFmt.SEPDELIMIT.toString();
			demoData[i] += readOneVarData(v2, i);
			nc2Asc.NC2Act.wrtMsg(demoData[i]);

		}

		return demoData;
	}

	public long getTimeMilSec() throws NCDataException {
		if (dataInf[0]== null) {
			throw new  NCDataException("getTimeMilSec: Variables are not read... Please get variables from the netcdf file.");
		}
		//nc2Asc.NC2Act.wrtMsg(dataInf[0]);
		String tmVar= dataInf[0];
		String date = tmVar.split(DataFmt.SEPDELIMIT.toString())[1].split(" ")[2];
		String tm   = tmVar.split(DataFmt.SEPDELIMIT.toString())[1].split(" ")[3];
		String[] dInf = date.split("-");
		String[] tmInf   = tm.split(":");

		Calendar cl = Calendar.getInstance();
		int y= Integer.parseInt(dInf[0]);//new Integer(dInf[0]).intValue();
		int mm= Integer.parseInt(dInf[1]);//new Integer(dInf[1]).intValue();
		int d= Integer.parseInt(dInf[2]);//new Integer(dInf[2]).intValue();

		int h= Integer.parseInt(tmInf[0]);//new Integer(tmInf[0]).intValue();
		int m= Integer.parseInt(tmInf[1]);//new Integer(tmInf[1]).intValue();
		int s= Integer.parseInt(tmInf[2]);//new Integer(tmInf[2]).intValue();

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
		tm = cl.get(Calendar.YEAR) + "-"+cl.get(Calendar.MONTH)+ "-"+ cl.get(Calendar.DAY_OF_MONTH) + DataFmt.SEPDELIMIT+ cl.get(Calendar.HOUR_OF_DAY)+ ":"+cl.get(Calendar.MINUTE)+":"+cl.get(Calendar.SECOND);
		return tm;
	}

	private String subStr(String s){
		String ss=s;
		while (true) {
			int idx =ss.indexOf("null,");
			if (idx==-1) { 
				break;
			}
			//assume "null," is always at beginning
			ss = ss.substring(idx+"null.".length());
		}
		return ss;
	}
	

} //eofclass


