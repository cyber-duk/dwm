/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static unsigned int systrayspacing = 2;   /* systray spacing */
static int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static int showsystray        = 1;     /* 0 means no systray */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "monospace:size=10" };
static char color0[]       = "#161821";
static char color1[]       = "#e27878";
static char color2[]       = "#b4be62";
static char color3[]       = "#e2a478";
static char color4[]       = "#84a0c6";
static char color5[]       = "#a093c7";
static char color6[]       = "#89b8c2";
static char color7[]       = "#c6c8d1";

static char *colors[][4]      = {
	/*               fg      bg      border  float  */
	[SchemeNorm] = { color7, color0, color1, color3 },
	[SchemeSel]  = { color0, color7, color2, color4 },
	[SchemeHid]  = { color0, color5, color6, color7 }, /* last two unused */
};

static int focusonclick            = 1;   /* 1 means focus on click */
static unsigned int fborderpx      = 3;   /* border pixel for floating windows */
static int floatbordercol          = 1;   /* 1 means different border color for floating windows */
static int smartborder             = 1;   /* 0 means no border when monocle mode/one tiled window */
static unsigned int gappih         = 20;  /* horiz inner gap between windows */
static unsigned int gappiv         = 10;  /* vert inner gap between windows */
static unsigned int gappoh         = 10;  /* horiz outer gap between windows and screen edge */
static unsigned int gappov         = 30;  /* vert outer gap between windows and screen edge */
static int smartgaps               = 0;   /* 1 means no outer gap when there is only one window */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title   tags mask  switchtag  iscentered  isfloating   monitor */
	{ "Gimp",           NULL,       NULL,   0,         1,         0,          1,           -1 },
	{ "Firefox",        NULL,       NULL,   1 << 8,    1,         0,          0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define MODOPT Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|MODOPT,                KEY,      sendtag,        {.ui = 1 << TAG} }, \
	{ MODKEY|MODOPT|ShiftMask,      KEY,      swaptag,        {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char *termcmd[]  = { "st", NULL };
static char *dmenucmd[]  = { "dmenu_run", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",			STRING,  &color0 },
		{ "color1",			STRING,  &color1 },
		{ "color2",			STRING,  &color2 },
		{ "color3",			STRING,  &color3 },
		{ "color4",			STRING,  &color4 },
		{ "color5",			STRING,  &color5 },
		{ "color6",			STRING,  &color6 },
		{ "color7",			STRING,  &color7 },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",			INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",			INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",			FLOAT,   &mfact },
		{ "fborderpx",		INTEGER, &fborderpx },
		{ "focusonclick",	INTEGER, &focusonclick },
		{ "floatbordercol",	INTEGER, &floatbordercol },
		{ "smartborder",	INTEGER, &smartborder },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY|ControlMask,           XK_space,  focusmaster,    {0} },
	{ MODKEY,                       XK_s,      swapfocus,      {.i = -1 } },
	{ MODKEY,                       XK_Left,   shiftviewc,     {.i = -1 } },       // switch to the prev tag containing a client  
	{ MODKEY,                       XK_Right,  shiftviewc,     {.i = +1 } },       // switch to the next tag containing a client
	{ MODKEY|ShiftMask,             XK_Left,   shifttagc,      {.i = -1 } },       // send focused client to the prev tag containing a client 
	{ MODKEY|ShiftMask,             XK_Right,  shifttagc,      {.i = +1 } },       // send focused client to the next tag containing a client
	{ MODKEY|MODOPT,                XK_Left,   shiftview,      {.i = -1 } },       // switch to the prev tag
	{ MODKEY|MODOPT,                XK_Right,  shiftview,      {.i = +1 } },       // switch to the next tag
	{ MODKEY|MODOPT|ShiftMask,      XK_Left,   shifttag,       {.i = -1 } },       // send client to the prev tag
	{ MODKEY|MODOPT|ShiftMask,      XK_Right,  shifttag,       {.i = +1 } },       // send client to the next tag
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|MODOPT,                XK_0,      togglegaps,     {0} },
	{ MODOPT,                       XK_Left,   focusdir,       {.i = 0 } }, // left
	{ MODOPT,                       XK_Right,  focusdir,       {.i = 1 } }, // right
	{ MODOPT,                       XK_Up,     focusdir,       {.i = 2 } }, // up
	{ MODOPT,                       XK_Down,   focusdir,       {.i = 3 } }, // down
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        showtitlewin,   {0} },
	{ ClkWinTitle,          0,              Button2,        zoomtitlewin,   {0} },
	{ ClkWinTitle,          0,              Button3,        hidetitlewin,   {0} },
	{ ClkWinTitle,          MODKEY,         Button3,        killtitlewin,   {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,      dragcfact,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,      dragmfact,      {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

