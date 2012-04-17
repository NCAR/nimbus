package edu.ucar.eol.nc2asc;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import edu.ucar.eol.nc2ascData.DataFmt;

/**
 * Class to convert between the user desired choices and the program, regards i/o files, data formats,
 * and variables, etc.
 * @author dongl
 */
public class BatchConfig {

	boolean _dbg=false;
	/**
	 * File names from command line:
	 * -b follows batchFile 	=> files[0]
	 * -i follows inputFile 	=> files[1]
	 * -o follows outputFile 	=> files[2]
	 */
	private String[] files = new String[3]; 

	/**
	 * dataFmt 	0 -- hd-head
	 * 		1 -- avg
	 * 		2 -- dt-date
	 * 		3 -- tm-time
	 * 		4 -- sp-separator 
	 * 		5 -- fv-fill value
	 * 		6 -- time set
	 */
	private String[] dataFmt = new String[7]; 


	/**
	 * list of selected variables' names -- equals to NCData-getsublvars 
	 */
	private List<String> selVars; 

	/**
	 * List selected Variables' data format
	 */
	//private List<String> varDatFmt;
	Map<String, String> tmpVdf; 

	/**
	 * ti- format
	 * ti= yyyy-mm-dd,hh:mm:ss~yyyy-mm-dd,hh:mm:ss
	 */

	private String tmSetOrig = DataFmt.FULLTM;
	/**
	 * Constructor catches the command-line inputs, and sign the files to its storage.
	 * The class users need to call start() to read and interpret the batch file to get ready for data writing.
	 * @param args
	 */

	/**
	 * get the user preferred variable data formats
	 */
	public Map<String, String> getTmpVarDataFmt() {return tmpVdf;}

	public BatchConfig(String[] args) {
		parseArgs(args);
	}

	public String getTmSetOrig() {
		return tmSetOrig;
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
			if (_dbg) System.out.println(" Batchconfig_parseConfig_start_exception...  exits");
			System.exit(-1);
		}
	}


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
		for (int i=0; i<fmt.length; i++) {
			dataFmt[i]=fmt[i];
		}
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
		if (_dbg) System.out.println(display);
		return display;
	}



	/**
	 * Display the variables specified in the input batch file
	 */
	public String showSelectedVars() {
		String display ="";
		for (int i=0; i<selVars.size(); i++) {
			display += selVars.get(i)+ "   " + "\n";
		}
		if (_dbg) System.out.println(display);
		return display;
	}


	/**
	 * @param args -- see the string[] files and documents
	 */
	private void parseArgs(String[] args){

		int i, loop = args.length;

		for (i=0; i<loop; i++) {
			if (_dbg) System.out.println(" "+args[i]);
			if (args[i].equals("-b") && i<loop) {
				files[0]=args[i+1];
			} 
			if (args[i].equals("-i") && i<loop) {
				files[1]=args[i+1];
			} 
			if (args[i].equals("-o") && i<loop) {
				files[2]=args[i+1];
			}
			if (args[i].equals("-d")) _dbg =true;
		}
	}

	/**
	 * A batch file contains i/o files, selected variables, and desired data formats.
	 * Therefore, parseBatchFile needs to sign i/o files if there are no i/o files from command line;
	 * adds the variables to the variable list, and signs desired output data formats. 
	 * 
	 * if=	/home/data/231RF10.nc
	 * of=	junk.asc
	 * hd= 	Plain/AmesDEF/ICARTT/Xml
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
		if (files[0]==null) { return;}

		FileReader fr;
		fr = new FileReader (new File(files[0]));
		BufferedReader br = new BufferedReader (fr);
		selVars = new ArrayList<String>();
		tmpVdf = new HashMap<String, String>();

		String line = br.readLine().trim();
		while (line != null) {
			String val  = line.substring(line.indexOf("=")+1).trim();
			// sign i/o files
			if (line.indexOf("if")==0 && files[1]==null) {
				files[1] = val;
			}
			if (line.indexOf("of")==0 && files[2]==null) {
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
			dataFmt[DataFmt.MVAL_IDX]= DataFmt.MISSVAL;//init fv
			if (line.indexOf("fv")==0) {
				String mval = line.substring(line.indexOf("=")+1);
				if (mval.toLowerCase().equals(DataFmt.REPLICATE.toLowerCase())) 
					dataFmt[DataFmt.MVAL_IDX]= val;
				if (mval.toLowerCase().equals(DataFmt.FILLVALUE.toLowerCase())) 
					dataFmt[DataFmt.MVAL_IDX]= DataFmt.MISSVAL;
				if (mval.toLowerCase().equals(DataFmt.LEAVEBLANK.toLowerCase())) {
					if (dataFmt[DataFmt.DMTR_IDX]== DataFmt.SPACEVAL) {  //if dmtr=space, fillval!=blank
						dataFmt[DataFmt.MVAL_IDX]=DataFmt.MISSVAL;
					} else {
						dataFmt[DataFmt.MVAL_IDX]= "";
					}
				} 
			}
			if (line.indexOf("ti")==0) {
				dataFmt[DataFmt.TMSET_IDX]= parseTmSet(val);
				tmSetOrig = val;
			}

			//sign vars
			if (line.indexOf("var")==0 ) {
				selVars.add(val);
			}

			//sign var format
			if (line.indexOf("%")> 0 ) {
				String var = line.substring(0, line.indexOf("=")).trim();
				if (_dbg) System.out.printf("parse batch file: var = %s format= %s \n", var, val);

				//check var-format %f %d %i % e %E %G %g, %d or %i=>%.0f
				if (val.endsWith("f")||val.endsWith("d")||val.endsWith("i")|| val.endsWith("e")|| val.endsWith("E")|| val.endsWith("g")|| val.endsWith("G") ){
					if (val.endsWith("d")|| val.endsWith("i")) 
						val= "%.0f";
				} else {
					NC2Act.wrtMsg("Warning: Invalid format-- var=" +var + " format="+val+ "   Using default" );
					val= "%f";
				}
				tmpVdf.put(var, val);

			}

			line = br.readLine();
		} //while 
		br.close();
		fr.close();

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
		if (dt.trim().toLowerCase().equals(DataFmt.FULLTM.toLowerCase())) {
			return dt.trim();
		}

		String[] tt = dt.split(DataFmt.TMSETDELIMIT);  //yyyy-mm-dd,hh:mm:ss~yyyy-mm-dd,hh:mm:ss
		tt[0]= tt[0].trim();
		tt[1]=tt[1].trim();
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
		//int start_s =s; //keep a copy

		Calendar cl = Calendar.getInstance();
		cl.set(y,mm-1,d,h,m,s);
		cl.set(Calendar.MILLISECOND, 0);

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
		cl2.set(y,mm-1,d,h,m,s);
		cl2.set(Calendar.MILLISECOND, 0);

		//time range in seconds
		int range =(int)(cl2.getTimeInMillis()/1000 - cl.getTimeInMillis()/1000) ;
		if (range<0){
			if (_dbg) System.out.println("The ending time is smaller than starting time. Use default");
			return DataFmt.FULLTM;
		}
		//int aj=0;

		String ret = cl.getTimeInMillis()+ DataFmt.TMSETDELIMIT;
		return ret+range;
	}



	private void checkBatchElements() {
		if (selVars.size()<1) {
			if (_dbg) System.out.println("No variables are read from the batch file, outputing all.");
			//System.exit(-1)
		}

		if (files[1]==null || files[1].isEmpty()) {
			if (_dbg) System.out.println("No Input file is read from the batch file. ");
		} 

		//format checking..........
		//check date
		String item = dataFmt[DataFmt.DATE_IDX];
		if (item!=null && !item.isEmpty() &&( item.equals(DataFmt.DATEDASH) || item.equals(DataFmt.DATESPACE) ||item.equals(DataFmt.NODATE))) {
		} else {
			if (_dbg) System.out.println("Can not find a good date format. Use default date format..."+ item);
			dataFmt[DataFmt.DATE_IDX]= DataFmt.DATEDASH;
		}
		//check time
		item = dataFmt[DataFmt.TM_IDX];
		if (item!=null && !item.isEmpty() &&( item.equals( DataFmt.TIMECOLON)  ||item.equals(DataFmt.TIMENOSPACE) || item.equals(DataFmt.TIMESPACE)|| item.equals(DataFmt.TIMESEC))) {
			if (item.equals(DataFmt.TIMESEC)) dataFmt[DataFmt.DATE_IDX]= DataFmt.NODATE; //if tm=SecOfDay set date=none
		} else {
			if (_dbg) System.out.println("Can not find a good time format. Use default format..."+ item);
			dataFmt[DataFmt.TM_IDX]= DataFmt.TIMECOLON;
		}
		//check delimiter
		item = dataFmt[DataFmt.DMTR_IDX];
		if (item !=null && !item.isEmpty() &&( item.equals(DataFmt.SPACEVAL) || item.equals(DataFmt.COMMAVAL))) {
		} else {
			if (_dbg) System.out.println("Can not find a good delimiter. Use default format..."+ item);
			dataFmt[DataFmt.DMTR_IDX]= DataFmt.COMMAVAL;
		}
		//check miss value
		item = dataFmt[DataFmt.MVAL_IDX];
		if (item==null || item.isEmpty() || item.equals(DataFmt.REPLICATE) || item.equals(DataFmt.MISSVAL)) { //empty =""
		} else {
			if (_dbg) System.out.println("Can not find a good fill value. Use default format..."+ item);
			dataFmt[DataFmt.MVAL_IDX]= DataFmt.MISSVAL;
		}
		//check time set
		item = dataFmt[DataFmt.TMSET_IDX]; boolean tmErr = false;
		if (item==null || item.isEmpty() ){
			tmErr = true;
		} else if (item.equals(DataFmt.FULLTM)) { 
		} else if (item.split(DataFmt.TMSETDELIMIT).length !=2) {
			tmErr = true;
		}
		if (tmErr) {
			if (_dbg) System.out.println("Invalid tmset. Use default format..."+ item);
			dataFmt[DataFmt.TMSET_IDX]= DataFmt.FULLTM;
		}

		item = dataFmt[DataFmt.HEAD_IDX];
		if (item != null && !item.isEmpty() &&
			(item.toLowerCase().equals(DataFmt.HDR_PLAIN.toLowerCase()) ||
			item.toLowerCase().equals(DataFmt.HDR_AMES.toLowerCase()) ||
			item.toLowerCase().equals(DataFmt.HDR_ICARTT.toLowerCase()) ||
			item.toLowerCase().equals(DataFmt.HDR_NCML.toLowerCase()))) {
		} else {
			if (_dbg) System.out.println("Can not find a good head title. Use default format..."+ item);
			dataFmt[DataFmt.HEAD_IDX]= DataFmt.HDR_PLAIN;
		}
		item = dataFmt[DataFmt.AVG_IDX];
		if (item==null || item.isEmpty() || Integer.parseInt(item)<1) {
			if (_dbg) System.out.println("Can not find a good average. Use default format..."+ item);
			dataFmt[DataFmt.AVG_IDX]="1";
		}

		//check AmesDEF, if it is selected, date=NODATE and tm=00Z, seperator=space, missdata=fillvalue,
		item = dataFmt[DataFmt.HEAD_IDX];
		if (item.equals(DataFmt.HDR_AMES) || item.equals(DataFmt.HDR_ICARTT)) {
			dataFmt[DataFmt.DATE_IDX]= DataFmt.NODATE;
			dataFmt[DataFmt.TM_IDX]= DataFmt.TIMESEC;
			dataFmt[DataFmt.MVAL_IDX]= DataFmt.MISSVAL;
			dataFmt[DataFmt.DMTR_IDX]= DataFmt.SPACEVAL;
		}

	}

}
