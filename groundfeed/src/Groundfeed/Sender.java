/*
 * Sender.java
 *
 */

package Groundfeed;

import java.lang.*;
import java.util.*;
import java.io.*;

import gnu.getopt.*;

/**
 * The program which creates the ground feed data.  First, the project groundvars file
 * is read to get the list of variables to send to the ground.  Then, the database on the
 * aircraft is read to get the data to send to the ground.  The files sent to the ground
 * are sql statements that also identify the plane and the type of statement in the file.
 * The files are sent to the ground via the LDM.
 */
public class Sender extends java.lang.Object {

    /** GroundVars File name
     */
    protected String groundvarsName;
    /** Database name.
     */
    protected String dbName;
    /** Database host name.
     */
    protected String dbHostName;
    /** Set true for verbose output.
     */
    protected boolean verbose = false;
    /** groundvars file.
     */
    protected GroundVarsInfo groundvarsInfo = null;
    protected DbAccess dataDB = null;
    protected LdmData dataFile = null;
    protected String outDirFile;
    protected String plane;
    /** Creates new Sender
     * @param dbHostName - Name of the host for db on plane.
     * @param dbName - Name of the correct db for data.
     * @param groundvarsName - Name of the project groundvars file that holds
     *    variables names that are sent to the ground.
     * @param verbose - Set true for verbose output to stdout.
     * @param outDirFile - Directory name + file prefix for output files.
     */
    public Sender(String dbHostName, String dbName, String groundvarsName, boolean verbose,
                  String outDirFile, String plane)
        throws IOException, Exception {

        System.out.println("Groundfeed.Sender: started at " + new Date());

        this.dbHostName = dbHostName;
        this.dbName = dbName;
        this.groundvarsName = groundvarsName;
        this.outDirFile = outDirFile;
        this.plane = plane;

        groundvarsInfo = new GroundVarsInfo(groundvarsName);
        dataFile = new LdmData(outDirFile, plane);
        dataDB = new DbAccess(dbHostName, dbName);
        dataDB.createRequestString(groundvarsInfo);
        while (true) {
           dataDB.openDB();
           dataDB.getNewData(dataFile);
           dataDB.closeDB();
           try {
              Thread.sleep(15000);
           } catch (InterruptedException e) {
           }
        }
    }

    /** Print the usage statement to stdout, and exit.
     */
    protected static void usage(int ret) {
        System.err.println("usage: Groundfeed.Sender\n" +
        "-g groundvarsfile\n" +
        "-v (verbose)\n" +
        "-h hostDBname\n" +
        "-n DBname\n" +
        "-o outdir+fileprefix\n" +
        "-c\n" +
        "-5\n");
        System.exit(ret);
    }

    /** Driver for groundfeed.
     *
     * @param args Command line arguments.
     */
    public static void main(java.lang.String[] args) {

        Getopt g = new Getopt("Groundfeed.Sender", args, "g:h:n:vo:c5");

        boolean verbose = false;
        String dbName = new String("");
        String dbHost = new String("");
        String groundvarsName = new String("");
        String outdirfile = new String("");
        String plane = new String("");

        int c;
        String arg;
        while ((c = g.getopt()) != -1) {
            switch(c) {
                case 'g':
                   groundvarsName = g.getOptarg();
                   break;
                case 'h':
                   dbHost = g.getOptarg();
                   break;
                case 'n':
                    dbName = g.getOptarg();
                    break;
                case 'v':
                   verbose = true;
                   break;
                case 'o':
                   outdirfile = g.getOptarg();
                   break;
                case 'c':
                   plane = "C130";
                   break;
                case '5':
                   plane = "G5";
                   break;
                default:
                   usage(1);
            }
        }

        if (groundvarsName.equals("") || dbName.equals("") || dbHost.equals("") ||
            plane.equals("") || outdirfile.equals("")) {
            usage(1);
        }

        try {
          Sender sdr = new Sender(dbHost, dbName, groundvarsName, verbose, outdirfile, plane);
        }
        catch (Exception exc) {
          System.err.println(exc);
          exc.printStackTrace();
          System.exit(1);
        }
    }
}
