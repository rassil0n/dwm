/* See LICENSE file for copyright and license details. */

//{{{ GENERAL APPEARANCE
static const unsigned int borderpx  =  1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            =  1;        /* 0 means no bar */
static const int topbar             =  1;        /* 0 means bottom bar */
static const int swallowfloating    =  0;        /* 1 means swallow floating windows by default */
static const char *fonts[]          = { "monospace:size=9" };
//}}}
//{{{ GAPS
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static                int smartgaps =  1;        /* 1 means no outer gap when there is only one window */
//}}}
//{{{ SYSTRAY
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray			 = 1;   /* 0 means no systray */
//}}}
//{{{ TABBAR
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab			= showtab_auto;        /* Default tab bar show mode */
static const int toptab				= False;               /* False means bottom tab bar */
//}}}
//{{{ DEFINE COLORS
static const char foreground[]      ="#eceff4";	// nord6
static const char myNormBorder[]    ="#3b4252";	// nord1
static const char myActive[]        ="#5E81AC";	// nord10
static const char myBlack[]         ="#2e3440";	// nord0
static const char myActiveBorder[]  ="#D8DEE9"; // nord4
static const char *colors[][3]      = {

	/*               fg          bg        border   */
	[SchemeNorm] = { foreground, myBlack,  myNormBorder   },
	[SchemeSel]  = { myActive,   myBlack,  myActiveBorder },
};
//}}}
//{{{ DEFINE SCRATCHPADS
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", "-e", "tmux", NULL };
const char *spcmd2[] = {"st", "-n", "spmusic", "-g", "144x41", "-e", "ncmpcpp", NULL };
const char *spcmd3[] = {"st", "-n", "sppy", "-g", "144x41", "-e", "python3", NULL };
const char *spcmd4[] = {"anki", NULL };
const char *spcmd5[] = {"zoom", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spmusic",    spcmd2},
	{"sppy",    spcmd3},
	{"anki",    spcmd4},
	{"zoom",    spcmd5},
};
//}}}
//{{{ DEFINE TAGS
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//}}}
//{{{ DEFINE RULES
static const Rule rules[] = {

	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */

	/* class            instance    title             tags mask     isfloating   isterminal noswallow	monitor */
	{ "Gimp",           NULL,       NULL,                 0,            1,           0,         0,       -1 },
	{ "st",             NULL,       NULL,                 0,            0,           1,         1,       -1 },
	{ NULL,             NULL,       "(nvim)",             0,            0,           0,         1,       -1 },
	{ "ksnip",          NULL,       NULL,                 0,            1,           0,         0,       -1 },
	{ NULL,             NULL,       "mpd-art-box",        0,            1,           0,         0,       -1 },
	{ NULL,             NULL,       "musicscratch",       0,            1,           0,         0,       -1 },
	{ NULL,             NULL,       "puddletag",          0,            1,           0,         0,       -1 },
	{ NULL,		        "spterm",	NULL,		          SPTAG(0),		1,			 1,			0,		 -1 },
	{ NULL,		        "spmusic",	NULL,		          SPTAG(1),		1,			 1,			0,		 -1 },
	{ NULL,		        "sppy",		NULL,		          SPTAG(2),		1,			 1,			0,		 -1 },
	{ NULL,		        "anki",		NULL,		          SPTAG(3),		1,			 0,			0,		 -1 },
	{ NULL,		        "zoom",		NULL,		          SPTAG(4),		1,			 0,			0,		 -1 },
};
//}}}

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "H[]",      deck },

	{ "===",      bstackhoriz },
	{ "TTT",      bstack },
	{ "HHH",      grid },
	{ "###",      nrowgrid },

	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c", "-l", "10", NULL };
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,						XK_Return, spawn,          {.v = termcmd } },

	// DMENU PROGRAMS
	{ MODKEY|ControlMask,           XK_u,      spawn,          SHCMD("dunicode") },
	{ MODKEY|ControlMask,           XK_o,      spawn,          SHCMD("dfileopen") },
	{ MODKEY|ControlMask,           XK_s,      spawn,          SHCMD("dwebsearch") },
	{ MODKEY|ControlMask,           XK_t,      spawn,          SHCMD("dtodo") },
	{ MODKEY|ControlMask,           XK_n,      spawn,          SHCMD("dnotes") },
	{ MODKEY|ControlMask,           XK_d,      spawn,          SHCMD("dswitch") },

	// VOLUME CONTROL
    { MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("smixer mute") },
    { MODKEY|ControlMask,           XK_m,      spawn,          SHCMD("smixer mic") },
    { MODKEY,                       XK_plus,   spawn,          SHCMD("smixer up 1") },
    { MODKEY|ShiftMask,             XK_plus,   spawn,          SHCMD("smixer up 5") },
    { MODKEY,                       XK_minus,  spawn,          SHCMD("smixer down 1") },
    { MODKEY|ShiftMask,             XK_minus,  spawn,          SHCMD("smixer down 5") },

	// MUSIC CONTROL
    { MODKEY,                       XK_p,      spawn,          SHCMD("smixer pause") },
    { MODKEY,                       XK_n,      spawn,          SHCMD("smixer next") },
    { MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("smixer prev") },

	// MOUNT EXTERNALS
    { MODKEY,                       XK_F9,      spawn,         SHCMD("dmount -m") },
    { MODKEY,                       XK_F10,     spawn,         SHCMD("dmount -u") },

	// TOGGLE BARS
	{ MODKEY|ShiftMask,             XK_d,      togglebar,      {0} },
	{ MODKEY,                       XK_w,      tabmode,        {-1} },

	// FOCUS
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

	// TOGGLE MASTER
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },

	// MOVE
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },

	// RESIZE
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },

	// GAPS
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY,						XK_g,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,				XK_g,      defaultgaps,    {0} },

	// SYSTEM
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("dpowermenu") },
	{ MODKEY|ShiftMask,             XK_F4,     quit,           {0} },

	// LAYOUTS
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_b,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_b,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_z,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,             XK_Tab,    setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	// MOVE-RESIZE-FLOATS
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },

	// MULTI-MONITOR
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	// SCRATCHPADS
	{ MODKEY,            			XK_space,  togglescratch,  {.ui = 0 } },
	{ MODKEY,            			XK_m,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,            			XK_y,	   togglescratch,  {.ui = 2 } },
	{ MODKEY,            			XK_a,	   togglescratch,  {.ui = 3 } },
	{ MODKEY,            			XK_o,	   togglescratch,  {.ui = 4 } },

	// TAGS
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
};

