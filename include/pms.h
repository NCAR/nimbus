

#ifndef _PMS_H_
#define _PMS_H_

#include "nimbus.h"

void ReadPMSspecs(const char fileName[]);
void setProbeCount(const char * location, int count);


void ComputePMS1DParams(NR_TYPE radius[], NR_TYPE eaw[], NR_TYPE cell_size[],
        NR_TYPE dof[], float minRange, float resolution, size_t nDiodes,
        size_t length, float dof_const, size_t armDistance);

void ComputeDOF(NR_TYPE radius[], NR_TYPE tas, NR_TYPE dof[],
        size_t FirstBin, size_t LastBin, float RES, NR_TYPE RESPONSE_TIME);

void addProbeMetadata(var_base *varp, const char *technique, const char *sz_dist_type);
void addDOFtoAttrs(var_base *varp, NR_TYPE eaw[], NR_TYPE dof[]);

#endif
