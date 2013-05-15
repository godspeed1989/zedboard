
`define		FREQ		72
// Horizontal	Parameter
`define		H_SYNC		120		// Sync a
`define		H_BACK		64		// Back porch b
`define		H_ACT		800		// Display interval c
`define		H_FRONT		56		// Front porch d
`define		H_BLANK		(`H_FRONT+`H_SYNC+`H_BACK)
`define		H_TOTAL		(`H_FRONT+`H_SYNC+`H_BACK+`H_ACT)
// Vertical Parameter
`define		V_SYNC		6		// o
`define		V_BACK		23		// p
`define		V_ACT		600		// q
`define		V_FRONT		37		// r
`define		V_BLANK		(`V_FRONT+`V_SYNC+`V_BACK)
`define		V_TOTAL		(`V_FRONT+`V_SYNC+`V_BACK+`V_ACT)

`define		D_WIDTH		4
`define		P_WIDTH		11
