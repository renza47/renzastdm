#define DIALOG_SIGN        				(100)

#define DIALOG_SIGN_EDIT   				(DIALOG_SIGN + 1)

#define DIALOG_SIGN_EDIT_BG   			(DIALOG_SIGN + 2)
#define DIALOG_SIGN_EDIT_BG_COL			(DIALOG_SIGN + 3)
#define DIALOG_SIGN_EDIT_BG_COL_0		(DIALOG_SIGN + 4)
#define DIALOG_SIGN_EDIT_BG_COL_1		(DIALOG_SIGN + 5)
#define DIALOG_SIGN_EDIT_BG_IMG			(DIALOG_SIGN + 6)
#define DIALOG_SIGN_EDIT_BG_OPC			(DIALOG_SIGN + 7)

#define DIALOG_SIGN_EDIT_PIC  			(DIALOG_SIGN + 8)
#define DIALOG_SIGN_EDIT_PIC_COL		(DIALOG_SIGN + 9)
#define DIALOG_SIGN_EDIT_PIC_COL_0		(DIALOG_SIGN + 10)
#define DIALOG_SIGN_EDIT_PIC_COL_1		(DIALOG_SIGN + 11)
#define DIALOG_SIGN_EDIT_PIC_IMG		(DIALOG_SIGN + 12)
#define DIALOG_SIGN_EDIT_PIC_OPC		(DIALOG_SIGN + 13)

#define DIALOG_SIGN_EDIT_MOTO  			(DIALOG_SIGN + 14)
#define DIALOG_SIGN_EDIT_MOTO_COL		(DIALOG_SIGN + 15)
#define DIALOG_SIGN_EDIT_MOTO_COL_0		(DIALOG_SIGN + 16)
#define DIALOG_SIGN_EDIT_MOTO_COL_1		(DIALOG_SIGN + 17)
#define DIALOG_SIGN_EDIT_MOTO_TEXT		(DIALOG_SIGN + 18)
#define DIALOG_SIGN_EDIT_MOTO_OPC		(DIALOG_SIGN + 19)

#define DIALOG_SIGN_RESTORE   			(DIALOG_SIGN + 20)

#define COLOR_SIGNATURE         		(COLOR_RED)
#define COLOR_SIGNATURE_RANK        	(0xFFFFFFFF)

//------------------------------------------------

enum e_RANK_DATA
{
    i_Score,
    s_RankName[35]
};

new g_ScoreBasedRanks[][e_RANK_DATA] =
{
    {0, 		"Unranked"},
    {50, 		"Bronze III"},
    {300, 		"Bronze II"},
    {500, 		"Bronze I"},
    {750, 		"Silver III"},
    {1000, 		"Silver II"},
    {1250,		"Silver I"},
    {1500, 		"Gold III"},
    {2000, 		"Gold II"},
    {3000, 		"Gold I"},
    {4000, 		"Platinum III"},
    {5000, 		"Platinum II"},
    {6000, 		"Platinum I"},
    {7000, 		"Diamond III"},
    {8000, 		"Diamond II"},
    {9000, 		"Diamond I"},
    {10000, 	"Champion III"},
    {12000, 	"Champion II"},
    {14000, 	"Champion I"},
    {15000, 	"Master"}
};