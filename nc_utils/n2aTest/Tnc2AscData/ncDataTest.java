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


public class ncDataTest extends TestCase {


	private NCData ncd;

	public void testReadVarData() {
		ncd= new NCData("/home/data/pac10.nc","/home/data/nctest.txt");
		try {ncd.openFile();} catch (IOException e) { TestCase.fail("NCData_openFile_IOException");}
		TestCase.assertNotNull(ncd.getInFile());
		TestCase.assertNotNull(ncd.getOutFile());

		try {ncd.readDataInf();} catch (IOException e) { TestCase.fail("NCData_readDataInf_IOException");}
		TestCase.assertNotNull(ncd.getDataInf());
		TestCase.assertTrue(ncd.getDataInf().length>1);

		List<Variable> sublvars = new ArrayList<Variable> ();
		sublvars.add(ncd.getVars().get(1));
		sublvars.add(ncd.getVars().get(2));
		sublvars.add(ncd.getVars().get(3));
		sublvars.add(ncd.getVars().get(4));

		try {
			String data = ncd.readOneVarData(sublvars.get(0), 0);
			TestCase.assertEquals("A1DC_LWO", sublvars.get(0).getName());
			TestCase.assertEquals(64, data.split(",").length);
			
			data = ncd.readOneVarData(sublvars.get(1), 0);
			TestCase.assertEquals("A2DC_LWO", sublvars.get(1).getName());
			TestCase.assertEquals(128, data.split(",").length);
			
			data = ncd.readOneVarData(sublvars.get(2), 0);
			TestCase.assertEquals("A2DTEMP_AEROSOL", sublvars.get(2).getName());
			TestCase.assertEquals(1, data.split(",").length);
						
		} catch (InvalidRangeException e) {
			TestCase.fail("readOneVarData_InvalidRangeException...");
		} catch (IOException ee) {
			TestCase.fail("readOneVarData_IOException...");
		}
		//return true;
	}


	//public static Test suite(){
		//return new TestSuite(ncDataTest.class);
	//}

}
