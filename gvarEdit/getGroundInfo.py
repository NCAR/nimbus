#Julian Quick
#This function is called to call information from the aircraft SQL and groundvars
import psycopg2
import os
def getInfo(self):
    foo = psycopg2.connect(database="real-time", user="ads", host="steam")
    cur=foo.cursor()
    cur.execute("SELECT * FROM variable_list")
    sqlnames=cur.fetchall()
    sqlnames=[i[0] for i in sqlnames]
    sqlnames.sort()

    grn=open(os.path.expandvars(self.gvfilename),'r')
    groundlines=grn.readlines()
    groundlines=map(lambda s: s.strip(),groundlines)
    grn.close()
    return sqlnames,groundlines
