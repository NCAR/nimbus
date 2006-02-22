/*
 * GroundVarsInfo.java
 *
 */

package Groundfeed;

import java.lang.*;
import java.io.*;
import java.util.*;

/*
 * Read the groundvars project file to determine variables to send to
 * the ground.
 */

public class GroundVarsInfo {

   protected String infile;
   protected BufferedReader indata = null;
   protected String sqlnames;

   public GroundVarsInfo(String groundvarsName)
       throws IOException {

      String data = null;

      this.infile = groundvarsName;
      try {
         indata = new BufferedReader(new FileReader(infile));
      } catch (FileNotFoundException e) {
         System.out.println("File not found " + infile);
      }
      try {
         sqlnames = indata.readLine();
         sqlnames.trim();
      } catch (IOException e) {
         System.out.println("End of data in file");
         indata.close();
      }
   }

   public String getNames() {
      return (sqlnames);
   }
}

