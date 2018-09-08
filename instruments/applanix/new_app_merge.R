require(Ranadu)
require(signal)
#library(knitr)
Flight <- "rf15"
Directory <- "/scr/raf_data/"
Project <- "DEEPWAVE"
fname = sprintf("%s%s%s_applanix_sr.nc", Directory, Project,Flight)
# copy to a new file before adding variables to it:
fnew = sprintf("%s%s/%s%s_applanixmerge_sr.nc", Directory, Project,Project,Flight)
system(paste ("cp", fname, fnew, sep=' '), wait=FALSE)
VarNames <- c("GGALT")
Data<-getNetCDF(fname, VarNames) # high-rate data OK here
#netcdffile starts at 7688 seconds, ends at 23711
starttime=7688
stoptime=23711
attach(Data)
DL <- length(GGALT)
DLL <- length(GGALT)/25
#define initial variables
LAT_APP<-vector("numeric",DL)
LON_APP<-vector("numeric",DL)
HT_APP<-vector("numeric",DL)
XVEL_APP<-vector("numeric",DL)
YVEL_APP<-vector("numeric",DL)
ZVEL_APP<-vector("numeric",DL)
ROLL_APP<-vector("numeric",DL)
PITCH_APP<-vector("numeric",DL)
PHDG_APP<-vector("numeric",DL)
WANGLE_APP<-vector("numeric",DL)
THDG_APP<-vector("numeric",DL)
XACCEL_APP<-vector("numeric",DL)
YACCEL_APP<-vector("numeric",DL)
ZACCEL_APP<-vector("numeric",DL)
XANGRATE_APP<-vector("numeric",DL)
YANGRATE_APP<-vector("numeric",DL)
ZANGRATE_APP<-vector("numeric",DL)
VELNORTH_APP<-vector("numeric",DL)
VELEAST_APP<-vector("numeric",DL)
#put data into above formed vectors
mydata=read.table("RF15_app_wyo.txt")
for (i in 1:DL) {
datatime<-mydata[i+1,1]
if (datatime<starttime){
  LAT_APP[i]=-32767.
  LON_APP[i]=-32767.
  HT_APP[i]=-32767.
  XVEL_APP[i]=-32767.
  YVEL_APP[i]=-32767.
  ZVEL_APP[i]=-32767.
  ROLL_APP[i]=-32767.
  PITCH_APP[i]=-32767.
  PHDG_APP[i]=-32767.
  WANGLE_APP[i]=-32767.
  THDG_APP[i]=-32767.
  XACCEL_APP[i]=-32767.
  YACCEL_APP[i]=-32767.
  ZACCEL_APP[i]=-32767.
  XANGRATE_APP[i]=-32767.
  YANGRATE_APP[i]=-32767.
  ZANGRATE_APP[i]=-32767.
  VELNORTH_APP[i]=-32767.
  VELEAST_APP[i]=-32767.
}
else if (datatime>=stoptime-1){
  LAT_APP[i]=-32767.
  LON_APP[i]=-32767.
  HT_APP[i]=-32767.
  XVEL_APP[i]=-32767.
  YVEL_APP[i]=-32767.
  ZVEL_APP[i]=-32767.
  ROLL_APP[i]=-32767.
  PITCH_APP[i]=-32767.
  PHDG_APP[i]=-32767.
  WANGLE_APP[i]=-32767.
  THDG_APP[i]=-32767.
  XACCEL_APP[i]=-32767.
  YACCEL_APP[i]=-32767.
  ZACCEL_APP[i]=-32767.
  XANGRATE_APP[i]=-32767.
  YANGRATE_APP[i]=-32767.
  ZANGRATE_APP[i]=-32767.
  VELNORTH_APP[i]=-32767.
  VELEAST_APP[i]=-32767. 
}
else{
  LAT_APP[i]=mydata[i+1,2]
  LON_APP[i]=mydata[i+1,3]
  HT_APP[i]=mydata[i+1,4]
  XVEL_APP[i]=mydata[i+1,5]
  YVEL_APP[i]=mydata[i+1,6]
  ZVEL_APP[i]=mydata[i+1,7]
  ROLL_APP[i]=mydata[i+1,8]
  PITCH_APP[i]=mydata[i+1,9]
  PHDG_APP[i]=mydata[i+1,10]
  WANGLE_APP[i]=mydata[i+1,11]
  THDG_APP[i]=mydata[i+1,12]
  XACCEL_APP[i]=mydata[i+1,13]
  YACCEL_APP[i]=mydata[i+1,14]
  ZACCEL_APP[i]=mydata[i+1,15]
  XANGRATE_APP[i]=mydata[i+1,16]
  YANGRATE_APP[i]=mydata[i+1,17]
  ZANGRATE_APP[i]=mydata[i+1,18]
  VELNORTH_APP[i]=mydata[i+1,19]
  VELEAST_APP[i]=mydata[i+1,20]
}  
  
}
#open netcdffile to write new variables
netCDFfile<-open.ncdf(fnew,write=TRUE)
#create new variable dimensions
dim(LAT_APP)<-c(10,DLL)
dim(LON_APP)<-c(10,DLL)
dim(HT_APP)<-c(10,DLL)
dim(XVEL_APP)<-c(10,DLL)
dim(YVEL_APP)<-c(10,DLL)
dim(ZVEL_APP)<-c(10,DLL)
dim(ROLL_APP)<-c(10,DLL)
dim(PITCH_APP)<-c(10,DLL)
dim(PHDG_APP)<-c(10,DLL)
dim(WANGLE_APP)<-c(10,DLL)
dim(THDG_APP)<-c(10,DLL)
dim(XACCEL_APP)<-c(10,DLL)
dim(YACCEL_APP)<-c(10,DLL)
dim(ZACCEL_APP)<-c(10,DLL)
dim(XANGRATE_APP)<-c(10,DLL)
dim(YANGRATE_APP)<-c(10,DLL)
dim(ZANGRATE_APP)<-c(10,DLL)
dim(VELNORTH_APP)<-c(10,DLL)
dim(VELEAST_APP)<-c(10,DLL)

#define new variables to add to netcdf files
varLAT_APP<-var.def.ncdf("LAT_APP","degree",c(fnew$dim["sps10"],fnew$dim["TIME"]),
                           -32767.,"Applanix Latitude")
varLON_APP<-var.def.ncdf("LON_APP","degree",c(fnew$dim["sps10"],fnew$dim["TIME"]),
                           -32767.,"Applanix Longitude")
varHT_APP<-var.def.ncdf("HT_APP","m",c(fnew$dim["sps10"],fnew$dim["TIME"]),
                         -32767.,"Applanix Ellipsoid Height")
varXVEL_APP<-var.def.ncdf("XVEL_APP","m/s",c(fnew$dim["sps10"],fnew$dim["TIME"]),
                         -32767.,"Applanix X-direction velocity")
varYVEL_APP<-var.def.ncdf("YVEL_APP","m/s",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix Y-direction velocity")
varZVEL_APP<-var.def.ncdf("ZVEL_APP","m/s",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix Z-direction velocity")
varROLL_APP<-var.def.ncdf("ROLL_APP","degree",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix aircraft roll")
varPITCH_APP<-var.def.ncdf("PITCH_APP","degree",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix aircraft pitch")
varPHDG_APP<-var.def.ncdf("PHDG_APP","degree",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix platform heading")
varWANGLE_APP<-var.def.ncdf("WANGLE_APP","degree",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix wander angle")
varTHDG_APP<-var.def.ncdf("THDG_APP","deg",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix true heading")
varXACCEL_APP<-var.def.ncdf("XACCEL_APP","m/sec2",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                          -32767.,"Applanix X-direction acceleration")
varYACCEL_APP<-var.def.ncdf("YACCEL_APP","m/sec2",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                            -32767.,"Applanix Y-direction acceleration")
varZACCEL_APP<-var.def.ncdf("ZACCEL_APP","m/sec2",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                            -32767.,"Applanix Z-direction acceleration")
varXANGRATE_APP<-var.def.ncdf("XANGRATE_APP","deg/sec",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                            -32767.,"Applanix X-direction angular rate")
varYANGRATE_APP<-var.def.ncdf("YANGRATE_APP","deg/sec",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                              -32767.,"Applanix Y-direction angular rate")
varZANGRATE_APP<-var.def.ncdf("ZANGRATE_APP","deg/sec",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                              -32767.,"Applanix Z-direction angular rate")
varVNORTH_APP<-var.def.ncdf("VNORTH_APP","m/s",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                              -32767.,"Applanix northward velocity")
varVEAST_APP<-var.def.ncdf("VEAST_APP","m/s",c(netcdf$dim["sps10"],netcdf$dim["TIME"]),
                              -32767.,"Applanix eastward velocity")
#add new variable definitions to the netcdf file
newfile<-var.add.ncdf(netCDFfile,varLAT_APP)
newfile<-var.add.ncdf(fnew,varLON_APP)
newfile<-var.add.ncdf(fnew,varHT_APP)
newfile<-var.add.ncdf(fnew,varROLL_APP)
newfile<-var.add.ncdf(fnew,varPITCH_APP)
newfile<-var.add.ncdf(fnew,varPHDG_APP)
newfile<-var.add.ncdf(fnew,varWANGLE_APP)
newfile<-var.add.ncdf(fnew,varTHDG_APP)
newfile<-var.add.ncdf(fnew,varXACCEL_APP)
newfile<-var.add.ncdf(fnew,varYACCEL_APP)
newfile<-var.add.ncdf(fnew,varZACCEL_APP)
newfile<-var.add.ncdf(fnew,varXANGRATE_APP)
newfile<-var.add.ncdf(fnew,varYANGRATE_APP)
newfile<-var.add.ncdf(fnew,varZANGRATE_APP)
newfile<-var.add.ncdf(fnew,varVNORTH_APP)
newfile<-var.add.ncdf(fnew,varVEAST_APP)
#put new variables
put.var.ncdf(newfile,"LAT_APP",LAT_APP)
put.var.ncdf(newfile,"LON_APP",LON_APP)
put.var.ncdf(newfile,"HT_APP",HT_APP)
put.var.ncdf(newfile,"ROLL_APP",ROLL_APP)
put.var.ncdf(newfile,"PITCH_APP",PITCH_APP)
put.var.ncdf(newfile,"PHDG_APP",PHDG_APP)
put.var.ncdf(newfile,"WANGLE_APP",WANGLE_APP)
put.var.ncdf(newfile,"THDG_APP",THDG_APP)
put.var.ncdf(newfile,"XACCEL_APP",XACCEL_APP)
put.var.ncdf(newfile,"YACCEL_APP",YACCEL_APP)
put.var.ncdf(newfile,"ZACCEL_APP",ZACCEL_APP)
put.var.ncdf(newfile,"XANGRATE_APP",XANGRATE_APP)
put.var.ncdf(newfile,"YANGRATE_APP",YANGRATE_APP)
put.var.ncdf(newfile,"ZANGRATE_APP",ZANGRATE_APP)
put.var.ncdf(newfile,"VNORTH_APP",VNORTH_APP)
put.var.ncdf(newfile,"VEAST_APP",VEAST_APP)
close.ncdf(newfile)
close.ncdf(netCDFfile)