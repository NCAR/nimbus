import junit.framework.Test;
import junit.framework.*;
import junit.framework.TestSuite;
import junit.*;
import Tnc2AscData.*;

public class n2aTestMain {
	
	 public static Test suite() {

	        TestSuite suite = new TestSuite();
	  
	        //
	        // test ncData
	        //
	       // suite.createTest(theClass, name)
	        suite.addTestSuite(ncDataTest.class);
	        //suite.addTest(ncDataTest.testReadVarData());
	        return suite;
	    }

	    /**
	     * Runs the test suite using the textual runner.
	     */
	    public static void main(String[] args) {
	        junit.textui.TestRunner.run(suite());
	    }


}
