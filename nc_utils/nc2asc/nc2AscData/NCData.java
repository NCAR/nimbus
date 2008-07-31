package nc2AscData;

import java.lang.*;
import java.lang.Exception.*;
import java.io.*;
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

	private final static String SEPDELIMIT =",";

	//constructor
	public NCData(String ifn, String ofn) {infn = ifn; outfn=ofn;}

	//data
	private String infn, outfn;
	NetcdfFile  fin=null;
	java.io.FileWriter  fout=null;
	List<Variable> lvars=null;
	String[] data, dataInf;

	
	public List<Variable> getVars(){return lvars;}
	/**
	 * Get the data from the selected variable names 
	 * @return
	 */
	public String[] getData(String vn) {
		return data;
	}

	/**
	 * 
	 * Get the data information such as variable name, units, OR, etc.
	 * Usually, it is the all the variable in a netcdf file.
	 * @return 
	 *  
	 */
	public String[] getDataInf() {
		return dataInf;
	}

	protected void finalize() {
		if (fin!=null)  {try {fin.close();} catch (IOException e) {}}
		if (fout!=null) {try {fout.close();} catch (IOException e) {}}
	}


	public NetcdfFile getInFile() {
		return fin;
	}
	public java.io.FileWriter getOutFile() {
		return fout;
	}

	//methods
	/**
	 * setFileName provides the users with ability to set up netcdf read-only file name and 
	 * an output file name
	 */
	public void setFileName(String ifn, String ofn){infn=ifn; outfn=ofn;}

	/**
	 * openFile is to open a netcdf read-only file and a writable output file.
	 * Since we need both files to operate, we open them at the same time.
	 */	
	public void openFile() throws IOException {
		//open netcdf readonly
		try {
			fin = NetcdfFile.open(infn);
		} catch (IOException e){
			try { 
				if (fin !=null){ 
					fin.close();
					fin = null;
				}
			} catch (IOException ie) {}
			throw e;//("File "+infn+ "open error:"+ e.getMessage());
		} 

		//open a filewriter 
		try {
			fout = new java.io.FileWriter(outfn);			
		} catch (IOException e) {
			try {
				if (fout !=null) {
					fout.close();
					fout = null;
				}
			} catch (IOException ee) { }
			throw e;//new IOException("File "+outfn+ "open error:"+ e.getMessage() );
		}
	}


	public void readDataInf() throws NCDataException, ArrayIndexOutOfBoundsException {

		try {
			if (fin == null) { 
				throw new NCDataException("readDataInf: Empty input file exception:");
			}
			if (lvars==null) { lvars =  new ArrayList<Variable>(); }
			lvars = fin.getVariables();
			

			int len = lvars.size();
			//if ((startIdx+num) >lvars.size()){ len = lvars.size()-startIdx;	} 
			dataInf = new String[len];		
			String disdata= "";
			for (int i=0; i<len-1 ; i++){ 
				String dat = "";
				Variable v = lvars.get(i);
				dat += v.getShortName() + SEPDELIMIT.toString();
				dat += v.getUnitsString() + SEPDELIMIT.toString();
				dat += getOR(v) + SEPDELIMIT.toString();
				dat += v.getName();
				if (i<10) {
					disdata += dat; 
					disdata += "\n ";
				}
				dataInf[i]=dat;
			}
			//nc2Asc.NC2Act.wrtMsg("Data Inf Example: \n"+disdata);

		} catch (ArrayIndexOutOfBoundsException r){
			throw r; 
		} 
	}

	/**
	 * Read all the data from a Variable within the time-range
	 * 
	 * @param v - variable
	 * @param startIdx - starttime 
	 * @param len - number of seconds data
	 * @return - array of string, each string contains one-data with delimeter, number of strings equals len.  
	 * @throws IOException
	 * @throws ArrayIndexOutOfBoundsException
	 * @throws InvalidRangeException
	 */
	public String[] readVariableData(Variable v, int startIdx, int len) throws IOException, ArrayIndexOutOfBoundsException, InvalidRangeException {
		String[] dat= new String[len]; // we handle 1-d to d-3 for now

		try {

			//set orig and size
			int[] shape = v.getShape();
			int[] origin= new int[1], size=new int[1];
			if (shape.length==1) {
				origin = new int[] {startIdx};
				size = new int[] {len};
			}
			if (shape.length==2) {
				origin = new int[] {startIdx, 0};
				size = new int[] {len, shape[1]};
			}

			if (shape.length==3){
				origin = new int[] {startIdx, 0,0};
				size = new int[] {len, shape[1], shape[2]};
			}

			//get data
			Array data = v.read(origin, size).reduce();

			shape = data.getShape();
			Index index = data.getIndex();

			//get double from Array
			if (shape.length == 1) { 
				for (int i =0; i<len; i++){
					dat[i] = ""+data.getDouble(index.set(i+startIdx));
				}		
			}

			//average data for 2d------not doing it
			if (shape.length == 2){
				for (int i=0; i<len; i++) {
					for (int j=0; j<shape[1]; j++) {
						dat[i] += data.getDouble(index.set(i+startIdx,j))+ ",";

					}
				}
			}

			// average data for 3d
			//d1=0; d2=0; d3=0;
			if (shape.length == 3){ 
				for (int i=0; i<len; i++) {
					for (int j=0; j<shape[1]; j++) {
						for (int k=0; k<shape[2] ; k++){
							dat[i] += data.getDouble(index.set(i+startIdx,j,k))+ "," ;
						}
					}
				}
			}
		} catch (ArrayIndexOutOfBoundsException ee) {
			throw ee;//new IOException("get reading data index-exception:"+ee.getMessage());
		}  catch (InvalidRangeException e) {
			throw new InvalidRangeException("Variable read error:"+ e.getMessage());
		} catch (IOException e){
			throw e;//new IOException("get reading data IO-exception:"+e.getMessage());
		}
		return dat;
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

	public String[] getDemoData() throws IOException, NCDataException, InvalidRangeException {
		String[] demoData = new String[10];

		if (lvars==null || lvars.size()==0) {
			throw new NCDataException("getDemoData encounts empty variable list.");
		}
		// get 10 example data
		Variable v1 = lvars.get(1);
		Variable v2 = lvars.get(2);
		demoData[0] ="Date,UTC,"+v1.getShortName() +","+v2.getShortName();
		//nc2Asc.NC2Act.wrtMsg(demoData[0]);
		long milSec;
		String[] vdata1, vdata2;
		try {
			milSec = getTimeMilSec();
			vdata1= readVariableData(v1, 0, 10);
			vdata2= readVariableData(v2, 0, 10);
		} catch (InvalidRangeException er) {
			throw er;
		} catch (IOException e) {
			throw e;// new IOException("getTimeSec encounts getTimeMilSec exception.");
		} 

		String data1 = vdata1[0];
		String[] d1 = data1.split(SEPDELIMIT.toString());

		String data2 = vdata2[0];
		//} else if (vdata2.length>1) {
		//data2 = vdata2[1];
		//}
		String[] d2 = data2.split(" ");

		int len =10;
		if (d1.length <len) {len = d1.length;}
		if (d2.length <len) {len = d2.length;}
		for (int i=1; i<len; i++) {
			demoData[i]= getNewTm(milSec, i)+ "," + d1[i]+ ","+d2[i];
		}
		return demoData;
	}

	public long getTimeMilSec() throws NCDataException {
		if (dataInf[0]== null) {
			throw new  NCDataException("getTimeMilSec: Variables are not read... Please get variables from the netcdf file.");
		}
		String tmVar= dataInf[0];
		String date = tmVar.split("~")[1].split(" ")[2];
		String tm   = tmVar.split("~")[1].split(" ")[3];
		String[] dInf = date.split("-");
		String[] tmInf   = tm.split(":");

		//nc2Asc.NC2Act.wrtMsg("Date and time to build calendar: "+date+ "   "+tm);
		Calendar cl = Calendar.getInstance();
		int y= new Integer(dInf[0]).intValue();
		int mm= new Integer(dInf[1]).intValue();
		int d= new Integer(dInf[2]).intValue();

		int h= new Integer(tmInf[0]).intValue();
		int m= new Integer(tmInf[1]).intValue();
		int s= new Integer(tmInf[2]).intValue();

		cl.set(y,mm,d,h,m,s);
		//nc2Asc.NC2Act.wrtMsg("Date and time after calendar: "+ cl.get(Calendar.YEAR) + "-"+cl.get(Calendar.MONTH) +"-" +cl.get(Calendar.DAY_OF_MONTH) +" "+ cl.get(Calendar.HOUR_OF_DAY)+":"+cl.get(Calendar.MINUTE)+":" +cl.get(Calendar.SECOND));
		//cl.add(Calendar.SECOND, 1)
		return cl.getTimeInMillis();
	}

	public String  getNewTm( long milSec, int sec) {
		String tm;
		Calendar cl = Calendar.getInstance();
		cl.setTimeInMillis(milSec);
		cl.add(Calendar.SECOND, sec);
		tm = cl.get(Calendar.YEAR) + "-"+cl.get(Calendar.MONTH)+ "-"+ cl.get(Calendar.DAY_OF_MONTH) + SEPDELIMIT.toString()+ cl.get(Calendar.HOUR_OF_DAY)+ ":"+cl.get(Calendar.MINUTE)+":"+cl.get(Calendar.SECOND);
		return tm;
	}



} //eofclass


