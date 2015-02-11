--
-- PostgreSQL database dump
--

-- Started on 2015-02-11 19:42:09 UTC

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- TOC entry 1881 (class 1262 OID 16385)
-- Name: real-time; Type: DATABASE; Schema: -; Owner: -
--

CREATE DATABASE "real-time" WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';


\connect "real-time"

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 149 (class 1259 OID 42603263)
-- Dependencies: 3
-- Name: a1dc_lp; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE a1dc_lp (
    datetime time(3) without time zone NOT NULL,
    nslices integer,
    particle integer[]
);


--
-- TOC entry 142 (class 1259 OID 41939306)
-- Dependencies: 3
-- Name: camera; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE camera (
    last_update timestamp with time zone,
    status integer,
    message text,
    guid text[],
    direction text[],
    latest text[]
);


--
-- TOC entry 145 (class 1259 OID 42603239)
-- Dependencies: 3
-- Name: categories; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE categories (
    variable text,
    category text
);


--
-- TOC entry 143 (class 1259 OID 42603223)
-- Dependencies: 3
-- Name: global_attributes; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE global_attributes (
    key text NOT NULL,
    value text
);


--
-- TOC entry 140 (class 1259 OID 3239617)
-- Dependencies: 3
-- Name: lightning; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE lightning (
    datetime timestamp without time zone,
    lat double precision,
    lon double precision,
    amplitude double precision
);


--
-- TOC entry 141 (class 1259 OID 5266572)
-- Dependencies: 3
-- Name: mission_control; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mission_control (
    key text NOT NULL,
    value text
);


--
-- TOC entry 146 (class 1259 OID 42603245)
-- Dependencies: 3
-- Name: parcels; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE parcels (
    id text,
    datetime timestamp without time zone
);


--
-- TOC entry 147 (class 1259 OID 42603251)
-- Dependencies: 3
-- Name: pms1d_list; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE pms1d_list (
    name text,
    serialnumber text,
    sampleratetable text,
    firstbin integer,
    lastbin integer,
    cellsizes double precision[]
);


--
-- TOC entry 148 (class 1259 OID 42603257)
-- Dependencies: 3
-- Name: pms2d_list; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE pms2d_list (
    name text,
    serialnumber text
);


--
-- TOC entry 144 (class 1259 OID 42603231)
-- Dependencies: 3
-- Name: variable_list; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE variable_list (
    name text NOT NULL,
    units text,
    uncalibrated_units text,
    long_name text,
    sampleratetable text,
    ndims integer,
    dims integer[],
    ncals integer,
    poly_cals double precision[],
    missing_value double precision,
    data_quality text
);


--
-- TOC entry 1878 (class 0 OID 42603263)
-- Dependencies: 149
-- Data for Name: a1dc_lp; Type: TABLE DATA; Schema: public; Owner: -
--

COPY a1dc_lp (datetime, nslices, particle) FROM stdin;
\.


--
-- TOC entry 1871 (class 0 OID 41939306)
-- Dependencies: 142
-- Data for Name: camera; Type: TABLE DATA; Schema: public; Owner: -
--

COPY camera (last_update, status, message, guid, direction, latest) FROM stdin;
2015-02-08 03:33:50.541516+00	1	Recording	{b09d0100a8f1b0,b09d01006e79b3}	{forward,down}	{150208-033350,150208-033350}
\.


--
-- TOC entry 1874 (class 0 OID 42603239)
-- Dependencies: 145
-- Data for Name: categories; Type: TABLE DATA; Schema: public; Owner: -
--

COPY categories (variable, category) FROM stdin;
A1DC_LPB	PMS Probe
A1DC_LPB	Raw
A2DTEMP_318	Raw
A2DTEMP_319	Raw
A2DTEMP_AFTL0	Raw
A2DTEMP_AFTL1	Raw
A2DTEMP_AFTR0	Raw
A2DTEMP_AFTR1	Raw
A2DTEMP_FWDFS0	Raw
A2DTEMP_FWDFS1	Raw
A2DTEMP_VID0	Raw
A2DTEMP_VID1	Raw
ABSPEC_VXL	Raw
ACDP_LPC	PMS Probe
ACDP_LPC	Raw
ACETONE_TOGA	Chemistry
ACETONE_TOGA	Raw
ACETONITRILE_TOGA	Chemistry
ACETONITRILE_TOGA	Raw
ACGAIN_VXL	Raw
ACINS	Aircraft State
ACINS	Raw
ACINS_IRS2	Aircraft State
ACINS_IRS2	Raw
ADIFR	Analog
AIRBM_AMS	Raw
ALT	Position
ALT	Raw
ALTHYB	Raw
ALTHYB_IRS2	Raw
ALT_A	Position
ALT_A	Raw
ALT_A2	Position
ALT_A2	Raw
ALT_IRS2	Position
ALT_IRS2	Raw
AS100_LPT	PMS Probe
AS100_LPT	Raw
AS200_RPT	PMS Probe
AS200_RPT	Raw
AT_A	Raw
AT_A2	Raw
AT_VXL	Raw
AUHSAS_RPC	PMS Probe
AUHSAS_RPC	Raw
AVGTRNS_LPC	Raw
BALNC_DPB	Raw
BALNC_DPT	Raw
BALT1_A	Raw
BALT1_A2	Raw
BALT2_A	Raw
BALT2_A2	Raw
BAROCOR1_A	Raw
BAROCOR1_A2	Raw
BAROCOR2_A	Raw
BAROCOR2_A2	Raw
BDIFR	Analog
BENZENE_TOGA	Chemistry
BENZENE_TOGA	Raw
BUTANE_TOGA	Chemistry
BUTANE_TOGA	Raw
BUTENE_TOGA	Raw
CAS_A	Raw
CAS_A2	Raw
CAVP_DPB	Analog
CAVP_DPT	Analog
CDP5VMON_LPC	Raw
CDPCBTMP_LPC	Raw
CDPLBTMP_LPC	Raw
CDPLSRC_LPC	Raw
CDPLSRP_LPC	Raw
CDPQBDW_LPC	Raw
CDPQBLINE_LPC	Raw
CDPQTHRSH_LPC	Raw
CDPSBDW_LPC	Raw
CDPSBLINE_LPC	Raw
CDPSTHRSH_LPC	Raw
CDPWBTMP_LPC	Raw
CH2CL2_TOGA	Chemistry
CH2CL2_TOGA	Raw
CH3CHO_TOGA	Raw
CH3OH_TOGA	Chemistry
CH3OH_TOGA	Raw
CH4C_PIC2311	Analog
CH4_PIC2311	Chemistry
CH4_PIC2311	Raw
CL_AMS	Raw
CNTEMP	Analog
CNTRCUR_VXL	Raw
CNTS	Uncorr'd Raw
CNTS	Raw
CO2C_PIC2311	Analog
CO2_PIC2311	Chemistry
CO2_PIC2311	Raw
COFLOW_AL	Analog
CONC_ISAF	Raw
CORAW_AL	Analog
DAY	Raw
DELTAT_LPC	Raw
DELTAT_LPT	Raw
DELTAT_RPT	Raw
DRFTA	Aircraft State
DRFTA	Raw
DRFTA_IRS2	Aircraft State
DRFTA_IRS2	Raw
DT1DC_LPB	PMS Probe
DT1DC_LPB	Raw
DUMMY_AFTL1	Analog
F2DIO1_LPB	Analog
F2DIO32_LPB	Analog
F2DIO64_LPB	Analog
F43_AMS	Raw
F44_AMS	Raw
F57_AMS	Raw
F60_AMS	Raw
FCN	Analog
FILEM_AMS	Raw
FLOW_AMS	Raw
FO3_ACD	Chemistry
FO3_ACD	Raw
FREF_LPT	PMS Probe
FREF_LPT	Raw
FRNG_LPT	PMS Probe
FRNG_LPT	Raw
FTMP_LPT	PMS Probe
FTMP_LPT	Raw
GASFLOW_ISAF	Raw
GCNTR1_ISAF	Raw
GCNTR3_ISAF	Raw
GGALT	Position
GGALT	Raw
GGALTSD	Raw
GGDAGE	Raw
GGEOIDHT	Raw
GGHORDIL	Raw
GGLAT	Position
GGLAT	Raw
GGLATSD	Raw
GGLON	Position
GGLON	Raw
GGLONSD	Raw
GGNSAT	Raw
GGQUAL	Raw
GGREFID	Raw
GGRepLag	Raw
GGSPD	Raw
GGSTATUS	Raw
GGTRK	Aircraft State
GGTRK	Raw
GGVEW	Aircraft State
GGVEW	Raw
GGVNS	Aircraft State
GGVNS	Raw
GGVSPD	Raw
GSPD	Raw
GSPDHYB	Raw
GSPDHYB_IRS2	Raw
GSPD_A	Raw
GSPD_A2	Raw
GSPD_IRS2	Raw
GSTAT_A	Housekeeping
GSTAT_A	Raw
GSTAT_A2	Housekeeping
GSTAT_A2	Raw
GTIME_A	Raw
GTIME_A2	Raw
GVEW_A	Aircraft State
GVEW_A	Raw
GVEW_A2	Aircraft State
GVEW_A2	Raw
GVNS_A	Aircraft State
GVNS_A	Raw
GVNS_A2	Aircraft State
GVNS_A2	Raw
H2O_PIC2311	Chemistry
H2O_PIC2311	Raw
HCHO_TOGA	Raw
HCN_TOGA	Chemistry
HCN_TOGA	Raw
HGM232	Position
HGM232	Raw
HOUR	Raw
HYBHFOM	Raw
HYBHFOM_IRS2	Raw
HYBVFOM	Raw
HYBVFOM_IRS2	Raw
INLETP_AL	Analog
INLETP_AMS	Analog
IRIG_Status_311	Raw
IRIG_Status_318	Raw
IRIG_Status_319	Raw
IRIG_Status_320	Raw
IRIG_Status_321	Raw
IRIG_Status_322	Raw
IRIG_Status_323	Raw
IRIG_Status_324	Raw
IRIG_Status_325	Raw
IRIG_Tdiff_311	Raw
IRIG_Tdiff_318	Raw
IRIG_Tdiff_319	Raw
IRIG_Tdiff_320	Raw
IRIG_Tdiff_321	Raw
IRIG_Tdiff_322	Raw
IRIG_Tdiff_323	Raw
IRIG_Tdiff_324	Raw
IRIG_Tdiff_325	Raw
ISOPRENE_TOGA	Uncorr'd Raw
ISOPRENE_TOGA	Raw
IWD	Raw
IWD_IRS2	Raw
IWS	Raw
IWS_IRS2	Raw
LAT	Position
LAT	Raw
LATF_A	Raw
LATF_A2	Raw
LATHYB	Raw
LATHYB_IRS2	Raw
LAT_A	Position
LAT_A	Raw
LAT_A2	Position
LAT_A2	Raw
LAT_IRS2	Position
LAT_IRS2	Raw
LENSP_AMS	Raw
LON	Position
LON	Raw
LONF_A	Raw
LONF_A2	Raw
LONHYB	Raw
LONHYB_IRS2	Raw
LON_A	Position
LON_A	Raw
LON_A2	Position
LON_A2	Raw
LON_IRS2	Position
LON_IRS2	Raw
LPWR1_ISAF	Raw
LPWR2_ISAF	Raw
LSETV_ISAF	Raw
LSRINT_VXL	Raw
LSRTMP_VXL	Raw
MACH_A	Raw
MACH_A2	Raw
MEK_TOGA	Raw
MHDG	Raw
MHDG_IRS2	Raw
MINUTE	Raw
MIRRTMP_DPB	Raw
MIRRTMP_DPT	Raw
MODE_VXL	Raw
MONPWRIN_ISAF	Raw
MONPWROUT_ISAF	Raw
MONTH	Raw
N2O5_ARN	Raw
NCNTR2_ISAF	Raw
NO2_ARN	Chemistry
NO2_ARN	Raw
NO3_AMS	Raw
NO3_ARN	Raw
NOCAL	Raw
NOREC	Raw
NOX_ARN	Chemistry
NOX_ARN	Raw
NOY_ACD	Chemistry
NOY_ACD	Raw
NOY_ARN	Chemistry
NOY_ARN	Raw
NO_ACD	Chemistry
NO_ACD	Raw
O3_AMS	Analog
OA_AMS	Raw
OFRUV_AMS	Raw
ORG43_AMS	Raw
ORG44_AMS	Raw
ORG57_AMS	Raw
OVFLW_LPC	PMS Probe
OVFLW_LPC	Raw
OVFLW_LPT	PMS Probe
OVFLW_LPT	Raw
OX_ARN	Raw
PALT_A	Position
PALT_A	Raw
PALT_A2	Position
PALT_A2	Raw
PCAB	Analog
PCIF_AMS	Analog
PCN	Analog
PFLWS_RPT	Analog
PFLW_RPT	Analog
PHGB_RPT	PMS Probe
PHGB_RPT	Raw
PITCH	Analog
PITCH_IRS2	Analog
PKPOS_VXL	Raw
PLGB_RPT	PMS Probe
PLGB_RPT	Raw
PLWC	Analog
PMGB_RPT	PMS Probe
PMGB_RPT	Raw
PREF_RPT	PMS Probe
PREF_RPT	Raw
PREGAIN_VXL	Raw
PSFD	Uncorr'd Raw
PSFD	Raw
PSFRD	Aircraft State
PSFRD	Raw
PS_A	Raw
PS_A2	Raw
PS_VXL	Analog
PTAB_AMS	Raw
PTMP_RPT	PMS Probe
PTMP_RPT	Raw
PT_A	Raw
PT_A2	Raw
PUMPIN3_ISAF	Raw
QCF	Analog
QCFR	Analog
QCR	Analog
QC_A	Raw
QC_A2	Raw
RAWCONC_VXL	Raw
REJAT_LPT	Housekeeping
REJAT_LPT	Raw
REJDOF_LPC	Housekeeping
REJDOF_LPC	Raw
REJDOF_LPT	Housekeeping
REJDOF_LPT	Raw
RICE	Analog
ROLL	Aircraft State
ROLL	Raw
ROLL_IRS2	Aircraft State
ROLL_IRS2	Raw
RSTB	Analog
RSTB1	Analog
RSTT	Analog
RTHL1	Analog
RTHL2	Analog
RT_A	Raw
RT_A2	Raw
SAMPFLG_AMS	Raw
SAMPLEF_ISAF	Raw
SAMPLEP_ISAF	Raw
SECOND	Raw
SELHYB	Raw
SELHYB_IRS2	Raw
SETTMP_VXL	Raw
SHDORC_LPB	PMS Probe
SHDORC_LPB	Raw
SMPLSRINT_VXL	Raw
SO2_NC	Chemistry
SO2_NC	Raw
SO4_AMS	Raw
STATUS_ISAF	Raw
STATUS_VXL	Raw
TAS_A	Aircraft State
TAS_A	Raw
TAS_A2	Aircraft State
TAS_A2	Raw
TCC_VXL	Raw
TEC_DPB	Raw
TEC_DPT	Raw
THDG	Analog
THDGHYB	Raw
THDGHYB_IRS2	Raw
THDG_IRS2	Analog
THIML1	Analog
THIML2	Analog
TIME_PIC2311	Raw
TKAT	Aircraft State
TKAT	Raw
TKAT_A	Aircraft State
TKAT_A	Raw
TKAT_A2	Aircraft State
TKAT_A2	Raw
TKAT_IRS2	Aircraft State
TKAT_IRS2	Raw
TOLUENE_TOGA	Chemistry
TOLUENE_TOGA	Raw
TRSTB	Analog
TVAP_AMS	Analog
UBTMP_RPC	Analog
UCURR_RPC	Raw
UDIFF_RPC	Raw
UPRESS_RPC	Analog
UREF_RPC	Raw
USCAT_RPC	Raw
USHFLW_RPC	Analog
USMPFLW_RPC	Analog
UTEMP_RPC	Raw
UTSEC_AMS	Raw
UVN315_HARP	Raw
UVN382_HARP	Raw
UVNRH_HARP	Raw
UVZ315_HARP	Raw
UVZ382_HARP	Raw
UVZRH_HARP	Raw
VALID_AMS	Raw
VEW	Aircraft State
VEW	Raw
VEWHYB	Raw
VEWHYB_IRS2	Raw
VEW_IRS2	Aircraft State
VEW_IRS2	Raw
VNS	Aircraft State
VNS	Raw
VNSHYB	Raw
VNSHYB_IRS2	Raw
VNS_IRS2	Aircraft State
VNS_IRS2	Raw
VSPD	Aircraft State
VSPD	Raw
VSPD_A	Aircraft State
VSPD_A	Raw
VSPD_A2	Aircraft State
VSPD_A2	Raw
VSPD_IRS2	Aircraft State
VSPD_IRS2	Raw
WOW_A	Raw
WOW_A2	Raw
XICN	Analog
YEAR	Raw
AKRD	Aircraft State
AKRD	Derived
AQRATIO	Derived
ATHL1	Atmos. State
ATHL1	Derived
ATHL2	Atmos. State
ATHL2	Derived
ATTACK	Aircraft State
ATTACK	Derived
ATX	Atmos. State
ATX	Derived
C1DC_LPB	PMS Probe
C1DC_LPB	Derived
CCDP_LPC	PMS Probe
CCDP_LPC	Derived
CONC1DC100_LPB	PMS Probe
CONC1DC100_LPB	Derived
CONC1DC150_LPB	PMS Probe
CONC1DC150_LPB	Derived
CONC1DC_LPB	PMS Probe
CONC1DC_LPB	Derived
CONCD_LPC	PMS Probe
CONCD_LPC	Derived
CONCF_LPT	PMS Probe
CONCF_LPT	Derived
CONCN	Chemistry
CONCN	Derived
CONCP_RPT	PMS Probe
CONCP_RPT	Derived
CONCU100_RPC	PMS Probe
CONCU100_RPC	Derived
CONCU500_RPC	PMS Probe
CONCU500_RPC	Derived
CONCU_RPC	PMS Probe
CONCU_RPC	Derived
CONCV_VXL	Atmos. State
CONCV_VXL	Derived
CS100_LPT	PMS Probe
CS100_LPT	Derived
CS200_RPT	PMS Probe
CS200_RPT	Derived
CUHSAS_RPC	PMS Probe
CUHSAS_RPC	Derived
DBAR1DC_LPB	PMS Probe
DBAR1DC_LPB	Derived
DBARD_LPC	PMS Probe
DBARD_LPC	Derived
DBARF_LPT	PMS Probe
DBARF_LPT	Derived
DBARP_RPT	PMS Probe
DBARP_RPT	Derived
DBARU_RPC	PMS Probe
DBARU_RPC	Derived
DBZ1DC_LPB	PMS Probe
DBZ1DC_LPB	Derived
DBZD_LPC	PMS Probe
DBZD_LPC	Derived
DBZF_LPT	PMS Probe
DBZF_LPT	Derived
DISP1DC_LPB	PMS Probe
DISP1DC_LPB	Derived
DISPD_LPC	PMS Probe
DISPD_LPC	Derived
DISPF_LPT	PMS Probe
DISPF_LPT	Derived
DISPP_RPT	PMS Probe
DISPP_RPT	Derived
DISPU_RPC	PMS Probe
DISPU_RPC	Derived
DPXC	Atmos. State
DPXC	Derived
DP_DPB	Atmos. State
DP_DPB	Derived
DP_DPT	Atmos. State
DP_DPT	Derived
DP_VXL	Atmos. State
DP_VXL	Derived
DVALUE	Position
DVALUE	Derived
EWX	Thermodynamic
EWX	Derived
EW_DPB	Thermodynamic
EW_DPB	Derived
EW_DPT	Thermodynamic
EW_DPT	Derived
EW_VXL	Thermodynamic
EW_VXL	Derived
FCNC	Chemistry
FCNC	Derived
GGALTC	Position
GGALTC	Derived
JNO2_HARP	Chemistry
JNO2_HARP	Derived
LATC	Position
LATC	Derived
LONC	Position
LONC	Derived
MACHF	Derived
MACHFR	Derived
MACHR	Derived
MACHX	Derived
MR	Atmos. State
MR	Derived
ONE	Housekeeping
ONE	Derived
PALT	Position
PALT	Derived
PALTF	Position
PALTF	Derived
PDRY	Derived
PFLWC_RPT	PMS Probe
PFLWC_RPT	Derived
PLWC1DC_LPB	PMS Probe
PLWC1DC_LPB	Derived
PLWCC	Liquid Water
PLWCC	Derived
PLWCD_LPC	PMS Probe
PLWCD_LPC	Derived
PLWCF_LPT	PMS Probe
PLWCF_LPT	Derived
PSFC	Atmos. State
PSFC	Derived
PSFDC	Atmos. State
PSFDC	Derived
PSURF	Thermodynamic
PSURF	Derived
PSX	Uncorr'd Raw
PSX	Derived
PSXC	Atmos. State
PSXC	Derived
PVOLP_RPT	PMS Probe
PVOLP_RPT	Derived
PVOLU_RPC	PMS Probe
PVOLU_RPC	Derived
QCFC	Aircraft State
QCFC	Derived
QCFRC	Derived
QCRC	Aircraft State
QCRC	Derived
QCX	Uncorr'd Raw
QCX	Derived
QCXC	Aircraft State
QCXC	Derived
RALT	Position
RALT	Derived
REFF2DC_LPB	PMS Probe
REFF2DC_LPB	Derived
REFFD_LPC	PMS Probe
REFFD_LPC	Derived
REFFF_LPT	PMS Probe
REFFF_LPT	Derived
RHODT	Atmos. State
RHODT	Derived
RHUM	Atmos. State
RHUM	Derived
RTX	Uncorr'd Raw
RTX	Derived
SOLAZ	Derived
SOLDE	Derived
SOLEL	Derived
SOLZE	Derived
SSLIP	Aircraft State
SSLIP	Derived
SSRD	Aircraft State
SSRD	Derived
TASF	Aircraft State
TASF	Derived
TASFR	Aircraft State
TASFR	Derived
TASHC	Aircraft State
TASHC	Derived
TASR	Aircraft State
TASR	Derived
TASX	Aircraft State
TASX	Derived
TCNTD_LPC	PMS Probe
TCNTD_LPC	Derived
TCNTF_LPT	PMS Probe
TCNTF_LPT	Derived
TCNTP_RPT	PMS Probe
TCNTP_RPT	Derived
TCNTU_RPC	PMS Probe
TCNTU_RPC	Derived
THETA	Thermodynamic
THETA	Derived
THETAE	Thermodynamic
THETAE	Derived
THETAP	Thermodynamic
THETAP	Derived
THETAQ	Thermodynamic
THETAQ	Derived
THETAV	Thermodynamic
THETAV	Derived
TVIR	Derived
UFLWC_RPC	PMS Probe
UFLWC_RPC	Derived
UI	Wind
UI	Derived
UIC	Wind
UIC	Derived
UX	Wind
UX	Derived
UXC	Wind
UXC	Derived
VEWC	Aircraft State
VEWC	Derived
VI	Wind
VI	Derived
VIC	Wind
VIC	Derived
VMR_VXL	Atmos. State
VMR_VXL	Derived
VNSC	Aircraft State
VNSC	Derived
VY	Wind
VY	Derived
VYC	Wind
VYC	Derived
WD	Wind
WD	Derived
WDC	Wind
WDC	Derived
WI	Wind
WI	Derived
WIC	Wind
WIC	Derived
WS	Wind
WS	Derived
WSC	Wind
WSC	Derived
XICNC	Chemistry
XICNC	Derived
ZERO	Housekeeping
ZERO	Derived
\.


--
-- TOC entry 1872 (class 0 OID 42603223)
-- Dependencies: 143
-- Data for Name: global_attributes; Type: TABLE DATA; Schema: public; Owner: -
--

COPY global_attributes (key, value) FROM stdin;
Source	NCAR Research Aviation Facility
Address	P.O. Box 3000, Boulder, CO 80307-3000
Phone	(303) 497-1030
ProjectName	WINTER
Platform	N130AR
ProjectNumber	WINTER
FlightNumber	rf04
DateProcessed	2015-02-11T17:10:12 +0000
latitude_coordinate	GGLAT
longitude_coordinate	GGLON
zaxis_coordinate	GGALT
time_coordinate	datetime
wind_field	WSC WDC WIC
landmarks	39.9088 -105.117 jeffco
DataRate	1
checksum	badc00e81f075805164632753bea2075
StartTime	20150211T171007
EndTime	20150211T194206.990
\.


--
-- TOC entry 1869 (class 0 OID 3239617)
-- Dependencies: 140
-- Data for Name: lightning; Type: TABLE DATA; Schema: public; Owner: -
--

COPY lightning (datetime, lat, lon, amplitude) FROM stdin;
\.


--
-- TOC entry 1870 (class 0 OID 5266572)
-- Dependencies: 141
-- Data for Name: mission_control; Type: TABLE DATA; Schema: public; Owner: -
--

COPY mission_control (key, value) FROM stdin;
region	off
cappi	off
lightning	off
camera	forward
lightningTspan	10
\.


--
-- TOC entry 1875 (class 0 OID 42603245)
-- Dependencies: 146
-- Data for Name: parcels; Type: TABLE DATA; Schema: public; Owner: -
--

COPY parcels (id, datetime) FROM stdin;
\.


--
-- TOC entry 1876 (class 0 OID 42603251)
-- Dependencies: 147
-- Data for Name: pms1d_list; Type: TABLE DATA; Schema: public; Owner: -
--

COPY pms1d_list (name, serialnumber, sampleratetable, firstbin, lastbin, cellsizes) FROM stdin;
1DC_LPB	F2DC001	SampleRate1	2	62	{12.5,37.5,62.5,87.5,112.5,137.5,162.5,187.5,212.5,237.5,262.5,287.5,312.5,337.5,362.5,387.5,412.5,437.5,462.5,487.5,512.5,537.5,562.5,587.5,612.5,637.5,662.5,687.5,712.5,737.5,762.5,787.5,812.5,837.5,862.5,887.5,912.5,937.5,962.5,987.5,1012.5,1037.5,1062.5,1087.5,1112.5,1137.5,1162.5,1187.5,1212.5,1237.5,1262.5,1287.5,1312.5,1337.5,1362.5,1387.5,1412.5,1437.5,1462.5,1487.5,1512.5,1537.5,1562.5,1587.5}
BSPEC_VXL		SampleRate1	0	0	{}
CDP_LPC	CDP016	SampleRate10	0	29	{2,3,4,5,6,7,8,9,10,11,12,13,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50}
S100_LPT	FSSP109	SampleRate10	0	29	{0.050000000000000003,0.10000000000000001,1.6599999999999999,3.23,4.79,6.3499999999999996,7.9199999999999999,9.4800000000000004,11.039999999999999,12.609999999999999,14.17,15.73,17.300000000000001,18.859999999999999,20.420000000000002,21.989999999999998,23.550000000000001,25.109999999999999,26.68,28.239999999999998,29.800000000000001,31.370000000000001,32.93,34.490000000000002,36.060000000000002,37.619999999999997,39.18,40.75,42.310000000000002,43.869999999999997,47}
S200_RPT	PCAS108	SampleRate10	0	29	{0.097000000000000003,0.105,0.113,0.121,0.129,0.14499999999999999,0.16200000000000001,0.182,0.20200000000000001,0.222,0.24199999999999999,0.26200000000000001,0.28199999999999997,0.30199999999999999,0.40100000000000002,0.56999999999999995,0.65600000000000003,0.73999999999999999,0.83299999999999996,0.91700000000000004,1.008,1.1479999999999999,1.319,1.4790000000000001,1.6359999999999999,1.796,1.9550000000000001,2.1840000000000002,2.4129999999999998,2.661,2.9910000000000001}
UHSAS_RPC	UHSAS001	SampleRate10	0	98	{0.059999999999999998,0.06173,0.063509999999999997,0.065339999999999995,0.067220000000000002,0.069159999999999999,0.071150000000000005,0.073209999999999997,0.075319999999999998,0.077490000000000003,0.079719999999999999,0.082019999999999996,0.084379999999999997,0.086819999999999994,0.089319999999999997,0.091889999999999999,0.094539999999999999,0.097269999999999995,0.10007000000000001,0.10296,0.10592,0.10897999999999999,0.11212,0.11534999999999999,0.11867,0.1221,0.12561,0.12923999999999999,0.13295999999999999,0.13678999999999999,0.14074,0.14479,0.14896999999999999,0.15326000000000001,0.15767999999999999,0.16223000000000001,0.16689999999999999,0.17171,0.17666000000000001,0.18174999999999999,0.18698999999999999,0.19238,0.19792999999999999,0.20363999999999999,0.20951,0.21554000000000001,0.22176000000000001,0.22814999999999999,0.23472999999999999,0.24149000000000001,0.24845,0.25562000000000001,0.26297999999999999,0.27056999999999998,0.27836,0.28638999999999998,0.29464000000000001,0.30314000000000002,0.31187999999999999,0.32086999999999999,0.33012000000000002,0.33962999999999999,0.34942000000000001,0.35948999999999998,0.36986000000000002,0.38052000000000002,0.39149,0.40277000000000002,0.41438000000000003,0.42632999999999999,0.43862000000000001,0.45125999999999999,0.46427000000000002,0.47765000000000002,0.49142000000000002,0.50558999999999998,0.52015999999999996,0.53515000000000001,0.55057999999999996,0.56645000000000001,0.58277999999999996,0.59958,0.61685999999999996,0.63463999999999998,0.65293999999999996,0.67176000000000002,0.69111999999999996,0.71104999999999996,0.73153999999999997,0.75263000000000002,0.77432000000000001,0.79664000000000001,0.81960999999999995,0.84323000000000004,0.86753999999999998,0.89254999999999995,0.91827999999999999,0.94474999999999998,0.97189999999999999,1}
\.


--
-- TOC entry 1877 (class 0 OID 42603257)
-- Dependencies: 148
-- Data for Name: pms2d_list; Type: TABLE DATA; Schema: public; Owner: -
--

COPY pms2d_list (name, serialnumber) FROM stdin;
A1DC_LP	F2DC001
\.


--
-- TOC entry 1873 (class 0 OID 42603231)
-- Dependencies: 144
-- Data for Name: variable_list; Type: TABLE DATA; Schema: public; Owner: -
--

COPY variable_list (name, units, uncalibrated_units, long_name, sampleratetable, ndims, dims, ncals, poly_cals, missing_value, data_quality) FROM stdin;
A1DC_LPB	count		Fast 2DC Raw Accumulation, entire-in (per cell)	SampleRate1	2	{1,63}	0	{}	-32767	Preliminary
A2DTEMP_318	deg_C		A2DTemperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_319	deg_C		A2DTemperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_AFTL0	deg_C		A2D Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_AFTL1	deg_C		A2DTemperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_AFTR0	deg_C		A2D Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_AFTR1	deg_C		A2DTemperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_FWDFS0	deg_C		A2D Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_FWDFS1	deg_C		A2D Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_VID0	deg_C		A2D Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
A2DTEMP_VID1	deg_C		A2D Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ABSPEC_VXL	count		VCSEL Absorbption Spectrum	SampleRate1	2	{1,97}	0	{}	-32767	Preliminary
ACDP_LPC	count		CDP Raw Accumulation (per cell)	SampleRate10	2	{1,30}	0	{}	-32767	Preliminary
ACETONE_TOGA	pptv		TOGA Acetone	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ACETONITRILE_TOGA	pptv		TOGA Acetonitrile	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ACGAIN_VXL	count		VCSEL AC Gain	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ACINS	m/s2		IRS Vertical Acceleration	SampleRate50	1	{1}	0	{}	-32767	Preliminary
ACINS_IRS2	m/s2		IRS Vertical Acceleration	SampleRate50	1	{1}	0	{}	-32767	Preliminary
ADIFR	hPa	V	Vertical Differential Pressure, Radome	SampleRate100	1	{1}	0	{}	-32767	Preliminary
AIRBM_AMS	Hz		AMS Air Beam Reference	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ALT	m		IRS Altitude	SampleRate25	1	{1}	0	{}	-32767	Preliminary
ALTHYB	feet		Hybrid Altitude - 261	SampleRate2	1	{1}	0	{}	-32767	Preliminary
ALTHYB_IRS2	feet		Hybrid Altitude - 261	SampleRate2	1	{1}	0	{}	-32767	Preliminary
ALT_A	m		Avionics GPS Altitude (MSL)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ALT_A2	m		Avionics GPS Altitude (MSL)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ALT_IRS2	m		IRS Altitude	SampleRate25	1	{1}	0	{}	-32767	Preliminary
AS100_LPT	count		SPP-100 Raw Accumulation (per cell) - DMT	SampleRate10	2	{1,30}	0	{}	-32767	Preliminary
AS200_RPT	count		SPP-200 (PCASP) Raw Accumulation (per cell) - DMT	SampleRate10	2	{1,30}	0	{}	-32767	Preliminary
AT_A	deg_C		ADC Ambient Air Temperature	SampleRate2	1	{1}	0	{}	-32767	Preliminary
AT_A2	deg_C		ADC Ambient Air Temperature	SampleRate2	1	{1}	0	{}	-32767	Preliminary
AT_VXL	deg_K		VCSEL Ambient Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
AUHSAS_RPC	count		UHSAS Raw Count Histogram	SampleRate10	2	{1,99}	0	{}	-32767	Preliminary
AVGTRNS_LPC	microsecond		CDP Average Transit Time	SampleRate10	1	{1}	0	{}	-32767	Preliminary
BALNC_DPB	unk		Dew/Frost Point Balance	SampleRate1	1	{1}	0	{}	-32767	Preliminary
BALNC_DPT	unk		Dew/Frost Point Balance	SampleRate1	1	{1}	0	{}	-32767	Preliminary
BALT1_A	m		ADC Baro Altitude	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BALT1_A2	m		ADC Baro Altitude	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BALT2_A	m		ADC baro corrected alt #2	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BALT2_A2	m		ADC baro corrected alt #2	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BAROCOR1_A	hPa		ADC baro correction #1 mB	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BAROCOR1_A2	hPa		ADC baro correction #1 mB	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BAROCOR2_A	hPa		ADC baro correction #2 mB	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BAROCOR2_A2	hPa		ADC baro correction #2 mB	SampleRate16	1	{1}	0	{}	-32767	Preliminary
BDIFR	hPa	V	Horizontal Differential Pressure, Radome	SampleRate100	1	{1}	0	{}	-32767	Preliminary
BENZENE_TOGA	pptv		TOGA Benzene	SampleRate1	1	{1}	0	{}	-32767	Preliminary
BUTANE_TOGA	pptv		TOGA Butane	SampleRate1	1	{1}	0	{}	-32767	Preliminary
BUTENE_TOGA	pptv		TOGA Butene	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CAS_A	m/s		ADC Computed Air Speed	SampleRate8	1	{1}	0	{}	-32767	Preliminary
CAS_A2	m/s		ADC Computed Air Speed	SampleRate8	1	{1}	0	{}	-32767	Preliminary
CAVP_DPB	hPa	V	Dewpoint Cavity Pressure	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CAVP_DPT	hPa	V	Dewpoint Cavity Pressure, Top	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDP5VMON_LPC	Vdc		CDP 5 Vdc Monitor	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPCBTMP_LPC	deg_C		CDP Control Board Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPLBTMP_LPC	deg_C		CDP Laser Block Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPLSRC_LPC	mAmp		CDP Laser Current	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPLSRP_LPC	V		CDP Laser Power Monitor	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPQBDW_LPC	unk		CDP Qualifier DT Bandwidth	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPQBLINE_LPC	Vdc		CDP Qualifier Baseline	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPQTHRSH_LPC	unk		CDP Qualifier Dynamic Threshold	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPSBDW_LPC	unk		CDP Sizer DT Bandwidth	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPSBLINE_LPC	Vdc		CDP Sizer Baseline	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPSTHRSH_LPC	unk		CDP Sizer Dynamic Threshold	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CDPWBTMP_LPC	deg_C		CDP Wing Board Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CH2CL2_TOGA	pptv		TOGA Dichloromethane	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CH3CHO_TOGA	pptv		TOGA Acetaldehyde	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CH3OH_TOGA	pptv		TOGA Methanol	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CH4C_PIC2311	ppmv	ppmv	Picarro Methane Mixing Ratio (Corrected)	SampleRate20	1	{1}	0	{}	-32767	Preliminary
CH4_PIC2311	ppmv		Picarro Methane Mixing Ratio	SampleRate20	1	{1}	0	{}	-32767	Preliminary
CL_AMS	ug/m3		AMS Chloride PM1 mass as measured by HRAMS	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CNTEMP	deg_C	V	CN Counter Inlet Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CNTRCUR_VXL	count		VCSEL Center Current	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CNTS	count		TSI CN Counter Output	SampleRate100	1	{1}	0	{}	-32767	Preliminary
CO2C_PIC2311	ppmv	ppmv	Picarro Carbon Dioxide Mixing Ratio (Corrected)	SampleRate20	1	{1}	0	{}	-32767	Preliminary
CO2_PIC2311	ppmv		Picarro Carbon Dioxide Mixing Ratio	SampleRate20	1	{1}	0	{}	-32767	Preliminary
COFLOW_AL		V	Aerolaser Flow	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CONC_ISAF	pptv		Detector Concentration	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CORAW_AL	ppbv	V	Aerolaser (2005) Carbon Monoxide Raw Signal	SampleRate5	1	{1}	0	{}	-32767	Preliminary
DAY	day		Raw Tape Date Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
DELTAT_LPC	msec		CDP Time between last sample and this sample	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DELTAT_LPT	msec		SPP-100 Time between last sample and this sample	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DELTAT_RPT	msec		SPP-200 Time between last sample and this sample	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DRFTA	degree		IRS Drift Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
DRFTA_IRS2	degree		IRS Drift Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
DT1DC_LPB	msec		Fast 2DC Probe Dead Time	SampleRate1	1	{1}	0	{}	-32767	Preliminary
DUMMY_AFTL1	V	V	Dummy to facilitate recording of A2D Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
F2DIO1_LPB	V	V	Fast 2DC End Diode Voltage #1	SampleRate10	1	{1}	0	{}	-32767	Preliminary
F2DIO32_LPB	V	V	Fast 2DC End Diode Voltage #32	SampleRate10	1	{1}	0	{}	-32767	Preliminary
F2DIO64_LPB	V	V	Fast 2DC End Diode Voltage #64	SampleRate10	1	{1}	0	{}	-32767	Preliminary
F43_AMS			AMS Fraction of OA from m/z 43	SampleRate1	1	{1}	0	{}	-32767	Preliminary
F44_AMS			AMS Fraction of OA from m/z 44	SampleRate1	1	{1}	0	{}	-32767	Preliminary
F57_AMS			AMS Fraction of OA from m/z 57	SampleRate1	1	{1}	0	{}	-32767	Preliminary
F60_AMS			AMS Fraction of OA from m/z 60	SampleRate1	1	{1}	0	{}	-32767	Preliminary
FCN	vlpm	V	Raw CN Counter Sample Flow Rate	SampleRate10	1	{1}	0	{}	-32767	Preliminary
FILEM_AMS	mA		AMS Filament Emission Current	SampleRate1	1	{1}	0	{}	-32767	Preliminary
FLOW_AMS	sccs		AMS Sampling Flow Rate	SampleRate1	1	{1}	0	{}	-32767	Preliminary
FO3_ACD	ppbv		Fast Response Ozone Mixing Ratio	SampleRate1	1	{1}	0	{}	-32767	Preliminary
FREF_LPT	V		SPP-100 Laser Reference Voltage	SampleRate10	1	{1}	0	{}	-32767	Preliminary
FRNG_LPT			SPP-100 Size Range Category	SampleRate10	1	{1}	0	{}	-32767	Preliminary
FTMP_LPT	deg_C		SPP-100 Housekeeping Data	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GASFLOW_ISAF	V		Bypass Gas Flow	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GCNTR1_ISAF	counts		Gated Detector Photon Counter	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GCNTR3_ISAF	counts		Gated Detector Counter (Fine)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GGALT	m		Reference GPS Altitude (MSL)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGALTSD	m		Standard Deviation of Reference GPS Altitude (MSL)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGDAGE	s		Time in seconds since last DGPS update	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGEOIDHT	m		Reference GPS Height of geoid (MSL) above WGS84 ellipsoid	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGHORDIL	none		Reference GPS Horizontal dilution of position	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGLAT	degree_N		Reference GPS Latitude	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGLATSD	m		Standard Deviation of Reference GPS Latitude	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGLON	degree_E		Reference GPS Longitude	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGLONSD	m		Standard Deviation of Reference GPS Longitude	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGNSAT	number		Reference GPS number of satellites used in solution	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGQUAL	none		Reference GPS Qual, 0=inval,1=GPS,2=DGPS,5=OMNISTAR	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGREFID	none		DGPS station ID number	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGRepLag	s		GPS reporting lag: (data sys time tag of NMEA receipt)-(NMEA time)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGSPD	m/s		Reference GPS Ground Speed	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGSTATUS	none		Reference GPS rcvr status: 1=OK(A), 0=warning(V)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGTRK	degree_T		Reference GPS Track Angle	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGVEW	m/s		Reference GPS Ground Speed Vector, East Component	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGVNS	m/s		Reference GPS Ground Speed Vector, North Component	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GGVSPD	m/s		Reference GPS Vertical Speed	SampleRate10	1	{1}	0	{}	-32767	Preliminary
GSPD	m/s		IRS Aircraft Ground Speed	SampleRate25	1	{1}	0	{}	-32767	Preliminary
GSPDHYB	knots		Hybrid Ground Speed - 175	SampleRate25	1	{1}	0	{}	-32767	Preliminary
GSPDHYB_IRS2	knots		Hybrid Ground Speed - 175	SampleRate25	1	{1}	0	{}	-32767	Preliminary
GSPD_A	m/s		Avionics GPS Ground Speed	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GSPD_A2	m/s		Avionics GPS Ground Speed	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GSPD_IRS2	m/s		IRS Aircraft Ground Speed	SampleRate25	1	{1}	0	{}	-32767	Preliminary
GSTAT_A	none		Avionics GPS Sensor Status	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GSTAT_A2	none		Avionics GPS Sensor Status	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GTIME_A	s		Avionics GPS UTC Measure Time	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GTIME_A2	s		Avionics GPS UTC Measure Time	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GVEW_A	m/s		Avionics GPS Ground Speed Vector, East Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GVEW_A2	m/s		Avionics GPS Ground Speed Vector, East Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GVNS_A	m/s		Avionics GPS Ground Speed Vector, North Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
GVNS_A2	m/s		Avionics GPS Ground Speed Vector, North Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
H2O_PIC2311	vol%		Picarro Water Mixing Ratio	SampleRate20	1	{1}	0	{}	-32767	Preliminary
HCHO_TOGA	pptv		TOGA Formaldehyde	SampleRate1	1	{1}	0	{}	-32767	Preliminary
HCN_TOGA	pptv		TOGA HCN	SampleRate1	1	{1}	0	{}	-32767	Preliminary
HGM232	feet		Altitude above ground (radar altimeter)	SampleRate25	1	{1}	0	{}	-32767	Preliminary
HOUR	hour		Raw Tape Time Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
HYBHFOM	feet		Hybrid Horizontal Figure of Merit- 264	SampleRate13	1	{1}	0	{}	-32767	Preliminary
HYBHFOM_IRS2	feet		Hybrid Horizontal Figure of Merit- 264	SampleRate13	1	{1}	0	{}	-32767	Preliminary
HYBVFOM	feet		Hybrid Vertical Figure of Merit - 135	SampleRate2	1	{1}	0	{}	-32767	Preliminary
HYBVFOM_IRS2	feet		Hybrid Vertical Figure of Merit - 135	SampleRate2	1	{1}	0	{}	-32767	Preliminary
INLETP_AL		V	Aerolaser Inlet Pressure Measurement	SampleRate10	1	{1}	0	{}	-32767	Preliminary
INLETP_AMS	Torr	V	AMS Inlet Pressure 	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_311	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_318	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_319	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_320	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_321	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_322	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_323	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_324	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Status_325	bits		IRIG status: bit 0=SYNC,1=NOCODE,2=NOPPS,3=NOMAJT,4=NOYEAR,5=NOSYNC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_311	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_318	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_319	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_320	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_321	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_322	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_323	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_324	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IRIG_Tdiff_325	sec		IRIG-UNIX clock diff	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ISOPRENE_TOGA	pptv		TOGA Isoprene	SampleRate1	1	{1}	0	{}	-32767	Preliminary
IWD	degree_T		IRS Wind Direction	SampleRate13	1	{1}	0	{}	-32767	Preliminary
IWD_IRS2	degree_T		IRS Wind Direction	SampleRate13	1	{1}	0	{}	-32767	Preliminary
IWS	m/s		IRS Wind Speed	SampleRate13	1	{1}	0	{}	-32767	Preliminary
IWS_IRS2	m/s		IRS Wind Speed	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LAT	degree_N		IRS Latitude	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LATF_A	degree_N		Avionics GPS Latitude Fine	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LATF_A2	degree_N		Avionics GPS Latitude Fine	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LATHYB	deg_N		Hybrid Lat - 254	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LATHYB_IRS2	deg_N		Hybrid Lat - 254	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LAT_A	degree_N		Avionics GPS Latitude	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LAT_A2	degree_N		Avionics GPS Latitude	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LAT_IRS2	degree_N		IRS Latitude	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LENSP_AMS	Torr		AMS Lens Pressure	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LON	degree_E		IRS Longitude	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LONF_A	degree_E		Avionics GPS Longitude Fine	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LONF_A2	degree_E		Avionics GPS Longitude Fine	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LONHYB	deg_E		Hybrid Lon - 255	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LONHYB_IRS2	deg_E		Hybrid Lon - 255	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LON_A	degree_E		Avionics GPS Longitude	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LON_A2	degree_E		Avionics GPS Longitude	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LON_IRS2	degree_E		IRS Longitude	SampleRate13	1	{1}	0	{}	-32767	Preliminary
LPWR1_ISAF	V		Detector Laser Power	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LPWR2_ISAF	V		Reference Laser Power	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LSETV_ISAF	V		Modulation Laser Voltage	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LSRINT_VXL	count		VCSEL Averaged Laser Intensity	SampleRate1	1	{1}	0	{}	-32767	Preliminary
LSRTMP_VXL	ohm		VCSEL Laser Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MACH_A	none		ADC Mach Number	SampleRate8	1	{1}	0	{}	-32767	Preliminary
MACH_A2	none		ADC Mach Number	SampleRate8	1	{1}	0	{}	-32767	Preliminary
MEK_TOGA	pptv		TOGA MEK	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MHDG	deg		IRS Aircraft Magnetic Heading Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
MHDG_IRS2	deg		IRS Aircraft Magnetic Heading Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
MINUTE	minute		Raw Tape Time Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MIRRTMP_DPB	deg_C		Raw Dew/Frost Point Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MIRRTMP_DPT	deg_C		Raw Dew/Frost Point Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MODE_VXL	none		VCSEL Mode	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MONPWRIN_ISAF	V		Laser Monitor Power Input	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MONPWROUT_ISAF	V		Laser Monitor Power Output	SampleRate1	1	{1}	0	{}	-32767	Preliminary
MONTH	month		Raw Tape Date Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
N2O5_ARN	pptv		Dinitrogen Pentoxide	SampleRate2	1	{1}	0	{}	-32767	Preliminary
NCNTR2_ISAF	counts		Detector Photon Counter, Normalized	SampleRate1	1	{1}	0	{}	-32767	Preliminary
NO2_ARN	ppbv		Nitrogen Dioxide	SampleRate2	1	{1}	0	{}	-32767	Preliminary
NO3_AMS	ug/m3		AMS Nitrate PM1 mass as measured by HRAMS	SampleRate1	1	{1}	0	{}	-32767	Preliminary
NO3_ARN	pptv		Nitrate Radical	SampleRate2	1	{1}	0	{}	-32767	Preliminary
NOCAL	bool		Do Not Calibrate	SampleRate1	1	{1}	0	{}	-32767	Preliminary
NOREC	bool		Do Not Record	SampleRate1	1	{1}	0	{}	-32767	Preliminary
NOX_ARN	ppbv		NOx	SampleRate2	1	{1}	0	{}	-32767	Preliminary
NOY_ACD	pptv		NOY Mixing Ratio	SampleRate1	1	{1}	0	{}	-32767	Preliminary
NOY_ARN	ppbv		Total Oxidized Nitrogen	SampleRate2	1	{1}	0	{}	-32767	Preliminary
NO_ACD	pptv		NO Mixing Ratio	SampleRate1	1	{1}	0	{}	-32767	Preliminary
O3_AMS	ppmv	V	AMS OFR Ozone Monitor	SampleRate1	1	{1}	0	{}	-32767	Preliminary
OA_AMS	ug/m3		AMS Organic Aerosol mass as measured by HRAMS	SampleRate1	1	{1}	0	{}	-32767	Preliminary
OFRUV_AMS	V		AMS OFR UV Light Meter	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ORG43_AMS	Hz		AMS Organics; Intensity of m/z 43	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ORG44_AMS	Hz		AMS Organics; Intensity of m/z 44	SampleRate1	1	{1}	0	{}	-32767	Preliminary
ORG57_AMS	Hz		AMS Organics; Intensity of m/z 57	SampleRate1	1	{1}	0	{}	-32767	Preliminary
OVFLW_LPC	count		CDP AtoD Converter Overflow/Overrange	SampleRate10	1	{1}	0	{}	-32767	Preliminary
OVFLW_LPT	count		SPP-100 AtoD Converter Overflow/Overrange	SampleRate10	1	{1}	0	{}	-32767	Preliminary
OX_ARN	ppbv		Odd Oxygen	SampleRate2	1	{1}	0	{}	-32767	Preliminary
PALT_A	m		ADC Pressure Altitude	SampleRate16	1	{1}	0	{}	-32767	Preliminary
PALT_A2	m		ADC Pressure Altitude	SampleRate16	1	{1}	0	{}	-32767	Preliminary
PCAB	hPa	V	Interior Cabin Static Pressure	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PCIF_AMS	sccm	V	AMS PCI Flow Meter	SampleRate1	1	{1}	0	{}	-32767	Preliminary
PCN	hPa	V	CN Counter Inlet Pressure	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PFLWS_RPT	std cm3/s	cnts	SPP-200 Sheath Flow	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PFLW_RPT	std cm3/s	cnts	SPP-200 Flow	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PHGB_RPT	V		SPP-200 High-Gain Baseline	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PITCH	degree	degree	IRS Aircraft Pitch Angle	SampleRate50	1	{1}	0	{}	-32767	Preliminary
PITCH_IRS2	degree	degree	IRS Aircraft Pitch Angle	SampleRate50	1	{1}	0	{}	-32767	Preliminary
PKPOS_VXL	count		VCSEL Peak Position	SampleRate1	1	{1}	0	{}	-32767	Preliminary
PLGB_RPT	V		SPP-200 Low-Gain Baseline	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PLWC	W	V	Raw PMS-King Liquid Water Content Output	SampleRate100	1	{1}	0	{}	-32767	Preliminary
PMGB_RPT	V		SPP-200 Mid-Gain Baseline	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PREF_RPT	V		SPP-200 Laser Reference Voltage	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PREGAIN_VXL	count		VCSEL Pre Gain	SampleRate1	1	{1}	0	{}	-32767	Preliminary
PSFD	hPa		Raw Static Pressure, Fuselage	SampleRate50	1	{1}	0	{}	-32767	Preliminary
PSFRD	hPa		Raw Static Pressure, Fuselage	SampleRate50	1	{1}	0	{}	-32767	Preliminary
PS_A	hPa		ADC Static Pressure	SampleRate8	1	{1}	0	{}	-32767	Preliminary
PS_A2	hPa		ADC Static Pressure	SampleRate8	1	{1}	0	{}	-32767	Preliminary
PS_VXL	hPa	torr	VCSEL Ambient Pressure	SampleRate1	1	{1}	0	{}	-32767	Preliminary
PTAB_AMS			AMS Particle Time of Flight	SampleRate1	1	{1}	0	{}	-32767	Preliminary
PTMP_RPT	deg_C		SPP-200 Detector Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PT_A	hPa		ADC Total Pressure	SampleRate16	1	{1}	0	{}	-32767	Preliminary
PT_A2	hPa		ADC Total Pressure	SampleRate16	1	{1}	0	{}	-32767	Preliminary
PUMPIN3_ISAF	Amperes		Laser 3rd Pump Diode Input Current	SampleRate1	1	{1}	0	{}	-32767	Preliminary
QCF	hPa	V	Raw Dynamic Pressure, Fuselage	SampleRate100	1	{1}	0	{}	-32767	Preliminary
QCFR	hPa	V	Raw Dynamic Pressure, Fuselage Right	SampleRate100	1	{1}	0	{}	-32767	Preliminary
QCR	hPa	V	Raw Dynamic Pressure, Radome	SampleRate100	1	{1}	0	{}	-32767	Preliminary
QC_A	hPa		ADC Impact Pressure	SampleRate8	1	{1}	0	{}	-32767	Preliminary
QC_A2	hPa		ADC Impact Pressure	SampleRate8	1	{1}	0	{}	-32767	Preliminary
RAWCONC_VXL	#/cm3		VCSEL Moisture Number Density	SampleRate25	1	{1}	0	{}	-32767	Preliminary
REJAT_LPT	count		SPP-100 Rejected Particle Count (Transit-Time)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
REJDOF_LPC	count		CDP Rejected Particle Count (Outside Depth-of-Field)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
REJDOF_LPT	count		SPP-100 Rejected Particle Count (Outside Depth-of-Field)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
RICE	V	V	Raw Icing-Rate Indicator	SampleRate100	1	{1}	0	{}	-32767	Preliminary
ROLL	degree		IRS Aircraft Roll Angle	SampleRate50	1	{1}	0	{}	-32767	Preliminary
ROLL_IRS2	degree		IRS Aircraft Roll Angle	SampleRate50	1	{1}	0	{}	-32767	Preliminary
RSTB	deg_C	V	Radiometric Surface Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
RSTB1	deg_C	V	Radiometric Surface Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
RSTT	deg_C	V	Radiometric Sky/Cloud-Base Temperature	SampleRate100	1	{1}	0	{}	-32767	Preliminary
RTHL1	deg_C	V	Recovery Temperature, Deiced Left, HARCO	SampleRate100	1	{1}	0	{}	-32767	Preliminary
RTHL2	deg_C	V	Recovery Temperature, Deiced Left, HARCO	SampleRate100	1	{1}	0	{}	-32767	Preliminary
RT_A	deg_C		ADC Recovery Air Temperature	SampleRate2	1	{1}	0	{}	-32767	Preliminary
RT_A2	deg_C		ADC Recovery Air Temperature	SampleRate2	1	{1}	0	{}	-32767	Preliminary
SAMPFLG_AMS	V		AMS Sample Flag #4 (0Vdc=OFR, 5Vdc=Ambient	SampleRate1	1	{1}	0	{}	-32767	Preliminary
SAMPLEF_ISAF	V		Sample Flow Rate	SampleRate1	1	{1}	0	{}	-32767	Preliminary
SAMPLEP_ISAF	V		Sample Pressure	SampleRate1	1	{1}	0	{}	-32767	Preliminary
SECOND	s		Raw Tape Time Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
SELHYB	none		Hybrid Maintenance Word - 353	SampleRate2	1	{1}	0	{}	-32767	Preliminary
SELHYB_IRS2	none		Hybrid Maintenance Word - 353	SampleRate2	1	{1}	0	{}	-32767	Preliminary
SETTMP_VXL	ohm		VCSEL Set Temperature	SampleRate1	1	{1}	0	{}	-32767	Preliminary
SHDORC_LPB	count		Fast 2DC Shadow OR Count	SampleRate10	1	{1}	0	{}	-32767	Preliminary
SMPLSRINT_VXL	mV		VCSEL Sample Laser Intensity	SampleRate25	1	{1}	0	{}	-32767	Preliminary
SO2_NC	ppbv		SO2 Ratio	SampleRate1	1	{1}	0	{}	-32767	Preliminary
SO4_AMS	ug/m3		AMS Sulfate PM1 mass as measured by HRAMS	SampleRate1	1	{1}	0	{}	-32767	Preliminary
STATUS_ISAF			Instrument Status (1-Ready, 2-Operating, 4-Calibrating)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
STATUS_VXL	none		VCSEL Status Code	SampleRate1	1	{1}	0	{}	-32767	Preliminary
TAS_A	m/s		ADC True Air Speed	SampleRate16	1	{1}	0	{}	-32767	Preliminary
TAS_A2	m/s		ADC True Air Speed	SampleRate16	1	{1}	0	{}	-32767	Preliminary
TCC_VXL	count		VCSEL Thermoelectric Cooler Current	SampleRate1	1	{1}	0	{}	-32767	Preliminary
TEC_DPB	unk		Dew/Frost Point TEC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
TEC_DPT	unk		Dew/Frost Point TEC	SampleRate1	1	{1}	0	{}	-32767	Preliminary
THDG	degree_T	degree_T	IRS Aircraft True Heading Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
THDGHYB	deg_T		Hybrid True Heading - 132	SampleRate25	1	{1}	0	{}	-32767	Preliminary
THDGHYB_IRS2	deg_T		Hybrid True Heading - 132	SampleRate25	1	{1}	0	{}	-32767	Preliminary
THDG_IRS2	degree_T	degree_T	IRS Aircraft True Heading Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
THIML1	deg_C	V	HIMIL Strut Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
THIML2	deg_C	V	HIMIL Cigar Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
TIME_PIC2311	s		Picarro Time	SampleRate20	1	{1}	0	{}	-32767	Preliminary
TKAT	degree_T		IRS Aircraft Track Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
TKAT_A	degree_T		Avionics GPS Track Angle	SampleRate1	1	{1}	0	{}	-32767	Preliminary
TKAT_A2	degree_T		Avionics GPS Track Angle	SampleRate1	1	{1}	0	{}	-32767	Preliminary
TKAT_IRS2	degree_T		IRS Aircraft Track Angle	SampleRate25	1	{1}	0	{}	-32767	Preliminary
TOLUENE_TOGA	pptv		TOGA Toluene	SampleRate1	1	{1}	0	{}	-32767	Preliminary
TRSTB	deg_C	V	RSTB Sensor Head Temperature	SampleRate10	1	{1}	0	{}	-32767	Preliminary
TVAP_AMS	deg_C	V	AMS Vaporizer Temperature 	SampleRate1	1	{1}	0	{}	-32767	Preliminary
UBTMP_RPC	K	V	UHSAS Temperature in Canister	SampleRate10	1	{1}	0	{}	-32767	Preliminary
UCURR_RPC	V		UHSAS Laser Current	SampleRate10	1	{1}	0	{}	-32767	Preliminary
UDIFF_RPC	V		UHSAS Absolute Pressure in Canister, signal	SampleRate10	1	{1}	0	{}	-32767	Preliminary
UPRESS_RPC	kPa	V	UHSAS Absolute Pressure in Canister	SampleRate10	1	{1}	0	{}	-32767	Preliminary
UREF_RPC	V		UHSAS Reference Intensity	SampleRate10	1	{1}	0	{}	-32767	Preliminary
USCAT_RPC	V		UHSAS Background Scatter	SampleRate10	1	{1}	0	{}	-32767	Preliminary
USHFLW_RPC	vccs	V	UHSAS Air Sheath Flow	SampleRate10	1	{1}	0	{}	-32767	Preliminary
USMPFLW_RPC	vccs	V	UHSAS Sample Flow	SampleRate10	1	{1}	0	{}	-32767	Preliminary
UTEMP_RPC	V		UHSAS Temperature in Canister, signal	SampleRate10	1	{1}	0	{}	-32767	Preliminary
UTSEC_AMS	sec		AMS Time (Seconds since midnight)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
UVN315_HARP	counts		UVN 315 nm Counts	SampleRate1	1	{1}	0	{}	-32767	Preliminary
UVN382_HARP	counts		UVN 382 nm Counts	SampleRate1	1	{1}	0	{}	-32767	Preliminary
UVNRH_HARP	%		UVN Relative Humidity	SampleRate1	1	{1}	0	{}	-32767	Preliminary
UVZ315_HARP	counts		UVZ 315 nm Counts	SampleRate1	1	{1}	0	{}	-32767	Preliminary
UVZ382_HARP	counts		UVZ 382 nm Counts	SampleRate1	1	{1}	0	{}	-32767	Preliminary
UVZRH_HARP	%		UVZ Relative Humidity	SampleRate1	1	{1}	0	{}	-32767	Preliminary
VALID_AMS	V		AMS Blankflag (0=ambient, 5=filter)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
VEW	m/s		IRS Ground Speed Vector, East Component	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VEWHYB	knots		Hybrid Velocity EW - 267	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VEWHYB_IRS2	knots		Hybrid Velocity EW - 267	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VEW_IRS2	m/s		IRS Ground Speed Vector, East Component	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VNS	m/s		IRS Ground Speed Vector, North Component	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VNSHYB	knots		Hybrid Velocity NS - 266	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VNSHYB_IRS2	knots		Hybrid Velocity NS - 266	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VNS_IRS2	m/s		IRS Ground Speed Vector, North Component	SampleRate13	1	{1}	0	{}	-32767	Preliminary
VSPD	m/s		IRS Vertical Speed	SampleRate50	1	{1}	0	{}	-32767	Preliminary
VSPD_A	m/s		Avionics GPS Vertical Velocity	SampleRate1	1	{1}	0	{}	-32767	Preliminary
VSPD_A2	m/s		Avionics GPS Vertical Velocity	SampleRate1	1	{1}	0	{}	-32767	Preliminary
VSPD_IRS2	m/s		IRS Vertical Speed	SampleRate50	1	{1}	0	{}	-32767	Preliminary
WOW_A			ADC Weight On Wheels	SampleRate2	1	{1}	0	{}	-32767	Preliminary
WOW_A2			ADC Weight On Wheels	SampleRate2	1	{1}	0	{}	-32767	Preliminary
XICN	vlpm	V	CN Isokinetic Side Flow Rate	SampleRate100	1	{1}	0	{}	-32767	Preliminary
YEAR	year		Raw Tape Date Component	SampleRate1	1	{1}	0	{}	-32767	Preliminary
AKRD	degree		Attack Angle, Radome Diff. Pressure		1	{1}	0	{}	-32767	Preliminary
AQRATIO			Al's Fudge Factor		1	{1}	0	{}	-32767	Preliminary
ATHL1	deg_C		Ambient Temperature, Deiced Left		1	{1}	0	{}	-32767	Preliminary
ATHL2	deg_C		Ambient Temperature, Deiced Left		1	{1}	0	{}	-32767	Preliminary
ATTACK	degree		Attack Angle, Reference		1	{1}	0	{}	-32767	Preliminary
ATX	deg_C		Ambient Temperature, Reference		1	{1}	0	{}	-32767	Preliminary
C1DC_LPB	#/L		2D-C Concentration, 260X Emulation (per cell)	SampleRate1	2	{1,63}	0	{}	-32767	Preliminary
CCDP_LPC	#/cm3		CDP Concentration (per cell)	SampleRate10	2	{1,30}	0	{}	-32767	Preliminary
CONC1DC100_LPB	#/L		2D-C Concentration 100u and larger, Center-in (all cells)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CONC1DC150_LPB	#/L		2D-C Concentration 150u and larger, Center-in (all cells)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CONC1DC_LPB	#/L		2D-C Concentration, 260X Emulation (all cells)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
CONCD_LPC	#/cm3		CDP Concentration (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CONCF_LPT	#/cm3		FSSP-100 Concentration (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CONCN	#/cm3		Condensation Nuclei (CN) Concentration		1	{1}	0	{}	-32767	Preliminary
CONCP_RPT	#/cm3		PCAS Concentration (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CONCU100_RPC	#/cm3		UHSAS Concentration, .1 micron and bigger	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CONCU500_RPC	#/cm3		UHSAS Concentration, .5 micron and bigger	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CONCU_RPC	#/cm3		UHSAS Concentration (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
CONCV_VXL	#/cm3		VCSEL Moisture Number Density		1	{1}	0	{}	-32767	Preliminary
CS100_LPT	#/cm3		FSSP-100 Concentration (per cell) - DMT	SampleRate10	2	{1,30}	0	{}	-32767	Preliminary
CS200_RPT	#/cm3		SPP-200 (PCASP) Concentration (per cell) - DMT	SampleRate10	2	{1,30}	0	{}	-32767	Preliminary
CUHSAS_RPC	#/cm3		UHSAS Concentration (per cell)	SampleRate10	2	{1,99}	0	{}	-32767	Preliminary
DBAR1DC_LPB	um		2D-C Mean Particle Diameter, 260X Emulation	SampleRate1	1	{1}	0	{}	-32767	Preliminary
DBARD_LPC	um		CDP Mean Particle Diameter	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DBARF_LPT	um		FSSP-100 Mean Particle Diameter	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DBARP_RPT	um		PCAS Mean Particle Diameter	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DBARU_RPC	um		UHSAS Mean Particle Diameter	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DBZ1DC_LPB	dBZ		2D-C Calculated Reflectivity, 260X Emulation	SampleRate1	1	{1}	0	{}	-32767	Preliminary
DBZD_LPC	dBZ		CDP Calculated Reflectivity	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DBZF_LPT	dBZ		FSSP-100 Calculated Reflectivity	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DISP1DC_LPB	none		2D-C Dispersion, 260X Emulation (sigma/dbarx)	SampleRate1	1	{1}	0	{}	-32767	Preliminary
DISPD_LPC	none		CDP Dispersion (sigma/dbarx)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DISPF_LPT	none		FSSP-100 Dispersion (sigma/dbarx)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DISPP_RPT	none		PCAS Dispersion (sigma/dbarx)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DISPU_RPC	none		UHSAS Dispersion (sigma/dbarx)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
DPXC	deg_C		Dew/Frost Point Temperature, Reference		1	{1}	0	{}	-32767	Preliminary
DP_DPB	deg_C		Dew/Frost Point Temperature		1	{1}	0	{}	-32767	Preliminary
DP_DPT	deg_C		Dew/Frost Point Temperature		1	{1}	0	{}	-32767	Preliminary
DP_VXL	deg_C		Dew/Frost Point Temperature		1	{1}	0	{}	-32767	Preliminary
DVALUE	m		D-VALUE (GGALT - PALT)		1	{1}	0	{}	-32767	Preliminary
EWX	hPa		Ambient Water Vapor Pressure, Reference		1	{1}	0	{}	-32767	Preliminary
EW_DPB	hPa		Ambient Water Vapor Pressure		1	{1}	0	{}	-32767	Preliminary
EW_DPT	hPa		Ambient Water Vapor Pressure		1	{1}	0	{}	-32767	Preliminary
EW_VXL	hPa		Ambient Water Vapor Pressure (VCSEL)		1	{1}	0	{}	-32767	Preliminary
FCNC	vlpm		Corrected BCN Counter Sample Flow Rate		1	{1}	0	{}	-32767	Preliminary
GGALTC	m		Corrected GPS Altitude		1	{1}	0	{}	-32767	Preliminary
JNO2_HARP	1/sec		HARP JNO2 Proxy		1	{1}	0	{}	-32767	Preliminary
LATC	degree_N		GPS-Corrected Inertial Latitude		1	{1}	0	{}	-32767	Preliminary
LONC	degree_E		GPS-Corrected Inertial Longitude		1	{1}	0	{}	-32767	Preliminary
MACHF	none		Aircraft Mach Number, Fuselage		1	{1}	0	{}	-32767	Preliminary
MACHFR	none		Aircraft Mach Number, Fuselage		1	{1}	0	{}	-32767	Preliminary
MACHR	none		Aircraft Mach Number, Radome		1	{1}	0	{}	-32767	Preliminary
MACHX	none		Aircraft Mach Number, Reference		1	{1}	0	{}	-32767	Preliminary
MR	gram/kg		Mixing Ratio, T-Electric		1	{1}	0	{}	-32767	Preliminary
ONE	none		Constant value of 1.		1	{1}	0	{}	-32767	Preliminary
PALT	m		NACA Pressure Altitude		1	{1}	0	{}	-32767	Preliminary
PALTF	feet		NACA Pressure Altitude		1	{1}	0	{}	-32767	Preliminary
PDRY	W		Power Dry Calculated		1	{1}	0	{}	-32767	Preliminary
PFLWC_RPT	vol cm3/s		PCAS Corrected Flow	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PLWC1DC_LPB	gram/m3		2D-C Water/Ice Content, 260X Emulation	SampleRate1	1	{1}	0	{}	-32767	Preliminary
PLWCC	gram/m3		Corrected PMS-King Liquid Water Content		1	{1}	0	{}	-32767	Preliminary
PLWCD_LPC	gram/m3		CDP Water/Ice Content	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PLWCF_LPT	gram/m3		FSSP-100 Water/Ice Content	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PSFC	hPa		Corrected Static Pressure, Fuselage		1	{1}	0	{}	-32767	Preliminary
PSFDC	hPa		Corrected Static Pressure, Fuselage Digital		1	{1}	0	{}	-32767	Preliminary
PSURF	hPa		Calculated Surface Pressure		1	{1}	0	{}	-32767	Preliminary
PSX	hPa		Raw Static Pressure, Reference		1	{1}	0	{}	-32767	Preliminary
PSXC	hPa		Corrected Static Pressure, Reference		1	{1}	0	{}	-32767	Preliminary
PVOLP_RPT	um3/m3		PCASP Equivalent Volume	SampleRate10	1	{1}	0	{}	-32767	Preliminary
PVOLU_RPC	um3/m3		UHSAS Equivalent Volume	SampleRate10	1	{1}	0	{}	-32767	Preliminary
QCFC	hPa		Corrected Dynamic Pressure, Fuselage		1	{1}	0	{}	-32767	Preliminary
QCFRC	hPa		Raw Dynamic Pressure, Fuselage Right, Corrected		1	{1}	0	{}	-32767	Preliminary
QCRC	hPa		Corrected Dynamic Pressure, Radome		1	{1}	0	{}	-32767	Preliminary
QCX	hPa		Raw Dynamic Pressure, Reference		1	{1}	0	{}	-32767	Preliminary
QCXC	hPa		Corrected Dynamic Pressure, Reference		1	{1}	0	{}	-32767	Preliminary
RALT	m		Geometric (Radar) Altitude (APN-232)		1	{1}	0	{}	-32767	Preliminary
REFF2DC_LPB	um		1DC Effective Radius	SampleRate1	1	{1}	0	{}	-32767	Preliminary
REFFD_LPC	um		CDP Effective Radius	SampleRate10	1	{1}	0	{}	-32767	Preliminary
REFFF_LPT	um		FSSP-100 Effective Radius	SampleRate10	1	{1}	0	{}	-32767	Preliminary
RHODT	gram/m3		Absolute Humidity, T-Electric		1	{1}	0	{}	-32767	Preliminary
RHUM	%		Relative Humidity		1	{1}	0	{}	-32767	Preliminary
RTX	deg_C		Recovery Temperature, Reference		1	{1}	0	{}	-32767	Preliminary
SOLAZ	radian		Solar Azimuth Angle		1	{1}	0	{}	-32767	Preliminary
SOLDE	radian		Solar Declination Angle		1	{1}	0	{}	-32767	Preliminary
SOLEL	radian		Solar Elevation Angle		1	{1}	0	{}	-32767	Preliminary
SOLZE	radian		Solar Zenith Angle		1	{1}	0	{}	-32767	Preliminary
SSLIP	degree		Sideslip Angle, Reference		1	{1}	0	{}	-32767	Preliminary
SSRD	degree		Sideslip Angle, Radome Diff. Pressure		1	{1}	0	{}	-32767	Preliminary
TASF	m/s		Aircraft True Airspeed, Fuselage		1	{1}	0	{}	-32767	Preliminary
TASFR	m/s		Aircraft True Airspeed, Fuselage		1	{1}	0	{}	-32767	Preliminary
TASHC	m/s		Aircraft True Airspeed, Humidity Corrected		1	{1}	0	{}	-32767	Preliminary
TASR	m/s		Aircraft True Airspeed, Radome		1	{1}	0	{}	-32767	Preliminary
TASX	m/s		Aircraft True Airspeed, Reference		1	{1}	0	{}	-32767	Preliminary
TCNTD_LPC	count		CDP Total Counts (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
TCNTF_LPT	count		FSSP-100 Total Counts (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
TCNTP_RPT	count		PCAS Total Counts (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
TCNTU_RPC	count		UHSAS Total Counts (all cells)	SampleRate10	1	{1}	0	{}	-32767	Preliminary
THETA	K		Potential Temperature		1	{1}	0	{}	-32767	Preliminary
THETAE	K		Equivalent Potential Temperature		1	{1}	0	{}	-32767	Preliminary
THETAP	K		Pseudo-adiabatic Equivalent Potential Temperature		1	{1}	0	{}	-32767	Preliminary
THETAQ	K		Wet Equivalent Potential Temperature		1	{1}	0	{}	-32767	Preliminary
THETAV	K		Virtual Potential Temperature		1	{1}	0	{}	-32767	Preliminary
TVIR	deg_C		Virtual Temperature		1	{1}	0	{}	-32767	Preliminary
UFLWC_RPC	vol cm3/s		UHSAS Corrected Flow	SampleRate10	1	{1}	0	{}	-32767	Preliminary
UI	m/s		Wind Vector, East Component		1	{1}	0	{}	-32767	Preliminary
UIC	m/s		GPS-Corrected Wind Vector, East Component		1	{1}	0	{}	-32767	Preliminary
UX	m/s		Wind Vector, Longitudinal Component		1	{1}	0	{}	-32767	Preliminary
UXC	m/s		GPS-Corrected Wind Vector, Longitudinal Component		1	{1}	0	{}	-32767	Preliminary
VEWC	m/s		GPS-Corrected Inertial Ground Speed Vector, East Component		1	{1}	0	{}	-32767	Preliminary
VI	m/s		Wind Vector, North Component		1	{1}	0	{}	-32767	Preliminary
VIC	m/s		GPS-Corrected Wind Vector, North Component		1	{1}	0	{}	-32767	Preliminary
VMR_VXL	ppmv		Volume Mixing Ratio		1	{1}	0	{}	-32767	Preliminary
VNSC	m/s		GPS-Corrected Inertial Ground Speed Vector, North Component		1	{1}	0	{}	-32767	Preliminary
VY	m/s		Wind Vector, Lateral Component		1	{1}	0	{}	-32767	Preliminary
VYC	m/s		GPS-Corrected Wind Vector, Lateral Component		1	{1}	0	{}	-32767	Preliminary
WD	degree_T		Horizontal Wind Direction		1	{1}	0	{}	-32767	Preliminary
WDC	degree_T		GPS-Corrected Horizontal Wind Direction		1	{1}	0	{}	-32767	Preliminary
WI	m/s		Wind Vector, Vertical Gust Component		1	{1}	0	{}	-32767	Preliminary
WIC	m/s		GPS-Corrected Wind Vector, Vertical Gust Component		1	{1}	0	{}	-32767	Preliminary
WS	m/s		Horizontal Wind Speed		1	{1}	0	{}	-32767	Preliminary
WSC	m/s		GPS-Corrected Horizontal Wind Speed		1	{1}	0	{}	-32767	Preliminary
XICNC	vlpm		Corrected CN Isokinetic Side Flow Rate		1	{1}	0	{}	-32767	Preliminary
ZERO	none		Constant value of 0.		1	{1}	0	{}	-32767	Preliminary
\.


--
-- TOC entry 1868 (class 2606 OID 42603270)
-- Dependencies: 149 149
-- Name: a1dc_lp_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY a1dc_lp
    ADD CONSTRAINT a1dc_lp_pkey PRIMARY KEY (datetime);


--
-- TOC entry 1864 (class 2606 OID 42603230)
-- Dependencies: 143 143
-- Name: global_attributes_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY global_attributes
    ADD CONSTRAINT global_attributes_pkey PRIMARY KEY (key);


--
-- TOC entry 1862 (class 2606 OID 5266579)
-- Dependencies: 141 141
-- Name: mission_control_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mission_control
    ADD CONSTRAINT mission_control_pkey PRIMARY KEY (key);


--
-- TOC entry 1866 (class 2606 OID 42603238)
-- Dependencies: 144 144
-- Name: variable_list_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY variable_list
    ADD CONSTRAINT variable_list_pkey PRIMARY KEY (name);


--
-- TOC entry 1668 (class 2618 OID 3253285)
-- Dependencies: 140 140
-- Name: update; Type: RULE; Schema: public; Owner: -
--

CREATE RULE update AS ON UPDATE TO lightning DO NOTIFY light_strike;


--
-- TOC entry 1669 (class 2618 OID 42603329)
-- Dependencies: 143 143
-- Name: update; Type: RULE; Schema: public; Owner: -
--

CREATE RULE update AS ON UPDATE TO global_attributes DO NOTIFY current;


--
-- TOC entry 1883 (class 0 OID 0)
-- Dependencies: 3
-- Name: public; Type: ACL; Schema: -; Owner: -
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2015-02-11 19:42:09 UTC

--
-- PostgreSQL database dump complete
--

