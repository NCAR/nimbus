# to run, setenv PROJ_DIR /tmp
rm VDB.xml
python VDBHeader.py
rm $PROJ_DIR/$PROJECT/$AIRCRAFT/VarDB.nc
make  
./vdb2xml $PROJ_DIR/$PROJECT/$AIRCRAFT/VarDB 
python standardNameCorrection.py
python appendInfo.py
#sudo mv VDB.xml $PROJ_DIR/Configuration/raf/

