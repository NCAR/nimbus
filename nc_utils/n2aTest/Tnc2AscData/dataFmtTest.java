package tnc2AscData;


import junit.framework.TestCase;
import java.lang.*;
import java.lang.Exception.*;
import java.io.*;
import java.util.*;
import ucar.nc2.*;
import ucar.ma2.*;
import junit.*;
import junit.framework.*;
import junit.runner.*;
import junit.textui.*;

import nc2AscData.*;
import nc2Asc.*;


public class dataFmtTest  extends TestCase { 

	private DataFmt dfmt;
	
	public void testDataFmtDef() {
		dfmt = new DataFmt();
		String[] fmt=dfmt.getDataFmt();
		TestCase.assertEquals(DataFmt.DATEDASH.toString(), fmt[DataFmt.DATE_IDX]);
		TestCase.assertEquals(DataFmt.TIMECOLON.toString(), fmt[DataFmt.TM_IDX]);
		TestCase.assertEquals(",", fmt[DataFmt.DMTR_IDX]);
		TestCase.assertEquals(DataFmt.MISSVAL.toString(), fmt[DataFmt.MVAL_IDX]);
		TestCase.assertEquals(DataFmt.FULLTM.toString(), fmt[DataFmt.TMSET_IDX]);
		TestCase.assertEquals(DataFmt.HEAD.toString(), fmt[DataFmt.HEAD_IDX]);
		TestCase.assertEquals("1", fmt[DataFmt.AVG_IDX]);
		
		//TestCase.assertEquals(DataFmt.DATEDASH.toString(), fmt[7]);
	}
	
	public void testFmtDate() {
		String[] d = new String[3];
		d[0]="1970"; d[1]="01"; d[2]="01";
		String dd ="";
		//try { dd= dfmt.fmtDate(d);} catch (Exception e) {TestCase.fail("test_fmtDate");}
		//TestCase.assertEquals("1970-01-01", dd);
		
	}
	
}
