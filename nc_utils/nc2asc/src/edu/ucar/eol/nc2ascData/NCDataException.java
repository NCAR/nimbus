package edu.ucar.eol.nc2ascData;

import java.io.IOException;
import edu.ucar.eol.nc2asc.*;

public class NCDataException extends IOException{
	private String msg="NCDataException: ";
	static final long serialVersionUID =0;


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
