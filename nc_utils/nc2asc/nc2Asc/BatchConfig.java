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


	BatchConfig(String[] args) {
		parseArgs(args);
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
		dataFmt = fmt;
	}
	
	
	public String[] getDataFmt() {
		return dataFmt;
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
	public void parseBatchFile() {
		if(files[0]==null) { return;}

		FileReader fr;
		try {
			fr = new FileReader (new File(files[0]));
			BufferedReader br = new BufferedReader (fr);
			selVars = new ArrayList<String>();
			
			String line = br.readLine();
			while (line != null) {
				line = br.readLine().trim();
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
			} //while 
			br.close();

		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	
	private String  parseTmSet(String dt) {
		String[] tt = dt.split(DataFmt.TMSETDELIMIT);
		return "";
	}

}//eof class