package edu.ucar.eol.nc2AscData;

import java.io.IOException;
import edu.ucar.eol.nc2Asc.*;

public class NCDataException extends IOException{
	private String msg="NCDataException: ";
	
	NCDataException(String msgs) {
		//super(msgs); 
		msg +=msgs+ super.getMessage();			
	}
	
	public String getMessage(){
		return msg;
	}
	
	public void prtMsg() {
		NC2Act.wrtMsg(msg);
	}
	
	
}
