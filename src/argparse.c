// SPDX-License-Identifier: MPL-2.0
// Copyright (c) Yuxuan Shui <yshuiv7@gmail.com>

#include <getopt.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>

#include "argparse.h"
#include "common.h"
#include "config.h"

/**
 * Print usage text and exit.
 */
static void usage(int ret) {
#define WARNING_DISABLED " (DISABLED AT COMPILE TIME)"
#define WARNING
	static const char *usage_text =
	    "compton (" COMPTON_VERSION ")\n"
	    "This is the maintenance fork of compton, please report\n"
	    "bugs to https://github.com/yshui/compton\n\n"
	    "usage: compton [options]\n"
	    "Options:\n"
	    "\n"
	    "-r radius\n"
	    "  The blur radius for shadows. (default 12)\n"
	    "\n"
	    "-o opacity\n"
	    "  The translucency for shadows. (default .75)\n"
	    "\n"
	    "-l left-offset\n"
	    "  The left offset for shadows. (default -15)\n"
	    "\n"
	    "-t top-offset\n"
	    "  The top offset for shadows. (default -15)\n"
	    "\n"
	    "-I fade-in-step\n"
	    "  Opacity change between steps while fading in. (default 0.028)\n"
	    "\n"
	    "-O fade-out-step\n"
	    "  Opacity change between steps while fading out. (default 0.03)\n"
	    "\n"
	    "-D fade-delta-time\n"
	    "  The time between steps in a fade in milliseconds. (default 10)\n"
	    "\n"
	    "-m opacity\n"
	    "  The opacity for menus. (default 1.0)\n"
	    "\n"
	    "-c\n"
	    "  Enabled client-side shadows on windows.\n"
	    "\n"
	    "-C\n"
	    "  Avoid drawing shadows on dock/panel windows.\n"
	    "\n"
	    "-z\n"
	    "  Zero the part of the shadow's mask behind the window.\n"
	    "\n"
	    "-f\n"
	    "  Fade windows in/out when opening/closing and when opacity\n"
	    "  changes, unless --no-fading-openclose is used.\n"
	    "\n"
	    "-F\n"
	    "  Equals to -f. Deprecated.\n"
	    "\n"
	    "-i opacity\n"
	    "  Opacity of inactive windows. (0.1 - 1.0)\n"
	    "\n"
	    "-e opacity\n"
	    "  Opacity of window titlebars and borders. (0.1 - 1.0)\n"
	    "\n"
	    "-G\n"
	    "  Don't draw shadows on DND windows\n"
	    "\n"
	    "-b\n"
	    "  Daemonize process.\n"
	    "\n"
	    "--show-all-xerrors\n"
	    "  Show all X errors (for debugging).\n"
	    "\n"
#undef WARNING
#ifndef CONFIG_LIBCONFIG
#define WARNING WARNING_DISABLED
#else
#define WARNING
#endif
	    "--config path\n"
	    "  Look for configuration file at the path. Use /dev/null to avoid\n"
	    "  loading configuration file." WARNING "\n"
	    "\n"
	    "--write-pid-path path\n"
	    "  Write process ID to a file.\n"
	    "\n"
	    "--shadow-red value\n"
	    "  Red color value of shadow (0.0 - 1.0, defaults to 0).\n"
	    "\n"
	    "--shadow-green value\n"
	    "  Green color value of shadow (0.0 - 1.0, defaults to 0).\n"
	    "\n"
	    "--shadow-blue value\n"
	    "  Blue color value of shadow (0.0 - 1.0, defaults to 0).\n"
	    "\n"
	    "--inactive-opacity-override\n"
	    "  Inactive opacity set by -i overrides value of _NET_WM_OPACITY.\n"
	    "\n"
	    "--inactive-dim value\n"
	    "  Dim inactive windows. (0.0 - 1.0, defaults to 0)\n"
	    "\n"
	    "--active-opacity opacity\n"
	    "  Default opacity for active windows. (0.0 - 1.0)\n"
	    "\n"
	    "--mark-wmwin-focused\n"
	    "  Try to detect WM windows and mark them as active.\n"
	    "\n"
	    "--shadow-exclude condition\n"
	    "  Exclude conditions for shadows.\n"
	    "\n"
	    "--fade-exclude condition\n"
	    "  Exclude conditions for fading.\n"
	    "\n"
	    "--mark-ovredir-focused\n"
	    "  Mark windows that have no WM frame as active.\n"
	    "\n"
	    "--no-fading-openclose\n"
	    "  Do not fade on window open/close.\n"
	    "\n"
	    "--no-fading-destroyed-argb\n"
	    "  Do not fade destroyed ARGB windows with WM frame. Workaround of bugs\n"
	    "  in Openbox, Fluxbox, etc.\n"
	    "\n"
	    "--shadow-ignore-shaped\n"
	    "  Do not paint shadows on shaped windows. (Deprecated, use\n"
	    "  --shadow-exclude \'bounding_shaped\' or\n"
	    "  --shadow-exclude \'bounding_shaped && !rounded_corners\' instead.)\n"
	    "\n"
	    "--detect-rounded-corners\n"
	    "  Try to detect windows with rounded corners and don't consider\n"
	    "  them shaped windows. Affects --shadow-ignore-shaped,\n"
	    "  --unredir-if-possible, and possibly others. You need to turn this\n"
	    "  on manually if you want to match against rounded_corners in\n"
	    "  conditions.\n"
	    "\n"
	    "--detect-client-opacity\n"
	    "  Detect _NET_WM_OPACITY on client windows, useful for window\n"
	    "  managers not passing _NET_WM_OPACITY of client windows to frame\n"
	    "  windows.\n"
	    "\n"
	    "--refresh-rate val\n"
	    "  Specify refresh rate of the screen. If not specified or 0, compton\n"
	    "  will try detecting this with X RandR extension.\n"
	    "\n"
	    "--vsync vsync-method\n"
	    "  Set VSync method. There are (up to) 5 VSync methods currently\n"
	    "  available:\n"
	    "    none = No VSync\n"
#undef WARNING
#ifndef CONFIG_VSYNC_DRM
#define WARNING WARNING_DISABLED
#else
#define WARNING
#endif
	    "    drm = VSync with DRM_IOCTL_WAIT_VBLANK. May only work on some\n"
	    "      (DRI-based) drivers." WARNING "\n"
#undef WARNING
#ifndef CONFIG_OPENGL
#define WARNING WARNING_DISABLED
#else
#define WARNING
#endif
	    "    opengl = Try to VSync with SGI_video_sync OpenGL extension. Only\n"
	    "      work on some drivers." WARNING "\n"
	    "    opengl-oml = Try to VSync with OML_sync_control OpenGL extension.\n"
	    "      Only work on some drivers." WARNING "\n"
	    "    opengl-swc = Enable driver-level VSync. Works only with GLX "
	    "backend." WARNING "\n"
	    "    opengl-mswc = Deprecated, use opengl-swc instead." WARNING "\n"
	    "\n"
	    "--vsync-aggressive\n"
	    "  Attempt to send painting request before VBlank and do XFlush()\n"
	    "  during VBlank. This switch may be lifted out at any moment.\n"
	    "\n"
	    "--paint-on-overlay\n"
	    "  Painting on X Composite overlay window.\n"
	    "\n"
	    "--sw-opti\n"
	    "  Limit compton to repaint at most once every 1 / refresh_rate\n"
	    "  second to boost performance.\n"
	    "\n"
	    "--use-ewmh-active-win\n"
	    "  Use _NET_WM_ACTIVE_WINDOW on the root window to determine which\n"
	    "  window is focused instead of using FocusIn/Out events.\n"
	    "\n"
	    "--respect-prop-shadow\n"
	    "  Respect _COMPTON_SHADOW. This a prototype-level feature, which\n"
	    "  you must not rely on.\n"
	    "\n"
	    "--unredir-if-possible\n"
	    "  Unredirect all windows if a full-screen opaque window is\n"
	    "  detected, to maximize performance for full-screen windows.\n"
	    "\n"
	    "--unredir-if-possible-delay ms\n"
	    "  Delay before unredirecting the window, in milliseconds.\n"
	    "  Defaults to 0.\n"
	    "\n"
	    "--unredir-if-possible-exclude condition\n"
	    "  Conditions of windows that shouldn't be considered full-screen\n"
	    "  for unredirecting screen.\n"
	    "\n"
	    "--focus-exclude condition\n"
	    "  Specify a list of conditions of windows that should always be\n"
	    "  considered focused.\n"
	    "\n"
	    "--inactive-dim-fixed\n"
	    "  Use fixed inactive dim value.\n"
	    "\n"
	    "--detect-transient\n"
	    "  Use WM_TRANSIENT_FOR to group windows, and consider windows in\n"
	    "  the same group focused at the same time.\n"
	    "\n"
	    "--detect-client-leader\n"
	    "  Use WM_CLIENT_LEADER to group windows, and consider windows in\n"
	    "  the same group focused at the same time. WM_TRANSIENT_FOR has\n"
	    "  higher priority if --detect-transient is enabled, too.\n"
	    "\n"
	    "--blur-background\n"
	    "  Blur background of semi-transparent / ARGB windows. Bad in\n"
	    "  performance. The switch name may change without prior\n"
	    "  notifications.\n"
	    "\n"
	    "--blur-background-frame\n"
	    "  Blur background of windows when the window frame is not opaque.\n"
	    "  Implies --blur-background. Bad in performance. The switch name\n"
	    "  may change.\n"
	    "\n"
	    "--blur-background-fixed\n"
	    "  Use fixed blur strength instead of adjusting according to window\n"
	    "  opacity.\n"
	    "\n"
	    "--blur-kern matrix\n"
	    "  Specify the blur convolution kernel, with the following format:\n"
	    "    WIDTH,HEIGHT,ELE1,ELE2,ELE3,ELE4,ELE5...\n"
	    "  The element in the center must not be included, it will be forever\n"
	    "  1.0 or changing based on opacity, depending on whether you have\n"
	    "  --blur-background-fixed.\n"
	    "  A 7x7 Gaussian blur kernel looks like:\n"
	    "    --blur-kern "
	    "'7,7,0.000003,0.000102,0.000849,0.001723,0.000849,0.000102,0.000003,0."
	    "000102,0.003494,0.029143,0.059106,0.029143,0.003494,0.000102,0.000849,0."
	    "029143,0.243117,0.493069,0.243117,0.029143,0.000849,0.001723,0.059106,0."
	    "493069,0.493069,0.059106,0.001723,0.000849,0.029143,0.243117,0.493069,0."
	    "243117,0.029143,0.000849,0.000102,0.003494,0.029143,0.059106,0.029143,0."
	    "003494,0.000102,0.000003,0.000102,0.000849,0.001723,0.000849,0.000102,0."
	    "000003'\n"
	    "  Up to 4 blur kernels may be specified, separated with semicolon, for\n"
	    "  multi-pass blur.\n"
	    "  May also be one the predefined kernels: 3x3box (default), 5x5box,\n"
	    "  7x7box, 3x3gaussian, 5x5gaussian, 7x7gaussian, 9x9gaussian,\n"
	    "  11x11gaussian.\n"
	    "\n"
	    "--blur-background-exclude condition\n"
	    "  Exclude conditions for background blur.\n"
	    "\n"
	    "--resize-damage integer\n"
	    "  Resize damaged region by a specific number of pixels. A positive\n"
	    "  value enlarges it while a negative one shrinks it. Useful for\n"
	    "  fixing the line corruption issues of blur. May or may not\n"
	    "  work with --glx-no-stencil. Shrinking doesn't function correctly.\n"
	    "\n"
	    "--invert-color-include condition\n"
	    "  Specify a list of conditions of windows that should be painted with\n"
	    "  inverted color. Resource-hogging, and is not well tested.\n"
	    "\n"
	    "--opacity-rule opacity:condition\n"
	    "  Specify a list of opacity rules, in the format \"PERCENT:PATTERN\",\n"
	    "  like \'50:name *= \"Firefox\"'. compton-trans is recommended over\n"
	    "  this. Note we do not distinguish 100% and unset, and we don't make\n"
	    "  any guarantee about possible conflicts with other programs that set\n"
	    "  _NET_WM_WINDOW_OPACITY on frame or client windows.\n"
	    "\n"
	    "--shadow-exclude-reg geometry\n"
	    "  Specify a X geometry that describes the region in which shadow\n"
	    "  should not be painted in, such as a dock window region.\n"
	    "  Use --shadow-exclude-reg \'x10+0-0\', for example, if the 10 pixels\n"
	    "  on the bottom of the screen should not have shadows painted on.\n"
#undef WARNING
#ifndef CONFIG_XINERAMA
#define WARNING WARNING_DISABLED
#else
#define WARNING
#endif
	    "\n"
	    "--xinerama-shadow-crop\n"
	    "  Crop shadow of a window fully on a particular Xinerama screen to the\n"
	    "  screen." WARNING "\n"
	    "\n"
#undef WARNING
#ifndef CONFIG_OPENGL
#define WARNING "(GLX BACKENDS DISABLED AT COMPILE TIME)"
#else
#define WARNING
#endif
	    "--backend backend\n"
	    "  Choose backend. Possible choices are xrender, glx, and\n"
	    "  xr_glx_hybrid" WARNING ".\n"
	    "\n"
	    "--glx-no-stencil\n"
	    "  GLX backend: Avoid using stencil buffer. Might cause issues\n"
	    "  when rendering transparent content. My tests show a 15% performance\n"
	    "  boost.\n"
	    "\n"
	    "--glx-no-rebind-pixmap\n"
	    "  GLX backend: Avoid rebinding pixmap on window damage. Probably\n"
	    "  could improve performance on rapid window content changes, but is\n"
	    "  known to break things on some drivers (LLVMpipe, xf86-video-intel,\n"
	    "  etc.).\n"
	    "\n"
	    "--glx-swap-method undefined/copy/exchange/3/4/5/6/buffer-age\n"
	    "  GLX backend: GLX buffer swap method we assume. Could be\n"
	    "  undefined (0), copy (1), exchange (2), 3-6, or buffer-age (-1).\n"
	    "  \"undefined\" is the slowest and the safest, and the default value.\n"
	    "  1 is fastest, but may fail on some drivers, 2-6 are gradually slower\n"
	    "  but safer (6 is still faster than 0). -1 means auto-detect using\n"
	    "  GLX_EXT_buffer_age, supported by some drivers. \n"
	    "\n"
	    "--glx-use-gpushader4\n"
	    "  GLX backend: Use GL_EXT_gpu_shader4 for some optimization on blur\n"
	    "  GLSL code. My tests on GTX 670 show no noticeable effect.\n"
	    "\n"
	    "--xrender-sync\n"
	    "  Attempt to synchronize client applications' draw calls with XSync(),\n"
	    "  used on GLX backend to ensure up-to-date window content is painted.\n"
#undef WARNING
#define WARNING
	    "\n"
	    "--xrender-sync-fence\n"
	    "  Additionally use X Sync fence to sync clients' draw calls. Needed\n"
	    "  on nvidia-drivers with GLX backend for some users." WARNING "\n"
	    "\n"
	    "--force-win-blend\n"
	    "  Force all windows to be painted with blending. Useful if you have a\n"
	    "  --glx-fshader-win that could turn opaque pixels transparent.\n"
	    "\n"
#undef WARNING
#ifndef CONFIG_DBUS
#define WARNING WARNING_DISABLED
#else
#define WARNING
#endif
	    "--dbus\n"
	    "  Enable remote control via D-Bus. See the D-BUS API section in the\n"
	    "  man page for more details." WARNING "\n"
	    "\n"
	    "--benchmark cycles\n"
	    "  Benchmark mode. Repeatedly paint until reaching the specified cycles.\n"
	    "\n"
	    "--benchmark-wid window-id\n"
	    "  Specify window ID to repaint in benchmark mode. If omitted or is 0,\n"
	    "  the whole screen is repainted.\n"
	    "--monitor-repaint\n"
	    "  Highlight the updated area of the screen. For debugging the xrender\n"
	    "  backend only.\n";
	FILE *f = (ret ? stderr : stdout);
	fputs(usage_text, f);
#undef WARNING
#undef WARNING_DISABLED
}

static const char *shortopts = "D:I:O:d:r:o:m:l:t:i:e:hscnfFCaSzGb";
static const struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"config", required_argument, NULL, 256},
    {"shadow-radius", required_argument, NULL, 'r'},
    {"shadow-opacity", required_argument, NULL, 'o'},
    {"shadow-offset-x", required_argument, NULL, 'l'},
    {"shadow-offset-y", required_argument, NULL, 't'},
    {"fade-in-step", required_argument, NULL, 'I'},
    {"fade-out-step", required_argument, NULL, 'O'},
    {"fade-delta", required_argument, NULL, 'D'},
    {"menu-opacity", required_argument, NULL, 'm'},
    {"shadow", no_argument, NULL, 'c'},
    {"no-dock-shadow", no_argument, NULL, 'C'},
    {"clear-shadow", no_argument, NULL, 'z'},
    {"fading", no_argument, NULL, 'f'},
    {"inactive-opacity", required_argument, NULL, 'i'},
    {"frame-opacity", required_argument, NULL, 'e'},
    {"daemon", no_argument, NULL, 'b'},
    {"no-dnd-shadow", no_argument, NULL, 'G'},
    {"shadow-red", required_argument, NULL, 257},
    {"shadow-green", required_argument, NULL, 258},
    {"shadow-blue", required_argument, NULL, 259},
    {"inactive-opacity-override", no_argument, NULL, 260},
    {"inactive-dim", required_argument, NULL, 261},
    {"mark-wmwin-focused", no_argument, NULL, 262},
    {"shadow-exclude", required_argument, NULL, 263},
    {"mark-ovredir-focused", no_argument, NULL, 264},
    {"no-fading-openclose", no_argument, NULL, 265},
    {"shadow-ignore-shaped", no_argument, NULL, 266},
    {"detect-rounded-corners", no_argument, NULL, 267},
    {"detect-client-opacity", no_argument, NULL, 268},
    {"refresh-rate", required_argument, NULL, 269},
    {"vsync", required_argument, NULL, 270},
    {"alpha-step", required_argument, NULL, 271},
    {"dbe", no_argument, NULL, 272},
    {"paint-on-overlay", no_argument, NULL, 273},
    {"sw-opti", no_argument, NULL, 274},
    {"vsync-aggressive", no_argument, NULL, 275},
    {"use-ewmh-active-win", no_argument, NULL, 276},
    {"respect-prop-shadow", no_argument, NULL, 277},
    {"unredir-if-possible", no_argument, NULL, 278},
    {"focus-exclude", required_argument, NULL, 279},
    {"inactive-dim-fixed", no_argument, NULL, 280},
    {"detect-transient", no_argument, NULL, 281},
    {"detect-client-leader", no_argument, NULL, 282},
    {"blur-background", no_argument, NULL, 283},
    {"blur-background-frame", no_argument, NULL, 284},
    {"blur-background-fixed", no_argument, NULL, 285},
    {"dbus", no_argument, NULL, 286},
    {"logpath", required_argument, NULL, 287},
    {"invert-color-include", required_argument, NULL, 288},
    {"opengl", no_argument, NULL, 289},
    {"backend", required_argument, NULL, 290},
    {"glx-no-stencil", no_argument, NULL, 291},
    {"glx-copy-from-front", no_argument, NULL, 292},
    {"benchmark", required_argument, NULL, 293},
    {"benchmark-wid", required_argument, NULL, 294},
    {"glx-use-copysubbuffermesa", no_argument, NULL, 295},
    {"blur-background-exclude", required_argument, NULL, 296},
    {"active-opacity", required_argument, NULL, 297},
    {"glx-no-rebind-pixmap", no_argument, NULL, 298},
    {"glx-swap-method", required_argument, NULL, 299},
    {"fade-exclude", required_argument, NULL, 300},
    {"blur-kern", required_argument, NULL, 301},
    {"resize-damage", required_argument, NULL, 302},
    {"glx-use-gpushader4", no_argument, NULL, 303},
    {"opacity-rule", required_argument, NULL, 304},
    {"shadow-exclude-reg", required_argument, NULL, 305},
    {"paint-exclude", required_argument, NULL, 306},
    {"xinerama-shadow-crop", no_argument, NULL, 307},
    {"unredir-if-possible-exclude", required_argument, NULL, 308},
    {"unredir-if-possible-delay", required_argument, NULL, 309},
    {"write-pid-path", required_argument, NULL, 310},
    {"vsync-use-glfinish", no_argument, NULL, 311},
    {"xrender-sync", no_argument, NULL, 312},
    {"xrender-sync-fence", no_argument, NULL, 313},
    {"show-all-xerrors", no_argument, NULL, 314},
    {"no-fading-destroyed-argb", no_argument, NULL, 315},
    {"force-win-blend", no_argument, NULL, 316},
    {"glx-fshader-win", required_argument, NULL, 317},
    {"version", no_argument, NULL, 318},
    {"no-x-selection", no_argument, NULL, 319},
    {"no-name-pixmap", no_argument, NULL, 320},
    {"log-level", required_argument, NULL, 321},
    {"reredir-on-root-change", no_argument, NULL, 731},
    {"glx-reinit-on-root-change", no_argument, NULL, 732},
    {"monitor-repaint", no_argument, NULL, 800},
    {"diagnostics", no_argument, NULL, 801},
    // Must terminate with a NULL entry
    {NULL, 0, NULL, 0},
};

/// Get config options that are needed to parse the rest of the options
/// Return true if we should quit
bool get_early_config(int argc, char *const *argv, char **config_file, bool *all_xerrors,
                      int *exit_code) {
	int o = 0, longopt_idx = -1;

	// Pre-parse the commandline arguments to check for --config and invalid
	// switches
	// Must reset optind to 0 here in case we reread the commandline
	// arguments
	optind = 1;
	while (-1 != (o = getopt_long(argc, argv, shortopts, longopts, &longopt_idx))) {
		if (o == 256)
			*config_file = strdup(optarg);
		else if (o == 'd') {
			log_warn("-d will be ignored, please use the DISPLAY "
			         "environment variable");
		} else if (o == 314) {
			*all_xerrors = true;
		} else if (o == 318) {
			printf("%s\n", COMPTON_VERSION);
			*exit_code = 0;
			return true;
		} else if (o == 'S') {
			log_warn("-S will be ignored");
		} else if (o == 320) {
			log_warn("--no-name-pixmap will be ignored");
		} else if (o == '?' || o == ':') {
			usage(1);
			*exit_code = 1;
			return true;
		}
	}

	// Check for abundant positional arguments
	if (optind < argc)
		log_fatal("compton doesn't accept positional arguments.");

	return false;
}

/**
 * Process arguments and configuration files.
 */
void get_cfg(session_t *ps, int argc, char *const *argv) {

	int o = 0, longopt_idx = -1;

	bool shadow_enable = false, fading_enable = false;
	char *lc_numeric_old = strdup(setlocale(LC_NUMERIC, NULL));

	win_option_mask_t winopt_mask[NUM_WINTYPES] = {{0}};

	// Enforce LC_NUMERIC locale "C" here to make sure dots are recognized
	// instead of commas in atof().
	setlocale(LC_NUMERIC, "C");

	parse_config(ps, &shadow_enable, &fading_enable, winopt_mask);

	// Parse commandline arguments. Range checking will be done later.

	const char *deprecation_message = "has been removed. If you encounter problems "
	                                  "without this feature, please feel free to "
	                                  "open a bug report.";
	optind = 1;
	while (-1 != (o = getopt_long(argc, argv, shortopts, longopts, &longopt_idx))) {
		long val = 0;
		switch (o) {
#define P_CASEBOOL(idx, option)                                                          \
	case idx:                                                                        \
		ps->o.option = true;                                                     \
		break
#define P_CASELONG(idx, option)                                                          \
	case idx:                                                                        \
		if (!parse_long(optarg, &val))                                           \
			exit(1);                                                         \
		ps->o.option = val;                                                      \
		break

		// Short options
		case 'h': usage(0); break;
		case 'd':
		case 'S':
		case 314:
		case 318:
		case 320: break; P_CASELONG('D', fade_delta);
		case 'I': ps->o.fade_in_step = normalize_d(atof(optarg)) * OPAQUE; break;
		case 'O': ps->o.fade_out_step = normalize_d(atof(optarg)) * OPAQUE; break;
		case 'c': shadow_enable = true; break;
		case 'C':
			winopt_mask[WINTYPE_DOCK].shadow = true;
			ps->o.wintype_option[WINTYPE_DOCK].shadow = true;
			break;
		case 'G':
			winopt_mask[WINTYPE_DND].shadow = true;
			ps->o.wintype_option[WINTYPE_DND].shadow = true;
			break;
		case 'm':;
			double tmp;
			tmp = normalize_d(atof(optarg));
			winopt_mask[WINTYPE_DROPDOWN_MENU].opacity = true;
			winopt_mask[WINTYPE_POPUP_MENU].opacity = true;
			ps->o.wintype_option[WINTYPE_POPUP_MENU].opacity = tmp;
			ps->o.wintype_option[WINTYPE_DROPDOWN_MENU].opacity = tmp;
			break;
		case 'f':
		case 'F':
			fading_enable = true;
			break;
			P_CASELONG('r', shadow_radius);
		case 'o':
			ps->o.shadow_opacity = atof(optarg);
			break;
			P_CASELONG('l', shadow_offset_x);
			P_CASELONG('t', shadow_offset_y);
		case 'i':
			ps->o.inactive_opacity = (normalize_d(atof(optarg)) * OPAQUE);
			break;
		case 'e': ps->o.frame_opacity = atof(optarg); break;
		case 'z':
			log_warn("clear-shadow is removed, shadows are automatically "
			         "cleared now. If you want to prevent shadow from been "
			         "cleared under certain types of windows, you can use "
			         "the \"full-shadow\" per window type option.");
			break;
		case 'n':
		case 'a':
		case 's':
			log_error("-n, -a, and -s have been removed.");
			break;
			P_CASEBOOL('b', fork_after_register);
		// Long options
		case 256:
			// --config
			break;
		case 257:
			// --shadow-red
			ps->o.shadow_red = atof(optarg);
			break;
		case 258:
			// --shadow-green
			ps->o.shadow_green = atof(optarg);
			break;
		case 259:
			// --shadow-blue
			ps->o.shadow_blue = atof(optarg);
			break;
			P_CASEBOOL(260, inactive_opacity_override);
		case 261:
			// --inactive-dim
			ps->o.inactive_dim = atof(optarg);
			break;
			P_CASEBOOL(262, mark_wmwin_focused);
		case 263:
			// --shadow-exclude
			condlst_add(ps, &ps->o.shadow_blacklist, optarg);
			break;
			P_CASEBOOL(264, mark_ovredir_focused);
			P_CASEBOOL(265, no_fading_openclose);
			P_CASEBOOL(266, shadow_ignore_shaped);
			P_CASEBOOL(267, detect_rounded_corners);
			P_CASEBOOL(268, detect_client_opacity);
			P_CASELONG(269, refresh_rate);
		case 270:
			// --vsync
			if (!parse_vsync(ps, optarg))
				exit(1);
			break;
		case 271:
			// --alpha-step
			log_warn("--alpha-step has been removed, compton now tries to "
			         "make use of all alpha values");
			break;
		case 272: log_warn("use of --dbe is deprecated"); break;
		case 273:
			log_warn("--paint-on-overlay has been removed, and is enabled "
			         "when possible");
			break;
			P_CASEBOOL(274, sw_opti);
			P_CASEBOOL(275, vsync_aggressive);
			P_CASEBOOL(276, use_ewmh_active_win);
			P_CASEBOOL(277, respect_prop_shadow);
			P_CASEBOOL(278, unredir_if_possible);
		case 279:
			// --focus-exclude
			condlst_add(ps, &ps->o.focus_blacklist, optarg);
			break;
			P_CASEBOOL(280, inactive_dim_fixed);
			P_CASEBOOL(281, detect_transient);
			P_CASEBOOL(282, detect_client_leader);
			P_CASEBOOL(283, blur_background);
			P_CASEBOOL(284, blur_background_frame);
			P_CASEBOOL(285, blur_background_fixed);
			P_CASEBOOL(286, dbus);
		case 287:
			// --logpath
			ps->o.logpath = strdup(optarg);
			break;
		case 288:
			// --invert-color-include
			condlst_add(ps, &ps->o.invert_color_list, optarg);
			break;
		case 289:
			// --opengl
			ps->o.backend = BKEND_GLX;
			break;
		case 290:
			// --backend
			if (!parse_backend(ps, optarg))
				exit(1);
			break;
			P_CASEBOOL(291, glx_no_stencil);
		case 292:
			log_warn("--glx-copy-from-front %s", deprecation_message);
			break;
			P_CASELONG(293, benchmark);
		case 294:
			// --benchmark-wid
			ps->o.benchmark_wid = strtol(optarg, NULL, 0);
			break;
		case 295:
			log_warn("--glx-use-copysubbuffermesa %s", deprecation_message);
			break;
		case 296:
			// --blur-background-exclude
			condlst_add(ps, &ps->o.blur_background_blacklist, optarg);
			break;
		case 297:
			// --active-opacity
			ps->o.active_opacity = (normalize_d(atof(optarg)) * OPAQUE);
			break;
			P_CASEBOOL(298, glx_no_rebind_pixmap);
		case 299:
			// --glx-swap-method
			if (!parse_glx_swap_method(ps, optarg))
				exit(1);
			break;
		case 300:
			// --fade-exclude
			condlst_add(ps, &ps->o.fade_blacklist, optarg);
			break;
		case 301:
			// --blur-kern
			if (!parse_conv_kern_lst(ps, optarg, ps->o.blur_kerns,
			                         MAX_BLUR_PASS))
				exit(1);
			break;
			P_CASELONG(302, resize_damage);
			P_CASEBOOL(303, glx_use_gpushader4);
		case 304:
			// --opacity-rule
			if (!parse_rule_opacity(ps, optarg))
				exit(1);
			break;
		case 305:
			// --shadow-exclude-reg
			ps->o.shadow_exclude_reg_str = strdup(optarg);
			log_warn("--shadow-exclude-reg is deprecated. You are likely "
			         "better off using --shadow-exclude anyway");
			break;
		case 306:
			// --paint-exclude
			condlst_add(ps, &ps->o.paint_blacklist, optarg);
			break;
			P_CASEBOOL(307, xinerama_shadow_crop);
		case 308:
			// --unredir-if-possible-exclude
			condlst_add(ps, &ps->o.unredir_if_possible_blacklist, optarg);
			break;
			P_CASELONG(309, unredir_if_possible_delay);
		case 310:
			// --write-pid-path
			ps->o.write_pid_path = strdup(optarg);
			break;
			P_CASEBOOL(311, vsync_use_glfinish);
			P_CASEBOOL(312, xrender_sync);
			P_CASEBOOL(313, xrender_sync_fence);
			P_CASEBOOL(315, no_fading_destroyed_argb);
			P_CASEBOOL(316, force_win_blend);
		case 317:
			ps->o.glx_fshader_win_str = strdup(optarg);
			log_warn("--glx-fshader-win is being deprecated, and might be "
			         "removed in the future. If you really need this "
			         "feature, please report an issue to let us know");
			break;
		case 321: {
			enum log_level tmp_level = string_to_log_level(optarg);
			if (tmp_level == LOG_LEVEL_INVALID) {
				log_warn("Invalid log level, defaults to WARN");
			} else {
				log_set_level_tls(tmp_level);
			}
			break;
		}
			P_CASEBOOL(319, no_x_selection);
			P_CASEBOOL(731, reredir_on_root_change);
			P_CASEBOOL(732, glx_reinit_on_root_change);
			P_CASEBOOL(800, monitor_repaint);
		case 801: ps->o.print_diagnostics = true; break;
		default: usage(1); break;
#undef P_CASEBOOL
		}
	}

	// Restore LC_NUMERIC
	setlocale(LC_NUMERIC, lc_numeric_old);
	free(lc_numeric_old);

	if (ps->o.monitor_repaint && ps->o.backend != BKEND_XRENDER) {
		log_warn("--monitor-repaint has no effect when backend is not xrender");
	}

	// Range checking and option assignments
	ps->o.fade_delta = max_i(ps->o.fade_delta, 1);
	ps->o.shadow_radius = max_i(ps->o.shadow_radius, 0);
	ps->o.shadow_red = normalize_d(ps->o.shadow_red);
	ps->o.shadow_green = normalize_d(ps->o.shadow_green);
	ps->o.shadow_blue = normalize_d(ps->o.shadow_blue);
	ps->o.inactive_dim = normalize_d(ps->o.inactive_dim);
	ps->o.frame_opacity = normalize_d(ps->o.frame_opacity);
	ps->o.shadow_opacity = normalize_d(ps->o.shadow_opacity);
	ps->o.refresh_rate = normalize_i_range(ps->o.refresh_rate, 0, 300);

	// Apply default wintype options that are dependent on global options
	for (int i = 0; i < NUM_WINTYPES; i++) {
		auto wo = &ps->o.wintype_option[i];
		auto mask = &winopt_mask[i];
		if (!mask->shadow) {
			wo->shadow = shadow_enable;
			mask->shadow = true;
		}
		if (!mask->fade) {
			wo->fade = fading_enable;
			mask->fade = true;
		}
	}

	// --blur-background-frame implies --blur-background
	if (ps->o.blur_background_frame)
		ps->o.blur_background = true;

	if (ps->o.xrender_sync_fence)
		ps->o.xrender_sync = true;

	// Other variables determined by options

	// Determine whether we need to track focus changes
	if (ps->o.inactive_opacity != ps->o.active_opacity || ps->o.inactive_dim) {
		ps->o.track_focus = true;
	}

	// Determine whether we track window grouping
	if (ps->o.detect_transient || ps->o.detect_client_leader) {
		ps->o.track_leader = true;
	}

	// Fill default blur kernel
	if (ps->o.blur_background && !ps->o.blur_kerns[0]) {
		// Convolution filter parameter (box blur)
		// gaussian or binomial filters are definitely superior, yet looks
		// like they aren't supported as of xorg-server-1.13.0
		static const xcb_render_fixed_t convolution_blur[] = {
		    // Must convert to XFixed with DOUBLE_TO_XFIXED()
		    // Matrix size
		    DOUBLE_TO_XFIXED(3),
		    DOUBLE_TO_XFIXED(3),
		    // Matrix
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		    DOUBLE_TO_XFIXED(1),
		};
		ps->o.blur_kerns[0] =
		    ccalloc(ARR_SIZE(convolution_blur), xcb_render_fixed_t);
		memcpy(ps->o.blur_kerns[0], convolution_blur, sizeof(convolution_blur));
	}

	if (ps->o.resize_damage < 0)
		log_warn("Negative --resize-damage will not work correctly.");
}

// vim: set noet sw=8 ts=8 :