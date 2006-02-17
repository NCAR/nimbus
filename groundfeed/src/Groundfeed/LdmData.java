/*
 * GroundVarsInfo.java
 *
 */

package Groundfeed;

import java.lang.*;
import java.io.*;
import java.util.*;

/*
 * Create the data files containing sql statements to be inserted into the LDM
 * queue for sending to the ground.
 */

public class LdmData {

   protected String outfile;
   protected BufferedWriter outdata = null;
   protected Vector data = new Vector();
   protected int counter = 100;
   protected String plane;
   protected String currentFile;
   protected int dataRecords = 0;
   protected String sqlString1 = new String("insert into raf_lrt values('");
   protected String sqlString2 = new String("update global_attributes set value='");
   protected String sqlString2end = new String("' where key='EndTime';");

   public LdmData(String outFile, String plane) {

      this.outfile = outFile;
      this.plane = plane;
   }

   public void openLdmFile() {

      counter++;
      String sNumber = "" + counter;
      while (sNumber.length() < 5) {
         sNumber = "0" + sNumber;
      }
      currentFile = outfile + plane + "_" + sNumber;
      try {
         outdata = new BufferedWriter(new FileWriter(currentFile));
         outdata.write(plane);
         outdata.newLine();
         outdata.write("sql");
         outdata.newLine();
      } catch (IOException e) {
         System.out.println("File can not be created or written to." + currentFile);
      }
   }

   public void addSqlStatement(String datetime, Vector dataResults) {

      String statement = sqlString1 + datetime.substring(0, 19) + "'";
      for (int i = 0; i < dataResults.size(); i++) {
         statement = statement + "," + dataResults.elementAt(i).toString();
      }
      statement = statement + ");";
      try {
         outdata.write(statement);
         outdata.newLine();
         dataRecords++;
      } catch (IOException e) {
         System.out.println("Error in writing to file " + currentFile);
      }
   }

   public void endSqlStatement(String datetime) {

      String statement = sqlString2 + datetime + sqlString2end;
      try {
         outdata.write(statement);
         outdata.newLine();
      } catch (IOException e) {
         System.out.println("Error in writing end statement to file " + currentFile);
      }
   }

   public void closeLdmFile() {

      System.out.println(dataRecords + " data records sent to ground.");
      try {
         outdata.close();
      } catch (IOException e) {
         System.out.println("File can not be closed" + currentFile);
      }
      try {
         Process child = Runtime.getRuntime().exec("gzip " + currentFile);
         child = Runtime.getRuntime().exec("/home/local/bin/pqinsert " + currentFile + ".gz");
      } catch (IOException e) {
         System.out.println("Could not execute command" + e);
      }
  }
}

