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

	public final static String SEPDELIMIT =",";

	//constructor
	public NCData(String ifn, String ofn) {infn = ifn; outfn=ofn;}

	//data
	private String infn, outfn;
	NetcdfFile  fin=null;
	FileWriter  fout=null;
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
	public FileWriter getOutFile() {
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
		try{
			fin = NetcdfFile.open(infn);
		} catch (IOException e){
			if (fin !=null){ 
				fin.close();
				fin = null;
			}
			throw e;
		} 

		//open a filewriter 
		try {
			fout = new FileWriter(outfn);		
		} catch (IOException e) {
			if (fout !=null) {
				fout.close();
				fout = null;
			}
			throw e;
		}
	}

	public void readDataInf() throws NCDataException, ArrayIndexOutOfBoundsException {

		//try {
		if (fin == null) { 
			throw new NCDataException("readDataInf: Empty input file exception:");
		}

		if (lvars==null) { lvars =  new ArrayList<Variable>(); }
		lvars = fin.getVariables();


		int len = lvars.size();
		dataInf = new String[len];		
		String disdata= "";
		for (int i=0; i<len-1 ; i++){ 
			String dat = "";
			Variable v = lvars.get(i);
			dat += v.getShortName() + SEPDELIMIT.toString();
			dat += v.getUnitsString() + SEPDELIMIT.toString();
			dat += getOR(v)+"/"+getLen(v) + SEPDELIMIT.toString() ;
			dat += v.getName();
			if (i<10) {
				disdata += dat; 
				disdata += "\n ";
			}
			dataInf[i]=dat;
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
	public String[] readVariableData(Variable v, int startIdx, int len) throws  ArrayIndexOutOfBoundsException, InvalidRangeException, IOException{

		String[] dat= new String[len]; // we handle 1-d to d-3 for now
		//try {
		//set orig and size
		int[] shape = v.getShape();
		int[] origin=new int[shape.length], size=new int[shape.length];
		if (shape.length==1) {
			origin[0] =  startIdx;
			size[0] = len;
		}
		if (shape.length==2) {
			origin[0] = startIdx;
			origin[1] = 0;
			size[0] = len;
			size[1] = shape[1];
		}

		if (shape.length==3){
			origin[0]=startIdx;
			origin[1]=0;
			origin[2]=0;
			size[0]  =len;
			size[1]  = shape[1];
			size[2]  = shape[2];
		}

		//get data
		Array data = v.read(origin, size);

		shape = data.getShape();
		Index index = data.getIndex();

		//get double from Array
		double d =0.0; String dStr;
		if (shape.length == 1) { 
			for (int i =0; i<len; i++){
				d = data.getFloat(index.set(i+startIdx));
				dat[i] = String.valueOf(d);
			}	
		}

		//average data for 2d------not doing it
		if (shape.length == 2){
			for (int i=0; i<len; i++) {
				for (int j=0; j<shape[1]; j++) {
					d = data.getFloat(index.set(i+startIdx,j));
					if (j==0) {
						dat[i] = String.valueOf(d);
					} else {
						dat[i] = dat[i]+ SEPDELIMIT.toString()+ String.valueOf(d);
					} 
				}
				dat[i] = subStr(dat[i]);
			}
		}

		// average data for 3d
		//d1=0; d2=0; d3=0;
		if (shape.length == 3){ 
			for (int i=0; i<len; i++) {
				for (int j=0; j<shape[1]; j++) {
					for (int k=0; k<shape[2] ; k++){ 
						d = data.getFloat(index.set(i+startIdx,j,k));
						if (j==0 && k==0) {
							dat[i]= String.valueOf(d);
						} else {
							dat[i]= dat[i]+ SEPDELIMIT.toString()+ String.valueOf(d);
						} 
					}
				}
				dat[i] = subStr(dat[i]);
			}
		}
		//} catch (ArrayIndexOutOfBoundsException ee) {
		//	throw ee;//new IOException("get reading data index-exception:"+ee.getMessage());
		//}  catch (InvalidRangeException e) {
		//	throw new InvalidRangeException("Variable read error:"+ e.getMessage());
		//	} catch (IOException e){
		//	throw e;//new IOException("get reading data IO-exception:"+e.getMessage());
		//}
		return dat;
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

		//get one data -- size=1
		Array data = v.read(origin, size);

		IndexIterator ii = data.getIndexIterator();
		int count =0;
		while (ii.hasNext()){
			float f =ii.getFloatNext();
			if (dat==null) {
				dat = String.valueOf(f);
			} else {
				dat += SEPDELIMIT.toString()+f;
			}
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

	private int getLen(Variable v) {
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
		nc2Asc.NC2Act.wrtMsg("getDemoData: "+demoData[0]);
		long milSec;
		String[] vdata1, vdata2;
		milSec = getTimeMilSec();
		vdata1= readVariableData(v1, 0, 10);
		vdata2= readVariableData(v2, 0, 10);

		int len =10;
		if (vdata1.length <len) {len = vdata1.length;}
		if (vdata2.length <len) {len = vdata2.length;}
		for (int i=1; i<len; i++) {
			demoData[i]= getNewTm(milSec, i)+ SEPDELIMIT.toString() 
			+ vdata1[i].split(SEPDELIMIT.toString())[0] + SEPDELIMIT.toString()
			+ vdata2[i].split(SEPDELIMIT.toString())[0];
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

		nc2Asc.NC2Act.wrtMsg(demoData[0]);
		long milSec;
		milSec = getTimeMilSec();
		
		//get 10 secons data
		int len =10;
		for (int i=1; i<len; i++) {
			demoData[i]= getNewTm(milSec, i)+ SEPDELIMIT.toString(); 
			demoData[i] += readOneVarData(v1, i) + SEPDELIMIT.toString();
			demoData[i] += readOneVarData(v2, i);
			nc2Asc.NC2Act.wrtMsg(demoData[i]);

		}

		return demoData;
	}

	public long getTimeMilSec() throws NCDataException {
		if (dataInf[0]== null) {
			throw new  NCDataException("getTimeMilSec: Variables are not read... Please get variables from the netcdf file.");
		}
		nc2Asc.NC2Act.wrtMsg(dataInf[0]);
		String tmVar= dataInf[0];
		String date = tmVar.split(SEPDELIMIT.toString())[1].split(" ")[2];
		String tm   = tmVar.split(SEPDELIMIT.toString())[1].split(" ")[3];
		String[] dInf = date.split("-");
		String[] tmInf   = tm.split(":");

		Calendar cl = Calendar.getInstance();
		int y= new Integer(dInf[0]).intValue();
		int mm= new Integer(dInf[1]).intValue();
		int d= new Integer(dInf[2]).intValue();

		int h= new Integer(tmInf[0]).intValue();
		int m= new Integer(tmInf[1]).intValue();
		int s= new Integer(tmInf[2]).intValue();

		cl.set(y,mm,d,h,m,s);
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


