#define TEAM_INVALID 0
#define TEAM_GSF   1
#define TEAM_SBF   2
#define TEAM_TDF   3
#define TEAM_FYB   4
#define TEAM_RHB   5
#define TEAM_KTB   6
#define TEAM_TDB   7
#define TEAM_LSV   8
#define TEAM_VLA   9
#define TEAM_LSPD  10
#define TEAM_FUGITIVE 11

new gTeams[][2] = {
{1, 106}, //gsf
{2, 106}, //sbf
{3, 106}, //tdf
{4, 102}, //fyb
{5, 102}, // rhb
{6, 102}, //ktb
{7, 102}, // tdb
{8, 108}, // lsv
{9, 114}, // vla
{10, 280}
};

new Float:gTeamCameras[][7] = {
{2511.6772,-1676.0472,13.5, 2499.9949,-1677.8268,18.3,55.9},
{2721.6421,-1943.8271,13.5, 2711.1494,-1936.1576,18.2,89.1},
{1089.5786,-1082.2280,26.7, 1081.7808,-1094.2229,30.2,85.5},
{2170.5664,-1676.2477,15.0, 2183.3525,-1674.2740,19.3,222.0},
{2219.1162,-1229.0890,23.9, 2207.6582,-1220.3164,28.8,356.7},
{1938.5989,-1120.7085,26.4, 1946.9611,-1129.1921,30.6,180.4},
{1252.7200,-920.8753,42.8, 1241.3347,-930.2741,47.5,188.3}, //TDB
{2458.6401,-1319.4425,24.0, 2450.8213,-1305.9304,28.8,87.2},
{1881.7505,-2016.2035,13.5, 1889.3148,-2025.2289,18.5,181.8},
{1553.5669,-1675.6675,16.1, 1547.0587,-1672.2072,22.9,92.6}
};

// spawns
new Float:gRandomSpawns_GSF[][4] = {
{2525.0076,-1665.7772,15.0839,89.9883},
{2480.2607,-1650.6929,13.4688,180.6230},
{2531.4810,-1695.0792,13.6393,33.7144}
};

new Float:gRandomSpawns_SBF[][4] = {
{2774.8203,-2008.0477,13.5547,1.1292 },
{2757.5144,-1964.2701,13.5493,271.273},
{2783.4346,-1979.6436,14.1210,89.4654}
};

new Float:gRandomSpawns_TDF[][4] = {
{1098.8905,-1067.8199,30.5560,90.5009 },
{1074.2881,-1088.1338,26.1243,269.8419},
{1091.1707,-1083.7219,26.6688,93.0026 }
};

new Float:gRandomSpawns_FYB[][4] = {
{2169.6980,-1675.1464,15.0859,222.3532},
{2180.3926,-1660.5557,14.9479,210.9682},
{2162.7988,-1691.4094,15.0859,310.5234}
};

new Float:gRandomSpawns_RHB[][4] = {
{2228.5344,-1228.0161,23.9715,25.2563 },
{2197.8069,-1228.1475,23.9688,0.5948  },
{2211.2148,-1213.4607,23.9658,180.4379}
};

new Float:gRandomSpawns_KTB[][4] = {
{1893.1775,-1127.5739,24.3056,178.7454},
{1938.5835,-1120.2385,26.4818,136.8034},
{1956.1102,-1124.2665,26.6178,159.5165}
};

new Float:gRandomSpawns_TDB[][4] = {
{1254.5941,-919.4865,42.8265,241.8235},
{1268.4747,-907.8293,42.8828,189.0521},
{1287.2819,-914.6128,41.1657,95.3027 }
};

new Float:gRandomSpawns_LSV[][4] = {
{2442.7813,-1340.4176,24.0000,270.9258},
{2443.5129,-1292.9231,24.0000,272.6312},
{2459.5803,-1311.4449,24.0000,90.1378 }
};

new Float:gRandomSpawns_VLA[][4] = {
{1891.1305,-2031.2087,13.5469,89.9500 },
{1879.6495,-2015.5587,13.5469,182.1423},
{1872.6481,-2021.2278,13.5469,270.0328}
};

new Float:gRandomSpawns_LSPD[][4] = {
{1527.8977,-1678.2205,5.8906,270.2804},
{1529.1665,-1665.1637,6.2188,178.1253},
{1553.8583,-1687.6238,6.2188,88.9738 }
};

enum E_ZONES
{
	zOwner,
	zCapturedBy,
	zCaptureTime,
	zDeaths
};
new ZoneInfo[MAX_ZONES][E_ZONES];

// zones
new ZONE_GSF;
new ZONE_SBF;
new ZONE_TDF;
new ZONE_FYB;
new ZONE_RHB;
new ZONE_LSV;
new ZONE_KTB;
new ZONE_TDB;
new ZONE_VLA;

new Capturable[23];