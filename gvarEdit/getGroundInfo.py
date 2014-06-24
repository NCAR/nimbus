#Julian Quick
#This function is called to call information from the aircraft SQL and groundvars
import psycopg2
def getInfo():
  #  foo = psycopg2.connect(database="real-time-C130", user="ads", host="eol-rt-data.fl-ext.ucar.edu")
    foo = psycopg2.connect(database="real-time", user="ads", host="hercules.raf-ext.ucar.edu")
    cur=foo.cursor()
    cur.execute("SELECT * FROM variable_list")
    sqlnames=cur.fetchall()
    sqlnames=[i[0] for i in sqlnames]
    sqlnames.sort()

    grn=open('groundvars','r')
    groundlines=grn.readlines()
    groundlines=map(lambda s: s.strip(),groundlines)
    grn.close()
    return sqlnames,groundlines
