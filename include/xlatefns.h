#ifndef DERIVEDEF_H
#define DERIVEDEF_H

struct _dnfn deriveftns[] =
	{
		"2D-C",	NULL,		xlAsync,	NULL,
		"2D-P",	NULL,		xlAsync,	NULL,
		"2DEE1",	NULL,		xl2d_hskp4,	NULL,
		"2DEE32",	NULL,		xl2d_hskp5,	NULL,
		"2DMTMP",	NULL,		xl2d_hskp1,	NULL,
		"2DV15",	NULL,		xl2d_hskp0,	NULL,
		"2DV5",	NULL,		xl2d_hskp7,	NULL,
		"2DV5",	NULL,		xl2d_hskp7,	NULL,
		"2DV5",	NULL,		xl2d_hskp7,	NULL,
		"2DVN15",	NULL,		xl2d_hskp6,	NULL,
		"AACT_OLD",	NULL,		xlaact,	NULL,
		"ACINS_IRS",	NULL,		xlvacc,	NULL,
		"ACLMT",	NULL,		xlClimet,	NULL,
		"ALPHA_INS",	NULL,		xlalpha,	NULL,
		"ALT_IRS",	NULL,		xlialt,	NULL,
		"AS100",	NULL,		xlSPP100,	NULL,
		"AS200",	NULL,		xlSPP200,	NULL,
		"AS300",	NULL,		xlSPP300,	NULL,
		"BLATA_IRS",	NULL,		xlblata,	NULL,
		"BLONGA_IRS",	NULL,		xlblona,	NULL,
		"BNORMA_IRS",	NULL,		xlbnorma,	NULL,
		"BPITCHR_IRS",	NULL,		xlbpitchr,	NULL,
		"BROLLR_IRS",	NULL,		xlbrollr,	NULL,
		"BYAWR_IRS",	NULL,		xlbyawr,	NULL,
		"CALT",	NULL,		xlcalt,	NULL,
		"CHGME",	NULL,		xlchgme,	NULL,
		"CLAT",	NULL,		xlclat,	NULL,
		"CLMPOF",	NULL,		xlClimetPOF,	NULL,
		"CLON",	NULL,		xlclon,	NULL,
		"CPITCH",	NULL,		xlcpitch,	NULL,
		"CROLL",	NULL,		xlcroll,	NULL,
		"CTHDG",	NULL,		xlcthdg,	NULL,
		"CVEW",	NULL,		xlcvew,	NULL,
		"CVNS",	NULL,		xlcvns,	NULL,
		"CVSPD",	NULL,		xlcvspd,	NULL,
		"DAY",	NULL,		xlday,	NULL,
		"DMECH",	NULL,		xldmech,	NULL,
		"DMEDI",	NULL,		xldmedi,	NULL,
		"DMEFL",	NULL,		xldmefl,	NULL,
		"DRFTA_IRS",	NULL,		xlidrift,	NULL,
		"DSAT",	NULL,		xldsat,	NULL,
		"DSGALT",	NULL,		xldsgalt,	NULL,
		"DSLAT",	NULL,		xldslat,	NULL,
		"DSLON",	NULL,		xldslon,	NULL,
		"DSPS",	NULL,		xldsps,	NULL,
		"DSRH",	NULL,		xldsmr,	NULL,
		"DSTIME",	NULL,		xldstime,	NULL,
		"DSVSPD",	NULL,		xldsvspd,	NULL,
		"DSWD",	NULL,		xldswd,	NULL,
		"DSWS",	NULL,		xldsws,	NULL,
		"ETORQ",	NULL,		xletorq,	NULL,
		"EVENT",	NULL,		xlevent,	NULL,
		"FANV",	NULL,		xlpms16_hskp5,	NULL,
		"FREF",	NULL,		xlpms16_hskp7,	NULL,
		"FREF3_V3",	NULL,		xlS300cabin4,	NULL,
		"FREF_V3",	NULL,		xlS100cabin4,	NULL,
		"FRNG",	NULL,		xlfrange2,	NULL,
		"FRNG_OLD",	NULL,		xlfrange,	NULL,
		"FRNG_V3",	NULL,		xlfrange3,	NULL,
		"FRST",	NULL,		xlpms16_hskp1,	NULL,
		"FRST_OLD",	NULL,		xlfreset,	NULL,
		"FSIG",	NULL,		xlpms16_hskp6,	NULL,
		"FSTB",	NULL,		xlpms16_hskp0,	NULL,
		"FSTB_OLD",	NULL,		xlfstrob,	NULL,
		"FSTT",	NULL,		xlpms16_hskp3,	NULL,
		"FTMP",	NULL,		xlpms16_hskp4,	NULL,
		"FTMP3_V3",	NULL,		xlS300cabin7,	NULL,
		"FTMP_V3",	NULL,		xlS100cabin7,	NULL,
		"GALT",	NULL,		xlgalt,	NULL,
		"GGALT",	NULL,		xlggalt,	NULL,
		"GGLAT",	NULL,		xlgglat,	NULL,
		"GGLON",	NULL,		xlgglon,	NULL,
		"GGMODE",	NULL,		xlggstat,	NULL,
		"GGNSAT",	NULL,		xlggnsat,	NULL,
		"GGSPD",	NULL,		xlggspd,	NULL,
		"GGTRK",	NULL,		xlggtrk,	NULL,
		"GLAT",	NULL,		xlglat,	NULL,
		"GLON",	NULL,		xlglon,	NULL,
		"GMODE",	NULL,		xlgmode,	NULL,
		"GREYEE1",	NULL,		xlGrey_hskp0,	NULL,
		"GREYEE64",	NULL,		xlGrey_hskp1,	NULL,
		"GSF_IRS",	NULL,		xligspd,	NULL,
		"GSI_INS",	NULL,		xlgsi,	NULL,
		"GSTAT",	NULL,		xlgstat,	NULL,
		"GTIMP",	NULL,		xlgtimp,	NULL,
		"GTIMV",	NULL,		xlgtimv,	NULL,
		"GVEW",	NULL,		xlgvew,	NULL,
		"GVNS",	NULL,		xlgvns,	NULL,
		"GVZI",	NULL,		xlgvzi,	NULL,
		"HGM",	NULL,		xlhgm,	NULL,
		"HGM232",	NULL,		xlhgm232,	NULL,
		"HGM232S",	NULL,		xlhgm232s,	NULL,
		"HGME",	NULL,		xlhgme,	NULL,
		"HOUR",	NULL,		xlhour,	NULL,
		"HVCBTMP",	NULL,		xl2d_hskp3,	NULL,
		"HVCMTMP",	NULL,		xl2d_hskp4,	NULL,
		"HVDATMP",	NULL,		xl2d_hskp2,	NULL,
		"HVLASER",	NULL,		xl2d_hskp7,	NULL,
		"HVSATMP",	NULL,		xl2d_hskp1,	NULL,
		"HVUVV1",	NULL,		xl2d_hskp5,	NULL,
		"HVUVV2",	NULL,		xl2d_hskp6,	NULL,
		"HVWTMP",	NULL,		xl2d_hskp0,	NULL,
		"INSSEC_INS",	NULL,		xlinssec,	NULL,
		"IWD_IRS",	NULL,		xliwd,	NULL,
		"IWS_IRS",	NULL,		xliws,	NULL,
		"LAG10_IRS",	NULL,		xllag10,	NULL,
		"LAG25_IRS",	NULL,		xllag25,	NULL,
		"LAG50_IRS",	NULL,		xllag50,	NULL,
		"LAG5_IRS",	NULL,		xllag5,	NULL,
		"LAT_INS",	NULL,		xlalat,	NULL,
		"LAT_IRS",	NULL,		xlilat,	NULL,
		"LON_INS",	NULL,		xlalon,	NULL,
		"LON_IRS",	NULL,		xlilon,	NULL,
		"MINUTE",	NULL,		xlmin,	NULL,
		"MONTH",	NULL,		xlmonth,	NULL,
		"MRLHS",	NULL,		xlfloat0,	NULL,
		"MRLHW",	NULL,		xlfloat1,	NULL,
		"MRUV",	NULL,		xlmruv,	NULL,
		"OBBCOD",	NULL,		xlobbcod,	NULL,
		"ODETDC",	NULL,		xlodetdc,	NULL,
		"ODETSG",	NULL,		xlodetsg,	NULL,
		"OGAIN",	NULL,		xlogain,	NULL,
		"OSMOTR",	NULL,		xlosmotr,	NULL,
		"OTBBC",	NULL,		xlotbbc,	NULL,
		"OTBBF",	NULL,		xlotbbf,	NULL,
		"OTBNCH",	NULL,		xlotbnch,	NULL,
		"OTBOX",	NULL,		xlotbox,	NULL,
		"OTDET",	NULL,		xlotdet,	NULL,
		"OTSNT",	NULL,		xlotsnt,	NULL,
		"OVFLW_V3",	NULL,		xloFlow,	NULL,
		"P200X",	NULL,		xlpartspace,	NULL,
		"P200Y",	NULL,		xlpartspace,	NULL,
		"P260X",	NULL,		xlpartspace,	NULL,
		"P6E01",	NULL,		xlpms64_hskp5,	NULL,
		"P6E64",	NULL,		xlpms64_hskp6,	NULL,
		"P6RST",	NULL,		xlpms64_hskp0,	NULL,
		"P6TMP",	NULL,		xlpms64_hskp4,	NULL,
		"PACT",	NULL,		xlpms16_hskp0,	NULL,
		"PBRS",	NULL,		xlpms16_hskp7,	NULL,
		"PCACTV",	NULL,		xlpms16_hskp0,	NULL,
		"PCFLOW",	NULL,		xlpms16_hskp6,	NULL,
		"PF300",	NULL,		xlpartspace,	NULL,
		"PFLW",	NULL,		xlpms16_hskp6,	NULL,
		"PFLWS_V3",	NULL,		xlS200cabin6,	NULL,
		"PFLW_V3",	NULL,		xlS200cabin3,	NULL,
		"PFSSP",	NULL,		xlpartspace,	NULL,
		"PHDG",	NULL,		xlphdg51,	NULL,
		"PHGB_V3",	NULL,		xlS200cabin0,	NULL,
		"PITCH",	NULL,		xlpitch51,	NULL,
		"PITCH_IRS",	NULL,		xlipitch,	NULL,
		"PLGB_V3",	NULL,		xlS200cabin2,	NULL,
		"PMGB_V3",	NULL,		xlS200cabin1,	NULL,
		"PPCAS",	NULL,		xlpartspace,	NULL,
		"PREF",	NULL,		xlpms16_hskp5,	NULL,
		"PREF_V3",	NULL,		xlS200cabin4,	NULL,
		"PSFD",	NULL,		xlpsfd,	NULL,
		"PSFD1",	NULL,		xlpsfd_ADS2,	NULL,
		"PSFRD",	NULL,		xlxpsfd,	NULL,
		"PSLH",	NULL,		xlfloat2,	NULL,
		"PSTT",	NULL,		xlpms16_hskp1,	NULL,
		"PTMP",	NULL,		xlpms16_hskp4,	NULL,
		"PTMP_V3",	NULL,		xlS200cabin7,	NULL,
		"RDMAHVPS",	NULL,		xlrdmahvps,	NULL,
		"RDMAPABS",	NULL,		xlrdmapabs,	NULL,
		"RDMAQA",	NULL,		xlrdmaqa,	NULL,
		"RDMAQEX",	NULL,		xlrdmaqex,	NULL,
		"RDMAQS",	NULL,		xlrdmaqs,	NULL,
		"RDMAQSH",	NULL,		xlrdmaqsh,	NULL,
		"RDMARH",	NULL,		xlrdmarh,	NULL,
		"RDMATEMP",	NULL,		xlrdmatemp,	NULL,
		"RDMAVD",	NULL,		xlrdmavd,	NULL,
		"RDMAVSH",	NULL,		xlrdmavsh,	NULL,
		"REJAT_V3",	NULL,		xlrejAT,	NULL,
		"REJDOF_V3",	NULL,		xlrejDOF,	NULL,
		"ROLL",	NULL,		xlroll51,	NULL,
		"ROLL_IRS",	NULL,		xliroll,	NULL,
		"SECOND",	NULL,		xlsec,	NULL,
		"SERIAL",	NULL,		xlserial,	NULL,
		"SHDORC",	NULL,		xl2d_shadow,	NULL,
		"SHDORP",	NULL,		xl2d_shadow,	NULL,
		"TEVT3",	NULL,		xlpms32_sum,	NULL,
		"THDG_IRS",	NULL,		xlithdg,	NULL,
		"THI_INS",	NULL,		xlthi,	NULL,
		"TKAR_IRS",	NULL,		xlitrkart,	NULL,
		"TKAT_IRS",	NULL,		xlittrka,	NULL,
		"TRKI_INS",	NULL,		xltrki,	NULL,
		"TTLH",	NULL,		xlfloat3,	NULL,
		"UVCHRF",	NULL,		xluvchrf,	NULL,
		"UVCHSP",	NULL,		xluvchsp,	NULL,
		"UVHREF",	NULL,		xluvhref,	NULL,
		"UVHSMP",	NULL,		xluvhsmp,	NULL,
		"UVKRRF",	NULL,		xluvkrrf,	NULL,
		"UVKRSP",	NULL,		xluvkrsp,	NULL,
		"UVPVOL",	NULL,		xluvpvol,	NULL,
		"UVTVOL",	NULL,		xluvtvol,	NULL,
		"VEW_IRS",	NULL,		xlivew,	NULL,
		"VNS_IRS",	NULL,		xlivns,	NULL,
		"VSPD_IRS",	NULL,		xlivspd,	NULL,
		"VZI",	NULL,		xlvzi51,	NULL,
		"WVAT",	NULL,		xllh_temp,	NULL,
		"WVIX",	NULL,		xllh_indexmax,	NULL,
		"WVMR",	NULL,		xllh_vmr,	NULL,
		"WVPF",	NULL,		xllh_pp2f,	NULL,
		"WVPR",	NULL,		xllh_Pv0,	NULL,
		"WVPS",	NULL,		xllh_press,	NULL,
		"WVRH",	NULL,		xllh_relhum,	NULL,
		"WVVV",	NULL,		xllh_ppmv,	NULL,
		"WVZO",	NULL,		xllh_yzero,	NULL,
		"XN9ATX",	NULL,		xlfloat3,	NULL,
		"XN9BSCT",	NULL,		xlfloat0,	NULL,
		"XN9CAL",	NULL,		xlfloat1,	NULL,
		"XN9PSX",	NULL,		xlfloat2,	NULL,
		"XN9RELA",	NULL,		xlfloat5,	NULL,
		"XN9RHUM",	NULL,		xlfloat4,	NULL,
		"XP10",	NULL,		xlNephP10,	NULL,
		"XRH10",	NULL,		xlNephRH10,	NULL,
		"XT10",	NULL,		xlNephT10,	NULL,
		"XTSG10",	NULL,		xlNephTSG10,	NULL,
		"XVI_INS",	NULL,		xlxvi,	NULL,
		"YEAR",	NULL,		xlyear,	NULL,
		"YVI_INS",	NULL,		xlyvi,	NULL,
	NULL,		NULL,	NULL,	NULL
	};

#endif
