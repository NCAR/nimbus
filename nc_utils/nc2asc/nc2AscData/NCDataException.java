package nc2AscData;

import java.util.*;
import java.lang.*;
import java.io.*;

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
		nc2Asc.NC2Act.wrtMsg(msg);
	}
	
	
}
