/*
 * DbAccess.java
 *
 */

package Groundfeed;

import java.lang.*;
import java.sql.*;
import java.util.*;

/*
 * Access to the postgres database on the aircraft.
 */

public class DbAccess {

    /** Connection to the database
     */
    protected Connection connection;
    protected String dbHostName;
    protected String dbName;
    protected String newTime;
    protected String oldTime = "2006-01-01 01:01:01";
    protected String sqlString = null;
    protected String sqlString2 = "select value from global_attributes where key='EndTime';";
    protected Vector dataResults = new Vector();

    public DbAccess(String dbHostName, String dbName) {
       this.dbHostName = dbHostName;
       this.dbName = dbName;
    }

    public void openDB() throws Exception {
       try {
          String url ="jdbc:postgresql://" + dbHostName + "/" + dbName;
          Class.forName("org.postgresql.Driver");
          connection = DriverManager.getConnection(url, "ads", "");
       }
       catch (java.lang.Exception ex) {
          System.out.println("Error opening database.\n" + ex);
       }
    }

    public void closeDB() throws SQLException {
       connection.close();
    }

    public void createRequestString(GroundVarsInfo groundVarsInfo) {
       String varNames = groundVarsInfo.getNames();
       sqlString = "select datetime, " + varNames + " from raf_lrt where datetime >= '";
    }

    public void getNewData(LdmData outDataFile) {

       String datetimeData = new String("");

       outDataFile.openLdmFile();
       try {
          Statement statement = connection.createStatement();
          ResultSet res = statement.executeQuery(sqlString2);
// Position at first record.
          res.next();
          newTime = res.getString(1);
          newTime = newTime.substring(0, 19);
//          System.out.println("newTime: " + newTime + " oldTime: " + oldTime);
          if (newTime.equals(oldTime)) {
             System.out.println("No new data.\n");
          } else {
//             System.out.println(sqlString + oldTime + "';");
             res = statement.executeQuery(sqlString + oldTime + "';");
             ResultSetMetaData rsmd = res.getMetaData();
             int numVars = rsmd.getColumnCount();
             while (res.next()) {
                datetimeData = res.getString(1);
                if (datetimeData.substring(0,19).equals(oldTime))
                   continue;
                else {
                   for (int i = 1; i <= numVars; i++) {
                      Double number = new Double(res.getDouble(i + 1));
                      dataResults.add(number);
                   }
                   outDataFile.addSqlStatement(datetimeData, dataResults);
                   dataResults.clear();
                }
             }
             outDataFile.endSqlStatement(datetimeData);
             outDataFile.closeLdmFile();
             oldTime = newTime;
          }
       } catch (java.lang.Exception ex) {
          System.out.println("Error in query statement.\n" + ex);
       }
    }
}


