package nc2Asc;

import java.lang.*;
import java.lang.Exception.*;
import java.io.*;
import java.util.*;

import ucar.nc2.*;

import nc2AscData.*;

/**
 * Class to convert between the user desired choices and the program, regards i/o files, data formats,
 * and variables, etc.
 * @author dongl
 */
public class BatchConfig {

	/**
	 * File names from command line:
	 * -b follows batchFile 	=> files[0]
	 * -i follows inputFile 	=> files[1]
	 * -o follows outputFile 	=> files[2]
	 */
	private String[] files = new String[3]; 

	/**
	 * dataFmt 	0 -- hd-head
	 * 			1 -- avg
	 * 			2 -- dt-date
	 * 			3 -- tm-time
	 * 			4 -- sp-separator 
	 * 			5 -- fv-fill value
	 * 			6 -- time set
	 */
	private String[] dataFmt = new String[7]; 


	/**
	 * list of selected variables' names -- equals to NCData-getsublvars 
	 */
	private List<String> selVars; 

	/**
	 * list of time range  -- 
	 * tmRange[0] - beg-index
	 * tmRange[1] - range
	 */
	private int[] tmRange = new int[2];  

	/**
	 * Constructor catches the command-line inputs, and sign the files to its storage.
	 * The class users need to call start() to read and interpret the batch file to get ready for data writing.
	 * @param args
	 */
	public BatchConfig(String[] args) {
		parseArgs(args);
	}

	/**
	 * This method ensures that the all the elements for data writing are read and ready.
	 * Users need to call this method after the class constructor, to get ready for data-writing. 
	 */
	public void start() {	
		try {
			parseBatchFile();
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(-1);
		}
	}

	/**
	 * The beginning and ending indexes of the input time set
	 * @return
	 */
	public int[] getTmRange() { return tmRange;}
	
	/**
	 * File names from command line:
	 * -b follows batchFile 	=> files[0]
	 * -i follows inputFile 	=> files[1]
	 * -o follows outputFile 	=> files[2]
	 * @param args  --see the String[] files and documents
	 */
	public void setFiles(String[] args) {
		parseArgs(args);
	}

	/**
	 * @return -- String[] 	files
	 * 						files[0]  -- batchfile
	 * 						files[1]  -- inputfile
	 * 						files[2]  -- outputfile
	 */
	public String[] getFiles() {
		return files;
	}


	/**
	 * Ref to dataFmt definition 
	 * @param fmt -- String[7]
	 */
	public void setDataFmt(String[] fmt){
		dataFmt = fmt;
	}

	public List<String> getSelVars() {
		return selVars;
	}

	/**
	 * 
	 * @return fmt --ref to dataFmt definition
	 */
	public String[] getDataFmt() {
		return dataFmt;
	}

	/**
	 * Display the data format specified in the input batch file
	 */
	public String showFmt() {
		String display ="";
		for (int i=0; i<dataFmt.length; i++) {
			display += dataFmt[i]+ "\n";
		}
		System.out.println(display);
		return display;
	}

	

	/**
	 * Display the variables specified in the input batch file
	 */
	public String showSelectedVars() {
		String display ="";
		for (int i=0; i<selVars.size(); i++) {
			display += selVars.get(i)+ "\n";
		}
		System.out.println(display);
		return display;
	}


	/**
	 * @param args -- see the string[] files and documents
	 */
	private void parseArgs(String[] args){
		for (int i=0; i<args.length; i++) {
			if (args[i].equals("-b")) {
				files[0]=args[++i];
			}
			if (args[i].equals("-i")) {
				files[1]=args[++i];
			}
			if (args[i].equals("-o")) {
				files[2]=args[++i];
			}
		}
	}

	/**
	 * A batch file contains i/o files, selected variables, and desired data formats.
	 * Therefore, parseBatchFile needs to sign i/o files if there are no i/o files from command line;
	 * adds the variables to the variable list, and signs desired output data formats. 
	 * 
	 * if=	/home/data/231RF10.nc
	 * of=	junk.asc
	 * hd= 	Plain/Icartt/Xml
	 * avg= 4
	 * dt= 	yyyy-mm-dd/yyy mm dd/NoDate
	 * tm= 	hh:mm:ss/hh mm ss/SecOfDay  
	 * ti=	yyyy-mm-dd,hh:mm:ss ~ yyyy-mm-dd,hh:mm:ss
	 * sp= 	Space/Comma
	 * fv=	FillValue/Blank/Replicate
	 * var=	ADIFR
	 * var=	BDIFR
	 */
	public void parseBatchFile() throws Exception{
		if(files[0]==null) { return;}

		FileReader fr;
		fr = new FileReader (new File(files[0]));
		BufferedReader br = new BufferedReader (fr);
		selVars = new ArrayList<String>();

		String line = br.readLine();
		while (line != null) {
			line = line.trim();
			String val  = line.substring(line.indexOf("=")+1).trim();
			// sign i/o files
			if (line.indexOf("if")==0) {
				if (files[1]!=null) continue; //got input file from cmd line
				files[1] = val;
			}
			if (line.indexOf("of")==0) {
				if (files[2]!=null) continue; //got output file from cmd line
				files[2] = val;
			}
			//sign format
			if (line.indexOf("hd")==0) {
				dataFmt[DataFmt.HEAD_IDX]= val;
			}
			if (line.indexOf("avg")==0) {
				dataFmt[DataFmt.AVG_IDX]= val;
			}
			if (line.indexOf("dt")==0) {
				dataFmt[DataFmt.DATE_IDX]= val;
			}
			if (line.indexOf("tm")==0) {
				dataFmt[DataFmt.TM_IDX]= val;
			}
			if (line.indexOf("sp")==0) {
				if (val.toLowerCase().equals(DataFmt.COMMA.toLowerCase())){
					dataFmt[DataFmt.DMTR_IDX]= DataFmt.COMMAVAL;
				} else {
					dataFmt[DataFmt.DMTR_IDX]= DataFmt.SPACEVAL;
				}
			}
			if (line.indexOf("fv")==0) {
				String mval = line.substring(line.indexOf("=")+1);
				if (mval.toLowerCase().equals(DataFmt.REPLICATE.toLowerCase())) 
					dataFmt[DataFmt.MVAL_IDX]= val;
				if (mval.toLowerCase().equals(DataFmt.FILLVALUE.toLowerCase())) 
					dataFmt[DataFmt.MVAL_IDX]= DataFmt.MISSVAL;
				if (mval.toLowerCase().equals(DataFmt.LEAVEBLANK.toLowerCase())) 
					dataFmt[DataFmt.MVAL_IDX]= "";
			}
			if (line.indexOf("ti")==0) {
				dataFmt[DataFmt.TMSET_IDX]= parseTmSet(val);
			}

			//sign vars
			if (line.indexOf("var")==0 ) {
				selVars.add(val);
			}
			
			line = br.readLine();
		} //while 
		br.close();
		fr.close();
		// set tm range
		calTmRange();

		//verify all the required batch elements
		checkBatchElements();
	}


	/**
	 * 
	 * @param dt --- yyyy-mm-dd,hh:mm:ss~yyyy-mm-dd,hh:mm:ss
	 * @return       beg-time in mil seconds , data/time range
	 * @throws Exception
	 */
	private String  parseTmSet(String dt) throws Exception  {
		String[] tt = dt.split(DataFmt.TMSETDELIMIT);  //yyyy-mm-dd,hh:mm:ss~yyyy-mm-dd,hh:mm:ss

		//start time 
		String[] start = tt[0].split(DataFmt.COMMAVAL); 
		String[] dInf  = start[0].split(DataFmt.DASHVAL);
		String[] tmInf  = start[1].split(DataFmt.COLONVAL);

		int y= Integer.parseInt(dInf[0]);
		int mm= Integer.parseInt(dInf[1]);
		int d= Integer.parseInt(dInf[2]);

		int h= Integer.parseInt(tmInf[0]);
		int m= Integer.parseInt(tmInf[1]);
		int s= Integer.parseInt(tmInf[2]);

		Calendar cl = Calendar.getInstance();
		cl.set(y,mm,d,h,m,s);
		String ret = cl.getTimeInMillis()+ DataFmt.COMMAVAL;

		//end time  
		String[] end = tt[1].split(DataFmt.COMMAVAL); 
		String[] dtm  = end[0].split(DataFmt.DASHVAL);
		String[] tm  = end[1].split(DataFmt.COLONVAL);

		y= Integer.parseInt(dtm[0]);
		mm= Integer.parseInt(dtm[1]);
		d= Integer.parseInt(dtm[2]);

		h= Integer.parseInt(tm[0]);
		m= Integer.parseInt(tm[1]);
		s= Integer.parseInt(tm[2]);

		Calendar cl2 = Calendar.getInstance();
		cl2.set(y,mm,d,h,m,s);

		//time range in seconds
		int range =(int)(cl2.getTimeInMillis() - cl.getTimeInMillis())/1000 ;
		return ret+range;
	}

	/**
	 * Open the netcdf file, read the Time variable to get start-time==>startLong,
	 * get the selected start time from dataFmt[FMT_IDX]split(",")[0]==>selectedLong,
	 * tmRange[0] = (selectedLong-startLong)/1000;
	 * tmRange[1] = dataFmt[FMT_IDX]split(",")[1];
	 * @throws Exception
	 */
	private void calTmRange() throws Exception {
		//open netcdf file to read our time
		NetcdfFile fin=null;
		try{
			fin = NetcdfFile.open(files[1]);
		} catch (IOException e){
			e.printStackTrace();
			System.exit(-1);
		} 

		//get time variable, 
		Variable v = fin.findVariable("Time");
		String tmVar = v.getUnitsString();
		String date = tmVar.split(" ")[2];
		String tm   = tmVar.split(" ")[3];
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
		long ncBegIdx = cl.getTimeInMillis();

		//selected tmset
		String[] selectTm = dataFmt[DataFmt.TMSET_IDX].split(DataFmt.COMMAVAL);
		long selBegIdx = Long.parseLong(selectTm[0]);
		tmRange[0]= (int)(selBegIdx - ncBegIdx)/1000;
		tmRange[1]= Integer.parseInt(selectTm[1]);
		
		fin.close();
	} 

	private void checkBatchElements() {
		if (selVars.size()<1) {
			System.out.println("No variables are read from the batch file.");
			System.exit(-1);
		}

		for (int i=0; i<dataFmt.length; i++)  {
			if (dataFmt[i]==null || dataFmt[i].length()<1) {
				System.out.println("No data format is read from the batch file. Index= "+i);
				System.exit(-1);
			}
		}

		for (int i=0; i<files.length; i++)  {
			if (files[i]==null || files[i].length()<1) {
				System.out.println("No I/O files are read from the batch file. Index= "+i);
				System.exit(-1);
			}
		}
		
		if (tmRange[0]<=0 || tmRange[1]<= 0) {
			System.out.println("No time range is calculated based on the batch file .");
			System.exit(-1);
		}
		
	}

}//eof class