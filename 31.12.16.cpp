#include "stdafx.h"
#include "natives.h"
#include "script.h"
#include "keyboard.h"
#include <string>
#include <ctime>
#include <WinBase.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib")

// MENYOO BASE ~ Coded by d3sk1ng
using namespace std;

namespace SUB {
	enum {
		// Define submenu (enum) indices here.
		CLOSED,
		MAINMENU,
		SELF,
		ONLINE_PLAYERS,
		ALL_ONLINE_PLAYERS,
		VEHICLES,
		WEAPONS,
		OBJECTS,
		WORLD,
		TELEPORT,
		MISC,
		PARTICLEFX,
		RECOVERY,
		SETTINGS,

		ONLINE_PLAYER_ACTIONS,
		PLAYER_VEHICLE,
		PLAYER_PED_OPTIONS,
		PLAYER_UNSAFE_THINGS,
		PLAYER_TELEPORT_THINGS,
		ATTACH_PLAYER_THINGS,
		ENTITY_SPAM_MENU,
		PARTICLE_FX_MENU,
		PLAYER_WEAPONS,
		PLAYER_GRIEFING,
		PLAYER_SEND_ATTACKERS,
		PLAYER_DOES_ANIM,

		ATTACH_OBJECT_TO_ALL_PLAYERS,
		ALL_ONLINE_ATTACKERS,

		VEHICLE_SPAWNER,
		VEHICLE_SPAWNER_CHOOSE,
		VEHICLE_MOBILE_LSC,
		VEHICLE_MULTIPLIERS,

		SELECT_OBJECT_CATEGORY,
		COOL_OBJECTS_LIST,
		STUNT_DLC_LIST,
		FULL_OBJECTS_LIST,
		SPAWNED_OBJECTS_DATABASE,

		MAPMOD,
		MAPMOD_MAZEDEMO,
		MAPMOD_MAZEROOFRAMP,
		MAPMOD_BEACHFERRISRAMP,
		MAPMOD_MOUNTCHILLIADRAMP,
		MAPMOD_AIRPORTMINIRAMP,
		MAPMOD_AIRPORTGATERAMP,
		MAPMOD_UFOTOWER,
		MAPMOD_MAZEBANKRAMPS,
		MAPMOD_FREESTYLEMOTOCROSS,
		MAPMOD_HALFPIPEFUNTRACK,
		MAPMOD_AIRPORTLOOP,
		MAPMOD_MAZEBANKMEGARAMP,

		IPL,

		ESP,
		CLEAR_AREA,
		MENU_PROTECTIONS,
		WEATHER,
		SCREEN_EFFECTS,
		NEARBY_VEHICLES,
		NEARBY_PEDS,
		MENU_INFO,

		SELF_MODEL_CHANGER,
		SELF_SCENARIOS,
		SELF_ANIMATIONS,
		OUTFITS,
		SELECT_PLAYER_MODEL,

		OBJECT_EDITOR,

		SETTINGS_COLOURS,
		SETTINGS_COLOURS2,
		SETTINGS_FONTS,
		SETTINGS_FONTS2,


	};
};


namespace {

	char* StringToChar(std::string string)
	{
		return _strdup(string.c_str());
	}

	static Player playerID;
	static Ped playerPed;


	// Declare/Initialise global variables here.


	static bool isPaidVersion = false; 	// FREE OR PAID VERSION BOOL (not really safe, i will improve it)
	static bool isTesterVersion = false; // TESTER VERSION (some griefing)

	static bool isMenuOpen = false;

	static int keyUpTimer = 1;
	static int keyDownTimer = 1;

	static bool canBackBePressed = true;
	static bool canEnterBePressed = true;

	static bool isBlackout = false;

	static bool attackersGodMode = false;
	unsigned __int16 latestSubBeforeClosing = SUB::MAINMENU;

	char str[50];
	bool controllerinput_bool = 1, menujustopened = 1, null;

	char* vehicleWeapons[] = { "VEHICLE_WEAPON_TANK", "VEHICLE_WEAPON_ROTORS", "VEHICLE_WEAPON_SPACE_ROCKET", "VEHICLE_WEAPON_PLANE_ROCKET", "VEHICLE_WEAPON_PLAYER_LAZER", "VEHICLE_WEAPON_PLAYER_LASER", "VEHICLE_WEAPON_PLAYER_BULLET", "VEHICLE_WEAPON_PLAYER_BUZZARD", "VEHICLE_WEAPON_PLAYER_HUNTER", "VEHICLE_WEAPON_ENEMY_LASER", "VEHICLE_WEAPON_SEARCHLIGHT", "VEHICLE_WEAPON_RADAR", "VEHICLE_WEAPON_WATER_CANNON", "VEHICLE_WEAPON_TURRET_INSURGENT", "VEHICLE_WEAPON_TURRET_TECHNICAL", "VEHICLE_WEAPON_NOSE_TURRET_VALKYRIE", "VEHICLE_WEAPON_PLAYER_SAVAGE" };
	int selectedVehicleWeapon = 0;

	float optionYBonus = 0.123f;
	float optionXBonus = 0.264f;
	float backXBonus = 0.331f;
	float titleYBonus = -0.019f;
	float backgroundYBonus = -0.135f;
	float backgroundLengthBonus = -0.193f;
	float titleBoxBonus = 0.045f;
	float bwidth = 0.135f;
	float titleXBonus = 0.171f;
	float titleTextYBonus = -0.016f;
	float titleScale = 0.590f;
	float titleScaleSmall = 0.540f;

	enum ControllerInputs
	{
		INPUT_NEXT_CAMERA = 0,
		INPUT_LOOK_LR = 1,
		INPUT_LOOK_UD = 2,
		INPUT_LOOK_UP_ONLY = 3,
		INPUT_LOOK_DOWN_ONLY = 4,
		INPUT_LOOK_LEFT_ONLY = 5,
		INPUT_LOOK_RIGHT_ONLY = 6,
		INPUT_CINEMATIC_SLOWMO = 7,
		INPUT_SCRIPTED_FLY_UD = 8,
		INPUT_SCRIPTED_FLY_LR = 9,
		INPUT_SCRIPTED_FLY_ZUP = 10,
		INPUT_SCRIPTED_FLY_ZDOWN = 11,
		INPUT_WEAPON_WHEEL_UD = 12,
		INPUT_WEAPON_WHEEL_LR = 13,
		INPUT_WEAPON_WHEEL_NEXT = 14,
		INPUT_WEAPON_WHEEL_PREV = 15,
		INPUT_SELECT_NEXT_WEAPON = 16,
		INPUT_SELECT_PREV_WEAPON = 17,
		INPUT_SKIP_CUTSCENE = 18,
		INPUT_CHARACTER_WHEEL = 19,
		INPUT_MULTIPLAYER_INFO = 20,
		INPUT_SPRINT = 21,
		INPUT_JUMP = 22,
		INPUT_ENTER = 23,
		INPUT_ATTACK = 24,
		INPUT_AIM = 25,
		INPUT_LOOK_BEHIND = 26,
		INPUT_PHONE = 27,
		INPUT_SPECIAL_ABILITY = 28,
		INPUT_SPECIAL_ABILITY_SECONDARY = 29,
		INPUT_MOVE_LR = 30,
		INPUT_MOVE_UD = 31,
		INPUT_MOVE_UP_ONLY = 32,
		INPUT_MOVE_DOWN_ONLY = 33,
		INPUT_MOVE_LEFT_ONLY = 34,
		INPUT_MOVE_RIGHT_ONLY = 35,
		INPUT_DUCK = 36,
		INPUT_SELECT_WEAPON = 37,
		INPUT_PICKUP = 38,
		INPUT_SNIPER_ZOOM = 39,
		INPUT_SNIPER_ZOOM_IN_ONLY = 40,
		INPUT_SNIPER_ZOOM_OUT_ONLY = 41,
		INPUT_SNIPER_ZOOM_IN_SECONDARY = 42,
		INPUT_SNIPER_ZOOM_OUT_SECONDARY = 43,
		INPUT_COVER = 44,
		INPUT_RELOAD = 45,
		INPUT_TALK = 46,
		INPUT_DETONATE = 47,
		INPUT_HUD_SPECIAL = 48,
		INPUT_ARREST = 49,
		INPUT_ACCURATE_AIM = 50,
		INPUT_CONTEXT = 51,
		INPUT_CONTEXT_SECONDARY = 52,
		INPUT_WEAPON_SPECIAL = 53,
		INPUT_WEAPON_SPECIAL_TWO = 54,
		INPUT_DIVE = 55,
		INPUT_DROP_WEAPON = 56,
		INPUT_DROP_AMMO = 57,
		INPUT_THROW_GRENADE = 58,
		INPUT_VEH_MOVE_LR = 59,
		INPUT_VEH_MOVE_UD = 60,
		INPUT_VEH_MOVE_UP_ONLY = 61,
		INPUT_VEH_MOVE_DOWN_ONLY = 62,
		INPUT_VEH_MOVE_LEFT_ONLY = 63,
		INPUT_VEH_MOVE_RIGHT_ONLY = 64,
		INPUT_VEH_SPECIAL = 65,
		INPUT_VEH_GUN_LR = 66,
		INPUT_VEH_GUN_UD = 67,
		INPUT_VEH_AIM = 68,
		INPUT_VEH_ATTACK = 69,
		INPUT_VEH_ATTACK2 = 70,
		INPUT_VEH_ACCELERATE = 71,
		INPUT_VEH_BRAKE = 72,
		INPUT_VEH_DUCK = 73,
		INPUT_VEH_HEADLIGHT = 74,
		INPUT_VEH_EXIT = 75,
		INPUT_VEH_HANDBRAKE = 76,
		INPUT_VEH_HOTWIRE_LEFT = 77,
		INPUT_VEH_HOTWIRE_RIGHT = 78,
		INPUT_VEH_LOOK_BEHIND = 79,
		INPUT_VEH_CIN_CAM = 80,
		INPUT_VEH_NEXT_RADIO = 81,
		INPUT_VEH_PREV_RADIO = 82,
		INPUT_VEH_NEXT_RADIO_TRACK = 83,
		INPUT_VEH_PREV_RADIO_TRACK = 84,
		INPUT_VEH_RADIO_WHEEL = 85,
		INPUT_VEH_HORN = 86,
		INPUT_VEH_FLY_THROTTLE_UP = 87,
		INPUT_VEH_FLY_THROTTLE_DOWN = 88,
		INPUT_VEH_FLY_YAW_LEFT = 89,
		INPUT_VEH_FLY_YAW_RIGHT = 90,
		INPUT_VEH_PASSENGER_AIM = 91,
		INPUT_VEH_PASSENGER_ATTACK = 92,
		INPUT_VEH_SPECIAL_ABILITY_FRANKLIN = 93,
		INPUT_VEH_STUNT_UD = 94,
		INPUT_VEH_CINEMATIC_UD = 95,
		INPUT_VEH_CINEMATIC_UP_ONLY = 96,
		INPUT_VEH_CINEMATIC_DOWN_ONLY = 97,
		INPUT_VEH_CINEMATIC_LR = 98,
		INPUT_VEH_SELECT_NEXT_WEAPON = 99,
		INPUT_VEH_SELECT_PREV_WEAPON = 100,
		INPUT_VEH_ROOF = 101,
		INPUT_VEH_JUMP = 102,
		INPUT_VEH_GRAPPLING_HOOK = 103,
		INPUT_VEH_SHUFFLE = 104,
		INPUT_VEH_DROP_PROJECTILE = 105,
		INPUT_VEH_MOUSE_CONTROL_OVERRIDE = 106,
		INPUT_VEH_FLY_ROLL_LR = 107,
		INPUT_VEH_FLY_ROLL_LEFT_ONLY = 108,
		INPUT_VEH_FLY_ROLL_RIGHT_ONLY = 109,
		INPUT_VEH_FLY_PITCH_UD = 110,
		INPUT_VEH_FLY_PITCH_UP_ONLY = 111,
		INPUT_VEH_FLY_PITCH_DOWN_ONLY = 112,
		INPUT_VEH_FLY_UNDERCARRIAGE = 113,
		INPUT_VEH_FLY_ATTACK = 114,
		INPUT_VEH_FLY_SELECT_NEXT_WEAPON = 115,
		INPUT_VEH_FLY_SELECT_PREV_WEAPON = 116,
		INPUT_VEH_FLY_SELECT_TARGET_LEFT = 117,
		INPUT_VEH_FLY_SELECT_TARGET_RIGHT = 118,
		INPUT_VEH_FLY_VERTICAL_FLIGHT_MODE = 119,
		INPUT_VEH_FLY_DUCK = 120,
		INPUT_VEH_FLY_ATTACK_CAMERA = 121,
		INPUT_VEH_FLY_MOUSE_CONTROL_OVERRIDE = 122,
		INPUT_VEH_SUB_TURN_LR = 123,
		INPUT_VEH_SUB_TURN_LEFT_ONLY = 124,
		INPUT_VEH_SUB_TURN_RIGHT_ONLY = 125,
		INPUT_VEH_SUB_PITCH_UD = 126,
		INPUT_VEH_SUB_PITCH_UP_ONLY = 127,
		INPUT_VEH_SUB_PITCH_DOWN_ONLY = 128,
		INPUT_VEH_SUB_THROTTLE_UP = 129,
		INPUT_VEH_SUB_THROTTLE_DOWN = 130,
		INPUT_VEH_SUB_ASCEND = 131,
		INPUT_VEH_SUB_DESCEND = 132,
		INPUT_VEH_SUB_TURN_HARD_LEFT = 133,
		INPUT_VEH_SUB_TURN_HARD_RIGHT = 134,
		INPUT_VEH_SUB_MOUSE_CONTROL_OVERRIDE = 135,
		INPUT_VEH_PUSHBIKE_PEDAL = 136,
		INPUT_VEH_PUSHBIKE_SPRINT = 137,
		INPUT_VEH_PUSHBIKE_FRONT_BRAKE = 138,
		INPUT_VEH_PUSHBIKE_REAR_BRAKE = 139,
		INPUT_MELEE_ATTACK_LIGHT = 140,
		INPUT_MELEE_ATTACK_HEAVY = 141,
		INPUT_MELEE_ATTACK_ALTERNATE = 142,
		INPUT_MELEE_BLOCK = 143,
		INPUT_PARACHUTE_DEPLOY = 144,
		INPUT_PARACHUTE_DETACH = 145,
		INPUT_PARACHUTE_TURN_LR = 146,
		INPUT_PARACHUTE_TURN_LEFT_ONLY = 147,
		INPUT_PARACHUTE_TURN_RIGHT_ONLY = 148,
		INPUT_PARACHUTE_PITCH_UD = 149,
		INPUT_PARACHUTE_PITCH_UP_ONLY = 150,
		INPUT_PARACHUTE_PITCH_DOWN_ONLY = 151,
		INPUT_PARACHUTE_BRAKE_LEFT = 152,
		INPUT_PARACHUTE_BRAKE_RIGHT = 153,
		INPUT_PARACHUTE_SMOKE = 154,
		INPUT_PARACHUTE_PRECISION_LANDING = 155,
		INPUT_MAP = 156,
		INPUT_SELECT_WEAPON_UNARMED = 157,
		INPUT_SELECT_WEAPON_MELEE = 158,
		INPUT_SELECT_WEAPON_HANDGUN = 159,
		INPUT_SELECT_WEAPON_SHOTGUN = 160,
		INPUT_SELECT_WEAPON_SMG = 161,
		INPUT_SELECT_WEAPON_AUTO_RIFLE = 162,
		INPUT_SELECT_WEAPON_SNIPER = 163,
		INPUT_SELECT_WEAPON_HEAVY = 164,
		INPUT_SELECT_WEAPON_SPECIAL = 165,
		INPUT_SELECT_CHARACTER_MICHAEL = 166,
		INPUT_SELECT_CHARACTER_FRANKLIN = 167,
		INPUT_SELECT_CHARACTER_TREVOR = 168,
		INPUT_SELECT_CHARACTER_MULTIPLAYER = 169,
		INPUT_SAVE_REPLAY_CLIP = 170,
		INPUT_SPECIAL_ABILITY_PC = 171,
		INPUT_CELLPHONE_UP = 172,
		INPUT_CELLPHONE_DOWN = 173,
		INPUT_CELLPHONE_LEFT = 174,
		INPUT_CELLPHONE_RIGHT = 175,
		INPUT_CELLPHONE_SELECT = 176,
		INPUT_CELLPHONE_CANCEL = 177,
		INPUT_CELLPHONE_OPTION = 178,
		INPUT_CELLPHONE_EXTRA_OPTION = 179,
		INPUT_CELLPHONE_SCROLL_FORWARD = 180,
		INPUT_CELLPHONE_SCROLL_BACKWARD = 181,
		INPUT_CELLPHONE_CAMERA_FOCUS_LOCK = 182,
		INPUT_CELLPHONE_CAMERA_GRID = 183,
		INPUT_CELLPHONE_CAMERA_SELFIE = 184,
		INPUT_CELLPHONE_CAMERA_DOF = 185,
		INPUT_CELLPHONE_CAMERA_EXPRESSION = 186,
		INPUT_FRONTEND_DOWN = 187,
		INPUT_FRONTEND_UP = 188,
		INPUT_FRONTEND_LEFT = 189,
		INPUT_FRONTEND_RIGHT = 190,
		INPUT_FRONTEND_RDOWN = 191,
		INPUT_FRONTEND_RUP = 192,
		INPUT_FRONTEND_RLEFT = 193,
		INPUT_FRONTEND_RRIGHT = 194,
		INPUT_FRONTEND_AXIS_X = 195,
		INPUT_FRONTEND_AXIS_Y = 196,
		INPUT_FRONTEND_RIGHT_AXIS_X = 197,
		INPUT_FRONTEND_RIGHT_AXIS_Y = 198,
		INPUT_FRONTEND_PAUSE = 199,
		INPUT_FRONTEND_PAUSE_ALTERNATE = 200,
		INPUT_FRONTEND_ACCEPT = 201,
		INPUT_FRONTEND_CANCEL = 202,
		INPUT_FRONTEND_X = 203,
		INPUT_FRONTEND_Y = 204,
		INPUT_FRONTEND_LB = 205,
		INPUT_FRONTEND_RB = 206,
		INPUT_FRONTEND_LT = 207,
		INPUT_FRONTEND_RT = 208,
		INPUT_FRONTEND_LS = 209,
		INPUT_FRONTEND_RS = 210,
		INPUT_FRONTEND_LEADERBOARD = 211,
		INPUT_FRONTEND_SOCIAL_CLUB = 212,
		INPUT_FRONTEND_SOCIAL_CLUB_SECONDARY = 213,
		INPUT_FRONTEND_DELETE = 214,
		INPUT_FRONTEND_ENDSCREEN_ACCEPT = 215,
		INPUT_FRONTEND_ENDSCREEN_EXPAND = 216,
		INPUT_FRONTEND_SELECT = 217,
		INPUT_SCRIPT_LEFT_AXIS_X = 218,
		INPUT_SCRIPT_LEFT_AXIS_Y = 219,
		INPUT_SCRIPT_RIGHT_AXIS_X = 220,
		INPUT_SCRIPT_RIGHT_AXIS_Y = 221,
		INPUT_SCRIPT_RUP = 222,
		INPUT_SCRIPT_RDOWN = 223,
		INPUT_SCRIPT_RLEFT = 224,
		INPUT_SCRIPT_RRIGHT = 225,
		INPUT_SCRIPT_LB = 226,
		INPUT_SCRIPT_RB = 227,
		INPUT_SCRIPT_LT = 228,
		INPUT_SCRIPT_RT = 229,
		INPUT_SCRIPT_LS = 230,
		INPUT_SCRIPT_RS = 231,
		INPUT_SCRIPT_PAD_UP = 232,
		INPUT_SCRIPT_PAD_DOWN = 233,
		INPUT_SCRIPT_PAD_LEFT = 234,
		INPUT_SCRIPT_PAD_RIGHT = 235,
		INPUT_SCRIPT_SELECT = 236,
		INPUT_CURSOR_ACCEPT = 237,
		INPUT_CURSOR_CANCEL = 238,
		INPUT_CURSOR_X = 239,
		INPUT_CURSOR_Y = 240,
		INPUT_CURSOR_SCROLL_UP = 241,
		INPUT_CURSOR_SCROLL_DOWN = 242,
		INPUT_ENTER_CHEAT_CODE = 243,
		INPUT_INTERACTION_MENU = 244,
		INPUT_MP_TEXT_CHAT_ALL = 245,
		INPUT_MP_TEXT_CHAT_TEAM = 246,
		INPUT_MP_TEXT_CHAT_FRIENDS = 247,
		INPUT_MP_TEXT_CHAT_CREW = 248,
		INPUT_PUSH_TO_TALK = 249,
		INPUT_CREATOR_LS = 250,
		INPUT_CREATOR_RS = 251,
		INPUT_CREATOR_LT = 252,
		INPUT_CREATOR_RT = 253,
		INPUT_CREATOR_MENU_TOGGLE = 254,
		INPUT_CREATOR_ACCEPT = 255,
		INPUT_CREATOR_DELETE = 256,
		INPUT_ATTACK2 = 257,
		INPUT_RAPPEL_JUMP = 258,
		INPUT_RAPPEL_LONG_JUMP = 259,
		INPUT_RAPPEL_SMASH_WINDOW = 260,
		INPUT_PREV_WEAPON = 261,
		INPUT_NEXT_WEAPON = 262,
		INPUT_MELEE_ATTACK1 = 263,
		INPUT_MELEE_ATTACK2 = 264,
		INPUT_WHISTLE = 265,
		INPUT_MOVE_LEFT = 266,
		INPUT_MOVE_RIGHT = 267,
		INPUT_MOVE_UP = 268,
		INPUT_MOVE_DOWN = 269,
		INPUT_LOOK_LEFT = 270,
		INPUT_LOOK_RIGHT = 271,
		INPUT_LOOK_UP = 272,
		INPUT_LOOK_DOWN = 273,
		INPUT_SNIPER_ZOOM_IN = 274,
		INPUT_SNIPER_ZOOM_OUT = 275,
		INPUT_SNIPER_ZOOM_IN_ALTERNATE = 276,
		INPUT_SNIPER_ZOOM_OUT_ALTERNATE = 277,
		INPUT_VEH_MOVE_LEFT = 278,
		INPUT_VEH_MOVE_RIGHT = 279,
		INPUT_VEH_MOVE_UP = 280,
		INPUT_VEH_MOVE_DOWN = 281,
		INPUT_VEH_GUN_LEFT = 282,
		INPUT_VEH_GUN_RIGHT = 283,
		INPUT_VEH_GUN_UP = 284,
		INPUT_VEH_GUN_DOWN = 285,
		INPUT_VEH_LOOK_LEFT = 286,
		INPUT_VEH_LOOK_RIGHT = 287,
		INPUT_REPLAY_START_STOP_RECORDING = 288,
		INPUT_REPLAY_START_STOP_RECORDING_SECONDARY = 289,
		INPUT_SCALED_LOOK_LR = 290,
		INPUT_SCALED_LOOK_UD = 291,
		INPUT_SCALED_LOOK_UP_ONLY = 292,
		INPUT_SCALED_LOOK_DOWN_ONLY = 293,
		INPUT_SCALED_LOOK_LEFT_ONLY = 294,
		INPUT_SCALED_LOOK_RIGHT_ONLY = 295,
		INPUT_REPLAY_MARKER_DELETE = 296,
		INPUT_REPLAY_CLIP_DELETE = 297,
		INPUT_REPLAY_PAUSE = 298,
		INPUT_REPLAY_REWIND = 299,
		INPUT_REPLAY_FFWD = 300,
		INPUT_REPLAY_NEWMARKER = 301,
		INPUT_REPLAY_RECORD = 302,
		INPUT_REPLAY_SCREENSHOT = 303,
		INPUT_REPLAY_HIDEHUD = 304,
		INPUT_REPLAY_STARTPOINT = 305,
		INPUT_REPLAY_ENDPOINT = 306,
		INPUT_REPLAY_ADVANCE = 307,
		INPUT_REPLAY_BACK = 308,
		INPUT_REPLAY_TOOLS = 309,
		INPUT_REPLAY_RESTART = 310,
		INPUT_REPLAY_SHOWHOTKEY = 311,
		INPUT_REPLAY_CYCLEMARKERLEFT = 312,
		INPUT_REPLAY_CYCLEMARKERRIGHT = 313,
		INPUT_REPLAY_FOVINCREASE = 314,
		INPUT_REPLAY_FOVDECREASE = 315,
		INPUT_REPLAY_CAMERAUP = 316,
		INPUT_REPLAY_CAMERADOWN = 317,
		INPUT_REPLAY_SAVE = 318,
		INPUT_REPLAY_TOGGLETIME = 319,
		INPUT_REPLAY_TOGGLETIPS = 320,
		INPUT_REPLAY_PREVIEW = 321,
		INPUT_REPLAY_TOGGLE_TIMELINE = 322,
		INPUT_REPLAY_TIMELINE_PICKUP_CLIP = 323,
		INPUT_REPLAY_TIMELINE_DUPLICATE_CLIP = 324,
		INPUT_REPLAY_TIMELINE_PLACE_CLIP = 325,
		INPUT_REPLAY_CTRL = 326,
		INPUT_REPLAY_TIMELINE_SAVE = 327,
		INPUT_REPLAY_PREVIEW_AUDIO = 328,
		INPUT_VEH_DRIVE_LOOK = 329,
		INPUT_VEH_DRIVE_LOOK2 = 330,
		INPUT_VEH_FLY_ATTACK2 = 331,
		INPUT_RADIO_WHEEL_UD = 332,
		INPUT_RADIO_WHEEL_LR = 333,
		INPUT_VEH_SLOWMO_UD = 334,
		INPUT_VEH_SLOWMO_UP_ONLY = 335,
		INPUT_VEH_SLOWMO_DOWN_ONLY = 336,
		INPUT_MAP_POI = 337
	};


	namespace COL { enum COL { TITLEBOX, BACKGROUND, TITLETEXT, OPTIONTEXT, OPTIONCOUNT, SELECTEDTEXT, BREAKS, SELECTIONHIGH }; }
	namespace FON { enum FON { TITLE = 10, OPTION, SELECTEDTEXT, BREAKS }; }
	int *settings_font, inull;
	RGBA *settings_rgba;
	RGBA titlebox = { 0, 0, 160, 255 };
	RGBA BG = { 20, 20, 20, 200 };
	RGBA titletext = { 255, 255, 255, 255 };
	RGBA optiontext = { 255, 255, 255, 255 };
	RGBA optioncount = { 255, 255, 255, 255 };
	RGBA selectedtext = { 255, 255, 255, 255 };
	RGBA optionbreaks = { 255, 255, 255, 240 };
	RGBA selectionhi = { 255, 255, 255, 110 };
	int font_title = 7, font_options = 4, font_selection = 4, font_breaks = 1;
	float menuPos = 0, OptionY;
	int screen_res_x, screen_res_y;
	DWORD myVeh, cam_gta2;
	float current_timescale = 1.0f;

	int vehDamMult = 0, vehDefMult = 0;
	bool vehDamageMult = 0, vehDefenseMult = 0;
	float AccelerationMultiplier = 0, BrakesMultiplier = 0, SuspensionHeight = 0;

	bool rpmMultiplier = 0, torqueMultiplier = 0;
	float lightMultiplier = 0;
	int rpm = 0, torque = 0;

	// Booleans for loops go here:
	bool godMode = 0, neverWanted = 0, superJump = 0, ghostMode = 0, superRun = 0, noRagdoll = 0, neverDraggedOut = 0, vehicleCollisions = 1, freezedPlayer = -1, vehicleGodMode = 0, invisibleVehicle = 0, noBikeFall = 0, superCarMode = 0, autoFixVehicle = 0, vehicleWeapon = 0, vehicleLasers = 0, lockAllVehicleDoors = 0, vehicleLowGrip = 0, vehicleAlwaysBurnout = 0, flyMode = 0, vehrpm = 0, vehTorque = 0, explosiveBullets = 0, explosiveMelee = 0, heatVisionOnAim = 0, oneShotKill = 0, aimBot = 0, infiniteAmmo = 0, rapidFire = 0, weaponsEverywhere = 0, deleteGun = 0, driveItGun = 0, teleportGun = 0, shootRhino = 0, mobileRadio = 0, displayCoords = 0, explodeNearbyVehicles = 0, killNearbyPeds = 0, maxNearbyVehicles, esp = 0, wtfMode = 0, derailedTrain = 0, pedDropHack = 0, rpHack = 0, loop_massacre_mode = 0, loop_RainbowBoxes = 0, gravityGun = 0, rainbowNearbyVehicles = 0, jumpLoopNearbyVehicles = 0, loop_gta2cam = 0;
	bool showESPNames = 0;
	bool box3D = 1;
	float ESPNameSize = 0.35f;
	RGBA boxColor = { 255, 0, 0, 255 };
	float entityDistance = 10.0f;
	float pickupDistance = 100.0f;
	bool forceGunPeds = 1;
	bool forceGunVehicles = 1;
	bool forceGunObjects = 1;
	bool initialized = 1;

	int openMenu_HOTKEY = VK_MULTIPLY;
	int teleportToWaypoint_HOTKEY = VK_F7;
	int teleportUp_HOTKEY = VK_F8;
	int vehicleWeapons_HOTKEY = VK_SUBTRACT;

	static int scrollDelay = 180;
	static int addIntEasyDelay = 180;

	std::string myProfilePath = getenv(StringToChar("USERPROFILE"));
	std::string menuFolderPath = myProfilePath + "\\Documents\\D3SK1NG_MENU";
	std::string menuConfigIniPath = menuFolderPath + "\\config.ini";

	void SaveAllIniSettings() {
		WritePrivateProfileStringA("HOTKEYS", "OPEN_MENU", std::to_string(openMenu_HOTKEY).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("HOTKEYS", "TELEPORT_TO_WAYPOINT", std::to_string(teleportToWaypoint_HOTKEY).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("HOTKEYS", "TELEPORT_UP", std::to_string(teleportUp_HOTKEY).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("HOTKEYS", "VEHICLE_WEAPONS", std::to_string(vehicleWeapons_HOTKEY).c_str(), menuConfigIniPath.c_str());

		WritePrivateProfileStringA("OTHER_SETTINGS", "AUTO_SCROLL_MS_DELAY", std::to_string(scrollDelay).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OTHER_SETTINGS", "INT_SCROLL_SPEED", std::to_string(addIntEasyDelay).c_str(), menuConfigIniPath.c_str());
		
		// TITLE BOX
		WritePrivateProfileStringA("TITLE_BOX", "RED", std::to_string(titlebox.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("TITLE_BOX", "GREEN", std::to_string(titlebox.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("TITLE_BOX", "BLUE", std::to_string(titlebox.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("TITLE_BOX", "OPACITY", std::to_string(titlebox.A).c_str(), menuConfigIniPath.c_str());
		// BACKGROUND BOX
		WritePrivateProfileStringA("BACKGROUND_BOX", "RED", std::to_string(BG.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("BACKGROUND_BOX", "GREEN", std::to_string(BG.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("BACKGROUND_BOX", "BLUE", std::to_string(BG.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("BACKGROUND_BOX", "OPACITY", std::to_string(BG.A).c_str(), menuConfigIniPath.c_str());
		// TITLE TEXT
		WritePrivateProfileStringA("TITLE_TEXT", "RED", std::to_string(titletext.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("TITLE_TEXT", "GREEN", std::to_string(titletext.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("TITLE_TEXT", "BLUE", std::to_string(titletext.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("TITLE_TEXT", "OPACITY", std::to_string(titletext.A).c_str(), menuConfigIniPath.c_str());
		// OPTION TEXT
		WritePrivateProfileStringA("OPTION_TEXT", "RED", std::to_string(optiontext.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_TEXT", "GREEN", std::to_string(optiontext.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_TEXT", "BLUE", std::to_string(optiontext.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_TEXT", "OPACITY", std::to_string(optiontext.A).c_str(), menuConfigIniPath.c_str());
		// OPTION COUNT
		WritePrivateProfileStringA("OPTION_COUNT", "RED", std::to_string(optioncount.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_COUNT", "GREEN", std::to_string(optioncount.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_COUNT", "BLUE", std::to_string(optioncount.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_COUNT", "OPACITY", std::to_string(optioncount.A).c_str(), menuConfigIniPath.c_str());
		// SELECTED TEXT
		WritePrivateProfileStringA("SELECTED_TEXT", "RED", std::to_string(selectedtext.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("SELECTED_TEXT", "GREEN", std::to_string(selectedtext.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("SELECTED_TEXT", "BLUE", std::to_string(selectedtext.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("SELECTED_TEXT", "OPACITY", std::to_string(selectedtext.A).c_str(), menuConfigIniPath.c_str());
		// OPTION BREAK
		WritePrivateProfileStringA("OPTION_BREAK", "RED", std::to_string(optionbreaks.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_BREAK", "GREEN", std::to_string(optionbreaks.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_BREAK", "BLUE", std::to_string(optionbreaks.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("OPTION_BREAK", "OPACITY", std::to_string(optionbreaks.A).c_str(), menuConfigIniPath.c_str());
		// SELECTED OPTION
		WritePrivateProfileStringA("SELECTED_OPTION", "RED", std::to_string(selectionhi.R).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("SELECTED_OPTION", "GREEN", std::to_string(selectionhi.G).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("SELECTED_OPTION", "BLUE", std::to_string(selectionhi.B).c_str(), menuConfigIniPath.c_str());
		WritePrivateProfileStringA("SELECTED_OPTION", "OPACITY", std::to_string(selectionhi.A).c_str(), menuConfigIniPath.c_str());
	}
	void LoadAllIniSettings() {
		char currentOptionBuffer[100];
		// HOTKEYS
		GetPrivateProfileStringA("HOTKEYS", "OPEN_MENU", std::to_string(VK_MULTIPLY).c_str(), currentOptionBuffer, 100, menuConfigIniPath.c_str());
		openMenu_HOTKEY = (DWORD)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("HOTKEYS", "TELEPORT_TO_WAYPOINT", std::to_string(VK_F7).c_str(), currentOptionBuffer, 100, menuConfigIniPath.c_str());
		teleportToWaypoint_HOTKEY = (DWORD)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("HOTKEYS", "TELEPORT_UP", std::to_string(VK_F8).c_str(), currentOptionBuffer, 100, menuConfigIniPath.c_str());
		teleportUp_HOTKEY = (DWORD)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("HOTKEYS", "VEHICLE_WEAPONS", std::to_string(VK_SUBTRACT).c_str(), currentOptionBuffer, 100, menuConfigIniPath.c_str());
		vehicleWeapons_HOTKEY = (DWORD)strtod(currentOptionBuffer, NULL);

		// OTHER SETTINGS
		GetPrivateProfileStringA("OTHER_SETTINGS", "AUTO_SCROLL_MS_DELAY", "180", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		scrollDelay = (int)strtod(currentOptionBuffer, NULL);

		GetPrivateProfileStringA("OTHER_SETTINGS", "INT_SCROLL_SPEED", "40", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		addIntEasyDelay = (int)strtod(currentOptionBuffer, NULL);

		// TITLE BOX
		GetPrivateProfileStringA("TITLE_BOX", "RED", "0", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titlebox.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("TITLE_BOX", "GREEN", "0", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titlebox.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("TITLE_BOX", "BLUE", "150", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titlebox.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("TITLE_BOX", "OPACITY", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titlebox.A = (int)strtod(currentOptionBuffer, NULL);
		// BACKGROUND BOX
		GetPrivateProfileStringA("BACKGROUND_BOX", "RED", "0", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		BG.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("BACKGROUND_BOX", "GREEN", "0", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		BG.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("BACKGROUND_BOX", "BLUE", "0", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		BG.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("BACKGROUND_BOX", "OPACITY", "75", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		BG.A = (int)strtod(currentOptionBuffer, NULL);
		// TITLE TEXT
		GetPrivateProfileStringA("TITLE_TEXT", "RED", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titletext.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("TITLE_TEXT", "GREEN", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titletext.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("TITLE_TEXT", "BLUE", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titletext.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("TITLE_TEXT", "OPACITY", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		titletext.A = (int)strtod(currentOptionBuffer, NULL);
		// OPTION TEXT
		GetPrivateProfileStringA("OPTION_TEXT", "RED", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optiontext.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_TEXT", "GREEN", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optiontext.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_TEXT", "BLUE", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optiontext.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_TEXT", "OPACITY", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optiontext.A = (int)strtod(currentOptionBuffer, NULL);
		// OPTION COUNT
		GetPrivateProfileStringA("OPTION_COUNT", "RED", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optioncount.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_COUNT", "GREEN", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optioncount.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_COUNT", "BLUE", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optioncount.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_COUNT", "OPACITY", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optioncount.A = (int)strtod(currentOptionBuffer, NULL);
		// SELECTED TEXT
		GetPrivateProfileStringA("SELECTED_TEXT", "RED", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectedtext.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("SELECTED_TEXT", "GREEN", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectedtext.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("SELECTED_TEXT", "BLUE", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectedtext.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("SELECTED_TEXT", "OPACITY", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectedtext.A = (int)strtod(currentOptionBuffer, NULL);
		// OPTION BREAKS
		GetPrivateProfileStringA("OPTION_BREAK", "RED", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optionbreaks.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_BREAK", "GREEN", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optionbreaks.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_BREAK", "BLUE", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optionbreaks.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("OPTION_BREAK", "OPACITY", "240", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		optionbreaks.A = (int)strtod(currentOptionBuffer, NULL);
		// SELECTIONHI
		GetPrivateProfileStringA("SELECTED_OPTION", "RED", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectionhi.R = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("SELECTED_OPTION", "GREEN", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectionhi.G = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("SELECTED_OPTION", "BLUE", "255", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectionhi.B = (int)strtod(currentOptionBuffer, NULL);
		GetPrivateProfileStringA("SELECTED_OPTION", "OPACITY", "110", currentOptionBuffer, 100, menuConfigIniPath.c_str());
		selectionhi.A = (int)strtod(currentOptionBuffer, NULL);

		SaveAllIniSettings();
	}

	void MenuStartSetup() {

		if (CreateDirectoryA(menuFolderPath.c_str(), NULL))
		{
			//MessageBox(NULL, L"DIRECTORY CREATED!", NULL, NULL);
			std::fstream fs(menuConfigIniPath, std::ios::out);
			SaveAllIniSettings();
		}
		else if (ERROR_ALREADY_EXISTS == GetLastError())
		{
			//MessageBox(NULL, L"DIRECTORY ALREADY EXISTS!", NULL, NULL);
			LoadAllIniSettings();

		}
		else
		{
			// Failed for some other reason :/
			MessageBox(NULL, L"ERROR: FAILED TO READ/WRITE THE MENU FOLDER IN MYDOCUMENTS", NULL, NULL);
		}
	}
}
namespace {
	// Declare subroutines here.



}
namespace {
	// Define subroutines here.

	void GenerateKey(int vk, BOOL bExtended)
	{
		KEYBDINPUT  kb = { 0 };
		INPUT    Input = { 0 };
		// generate down 
		if (bExtended)
			kb.dwFlags = KEYEVENTF_EXTENDEDKEY;
		kb.wVk = vk;
		Input.type = INPUT_KEYBOARD;

		Input.ki = kb;
		::SendInput(1, &Input, sizeof(Input));

		// generate up 
		::ZeroMemory(&kb, sizeof(KEYBDINPUT));
		::ZeroMemory(&Input, sizeof(INPUT));
		kb.dwFlags = KEYEVENTF_KEYUP;
		if (bExtended)
			kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;

		kb.wVk = vk;
		Input.type = INPUT_KEYBOARD;
		Input.ki = kb;
		::SendInput(1, &Input, sizeof(Input));
	}

	void VectorToFloat(Vector3 unk, float *Out)
	{
		Out[0] = unk.x;
		Out[1] = unk.y;
		Out[2] = unk.z;
	}
	int RandomRGB()
	{
		srand(GetTickCount());
		return (GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, 255));
	}
	bool get_key_pressed(int nVirtKey)
	{
		return (GetAsyncKeyState(nVirtKey) & 0x8000) != 0;
	}
	bool Check_self_in_vehicle()
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) return true; else return false;
	}
	int FindFreeCarSeat(DWORD vehicle)
	{
		int max = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(vehicle) - 2;
		for (static int tick = -1; tick <= max; tick++)
		{
			if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, tick))
			{
				return tick;
			}
		}

		return -1;
	}
	void offset_from_entity(int entity, float X, float Y, float Z, float * Out)
	{
		VectorToFloat(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, X, Y, Z), Out);
	}

	void RequestControlOfVehicle(Player p) {
		Ped pPed = PLAYER::GET_PLAYER_PED(p);
		if (!NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PLAYER::GET_PLAYER_PED(p))) {

		}
	}
	void RequestControlOfid(DWORD netid)
	{
		int tick = 0;

		while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && tick <= 12)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
			tick++;
		}
	}
	void RequestModel(DWORD Hash)
	{
		STREAMING::REQUEST_MODEL(Hash);
		while (!(STREAMING::HAS_MODEL_LOADED(Hash)))
		{
			WAIT(5);
		}
	}
	int PlaceObject(DWORD Hash, float X, float Y, float Z, float Pitch, float Roll, float Yaw)
	{
		RequestModel(Hash);
		static Object object = OBJECT::CREATE_OBJECT(Hash, X, Y, Z, 1, 1, 0);
		ENTITY::SET_ENTITY_ROTATION(object, Pitch, Roll, Yaw, 2, 1);
		ENTITY::FREEZE_ENTITY_POSITION(object, 1);
		ENTITY::SET_ENTITY_LOD_DIST(object, 696969);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
		ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&object);

		return object;
	}
	void setupdraw()
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.4f, 0.4f);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0f, 1.0f);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_OUTLINE();
	}
	void drawstring(char *text, float X, float Y)
	{
		UI::_SET_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::_DRAW_TEXT(X, Y);
	}
	void drawinteger(int text, float X, float Y)
	{
		UI::_SET_TEXT_ENTRY("NUMBER");
		UI::ADD_TEXT_COMPONENT_INTEGER(text);
		UI::_DRAW_TEXT(X, Y);
	}
	void drawfloat(float text, DWORD decimal_places, float X, float Y)
	{
		UI::_SET_TEXT_ENTRY("NUMBER");
		UI::ADD_TEXT_COMPONENT_FLOAT(text, decimal_places);
		UI::_DRAW_TEXT(X, Y);
	}
	void PlaySoundFrontend(char* sound_dict, char* sound_name)
	{
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name, sound_dict, 0);
	}
	void PlaySoundFrontend_default(char* sound_name)
	{
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name, "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
	}
	bool Check_compare_string_length(char* unk1, size_t max_length)
	{
		if (strlen(unk1) <= max_length) return true; else return false;
	}
	char* AddStrings(char* string1, char* string2)
	{
		memset(str, 0, sizeof(str));
		strcpy_s(str, "");
		strcpy_s(str, string1);
		strcat_s(str, string2);
		return str;
	}
	char* AddInt_S(char* string1, int int2)
	{
		static char* Return;
		printf(Return, "%i", int2);
		Return = AddStrings(string1, Return);
		return Return;
	}

	int StringToInt(char* text)
	{
		static int tempp;
		if (text == "") return NULL;
		if (GAMEPLAY::STRING_TO_INT(text, &tempp)) return NULL;

		return tempp;
	}
	void PrintStringBottomCentre(char* text)
	{
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::_DRAW_NOTIFICATION(1000, 1);
	}
	void PrintFloatBottomCentre(float text, __int8 decimal_places)
	{
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_FLOAT(text, (DWORD)decimal_places);
		UI::_DRAW_NOTIFICATION(1000, 1);
	}
	void PrintBottomLeft(char* text)
	{
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::_DRAW_NOTIFICATION(1000, 1);
	}
	void PrintError_InvalidInput()
	{
		PrintStringBottomCentre("~r~Error:~s~ Invalid Input.");
	}
	class menu
	{
	public:
		static unsigned __int16 currentsub;
		static unsigned __int16 currentop;
		static unsigned __int16 currentop_w_breaks;
		static unsigned __int16 totalop;
		static unsigned __int16 printingop;

		static unsigned __int16 currentopb;
		static unsigned __int16 printingopb;

		static unsigned __int16 breakcount;
		static unsigned __int16 totalbreaks;
		static unsigned __int8 breakscroll;
		static __int16 currentsub_ar_index;
		static int currentsub_ar[20];
		static int currentop_ar[20];
		static int SetSub_delayed;
		static unsigned long int livetimer;
		static bool bit_centre_title, bit_centre_options, bit_centre_breaks;

		static void loops();
		static void sub_handler();
		static void submenu_switch();
		static void DisableControls()
		{
			UI::HIDE_HELP_TEXT_THIS_FRAME();
			CAM::SET_CINEMATIC_BUTTON_ACTIVE(1);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_NEXT_CAMERA, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_SELECT_NEXT_WEAPON, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_CIN_CAM, controllerinput_bool);
			//CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_X);
			CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_ACCEPT);
			CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_CANCEL);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_HUD_SPECIAL, controllerinput_bool);
			CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_DOWN);
			CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_UP);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_ACCEPT, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_CANCEL, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_LEFT, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_RIGHT, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_DOWN, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_UP, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_RDOWN, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_FRONTEND_ACCEPT, controllerinput_bool);
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(10);
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(6);
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(7);
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(9);
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(8);
			CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_LEFT);
			CONTROLS::SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_RIGHT);
			/*	CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_UNARMED, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_MELEE, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_HANDGUN, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SHOTGUN, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SMG, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_AUTO_RIFLE, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SNIPER, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_HEAVY, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_WEAPON_SPECIAL, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_WEAPON_WHEEL_NEXT, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_WEAPON_WHEEL_PREV, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_WEAPON_SPECIAL_TWO, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_DIVE, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_MELEE_ATTACK_LIGHT, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_MELEE_ATTACK_HEAVY, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_MELEE_BLOCK, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_ARREST, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_HEADLIGHT, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_RADIO_WHEEL, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CONTEXT, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_RELOAD, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_DIVE, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_CIN_CAM, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_JUMP, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_SELECT_NEXT_WEAPON, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_SELECT_NEXT_WEAPON, controllerinput_bool);*/
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_FRANKLIN, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_MICHAEL, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_TREVOR, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_SELECT_CHARACTER_MULTIPLAYER, controllerinput_bool);
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_CHARACTER_WHEEL, controllerinput_bool);
		}
		static void base()
		{
			if (isMenuOpen)
			{
				background();
				optionhi();
			}
		}
		static void background()
		{
			float temp;
			if (totalop > 14) temp = 14; else temp = (float)totalop; // Calculate last option number to draw rect

																	 // Calculate Y Coord
			float bg_Y = ((temp * 0.035f) / 2.0f) + 0.159f;
			float bg_length = temp * 0.035f;

			// Draw titlebox
			GRAPHICS::DRAW_RECT(0.16f + menuPos, 0.1175f, 0.20f, 0.083f, titlebox.R, titlebox.G, titlebox.B, titlebox.A);

			// Draw background
			GRAPHICS::DRAW_RECT(0.16f + menuPos, bg_Y, 0.20f, bg_length, BG.R, BG.G, BG.B, BG.A);

			// Draw scroller indicator rect
			if (totalop > 14) temp = 14.0f; else temp = (float)totalop;
			float scr_rect_Y = ((temp + 1.0f) * 0.035f) + 0.1415f;
			GRAPHICS::DRAW_RECT(0.16f + menuPos, scr_rect_Y, 0.20f, 0.0345f, BG.R, BG.G, BG.B, BG.A);

			// Draw thin line over scroller indicator rect
			if (totalop < 14) GRAPHICS::DRAW_RECT(0.16f + menuPos, (float)(totalop)* 0.035f + 0.16f, 0.20f, 0.0022f, 255, 255, 255, 255);
			else GRAPHICS::DRAW_RECT(0.16f + menuPos, 14.0f * 0.035f + 0.16f, 0.20f, 0.0022f, 255, 255, 255, 255);

			// Draw scroller indicator
			char *dword = "CommonMenu";
			if ((totalop > 14) && GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dword))
			{
				char *dword2 = "shop_arrows_upANDdown";
				char *dword3 = "arrowright";
				Vector3 texture_res = GRAPHICS::GET_TEXTURE_RESOLUTION(dword, dword2);

				temp = ((14.0f + 1.0f) * 0.035f) + 0.1259f;

				if (currentop == 1)	GRAPHICS::DRAW_SPRITE(dword, dword3, 0.16f + menuPos, temp, texture_res.x / (float)screen_res_x, texture_res.y / (float)screen_res_y, 270.0f, titlebox.R, titlebox.G, titlebox.B, 255);
				else if (currentop == totalop) GRAPHICS::DRAW_SPRITE(dword, dword3, 0.16f + menuPos, temp, texture_res.x / (float)screen_res_x, texture_res.y / (float)screen_res_y, 90.0f, titlebox.R, titlebox.G, titlebox.B, 255);
				else GRAPHICS::DRAW_SPRITE(dword, dword2, 0.16f + menuPos, temp, texture_res.x / (float)screen_res_x, texture_res.y / (float)screen_res_y, 0.0f, titlebox.R, titlebox.G, titlebox.B, 255);

			}

			// Draw option count
			temp = scr_rect_Y - 0.0124f;
			setupdraw();
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.0f, 0.26f);
			UI::SET_TEXT_COLOUR(optioncount.R, optioncount.G, optioncount.B, optioncount.A);

			UI::_SET_TEXT_ENTRY("CM_ITEM_COUNT");
			UI::ADD_TEXT_COMPONENT_INTEGER(currentop); // ! currentop_w_breaks
			UI::ADD_TEXT_COMPONENT_INTEGER(totalop); // ! totalop - totalbreaks
			UI::_DRAW_TEXT(0.2205f + menuPos, temp);
		}

		static void background2()
		{
			float temp;
			//	if (totalop > 14) temp = 14; else temp = (float)totalop; // Calculate last option number to draw rect

			if (totalop > 14) temp = 14; else temp = (float)totalop; // Calculate last option number to draw rect
			temp = 14;

			// Calculate Y Coord
			float bg_Y = ((temp * 0.035f) / 2.0f) + 0.159f;
			float bg_length = temp * 0.035f + backgroundLengthBonus;
			if (menuPos < 0.32f) {
				// Draw titlebox
				GRAPHICS::DRAW_RECT(backXBonus + menuPos, 0.1175f + titleYBonus, bwidth, titleBoxBonus, titlebox.R, titlebox.G, titlebox.B, titlebox.A);

				// Draw background
				GRAPHICS::DRAW_RECT(backXBonus + menuPos, bg_Y + backgroundYBonus, bwidth, bg_length, BG.R, BG.G, BG.B, BG.A);
			}
			else {
				// Draw titlebox
				GRAPHICS::DRAW_RECT(-0.015 + menuPos, 0.1175f + titleYBonus, bwidth, titleBoxBonus, titlebox.R, titlebox.G, titlebox.B, titlebox.A);

				// Draw background
				GRAPHICS::DRAW_RECT(-0.015f + menuPos, bg_Y + backgroundYBonus, bwidth, bg_length, BG.R, BG.G, BG.B, BG.A);
			}

		}
		static void optionhi()
		{
			float Y_coord;
			if (currentop > 14) Y_coord = 14.0f; else Y_coord = (float)currentop;

			Y_coord = (Y_coord * 0.035f) + 0.1415f;
			GRAPHICS::DRAW_RECT(0.16f + menuPos, Y_coord, 0.20f, 0.035f, selectionhi.R, selectionhi.G, selectionhi.B, selectionhi.A);
		}

		static void while_closed()
		{
			if (KeyJustUp(openMenu_HOTKEY) || KeyJustUp(VK_INSERT) || ((CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_X) && CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_CELLPHONE_RIGHT)))) {  // JOYSTICK MENU OPEN COMBO
				isMenuOpen = !isMenuOpen;
				currentsub = latestSubBeforeClosing;
				menujustopened = true;
			}
		}



		static void while_opened()
		{
			if (totalop < currentop) {
				currentop = 1;
			}
			if (KeyJustUp(openMenu_HOTKEY) || KeyJustUp(VK_INSERT) || ((CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_X) && CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_CELLPHONE_RIGHT)))) {  // JOYSTICK MENU OPEN COMBO
				isMenuOpen = !isMenuOpen;
			}

			totalop = printingop; printingop = 0;
			totalbreaks = breakcount; breakcount = 0; breakscroll = 0;

			if (UI::IS_PAUSE_MENU_ACTIVE()) SetSub_closed();

			UI::DISPLAY_AMMO_THIS_FRAME(0);
			UI::DISPLAY_CASH(0);
			UI::SET_RADAR_ZOOM(0);

			DisableControls();

			// Scroll up
			if (IsKeyPressed(VK_NUMPAD8)) {

				if (keyUpTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					if (currentop == 1) Bottom(); else Up();
					keyUpTimer = GAMEPLAY::GET_GAME_TIMER() + scrollDelay;
				}
			}
			else if (KeyJustUp(VK_NUMPAD8))  // RELEASED
			{
				keyUpTimer = 0;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 188)) {

				if (keyUpTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					if (currentop == 1) Bottom(); else Up();
					keyUpTimer = GAMEPLAY::GET_GAME_TIMER() + scrollDelay;
				}
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(0, 188)) { // RELEASED
				keyUpTimer = 0;
			}

			// Scroll DOWN ///////////////////////////////////////// DOWN ///////////////////////
			if (IsKeyPressed(VK_NUMPAD2))
			{
				if (keyDownTimer == -1) {
					keyDownTimer = GAMEPLAY::GET_GAME_TIMER() + scrollDelay; // every second, paint the vehicle
				}
				else if (keyDownTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					if (currentop == totalop) Top(); else Down();
					keyDownTimer = GAMEPLAY::GET_GAME_TIMER() + scrollDelay;
				}

			}
			else if (KeyJustUp(VK_NUMPAD2)) { // RELEASED
				keyDownTimer = 0;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 187)) {
				if (keyDownTimer == -1) {
					keyDownTimer = GAMEPLAY::GET_GAME_TIMER() + scrollDelay; // every second, paint the vehicle
				}
				else if (keyDownTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					if (currentop == totalop) Top(); else Down();
					keyDownTimer = GAMEPLAY::GET_GAME_TIMER() + scrollDelay;
				}
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(0, 187)) { // RELEASED
				keyDownTimer = 0;
			}
			// BACK press
			if (KeyJustUp(VK_NUMPAD0))
			{
				if (currentsub == SUB::MAINMENU) SetSub_closed(); else SetSub_previous();
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, 202))
			{

				if (canBackBePressed) {
					if (currentsub == SUB::MAINMENU) SetSub_closed(); else SetSub_previous();
					canBackBePressed = false;
				}
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(0, 202)) {
				canBackBePressed = true;
			}
		}
		static void Up()
		{
			currentop--; currentop_w_breaks--;
			PlaySoundFrontend_default("NAV_UP_DOWN");
			breakscroll = 1;
		}
		static void Down()
		{
			currentop++; currentop_w_breaks++;
			PlaySoundFrontend_default("NAV_UP_DOWN");
			breakscroll = 2;
		}
		static void Bottom()
		{
			currentop = totalop; currentop_w_breaks = totalop;
			PlaySoundFrontend_default("NAV_UP_DOWN");
			breakscroll = 3;
		}
		static void Top()
		{
			currentop = 1; currentop_w_breaks = 1;
			PlaySoundFrontend_default("NAV_UP_DOWN");
			breakscroll = 4;
		}
		static void SetSub_previous()
		{
			currentsub = currentsub_ar[currentsub_ar_index]; // Get previous submenu from array and set as current submenu
			currentop = currentop_ar[currentsub_ar_index]; // Get last selected option from array and set as current selected option
			currentsub_ar_index--; // Decrement array index by 1
			printingop = 0; // Reset option print variable
			PlaySoundFrontend_default("BACK"); // Play sound
		}
		static void SetSub_new(int sub_index)
		{
			currentsub_ar_index++; // Increment array index
			currentsub_ar[currentsub_ar_index] = currentsub; // Store current submenu index in array
			currentsub = sub_index; // Set new submenu as current submenu (Static_1)

			currentop_ar[currentsub_ar_index] = currentop; // Store currently selected option in array
			currentop = 1; currentop_w_breaks = 1; // Set new selected option as first option in submenu

			printingop = 0; // Reset currently printing option var
		}
		static void SetSub_closed()
		{
			latestSubBeforeClosing = menu::currentsub;
			CONTROLS::ENABLE_ALL_CONTROL_ACTIONS(2);
			isMenuOpen = false;
			PlaySoundFrontend_default("BACK");
		}

	}; unsigned __int16 menu::currentsub = 0; unsigned __int16 menu::currentop = 0; unsigned __int16 menu::currentopb = 0; unsigned __int16 menu::currentop_w_breaks = 0; unsigned __int16 menu::totalop = 0; unsigned __int16 menu::printingop = 0; unsigned __int16 menu::printingopb = 0;  unsigned __int16 menu::breakcount = 0; unsigned __int16 menu::totalbreaks = 0; unsigned __int8 menu::breakscroll = 0; __int16 menu::currentsub_ar_index = 0; int menu::currentsub_ar[20] = {}; int menu::currentop_ar[20] = {}; int menu::SetSub_delayed = 0; unsigned long int menu::livetimer; bool menu::bit_centre_title = 1, menu::bit_centre_options = 1, menu::bit_centre_breaks = 1;
	bool KeyPressed(int key) {
		return (GetAsyncKeyState(key) & 0x8000 != 0);
	}

	bool CheckAJPressed()
	{
		if (KeyJustUp(VK_NUMPAD5))
		{
			canEnterBePressed = true;
			return true;
		}
		else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_ACCEPT))
		{
			if (canEnterBePressed) {
				canEnterBePressed = false;
				return true;
			}
			else {
				return false;
			}
		}
		else if (CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(0, INPUT_FRONTEND_ACCEPT)) {
			canEnterBePressed = true;
			return false;
		}
		else {
			return false;
		}

	}
	bool CheckRPressed()
	{
		if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 175) || KeyDown(VK_NUMPAD6)) return true; else return false;
	}
	bool CheckRJPressed()
	{
		if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 175) || KeyJustUp(VK_NUMPAD6)) return true; else return false;
	}
	bool CheckLPressed()
	{
		if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 174) || KeyDown(VK_NUMPAD4)) return true; else return false;
	}
	bool CheckLJPressed()
	{
		if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, 174) || KeyJustUp(VK_NUMPAD4)) return true; else return false;
	}
	bool IsOptionPressed()
	{
		if (CheckAJPressed())
		{
			PlaySoundFrontend_default("SELECT");
			return true;
		}
		else return false;
	}
	bool IsOptionRPressed()
	{
		if (CheckRPressed())
		{
			PlaySoundFrontend_default("NAV_LEFT_RIGHT");
			return true;
		}
		else return false;
	}
	bool IsOptionRJPressed()
	{
		if (CheckRJPressed())
		{
			PlaySoundFrontend_default("NAV_LEFT_RIGHT");
			return true;
		}
		else return false;
	}
	bool IsOptionLPressed()
	{
		if (CheckLPressed())
		{
			PlaySoundFrontend_default("NAV_LEFT_RIGHT");
			return true;
		}
		else return false;
	}
	bool IsOptionLJPressed()
	{
		if (CheckLJPressed())
		{
			PlaySoundFrontend_default("NAV_LEFT_RIGHT");
			return true;
		}
		else return false;
	}
	void AddTitle(char* text)
	{
		setupdraw();
		UI::SET_TEXT_FONT(font_title);

		UI::SET_TEXT_COLOUR(titletext.R, titletext.G, titletext.B, titletext.A);

		if (menu::bit_centre_title)
		{
			UI::SET_TEXT_CENTRE(1);
			OptionY = 0.16f + menuPos; // X coord
		}
		else OptionY = 0.066f + menuPos; // X coord

		if (Check_compare_string_length(text, 14))
		{
			UI::SET_TEXT_SCALE(0.75f, 0.75f);
			drawstring(text, OptionY, 0.1f);
		}
		else drawstring(text, OptionY, 0.13f);

	}
	void nullFunc() { return; }

	void AddOption(char* text, bool &option_code_bool = null, void(&Func)() = nullFunc, int submenu_index = -1, bool show_arrow = 0)
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				/*if (&option_code_bool != &null)*/ option_code_bool = true;
				Func();
				if (submenu_index != -1) menu::SetSub_delayed = submenu_index;
			}
		}

		if (show_arrow || submenu_index != -1) text = AddStrings(text, tempChar);
		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.16f + menuPos, OptionY);
	}
	static bool teleportInSpawnedVehicle = 0, maxSpawnedVehicle = 0, highEndBypass = 1;

	void MaxThisVehicle(Vehicle veh) {
		Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_COLOURS(veh, 120, 120);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "NGU");
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 16, 5, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 12, 2, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 11, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 14, 14, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 15, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 13, 2, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 6);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 5);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 19, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 0, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 1, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 2, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 3, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 4, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 5, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 6, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 7, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 8, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 9, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 10, 1, 0);
	}
	Vehicle SpawnVehicle(char* vehicleName)
	{
		Hash vehicleHash = GAMEPLAY::GET_HASH_KEY(vehicleName);
		auto heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		auto coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);

		float forward = 5.f;
		float xVect = forward * sin(degToRad(heading)) * -1.0f;
		float yVect = forward * cos(degToRad(heading));
		BOOL isAircraft = VEHICLE::IS_THIS_MODEL_A_HELI(vehicleHash) || VEHICLE::IS_THIS_MODEL_A_PLANE(vehicleHash);

		Vehicle vehicle = NULL;

		vehicle = VEHICLE::CREATE_VEHICLE(vehicleHash, coords.x + xVect, coords.y + yVect, coords.z, heading, TRUE, TRUE);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle);


		DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", 0);

		if (teleportInSpawnedVehicle) { PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, -1); }
		if (maxSpawnedVehicle) { MaxThisVehicle(vehicle); }

		return vehicle;
	}

	Vehicle requestVehicle(char* modelname) {
		Hash model = GAMEPLAY::GET_HASH_KEY(modelname);
		STREAMING::REQUEST_MODEL(model);
		DWORD now = GetTickCount();
		while (!STREAMING::HAS_MODEL_LOADED(model) && GetTickCount() < now + 5000)
		{
			WAIT(0);
		}

		if (!STREAMING::HAS_MODEL_LOADED(model))
		{
			PrintBottomLeft("Timed out");
		}

		Vehicle vehicle = SpawnVehicle(modelname);

		now = GetTickCount();
		while (!ENTITY::DOES_ENTITY_EXIST(vehicle) && GetTickCount() < now + 10000) {
			vehicle = SpawnVehicle(modelname);
			WAIT(1);
		}

		if (vehicle)
		{
			PrintBottomLeft("Vehicle Spawned");
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle);

		}

		return vehicle;
	}

	void AddVehicle(char* text, char* vehicleName, bool &option_code_bool = null, int submenu_index = -1, bool show_arrow = 0)
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				requestVehicle(vehicleName);
				/*if (&option_code_bool != &null)*/ option_code_bool = true;
				if (submenu_index != -1) menu::SetSub_delayed = submenu_index;
			}
		}

		if (show_arrow || submenu_index != -1) text = AddStrings(text, tempChar);
		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.16f + menuPos, OptionY);
	}
	static int selectedParticleFX;
	void AddParticleFx(char* text, char* ptfxAsset, char* ptfxName, int selectedPlayer, bool &option_code_bool = null, bool show_arrow = 0)
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				selectedParticleFX = menu::currentop;
				/*if (&option_code_bool != &null)*/ option_code_bool = true;
				STREAMING::REQUEST_NAMED_PTFX_ASSET(ptfxAsset);
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL(ptfxAsset);
				GRAPHICS::_START_PARTICLE_FX_LOOPED_ON_ENTITY_2(ptfxName, PLAYER::GET_PLAYER_PED(selectedPlayer), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
			}
		}

		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.16f + menuPos, OptionY);


		if (menu::printingop == selectedParticleFX) {
			char* selectorChar = "(X)";
			UI::SET_TEXT_FONT(4);
			UI::SET_TEXT_SCALE(0.34f, 0.34f);
			UI::SET_TEXT_CENTRE(1);
			UI::SET_TEXT_COLOUR(255, 102, 102, 250); //RED
			drawstring(tempChar, 0.233f + menuPos, OptionY);
		}

	}

	static int selectedCharOption;
	void AddCharOption(char* text, char* &option_code_bool, char* clickedParameter, int submenu_index = -1, bool show_arrow = 0)
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				selectedCharOption = menu::currentop;
				option_code_bool = clickedParameter;

				if (submenu_index != -1) menu::SetSub_delayed = submenu_index;
			}
		}

		if (show_arrow || submenu_index != -1) text = AddStrings(text, tempChar);
		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.16f + menuPos, OptionY);



		if (menu::printingop == selectedCharOption) {
			char* selectorChar = "(X)";
			UI::SET_TEXT_FONT(4);
			UI::SET_TEXT_SCALE(0.34f, 0.34f);
			UI::SET_TEXT_CENTRE(1);
			UI::SET_TEXT_COLOUR(255, 102, 102, 250); //RED
			drawstring(tempChar, 0.233f + menuPos, OptionY);
		}
	}

	void PlayAnimation(char* anim, char* animID) {
		if (ENTITY::DOES_ENTITY_EXIST(playerPed)) {
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, 1);

			STREAMING::REQUEST_ANIM_DICT(anim);
			while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
				WAIT(0);

			AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
		}
	}

	void AddAnimation(char* text, char* anim, char *animID, bool &option_code_bool = null, void(&Func)() = nullFunc, int submenu_index = -1, bool show_arrow = 0)
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				PlayAnimation(anim, animID);
				/*if (&option_code_bool != &null)*/ option_code_bool = true;
				Func();
				if (submenu_index != -1) menu::SetSub_delayed = submenu_index;
			}
		}

		if (show_arrow || submenu_index != -1) text = AddStrings(text, tempChar);
		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.16f + menuPos, OptionY);
	}
	static Object objectInEditor;

	void AddObjectDBElement(char* text, Object objectHandler, bool &option_code_bool = null, void(&Func)() = nullFunc, int submenu_index = -1, bool show_arrow = 0)
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			// MARKER
			Vector3 objectPos = ENTITY::GET_ENTITY_COORDS(objectHandler, true);
			Vector3 minObjectDimensions;
			Vector3 maxObjectDimensions;
			GAMEPLAY::GET_MODEL_DIMENSIONS(objectHandler, &minObjectDimensions, &maxObjectDimensions);

			/*char Buffer[400];
			snprintf(Buffer, sizeof(Buffer), "Xmin: %f Ymin: %f Zmin: %f Xmax: %f Ymax: %f Zmax: %f", minObjectDimensions.x, minObjectDimensions.y, minObjectDimensions.z, maxObjectDimensions.x, maxObjectDimensions.y, maxObjectDimensions.z);
			UI::_SET_TEXT_ENTRY("STRING");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(Buffer);
			UI::_DRAW_SUBTITLE_TIMED(100, 1);*/

			GRAPHICS::DRAW_MARKER(0, objectPos.x, objectPos.y, objectPos.z + maxObjectDimensions.z / 2 + 2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.75f, 1.75f, 1.75f, 204, 204, 0, 70, false, true, 2, false, false, false, false);

			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				/*if (&option_code_bool != &null)*/ option_code_bool = true;
				Func();
				objectInEditor = objectHandler;
				if (submenu_index != -1) menu::SetSub_delayed = submenu_index;
			}
		}

		if (show_arrow || submenu_index != -1) text = AddStrings(text, tempChar);
		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.16f + menuPos, OptionY);




	}

	void AddAttacker(char* attackerName, Ped victimPed, char* pedCodeName, bool isInvincible = true, char* weaponCodeName = "WEAPON_PISTOL", bool sendToEveryone = false)
	{
		null = 0;
		AddOption(attackerName);

		if (null) {
			if (sendToEveryone) {
				for (int i = 0; i <= 32; i++) {
					if (i == playerID)continue;

					Ped victimPed = PLAYER::GET_PLAYER_PED(i);
					if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
					Hash guysex = GAMEPLAY::GET_HASH_KEY(pedCodeName);
					STREAMING::REQUEST_MODEL(guysex);
					while (!STREAMING::HAS_MODEL_LOADED(guysex))
						WAIT(0);

					Vector3 victimPos = ENTITY::GET_ENTITY_COORDS(victimPed, 0);
					Ped createdAttacker = PED::CREATE_PED(26, guysex, victimPos.x, victimPos.y, victimPos.z, 1, 1, 0);

					if (isInvincible) {
						ENTITY::SET_ENTITY_INVINCIBLE(createdAttacker, true);
					}
					Hash weaponHash = GAMEPLAY::GET_HASH_KEY(weaponCodeName);
					WEAPON::GIVE_WEAPON_TO_PED(createdAttacker, weaponHash, weaponHash, 9999, 9999);
					AI::TASK_COMBAT_PED(createdAttacker, victimPed, 1, 1);
					PED::SET_PED_COMBAT_ABILITY(createdAttacker, 100);
					PED::SET_PED_CAN_SWITCH_WEAPON(createdAttacker, true);
					AI::TASK_COMBAT_PED(createdAttacker, victimPed, 1, 1);
					PED::SET_PED_AS_ENEMY(createdAttacker, 1);
					PED::SET_PED_COMBAT_RANGE(createdAttacker, 1000);
					PED::SET_PED_KEEP_TASK(createdAttacker, true);
					PED::SET_PED_AS_COP(createdAttacker, 1000);
					PED::SET_PED_ALERTNESS(createdAttacker, 1000);
				}
			}
			else {
				if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
				Hash guysex = GAMEPLAY::GET_HASH_KEY(pedCodeName);
				STREAMING::REQUEST_MODEL(guysex);
				while (!STREAMING::HAS_MODEL_LOADED(guysex))
					WAIT(0);

				Vector3 victimPos = ENTITY::GET_ENTITY_COORDS(victimPed, 0);
				Ped createdAttacker = PED::CREATE_PED(26, guysex, victimPos.x, victimPos.y, victimPos.z, 1, 1, 0);

				if (isInvincible) {
					ENTITY::SET_ENTITY_INVINCIBLE(createdAttacker, true);
				}
				Hash weaponHash = GAMEPLAY::GET_HASH_KEY(weaponCodeName);
				WEAPON::GIVE_WEAPON_TO_PED(createdAttacker, weaponHash, weaponHash, 9999, 9999);
				AI::TASK_COMBAT_PED(createdAttacker, victimPed, 1, 1);
				PED::SET_PED_COMBAT_ABILITY(createdAttacker, 100);
				PED::SET_PED_CAN_SWITCH_WEAPON(createdAttacker, true);
				AI::TASK_COMBAT_PED(createdAttacker, victimPed, 1, 1);
				PED::SET_PED_AS_ENEMY(createdAttacker, 1);
				PED::SET_PED_COMBAT_RANGE(createdAttacker, 1000);
				PED::SET_PED_KEEP_TASK(createdAttacker, true);
				PED::SET_PED_AS_COP(createdAttacker, 1000);
				PED::SET_PED_ALERTNESS(createdAttacker, 1000);
				PrintBottomLeft("~y~Attacker(s) Sent!");
			}
		}
	}




	std::list<Object> objectsDB = {};


	static char *objectInEditorName;

	Object SpawnObject(char* name, float x, float y, float z) {
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		objectInEditorName = name;
		Object obj = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY(name), x, y, z, true, 1, 0);
		ENTITY::FREEZE_ENTITY_POSITION(obj, true);
		objectInEditor = obj;
		objectsDB.push_back(obj);
		return obj;
	}

	void AddObject(char* codeName)
	{
		null = 0;
		AddOption(codeName, null);

		if (null) {
			Vector3 myPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			SpawnObject(codeName, myPosition.x, myPosition.y, myPosition.z);
			PrintBottomLeft("~g~Object Spawned");
		}
	}

	void ChangeModel(char *Model)
	{
		int modelHash = GAMEPLAY::GET_HASH_KEY(Model);
		if (STREAMING::IS_MODEL_IN_CDIMAGE(modelHash) && STREAMING::IS_MODEL_VALID(modelHash))
		{
			STREAMING::REQUEST_MODEL(modelHash);
			while (!STREAMING::HAS_MODEL_LOADED(modelHash))	WAIT(0);
			//STREAMING::LOAD_ALL_OBJECTS_NOW();
			PLAYER::SET_PLAYER_MODEL(playerID, modelHash);
			//PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), FALSE);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
			WAIT(0);
			for (int i = 0; i < 12; i++)
				for (int j = 0; j < 100; j++)
				{
					int drawable = rand() % 10;
					int texture = rand() % 10;
					if (PED::IS_PED_COMPONENT_VARIATION_VALID(playerPed, i, drawable, texture))
					{
						PED::SET_PED_COMPONENT_VARIATION(playerPed, i, drawable, texture, 0);
						break;
					}
				}
			WAIT(100);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);
		}
	}

	void AddModel(char* codeName)
	{
		null = 0;
		AddOption(codeName, null);

		if (null) {
			ChangeModel(codeName);
		}
	}
	void AddWeather(char* wheatherCodeName) {
		null = 0;
		AddOption(wheatherCodeName, null);

		if (null) {
			GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST(wheatherCodeName);
		}
	}
	void drawint(int text, float X, float Y)
	{
		UI::_SET_TEXT_ENTRY("NUMBER");
		UI::ADD_TEXT_COMPONENT_INTEGER(text);
		UI::_DRAW_TEXT(X, Y);
	}

	int NumberKeyboard() {
		GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", "", "", "", "", 5);
		while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
		if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT()) return 0;
		return atof(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
	}

	static int addIntEasyTimer = 0;

	void AddIntEasy(char* text, int value, int &val, int inc = 1, bool fast = 0, bool &toggled = null, bool enableminmax = 0, int max = 0, int min = 0)
	{
		null = 0;
		AddOption(text, null);

		if (OptionY < 0.6325 && OptionY > 0.1425)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.26f, 0.26f);
			UI::SET_TEXT_CENTRE(1);

			drawint(value, 0.233f + menuPos, OptionY);
		}

		if (menu::printingop == menu::currentop)
		{
			if (IsOptionRJPressed()) {
				if (addIntEasyTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					toggled = 1;
					if (enableminmax) {
						if (!((val + inc) > max)) {
							val += inc;
						}
					}
					else {
						val += inc;
					}
					addIntEasyTimer = GAMEPLAY::GET_GAME_TIMER() + addIntEasyDelay;
				}
			}
			else if (IsOptionRPressed()) {
				if (addIntEasyTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					toggled = 1;
					if (enableminmax) {
						if (!((val + inc) > max)) {
							val += inc;
						}
					}
					else {
						val += inc;
					}
					addIntEasyTimer = GAMEPLAY::GET_GAME_TIMER() + addIntEasyDelay;
				}
			}
			else if (IsOptionLJPressed()) {
				if (addIntEasyTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					toggled = 1;
					if (enableminmax) {
						if (!((val - inc) < min)) {
							val -= inc;
						}
					}
					else {
						val -= inc;
					}
					addIntEasyTimer = GAMEPLAY::GET_GAME_TIMER() + addIntEasyDelay;
				}
			}
			else if (IsOptionLPressed()) {
				if (addIntEasyTimer <= GAMEPLAY::GET_GAME_TIMER()) {
					toggled = 1;
					if (enableminmax) {
						if (!((val - inc) < min)) {
							val -= inc;
						}
					}
					else {
						val -= inc;
					}
					addIntEasyTimer = GAMEPLAY::GET_GAME_TIMER() + addIntEasyDelay;
				}
			}
		}
	}

	void TuneVehicle(Vehicle veh, int ModType, int ModIndex)
	{
		RequestControlOfEnt(veh);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, ModType, ModIndex, false);
	}
	void AddVehicleMod(char* text, int value, int &val,  int ModIndex, int inc = 1, bool fast = 0, bool &toggled = null, bool enableminmax = 0, int max = 0, int min = 0)
	{
		null = 0;
		AddOption(text, null);

		if (OptionY < 0.6325 && OptionY > 0.1425)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.26f, 0.26f);
			UI::SET_TEXT_CENTRE(1);

			drawint(value, 0.233f + menuPos, OptionY);
		}

		if (menu::printingop == menu::currentop)
		{
			if (IsOptionRJPressed()) {

				toggled = 1;
				if (enableminmax) {
					if (!((val + inc) > max)) {
						val += inc;
					}
				}
				else {
					val += inc;
				}
				TuneVehicle(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), val, ModIndex);
			}
			else if (IsOptionRPressed()) {
				toggled = 1;
				if (enableminmax) {
					if (!((val + inc) > max)) {
						val += inc;
					}
				}
				else {
					val += inc;
				}
				TuneVehicle(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), val, ModIndex);
			}
			else if (IsOptionLJPressed()) {
				toggled = 1;
				if (enableminmax) {
					if (!((val - inc) < min)) {
						val -= inc;
					}
				}
				else {
					val -= inc;
				}
				TuneVehicle(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), val, ModIndex);
			}
			else if (IsOptionLPressed()) {
				toggled = 1;
				if (enableminmax) {
					if (!((val - inc) < min)) {
						val -= inc;
					}
				}
				else {
					val -= inc;
				}
				TuneVehicle(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), val, ModIndex);
			}
		}
	}

	void AddCharArray(char *optionName, char *arrayName[], int &arrayCurrentIndex, int arraySize, bool &toggled = null)
	{
		null = 0;
		AddOption(optionName, null);

		if (OptionY < 0.6325 && OptionY > 0.1425)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.26f, 0.26f);
			UI::SET_TEXT_CENTRE(1);

			drawint(arrayCurrentIndex, 0.233f + menuPos, OptionY);
		}

		if (menu::printingop == menu::currentop)
		{
			if (IsOptionRJPressed()) {
				toggled = 1;
				if (arrayCurrentIndex < arraySize) {
					arrayCurrentIndex += 1;
				}
			}
			else if (IsOptionRPressed()) {
				toggled = 1;
				if (arrayCurrentIndex < arraySize) {
					arrayCurrentIndex += 1;
				}
			}
			else if (IsOptionLJPressed()) {
				toggled = 1;
				if (arraySize > 0) {
					arrayCurrentIndex -= 1;
				}
			}
			else if (IsOptionLPressed()) {
				toggled = 1;
				if (arraySize > 0) {
					arrayCurrentIndex -= 1;
				}
			}
		}
	}
	void AddSmallTitle(char* text)
	{
		setupdraw();
		UI::SET_TEXT_FONT(font_title);

		UI::SET_TEXT_COLOUR(titletext.R, titletext.G, titletext.B, titletext.A);

		if (menu::bit_centre_title)
		{
			UI::SET_TEXT_CENTRE(1);
			if (menuPos > 0.32f) OptionY = 0.16f + menuPos - 0.174f;
			else
				OptionY = 0.16f + menuPos + titleXBonus; // X coord
		}
		else {
			if (menuPos > 0.32f)
				OptionY = 0.066f + menuPos - 0.174f;
			else
				OptionY = 0.066f + menuPos + titleXBonus;
		}// X coord

		if (Check_compare_string_length(text, 14))
		{
			// IF STRING LENGHT <= 14
			UI::SET_TEXT_CENTRE(1);
			UI::SET_TEXT_SCALE(titleScale, titleScale);
			//OptionY = 0.066f + menuPos + 0.171f;
			titleTextYBonus = -0.048f;
			drawstring(text, OptionY, 0.13f + titleTextYBonus);
		}
		else {
			UI::SET_TEXT_CENTRE(1);
			UI::SET_TEXT_SCALE(titleScaleSmall, titleScaleSmall);
			drawstring(text, OptionY, 0.13f + titleTextYBonus);
		}

	}

	void AddSmallInfo(char* text, short line)
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe
		if (menu::printingopb + 1 == menu::currentopb && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe
		menu::printingopb = line;
		OptionY = 0.0f;
		OptionY = ((float)(menu::printingopb) * 0.020f) + optionYBonus;

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menuPos > 0.32f) {
			drawstring(text, 0.02f + menuPos - 0.082f, OptionY);
		}
		else {
			drawstring(text, 0.02f + menuPos + optionXBonus, OptionY);
		}
	}

	float Get3DDistance(Vector3 a, Vector3 b) {
		float x = pow((a.x - b.x), 2);
		float y = pow((a.y - b.y), 2);
		float z = pow((a.z - b.z), 2);
		return sqrt(x + y + z);
	}


	static bool useMPH = true;
	static void LoadPlayerInfo(char* playerName, Player p) {


		menu::background2();
		Ped ped = PLAYER::GET_PLAYER_PED(p);
		RequestControlOfEnt(ped);
		float health = ENTITY::GET_ENTITY_HEALTH(ped);
		float maxHealth = ENTITY::GET_ENTITY_MAX_HEALTH(ped);
		float healthPercent = health * 100 / maxHealth;
		ostringstream Health; Health << "~b~Health:~s~ " << healthPercent;
		float armor = PED::GET_PED_ARMOUR(ped);
		float maxArmor = PLAYER::GET_PLAYER_MAX_ARMOUR(p);
		float armorPercent = armor * 100 / maxArmor;
		ostringstream Armor; Armor << "~b~Armor:~s~ " << armorPercent;
		bool alive = !PED::IS_PED_DEAD_OR_DYING(ped, 1);
		char* aliveStatus;
		if (alive) aliveStatus = "Yes"; else aliveStatus = "No";
		ostringstream Alive; Alive << "~b~Alive:~s~ " << aliveStatus;
		bool inVehicle = PED::IS_PED_IN_ANY_VEHICLE(ped, 0);
		ostringstream VehicleModel; VehicleModel << "~b~Vehicle:~s~ ";
		ostringstream Speed; Speed << "~b~Speed:~s~ ";
		ostringstream IsInAVehicle; IsInAVehicle << "~b~In Vehicle:~s~ ";
		if (inVehicle) {
			IsInAVehicle << "Yes";
			Hash vehHash = ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_IN(ped, 0));
			VehicleModel << UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehHash));
			float vehSpeed = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_IN(ped, 0));
			float vehSpeedConverted;
			if (useMPH) {
				vehSpeedConverted = round(vehSpeed * 2.23693629);
				Speed << vehSpeedConverted << " MPH";
			}
			else {
				vehSpeedConverted = round(vehSpeed * 1.6);
				Speed << vehSpeedConverted << " KM/H";
			}
		}
		else {
			IsInAVehicle << "No";
			float speed = round(ENTITY::GET_ENTITY_SPEED(ped) * 100) / 100;
			Speed << speed << " M/S";
			VehicleModel << "--------";
		}
		ostringstream WantedLevel; WantedLevel << "~b~Wanted Level:~s~ " << PLAYER::GET_PLAYER_WANTED_LEVEL(p);
		ostringstream Weapon; Weapon << "~b~Weapon: ~s~";
		Hash weaponHash;
#pragma region Weapon Check
		if (WEAPON::GET_CURRENT_PED_WEAPON(ped, &weaponHash, 1)) {
			char* weaponName;
			//weaponHash = GET_SELECTED_PED_WEAPON(ped);
			if (weaponHash == 2725352035) {//Unarmed
				weaponName = "Unarmed";
			}
			else if (weaponHash == 2578778090) {//Knife
				weaponName = "Knife";
			}
			else if (weaponHash == 0x678B81B1) {//Nightstick
				weaponName = "Nightstick";
			}
			else if (weaponHash == 0x4E875F73) {//Hammer
				weaponName = "Hammer";
			}
			else if (weaponHash == 0x958A4A8F) {//Bat
				weaponName = "Bat";
			}
			else if (weaponHash == 0x440E4788) {//GolfClub
				weaponName = "GolfClub";
			}
			else if (weaponHash == 0x84BD7BFD) {//Crowbar
				weaponName = "Crowbar";
			}
			else if (weaponHash == 0x1B06D571) {//Pistol
				weaponName = "Pistol";
			}
			else if (weaponHash == 0x5EF9FEC4) {//Combat Pistol
				weaponName = "Combat Pistol";
			}
			else if (weaponHash == 0x22D8FE39) {//AP Pistol
				weaponName = "AP Pistol";
			}
			else if (weaponHash == 0x99AEEB3B) {//Pistol 50
				weaponName = "Pistol 50";
			}
			else if (weaponHash == 0x13532244) {//Micro SMG
				weaponName = "Micro SMG";
			}
			else if (weaponHash == 0x2BE6766B) {//SMG
				weaponName = "SMG";
			}
			else if (weaponHash == 0xEFE7E2DF) {//Assault SMG
				weaponName = "Assault SMG";
			}
			else if (weaponHash == 0xBFEFFF6D) {//Assault Riffle
				weaponName = "Assault Riffle";
			}
			else if (weaponHash == 0x83BF0278) {//Carbine Riffle
				weaponName = "Carbine Riffle";
			}
			else if (weaponHash == 0xAF113F99) {//Advanced Riffle
				weaponName = "Advanced Riffle";
			}
			else if (weaponHash == 0x9D07F764) {//MG
				weaponName = "MG";
			}
			else if (weaponHash == 0x7FD62962) {//Combat MG
				weaponName = "Combat MG";
			}
			else if (weaponHash == 0x1D073A89) {//Pump Shotgun
				weaponName = "Pump Shotgun";
			}
			else if (weaponHash == 0x7846A318) {//Sawed-Off Shotgun
				weaponName = "Sawed-Off Shotgun";
			}
			else if (weaponHash == 0xE284C527) {//Assault Shotgun
				weaponName = "Assault Shotgun";
			}
			else if (weaponHash == 0x9D61E50F) {//Bullpup Shotgun
				weaponName = "Bullpup Shotgun";
			}
			else if (weaponHash == 0x3656C8C1) {//Stun Gun
				weaponName = "Stun Gun";
			}
			else if (weaponHash == 0x05FC3C11) {//Sniper Rifle
				weaponName = "Sniper Rifle";
			}
			else if (weaponHash == 0x0C472FE2) {//Heavy Sniper
				weaponName = "Heavy Sniper";
			}
			else if (weaponHash == 0xA284510B) {//Grenade Launcher
				weaponName = "Grenade Launcher";
			}
			else if (weaponHash == 0x4DD2DC56) {//Smoke Grenade Launcher
				weaponName = "Smoke Grenade Launcher";
			}
			else if (weaponHash == 0xB1CA77B1) {//RPG
				weaponName = "RPG";
			}
			else if (weaponHash == 0x42BF8A85) {//Minigun
				weaponName = "Minigun";
			}
			else if (weaponHash == 0x93E220BD) {//Grenade
				weaponName = "Grenade";
			}
			else if (weaponHash == 0x2C3731D9) {//Sticky Bomb
				weaponName = "Sticky Bomb";
			}
			else if (weaponHash == 0xFDBC8A50) {//Smoke Grenade
				weaponName = "Smoke Grenade";
			}
			else if (weaponHash == 0xA0973D5E) {//BZGas
				weaponName = "BZGas";
			}
			else if (weaponHash == 0x24B17070) {//Molotov
				weaponName = "Molotov";
			}
			else if (weaponHash == 0x060EC506) {//Fire Extinguisher
				weaponName = "Fire Extinguisher";
			}
			else if (weaponHash == 0x34A67B97) {//Petrol Can
				weaponName = "Petrol Can";
			}
			else if (weaponHash == 0xFDBADCED) {//Digital scanner
				weaponName = "Digital scanner";
			}
			else if (weaponHash == 0x88C78EB7) {//Briefcase
				weaponName = "Briefcase";
			}
			else if (weaponHash == 0x23C9F95C) {//Ball
				weaponName = "Ball";
			}
			else if (weaponHash == 0x497FACC3) {//Flare
				weaponName = "Flare";
			}
			else if (weaponHash == 0xF9E6AA4B) {//Bottle
				weaponName = "Bottle";
			}
			else if (weaponHash == 0x61012683) {//Gusenberg
				weaponName = "Gusenberg";
			}
			else if (weaponHash == 0xC0A3098D) {//Special Carabine
				weaponName = "Special Carabine";
			}
			else if (weaponHash == 0xD205520E) {//Heavy Pistol
				weaponName = "Heavy Pistol";
			}
			else if (weaponHash == 0xBFD21232) {//SNS Pistol
				weaponName = "SNS Pistol";
			}
			else if (weaponHash == 0x7F229F94) {//Bullpup Rifle
				weaponName = "Bullpup Rifle";
			}
			else if (weaponHash == 0x92A27487) {//Dagger
				weaponName = "Dagger";
			}
			else if (weaponHash == 0x083839C4) {//Vintage Pistol
				weaponName = "Vintage Pistol";
			}
			else if (weaponHash == 0x7F7497E5) {//Firework
				weaponName = "Firework";
			}
			else if (weaponHash == 0xA89CB99E) {//Musket
				weaponName = "Musket";
			}
			else if (weaponHash == 0x3AABBBAA) {//Heavy Shotgun
				weaponName = "Heavy Shotgun";
			}
			else if (weaponHash == 0xC734385A) {//Marksman Rifle
				weaponName = "Marksman Rifle";
			}
			else if (weaponHash == 0x63AB0442) {//Homing Launcher
				weaponName = "Homing Launcher";
			}
			else if (weaponHash == 0xAB564B93) {//Proxmine
				weaponName = "Proximity Mine";
			}
			else if (weaponHash == 0x787F0BB) {//Snowball
				weaponName = "Snowball";
			}
			else if (weaponHash == 0x47757124) {//Flare Gun
				weaponName = "Flare Gun";
			}
			else if (weaponHash == 0xE232C28C) {//Garbage Bag
				weaponName = "Garbage Bag";
			}
			else if (weaponHash == 0xD04C944D) {//Handcuffs
				weaponName = "Handcuffs";
			}
			else if (weaponHash == 0x0A3D4D34) {//Combat PDW
				weaponName = "Combat PDW";
			}
			else if (weaponHash == 0xDC4DB296) {//Marksman Pistol
				weaponName = "Marksman Pistol";
			}
			else if (weaponHash == 0xD8DF3C3C) {//Brass Knuckles
				weaponName = "Brass Knuckles";
			}
			else if (weaponHash == 0x6D544C99) {//Brass Knuckles
				weaponName = "Railgun";
			}
			else {
				weaponName = "Unarmed";
			}
			Weapon << weaponName;
		}
		else Weapon << "Unarmed";
#pragma endregion
		Vector3 myCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, 1);
		ostringstream Zone; Zone << "~b~Zone: ~s~" << UI::_GET_LABEL_TEXT(ZONE::GET_NAME_OF_ZONE(coords.x, coords.y, coords.z));
		Hash streetName, crossingRoad;
		PATHFIND::GET_STREET_NAME_AT_COORD(coords.x, coords.y, coords.z, &streetName, &crossingRoad);
		ostringstream Street; Street << "~b~Street: ~s~" << UI::GET_STREET_NAME_FROM_HASH_KEY(streetName);
		float distance = Get3DDistance(coords, myCoords);
		ostringstream Distance; Distance << "~b~Distance: ~s~";
		if (useMPH) {
			if (distance > 1609.344) {
				distance = round((distance / 1609.344) * 100) / 100;
				Distance << distance << " Miles";
			}
			else {
				distance = round((distance * 3.2808399) * 100) / 100;
				Distance << distance << " Feets";
			}
		}
		else {
			if (distance > 1000) {
				distance = round((distance / 1000) * 100) / 100;
				Distance << distance << " Kilometers";
			}
			else {
				distance = round(distance * 1000) / 100;
				Distance << distance << " Meters";
			}
		}

		AddSmallTitle(playerName);
		AddSmallInfo((char*)Health.str().c_str(), 0);
		AddSmallInfo((char*)Armor.str().c_str(), 1);
		AddSmallInfo((char*)Alive.str().c_str(), 2);
		AddSmallInfo((char*)IsInAVehicle.str().c_str(), 3);
		AddSmallInfo((char*)VehicleModel.str().c_str(), 4);
		AddSmallInfo((char*)Speed.str().c_str(), 5);
		AddSmallInfo((char*)WantedLevel.str().c_str(), 6);
		AddSmallInfo((char*)Weapon.str().c_str(), 7);
		AddSmallInfo((char*)Zone.str().c_str(), 8);
		AddSmallInfo((char*)Street.str().c_str(), 9);
		AddSmallInfo((char*)Distance.str().c_str(), 10);

	}
	void AddPlayer(char* text, Player player, bool &option_code_bool = null, void(&Func)() = nullFunc, int submenu_index = -1)
	{
		AddOption(text, option_code_bool, Func, submenu_index);
		if (menu::printingop == menu::currentop)
		{
			LoadPlayerInfo(PLAYER::GET_PLAYER_NAME(player), player);
		}

	}

	void AddBoxType(char* text, bool &Box3D = box3D) {
		null = 0;
		AddOption(text, null);
		if (OptionY < 0.6325 && OptionY > 0.1425)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.26f, 0.26f);
			UI::SET_TEXT_CENTRE(1);
			if (Box3D)
				drawstring("~g~2D", 0.233f + menuPos, OptionY);
			else
				drawstring("~g~3D", 0.233f + menuPos, OptionY);
			if (menu::printingop == menu::currentop)
			{
				if (IsOptionRJPressed()) {//num6
					Box3D = !Box3D;
				}
				if (IsOptionLJPressed()) {//num4
					Box3D = !Box3D;
				}
			}
		}
	}


	void AddNumberEasy(char* text, float value, __int8 decimal_places, float &val, float inc = 1.0f, bool fast = 0, bool &toggled = null, bool enableminmax = 0, float max = 0.0f, float min = 0.0f)
	{
		null = 0;
		AddOption(text, null);

		if (OptionY < 0.6325 && OptionY > 0.1425)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.26f, 0.26f);
			UI::SET_TEXT_CENTRE(1);

			drawfloat(value, (DWORD)decimal_places, 0.233f + menuPos, OptionY);
		}

		if (menu::printingop == menu::currentop)
		{
			if (IsOptionRJPressed()) {
				toggled = 1;
				if (enableminmax) {
					if (!((val + inc) > max)) {
						val += inc;
					}
				}
				else {
					val += inc;
				}
			}
			else if (IsOptionRPressed()) {
				toggled = 1;
				if (enableminmax) {
					if (!((val + inc) > max)) {
						val += inc;
					}
				}
				else {
					val += inc;
				}
			}
			else if (IsOptionLJPressed()) {
				toggled = 1;
				if (enableminmax) {
					if (!((val - inc) < min)) {
						val -= inc;
					}
				}
				else {
					val -= inc;
				}
			}
			else if (IsOptionLPressed()) {
				toggled = 1;
				if (enableminmax) {
					if (!((val - inc) < min)) {
						val -= inc;
					}
				}
				else {
					val -= inc;
				}
			}
		}
	}

	void AddFloat(char* text, double value, __int8 decimal_places, float &val, double inc = 1.0, bool &toggled = null)
	{
		null = 0;
		AddOption(text, null);

		if (OptionY < 0.6325 && OptionY > 0.1425)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.26f, 0.26f);
			UI::SET_TEXT_CENTRE(1);
			char* buff = "~b~<~s~ ";
			for (int i = 0; i <= decimal_places; i++) AddStrings(buff, " ");
			AddStrings(buff, " ~b~>");
			drawstring(buff, 0.233f + menuPos, OptionY);
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.26f, 0.26f);
			UI::SET_TEXT_CENTRE(1);
			drawfloat(value, decimal_places, 0.233f + menuPos, OptionY);
		}

		if (menu::printingop == menu::currentop)

		{
			if (null) {
				val = NumberKeyboard();
				toggled = 1;
			}
			else if (IsOptionRJPressed()) {
				toggled = 1;
				val = val + (double)inc;
			}
			else if (IsOptionRPressed()) {
				toggled = 1;
				val = val + (double)inc;
			}
			else if (IsOptionLJPressed()) {
				toggled = 1;
				val -= (double)inc;
			}
			else if (IsOptionLPressed()) {
				toggled = 1;
				val -= (double)inc;
			}
		}
	}

	char *vehicleCategoryName;
	char *vehicleCategory[];

	void AddVehicleCategory(char* text, bool &option_code_bool = null, void(&Func)() = nullFunc, int submenu_index = -1, char *category = "Super")
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				if (&option_code_bool != &null) option_code_bool = true;
				if (submenu_index != -1) {
					vehicleCategoryName = category;
					menu::SetSub_delayed = submenu_index;
				}
				Func();
			}
		}

		if (submenu_index != -1) text = AddStrings(text, tempChar);
		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.066f + menuPos, OptionY);

	}
	static char *modelCategoryName;
	void AddModelCategory(char* text, bool &option_code_bool = null, void(&Func)() = nullFunc, int submenu_index = -1, char* category = "Animals")
	{
		char* tempChar;
		if (font_options == 2 || font_options == 7) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		if (menu::printingop + 1 == menu::currentop && (font_selection == 2 || font_selection == 7)) tempChar = "  ------"; // Font unsafe
		else tempChar = "  ~b~>"; // Font safe

		menu::printingop++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_options);
		UI::SET_TEXT_COLOUR(optiontext.R, optiontext.G, optiontext.B, optiontext.A);
		if (menu::printingop == menu::currentop)
		{
			UI::SET_TEXT_FONT(font_selection);
			UI::SET_TEXT_COLOUR(selectedtext.R, selectedtext.G, selectedtext.B, selectedtext.A);
			if (IsOptionPressed())
			{
				if (&option_code_bool != &null) option_code_bool = true;

				if (submenu_index != -1) {
					modelCategoryName = category;
					menu::SetSub_delayed = submenu_index;
				}
				Func();
			}
		}

		if (submenu_index != -1) text = AddStrings(text, tempChar);
		if (menu::bit_centre_options)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else drawstring(text, 0.066f + menuPos, OptionY);

	}
	void OptionStatus(int status)
	{
		if (OptionY < 0.6325f && OptionY > 0.1425f)
		{
			char* tempChar;
			UI::SET_TEXT_FONT(4);
			UI::SET_TEXT_SCALE(0.34f, 0.34f);
			UI::SET_TEXT_CENTRE(1);

			if (status == 0) {
				UI::SET_TEXT_COLOUR(255, 102, 102, 250); //RED
				tempChar = "OFF";
			}
			else if (status == 1) {
				UI::SET_TEXT_COLOUR(102, 255, 102, 250); //GREEN
				tempChar = "ON";
			}
			else {
				UI::SET_TEXT_COLOUR(255, 255, 102, 250); //YELLOW
				tempChar = "??";
			}

			drawstring(tempChar, 0.233f + menuPos, OptionY);

		}
	}
	void AddLightToggle(char* text, bool &loop_variable, void(&Func)(bool state), bool &extra_option_code_ON = null, bool &extra_option_code_OFF = null)
	{
		null = 0;
		AddOption(text, null);

		if (null) {
			loop_variable = !loop_variable;
			Func(loop_variable);
			if (loop_variable != 0) extra_option_code_ON = true;
			else extra_option_code_OFF = true;
		}

		if (!loop_variable) OptionStatus(0); // Display OFF
		else if (loop_variable) OptionStatus(1); // Display ON
	}

	void AddToggle(char* text, bool &loop_variable, bool &extra_option_code_ON = null, bool &extra_option_code_OFF = null)
	{
		null = 0;
		AddOption(text, null);

		if (null) {
			loop_variable = !loop_variable;
			if (loop_variable != 0) extra_option_code_ON = true;
			else extra_option_code_OFF = true;
		}

		if (!loop_variable) OptionStatus(0); // Display OFF
		else if (loop_variable) OptionStatus(1); // Display ON
	}
	static char *latestScreenEffect;

	void DisableLatestScreenFx() {
		GRAPHICS::_STOP_SCREEN_EFFECT(latestScreenEffect);
	}
	void AddScreenFx(char *effectName, char *effectCode) {
		null = 0;
		AddOption(effectName, null);

		if (null) {
			GRAPHICS::_STOP_SCREEN_EFFECT(latestScreenEffect);
			latestScreenEffect = effectCode;
			GRAPHICS::_START_SCREEN_EFFECT(effectCode, 2147000000, 0);
		}
	}

	void AddIntToggle(char* text, int &loop_variable, bool &extra_option_code_ON = null, bool &extra_option_code_OFF = null)
	{
		null = 0;
		AddOption(text, null);

		if (null) {
			if (loop_variable == 0) {
				loop_variable = 1;
			}
			else {
				loop_variable = 0;
			}
			if (loop_variable != 0) extra_option_code_ON = true;
			else extra_option_code_OFF = true;
		}

		if (loop_variable == 0) OptionStatus(0); // Display OFF
		else if (loop_variable == 1) OptionStatus(1); // Display ON
	}

	static int spectatedPlayerID;
	static bool isSpectating = false;
	void SpectatePlayer() {
		if (isSpectating) {
			PrintBottomLeft("SPECTATING");
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(1, PLAYER::GET_PLAYER_PED(spectatedPlayerID));
		}
		else {
			PrintBottomLeft("SPECTATING STOPPED");
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(0, PLAYER::GET_PLAYER_PED(spectatedPlayerID));
		}
	}


	void AddSpectate(char* text, bool &loop_variable, int spectatedID)
	{
		null = 0;
		AddOption(text, null);

		if (null) {
			loop_variable = !loop_variable;
			if (loop_variable) {
				spectatedPlayerID = spectatedID;
				isSpectating = true;
				SpectatePlayer();
			}
			else {
				isSpectating = false;
				SpectatePlayer();
			}
		}

		if (!loop_variable) OptionStatus(0); // Display OFF
		else if (loop_variable) OptionStatus(1); // Display ON
	}

	void AddLocal(char* text, Void condition, bool &option_code_ON, bool &option_code_OFF)
	{
		AddOption(text);
		if (menu::printingop == menu::currentop) {
			if (IsOptionPressed()) {
				if (condition == 0 && &option_code_ON != &null) option_code_ON = true;
				else if (condition != 0 && &option_code_OFF != &null) option_code_OFF = true;
			}
		}

		if (!condition) OptionStatus(0); // Display OFF
		else if (condition) OptionStatus(1); // Display ON
	}
	void AddBreak(char* text)
	{
		menu::printingop++; menu::breakcount++;

		OptionY = 0.0f;
		if (menu::currentop <= 14)
		{
			if (menu::printingop > 14) return;
			else OptionY = ((float)(menu::printingop) * 0.035f) + 0.125f;
		}
		else
		{
			if (menu::printingop < (menu::currentop - 13) || menu::printingop > menu::currentop) return;
			else OptionY = ((float)(menu::printingop - (menu::currentop - 14))* 0.035f) + 0.125f;
		}

		setupdraw();
		UI::SET_TEXT_FONT(font_breaks);
		UI::SET_TEXT_COLOUR(optionbreaks.R, optionbreaks.G, optionbreaks.B, optionbreaks.A);
		if (menu::printingop == menu::currentop)
		{
			switch (menu::breakscroll)
			{
			case 1:
				menu::currentop_w_breaks = menu::currentop_w_breaks + 1;
				menu::currentop--; break;
			case 2:
				menu::currentop_w_breaks = menu::currentop - menu::breakcount;
				menu::currentop++; break;
			case 3:
				menu::currentop_w_breaks = menu::totalop - (menu::totalbreaks - 1);
				menu::currentop--; break;
			case 4:
				menu::currentop_w_breaks = 1;
				menu::currentop++; break;
			}

		}
		if (menu::bit_centre_breaks)
		{
			UI::SET_TEXT_CENTRE(1);
			drawstring(text, 0.16f + menuPos, OptionY);
		}
		else
		{
			drawstring(text, 0.066f + menuPos, OptionY);
		}

	}
	void AddNumber(char* text, long value, __int8 decimal_places, bool &A_PRESS = null, bool &RIGHT_PRESS = null, bool &LEFT_PRESS = null)
	{
		AddOption(text, null);

		if (OptionY < 0.6325 && OptionY > 0.1425)
		{
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.275f, 0.275f);
			UI::SET_TEXT_CENTRE(1);

			drawfloat(value, (DWORD)decimal_places, 0.233f + menuPos, OptionY);
		}

		if (menu::printingop == menu::currentop)
		{
			if (IsOptionPressed() && &A_PRESS != &null) *&A_PRESS = true;
			else if (IsOptionRJPressed() && &RIGHT_PRESS != &null) RIGHT_PRESS = true;
			else if (IsOptionRPressed() && &RIGHT_PRESS != &null) RIGHT_PRESS = true;
			else if (IsOptionLJPressed() && &LEFT_PRESS != &null) LEFT_PRESS = true;
			else if (IsOptionLPressed() && &LEFT_PRESS != &null) LEFT_PRESS = true;

		}

	}

	void Teleport(float X, float Y, float Z) {
		if (!Check_self_in_vehicle())
		{
			ENTITY::SET_ENTITY_COORDS(playerPed, X, Y, Z, 0, 0, 0, 1);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY("scr_clown_appears", playerPed, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
		}
		else
		{
			Vehicle myCurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
			RequestControlOfEnt(myCurrentVehicle);
			ENTITY::SET_ENTITY_COORDS(myCurrentVehicle, X, Y, Z, 0, 0, 0, 1);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY("scr_clown_appears", playerPed, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
		}
		STREAMING::LOAD_ALL_OBJECTS_NOW();
	}

	void AddTele(char* text, float X, float Y, float Z, bool &extra_option_code = null)
	{
		null = 0;
		AddOption(text, null);

		if (null) {
			Teleport(X, Y, Z);
		}

	}


	//////////////////////////////////////////////////////////////////// Hax functions:   ////////////////////////////////////////////////////////////////////////////////////////////////////

	// NETWORK HANDLE
	int GetNetworkHandle(Player player)
	{
		int handle[76]; //var num3 = sub_34009(A_0, (A_1) + 264, (A_1) + 272);
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
		if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
		{
			return handle[0];
		}
		return NULL;
	}

	//GOD MODE
	void GodMode()
	{
		static int armour_player = 0;
		if (armour_player == 0)
		{
			armour_player = PED::GET_PED_ARMOUR(playerPed);
		}

		if (godMode) {
			//set_status_text("Activating godmode");
			PLAYER::SET_PLAYER_INVINCIBLE(playerID, true);
			ENTITY::SET_ENTITY_PROOFS(playerPed, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
			PED::SET_PED_CAN_RAGDOLL(playerPed, FALSE);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, FALSE);
			PED::ADD_ARMOUR_TO_PED(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(playerID) - PED::GET_PED_ARMOUR(playerPed));
		}
		else
		{
			//set_status_text("Deactivating godmode");
			PLAYER::SET_PLAYER_INVINCIBLE(playerID, false);
			ENTITY::SET_ENTITY_PROOFS(playerPed, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
			PED::SET_PED_CAN_RAGDOLL(playerPed, TRUE);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, TRUE);
			if (armour_player != 0)
			{
				PED::SET_PED_ARMOUR(playerPed, armour_player);
				armour_player = 0;
			}
		}
	}

	// NEVER WANTED
	void NeverWanted() {
		if (neverWanted) {
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(playerID);
			PLAYER::SET_MAX_WANTED_LEVEL(0);
		}
		else {
			PLAYER::SET_MAX_WANTED_LEVEL(5);
		}
	}

	// SUPER JUMP
	void SuperJump() {
		if (superJump) {
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(playerID);
		}
	}

	// PLAYER GHOST MODE
	void GhostMode() {
		if (ghostMode) {
			ENTITY::SET_ENTITY_VISIBLE(playerPed, false, 0);
		}
		else {
			ENTITY::SET_ENTITY_VISIBLE(playerPed, true, 0);
		}
	}

	void SuperRun() {
		if (superRun && IsKeyPressed(VK_LSHIFT)) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) == false) {
				Ped ped = playerPed;
				Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0, 0.6, 0);
				ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0.0f, 1.3, 0, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
				PLAYER::SET_PLAYER_SPRINT(playerID, 1);
				PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(playerID, 1.59);
			}
		}
	}
	void NoRagdoll() {
		PED::SET_PED_CAN_RAGDOLL(playerPed, FALSE);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, FALSE);
	}

	void VehicleGodMode() {
		if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_USING(playerPed), -1) == playerPed) {
			if (vehicleGodMode) {
				Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
				ENTITY::SET_ENTITY_INVINCIBLE(currentVehicle, true);
			}
			else {
				Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
				ENTITY::SET_ENTITY_INVINCIBLE(currentVehicle, false);
			}
		}
	}

	void InvisibleVehicle() {
		if (invisibleVehicle) {
			ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(playerPed), false, 0);
		}
		else {
			ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(playerPed), true, 0);
		}
	}

	void MaxVehicle() {
		Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
		VEHICLE::SET_VEHICLE_MOD_KIT(currentVehicle, 0);
		VEHICLE::SET_VEHICLE_COLOURS(currentVehicle, 120, 120);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(currentVehicle, "NGU");
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(currentVehicle, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(currentVehicle, 18, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(currentVehicle, 22, 1);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 16, 5, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 12, 2, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 11, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 14, 14, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 15, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 13, 2, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(currentVehicle, 6);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(currentVehicle, 5);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 23, 19, 1);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 0, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 1, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 2, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 3, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 4, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 5, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 6, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 7, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 8, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 9, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(currentVehicle, 10, 1, 0);
	}

	
	void FixVehicle() {
		Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
		VEHICLE::SET_VEHICLE_FIXED(currentVehicle);
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(currentVehicle);
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(currentVehicle, 0);
		PrintBottomLeft("~g~Vehicle Fixed!");
	}
	void AutoFixVehicle() {
		if (autoFixVehicle) {
			Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
			VEHICLE::SET_VEHICLE_FIXED(currentVehicle);
			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(currentVehicle);
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(currentVehicle, 0);
		}
	}
	void NoBikeFall() {
		if (noBikeFall) {
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPed, 1);
		}
		else {
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPed, 0);
		}
	}

	void SuperCarMode() {
		if (superCarMode) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
				if (IsKeyPressed(VK_NUMPAD9)) {
					ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_USING(playerPed), 1, 0.0f, 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
				}
				if (IsKeyPressed(VK_NUMPAD3)) {
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0.0f);
				}
			}
		}
	}

	void CarWeaponLoop(char* bulletsType)
	{
		if (vehicleWeapon) {
			if (IsKeyPressed(VK_SUBTRACT)) {
				int VehicleHandle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 0);
				int launchDistance = 250;
				Hash weaponHash = GAMEPLAY::GET_HASH_KEY(bulletsType);
				float launchSpeed = 1500.0f;
				float idkcoords1[] = { 0.6f, 0.6707f, 0.3711f };
				Vector3 getcoords1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehicleHandle, idkcoords1[0], idkcoords1[1], idkcoords1[2]);
				float idkcoords2[] = { -0.6f, 0.6707f, 0.3711f };
				Vector3 getcoords2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehicleHandle, idkcoords2[0], idkcoords2[1], idkcoords2[2]);
				float idkcoords3[] = { 0.6f, 5.0707f, 0.3711f };
				Vector3 getcoords3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehicleHandle, idkcoords3[0], idkcoords3[1], idkcoords3[2]);
				float idkcoords4[] = { -0.6f, 5.0707f, 0.3711f };
				Vector3 getcoords4 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehicleHandle, idkcoords4[0], idkcoords4[1], idkcoords4[2]);
				float coords1[] = { getcoords1.x, getcoords1.y, getcoords1.z };
				float coords2[] = { getcoords2.x, getcoords2.y, getcoords2.z };
				float coords3[] = { getcoords3.x, getcoords3.y, getcoords3.z };
				float coords4[] = { getcoords4.x, getcoords4.y, getcoords4.z };
				GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1[0], coords1[1], coords1[2], coords3[0], coords3[1], coords3[2], launchDistance, 0, weaponHash, PLAYER::PLAYER_PED_ID(), 1, 1, launchSpeed);
				GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords2[0], coords2[1], coords2[2], coords4[0], coords4[1], coords4[2], launchDistance, 0, weaponHash, PLAYER::PLAYER_PED_ID(), 1, 1, launchSpeed);
			}
		}
	}

	// AIM LASER
	void DrawAimingLasers()
	{
		if (vehicleLasers) {
			int VehHandle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 0);
			float Offset1[] = { 0.6f, 0.6707f, 0.3711f };
			float Offset2[] = { -0.6f, 0.6707f, 0.3711f };
			float Offset3[] = { 0.6f, 25.0707f, 0.3711f };
			float Offset4[] = { -0.6f, 25.0707f, 0.3711f };
			Vector3 getcoords1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, Offset1[0], Offset1[1], Offset1[2]);
			Vector3 getcoords2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, Offset2[0], Offset2[1], Offset2[2]);
			Vector3 getcoords3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, Offset3[0], Offset3[1], Offset3[2]);
			Vector3 getcoords4 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, Offset4[0], Offset4[1], Offset4[2]);
			float StartCoords1[] = { getcoords1.x, getcoords1.y, getcoords1.z };
			float StartCoords2[] = { getcoords2.x, getcoords2.y, getcoords2.z };
			float EndCoords1[] = { getcoords3.x, getcoords3.y, getcoords3.z };
			float EndCoords2[] = { getcoords4.x, getcoords4.y, getcoords4.z };
			GRAPHICS::DRAW_LINE(StartCoords1[0], StartCoords1[1], StartCoords1[2], EndCoords1[0], EndCoords1[1], EndCoords1[2], 255, 0, 0, 255);
			GRAPHICS::DRAW_LINE(StartCoords2[0], StartCoords2[1], StartCoords2[2], EndCoords2[0], EndCoords2[1], EndCoords2[2], 255, 0, 0, 255);
		}
	}

	void VehicleCollisions()
	{
		if (vehicleCollisions) {
			ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(playerPed, 1), true, true);
		}
		else {
			ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(playerPed, 1), false, true);
		}
	}
	void FlyMode() {
		if (flyMode && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
			Vector3 cameraRotation = CAM::GET_GAMEPLAY_CAM_ROT(0);
			Vehicle myCurrentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
			ENTITY::SET_ENTITY_ROTATION(myCurrentVehicle, cameraRotation.x, cameraRotation.y, cameraRotation.z, 2, true);

			if (get_key_pressed(VK_SPACE)) {
				ENTITY::APPLY_FORCE_TO_ENTITY(myCurrentVehicle, 1, 0.0f, 0.0f, 250.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			}
			if (get_key_pressed(VK_KEY_W)) {
				ENTITY::APPLY_FORCE_TO_ENTITY(myCurrentVehicle, 1, 0.0f, 300.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			}
			if (get_key_pressed(VK_LSHIFT)) {
				ENTITY::APPLY_FORCE_TO_ENTITY(myCurrentVehicle, 1, 0.0f, 0.0f, -300.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			}
			if (get_key_pressed(VK_KEY_A)) {
				ENTITY::APPLY_FORCE_TO_ENTITY(myCurrentVehicle, 1, -100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			}
			if (get_key_pressed(VK_KEY_D)) {
				ENTITY::APPLY_FORCE_TO_ENTITY(myCurrentVehicle, 1, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			}
			if (get_key_pressed(VK_KEY_S)) {
				ENTITY::SET_ENTITY_VELOCITY(myCurrentVehicle, 0, 0, 0);
			}
		}
	}
	static LPCSTR weaponNames[] = {
		"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
		"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
		"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
		"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
		"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_COMBATPDW", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
		"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
		"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN", "WEAPON_KNUCKLE", "WEAPON_MARKSMANPISTOL",
		"WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER",
		"WEAPON_PROXMINE", "WEAPON_SNOWBALL", "WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET",
		"WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN", "WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN", "WEAPON_FLASHLIGHT", "WEAPON_MACHINEPISTOL", "WEAPON_MACHETE", "GADGET_PARACHUTE"
	};

	void GetAllWeapons() {
		for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 9999, 9999);
		}
	}
	// EXPLOSIVE BULLETS
	void ExplosiveBullets() {
		if (explosiveBullets) {
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(playerID);
		}
	}

	void ExplosiveMelee() {
		if (explosiveMelee && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1)) {
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(playerID);
		}
	}

	void HeatVisionOnAim() {
		if (heatVisionOnAim) {
			if (PLAYER::IS_PLAYER_FREE_AIMING(playerID)) {
				GRAPHICS::SET_SEETHROUGH(true);
			}
			else if (!PLAYER::IS_PLAYER_FREE_AIMING(playerID) && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1))
			{
				GRAPHICS::SET_SEETHROUGH(false);
			}
		}
	}
	// ONE SHOT KILL
	void OneShotKill() {
		if (oneShotKill) {
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(playerID, 9999);
		}
	}

	void AimBot() {
		if (aimBot) {
			if (PLAYER::IS_PLAYER_FREE_AIMING(playerID)) {
				Entity aimedObject;
				PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(playerID, &aimedObject);
				if (ENTITY::IS_ENTITY_A_PED(aimedObject)) {
					Vector3 headCoords = PED::GET_PED_BONE_COORDS(aimedObject, 31086, 0, 0, 0);
					PED::SET_PED_SHOOTS_AT_COORD(playerPed, headCoords.x, headCoords.y, headCoords.z, true);
				}
			}
		}
	}

	void InfiniteAmmo() {
		if (infiniteAmmo) {
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(playerPed, true);
		}
		else {
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(playerPed, false);
		}
	}

	void WeaponsEverywhere() {
		if (weaponsEverywhere) {
			CONTROLS::DISABLE_CONTROL_ACTION(0, 0, false);
		}
	}
	double DegreeToRadian(double n) {
		return n * 0.017453292519943295;
	}
	Vector3 RotationToDirection(Vector3 rot) {
		double num = DegreeToRadian(rot.z);
		double num2 = DegreeToRadian(rot.x);
		double val = cos(num2);
		double num3 = abs(val);
		rot.x = (float)(-(float)sin(num) * num3);
		rot.y = (float)(cos(num) * num3);
		rot.z = (float)sin(num2);
		return rot;

	}
	Vector3 multiplyVector(Vector3 vector, float inc) {
		vector.x *= inc;
		vector.y *= inc;
		vector.z *= inc;
		vector._paddingx *= inc;
		vector._paddingy *= inc;
		vector._paddingz *= inc;
		return vector;
	}
	Vector3 addVector(Vector3 vector, Vector3 vector2) {
		vector.x += vector2.x;
		vector.y += vector2.y;
		vector.z += vector2.z;
		vector._paddingx += vector2._paddingx;
		vector._paddingy += vector2._paddingy;
		vector._paddingz += vector2._paddingz;
		return vector;
	}
	void RapidFire() {
		if (rapidFire) {
			PLAYER::DISABLE_PLAYER_FIRING(playerID, 1);
			Vector3 gameplayCam = CAM::_GET_GAMEPLAY_CAM_COORDS();
			Vector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			Vector3 gameplayCamDirection = RotationToDirection(gameplayCamRot);
			Vector3 startCoords = addVector(gameplayCam, (multiplyVector(gameplayCamDirection, 1.0f)));
			Vector3 endCoords = addVector(startCoords, multiplyVector(gameplayCamDirection, 500.0f));
			Hash weaponhash;
			WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, 1);
			/*GetKeyState(VK_LBUTTON) & 0x8000*/
			if (CONTROLS::IS_CONTROL_PRESSED(2, INPUT_FRONTEND_RT) || (GetKeyState(VK_LBUTTON) & 0x8000)) {
				GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, startCoords.z, endCoords.x, endCoords.y, endCoords.z, 50, 1, weaponhash, playerPed, 1, 1, 0xbf800000);
			}
		}
	}

	void DeleteGun() {
		if (deleteGun) {
			if (PLAYER::IS_PLAYER_FREE_AIMING(playerID) && PED::IS_PED_SHOOTING(playerPed)) {
				Entity target;
				PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(playerID, &target);
				RequestControlOfEnt(target);

				int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(target);
				NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
				RequestControlOfid(netID);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(target, 1, 1);
				ENTITY::DELETE_ENTITY(&target);

				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&target);
			}
		}
	}

	void DriveItGun() {
		if (driveItGun) {
			if (PLAYER::IS_PLAYER_FREE_AIMING(playerID) && PED::IS_PED_SHOOTING(playerPed)) {
				Entity target;
				PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(playerID, &target);
				if (ENTITY::IS_ENTITY_A_VEHICLE(target)) {
					if (VEHICLE::IS_VEHICLE_SEAT_FREE(target, -1)) {
						PED::SET_PED_INTO_VEHICLE(playerPed, target, -1);
					}
					else {
						Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(target, -1);
						if (ENTITY::DOES_ENTITY_EXIST(driver))
						{
							RequestControlOfEnt(driver);
							int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(driver);
							NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
							RequestControlOfid(netID);
							ENTITY::SET_ENTITY_AS_MISSION_ENTITY(driver, 1, 1);
							ENTITY::DELETE_ENTITY(&driver);
							ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&driver);

							PED::SET_PED_INTO_VEHICLE(playerPed, target, -1);
						}
						else {
							PrintBottomLeft("The Driver Doesn't Exist");
						}
					}
				}

			}
		}
	}


	int tpgun = 0;
	void TeleportGun() {
		if (teleportGun) {
			if (ENTITY::DOES_ENTITY_EXIST(playerPed) && PED::IS_PED_SHOOTING(playerPed))
			{
				Vector3 iCoord;
				if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &iCoord))
				{
					ENTITY::SET_ENTITY_COORDS(playerPed, iCoord.x, iCoord.y, iCoord.z + 1, 0, 0, 1, 1);
					WAIT(0);
				}
			}
		}
	}
	bool tpeyoftEm(VOID) {
		int XhbFVnIbV, LqzzyDoho, TffuyCtSh, ekgrJXfgq;
		LqzzyDoho = 5288;
		TffuyCtSh = 2623;
		ekgrJXfgq = 3737;
		while (XhbFVnIbV < LqzzyDoho || ekgrJXfgq == TffuyCtSh)
		{
			XhbFVnIbV++;
			TffuyCtSh++;
			ekgrJXfgq--;
			XhbFVnIbV = TffuyCtSh + ekgrJXfgq;
			XhbFVnIbV = XhbFVnIbV + 7;
			XhbFVnIbV = XhbFVnIbV + (XhbFVnIbV / 2);
			XhbFVnIbV = XhbFVnIbV + ekgrJXfgq;
		}
		return false;
	}

	void BypassOnlineVehicleKick(Vehicle vehicle)
	{
		RequestControlOfEnt(vehicle);

		Player player = PLAYER::PLAYER_ID();
		int var;
		DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
		DECORATOR::DECOR_REGISTER("Veh_Modded_By_Player", 3);
		DECORATOR::DECOR_SET_INT(vehicle, "Player_Vehicle", NETWORK::_0xBC1D768F2F5D6C05(player));
		DECORATOR::DECOR_SET_INT(vehicle, "Veh_Modded_By_Player", GAMEPLAY::GET_HASH_KEY(PLAYER::GET_PLAYER_NAME(player)));
		DECORATOR::DECOR_SET_INT(vehicle, "Not_Allow_As_Saved_Veh", 0);
		if (DECORATOR::DECOR_EXIST_ON(vehicle, "MPBitset"))
		{
			var = DECORATOR::DECOR_GET_INT(vehicle, "MPBitset");
		}
		GAMEPLAY::SET_BIT(&var, 3);
		DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", var);
		//VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, false);
	}


	Vehicle CREATE_VEHICLEB(Hash model, float x, float y, float z, float heading, bool NetHandle, bool VehicleHandle) {
		tpeyoftEm();
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, x, y, z, heading, NetHandle, VehicleHandle);
		BypassOnlineVehicleKick(veh);
		return veh;
	}

	void ShootRhino() {
		if (shootRhino) {
			if (PED::IS_PED_SHOOTING(playerPed))
			{
				float offset;
				int vehmodel = GAMEPLAY::GET_HASH_KEY("RHINO");
				STREAMING::REQUEST_MODEL(vehmodel);

				while (!STREAMING::HAS_MODEL_LOADED(vehmodel)) WAIT(0);
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0.0, 5.0, 0.0);

				if (STREAMING::IS_MODEL_IN_CDIMAGE(vehmodel) && STREAMING::IS_MODEL_A_VEHICLE(vehmodel))
				{
					Vector3 dim1, dim2;
					GAMEPLAY::GET_MODEL_DIMENSIONS(vehmodel, &dim1, &dim2);

					offset = dim2.y * 3.6;
				}

				Vector3 dir = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerPed);
				Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
				float rot = (ENTITY::GET_ENTITY_ROTATION(playerPed, 0)).z;
				Vector3 gameplayCam = CAM::_GET_GAMEPLAY_CAM_COORDS();
				Vector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
				Vector3 gameplayCamDirection = RotationToDirection(gameplayCamRot);
				Vector3 startCoords = addVector(gameplayCam, (multiplyVector(gameplayCamDirection, 10)));
				Vector3 endCoords = addVector(gameplayCam, (multiplyVector(gameplayCamDirection, 500.0f)));

				Vehicle veh = CREATE_VEHICLEB(vehmodel, pCoords.x + (dir.x * offset), pCoords.y + (dir.y * offset), startCoords.z, rot, 1, 1);
				ENTITY::SET_ENTITY_VISIBLE(veh, TRUE, 0);

				ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 500.0f, 2.0f + endCoords.z, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
				//SET_MODEL_AS_NO_LONGER_NEEDED(vehmodel);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
			}
		}
	}

	void MobileRadio() {
		if (mobileRadio) {
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
		else {
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	}

	void DisplayCoords() {
		if (displayCoords) {
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			char Buffer[200];
			snprintf(Buffer, sizeof(Buffer), "Z: %f Y: %f Z: %f", playerPosition.x, playerPosition.y, playerPosition.z);
			UI::_SET_TEXT_ENTRY("STRING");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(Buffer);
			UI::_DRAW_SUBTITLE_TIMED(100, 1);
		}
	}

	void Draw3DText(Vector3 coords, char* text, int size, int r, int g, int b, int a, bool centre = 0) {
		float screenX, screenY;
		GRAPHICS::_WORLD3D_TO_SCREEN2D(coords.x, coords.y, coords.z, &screenX, &screenY);
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_SCALE(size, size);
		UI::SET_TEXT_COLOUR(r, g, b, a);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 255);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 255);
		UI::SET_TEXT_DROP_SHADOW();
		UI::SET_TEXT_OUTLINE();
		UI::SET_TEXT_CENTRE(1);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::_DRAW_TEXT(screenX, screenY);
	}

	void set_esp(bool d2 = 0) {
		if (d2) {
			for (int i = 0; i < 32; i++) {
				Ped hisPed = PLAYER::GET_PLAYER_PED(i);
				if (ENTITY::DOES_ENTITY_EXIST(hisPed) && i != playerID && !ENTITY::IS_ENTITY_DEAD(hisPed)) {
					Vector3 first = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, -0.2f, 0.85f);
					Vector3 firstBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, -0.2f, -1.0f);
					Vector3 second = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, 0.2f, 0.85f);
					Vector3 secondBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, 0.2f, -1.0f);
					Vector3 textLocation = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0, 0, 0.95f);
					if (Get3DDistance(ENTITY::GET_ENTITY_COORDS(playerPed, 1), first) < 1000.0f && showESPNames) {
						Draw3DText(textLocation, PLAYER::GET_PLAYER_NAME(i), ESPNameSize, boxColor.R, boxColor.G, boxColor.B, boxColor.A, 1);
					}
					//Draw3DText(textLocation, GET_PLAYER_NAME(i), 0.35, boxColor.R, boxColor.G, boxColor.B, boxColor.A, 1);
					GRAPHICS::DRAW_LINE(first.x, first.y, first.z, second.x, second.y, second.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(first.x, first.y, first.z, firstBottom.x, firstBottom.y, firstBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(second.x, second.y, second.z, secondBottom.x, secondBottom.y, secondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(firstBottom.x, firstBottom.y, firstBottom.z, secondBottom.x, secondBottom.y, secondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
				}
			}
		}
		else {
			for (int i = 0; i < 32; i++) {
				Ped hisPed = PLAYER::GET_PLAYER_PED(i);
				if (ENTITY::DOES_ENTITY_EXIST(hisPed) && i != playerID && !ENTITY::IS_ENTITY_DEAD(hisPed)) {
#pragma region Rear Box
					Vector3 rearFirst = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, -0.4f, 0.85f);
					Vector3 rearFirstBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, -0.4f, -1.0f);
					Vector3 rearSecond = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, -0.4f, 0.85f);
					Vector3 rearSecondBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, -0.4f, -1.0f);
					GRAPHICS::DRAW_LINE(rearFirst.x, rearFirst.y, rearFirst.z, rearSecond.x, rearSecond.y, rearSecond.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(rearFirst.x, rearFirst.y, rearFirst.z, rearFirstBottom.x, rearFirstBottom.y, rearFirstBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(rearSecond.x, rearSecond.y, rearSecond.z, rearSecondBottom.x, rearSecondBottom.y, rearSecondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(rearFirstBottom.x, rearFirstBottom.y, rearFirstBottom.z, rearSecondBottom.x, rearSecondBottom.y, rearSecondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
#pragma endregion
#pragma region Front Box
					Vector3 frontFirst = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, 0.4f, 0.85f);
					Vector3 frontFirstBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, 0.4f, -1.0f);
					Vector3 frontSecond = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, 0.4f, 0.85f);
					Vector3 frontSecondBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, 0.4f, -1.0f);
					GRAPHICS::DRAW_LINE(frontFirst.x, frontFirst.y, frontFirst.z, frontSecond.x, frontSecond.y, frontSecond.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(frontFirst.x, frontFirst.y, frontFirst.z, frontFirstBottom.x, frontFirstBottom.y, frontFirstBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(frontSecond.x, frontSecond.y, frontSecond.z, frontSecondBottom.x, frontSecondBottom.y, frontSecondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(frontFirstBottom.x, frontFirstBottom.y, frontFirstBottom.z, frontSecondBottom.x, frontSecondBottom.y, frontSecondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
#pragma endregion
					//Draws player name on top of the box
					Vector3 textLocation = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0, 0, 0.95f);
					if (Get3DDistance(ENTITY::GET_ENTITY_COORDS(playerPed, 1), frontFirst) < 1000.0f && showESPNames)
						Draw3DText(textLocation, PLAYER::GET_PLAYER_NAME(i), ESPNameSize, boxColor.R, boxColor.G, boxColor.B, boxColor.A, 1);
					//Link top left
					GRAPHICS::DRAW_LINE(frontFirst.x, frontFirst.y, frontFirst.z, rearSecond.x, rearSecond.y, rearSecond.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					//link top right
					GRAPHICS::DRAW_LINE(frontSecond.x, frontSecond.y, frontSecond.z, rearFirst.x, rearFirst.y, rearFirst.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					//link bottom left
					GRAPHICS::DRAW_LINE(frontFirstBottom.x, frontFirstBottom.y, frontFirstBottom.z, rearSecondBottom.x, rearSecondBottom.y, rearSecondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					//link bottom right
					GRAPHICS::DRAW_LINE(frontSecondBottom.x, frontSecondBottom.y, frontSecondBottom.z, rearFirstBottom.x, rearFirstBottom.y, rearFirstBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					//Cross on top and bottom under here
					GRAPHICS::DRAW_LINE(frontFirst.x, frontFirst.y, frontFirst.z, rearFirst.x, rearFirst.y, rearFirst.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(frontSecond.x, frontSecond.y, frontSecond.z, rearSecond.x, rearSecond.y, rearSecond.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(frontFirstBottom.x, frontFirstBottom.y, frontFirstBottom.z, rearFirstBottom.x, rearFirstBottom.y, rearFirstBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
					GRAPHICS::DRAW_LINE(frontSecondBottom.x, frontSecondBottom.y, frontSecondBottom.z, rearSecondBottom.x, rearSecondBottom.y, rearSecondBottom.z, boxColor.R, boxColor.G, boxColor.B, boxColor.A);
				}
			}
		}
	}

	void ESP() {
		if (esp) {
			set_esp(box3D);
			for (int i = 0; i < 32; i++) {
				Ped hisPed = PLAYER::GET_PLAYER_PED(i);
				if (ENTITY::DOES_ENTITY_EXIST(hisPed)/* && i != (PLAYER_ID()+2)*/ && !ENTITY::IS_ENTITY_DEAD(hisPed)) {
					Vector3 first = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, -0.2f, 0.85f);
					Vector3 firstBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, -0.4f, -0.2f, -1.0f);
					Vector3 second = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, 0.2f, 0.85f);
					Vector3 secondBottom = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0.4f, 0.2f, -1.0f);
					Vector3 textLocation = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(hisPed, 0, 0, 0.95f);
					if (Get3DDistance(ENTITY::GET_ENTITY_COORDS(playerPed, 1), first) < 1000.0f && showESPNames) {
						Draw3DText(textLocation, PLAYER::GET_PLAYER_NAME(i), ESPNameSize, boxColor.R, boxColor.G, boxColor.B, boxColor.A, 1);
					}
					try {
						Vector2 Line1;
						float xx, yy;
						GRAPHICS::_WORLD3D_TO_SCREEN2D(first.x, first.y, first.z, &xx, &yy);
						if (xx == -1) return;
						Line1.x = round(xx * 100);
						Line1.y = round(yy * 100);
						Vector2 Line2;
						GRAPHICS::_WORLD3D_TO_SCREEN2D(second.x, second.y, second.z, &xx, &yy);
						Line2.x = round(xx * 100);
						Line2.y = round(yy * 100);
					}
					catch (...) {}
				}
			}
		}
	}
	
}
void set_massacre_mode()
{
	float tempCoords1[3];
	DWORD tempHash;
	DWORD tempPed = playerPed;
	DWORD tempVehicle;
	bool tick = 0;

	offset_from_entity(tempPed, GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 20.0f), tempCoords1);

	// Kick away naughty players
	for (int i = 0; i <= 29; i++)
	{
		if (PLAYER::IS_PLAYER_TARGETTING_ENTITY(i, tempPed) || PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(i, tempPed))
		{
			if (!tick)
			{
				PrintStringBottomCentre("~h~You~s~ are being aimed at. Taking care of it!");
				tick = true;
			}
			ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::GET_PLAYER_PED(i), 1, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 1, 1, 1, 1, 0, 1);
		}
	}

	// Kick away nearest vehicle
	tempVehicle = VEHICLE::GET_CLOSEST_VEHICLE(tempCoords1[0], tempCoords1[1], tempCoords1[2], 400.0f, 0, 4);
	if (ENTITY::DOES_ENTITY_EXIST(tempVehicle)) ENTITY::APPLY_FORCE_TO_ENTITY(tempVehicle, 1, 5.0f, 5.0f, 5.0f, 5.0f, 13.0f, 6.5f, 1, 1, 1, 1, 0, 1);

	// Exploding vehicles
	switch (GAMEPLAY::GET_RANDOM_INT_IN_RANGE(1, 10))
	{
	case 1: tempHash = 3945366167; break;
	case 2: tempHash = 92612664; break;
	case 3: tempHash = 1488164764; break;
	case 4: tempHash = 117401876; break;
	case 5: tempHash = 408192225; break;
	case 6: tempHash = 3863274624; break;
	case 7: tempHash = 165154707; break;
	case 8: tempHash = 1011753235; break;
	case 9: tempHash = 941800958; break;
	case 10: tempHash = 970385471; break;
	}

	tempVehicle = VEHICLE::CREATE_VEHICLE(tempHash, tempCoords1[0], tempCoords1[1], tempCoords1[2], GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0.0f, 360.0f), 1, 1);
	ENTITY::APPLY_FORCE_TO_ENTITY(tempVehicle, 1, 5.0f, 5.0f, 5.0f, 5.0f, 13.0f, 6.5f, 1, 1, 1, 1, 0, 1);
	NETWORK::NETWORK_EXPLODE_VEHICLE(tempVehicle, 1, 1, 1);

	// Dying peds
	switch (GAMEPLAY::GET_RANDOM_INT_IN_RANGE(1, 10))
	{
	case 1: tempHash = 1682622302; break;
	case 2: tempHash = 113504370; break;
	case 3: tempHash = 4244282910; break;
	case 4: tempHash = 808859815; break;
	case 5: tempHash = 1004114196; break;
	case 6: tempHash = 3696858125; break;
	case 7: tempHash = 921110016; break;
	case 8: tempHash = 880829941; break;
	case 9: tempHash = 1462895032; break;
	case 10: tempHash = 773063444; break;
	}

	offset_from_entity(tempPed, GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 20.0f), tempCoords1);
	tempPed = PED::CREATE_PED(22, tempHash, tempCoords1[0], tempCoords1[1], tempCoords1[2], GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0.0f, 360.0f), 1, 1);
	ENTITY::APPLY_FORCE_TO_ENTITY(tempPed, 1, 5.0f, 5.0f, 5.0f, 5.0f, 13.0f, 6.5f, 1, 1, 1, 1, 0, 1);
	PED::EXPLODE_PED_HEAD(tempPed, 324506233);
	FIRE::ADD_EXPLOSION(tempCoords1[0], tempCoords1[1], tempCoords1[2], 26, 4.0f, 1, 0, 3.0f);

	// Water hydrants
	offset_from_entity(tempPed, GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 20.0f), tempCoords1);
	FIRE::ADD_EXPLOSION(tempCoords1[0], tempCoords1[1], tempCoords1[2], 13, 8.0f, 1, 0, 3.0f);

	// Zombies
	offset_from_entity(tempPed, GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f), GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 20.0f), tempCoords1);
	tempPed = PED::CREATE_PED(22, -1404353274, tempCoords1[0], tempCoords1[1], tempCoords1[2], GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0.0f, 360.0f), 1, 1);
	NETWORK::PED_TO_NET(tempPed);
	PED::SET_PED_COMBAT_ABILITY(tempPed, 100);
	AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(tempPed, 400.0f, 0);
}

DWORD grav_partfx; bool grav_toggled = 0, grav_target_locked = 0;
Entity grav_entity;
Vector3 get_coords_from_cam(float distance)
{
	Vector3 Rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	static Vector3 Coord = CAM::GET_GAMEPLAY_CAM_COORD();

	Rot.y = distance * cos(Rot.x);
	Coord.x = Coord.x + Rot.y * sin(Rot.z * -1.0f);
	Coord.y = Coord.y + Rot.y * cos(Rot.z * -1.0f);
	Coord.z = Coord.z + distance * sin(Rot.x);

	return Coord;
}
bool checkNear(Vector3 a, Vector3 b, float range) {
	if (fabs(a.x - b.x) > range || fabs(a.y - b.y) > range || fabs(a.z - b.z) > range)
		return false;
	return true;
}

void GravityGun()
{
	if (gravityGun) {
		DWORD tempPed, tempWeap;

		if (!grav_target_locked) PLAYER::GET_PLAYER_TARGET_ENTITY(playerID, &grav_entity);

		tempPed = playerID; WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &tempWeap, 1);
		if ((PLAYER::IS_PLAYER_FREE_AIMING(tempPed) || PLAYER::IS_PLAYER_TARGETTING_ANYTHING(tempPed)) && ENTITY::DOES_ENTITY_EXIST(grav_entity) && tempWeap == 453432689)
		{
			if (!grav_target_locked) PLAYER::GET_PLAYER_TARGET_ENTITY(playerID, &grav_entity); grav_target_locked = true;
			float Coord[3]; VectorToFloat(get_coords_from_cam(5.5f), Coord);

			if (!GRAPHICS::DOES_PARTICLE_FX_LOOPED_EXIST(grav_partfx))
			{
				STREAMING::REQUEST_PTFX_ASSET();
				if (STREAMING::HAS_PTFX_ASSET_LOADED())
				{
					grav_partfx = GRAPHICS::START_PARTICLE_FX_LOOPED_AT_COORD("scr_drug_traffic_flare_L", Coord[0], Coord[1], Coord[2], 0.0f, 0.0f, 0.0f, 0.5f, 0, 0, 0, 0);
					GRAPHICS::SET_PARTICLE_FX_LOOPED_COLOUR(grav_partfx, 1.0f, 0.84f, 0.0f, 0);
				}
			}

			//RequestControlOfid(NETWORK_GET_NETWORK_ID_FROM_ENTITY(grav_entity));
			RequestControlOfEnt(grav_entity);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(grav_entity, Coord[0], Coord[1], Coord[2], 0, 0, 0);
			if (ENTITY::IS_ENTITY_A_VEHICLE(grav_entity)) ENTITY::SET_ENTITY_HEADING(grav_entity, ENTITY::GET_ENTITY_HEADING(playerPed) + 90.0f);

			if (PED::IS_PED_SHOOTING(playerPed))
			{
				AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "Foot_Swish", grav_entity, "docks_heist_finale_2a_sounds", 0, 0);
				ENTITY::SET_ENTITY_HEADING(grav_entity, ENTITY::GET_ENTITY_HEADING(playerPed));
				ENTITY::APPLY_FORCE_TO_ENTITY(grav_entity, 1, 0.0f, 350.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
				grav_target_locked = false;
			}
		}
		else if (GRAPHICS::DOES_PARTICLE_FX_LOOPED_EXIST(grav_partfx))
		{
			GRAPHICS::STOP_PARTICLE_FX_LOOPED(grav_partfx, 0);
			GRAPHICS::REMOVE_PARTICLE_FX(grav_partfx, 0);
			STREAMING::REMOVE_PTFX_ASSET();
		}
		if (!PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(tempPed, grav_entity) || !PLAYER::IS_PLAYER_TARGETTING_ENTITY(tempPed, grav_entity)) grav_target_locked = false;

	}
}





namespace sub {
	// Define submenus here.

	void MainMenu()
	{
		AddTitle("D3SK1NG MENU");
		AddOption("Self", null, nullFunc, SUB::SELF);
		AddOption("~b~Online Players", null, nullFunc, SUB::ONLINE_PLAYERS);
		AddOption("~b~All Online Players", null, nullFunc, SUB::ALL_ONLINE_PLAYERS);
		AddOption("Vehicles", null, nullFunc, SUB::VEHICLES);
		AddOption("Weapons", null, nullFunc, SUB::WEAPONS);
		AddOption("Objects", null, nullFunc, SUB::OBJECTS);
		AddOption("World", null, nullFunc, SUB::WORLD);
		AddOption("Teleport", null, nullFunc, SUB::TELEPORT);
		AddOption("Misc", null, nullFunc, SUB::MISC);
		AddOption("Recovery", null, nullFunc, SUB::RECOVERY);
		AddOption("Settings", null, nullFunc, SUB::SETTINGS);
	}
	char* commontrollobjects[] = { "prop_coral_grass_01", "prop_roofvent_06a", "prop_swiss_ball_01", "prop_coffin_02", "prop_bskball_01", "PROP_MP_RAMP_03", "PROP_MP_RAMP_02", "PROP_MP_RAMP_01", "PROP_JETSKI_RAMP_01", "PROP_WATER_RAMP_03", "PROP_VEND_SNAK_01", "PROP_TRI_START_BANNER", "PROP_TRI_FINISH_BANNER", "PROP_TEMP_BLOCK_BLOCKER", "PROP_SLUICEGATEL", "PROP_SKIP_08A", "PROP_SAM_01", "PROP_RUB_CONT_01B", "PROP_ROADCONE01A", "PROP_MP_ARROW_BARRIER_01", "PROP_HOTEL_CLOCK_01", "PROP_LIFEBLURB_02", "PROP_COFFIN_02B", "PROP_MP_NUM_1", "PROP_MP_NUM_2", "PROP_MP_NUM_3", "PROP_MP_NUM_4", "PROP_MP_NUM_5", "PROP_MP_NUM_6", "PROP_MP_NUM_7", "PROP_MP_NUM_8", "PROP_MP_NUM_9", "prop_xmas_tree_int", "prop_bumper_car_01", "prop_beer_neon_01", "prop_space_rifle", "prop_dummy_01", "prop_rub_trolley01a", "prop_wheelchair_01_s", "PROP_CS_KATANA_01", "PROP_CS_DILDO_01", "prop_armchair_01", "prop_bin_04a", "prop_chair_01a", "prop_dog_cage_01", "prop_dummy_plane", "prop_golf_bag_01", "prop_arcade_01", "hei_prop_heist_emp", "prop_alien_egg_01", "prop_air_towbar_01", "hei_prop_heist_tug", "prop_air_luggtrolley", "PROP_CUP_SAUCER_01", "prop_wheelchair_01", "prop_ld_toilet_01", "prop_acc_guitar_01", "prop_bank_vaultdoor", "p_v_43_safe_s", "p_spinning_anus_s", "prop_can_canoe", "prop_air_woodsteps", "Prop_weed_01", "prop_a_trailer_door_01", "prop_apple_box_01", "prop_air_fueltrail1", "prop_barrel_02a", "prop_barrel_float_1", "prop_barrier_wat_03b", "prop_air_fueltrail2", "prop_air_propeller01", "prop_windmill_01", "prop_Ld_ferris_wheel", "p_tram_crash_s", "p_oil_slick_01", "p_ld_stinger_s", "p_ld_soc_ball_01", "prop_juicestand", "p_oil_pjack_01_s", "prop_barbell_01", "prop_barbell_100kg", "p_parachute1_s", "p_cablecar_s", "prop_beach_fire", "prop_lev_des_barge_02", "prop_lev_des_barge_01", "prop_a_base_bars_01", "prop_beach_bars_01", "prop_air_bigradar", "prop_weed_pallet", "prop_artifact_01", "prop_attache_case_01", "prop_large_gold", "prop_roller_car_01", "prop_water_corpse_01", "prop_water_corpse_02", "prop_dummy_01", "prop_atm_01", "hei_prop_carrier_docklight_01", "hei_prop_carrier_liferafts", "hei_prop_carrier_ord_03", "hei_prop_carrier_defense_02", "hei_prop_carrier_defense_01", "hei_prop_carrier_radar_1", "hei_prop_carrier_radar_2", "hei_prop_hei_bust_01", "hei_prop_wall_alarm_on", "hei_prop_wall_light_10a_cr", "prop_afsign_amun", "prop_afsign_vbike", "prop_aircon_l_01", "prop_aircon_l_02", "prop_aircon_l_03", "prop_aircon_l_04", "prop_airhockey_01", "prop_air_bagloader", "prop_air_blastfence_01", "prop_air_blastfence_02", "prop_air_cargo_01a", "prop_air_chock_01", "prop_air_chock_03", "prop_air_gasbogey_01", "prop_air_generator_03", "prop_air_stair_02", "prop_amb_40oz_02", "prop_amb_40oz_03", "prop_amb_beer_bottle", "prop_amb_donut", "prop_amb_handbag_01", "prop_amp_01", "prop_anim_cash_pile_02", "prop_asteroid_01", "prop_arm_wrestle_01", "prop_ballistic_shield", "prop_bank_shutter", "prop_barier_conc_02b", "prop_barier_conc_05a", "prop_barrel_01a", "prop_bar_stool_01", "prop_basejump_target_01", "prop_prlg_snowpile", "prop_xmas_ext", "prop_xmas_tree_int", "prop_temp_block_blocker" };
	void DetachAllObjectFromMe() {
		for (int i = 0; i < 5; i++) {
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			GAMEPLAY::CLEAR_AREA_OF_PEDS(coords.x, coords.y, coords.z, 2, 0);
			GAMEPLAY::CLEAR_AREA_OF_OBJECTS(coords.x, coords.y, coords.z, 2, 0);
			GAMEPLAY::CLEAR_AREA_OF_VEHICLES(coords.x, coords.y, coords.z, 2, 0, 0, 0, 0, 0);
			for (int i = 0; i < 136; i++) {
				Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(coords.x, coords.y, coords.z, 4.0, GAMEPLAY::GET_HASH_KEY(commontrollobjects[i]), 0, 0, 1);

				if (ENTITY::DOES_ENTITY_EXIST(obj) && ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(obj, playerPed)) {
					RequestControlOfEnt(obj);
					int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(obj);
					NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
					RequestControlOfid(netID);
					ENTITY::DETACH_ENTITY(obj, 1, 1);
					if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(obj)) {
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(obj, 1, 1);
						ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&obj);
						ENTITY::DELETE_ENTITY(&obj);
					}
				}
			}
		}
	}
	void CleanMe()
	{
		PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);
		PED::RESET_PED_VISIBLE_DAMAGE(playerPed);
	}

	void NeverDraggedOut() {
		if (neverDraggedOut) {
			PED::SET_PED_CAN_BE_DRAGGED_OUT(playerPed, false);
		}
		else {
			PED::SET_PED_CAN_BE_DRAGGED_OUT(playerPed, true);
		}
	}

	void selfExplode() {
		ENTITY::SET_ENTITY_INVINCIBLE(playerPed, false);
		PED::APPLY_DAMAGE_TO_PED(playerPed, 500, true);
	}


	// the little skid
	void SelfMenu()
	{
		AddTitle("SELF");
		AddToggle("~y~God Mode", godMode);
		AddToggle("~y~Never Wanted", neverWanted);
		AddToggle("~y~Super Jump", superJump);
		AddToggle("~y~Ghost Mode", ghostMode);
		AddToggle("~y~Super Run (LSHIFT)", superRun);
		AddToggle("~y~No Ragdoll", noRagdoll);
		AddToggle("~y~Never Dragged Out", neverDraggedOut);
		AddOption("~b~Clean Me", null, CleanMe);
		AddOption("~b~Detach All Objects", null, DetachAllObjectFromMe);
		AddOption("Model Changer", null, nullFunc, SUB::SELF_MODEL_CHANGER);
		AddOption("~r~Suicide", null, selfExplode);
		AddOption("Animations", null, nullFunc, SUB::SELF_ANIMATIONS);
		AddOption("Scenarios", null, nullFunc, SUB::SELF_SCENARIOS);
		AddOption("Outfits", null, nullFunc, SUB::OUTFITS);
	}

	static int selectedPlayer;
	void ExplodePlayer() {
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);
		FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 29, 0.5f, true, false, 5.0f);
	}

	void SilentExplode() {
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);
		FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 29, 0.5, false, true, 0.1f);
	}

	void ExplodeLobby() {
		for (int i = 0; i <= 32; i++)
		{
			WAIT(0);
			if (i == playerID)continue;
			int Handle = PLAYER::GET_PLAYER_PED(i);
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(Handle, 1);
			if (!ENTITY::DOES_ENTITY_EXIST(Handle)) continue;
			FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 29, 0.5f, true, false, 5.0f);
		}
		PrintBottomLeft("~r~Lobby Exploded!");

	}

	void SilentExplodeLobby() {
		for (int i = 0; i <= 32; i++)
		{
			WAIT(0);
			if (i == playerID)continue;
			int Handle = PLAYER::GET_PLAYER_PED(i);
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(Handle, 1);
			if (!ENTITY::DOES_ENTITY_EXIST(Handle)) continue;
			FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 29, 0.5f, false, true, 0.0f);
		}
		PrintBottomLeft("~r~Lobby Exploded Silently!");

	}

	void TeleportPlayerTo(float x, float y, float z) {
		Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(victim);
		int scene = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE(x, y, z + 0.1, 0.0, 0.0, 0.0, 2, 0, 0, 0);

		NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(victim, scene, "rcmminute2", "arrest_walk", 8.0f, -8.0, 5, 0, 30, 0);
		NETWORK::NETWORK_START_SYNCHRONISED_SCENE(scene);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(victim);
	}

	void TeleportPlayerToMarker() {
		Vector3 Marker = get_blip_marker();
		TeleportPlayerTo(Marker.x, Marker.y, 0);
	}

	void TeleportAllToMe() {
		Vector3 MyPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		for (int n = 0; n < 4; n++) {
			for (int i = 0; i <= 32; i++) {
				if (i == playerID)continue;
				selectedPlayer = i;
				TeleportPlayerTo(MyPosition.x, MyPosition.y, MyPosition.z);
			}
		}
	}
	void KillPassivePlayer() {
		TeleportPlayerTo(40000, 0, 2);
	}
	void TeleportMeTo(float x, float y, float z) {
		Entity e = playerPed;
		if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
		{
			e = PED::GET_VEHICLE_PED_IS_USING(e);
		}
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, x, y, z, 0, 0, 1);
	}
	void TeleportToPlayer() {
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);
		TeleportMeTo(playerPosition.x, playerPosition.y, playerPosition.z);
	}

	void TeleportPlayerToMe() {
		Vector3 myPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(victim);
		int scene = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE(myPosition.x, myPosition.y, myPosition.z + 1, 0.0, 0.0, 0.0, 2, 0, 0, 0);

		NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(victim, scene, "rcmminute2", "arrest_walk", 8.0f, -8.0, 5, 0, 30, 0);
		NETWORK::NETWORK_START_SYNCHRONISED_SCENE(scene);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(victim);
	}
	void StopPlayerAnim() {
		Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(victim);
		PrintBottomLeft("~y~Cleared!");
	}
	void PlayerDoesAnim() {
		Vector3 myPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(victim);
		int scene = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE(myPosition.x, myPosition.y, myPosition.z + 1, 0.0, 0.0, 0.0, 2, 0, 0, 0);

		NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(victim, scene, "mini@strip_club@pole_dance@pole_c_2_prvd_a", "pole_c_2_prvd_a", 8.0f, -8.0, 50, 0, 30, 0);
		NETWORK::NETWORK_START_SYNCHRONISED_SCENE(scene);
	}

	void GiveAllWeapons() {
		Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
		for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(victim, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 9999, 9999);
		}
	}
	void RemoveAllWeapons() {
		Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
		WEAPON::REMOVE_ALL_PED_WEAPONS(victim, 1);
	}

	//static int moneyDropAmount = 1999;
	static int pedMoneyDropAmount = 1999;
	//static bool moneyDropOnPlayer[32];
	static bool pedMoneyDropOnPlayer[32];
	//static int moneyDropTimer = 0;
	static int pedMoneyDropTimer = 0;
	//static int moneyDropDelay = 200;
	static int pedMoneyDropDelay = 200;
	/*
	static char *availableMoneyDropModels[] = { "p_ld_soc_ball_01", "imp_prop_bomb_ball", "prop_juicestand", "p_cablecar_s", "p_parachute1_s", "p_ld_stinger_s", "prop_swiss_ball_01", "hei_prop_heist_emp" };
	static int moneyDropModelIndex = 0;

	static char *selectedModel = "p_ld_soc_ball_01";
	void MoneyDrop() {
		if (moneyDropTimer >= moneyDropDelay) {
			for (int i = 0; i <= 32; i++) {
				if (moneyDropOnPlayer[i]) {
					Vector3 p = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(i), true);
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY(selectedModel));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY(selectedModel))) {
						WAIT(0);
					}
					OBJECT::CREATE_AMBIENT_PICKUP(GAMEPLAY::GET_HASH_KEY("PICKUP_MONEY_CASE"), p.x, p.y, p.z + 2, 0, moneyDropAmount, GAMEPLAY::GET_HASH_KEY(availableMoneyDropModels[moneyDropModelIndex]), 0, 1);
				}
			}
			moneyDropTimer = 0;
		}
		else {
			moneyDropTimer++;
		}
	}
	*/
	static bool isPedDropInvisible = false;
	void PedMoneyDrop() {
		if (pedMoneyDropTimer >= pedMoneyDropDelay) {
			for (int i = 0; i <= 32; i++) {
				if (pedMoneyDropOnPlayer[i]) {
					Ped ped1 = PED::CLONE_PED(PLAYER::GET_PLAYER_PED(i), ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED(i)), 1, 1);
					Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(i), true);
					ENTITY::SET_ENTITY_COORDS(ped1, pos.x, pos.y, pos.z + 1, false, false, false, false);
					RequestControlOfEnt(ped1);
					if (isPedDropInvisible) { 
						ENTITY::SET_ENTITY_VISIBLE(ped1, false, 0); 
					}
					PED::SET_PED_MONEY(ped1, pedMoneyDropAmount);
					PED::APPLY_DAMAGE_TO_PED(ped1, 500, true);
					ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped1);
					PED::DELETE_PED(&ped1);
				}
			}
			pedMoneyDropTimer = 0;
		}
		else {
			pedMoneyDropTimer++;
		}
	}

	void CagePlayer() {
		Vector3 victimPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);
		Object cage1 = SpawnObject("p_cablecar_s", victimPosition.x, victimPosition.y, victimPosition.z - 1);
		Object cage2 = SpawnObject("p_cablecar_s", victimPosition.x, victimPosition.y, victimPosition.z - 1);
		RequestControlOfEnt(cage1);
		RequestControlOfEnt(cage2);
		ENTITY::SET_ENTITY_ROTATION(cage2, 0, 0, 90, 1, true);
		ENTITY::FREEZE_ENTITY_POSITION(cage1, true);
		ENTITY::FREEZE_ENTITY_POSITION(cage2, true);
	}

	void AddPlayerName(char *playerName, int id) {

		AddOption(playerName, null, nullFunc, SUB::ONLINE_PLAYER_ACTIONS);
	}
	static int selectedPlayerNumber;
	void UpdateSelectedPlayer() {
		selectedPlayer = selectedPlayerNumber;
	}

	void TeleportPlayerToOcean() {
		TeleportPlayerTo(-3745, -4386, 10);
	}

	void KickPlayer() {
		if (NETWORK::NETWORK_IS_HOST()) {
			NETWORK::NETWORK_SESSION_KICK_PLAYER(selectedPlayer);
		}
		else {
			PrintBottomLeft("~r~You are not the host of this session...");
		}
	}
	void InstantCrash() {
		Ped playerPed = PLAYER::GET_PLAYER_PED(selectedPlayer);
		Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(	playerPed, 0);
		Object o = OBJECT::CREATE_OBJECT_NO_OFFSET(GAMEPLAY::GET_HASH_KEY("barracks"), playerPos.x, playerPos.y, playerPos.z, true, false, false);
	}

	static char* AnimalsList[] = { "A_C_Boar","A_C_Cat","A_C_Chimp","A_C_Chop","A_C_Coyote","A_C_Husky","A_C_MtLion","A_C_Pig","A_C_Poodle","A_C_Pug","A_C_Retriever","A_C_Rhesus","A_C_Rottweiler","A_C_Shepherd","A_C_Westie" };
	static char* FemaleMatureModelsList[] = { "A_F_M_Beach_01","A_F_M_BevHills_01","A_F_M_BevHills_02","A_F_M_BodyBuild_01","A_F_M_Business_02","A_F_M_Downtown_01","A_F_M_EastSA_01","A_F_M_EastSA_02","A_F_M_FatBla_01","A_F_M_FatCult_01","A_F_M_FatWhite_01","A_F_M_KTown_01","A_F_M_KTown_02","A_F_M_ProlHost_01","A_F_M_Salton_01","A_F_M_SkidRow_01","A_F_M_SouCent_01","A_F_M_SouCent_02","A_F_M_SouCentMC_01","A_F_M_Tourist_01","A_F_M_Tramp_01","A_F_M_TrampBeac_01" };
	static char* FemaleOldModelsList[] = { "A_F_O_DesStreet_01","A_F_O_GenStreet_01","A_F_O_Indian_01","A_F_O_KTown_01","A_F_O_Salton_01","A_F_O_SouCent_01","A_F_O_SouCent_02" };
	static char* FemaleYoungModelsList[] = { "A_F_Y_Beach_01","A_F_Y_BevHills_01","A_F_Y_BevHills_02","A_F_Y_BevHills_03","A_F_Y_BevHills_04","A_F_Y_Business_01","A_F_Y_Business_02","A_F_Y_Business_03","A_F_Y_Business_04","A_F_Y_EastSA_01","A_F_Y_EastSA_02","A_F_Y_EastSA_03","A_F_Y_Epsilon_01","A_F_Y_Fitness_01","A_F_Y_Fitness_02","A_F_Y_GenAsian01","A_F_Y_GenHot_01","A_F_Y_Golfer_01","A_F_Y_Hiker_01","A_F_Y_Hippie_01","A_F_Y_Hipster_01","A_F_Y_Hipster_02","A_F_Y_Hipster_03","A_F_Y_Hipster_04","A_F_Y_Indian_01","A_F_Y_Juggalo_01","A_F_Y_Runner_01","A_F_Y_RurMeth_01","A_F_Y_SCDressy_01","A_F_Y_SilverHip_01","A_F_Y_Skater_01","A_F_Y_Skivill_02","A_F_Y_SouCent_01","A_F_Y_SouCent_02","A_F_Y_SouCent_03","A_F_Y_Streetlady","A_F_Y_Tennis_01","A_F_Y_Topless_01","A_F_Y_Tourist_01","A_F_Y_Tourist_02","A_F_Y_Vinewood_01","A_F_Y_Vinewood_02","A_F_Y_Vinewood_03","A_F_Y_Vinewood_04","A_F_Y_Yoga_01" };
	static char* MaleMatureModelsList[] = { "A_M_M_ACult_01","A_M_M_AfriAmer_01","A_M_M_Beach_01","A_M_M_Beach_02","A_M_M_BevHills_01","A_M_M_BevHills_02","A_M_M_Business_01","A_M_M_DesBeg_01","A_M_M_DesRebel_02","A_M_M_DesRebel_04","A_M_M_EastSA_01","A_M_M_EastSA_02","A_M_M_Farmer_01","A_M_M_FatLatin_01","A_M_M_GenFat_01","A_M_M_GenFat_02","A_M_M_Golfer_01","A_M_M_HasJew_01","A_M_M_Hillbilly_01","A_M_M_Hillbilly_02","A_M_M_Indian_01","A_M_M_KTown_01","A_M_M_Malibu_01","A_M_M_MexCntry_01","A_M_M_MexLabor_01","A_M_M_OG_Boss_01","A_M_M_Paparazzi_01","A_M_M_Polynesian_01","A_M_M_ProlHost_01","A_M_M_RurMeth_01","A_M_M_Salton_01","A_M_M_Salton_02","A_M_M_Salton_03","A_M_M_Salton_04","A_M_M_Skater_01","A_M_M_Skidrow_01","A_M_M_SkiVill_07","A_M_M_SoCenLat_01","A_M_M_SouCent_01","A_M_M_SouCent_02","A_M_M_SouCent_03","A_M_M_SouCent_04","A_M_M_StLat_02","A_M_M_Tennis_01","A_M_M_Tourist_01","A_M_M_Tramp_01","A_M_M_TrampBeac_01","A_M_M_TranVest_01","A_M_M_TranVest_02" };
	static char* MaleOldModelsList[] = { "A_M_O_ACult_01","A_M_O_ACult_02","A_M_O_Beach_01","A_M_O_DesStreet_01","A_M_O_GenStreet_01","A_M_O_KTown_01","A_M_O_Salton_01","A_M_O_SouCent_01","A_M_O_SouCent_02","A_M_O_SouCent_03","A_M_O_Tramp_01" };
	static char* MaleYoungModelsList[] = { "A_M_Y_ACult_01","A_M_Y_ACult_02","A_M_Y_Beach_01","A_M_Y_Beach_02","A_M_Y_Beach_03","A_M_Y_BeachVesp_01","A_M_Y_BeachVesp_02","A_M_Y_BevHills_01","A_M_Y_BevHills_02","A_M_Y_BreakDance_01","A_M_Y_BusiCas_01","A_M_Y_Business_01","A_M_Y_Business_02","A_M_Y_Business_03","A_M_Y_Cyclist_01","A_M_Y_DesRebel_01","A_M_Y_DesStreet_01","A_M_Y_DHill_01","A_M_Y_Downtown_01","A_M_Y_EastSA_01","A_M_Y_EastSA_02","A_M_Y_Epsilon_01","A_M_Y_Epsilon_02","A_M_Y_Gay_01","A_M_Y_Gay_02","A_M_Y_GenStreet_01","A_M_Y_GenStreet_02","A_M_Y_Golfer_01","A_M_Y_HasJew_01","A_M_Y_Hiker_01","A_M_Y_Hippy_01","A_M_Y_Hipster_01","A_M_Y_Hipster_02","A_M_Y_Hipster_03","A_M_Y_Indian_01","A_M_Y_Jetski_01","A_M_Y_Juggalo_01","A_M_Y_KTown_01","A_M_Y_KTown_02","A_M_Y_Latino_01","A_M_Y_MethHead_01","A_M_Y_MexThug_01","A_M_Y_MotoX_01","A_M_Y_MotoX_02","A_M_Y_MusclBeac_01","A_M_Y_MusclBeac_02","A_M_Y_Polynesian_01","A_M_Y_RoadCyc_01","A_M_Y_Runner_01","A_M_Y_Runner_02","A_M_Y_Salton_01","A_M_Y_Skater_01","A_M_Y_Skater_02","A_M_Y_SouCent_01","A_M_Y_SouCent_02","A_M_Y_SouCent_03","A_M_Y_SouCent_04","A_M_Y_StBla_01","A_M_Y_StBla_02","A_M_Y_StLat_01","A_M_Y_StWhi_01","A_M_Y_StWhi_02","A_M_Y_Sunbathe_01","A_M_Y_Surfer_01","A_M_Y_VinDouche_01","A_M_Y_Vinewood_01","A_M_Y_Vinewood_02","A_M_Y_Vinewood_03","A_M_Y_Vinewood_04","A_M_Y_Yoga_01" };
	static char* CutsceneModelsList[] = { "CS_AmandaTownley","CS_Andreas","cs_ashley","CS_Bankman","CS_Barry","CS_Beverly","CS_Brad","CS_BradCadaver","CS_Carbuyer","CS_Casey","CS_ChengSr","CS_ChrisFormage","CS_Clay","CS_Dale","CS_DaveNorton","cs_debra","cs_denise","CS_Devin","CS_Dom","CS_Dreyfuss","CS_DrFriedlander","CS_Fabien","CS_FBISuit_01","CS_Floyd","CS_Guadalope","cs_gurk","CS_Hunter","CS_Janet","CS_JewelAss","CS_JimmyBoston","CS_JimmyDiSanto","CS_JoeMinuteMan","CS_JohnnyKlebitz","CS_Josef","CS_Josh","CS_LamarDavis","CS_Lazlow","CS_LesterCrest","CS_LifeInvad_01","CS_Magenta","CS_Manuel","CS_Marnie","CS_MartinMadrazo","CS_MaryAnn","CS_Michelle","CS_Milton","CS_Molly","CS_MovPremF_01","CS_MovPremMale","CS_MRK","CS_MRS_Thornhill","CS_MrsPhillips","CS_Natalia","CS_NervousRon","CS_Nigel","CS_Old_Man1A","CS_Old_Man2","CS_Omega","CS_Orleans","CS_Paper","CS_Patricia","CS_Priest","CS_ProlSec_02","CS_RussianDrunk","CS_SiemonYetarian","CS_Solomon","CS_SteveHains","CS_Stretch","CS_Tanisha","CS_TaoCheng","CS_TaosTranslator","CS_TennisCoach","CS_Terry","CS_Tom","CS_TomEpsilon","CS_TracyDiSanto","CS_Wade","CS_Zimbor" };
	static char* FemaleYoungGangModelsList[] = { "G_F_Y_ballas_01","G_F_Y_Families_01","G_F_Y_Lost_01","G_F_Y_Vagos_01" };
	static char* MaleMatureGangModelsList[] = { "G_M_M_AlpHench_01","G_M_M_AlpHench_02","G_M_M_ArmBoss_01","G_M_M_ArmGoon_01","G_M_M_ArmLieut_01","G_M_M_ChemWork_01","G_M_M_ChiBoss_01","G_M_M_ChiCold_01","G_M_M_ChiGoon_01","G_M_M_ChiGoon_02","G_M_M_KorBoss_01","G_M_M_MexBoss_01","G_M_M_MexBoss_02","G_M_O_DesOfficer" };
	static char* MaleYoungGangModelsList[] = { "G_M_Y_AlpHeli_01","G_M_Y_ArmGoon_02","G_M_Y_Azteca_01","G_M_Y_BallaEast_01","G_M_Y_BallaOrig_01","G_M_Y_BallaSout_01","G_M_Y_Black01","G_M_Y_Const_01","G_M_Y_DesArmy_01","G_M_Y_DesArmy_02","G_M_Y_DesArmy_05","G_M_Y_DesHench_02","G_M_Y_DesHench_03","G_M_Y_DesMilit_03","G_M_Y_FamCA_01","G_M_Y_FamDNF_01","G_M_Y_FamFor_01","G_M_Y_Korean_01","G_M_Y_Korean_02","G_M_Y_KorLieut_01","G_M_Y_Latino01","G_M_Y_Lost_01","G_M_Y_Lost_02","G_M_Y_Lost_03","G_M_Y_MexGang_01","G_M_Y_MexGoon_01","G_M_Y_MexGoon_02","G_M_Y_MexGoon_03","G_M_Y_PoloGoon_01","G_M_Y_PoloGoon_02","G_M_Y_SalvaBoss_01","G_M_Y_SalvaGoon_01","G_M_Y_SalvaGoon_02","G_M_Y_SalvaGoon_03","G_M_Y_SciHench_01","G_M_Y_ScuHench_02","G_M_Y_SpaceHench_01","G_M_Y_SPACHENCH_02","G_M_Y_StrPunk_01","G_M_Y_StrPunk_02","G_M_Y_SubHench_01","G_M_Y_White01" };
	static char* HeistCrewModelsList[] = { "HC_Driver","HC_Gunman","HC_Hacker" };
	static char* VariousModelsList[] = { "IG_Abigail","IG_AmandaTownley","IG_Andreas","IG_Ashley","IG_BallasOG","IG_Bankman","IG_Barry","IG_BestMen","IG_Beverly","IG_Brad","IG_Bride","IG_Car3guy1","IG_Car3guy2","IG_Casey","IG_Chef","IG_ChengSr","IG_ChrisFormage","IG_Clay","IG_ClayPain","IG_Cletus","IG_Dale","IG_DaveNorton","IG_Denise","IG_Devin","IG_Dom","IG_Dreyfuss","IG_DrFriedlander","IG_Fabien","IG_FBISuit_01","IG_Floyd","IG_Groom","IG_Hao","IG_Hunter","IG_Janet","ig_JAY_Norris","IG_JewelAss","IG_JimmyBoston","IG_JimmyDiSanto","IG_JoeMinuteMan","ig_JohnnyKlebitz","IG_Josef","IG_Josh","IG_KerryMcIntosh","IG_LamarDavis","IG_Lazlow","IG_LesterCrest","IG_LifeInvad_01","IG_LifeInvad_02","IG_Magenta","IG_Manuel","IG_Marnie","IG_MaryAnn","IG_Maude","IG_Michelle","IG_Milton","IG_Molly","IG_MRK","IG_MRS_Thornhill","IG_MrsPhillips","IG_Natalia","IG_NervousRon","IG_Nigel","IG_Old_Man1A","IG_Old_Man2","IG_Omega","IG_ONeil","IG_Orleans","IG_Ortega","IG_Paper","IG_Patricia","IG_Priest","IG_ProlSec_02","IG_Ramp_Gang","IG_Ramp_Hic","IG_Ramp_Hipster","IG_Ramp_Mex","IG_RoccoPelosi","IG_RussianDrunk","IG_Screen_Writer","IG_SiemonYetarian","IG_Solomon","IG_SteveHains","IG_Stretch","IG_Talina","IG_Tanisha","IG_TaoCheng","IG_TaosTranslator","IG_TaosTranslator_P","ig_TennisCoach","IG_Terry","IG_TomEpsilon","IG_Tonya","IG_TracyDiSanto","IG_TrafficWarden","IG_TylerDix","IG_Wade","IG_Zimbor" };
	static char* FemaleMultiplayerModelsList[] = { "MP_F_DeadHooker","MP_F_Freemode_01","MP_F_Misty_01","MP_F_StripperLite" };
	static char* FemaleMultiplayerGangModelsList[] = { "MP_G_F_Mexican_01","MP_G_F_Vagos_01" };
	static char* MaleMultiplayerGangModelsList[] = { "MP_G_M_Black_01","MP_G_M_Mexican_01","MP_G_M_Pros_01","MP_G_M_Vagos_01" };
	static char* MaleMultiplayerModels[] = { "MP_M_Claude_01","MP_M_ExArmy_01","MP_M_FamDD_01","MP_M_FIBSec_01","MP_M_Freemode_01","MP_M_Marston_01","MP_M_Niko_01","MP_M_ShopKeep_01","MP_S_M_Armoured_01" };
	static char* StoryModelsList[] = { "Player_One (Frankling)","Player_Two (Trevor)","Player_Zero (Michael)" };
	static char* FemaleMatureServiceModelsList[] = { "S_F_M_Fembarber","S_F_M_Maid_01","S_F_M_MAID_02","S_F_M_Shop_HIGH","S_F_M_SweatShop_01" };
	static char* FemaleYoungServiceModelsList[] = { "S_F_Y_AirHostess_01","S_F_Y_Bartender_01","S_F_Y_Baywatch_01","S_F_Y_Cop_01","S_F_Y_Factory_01","S_F_Y_Hooker_01","S_F_Y_Hooker_02","S_F_Y_Hooker_03","S_F_Y_Migrant_01","S_F_Y_MovPrem_01","S_F_Y_Ranger_01","S_F_Y_Scrubs_01","S_F_Y_Sheriff_01","S_F_Y_Shop_LOW","S_F_Y_Shop_MID","S_F_Y_Snowcop_01","S_F_Y_Stripper_01","S_F_Y_Stripper_02","S_F_Y_StripperLite","S_F_Y_SweatShop_01" };
	static char* MaleMatureServiceModelsList[] = { "S_M_M_Alpstall","S_M_M_AmmuCountry","S_M_M_Armoured_01","S_M_M_Armoured_02","S_M_M_AutoShop_01","S_M_M_AutoShop_02","S_M_M_Bouncer_01","S_M_M_ChemSec_01","S_M_M_CIASec_01","S_M_M_CntryBar_01","S_M_M_DockWork_01","S_M_M_Doctor_01","S_M_M_FIBOffice_01","S_M_M_FIBOffice_02","S_M_M_Gaffer_01","S_M_M_Gardener_01","S_M_M_GenTransport","S_M_M_HairDress_01","S_M_M_HighSec_01","S_M_M_HighSec_02","S_M_M_INTWork","S_M_M_Janitor","S_M_M_LatHandy_01","S_M_M_LifeInvad_01","S_M_M_Linecook","S_M_M_LSMetro_01","S_M_M_Mariachi_01","S_M_M_Marine_01","S_M_M_Marine_02","S_M_M_Migrant_01","S_M_M_MovAlien_01","S_M_M_MovPrem_01","S_M_M_MovSpace_01","S_M_M_Paramedic_01","S_M_M_Pilot_01","S_M_M_Pilot_02","S_M_M_Postal_01","S_M_M_Postal_02","S_M_M_PrisGuard_01","S_M_M_Scientist_01","S_M_M_Security_01","S_M_M_SnowCop_01","S_M_M_StrPerf_01","S_M_M_StrPreach_01","S_M_M_StrVend_01","S_M_M_Trucker_01","S_M_M_UPS_01","S_M_M_UPS_02" };
	static char* MaleOldServiceModelsList[] = { "S_M_O_Busker_01","S_M_O_Snowcop_01" };
	static char* MaleYoungServiceModelsList[] = { "S_M_Y_AirWorker","S_M_Y_AmmuCity_01","S_M_Y_ArmyMech_01","S_M_Y_Autopsy_01","S_M_Y_Barman_01","S_M_Y_BayWatch_01","S_M_Y_BlackOps_01","S_M_Y_BlackOps_02","S_M_Y_BusBoy_01","S_M_Y_Chef_01","S_M_Y_Clown_01","S_M_Y_Construct_01","S_M_Y_Construct_02","S_M_Y_Cop_01","S_M_Y_Dealer_01","S_M_Y_DevinSec_01","S_M_Y_DockWork_01","S_M_Y_Doorman_01","S_M_Y_DWService_01","S_M_Y_DWService_02","S_M_Y_Factory_01","S_M_Y_Fireman_01","S_M_Y_Garbage","S_M_Y_Grip_01","S_M_Y_HwayCop_01","S_M_Y_Marine_01","S_M_Y_Marine_02","S_M_Y_Marine_03","S_M_Y_Mime","S_M_Y_PestCont_01","S_M_Y_Pilot_01","S_M_Y_PrisMuscl_01","S_M_Y_Prisoner_01","S_M_Y_Ranger_01","S_M_Y_Robber_01","S_M_Y_Scrubs_01","S_M_Y_Sheriff_01","S_M_Y_Shop_MASK","S_M_Y_StrVend_01","S_M_Y_Swat_01","S_M_Y_USCG_01","S_M_Y_Valet_01","S_M_Y_Waiter_01","S_M_Y_WinClean_01","S_M_Y_XMech_01","S_M_Y_XMech_02" };
	static char* FemaleMatureUniqueModelsList[] = { "U_F_M_Corpse_01","U_F_M_Miranda","U_F_M_ProMourn_01" };
	static char* FemaleOldUniqueModelsList[] = { "U_F_O_MovieStar", "U_F_O_ProlHost_01" };
	static char* FemaleYoungUniqueModelsList[] = { "U_F_Y_BikerChic","U_F_Y_COMJane","U_F_Y_corpse_01","U_F_Y_corpse_02","U_F_Y_HotPosh_01","U_F_Y_JewelAss_01","U_F_Y_Mistress","U_F_Y_PoppyMich","U_F_Y_Princess","U_F_Y_SpyActress" };
	static char* MaleMatureUniqueModelsList[] = { "U_M_M_Aldinapoli","U_M_M_BankMan","U_M_M_BikeHire_01","U_M_M_FIBArchitect","U_M_M_FilmDirector","U_M_M_GlenStank_01","U_M_M_Griff_01","U_M_M_Jesus_01","U_M_M_JewelSec_01","U_M_M_JewelThief","U_M_M_MarkFost","U_M_M_PartyTarget","U_M_M_ProlSec_01","U_M_M_ProMourn_01","U_M_M_RivalPap","U_M_M_SpyActor","U_M_M_WillyFist" };
	static char* MaleOldUniqueModelsList[] = { "U_M_O_FinGuru_01","U_M_O_TapHillBilly","U_M_O_Tramp_01" };
	static char* MaleYoungUniqueModelsList[] = { "U_M_Y_Abner","U_M_Y_AntonB","U_M_Y_BabyD","U_M_Y_Baygor","U_M_Y_BurgerDrug_01","U_M_Y_Chip","U_M_Y_Cyclist_01","U_M_Y_FIBMugger_01","U_M_Y_Guido_01","U_M_Y_GunVend_01","U_M_Y_Hippie_01","U_M_Y_ImpoRage","U_M_Y_Justin","U_M_Y_Mani","U_M_Y_MilitaryBum","U_M_Y_Paparazzi","U_M_Y_Party_01","U_M_Y_Pogo_01","U_M_Y_Prisoner_01","U_M_Y_ProlDriver_01","U_M_Y_RSRanger_01","U_M_Y_SBike","U_M_Y_StagGrm_01","U_M_Y_Tattoo_01","U_M_Y_Zombie_01" };

	static char *playerModels[] = { "Animals",  "Female Mature Models", "Female Old Models", "Female Young Models", "Male Mature Models", "Male Old Models", "Male Young Models", "Cutscene Models", "Female Young Gang Models", "Male Mature Gang Models", "Male Young Gang Models", "Heist Crew", "Various", "Female MP Models", "Female MP Gang Models", "Male MP Gang Models", "Male MP Models", "Story", "Female Mature Service Models", "Female Young Service Models", "Male Mature Service Models", "Male Old Service Models", "Male Young Service Models", "Female Mature Unique Models", "Female Old Unique Models", "Female Young Unique Models", "Male Mature Unique Models", "Male Old Unique Models", "Male Young Unique Models" };

	void SelectPlayerModelMenu() {
		if (modelCategoryName == "Animals") { // 1
			for (int i = 0; i < ARRAYSIZE(AnimalsList); i++) {
				AddModel(AnimalsList[i]);
			}
		}
		else if (modelCategoryName == "Female Mature Models") { // 2
			for (int i = 0; i < ARRAYSIZE(FemaleMatureModelsList); i++) {
				AddModel(FemaleMatureModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Old Models") { // 3
			for (int i = 0; i < ARRAYSIZE(FemaleOldModelsList); i++) {
				AddModel(FemaleOldModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Young Models") { // 4
			for (int i = 0; i < ARRAYSIZE(FemaleYoungModelsList); i++) {
				AddModel(FemaleYoungModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Mature Models") { // 5
			for (int i = 0; i < ARRAYSIZE(MaleMatureModelsList); i++) {
				AddModel(MaleMatureModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Old Models") { // 6
			for (int i = 0; i < ARRAYSIZE(MaleOldModelsList); i++) {
				AddModel(MaleOldModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Young Models") { // 7
			for (int i = 0; i < ARRAYSIZE(MaleYoungModelsList); i++) {
				AddModel(MaleYoungModelsList[i]);
			}
		}
		else if (modelCategoryName == "Cutscene Models") { // 8
			for (int i = 0; i < ARRAYSIZE(CutsceneModelsList); i++) {
				AddModel(CutsceneModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Young Gang Models") { // 9
			for (int i = 0; i < ARRAYSIZE(FemaleYoungGangModelsList); i++) {
				AddModel(FemaleYoungGangModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Mature Gang Models") { // 10
			for (int i = 0; i < ARRAYSIZE(MaleMatureGangModelsList); i++) {
				AddModel(MaleMatureGangModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Young Gang Models") { // 11
			for (int i = 0; i < ARRAYSIZE(MaleYoungGangModelsList); i++) {
				AddModel(MaleYoungGangModelsList[i]);
			}
		}
		else if (modelCategoryName == "Heist Crew Models") { // 12
			for (int i = 0; i < ARRAYSIZE(HeistCrewModelsList); i++) {
				AddModel(HeistCrewModelsList[i]);
			}
		}
		else if (modelCategoryName == "Various Models") { // 13
			for (int i = 0; i < ARRAYSIZE(VariousModelsList); i++) {
				AddModel(VariousModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Multiplayer Models") { // 14
			for (int i = 0; i < ARRAYSIZE(FemaleMultiplayerModelsList); i++) {
				AddModel(FemaleMultiplayerModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Multiplayer Gang Models") { // 15
			for (int i = 0; i < ARRAYSIZE(FemaleMultiplayerGangModelsList); i++) {
				AddModel(FemaleMultiplayerGangModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Multiplayer Gang Models") { // 16
			for (int i = 0; i < ARRAYSIZE(MaleMultiplayerGangModelsList); i++) {
				AddModel(MaleMultiplayerGangModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Multiplayer") { // 17
			for (int i = 0; i < ARRAYSIZE(MaleMultiplayerModels); i++) {
				AddModel(MaleMultiplayerModels[i]);
			}
		}
		else if (modelCategoryName == "Story Models") { // 18
			for (int i = 0; i < ARRAYSIZE(StoryModelsList); i++) {
				AddModel(StoryModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Mature Service Models") { // 19
			for (int i = 0; i < ARRAYSIZE(FemaleMatureServiceModelsList); i++) {
				AddModel(FemaleMatureServiceModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Mature Service Models") { // 20
			for (int i = 0; i < ARRAYSIZE(MaleMatureServiceModelsList); i++) {
				AddModel(MaleMatureServiceModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Old Service Models") { // 21
			for (int i = 0; i < ARRAYSIZE(MaleOldServiceModelsList); i++) {
				AddModel(MaleOldServiceModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Young Service Models") { // 22
			for (int i = 0; i < ARRAYSIZE(MaleYoungServiceModelsList); i++) {
				AddModel(MaleYoungServiceModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Mature Unique Models") { // 23
			for (int i = 0; i < ARRAYSIZE(FemaleMatureUniqueModelsList); i++) {
				AddModel(FemaleMatureUniqueModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Old Unique Models") { // 24
			for (int i = 0; i < ARRAYSIZE(FemaleOldUniqueModelsList); i++) {
				AddModel(FemaleOldUniqueModelsList[i]);
			}
		}
		else if (modelCategoryName == "Female Young Unique Models") { // 25
			for (int i = 0; i < ARRAYSIZE(FemaleYoungUniqueModelsList); i++) {
				AddModel(FemaleYoungUniqueModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Mature Unique Models") { // 26
			for (int i = 0; i < ARRAYSIZE(MaleMatureUniqueModelsList); i++) {
				AddModel(MaleMatureUniqueModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Old Unique Models") { // 27
			for (int i = 0; i < ARRAYSIZE(MaleOldUniqueModelsList); i++) {
				AddModel(MaleOldUniqueModelsList[i]);
			}
		}
		else if (modelCategoryName == "Male Young Unique Models") { // 28
			for (int i = 0; i < ARRAYSIZE(MaleYoungUniqueModelsList); i++) {
				AddModel(MaleYoungUniqueModelsList[i]);
			}
		}
	}

	void OutfitsMenu() {
		AddTitle("Outfit Changer");
		
		}
	void ModelChangerMenu() {
		AddTitle("Model Changer");
		for (int i = 0; i < ARRAYSIZE(playerModels); i++) {
			AddModelCategory(playerModels[i], null, nullFunc, SUB::SELECT_PLAYER_MODEL, playerModels[i]);
		}
	}

	void SelfScenariosMenu() {
		bool a = 0, a0 = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, o = 0, p = 0, q = 0, r = 0, s = 0, t = 0, u = 0, v = 0;

		AddTitle("SCENARIOS");
		AddOption("Stop Scenario", a0);
		AddOption("Musician", f);
		AddOption("Welding", p);
		AddOption("Lifting Weights", d);
		AddOption("Gardener Leaf Blower", a);
		AddOption("Binoculars", b);
		AddOption("Bum Freeway", c);
		AddOption("High Class Prostitute", e);
		AddOption("Smoke Break", g);
		AddOption("Hammering", h);
		AddOption("Maid Clean", i);
		AddOption("Paparazzi", j);
		AddOption("Strip Watch", k);
		AddOption("Partying", l);
		AddOption("Smoking Pot", m);
		AddOption("Stupor", n);
		AddOption("Sumbathe", o);
		AddOption("Tourist Mobile", q);
		AddOption("Human Statue", r);


		// Options' code here:
		if (r)
		{

			char *anim = "WORLD_HUMAN_HUMAN_STATUE";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (q)
		{

			char *anim = "WORLD_HUMAN_TOURIST_MOBILE";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (p)
		{

			char *anim = "WORLD_HUMAN_WELDING";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (o)
		{

			char *anim = "WORLD_HUMAN_SUNBATHE";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (n)
		{

			char *anim = "WORLD_HUMAN_STUPOR";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (m)
		{

			char *anim = "WORLD_HUMAN_SMOKING_POT";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (l)
		{

			char *anim = "WORLD_HUMAN_PARTYING";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (k)
		{

			char *anim = "PROP_HUMAN_SEAT_STRIP_WATCH";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (j)
		{

			char *anim = "WORLD_HUMAN_PAPARAZZI";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (i)
		{

			char *anim = "WORLD_HUMAN_MAID_CLEAN";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (h)
		{

			char *anim = "WORLD_HUMAN_HAMMERING";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (a0)
		{
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
		}
		if (a)
		{

			char *anim = "WORLD_HUMAN_GARDENER_LEAF_BLOWER";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (b)
		{

			char *anim = "WORLD_HUMAN_BINOCULARS";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (c)
		{

			char *anim = "WORLD_HUMAN_BUM_FREEWAY";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (d)
		{

			char *anim = "WORLD_HUMAN_MUSCLE_FREE_WEIGHTS";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (e)
		{

			char *anim = "WORLD_HUMAN_PROSTITUTE_HIGH_CLASS";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (f)
		{

			char *anim = "WORLD_HUMAN_MUSICIAN";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}
		if (g)
		{

			char *anim = "WORLD_HUMAN_AA_SMOKE";
			AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
			AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);
		}

	}

	void StopAnimation() {
		AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	}
	void SelfAnimationsMenu()

	{

		//variables
		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, 1);


		// Options' text here:
		AddTitle("Self Animations");
		AddOption("Stop Animation", null, StopAnimation);

		AddAnimation("Sexy Dance", "mini@strip_club@lap_dance_2g@ld_2g_p2", "ld_2g_p2_s2");
		AddAnimation("MissChinese Broll", "misschinese2_barrelroll", "barrel_roll_loop_A");
		AddAnimation("Shower", "mp_safehouseshower@male@", "male_shower_idle_b");
		AddAnimation("Push Ups", "amb@world_human_push_ups@male@base", "base");
		AddAnimation("Sit Ups", "amb@world_human_sit_ups@male@base", "base");
		AddAnimation("Pull Ups", "SWITCH@FRANKLIN@GYM", "001942_02_GC_FRAS_IG_5_BASE");
		AddAnimation("Meditation", "rcmcollect_paperleadinout@", "meditiate_idle");
		AddAnimation("Car Sex", "ODDJOBS@ASSASSINATE@VICE@SEX", "frontseat_carsex_loop_f");
		AddAnimation("Arrest", "mp_bank_heist_1", "m_cower_02");
		AddAnimation("Sex Give", "rcmpaparazzo_2", "shag_loop_a");
		AddAnimation("Sex Take", "rcmpaparazzo_2", "shag_loop_poppy");
		AddAnimation("Stripper", "mini@strip_club@idles@stripper", "stripper_idle_04");
		AddAnimation("Pole Dance", "mini@strip_club@pole_dance@pole_dance2", "base");
		AddAnimation("Pee", "missbigscore1switch_trevor_piss", "piss_loop");
		AddAnimation("Parachute Loop", "missexile1_cargoplanejumpout", "jump_launch_l_to_skydive");
		AddAnimation("Sleep", "mp_sleep", "sleep_loop");
		AddAnimation("Suicide", "mp_suicide", "pistol");
		AddAnimation("Hacking", "mp_shipment_steal", "hack_loop");
		AddAnimation("WTF", "mini@triathlon", "wot_the_fuck");

	}
	char *isThisPlayerHost(int i) {
		char *freemd = "Freemode";
		int Host = NETWORK::NETWORK_GET_HOST_OF_SCRIPT(freemd, -1, 0);
		if (Host == i)
		{
			return " ~g~[HOST]";
		}
		else {
			return "";
		}
	}

	void OnlinePlayersMenu()
	{
		AddTitle("SELECT PLAYER");

		for (int player = 0; player <= 32; player++) {
			if (strcmp("**Invalid**", PLAYER::GET_PLAYER_NAME(player)) != 0) {
				selectedPlayerNumber = player;
				std::string playerName = PLAYER::GET_PLAYER_NAME(player);
				std::string hostText = isThisPlayerHost(player);
				std::string finalPlayerText = playerName + hostText;

				AddPlayer(StringToChar(finalPlayerText), selectedPlayerNumber, null, UpdateSelectedPlayer, SUB::ONLINE_PLAYER_ACTIONS);
			}
		}
	}

	void GiveAllWeaponsToEveryone() {
		for (int i = 0; i <= 32; i++)
		{
			WAIT(0);
			if (i == playerID)continue;
			int Handle = PLAYER::GET_PLAYER_PED(i);


			for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(Handle, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 9999, 9999);
			WAIT(100);
		}
		PrintBottomLeft("~g~Weapons Given To All Players");
	}

	void RemoveAllWeaponsToEveryone() {
		for (int i = 0; i <= 32; i++)
		{
			WAIT(0);
			if (i == playerID)continue;
			int Handle = PLAYER::GET_PLAYER_PED(i);
			WEAPON::REMOVE_ALL_PED_WEAPONS(Handle, 1);
			{
				if (i == 32)
				{
					break;
				}

			}
		}
		PrintBottomLeft("Weapons Removed");
	}

	void AllOnlinePlayersMenu()
	{
		AddTitle("ALL ONLINE PLAYERS");
		//if (isPaidVersion)AddOption("Explode Lobby", null, ExplodeLobby);
		//if (isPaidVersion)AddOption("Silent Explode Lobby", null, SilentExplodeLobby);
		if (isPaidVersion) AddOption("Teleport All To Me", null, TeleportAllToMe);
		AddOption("Give All Weapons", null, GiveAllWeaponsToEveryone);
		if (isPaidVersion) AddOption("Remove All Weapons", null, RemoveAllWeaponsToEveryone);
		if (isPaidVersion || isTesterVersion) AddOption("Send Attackers", null, nullFunc, SUB::ALL_ONLINE_ATTACKERS);
		if (isPaidVersion || isTesterVersion) AddOption("Attach Object", null, nullFunc, SUB::ATTACH_OBJECT_TO_ALL_PLAYERS);
	}

	void LobbyAttackersMenu() {
		AddTitle("Select Attacker");
		AddToggle("Attackers God Mode", attackersGodMode);
		AddAttacker("Send Jesus with Minigun", PLAYER::GET_PLAYER_PED(selectedPlayer), "u_m_m_jesus_01", attackersGodMode, "WEAPON_MINIGUN", true);
		AddAttacker("Cop with Stungun", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Cop_01", attackersGodMode, "WEAPON_STUNGUN", true);
		AddAttacker("Marine with RPG", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Marine_01", attackersGodMode, "WEAPON_RPG", true);
		AddAttacker("Alien with Railgun ~y~(Strong!)", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_M_MovAlien_01", attackersGodMode, "WEAPON_RAILGUN", true);
		AddAttacker("Snipers", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Marine_01", attackersGodMode, "WEAPON_HEAVYSNIPER", true);
		AddAttacker("Soldiers", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Marine_01", attackersGodMode, "WEAPON_ASSAULTRIFLE", true);
	}

	void AttackersMenu() {
		AddTitle("Select Attacker");
		if (isPaidVersion || isTesterVersion) AddToggle("Attackers God Mode", attackersGodMode);
		AddAttacker("Cop with Stungun", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Cop_01", attackersGodMode, "WEAPON_STUNGUN", false);
		AddAttacker("Marine with RPG", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Marine_01", attackersGodMode, "WEAPON_RPG", false);
		AddAttacker("Alien with Railgun ~y~(Strong!)", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_M_MovAlien_01", attackersGodMode, "WEAPON_RAILGUN", false);
		AddAttacker("Snipers", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Marine_01", attackersGodMode, "WEAPON_HEAVYSNIPER", false);
		AddAttacker("Soldiers", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Marine_01", attackersGodMode, "WEAPON_ASSAULTRIFLE", false);
		AddAttacker("Send Jesus with Minigun", PLAYER::GET_PLAYER_PED(selectedPlayer), "u_m_m_jesus_01", attackersGodMode, "WEAPON_MINIGUN", false);
	}

	int GET_FREE_SEAT(int VehicleHandle)
	{
		int numSeats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(VehicleHandle);
		int SeatIndex = -1;  //Driver seat = -1
		while (SeatIndex < numSeats)
		{
			if (VEHICLE::IS_VEHICLE_SEAT_FREE(VehicleHandle, SeatIndex))
				return SeatIndex;
			SeatIndex++;
		}
		return -2;
	}
	void TeleportInPlayerVehicle() {
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(selectedPlayer), true))
		{
			Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
			int VehicleHandle = PED::GET_VEHICLE_PED_IS_USING(victim);
			PED::SET_PED_INTO_VEHICLE(playerPed, VehicleHandle, GET_FREE_SEAT(VehicleHandle));
		}
		else
		{
			PrintBottomLeft("ERROR: Player is not in a Vehicle");
		}

	}
	void SlingshotVehicle() {
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(selectedPlayer), true)) {
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(selectedPlayer));
			RequestControlOfEnt(playerVeh);
			ENTITY::APPLY_FORCE_TO_ENTITY(playerVeh, 0, 0.0f, 9999999.0f, 9999999.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 0, 0, 1);
			RequestControlOfEnt(playerVeh);
			ENTITY::APPLY_FORCE_TO_ENTITY(playerVeh, 0, 0.0f, 9999999.0f, 9999999.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 0, 0, 1);
			RequestControlOfEnt(playerVeh);
			ENTITY::APPLY_FORCE_TO_ENTITY(playerVeh, 0, 0.0f, 9999999.0f, 9999999.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 0, 0, 1);
		}
		else {
			PrintBottomLeft("~r~ERROR: Player is not in a vehicle");
		}
	}
	void RemoveOwnership()
	{
		Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(selectedPlayer));
		RequestControlOfEnt(playerVeh);
		if (DECORATOR::DECOR_EXIST_ON(playerVeh, "Player_Vehicle"))
		{
			DECORATOR::DECOR_REMOVE(playerVeh, "Player_Vehicle");
			PrintStringBottomCentre("~g~Removed Ownership Of Vehicle");
		}
		else
		{
			PrintStringBottomCentre("~r~ERROR: This player does not own the vehicle!");
		}

	}
	void DeleteVehicle() {
		Ped victimPed = PLAYER::GET_PLAYER_PED(selectedPlayer);
		if (PED::IS_PED_IN_ANY_VEHICLE(victimPed, true))
		{
			Vehicle victimVeh = PED::GET_VEHICLE_PED_IS_IN(selectedPlayer, 0);
			RequestControlOfVehicle(victimVeh);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(victimVeh, 1, 1);
			ENTITY::DELETE_ENTITY(&victimVeh);
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&victimVeh);
		}
		else {
			PrintBottomLeft("~r~ERROR: Player is not in a vehicle");
		}
	}
	void TeleportVictimVehicleToMe() {
		Vector3 myPos = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		Ped victimPed = PLAYER::GET_PLAYER_PED(selectedPlayer);
		if (PED::IS_PED_IN_ANY_VEHICLE(victimPed, true))
		{

			Vehicle victimVeh = PED::GET_VEHICLE_PED_IS_IN(selectedPlayer, 0);
			RequestControlOfVehicle(victimVeh);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(victimVeh, myPos.x, myPos.y, myPos.z, 1, 0, 0);
		}
		else {
			PrintBottomLeft("~r~ERROR: Player is not in a vehicle");
		}

	}

	void HijackVehicle() {
		Ped victim = PLAYER::GET_PLAYER_PED(selectedPlayer);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(selectedPlayer), true)) {
			Vehicle victimVeh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(selectedPlayer));

			AI::CLEAR_PED_TASKS_IMMEDIATELY(victim);
			PED::SET_PED_INTO_VEHICLE(playerPed, victimVeh, -1);
			PrintStringBottomCentre("~g~Vehicle Hijacked!");
		}
		else {
			PrintBottomLeft("~r~ERROR: Player is not in a vehicle");
		}
	}
	void PlayerVehicleActions() {
		AddTitle("VEHICLE");
		AddOption("Teleport in his Vehicle", null, TeleportInPlayerVehicle);
		AddOption("Slingshot Vehicle", null, SlingshotVehicle);
		if (isPaidVersion) AddOption("Delete Vehicle", null, DeleteVehicle);
		if (isPaidVersion) AddOption("Teleport Vehicle to me", null, TeleportVictimVehicleToMe);
		if (isPaidVersion) AddOption("Hijack Vehicle", null, HijackVehicle);
		if (isPaidVersion) AddOption("Remove Ownership", null, RemoveOwnership);
	}
	void ClonePed() {
		Ped clonedPed = PED::CLONE_PED(PLAYER::GET_PLAYER_PED(selectedPlayer), ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED(selectedPlayer)), 1, 1);
	}

	void ClonePedAsBodyguard() {
		int playerGroup = PLAYER::GET_PLAYER_GROUP(selectedPlayer);
		Ped clonedPed = PED::CLONE_PED(PLAYER::GET_PLAYER_PED(selectedPlayer), ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED(selectedPlayer)), 1, 1);
		PED::SET_PED_AS_GROUP_LEADER(PLAYER::GET_PLAYER_PED(selectedPlayer), playerGroup);
		PED::SET_PED_AS_GROUP_MEMBER(clonedPed, playerGroup);
		PED::SET_PED_NEVER_LEAVES_GROUP(clonedPed, playerGroup);
		ENTITY::SET_ENTITY_INVINCIBLE(clonedPed, true);
		PED::SET_PED_COMBAT_ABILITY(clonedPed, 100);
		WEAPON::GIVE_WEAPON_TO_PED(clonedPed, GAMEPLAY::GET_HASH_KEY("WEAPON_RPG"), 9999, false, 9999);
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(clonedPed, true);
		PED::SET_PED_CAN_SWITCH_WEAPON(clonedPed, true);
	}

	void CloneAsAttacker() {
		Ped clonedPed = PED::CLONE_PED(PLAYER::GET_PLAYER_PED(selectedPlayer), ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED(selectedPlayer)), 1, 1);
		PED::SET_PED_ACCURACY(clonedPed, 100);
		PED::SET_PED_COMBAT_ABILITY(clonedPed, 100);
		PED::SET_PED_ALERTNESS(clonedPed, 100);
		PED::SET_PED_AS_ENEMY(clonedPed, true);
		PED::SET_PED_CAN_SWITCH_WEAPON(clonedPed, true);
		PED::SET_DRIVER_ABILITY(clonedPed, 1.0);
		PED::SET_PED_STEERS_AROUND_OBJECTS(clonedPed, true);
		PED::SET_PED_STEERS_AROUND_PEDS(clonedPed, true);
		PED::SET_PED_STEERS_AROUND_VEHICLES(clonedPed, true);
		WEAPON::GIVE_WEAPON_TO_PED(clonedPed, GAMEPLAY::GET_HASH_KEY("WEAPON_RPG"), 9999, false, 9999);
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(clonedPed, true);
		Blip Blip = UI::ADD_BLIP_FOR_ENTITY(clonedPed);
		UI::SET_BLIP_SCALE(clonedPed, 1);
		PED::SET_PED_FIRING_PATTERN(clonedPed, GAMEPLAY::GET_HASH_KEY("firing_pattern_full_auto"));
	}


	void PlayerPedActions() {
		AddTitle("PED OPTIONS");
		AddOption("Clone Ped", null, ClonePed);
		AddOption("Clone Ped As Bodyguard", null, ClonePedAsBodyguard);
		if (isPaidVersion) AddToggle("Attackers GodMode", attackersGodMode);
		if (isPaidVersion) AddAttacker("Send Cop with Railgun", PLAYER::GET_PLAYER_PED(selectedPlayer), "s_m_y_cop_01", attackersGodMode, "WEAPON_RAILGUN");
		if (isPaidVersion) AddAttacker("Send Jesus with Minigun", PLAYER::GET_PLAYER_PED(selectedPlayer), "u_m_m_jesus_01", attackersGodMode, "WEAPON_MINIGUN");
		if (isPaidVersion) AddAttacker("Send Pogo with Stungun", PLAYER::GET_PLAYER_PED(selectedPlayer), "U_M_Y_pogo_01", attackersGodMode, "WEAPON_STUNGUN");
		if (isPaidVersion) AddAttacker("Marine with RPG", PLAYER::GET_PLAYER_PED(selectedPlayer), "S_M_Y_Marine_03", attackersGodMode, "WEAPON_RPG");
	}
	static bool meAttachedToAPlayer = false;
	static int attachedPlayer;

	void AttachToPlayer() {
		meAttachedToAPlayer = !meAttachedToAPlayer;
		attachedPlayer = selectedPlayer;

		if (meAttachedToAPlayer) {
			ENTITY::ATTACH_ENTITY_TO_ENTITY(playerPed, PLAYER::GET_PLAYER_PED(selectedPlayer), 0.059999998658895f, 0.0f, -0.25f, 0.0f, 0.0f, 0.0f, 1, 1, 0, 0, 2, 1, 1);
		}
		else {
			ENTITY::DETACH_ENTITY(playerPed, 1, 1);
		}
	}


	void StopAllPedMoneyDrops() {
		for (int i = 0; i < 32; i++) {
			pedMoneyDropOnPlayer[i] = false;
		}
		PrintBottomLeft("~b~Ped Drop Stopped For Everyone!");
	}
	static bool delayToggle[32];
	static bool pedMoneyDropToggle = false;
	static bool moneyDropToggle = false;

	void UnsafeThingsMenu() {
		AddTitle("Money Drop");
		AddBreak("~g~This IS safe!");
		AddToggle("Ped Drop V3", pedMoneyDropOnPlayer[selectedPlayer]);
		AddIntEasy("Delay", pedMoneyDropDelay, pedMoneyDropDelay, 1, 1, pedMoneyDropToggle);
		AddIntEasy("Amount", pedMoneyDropAmount, pedMoneyDropAmount, 1, 0, null, true, 10000, 0);
		AddToggle("Invisible Peds", isPedDropInvisible);

		AddOption("~b~Stop Ped Drop For Everyone", null, StopAllPedMoneyDrops);

	}
	Object AttachObjectToSelectedPlayer(char *objectToAttach, bool collisions = false, bool invisible = true) {
		Ped Handle = PLAYER::GET_PLAYER_PED(selectedPlayer);
		Hash obj = GAMEPLAY::GET_HASH_KEY(objectToAttach);
		STREAMING::REQUEST_MODEL(obj);
		while (!STREAMING::HAS_MODEL_LOADED(obj)) {
			WAIT(0);
		}
		Object Obj = OBJECT::CREATE_OBJECT(obj, 0, 0, 0, true, 1, 0);
		if (invisible) {
			ENTITY::SET_ENTITY_VISIBLE(obj, false, 0);
		}
		ENTITY::ATTACH_ENTITY_TO_ENTITY(Obj, Handle, 0, 0, 0, 0, 0, 0, 0, 1, 1, collisions, 0, 2, 1);

		return Obj;
	}
	static bool isFreezingHim[32];
	static void FreezeFreezedPlayers() {
		for (int i = 0; i <= 32; i++) {
			if (isFreezingHim[i]) {
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED(i));
			}
		}
	}
	void GiveSemiGodMode() {
		AttachObjectToSelectedPlayer("p_v_43_safe_s", false, true);
		PrintBottomLeft("~g~Done!");
	}



	static bool isSpectating[32];

	static bool isSpammed[32];

	static Vehicle spawnedVehicleCustom;
	static bool vehicleSpawnRequestCustom;

	static char *customVehicleName;
	static float vehicleCustomX, vehicleCustomY, vehicleCustomZ;
	static bool vehicleCustomGodMode, vehicleCustomMaxUpgrade, vehicleCustomVisible, vehicleCustomSetMeIntoVehicle, vehicleCustomRandomColors;

	static Vehicle SpawnVehicleCustom(char* vehicleName, float x, float y, float z, bool godMode, bool maxUpgrade, bool visible, bool setMeIntoVehicle = false, bool randomColors = false) {

		Hash VehicleHash = GAMEPLAY::GET_HASH_KEY(vehicleName);
		STREAMING::REQUEST_MODEL(VehicleHash);
		if (STREAMING::HAS_MODEL_LOADED(VehicleHash))
		{
			spawnedVehicleCustom = VEHICLE::CREATE_VEHICLE(VehicleHash, x, y, z, 0, 1, 0);
			BypassOnlineVehicleKick(spawnedVehicleCustom);
			if (godMode) {
				ENTITY::SET_ENTITY_INVINCIBLE(spawnedVehicleCustom, true);
			}
			if (maxUpgrade) {
				MaxThisVehicle(spawnedVehicleCustom);
			}
			if (!visible) {
				ENTITY::SET_ENTITY_VISIBLE(spawnedVehicleCustom, false, 0);
				ENTITY::SET_ENTITY_ALPHA(spawnedVehicleCustom, 0, 0);
			}
			if (setMeIntoVehicle) {
				PED::SET_PED_INTO_VEHICLE(playerPed, spawnedVehicleCustom, -1);
			}
			if (randomColors) {
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(spawnedVehicleCustom, RandomRGB(), RandomRGB(), RandomRGB());
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(spawnedVehicleCustom, RandomRGB(), RandomRGB(), RandomRGB());
			}

		}
		if (spawnedVehicleCustom != NULL && ENTITY::DOES_ENTITY_EXIST(spawnedVehicleCustom)) {
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(VehicleHash);
			customVehicleName = vehicleName, vehicleCustomX = x; vehicleCustomY = y; vehicleCustomZ = z; vehicleCustomGodMode = godMode; vehicleCustomMaxUpgrade = maxUpgrade; vehicleCustomVisible = visible; vehicleCustomSetMeIntoVehicle = setMeIntoVehicle; vehicleCustomRandomColors = randomColors;
			vehicleSpawnRequestCustom = false;
			return spawnedVehicleCustom;
		}
	}
	static void EntitySpam(char* vehicleName) {
		for (int i = 0; i <= 32; i++) {
			if (isSpammed[i]) {
				Vector3 victim = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(i), true);
				SpawnVehicleCustom(vehicleName, victim.x, victim.y, victim.z, false, false, true, false, true);
			}
		}
	}

	void ParticleFxMenu() {
		AddTitle("Particle FX");
		AddParticleFx("Clown Appears", "scr_rcbarry2", "scr_clown_appears", selectedPlayer);
		AddParticleFx("Alien Teleport", "scr_rcbarry1", "scr_alien_teleport", selectedPlayer);
		AddParticleFx("Smoke ~y~(Huge)", "scr_mp_creator", "scr_mp_plane_landing_tyre_smoke", selectedPlayer);
		AddParticleFx("Blood ~y~(Huge)", "scr_michael2", "scr_abattoir_ped_sliced", selectedPlayer);
		AddParticleFx("Sparks", "scr_trevor1", "scr_trev1_trailer_boosh", selectedPlayer);
		AddParticleFx("Alien Disintegrate", "scr_rcbarry1", "scr_alien_disintegrate", selectedPlayer);
		AddParticleFx("Money", "scr_paletoscore", "scr_paleto_banknotes", selectedPlayer);
		AddParticleFx("Camera Flash", "scr_rcpaparazzo1", "scr_rcpap1_camera", selectedPlayer);
	}

	void SelfPtfxMenu() {
		AddTitle("~y~Particle FX");
		AddParticleFx("Clown Appears", "scr_rcbarry2", "scr_clown_appears", playerID);
		AddParticleFx("Alien Teleport", "scr_rcbarry1", "scr_alien_teleport", playerID);
		AddParticleFx("Smoke ~y~(Huge)", "scr_mp_creator", "scr_mp_plane_landing_tyre_smoke", playerID);
		AddParticleFx("Blood ~y~(Huge)", "scr_michael2", "scr_abattoir_ped_sliced", playerID);
		AddParticleFx("Sparks", "scr_trevor1", "scr_trev1_trailer_boosh", playerID);
		AddParticleFx("Alien Disintegrate", "scr_rcbarry1", "scr_alien_disintegrate", playerID);
		AddParticleFx("Money", "scr_paletoscore", "scr_paleto_banknotes", playerID);
		AddParticleFx("Camera Flash", "scr_rcpaparazzo1", "scr_rcpap1_camera", playerID);
	}

	void OnlinePlayerActions() {
		AddTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		AddOption("Player Options", null, nullFunc, SUB::PLAYER_GRIEFING);
		AddOption("Vehicle Options", null, nullFunc, SUB::PLAYER_VEHICLE);
		AddOption("Weapon Options", null, nullFunc, SUB::PLAYER_WEAPONS);
		AddOption("Ped Options", null, nullFunc, SUB::PLAYER_PED_OPTIONS);
		AddOption("Money Drop", null, nullFunc, SUB::PLAYER_UNSAFE_THINGS);
		if (isPaidVersion) AddOption("Animation Options", null, nullFunc, SUB::PLAYER_DOES_ANIM);
		AddOption("Teleport Options", null, nullFunc, SUB::PLAYER_TELEPORT_THINGS);
		if (isPaidVersion) AddOption("Attach Objects", null, nullFunc, SUB::ATTACH_PLAYER_THINGS);
		if (isPaidVersion) AddOption("Entity Spam", null, nullFunc, SUB::ENTITY_SPAM_MENU);
		AddOption("ParticleFX", null, nullFunc, SUB::PARTICLE_FX_MENU);
		AddOption("Send attackers", null, nullFunc, SUB::PLAYER_SEND_ATTACKERS);
		if (selectedPlayer != playerID) {
			if (meAttachedToAPlayer == false) {
				AddOption("Attach To Player", null, AttachToPlayer);
			}
			else if (meAttachedToAPlayer == true && attachedPlayer == selectedPlayer) {
				AddOption("Detach From Player", null, AttachToPlayer);
			}
		}
		AddSpectate("~g~Spectate Player", isSpectating[32], selectedPlayer);
	}

	void GriefingMenu() {
		AddTitle("Player Options");
		AddOption("~g~Give Semi GodMode", null, GiveSemiGodMode);
		if (isPaidVersion) AddToggle("~r~Freeze Player", isFreezingHim[selectedPlayer]);
		// AddOption("~r~Explode Player", null, ExplodePlayer);
		//if (isPaidVersion) AddOption("~r~Silent Explode", null, SilentExplode);
		AddOption("~y~Cage Player", null, CagePlayer);
		if (isPaidVersion || isTesterVersion) AddOption("~r~Kill Passive Player", null, KillPassivePlayer);
		AddOption("~y~Kick Player ~r~[HOST ONLY]", null, KickPlayer);
		if (isPaidVersion) AddOption("~r~INSTANT CRASH", null, InstantCrash);
	}

	void PlayerDoesAnimMenu() {
		AddTitle("Animations");
		AddOption("Stop Animation", null, StopPlayerAnim);
		AddOption("Dance", null, PlayerDoesAnim);
	}


	char* entitySpamVehicleName;
	void EntitySpamMenu() {
		AddTitle("Entity Spammer");
		//			OPTION TITLE	var that contains the vehicle name           |           vehicleName (if pressed, this will become the current entity which will be spawned)
		AddToggle("~r~Entity Spam", isSpammed[selectedPlayer]);
		AddCharOption("~y~Cargoplane", entitySpamVehicleName, "cargoplane"); // <------------------------------------|
		AddCharOption("~y~Rhino", entitySpamVehicleName, "rhino");
		AddCharOption("~y~Dump", entitySpamVehicleName, "dump");
		AddCharOption("~y~Blimp", entitySpamVehicleName, "blimp");
		AddCharOption("Buzzard", entitySpamVehicleName, "buzzard");
		AddCharOption("Spacedocker", entitySpamVehicleName, "dune2");
		AddCharOption("Motorcycle", entitySpamVehicleName, "pcj");
		AddCharOption("Fighter Jet", entitySpamVehicleName, "lazer");
		AddCharOption("Adder", entitySpamVehicleName, "adder");
		AddCharOption("T20", entitySpamVehicleName, "t20");
		AddCharOption("Towtruck", entitySpamVehicleName, "towtruck");
	}

	void PlayerTeleportThingsMenu() {
		AddTitle("Teleport Options");
		AddOption("Teleport To Player", null, TeleportToPlayer);
		if (isPaidVersion || isTesterVersion) AddOption("Teleport Player To Me", null, TeleportPlayerToMe);
		if (isPaidVersion || isTesterVersion) AddOption("Teleport Player To Waypoint", null, TeleportPlayerToMarker);
		if (isPaidVersion || isTesterVersion) AddOption("Teleport Player To Ocean", null, TeleportPlayerToOcean);
	}

	void WeaponsMenu2() {
		AddTitle("Weapon Options");
		AddOption("~b~Give All Weapons", null, GiveAllWeapons);
		if (isPaidVersion || isTesterVersion) AddOption("~r~Remove All Weapons", null, RemoveAllWeapons);
	}

	static bool objectCollision = 0;
	static char *objectToAttach;
	void AttachObjectToAll() {
		for (int i = 0; i <= 32; i++)
		{
			WAIT(0);
			if (i == playerID)continue;
			int Handle = PLAYER::GET_PLAYER_PED(i);
			Hash oball = GAMEPLAY::GET_HASH_KEY(objectToAttach);
			STREAMING::REQUEST_MODEL(oball);
			while (!STREAMING::HAS_MODEL_LOADED(oball)) {
				WAIT(0);
			}
			int orangeball = OBJECT::CREATE_OBJECT(oball, 0, 0, 0, true, 1, 0);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(orangeball, Handle, 0, 0, 0, 0, 0, 0, 0, 1, 1, objectCollision, 0, 2, 1);
		}
	}

	void AttachObjectToPlayer() {
		WAIT(0);
		int Handle = PLAYER::GET_PLAYER_PED(selectedPlayer);
		Hash oball = GAMEPLAY::GET_HASH_KEY(objectToAttach);
		STREAMING::REQUEST_MODEL(oball);
		while (!STREAMING::HAS_MODEL_LOADED(oball)) {
			WAIT(0);
		}
		int orangeball = OBJECT::CREATE_OBJECT(oball, 0, 0, 0, true, 1, 0);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(orangeball, Handle, 0, 0, 0, 0, 0, 0, 0, 1, 1, objectCollision, 0, 2, 1);
	}

	void AttachObjectToAllMenu() {
		AddTitle("Select Object");
		AddToggle("~y~Collisions", objectCollision);
		objectToAttach = "prop_windmill_01";
		AddOption("Windmill", null, AttachObjectToAll);
		objectToAttach = "prop_Ld_ferris_wheel";
		AddOption("Ferris Wheel", null, AttachObjectToAll);
		objectToAttach = "prop_beach_fire";
		AddOption("Beach Fire", null, AttachObjectToAll);
		objectToAttach = "stt_prop_hoop_tyre_01a";
		AddOption("Something Annoying", null, AttachObjectToAll);
	}

	void AttachObjectToPlayerMenu() {
		AddTitle("Select Object");
		AddToggle("~y~Collisions", objectCollision);

		AddOption("Bank Safe", null, AttachObjectToPlayer);
		objectToAttach = "prop_windmill_01";

		AddOption("Windmill", null, AttachObjectToPlayer);
		objectToAttach = "prop_Ld_ferris_wheel";

		AddOption("Ferris Wheel", null, AttachObjectToPlayer);
		objectToAttach = "prop_beach_fire";

		AddOption("Beach Fire", null, AttachObjectToPlayer);
		objectToAttach = "prop_container_01a";

		AddOption("Container", null, AttachObjectToPlayer);
		objectToAttach = "Prop_weed_01";

		AddOption("Weed", null, AttachObjectToPlayer);
		objectToAttach = "prop_large_gold";

		AddOption("Gold", null, AttachObjectToPlayer);
		objectToAttach = "hei_prop_heist_emp";

		AddOption("Glowing EMP", null, AttachObjectToPlayer);
		objectToAttach = "prop_air_bigradar";

		AddOption("Big Radar", null, AttachObjectToPlayer);
		objectToAttach = "prop_atm_01";

		AddOption("ATM Machine", null, AttachObjectToPlayer);
		objectToAttach = "p_v_43_safe_s";
	}

	// Sappi che sto usando un casino le tue funzioni come template per creare le mie, spero di imparare meglio presto.

	void FreezeMyVehicle() {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			Vehicle myVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::FREEZE_ENTITY_POSITION(myVeh, true);
		}
	}
	void UnfreezeMyVehicle() {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			Vehicle myVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::FREEZE_ENTITY_POSITION(myVeh, false);
		}
	}

	void OpenAllDoors() {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			Vehicle myVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			for (int i = 0; i < 10; i++) {
				VEHICLE::SET_VEHICLE_DOOR_OPEN(myVeh, i, 0, 1);
			}
		}
	}

	void CloseAllDoors() {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			Vehicle myVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			for (int i = 0; i < 10; i++) {
				VEHICLE::SET_VEHICLE_DOOR_SHUT(myVeh, i, 0);
			}
		}
	}

	static void LockAllVehicleDoors() {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			Vehicle myVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			RequestControlOfEnt(myVeh);
			if (lockAllVehicleDoors) {
				VEHICLE::SET_VEHICLE_DOORS_LOCKED(myVeh, 4);
			}
			else {
				VEHICLE::SET_VEHICLE_DOORS_LOCKED(myVeh, 0);
			}
		}
	}

	static void VehicleLowGrip() {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			Vehicle myVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (vehicleLowGrip) {
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(myVeh, true);
			}
			else {
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(myVeh, false);
			}
		}
	}

	static void VehicleAlwaysBurnout() {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			Vehicle myVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (vehicleAlwaysBurnout) {
				VEHICLE::SET_VEHICLE_BURNOUT(myVeh, true);
			}
			else {
				VEHICLE::SET_VEHICLE_BURNOUT(myVeh, false);
			}
		}
	}

	void VehiclesMenu()
	{
		AddTitle("VEHICLES");
		AddOption("Vehicle Spawner", null, nullFunc, SUB::VEHICLE_SPAWNER);
		AddOption("Mobile LSC", null, nullFunc, SUB::VEHICLE_MOBILE_LSC);
		AddOption("Vehicle Multipliers", null, nullFunc, SUB::VEHICLE_MULTIPLIERS);
		AddToggle("Vehicle God Mode", vehicleGodMode);
		AddToggle("Invisible Vehicle", invisibleVehicle);
		AddOption("Max Vehicle", null, MaxVehicle);
		AddOption("Fix Vehicle", null, FixVehicle);
		AddToggle("Auto Fix Vehicle", autoFixVehicle);
		AddToggle("No Bike Fall", noBikeFall);
		AddToggle("Super Car Mode (3 & 9)", superCarMode);
		AddToggle("Vehicle Weapons (-)", vehicleWeapon);
		AddCharArray("Select Weapon:", vehicleWeapons, selectedVehicleWeapon, ARRAYSIZE(vehicleWeapons), null);
		AddToggle("Vehicle Aiming Lasers", vehicleLasers);
		AddToggle("Vehicle Collisions", vehicleCollisions);
		AddToggle("Fly Mode", flyMode);
		AddOption("Open All Doors", null, OpenAllDoors);
		AddOption("Close All Doors", null, CloseAllDoors);
		AddToggle("Lock All Doors", lockAllVehicleDoors);
		AddToggle("Low Grip", vehicleLowGrip);
		AddToggle("Always Burnout", vehicleAlwaysBurnout);
		AddOption("Freeze Vehicle", null, FreezeMyVehicle);
		AddOption("Unfreeze Vehicle", null, UnfreezeMyVehicle);
	}

	void WeaponsMenu()
	{
		AddTitle("WEAPONS");
		AddOption("Get All Weapons", null, GetAllWeapons);
		AddToggle("One Shot Kill", oneShotKill);
		AddToggle("Explosive Bullets", explosiveBullets);
		AddToggle("Explosive Melee", explosiveMelee);
		AddToggle("HeatVision On Aim", heatVisionOnAim);
		AddToggle("Triggerbot", aimBot);
		AddToggle("Rapid Fire", rapidFire);
		AddToggle("Infinite Ammo", infiniteAmmo);
		//AddToggle("Weapons Everywhere", weaponsEverywhere);
		AddToggle("~b~Delete Gun", deleteGun);
		AddToggle("Drive It! Gun", driveItGun);
		AddToggle("Teleport Gun", teleportGun);
		//AddToggle("Gravity Gun", gravityGun);
		AddToggle("Shoot Rhino", shootRhino);
	}

	float precision = 0.10f;
	void ObjectEditor(Object o = objectInEditor) {
		bool deleteo = 0, teleportToObject = 0, teleportObjectToMe = 0, ground = 0, clone = 0, deleteall = 0, save = 0, numbersChanged = 0;
		Object obj = o;
		Vector3 objCoords = ENTITY::GET_ENTITY_COORDS(obj, 0);
		Vector3 objRot = ENTITY::GET_ENTITY_ROTATION(obj, 2);
		float ox = objCoords.x;
		float oy = objCoords.y;
		float oz = objCoords.z;
		float opitch = objRot.x;
		float oyaw = objRot.z;
		float oroll = objRot.y;
		if (!ENTITY::DOES_ENTITY_EXIST(obj)) { PrintStringBottomCentre("Object not found"); menu::SetSub_previous(); }
		AddTitle("Object Editor");
		//	AddOption("Attach to vehicle", null, nullFunc, SUB::ATTACHTOVEHICLE);
		AddNumberEasy("Precision", precision, 2, precision, 0.01f, 1);
		AddOption("Clone", clone);
		AddFloat("X", ox, 3, ox, precision, numbersChanged);
		AddFloat("Y", oy, 3, oy, precision, numbersChanged);
		AddFloat("Z", oz, 3, oz, precision, numbersChanged);
		AddFloat("Pitch", opitch, 3, opitch, precision, numbersChanged);
		AddFloat("Yaw", oyaw, 3, oyaw, precision, numbersChanged);
		AddFloat("Roll", oroll, 3, oroll, precision, numbersChanged);
		AddOption("Place on ground", ground);
		AddOption("Teleport To Object", teleportToObject);
		AddOption("Teleport Object To Me", teleportObjectToMe);
		AddOption("Delete", deleteo);
		Vector3 lastCoords;
		if (numbersChanged) {
			lastCoords = ENTITY::GET_ENTITY_COORDS(objectInEditor, 0);
			ENTITY::SET_ENTITY_COORDS(obj, ox, oy, oz, 0, 0, 0, 1);
			ENTITY::SET_ENTITY_ROTATION(obj, opitch, oroll, oyaw, 2, 1);
		}

		if (clone) {
			Object clonedObject = SpawnObject(objectInEditorName, ox, oy, oz + 1);
			ENTITY::SET_ENTITY_ROTATION(clonedObject, opitch, oroll, oyaw, 2, 1);
			Vector3 objCoords = ENTITY::GET_ENTITY_COORDS(clonedObject, 1);
			Vector3 objRot = ENTITY::GET_ENTITY_ROTATION(clonedObject, 0);
			objectInEditor = clonedObject;
		}

		//Tp To Player
		if (deleteo) {
			RequestControlOfEnt(objectInEditor);
			ENTITY::DELETE_ENTITY(&objectInEditor);
		}

		if (teleportToObject) {
			Vector3 objectPos = ENTITY::GET_ENTITY_COORDS(objectInEditor, true);
			TeleportMeTo(objectPos.x, objectPos.y, objectPos.z + 1);
		}

		if (teleportObjectToMe) {
			Vector3 myPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			RequestControlOfEnt(objectInEditor);
			ENTITY::SET_ENTITY_COORDS(objectInEditor, ox, oy, oz, 0, 0, 0, 1);
		}
		if (ground) OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(obj);
	}

	float clearRange = 20.0f;
	void ClearArea() {
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		bool peds = 0, objs = 0, vehs = 0;
		AddTitle("Clear Area");
		AddNumberEasy("Radius", clearRange, 0, clearRange, 1.0f, 1);
		AddOption("Clear Vehicles", vehs);
		AddOption("Clear Peds", peds);
		AddOption("Clear Objects", objs);
		if (vehs) GAMEPLAY::CLEAR_AREA_OF_VEHICLES(coords.x, coords.y, coords.z, clearRange, 0, 0, 0, 0, 0);
		if (peds) GAMEPLAY::CLEAR_AREA_OF_PEDS(coords.x, coords.y, coords.z, clearRange, 0);
		if (objs) GAMEPLAY::CLEAR_AREA_OF_OBJECTS(coords.x, coords.y, coords.z, clearRange, 0);
	}
	void ObjectListMenu() {
		AddTitle("Spawn Object");
		AddOption("Cool Objects", null, nullFunc, SUB::COOL_OBJECTS_LIST);
		AddOption("Stunt DLC Objects", null, nullFunc, SUB::STUNT_DLC_LIST);
		AddOption("Full Objects List", null, nullFunc, SUB::FULL_OBJECTS_LIST);
	}
	char* coolObjectsList[] = { "prop_atm_01", "prop_windmill_01", "p_spinning_anus_s", "prop_Ld_ferris_wheel", "prop_beach_fire", "prop_asteroid_01", "prop_air_bigradar", "Prop_weed_01", "hei_prop_heist_emp", "prop_juicestand", "prop_acc_guitar_01", "prop_coral_grass_01", "prop_alien_egg_01" };
	void CoolObjectsMenu() {
		AddTitle("Cool Objects");
		for (int i = 0; i < ARRAYSIZE(coolObjectsList); i++) {
			AddObject(coolObjectsList[i]);
		}
	}

	char* stuntPropList[] = { "stt_prop_corner_sign_01",
		"stt_prop_corner_sign_02",
		"stt_prop_corner_sign_03",
		"stt_prop_corner_sign_04",
		"stt_prop_corner_sign_05",
		"stt_prop_corner_sign_06",
		"stt_prop_corner_sign_07",
		"stt_prop_corner_sign_08",
		"stt_prop_corner_sign_09",
		"stt_prop_corner_sign_10",
		"stt_prop_corner_sign_11",
		"stt_prop_corner_sign_12",
		"stt_prop_corner_sign_13",
		"stt_prop_corner_sign_14",
		"stt_prop_flagpole_1a",
		"stt_prop_flagpole_1b",
		"stt_prop_flagpole_1c",
		"stt_prop_flagpole_1d",
		"stt_prop_flagpole_1e",
		"stt_prop_flagpole_1f",
		"stt_prop_flagpole_2a",
		"stt_prop_flagpole_2b",
		"stt_prop_flagpole_2c",
		"stt_prop_flagpole_2d",
		"stt_prop_flagpole_2e",
		"stt_prop_flagpole_2f",
		"stt_prop_hoop_constraction_01a",
		"stt_prop_hoop_small_01",
		"stt_prop_hoop_tyre_01a",
		"stt_prop_lives_bottle",
		"stt_prop_race_gantry_01",
		"stt_prop_race_start_line_01",
		"stt_prop_race_start_line_01b",
		"stt_prop_race_start_line_02",
		"stt_prop_race_start_line_02b",
		"stt_prop_race_start_line_03",
		"stt_prop_race_start_line_03b",
		"stt_prop_race_tannoy",
		"stt_prop_ramp_adj_flip_m",
		"stt_prop_ramp_adj_flip_mb",
		"stt_prop_ramp_adj_flip_s",
		"stt_prop_ramp_adj_flip_sb",
		"stt_prop_ramp_adj_hloop",
		"stt_prop_ramp_adj_loop",
		"stt_prop_ramp_jump_l",
		"stt_prop_ramp_jump_m",
		"stt_prop_ramp_jump_s",
		"stt_prop_ramp_jump_xl",
		"stt_prop_ramp_jump_xs",
		"stt_prop_ramp_jump_xxl",
		"stt_prop_ramp_multi_loop_rb",
		"stt_prop_ramp_spiral_l",
		"stt_prop_ramp_spiral_l_l",
		"stt_prop_ramp_spiral_l_m",
		"stt_prop_ramp_spiral_l_s",
		"stt_prop_ramp_spiral_l_xxl",
		"stt_prop_ramp_spiral_m",
		"stt_prop_ramp_spiral_s",
		"stt_prop_ramp_spiral_xxl",
		"stt_prop_sign_circuit_01",
		"stt_prop_sign_circuit_02",
		"stt_prop_sign_circuit_03",
		"stt_prop_sign_circuit_04",
		"stt_prop_sign_circuit_05",
		"stt_prop_sign_circuit_06",
		"stt_prop_sign_circuit_07",
		"stt_prop_sign_circuit_08",
		"stt_prop_sign_circuit_09",
		"stt_prop_sign_circuit_10",
		"stt_prop_sign_circuit_11",
		"stt_prop_sign_circuit_11b",
		"stt_prop_sign_circuit_12",
		"stt_prop_sign_circuit_13",
		"stt_prop_sign_circuit_13b",
		"stt_prop_sign_circuit_14",
		"stt_prop_sign_circuit_14b",
		"stt_prop_sign_circuit_15",
		"stt_prop_slow_down",
		"stt_prop_speakerstack_01a",
		"stt_prop_startline_gantry",
		"stt_prop_stunt_bblock_huge_01",
		"stt_prop_stunt_bblock_huge_02",
		"stt_prop_stunt_bblock_huge_03",
		"stt_prop_stunt_bblock_huge_04",
		"stt_prop_stunt_bblock_huge_05",
		"stt_prop_stunt_bblock_hump_01",
		"stt_prop_stunt_bblock_hump_02",
		"stt_prop_stunt_bblock_lrg1",
		"stt_prop_stunt_bblock_lrg2",
		"stt_prop_stunt_bblock_lrg3",
		"stt_prop_stunt_bblock_mdm1",
		"stt_prop_stunt_bblock_mdm2",
		"stt_prop_stunt_bblock_mdm3",
		"stt_prop_stunt_bblock_qp",
		"stt_prop_stunt_bblock_qp2",
		"stt_prop_stunt_bblock_qp3",
		"stt_prop_stunt_bblock_sml1",
		"stt_prop_stunt_bblock_sml2",
		"stt_prop_stunt_bblock_sml3",
		"stt_prop_stunt_bblock_xl1",
		"stt_prop_stunt_bblock_xl2",
		"stt_prop_stunt_bblock_xl3",
		"stt_prop_stunt_bowling_ball",
		"stt_prop_stunt_bowling_pin",
		"stt_prop_stunt_bowlpin_stand",
		"stt_prop_stunt_domino",
		"stt_prop_stunt_jump15",
		"stt_prop_stunt_jump30",
		"stt_prop_stunt_jump45",
		"stt_prop_stunt_jump_l",
		"stt_prop_stunt_jump_lb",
		"stt_prop_stunt_jump_loop",
		"stt_prop_stunt_jump_m",
		"stt_prop_stunt_jump_mb",
		"stt_prop_stunt_jump_s",
		"stt_prop_stunt_jump_sb",
		"stt_prop_stunt_landing_zone_01",
		"stt_prop_stunt_ramp",
		"stt_prop_stunt_soccer_ball",
		"stt_prop_stunt_soccer_goal",
		"stt_prop_stunt_soccer_lball",
		"stt_prop_stunt_soccer_sball",
		"stt_prop_stunt_target",
		"stt_prop_stunt_target_small",
		"stt_prop_stunt_track_bumps",
		"stt_prop_stunt_track_cutout",
		"stt_prop_stunt_track_dwlink",
		"stt_prop_stunt_track_dwlink_02",
		"stt_prop_stunt_track_dwsh15",
		"stt_prop_stunt_track_dwshort",
		"stt_prop_stunt_track_dwslope15",
		"stt_prop_stunt_track_dwslope30",
		"stt_prop_stunt_track_dwslope45",
		"stt_prop_stunt_track_dwturn",
		"stt_prop_stunt_track_dwuturn",
		"stt_prop_stunt_track_exshort",
		"stt_prop_stunt_track_fork",
		"stt_prop_stunt_track_funlng",
		"stt_prop_stunt_track_funnel",
		"stt_prop_stunt_track_hill",
		"stt_prop_stunt_track_hill2",
		"stt_prop_stunt_track_jump",
		"stt_prop_stunt_track_link",
		"stt_prop_stunt_track_otake",
		"stt_prop_stunt_track_sh15",
		"stt_prop_stunt_track_sh30",
		"stt_prop_stunt_track_sh45",
		"stt_prop_stunt_track_sh45_a",
		"stt_prop_stunt_track_short",
		"stt_prop_stunt_track_slope15",
		"stt_prop_stunt_track_slope30",
		"stt_prop_stunt_track_slope45",
		"stt_prop_stunt_track_start",
		"stt_prop_stunt_track_start_02",
		"stt_prop_stunt_track_straight",
		"stt_prop_stunt_track_straightice",
		"stt_prop_stunt_track_st_01",
		"stt_prop_stunt_track_st_02",
		"stt_prop_stunt_track_turn",
		"stt_prop_stunt_track_turnice",
		"stt_prop_stunt_track_uturn",
		"stt_prop_stunt_tube_crn",
		"stt_prop_stunt_tube_crn2",
		"stt_prop_stunt_tube_crn_15d",
		"stt_prop_stunt_tube_crn_30d",
		"stt_prop_stunt_tube_crn_5d",
		"stt_prop_stunt_tube_cross",
		"stt_prop_stunt_tube_end",
		"stt_prop_stunt_tube_ent",
		"stt_prop_stunt_tube_fn_01",
		"stt_prop_stunt_tube_fn_02",
		"stt_prop_stunt_tube_fn_03",
		"stt_prop_stunt_tube_fn_04",
		"stt_prop_stunt_tube_fn_05",
		"stt_prop_stunt_tube_fork",
		"stt_prop_stunt_tube_gap_01",
		"stt_prop_stunt_tube_gap_02",
		"stt_prop_stunt_tube_gap_03",
		"stt_prop_stunt_tube_hg",
		"stt_prop_stunt_tube_jmp",
		"stt_prop_stunt_tube_jmp2",
		"stt_prop_stunt_tube_l",
		"stt_prop_stunt_tube_m",
		"stt_prop_stunt_tube_qg",
		"stt_prop_stunt_tube_s",
		"stt_prop_stunt_tube_speed",
		"stt_prop_stunt_tube_speeda",
		"stt_prop_stunt_tube_speedb",
		"stt_prop_stunt_tube_xs",
		"stt_prop_stunt_tube_xxs",
		"stt_prop_stunt_wideramp",
		"stt_prop_track_bend2_bar_l",
		"stt_prop_track_bend2_bar_l_b",
		"stt_prop_track_bend2_l",
		"stt_prop_track_bend2_l_b",
		"stt_prop_track_bend_15d",
		"stt_prop_track_bend_15d_bar",
		"stt_prop_track_bend_180d",
		"stt_prop_track_bend_180d_bar",
		"stt_prop_track_bend_30d",
		"stt_prop_track_bend_30d_bar",
		"stt_prop_track_bend_5d",
		"stt_prop_track_bend_5d_bar",
		"stt_prop_track_bend_bar_l",
		"stt_prop_track_bend_bar_l_b",
		"stt_prop_track_bend_bar_m",
		"stt_prop_track_bend_l",
		"stt_prop_track_bend_l_b",
		"stt_prop_track_bend_m",
		"stt_prop_track_block_01",
		"stt_prop_track_block_02",
		"stt_prop_track_block_03",
		"stt_prop_track_chicane_l",
		"stt_prop_track_chicane_l_02",
		"stt_prop_track_chicane_r",
		"stt_prop_track_chicane_r_02",
		"stt_prop_track_cross",
		"stt_prop_track_cross_bar",
		"stt_prop_track_fork",
		"stt_prop_track_fork_bar",
		"stt_prop_track_funnel",
		"stt_prop_track_funnel_ads_01a",
		"stt_prop_track_funnel_ads_01b",
		"stt_prop_track_funnel_ads_01c",
		"stt_prop_track_jump_01a",
		"stt_prop_track_jump_01b",
		"stt_prop_track_jump_01c",
		"stt_prop_track_jump_02a",
		"stt_prop_track_jump_02b",
		"stt_prop_track_jump_02c",
		"stt_prop_track_link",
		"stt_prop_track_slowdown",
		"stt_prop_track_slowdown_t1",
		"stt_prop_track_slowdown_t2",
		"stt_prop_track_speedup",
		"stt_prop_track_speedup_t1",
		"stt_prop_track_speedup_t2",
		"stt_prop_track_start",
		"stt_prop_track_start_02",
		"stt_prop_track_stop_sign",
		"stt_prop_track_straight_bar_l",
		"stt_prop_track_straight_bar_m",
		"stt_prop_track_straight_bar_s",
		"stt_prop_track_straight_l",
		"stt_prop_track_straight_lm",
		"stt_prop_track_straight_lm_bar",
		"stt_prop_track_straight_m",
		"stt_prop_track_straight_s",
		"stt_prop_track_tube_01",
		"stt_prop_track_tube_02",
		"stt_prop_tyre_wall_01",
		"stt_prop_tyre_wall_010",
		"stt_prop_tyre_wall_011",
		"stt_prop_tyre_wall_012",
		"stt_prop_tyre_wall_013",
		"stt_prop_tyre_wall_014",
		"stt_prop_tyre_wall_015",
		"stt_prop_tyre_wall_02",
		"stt_prop_tyre_wall_03",
		"stt_prop_tyre_wall_04",
		"stt_prop_tyre_wall_05",
		"stt_prop_tyre_wall_06",
		"stt_prop_tyre_wall_07",
		"stt_prop_tyre_wall_08",
		"stt_prop_tyre_wall_09",
		"stt_prop_tyre_wall_0l010",
		"stt_prop_tyre_wall_0l012",
		"stt_prop_tyre_wall_0l013",
		"stt_prop_tyre_wall_0l014",
		"stt_prop_tyre_wall_0l015",
		"stt_prop_tyre_wall_0l018",
		"stt_prop_tyre_wall_0l019",
		"stt_prop_tyre_wall_0l020",
		"stt_prop_tyre_wall_0l04",
		"stt_prop_tyre_wall_0l05",
		"stt_prop_tyre_wall_0l06",
		"stt_prop_tyre_wall_0l07",
		"stt_prop_tyre_wall_0l08",
		"stt_prop_tyre_wall_0l1",
		"stt_prop_tyre_wall_0l16",
		"stt_prop_tyre_wall_0l17",
		"stt_prop_tyre_wall_0l2",
		"stt_prop_tyre_wall_0l3",
		"stt_prop_tyre_wall_0r010",
		"stt_prop_tyre_wall_0r011",
		"stt_prop_tyre_wall_0r012",
		"stt_prop_tyre_wall_0r013",
		"stt_prop_tyre_wall_0r014",
		"stt_prop_tyre_wall_0r015",
		"stt_prop_tyre_wall_0r016",
		"stt_prop_tyre_wall_0r017",
		"stt_prop_tyre_wall_0r018",
		"stt_prop_tyre_wall_0r019",
		"stt_prop_tyre_wall_0r04",
		"stt_prop_tyre_wall_0r05",
		"stt_prop_tyre_wall_0r06",
		"stt_prop_tyre_wall_0r07",
		"stt_prop_tyre_wall_0r08",
		"stt_prop_tyre_wall_0r09",
		"stt_prop_tyre_wall_0r1",
		"stt_prop_tyre_wall_0r2",
		"stt_prop_tyre_wall_0r3",
		"stt_prop_wallride_01",
		"stt_prop_wallride_01b",
		"stt_prop_wallride_02",
		"stt_prop_wallride_02b",
		"stt_prop_wallride_04",
		"stt_prop_wallride_05",
		"stt_prop_wallride_05b",
		"stt_prop_wallride_45l",
		"stt_prop_wallride_45la",
		"stt_prop_wallride_45r",
		"stt_prop_wallride_45ra",
		"stt_prop_wallride_90l",
		"stt_prop_wallride_90lb",
		"stt_prop_wallride_90r",
		"stt_prop_wallride_90rb" };


	void StuntObjectsMenu() {
		AddTitle("Cool Objects");
		for (int i = 0; i < ARRAYSIZE(stuntPropList); i++) {
			AddObject(stuntPropList[i]);
		}
	}

	char* propList[] =
	{
		"prop_a4_pile_01",
		"prop_a4_sheet_01",
		"prop_a4_sheet_02",
		"prop_a4_sheet_03",
		"prop_a4_sheet_04",
		"prop_a4_sheet_05",
		"prop_abat_roller_static",
		"prop_abat_slide",
		"prop_acc_guitar_01",
		"prop_acc_guitar_01_d1",
		"prop_aerial_01a",
		"prop_aerial_01b",
		"prop_aerial_01c",
		"prop_aerial_01d",
		"prop_afsign_amun",
		"prop_afsign_vbike",
		"prop_agave_01",
		"prop_agave_02",
		"prop_aiprort_sign_01",
		"prop_aiprort_sign_02",
		"prop_aircon_l_01",
		"prop_aircon_l_02",
		"prop_aircon_l_03",
		"prop_aircon_l_04",
		"prop_aircon_m_09",
		"prop_aircon_s_01a",
		"prop_aircon_s_02a",
		"prop_aircon_s_02b",
		"prop_aircon_s_03a",
		"prop_aircon_s_03b",
		"prop_aircon_s_04a",
		"prop_aircon_s_05a",
		"prop_aircon_s_06a",
		"prop_aircon_s_07a",
		"prop_aircon_s_07b",
		"prop_airhockey_01",
		"prop_air_bagloader",
		"prop_air_bagloader2",
		"prop_air_barrier",
		"prop_air_bench_01",
		"prop_air_bench_02",
		"prop_air_bigradar_l1",
		"prop_air_bigradar_l2",
		"prop_air_bigradar_slod",
		"prop_air_blastfence_01",
		"prop_air_blastfence_02",
		"prop_air_bridge01",
		"prop_air_bridge02",
		"prop_air_cargoloader_01",
		"prop_air_cargo_01a",
		"prop_air_cargo_01b",
		"prop_air_cargo_01c",
		"prop_air_cargo_02a",
		"prop_air_cargo_02b",
		"prop_air_cargo_03a",
		"prop_air_cargo_04a",
		"prop_air_cargo_04b",
		"prop_air_cargo_04c",
		"prop_air_chock_01",
		"prop_air_chock_03",
		"prop_air_chock_04",
		"prop_air_fueltrail1",
		"prop_air_fueltrail2",
		"prop_air_gasbogey_01",
		"prop_air_generator_01",
		"prop_air_generator_03",
		"prop_air_hoc_paddle_01",
		"prop_air_hoc_paddle_02",
		"prop_air_lights_01a",
		"prop_air_lights_01b",
		"prop_air_lights_03a",
		"prop_air_luggtrolley",
		"prop_air_mast_01",
		"prop_air_mast_02",
		"prop_air_monhut_01",
		"prop_air_monhut_02",
		"prop_air_monhut_03",
		"prop_air_propeller01",
		"prop_air_radar_01",
		"prop_air_stair_01",
		"prop_air_stair_02",
		"prop_air_stair_03",
		"prop_air_stair_04a",
		"prop_air_stair_04b",
		"prop_air_towbar_01",
		"prop_air_towbar_02",
		"prop_air_towbar_03",
		"prop_air_trailer_4a",
		"prop_air_trailer_4b",
		"prop_air_trailer_4c",
		"prop_air_watertank1",
		"prop_air_watertank2",
		"prop_air_windsock_base",
		"prop_air_woodsteps",
		"prop_alarm_01",
		"prop_alarm_02",
		"prop_alien_egg_01",
		"prop_aloevera_01",
		"prop_amanda_note_01",
		"prop_amanda_note_01b",
		"prop_amb_40oz_02",
		"prop_amb_40oz_03",
		"prop_amb_beer_bottle",
		"prop_amb_ciggy_01",
		"prop_amb_donut",
		"prop_amb_handbag_01",
		"prop_amb_phone",
		"prop_amp_01",
		"prop_am_box_wood_01",
		"prop_anim_cash_note",
		"prop_anim_cash_note_b",
		"prop_anim_cash_pile_01",
		"prop_anim_cash_pile_02",
		"prop_apple_box_01",
		"prop_apple_box_02",
		"prop_arcade_01",
		"prop_arcade_02",
		"prop_arc_blueprints_01",
		"prop_armchair_01",
		"prop_armenian_gate",
		"prop_armour_pickup",
		"prop_arm_gate_l",
		"prop_arm_wrestle_01",
		"prop_artgallery_02_dl",
		"prop_artgallery_02_dr",
		"prop_artgallery_dl",
		"prop_artgallery_dr",
		"prop_artifact_01",
		"prop_ashtray_01",
		"prop_asteroid_01",
		"prop_atm_02",
		"prop_atm_03",
		"prop_attache_case_01",
		"prop_aviators_01",
		"prop_a_base_bars_01",
		"prop_a_trailer_door_01",
		"prop_bahammenu",
		"prop_ballistic_shield",
		"prop_ballistic_shield_lod1",
		"prop_bandsaw_01",
		"prop_bank_shutter",
		"prop_bank_vaultdoor",
		"prop_barbell_01",
		"prop_barbell_02",
		"prop_barbell_100kg",
		"prop_barbell_10kg",
		"prop_barbell_20kg",
		"prop_barbell_30kg",
		"prop_barbell_40kg",
		"prop_barbell_50kg",
		"prop_barbell_60kg",
		"prop_barbell_80kg",
		"prop_barier_conc_01b",
		"prop_barier_conc_01c",
		"prop_barier_conc_02b",
		"prop_barier_conc_02c",
		"prop_barier_conc_03a",
		"prop_barier_conc_04a",
		"prop_barier_conc_05a",
		"prop_barier_conc_05b",
		"prop_barn_door_l",
		"prop_barn_door_r",
		"prop_barrachneon",
		"prop_barrel_01a",
		"prop_barrel_02a",
		"prop_barrel_02b",
		"prop_barrel_03a",
		"prop_barrel_03d",
		"prop_barrel_float_1",
		"prop_barrel_float_2",
		"prop_barriercrash_03",
		"prop_barriercrash_04",
		"prop_barrier_wat_01a",
		"prop_barrier_wat_03b",
		"prop_barrier_work01c",
		"prop_barry_table_detail",
		"prop_bar_coastbarr",
		"prop_bar_coastchamp",
		"prop_bar_coastdusc",
		"prop_bar_coastmount",
		"prop_bar_cooler_01",
		"prop_bar_cooler_03",
		"prop_bar_fridge_01",
		"prop_bar_fridge_02",
		"prop_bar_fridge_03",
		"prop_bar_fridge_04",
		"prop_bar_ice_01",
		"prop_bar_napkindisp",
		"prop_bar_pump_01",
		"prop_bar_pump_04",
		"prop_bar_pump_05",
		"prop_bar_pump_06",
		"prop_bar_pump_07",
		"prop_bar_pump_08",
		"prop_bar_pump_09",
		"prop_bar_pump_10",
		"prop_bar_sink_01",
		"prop_bar_stool_01",
		"prop_basejump_target_01",
		"prop_basketball_net",
		"prop_battery_01",
		"prop_battery_02",
		"prop_bball_arcade_01",
		"prop_bbq_2",
		"prop_bbq_3",
		"prop_beachbag_01",
		"prop_beachbag_02",
		"prop_beachbag_03",
		"prop_beachbag_04",
		"prop_beachbag_05",
		"prop_beachbag_06",
		"prop_beachbag_combo_01",
		"prop_beachbag_combo_02",
		"prop_beachball_02",
		"prop_beachflag_le",
		"prop_beach_bars_01",
		"prop_beach_bars_02",
		"prop_beach_bbq",
		"prop_beach_dip_bars_01",
		"prop_beach_dip_bars_02",
		"prop_beach_fire",
		"prop_beach_lg_float",
		"prop_beach_lg_stretch",
		"prop_beach_lg_surf",
		"prop_beach_lotion_01",
		"prop_beach_lotion_02",
		"prop_beach_lotion_03",
		"prop_beach_punchbag",
		"prop_beach_rings_01",
		"prop_beach_sculp_01",
		"prop_beach_towel_02",
		"prop_beach_volball01",
		"prop_beach_volball02",
		"prop_beerneon",
		"prop_beer_bison",
		"prop_beer_box_01",
		"prop_beer_neon_01",
		"prop_beer_neon_02",
		"prop_beer_neon_03",
		"prop_beer_neon_04",
		"prop_beggers_sign_01",
		"prop_beggers_sign_02",
		"prop_beggers_sign_03",
		"prop_beggers_sign_04",
		"prop_bench_01b",
		"prop_bench_01c",
		"prop_bench_04",
		"prop_bench_05",
		"prop_bench_09",
		"prop_beta_tape",
		"prop_beware_dog_sign",
		"prop_bh1_03_gate_l",
		"prop_bh1_03_gate_r",
		"prop_bh1_08_mp_gar",
		"prop_bh1_09_mp_gar",
		"prop_bh1_09_mp_l",
		"prop_bh1_09_mp_r",
		"prop_bh1_16_display",
		"prop_bh1_44_door_01l",
		"prop_bh1_44_door_01r",
		"prop_bh1_48_backdoor_l",
		"prop_bh1_48_backdoor_r",
		"prop_bh1_48_gate_1",
		"prop_bhhotel_door_l",
		"prop_bhhotel_door_r",
		"prop_big_bag_01",
		"prop_big_clock_01",
		"prop_big_shit_01",
		"prop_big_shit_02",
		"prop_bikerack_2",
		"prop_bikini_disp_01",
		"prop_bikini_disp_02",
		"prop_bikini_disp_03",
		"prop_bikini_disp_04",
		"prop_bikini_disp_05",
		"prop_bikini_disp_06",
		"prop_billboard_01",
		"prop_billboard_02",
		"prop_billboard_03",
		"prop_billboard_04",
		"prop_billboard_05",
		"prop_billboard_06",
		"prop_billboard_07",
		"prop_billboard_08",
		"prop_billboard_09",
		"prop_billboard_09wall",
		"prop_billboard_10",
		"prop_billboard_11",
		"prop_billboard_12",
		"prop_billboard_13",
		"prop_billboard_14",
		"prop_billboard_15",
		"prop_billboard_16",
		"prop_billb_frame01a",
		"prop_billb_frame01b",
		"prop_billb_frame03a",
		"prop_billb_frame03b",
		"prop_billb_frame03c",
		"prop_billb_frame04a",
		"prop_billb_frame04b",
		"prop_binoc_01",
		"prop_bin_04a",
		"prop_bin_10a",
		"prop_bin_10b",
		"prop_bin_11a",
		"prop_bin_11b",
		"prop_bin_12a",
		"prop_bin_13a",
		"prop_bin_14a",
		"prop_bin_14b",
		"prop_bin_beach_01d",
		"prop_bin_delpiero",
		"prop_bin_delpiero_b",
		"prop_biolab_g_door",
		"prop_biotech_store",
		"prop_bison_winch",
		"prop_blackjack_01",
		"prop_bleachers_01",
		"prop_bleachers_02",
		"prop_bleachers_03",
		"prop_bleachers_04",
		"prop_bleachers_05",
		"prop_blox_spray",
		"prop_bmu_01",
		"prop_bmu_01_b",
		"prop_bmu_02",
		"prop_bmu_02_ld",
		"prop_bmu_02_ld_cab",
		"prop_bmu_02_ld_sup",
		"prop_bmu_track01",
		"prop_bmu_track02",
		"prop_bmu_track03",
		"prop_bodyarmour_02",
		"prop_bodyarmour_03",
		"prop_bodyarmour_04",
		"prop_bodyarmour_05",
		"prop_bodyarmour_06",
		"prop_bollard_01a",
		"prop_bollard_01b",
		"prop_bollard_01c",
		"prop_bollard_03a",
		"prop_bomb_01",
		"prop_bomb_01_s",
		"prop_bonesaw",
		"prop_bongos_01",
		"prop_bong_01",
		"prop_boogbd_stack_01",
		"prop_boogbd_stack_02",
		"prop_boogieboard_01",
		"prop_boogieboard_02",
		"prop_boogieboard_03",
		"prop_boogieboard_04",
		"prop_boogieboard_05",
		"prop_boogieboard_06",
		"prop_boogieboard_07",
		"prop_boogieboard_08",
		"prop_boogieboard_09",
		"prop_boogieboard_10",
		"prop_boombox_01",
		"prop_bottle_cap_01",
		"prop_bowling_ball",
		"prop_bowling_pin",
		"prop_bowl_crisps",
		"prop_boxcar5_handle",
		"prop_boxing_glove_01",
		"prop_boxpile_10a",
		"prop_boxpile_10b",
		"prop_box_ammo01a",
		"prop_box_ammo02a",
		"prop_box_ammo03a_set",
		"prop_box_ammo03a_set2",
		"prop_box_ammo04a",
		"prop_box_ammo05b",
		"prop_box_ammo07a",
		"prop_box_ammo07b",
		"prop_box_guncase_01a",
		"prop_box_guncase_02a",
		"prop_box_guncase_03a",
		"prop_box_tea01a",
		"prop_box_wood05a",
		"prop_box_wood05b",
		"prop_box_wood08a",
		"prop_breadbin_01",
		"prop_bread_rack_01",
		"prop_bread_rack_02",
		"prop_broken_cboard_p1",
		"prop_broken_cboard_p2",
		"prop_broken_cell_gate_01",
		"prop_bskball_01",
		"prop_bs_map_door_01",
		"prop_buckets_02",
		"prop_bucket_01a",
		"prop_bucket_01b",
		"prop_bucket_02a",
		"prop_buck_spade_01",
		"prop_buck_spade_02",
		"prop_buck_spade_03",
		"prop_buck_spade_04",
		"prop_buck_spade_05",
		"prop_buck_spade_06",
		"prop_buck_spade_07",
		"prop_buck_spade_08",
		"prop_buck_spade_09",
		"prop_buck_spade_10",
		"prop_bumper_01",
		"prop_bumper_02",
		"prop_bumper_03",
		"prop_bumper_04",
		"prop_bumper_05",
		"prop_bumper_06",
		"prop_bumper_car_01",
		"prop_burto_gate_01",
		"prop_bush_dead_02",
		"prop_bush_grape_01",
		"prop_bush_ivy_01_1m",
		"prop_bush_ivy_01_2m",
		"prop_bush_ivy_01_bk",
		"prop_bush_ivy_01_l",
		"prop_bush_ivy_01_pot",
		"prop_bush_ivy_01_r",
		"prop_bush_ivy_01_top",
		"prop_bush_ivy_02_1m",
		"prop_bush_ivy_02_2m",
		"prop_bush_ivy_02_l",
		"prop_bush_ivy_02_pot",
		"prop_bush_ivy_02_r",
		"prop_bush_ivy_02_top",
		"prop_bush_lrg_01",
		"prop_bush_lrg_01b",
		"prop_bush_lrg_01c",
		"prop_bush_lrg_01d",
		"prop_bush_lrg_01e",
		"prop_bush_lrg_02",
		"prop_bush_lrg_02b",
		"prop_bush_lrg_03",
		"prop_bush_lrg_04b",
		"prop_bush_lrg_04c",
		"prop_bush_lrg_04d",
		"prop_bush_med_01",
		"prop_bush_med_02",
		"prop_bush_med_03",
		"prop_bush_med_05",
		"prop_bush_med_06",
		"prop_bush_med_07",
		"prop_bush_neat_01",
		"prop_bush_neat_02",
		"prop_bush_neat_03",
		"prop_bush_neat_04",
		"prop_bush_neat_05",
		"prop_bush_neat_06",
		"prop_bush_neat_07",
		"prop_bush_neat_08",
		"prop_bush_ornament_01",
		"prop_bush_ornament_02",
		"prop_bush_ornament_03",
		"prop_bush_ornament_04",
		"prop_busker_hat_01",
		"prop_byard_bench01",
		"prop_byard_bench02",
		"prop_byard_block_01",
		"prop_byard_boat01",
		"prop_byard_boat02",
		"prop_byard_chains01",
		"prop_byard_dingy",
		"prop_byard_elecbox01",
		"prop_byard_elecbox02",
		"prop_byard_elecbox03",
		"prop_byard_elecbox04",
		"prop_byard_floatpile",
		"prop_byard_float_01",
		"prop_byard_float_01b",
		"prop_byard_float_02",
		"prop_byard_float_02b",
		"prop_byard_hoist",
		"prop_byard_hoist_2",
		"prop_byard_hoses01",
		"prop_byard_hoses02",
		"prop_byard_ladder01",
		"prop_byard_machine01",
		"prop_byard_machine02",
		"prop_byard_machine03",
		"prop_byard_motor_01",
		"prop_byard_motor_02",
		"prop_byard_motor_03",
		"prop_byard_net02",
		"prop_byard_phone",
		"prop_byard_pipes01",
		"prop_byard_pipe_01",
		"prop_byard_planks01",
		"prop_byard_pulley01",
		"prop_byard_rack",
		"prop_byard_ramp",
		"prop_byard_rampold",
		"prop_byard_rowboat1",
		"prop_byard_rowboat2",
		"prop_byard_rowboat3",
		"prop_byard_rowboat4",
		"prop_byard_rowboat5",
		"prop_byard_scfhold01",
		"prop_byard_sleeper01",
		"prop_byard_sleeper02",
		"prop_byard_steps_01",
		"prop_byard_tank_01",
		"prop_byard_trailer01",
		"prop_byard_trailer02",
		"prop_b_board_blank",
		"prop_c4_final",
		"prop_c4_final_green",
		"prop_cabinet_01",
		"prop_cabinet_01b",
		"prop_cabinet_02b",
		"prop_cablespool_01a",
		"prop_cablespool_01b",
		"prop_cablespool_02",
		"prop_cablespool_03",
		"prop_cablespool_04",
		"prop_cablespool_05",
		"prop_cablespool_06",
		"prop_cable_hook_01",
		"prop_camera_strap",
		"prop_candy_pqs",
		"prop_can_canoe",
		"prop_cap_01",
		"prop_cap_01b",
		"prop_cap_row_01",
		"prop_cap_row_01b",
		"prop_cap_row_02",
		"prop_cap_row_02b",
		"prop_carcreeper",
		"prop_cargo_int",
		"prop_carjack",
		"prop_carjack_l2",
		"prop_carrier_bag_01",
		"prop_carrier_bag_01_lod",
		"prop_cartwheel_01",
		"prop_carwash_roller_horz",
		"prop_carwash_roller_vert",
		"prop_car_battery_01",
		"prop_car_bonnet_01",
		"prop_car_bonnet_02",
		"prop_car_door_01",
		"prop_car_door_02",
		"prop_car_door_03",
		"prop_car_door_04",
		"prop_car_engine_01",
		"prop_car_exhaust_01",
		"prop_car_ignition",
		"prop_car_seat",
		"prop_casey_sec_id",
		"prop_cash_case_01",
		"prop_cash_case_02",
		"prop_cash_crate_01",
		"prop_cash_dep_bag_01",
		"prop_cash_envelope_01",
		"prop_cash_note_01",
		"prop_cash_pile_01",
		"prop_cash_pile_02",
		"prop_cash_trolly",
		"prop_casino_door_01l",
		"prop_casino_door_01r",
		"prop_cattlecrush",
		"prop_cat_tail_01",
		"prop_cctv_02_sm",
		"prop_cctv_cont_01",
		"prop_cctv_cont_03",
		"prop_cctv_cont_04",
		"prop_cctv_cont_05",
		"prop_cctv_cont_06",
		"prop_cctv_unit_01",
		"prop_cctv_unit_02",
		"prop_cctv_unit_05",
		"prop_cementmixer_01a",
		"prop_cementmixer_02a",
		"prop_ceramic_jug_01",
		"prop_ceramic_jug_cork",
		"prop_ch1_07_door_01l",
		"prop_ch1_07_door_01r",
		"prop_ch1_07_door_02l",
		"prop_ch1_07_door_02r",
		"prop_ch2_05d_g_door",
		"prop_ch2_07b_20_g_door",
		"prop_ch2_09b_door",
		"prop_ch2_09c_garage_door",
		"prop_ch3_01_trlrdoor_l",
		"prop_ch3_01_trlrdoor_r",
		"prop_ch3_04_door_01l",
		"prop_ch3_04_door_01r",
		"prop_ch3_04_door_02",
		"prop_chair_01a",
		"prop_chair_01b",
		"prop_chair_02",
		"prop_chair_03",
		"prop_chair_04a",
		"prop_chair_04b",
		"prop_chair_05",
		"prop_chair_06",
		"prop_chair_07",
		"prop_chair_08",
		"prop_chair_09",
		"prop_chair_10",
		"prop_chair_pile_01",
		"prop_chall_lamp_01",
		"prop_chall_lamp_01n",
		"prop_chall_lamp_02",
		"prop_chateau_chair_01",
		"prop_cheetah_covered",
		"prop_chem_grill",
		"prop_chem_grill_bit",
		"prop_chem_vial_02",
		"prop_chem_vial_02b",
		"prop_cherenneon",
		"prop_chickencoop_a",
		"prop_chip_fryer",
		"prop_choc_ego",
		"prop_choc_meto",
		"prop_choc_pq",
		"prop_ch_025c_g_door_01",
		"prop_cigar_01",
		"prop_cigar_02",
		"prop_cigar_03",
		"prop_cigar_pack_01",
		"prop_cigar_pack_02",
		"prop_clapper_brd_01",
		"prop_cleaver",
		"prop_cliff_paper",
		"prop_clippers_01",
		"prop_clothes_rail_02",
		"prop_clothes_rail_03",
		"prop_clothes_rail_2b",
		"prop_clothes_tub_01",
		"prop_clown_chair",
		"prop_cntrdoor_ld_l",
		"prop_cntrdoor_ld_r",
		"prop_coathook_01",
		"prop_cockneon",
		"prop_coffee_cup_trailer",
		"prop_coffee_mac_02",
		"prop_coffin_02",
		"prop_coffin_02b",
		"prop_coke_block_01",
		"prop_coke_block_half_a",
		"prop_coke_block_half_b",
		"prop_compressor_01",
		"prop_compressor_02",
		"prop_compressor_03",
		"prop_com_gar_door_01",
		"prop_com_ls_door_01",
		"prop_conc_sacks_02a",
		"prop_cone_float_1",
		"prop_conschute",
		"prop_consign_01c",
		"prop_consign_02a",
		"prop_conslift_base",
		"prop_conslift_brace",
		"prop_conslift_cage",
		"prop_conslift_door",
		"prop_conslift_lift",
		"prop_conslift_rail",
		"prop_conslift_rail2",
		"prop_conslift_steps",
		"prop_console_01",
		"prop_construcionlamp_01",
		"prop_const_fence01a",
		"prop_const_fence01b",
		"prop_const_fence02a",
		"prop_const_fence02b",
		"prop_const_fence03b",
		"prop_cons_crate",
		"prop_cons_plank",
		"prop_cons_ply01",
		"prop_cons_ply02",
		"prop_container_01a",
		"prop_container_01b",
		"prop_container_01c",
		"prop_container_01d",
		"prop_container_01e",
		"prop_container_01f",
		"prop_container_01g",
		"prop_container_01h",
		"prop_container_01mb",
		"prop_container_02a",
		"prop_container_03a",
		"prop_container_03b",
		"prop_container_03mb",
		"prop_container_03_ld",
		"prop_container_04a",
		"prop_container_04mb",
		"prop_container_05mb",
		"prop_container_door_mb_l",
		"prop_container_door_mb_r",
		"prop_container_hole",
		"prop_container_ld",
		"prop_container_ld2",
		"prop_container_old1",
		"prop_contnr_pile_01a",
		"prop_controller_01",
		"prop_control_rm_door_01",
		"prop_cont_chiller_01",
		"prop_cooker_03",
		"prop_copier_01",
		"prop_copper_pan",
		"prop_coral_bush_01",
		"prop_coral_flat_01",
		"prop_coral_flat_01_l1",
		"prop_coral_flat_02",
		"prop_coral_flat_brainy",
		"prop_coral_flat_clam",
		"prop_coral_grass_01",
		"prop_coral_grass_02",
		"prop_coral_kelp_01",
		"prop_coral_kelp_01_l1",
		"prop_coral_kelp_02",
		"prop_coral_kelp_02_l1",
		"prop_coral_kelp_03",
		"prop_coral_kelp_03a",
		"prop_coral_kelp_03b",
		"prop_coral_kelp_03c",
		"prop_coral_kelp_03d",
		"prop_coral_kelp_03_l1",
		"prop_coral_kelp_04",
		"prop_coral_kelp_04_l1",
		"prop_coral_pillar_01",
		"prop_coral_pillar_02",
		"prop_coral_spikey_01",
		"prop_coral_stone_03",
		"prop_coral_stone_04",
		"prop_coral_sweed_01",
		"prop_coral_sweed_02",
		"prop_coral_sweed_03",
		"prop_coral_sweed_04",
		"prop_cora_clam_01",
		"prop_cork_board",
		"prop_couch_01",
		"prop_couch_03",
		"prop_couch_04",
		"prop_couch_lg_02",
		"prop_couch_lg_05",
		"prop_couch_lg_06",
		"prop_couch_lg_07",
		"prop_couch_lg_08",
		"prop_couch_sm1_07",
		"prop_couch_sm2_07",
		"prop_couch_sm_02",
		"prop_couch_sm_05",
		"prop_couch_sm_06",
		"prop_couch_sm_07",
		"prop_crane_01_truck1",
		"prop_crane_01_truck2",
		"prop_cranial_saw",
		"prop_crashed_heli",
		"prop_cratepile_07a_l1",
		"prop_crate_01a",
		"prop_crate_02a",
		"prop_crate_08a",
		"prop_crate_09a",
		"prop_crate_10a",
		"prop_crate_11a",
		"prop_crate_11b",
		"prop_crate_11c",
		"prop_crate_11d",
		"prop_crate_float_1",
		"prop_creosote_b_01",
		"prop_crisp",
		"prop_crisp_small",
		"prop_crosssaw_01",
		"prop_cs1_14b_traind",
		"prop_cs1_14b_traind_dam",
		"prop_cs4_05_tdoor",
		"prop_cs4_10_tr_gd_01",
		"prop_cs4_11_door",
		"prop_cs6_03_door_l",
		"prop_cs6_03_door_r",
		"prop_cs_20m_rope",
		"prop_cs_30m_rope",
		"prop_cs_abattoir_switch",
		"prop_cs_aircon_01",
		"prop_cs_aircon_fan",
		"prop_cs_amanda_shoe",
		"prop_cs_ashtray",
		"prop_cs_bandana",
		"prop_cs_bar",
		"prop_cs_beachtowel_01",
		"prop_cs_beer_bot_01",
		"prop_cs_beer_bot_01b",
		"prop_cs_beer_bot_01lod",
		"prop_cs_beer_bot_02",
		"prop_cs_beer_bot_03",
		"prop_cs_beer_bot_40oz",
		"prop_cs_beer_bot_40oz_02",
		"prop_cs_beer_bot_40oz_03",
		"prop_cs_beer_bot_test",
		"prop_cs_binder_01",
		"prop_cs_bin_01",
		"prop_cs_bin_01_lid",
		"prop_cs_bin_01_skinned",
		"prop_cs_bin_02",
		"prop_cs_bin_03",
		"prop_cs_book_01",
		"prop_cs_bottle_opener",
		"prop_cs_bowie_knife",
		"prop_cs_bowl_01",
		"prop_cs_bowl_01b",
		"prop_cs_box_clothes",
		"prop_cs_box_step",
		"prop_cs_brain_chunk",
		"prop_cs_bs_cup",
		"prop_cs_bucket_s",
		"prop_cs_bucket_s_lod",
		"prop_cs_burger_01",
		"prop_cs_business_card",
		"prop_cs_cardbox_01",
		"prop_cs_cash_note_01",
		"prop_cs_cctv",
		"prop_cs_champ_flute",
		"prop_cs_ciggy_01",
		"prop_cs_ciggy_01b",
		"prop_cs_clothes_box",
		"prop_cs_coke_line",
		"prop_cs_cont_latch",
		"prop_cs_crackpipe",
		"prop_cs_credit_card",
		"prop_cs_creeper_01",
		"prop_cs_crisps_01",
		"prop_cs_cuffs_01",
		"prop_cs_diaphram",
		"prop_cs_dildo_01",
		"prop_cs_documents_01",
		"prop_cs_dog_lead_2a",
		"prop_cs_dog_lead_2b",
		"prop_cs_dog_lead_2c",
		"prop_cs_dog_lead_3a",
		"prop_cs_dog_lead_3b",
		"prop_cs_dog_lead_a",
		"prop_cs_dog_lead_b",
		"prop_cs_dog_lead_c",
		"prop_cs_duffel_01",
		"prop_cs_duffel_01b",
		"prop_cs_dumpster_01a",
		"prop_cs_dumpster_lidl",
		"prop_cs_dumpster_lidr",
		"prop_cs_dvd",
		"prop_cs_dvd_case",
		"prop_cs_dvd_player",
		"prop_cs_envolope_01",
		"prop_cs_fertilizer",
		"prop_cs_film_reel_01",
		"prop_cs_folding_chair_01",
		"prop_cs_fork",
		"prop_cs_frank_photo",
		"prop_cs_freightdoor_l1",
		"prop_cs_freightdoor_r1",
		"prop_cs_fridge",
		"prop_cs_fridge_door",
		"prop_cs_fuel_hose",
		"prop_cs_fuel_nozle",
		"prop_cs_gascutter_1",
		"prop_cs_gascutter_2",
		"prop_cs_glass_scrap",
		"prop_cs_gravyard_gate_l",
		"prop_cs_gravyard_gate_r",
		"prop_cs_gunrack",
		"prop_cs_hand_radio",
		"prop_cs_heist_bag_01",
		"prop_cs_heist_bag_02",
		"prop_cs_heist_bag_strap_01",
		"prop_cs_heist_rope",
		"prop_cs_heist_rope_b",
		"prop_cs_hotdog_01",
		"prop_cs_hotdog_02",
		"prop_cs_h_bag_strap_01",
		"prop_cs_ice_locker",
		"prop_cs_ice_locker_door_l",
		"prop_cs_ice_locker_door_r",
		"prop_cs_ilev_blind_01",
		"prop_cs_ironing_board",
		"prop_cs_katana_01",
		"prop_cs_kettle_01",
		"prop_cs_keyboard_01",
		"prop_cs_keys_01",
		"prop_cs_kitchen_cab_l2",
		"prop_cs_kitchen_cab_ld",
		"prop_cs_kitchen_cab_rd",
		"prop_cs_lazlow_ponytail",
		"prop_cs_lazlow_shirt_01",
		"prop_cs_lazlow_shirt_01b",
		"prop_cs_leaf",
		"prop_cs_leg_chain_01",
		"prop_cs_lester_crate",
		"prop_cs_lipstick",
		"prop_cs_magazine",
		"prop_cs_marker_01",
		"prop_cs_meth_pipe",
		"prop_cs_milk_01",
		"prop_cs_mini_tv",
		"prop_cs_mopbucket_01",
		"prop_cs_mop_s",
		"prop_cs_mouse_01",
		"prop_cs_nail_file",
		"prop_cs_newspaper",
		"prop_cs_office_chair",
		"prop_cs_overalls_01",
		"prop_cs_package_01",
		"prop_cs_padlock",
		"prop_cs_pamphlet_01",
		"prop_cs_panel_01",
		"prop_cs_panties",
		"prop_cs_panties_02",
		"prop_cs_panties_03",
		"prop_cs_paper_cup",
		"prop_cs_para_ropebit",
		"prop_cs_para_ropes",
		"prop_cs_pebble",
		"prop_cs_pebble_02",
		"prop_cs_petrol_can",
		"prop_cs_phone_01",
		"prop_cs_photoframe_01",
		"prop_cs_pills",
		"prop_cs_plane_int_01",
		"prop_cs_planning_photo",
		"prop_cs_plant_01",
		"prop_cs_plate_01",
		"prop_cs_polaroid",
		"prop_cs_police_torch",
		"prop_cs_pour_tube",
		"prop_cs_power_cell",
		"prop_cs_power_cord",
		"prop_cs_protest_sign_01",
		"prop_cs_protest_sign_02",
		"prop_cs_protest_sign_02b",
		"prop_cs_protest_sign_03",
		"prop_cs_protest_sign_04a",
		"prop_cs_protest_sign_04b",
		"prop_cs_rage_statue_p1",
		"prop_cs_rage_statue_p2",
		"prop_cs_remote_01",
		"prop_cs_rolled_paper",
		"prop_cs_rope_tie_01",
		"prop_cs_rub_binbag_01",
		"prop_cs_rub_box_01",
		"prop_cs_rub_box_02",
		"prop_cs_r_business_card",
		"prop_cs_sack_01",
		"prop_cs_saucer_01",
		"prop_cs_sc1_11_gate",
		"prop_cs_scissors",
		"prop_cs_script_bottle",
		"prop_cs_script_bottle_01",
		"prop_cs_server_drive",
		"prop_cs_sheers",
		"prop_cs_shirt_01",
		"prop_cs_shopping_bag",
		"prop_cs_shot_glass",
		"prop_cs_silver_tray",
		"prop_cs_sink_filler",
		"prop_cs_sink_filler_02",
		"prop_cs_sink_filler_03",
		"prop_cs_sm_27_gate",
		"prop_cs_sol_glasses",
		"prop_cs_spray_can",
		"prop_cs_steak",
		"prop_cs_stock_book",
		"prop_cs_street_binbag_01",
		"prop_cs_street_card_01",
		"prop_cs_street_card_02",
		"prop_cs_sub_hook_01",
		"prop_cs_sub_rope_01",
		"prop_cs_swipe_card",
		"prop_cs_tablet",
		"prop_cs_tablet_02",
		"prop_cs_toaster",
		"prop_cs_trolley_01",
		"prop_cs_trowel",
		"prop_cs_truck_ladder",
		"prop_cs_tshirt_ball_01",
		"prop_cs_tv_stand",
		"prop_cs_t_shirt_pile",
		"prop_cs_valve",
		"prop_cs_vent_cover",
		"prop_cs_vial_01",
		"prop_cs_walkie_talkie",
		"prop_cs_walking_stick",
		"prop_cs_whiskey_bottle",
		"prop_cs_whiskey_bot_stop",
		"prop_cs_wrench",
		"prop_cub_door_lifeblurb",
		"prop_cub_lifeblurb",
		"prop_cuff_keys_01",
		"prop_cup_saucer_01",
		"prop_curl_bar_01",
		"prop_damdoor_01",
		"prop_dart_1",
		"prop_dart_2",
		"prop_dart_bd_01",
		"prop_dart_bd_cab_01",
		"prop_defilied_ragdoll_01",
		"prop_desert_iron_01",
		"prop_detergent_01a",
		"prop_detergent_01b",
		"prop_devin_box_01",
		"prop_devin_rope_01",
		"prop_diggerbkt_01",
		"prop_direct_chair_01",
		"prop_direct_chair_02",
		"prop_display_unit_01",
		"prop_display_unit_02",
		"prop_disp_cabinet_002",
		"prop_disp_cabinet_01",
		"prop_disp_razor_01",
		"prop_distantcar_day",
		"prop_distantcar_night",
		"prop_distantcar_truck",
		"prop_dj_deck_01",
		"prop_dj_deck_02",
		"prop_dock_bouy_1",
		"prop_dock_bouy_2",
		"prop_dock_bouy_3",
		"prop_dock_bouy_5",
		"prop_dock_crane_01",
		"prop_dock_crane_02",
		"prop_dock_crane_02_cab",
		"prop_dock_crane_02_hook",
		"prop_dock_crane_02_ld",
		"prop_dock_crane_04",
		"prop_dock_crane_lift",
		"prop_dock_float_1",
		"prop_dock_float_1b",
		"prop_dock_moor_01",
		"prop_dock_moor_04",
		"prop_dock_moor_05",
		"prop_dock_moor_06",
		"prop_dock_moor_07",
		"prop_dock_ropefloat",
		"prop_dock_ropetyre1",
		"prop_dock_ropetyre2",
		"prop_dock_ropetyre3",
		"prop_dock_rtg_01",
		"prop_dock_rtg_ld",
		"prop_dock_shippad",
		"prop_dock_sign_01",
		"prop_dock_woodpole1",
		"prop_dock_woodpole2",
		"prop_dock_woodpole3",
		"prop_dock_woodpole4",
		"prop_dock_woodpole5",
		"prop_dog_cage_01",
		"prop_dog_cage_02",
		"prop_dolly_01",
		"prop_dolly_02",
		"prop_donut_01",
		"prop_donut_02",
		"prop_donut_02b",
		"prop_door_01",
		"prop_door_balcony_frame",
		"prop_door_balcony_left",
		"prop_door_balcony_right",
		"prop_door_bell_01",
		"prop_double_grid_line",
		"prop_dress_disp_01",
		"prop_dress_disp_02",
		"prop_dress_disp_03",
		"prop_dress_disp_04",
		"prop_drop_armscrate_01",
		"prop_drop_armscrate_01b",
		"prop_drop_crate_01",
		"prop_drop_crate_01_set",
		"prop_drop_crate_01_set2",
		"prop_drug_burner",
		"prop_drug_package",
		"prop_drug_package_02",
		"prop_drywallpile_01",
		"prop_drywallpile_02",
		"prop_dt1_13_groundlight",
		"prop_dt1_13_walllightsource",
		"prop_dt1_20_mp_door_l",
		"prop_dt1_20_mp_door_r",
		"prop_dt1_20_mp_gar",
		"prop_ducktape_01",
		"prop_dummy_01",
		"prop_dummy_car",
		"prop_dummy_light",
		"prop_dummy_plane",
		"prop_dumpster_3a",
		"prop_dumpster_3step",
		"prop_dumpster_4a",
		"prop_dumpster_4b",
		"prop_d_balcony_l_light",
		"prop_d_balcony_r_light",
		"prop_ear_defenders_01",
		"prop_ecg_01",
		"prop_ecg_01_cable_01",
		"prop_ecg_01_cable_02",
		"prop_ecola_can",
		"prop_egg_clock_01",
		"prop_ejector_seat_01",
		"prop_elecbox_03a",
		"prop_elecbox_10",
		"prop_elecbox_12",
		"prop_elecbox_13",
		"prop_elecbox_14",
		"prop_elecbox_15",
		"prop_elecbox_16",
		"prop_elecbox_17",
		"prop_elecbox_18",
		"prop_elecbox_19",
		"prop_elecbox_20",
		"prop_elecbox_21",
		"prop_elecbox_22",
		"prop_elecbox_23",
		"prop_elecbox_24",
		"prop_elecbox_24b",
		"prop_elecbox_25",
		"prop_el_guitar_01",
		"prop_el_guitar_02",
		"prop_el_guitar_03",
		"prop_employee_month_01",
		"prop_employee_month_02",
		"prop_energy_drink",
		"prop_entityxf_covered",
		"prop_epsilon_door_l",
		"prop_epsilon_door_r",
		"prop_etricmotor_01",
		"prop_exer_bike_01",
		"prop_faceoffice_door_l",
		"prop_faceoffice_door_r",
		"prop_face_rag_01",
		"prop_facgate_01",
		"prop_facgate_01b",
		"prop_facgate_02pole",
		"prop_facgate_02_l",
		"prop_facgate_03post",
		"prop_facgate_03_l",
		"prop_facgate_03_ld_l",
		"prop_facgate_03_ld_r",
		"prop_facgate_03_r",
		"prop_facgate_04_l",
		"prop_facgate_04_r",
		"prop_facgate_05_r",
		"prop_facgate_05_r_dam_l1",
		"prop_facgate_05_r_l1",
		"prop_facgate_06_l",
		"prop_facgate_06_r",
		"prop_facgate_07",
		"prop_facgate_07b",
		"prop_facgate_08",
		"prop_facgate_08_frame",
		"prop_facgate_08_ld2",
		"prop_facgate_id1_27",
		"prop_fac_machine_02",
		"prop_fag_packet_01",
		"prop_fan_01",
		"prop_fan_palm_01a",
		"prop_fax_01",
		"prop_fbi3_coffee_table",
		"prop_fbibombbin",
		"prop_fbibombcupbrd",
		"prop_fbibombfile",
		"prop_fbibombplant",
		"prop_feeder1",
		"prop_feed_sack_01",
		"prop_feed_sack_02",
		"prop_fence_log_01",
		"prop_fence_log_02",
		"prop_ferris_car_01",
		"prop_ferris_car_01_lod1",
		"prop_ff_counter_01",
		"prop_ff_counter_02",
		"prop_ff_counter_03",
		"prop_ff_noodle_01",
		"prop_ff_noodle_02",
		"prop_ff_shelves_01",
		"prop_ff_sink_01",
		"prop_ff_sink_02",
		"prop_fib_badge",
		"prop_fib_broken_window",
		"prop_fib_skylight_piece",
		"prop_film_cam_01",
		"prop_fireescape_01a",
		"prop_fireescape_01b",
		"prop_fireescape_02a",
		"prop_fireescape_02b",
		"prop_fire_driser_1a",
		"prop_fire_driser_1b",
		"prop_fire_driser_2b",
		"prop_fire_driser_3b",
		"prop_fire_driser_4a",
		"prop_fire_driser_4b",
		"prop_fire_hosereel",
		"prop_fishing_rod_01",
		"prop_fishing_rod_02",
		"prop_fish_slice_01",
		"prop_flagpole_1a",
		"prop_flagpole_2a",
		"prop_flagpole_3a",
		"prop_flare_01",
		"prop_flare_01b",
		"prop_flash_unit",
		"prop_flatbed_strap",
		"prop_flatbed_strap_b",
		"prop_flatscreen_overlay",
		"prop_flattrailer_01a",
		"prop_flattruck_01a",
		"prop_fleeca_atm",
		"prop_flight_box_01",
		"prop_flight_box_insert",
		"prop_flight_box_insert2",
		"prop_flipchair_01",
		"prop_floor_duster_01",
		"prop_fncconstruc_02a",
		"prop_fnccorgm_05a",
		"prop_fnccorgm_05b",
		"prop_fnccorgm_06a",
		"prop_fnccorgm_06b",
		"prop_fnclink_01gate1",
		"prop_fnclink_02gate1",
		"prop_fnclink_02gate2",
		"prop_fnclink_02gate5",
		"prop_fnclink_02gate6_l",
		"prop_fnclink_02gate6_r",
		"prop_fnclink_02gate7",
		"prop_fnclink_03gate1",
		"prop_fnclink_03gate2",
		"prop_fnclink_03gate4",
		"prop_fnclink_03gate5",
		"prop_fnclink_04gate1",
		"prop_fnclink_04h_l2",
		"prop_fnclink_06gate2",
		"prop_fnclink_06gate3",
		"prop_fnclink_06gatepost",
		"prop_fnclink_07gate1",
		"prop_fnclink_07gate2",
		"prop_fnclink_07gate3",
		"prop_fnclink_09gate1",
		"prop_fnclink_10a",
		"prop_fnclink_10b",
		"prop_fnclink_10c",
		"prop_fnclink_10d",
		"prop_fnclink_10e",
		"prop_fnclog_01a",
		"prop_fnclog_01b",
		"prop_fncpeir_03a",
		"prop_fncres_02a",
		"prop_fncres_02b",
		"prop_fncres_02c",
		"prop_fncres_02d",
		"prop_fncres_02_gate1",
		"prop_fncres_03gate1",
		"prop_fncres_05c_l1",
		"prop_fncsec_01a",
		"prop_fncsec_01b",
		"prop_fncsec_01crnr",
		"prop_fncsec_01gate",
		"prop_fncsec_01pole",
		"prop_fncsec_02a",
		"prop_fncsec_02pole",
		"prop_fncsec_04a",
		"prop_fncwood_07gate1",
		"prop_fncwood_11a_l1",
		"prop_fncwood_16a",
		"prop_fncwood_16b",
		"prop_fncwood_16c",
		"prop_fncwood_18a",
		"prop_folded_polo_shirt",
		"prop_folder_01",
		"prop_folder_02",
		"prop_food_bin_01",
		"prop_food_bin_02",
		"prop_food_bs_bshelf",
		"prop_food_bs_cups01",
		"prop_food_bs_cups03",
		"prop_food_bs_soda_01",
		"prop_food_bs_soda_02",
		"prop_food_bs_tray_01",
		"prop_food_bs_tray_06",
		"prop_food_burg1",
		"prop_food_burg2",
		"prop_food_cb_bshelf",
		"prop_food_cb_burg01",
		"prop_food_cb_cups01",
		"prop_food_cb_donuts",
		"prop_food_cb_nugets",
		"prop_food_cb_soda_01",
		"prop_food_cb_soda_02",
		"prop_food_cb_tray_01",
		"prop_food_cups1",
		"prop_food_napkin_01",
		"prop_food_napkin_02",
		"prop_food_tray_01",
		"prop_food_van_01",
		"prop_food_van_02",
		"prop_forsalejr2",
		"prop_forsalejr3",
		"prop_forsalejr4",
		"prop_foundation_sponge",
		"prop_fountain1",
		"prop_fountain2",
		"prop_franklin_dl",
		"prop_freeweight_01",
		"prop_freeweight_02",
		"prop_fridge_01",
		"prop_fridge_03",
		"prop_front_seat_01",
		"prop_front_seat_02",
		"prop_front_seat_03",
		"prop_front_seat_04",
		"prop_front_seat_05",
		"prop_front_seat_06",
		"prop_front_seat_07",
		"prop_front_seat_row_01",
		"prop_fruitstand_b_nite",
		"prop_fruit_basket",
		"prop_ftowel_01",
		"prop_ftowel_07",
		"prop_ftowel_08",
		"prop_ftowel_10",
		"prop_f_b_insert_broken",
		"prop_f_duster_01_s",
		"prop_f_duster_02",
		"prop_gaffer_arm_bind",
		"prop_gaffer_arm_bind_cut",
		"prop_gaffer_leg_bind",
		"prop_gaffer_leg_bind_cut",
		"prop_gaffer_tape",
		"prop_gaffer_tape_strip",
		"prop_game_clock_01",
		"prop_game_clock_02",
		"prop_garden_dreamcatch_01",
		"prop_garden_edging_01",
		"prop_garden_edging_02",
		"prop_garden_zapper_01",
		"prop_gardnght_01",
		"prop_gar_door_01",
		"prop_gar_door_02",
		"prop_gar_door_03",
		"prop_gar_door_03_ld",
		"prop_gar_door_04",
		"prop_gar_door_05",
		"prop_gar_door_05_l",
		"prop_gar_door_05_r",
		"prop_gar_door_a_01",
		"prop_gar_door_plug",
		"prop_gascage01",
		"prop_gascyl_ramp_01",
		"prop_gascyl_ramp_door_01",
		"prop_gas_01",
		"prop_gas_02",
		"prop_gas_03",
		"prop_gas_04",
		"prop_gas_05",
		"prop_gas_grenade",
		"prop_gatecom_02",
		"prop_gate_airport_01",
		"prop_gate_bridge_ld",
		"prop_gate_cult_01_l",
		"prop_gate_cult_01_r",
		"prop_gate_docks_ld",
		"prop_gate_farm_01a",
		"prop_gate_farm_post",
		"prop_gate_frame_01",
		"prop_gate_frame_02",
		"prop_gate_frame_04",
		"prop_gate_frame_05",
		"prop_gate_frame_06",
		"prop_gate_military_01",
		"prop_gate_prison_01",
		"prop_gate_tep_01_l",
		"prop_gate_tep_01_r",
		"prop_gazebo_03",
		"prop_gd_ch2_08",
		"prop_generator_02a",
		"prop_generator_03a",
		"prop_generator_04",
		"prop_ghettoblast_02",
		"prop_girder_01a",
		"prop_glasscutter_01",
		"prop_glass_suck_holder",
		"prop_glf_roller",
		"prop_glf_spreader",
		"prop_gold_bar",
		"prop_gold_cont_01",
		"prop_gold_cont_01b",
		"prop_gold_trolly",
		"prop_gold_trolly_full",
		"prop_gold_trolly_strap_01",
		"prop_golf_bag_01",
		"prop_golf_bag_01b",
		"prop_golf_bag_01c",
		"prop_golf_ball",
		"prop_golf_ball_p2",
		"prop_golf_ball_p3",
		"prop_golf_ball_p4",
		"prop_golf_ball_tee",
		"prop_golf_driver",
		"prop_golf_iron_01",
		"prop_golf_marker_01",
		"prop_golf_pitcher_01",
		"prop_golf_putter_01",
		"prop_golf_tee",
		"prop_golf_wood_01",
		"prop_grain_hopper",
		"prop_grapes_01",
		"prop_grapes_02",
		"prop_grass_dry_02",
		"prop_grass_dry_03",
		"prop_gravestones_01a",
		"prop_gravestones_02a",
		"prop_gravestones_03a",
		"prop_gravestones_04a",
		"prop_gravestones_05a",
		"prop_gravestones_06a",
		"prop_gravestones_07a",
		"prop_gravestones_08a",
		"prop_gravestones_09a",
		"prop_gravestones_10a",
		"prop_gravetomb_01a",
		"prop_gravetomb_02a",
		"prop_griddle_01",
		"prop_griddle_02",
		"prop_grumandoor_l",
		"prop_grumandoor_r",
		"prop_gshotsensor_01",
		"prop_gun_case_01",
		"prop_gun_case_02",
		"prop_gun_frame",
		"prop_hacky_sack_01",
		"prop_handdry_01",
		"prop_handdry_02",
		"prop_handrake",
		"prop_handtowels",
		"prop_hand_toilet",
		"prop_hanger_door_1",
		"prop_hard_hat_01",
		"prop_hat_box_01",
		"prop_hat_box_02",
		"prop_hat_box_03",
		"prop_hat_box_04",
		"prop_hat_box_05",
		"prop_hat_box_06",
		"prop_haybailer_01",
		"prop_haybale_01",
		"prop_haybale_02",
		"prop_haybale_stack_01",
		"prop_hd_seats_01",
		"prop_headphones_01",
		"prop_headset_01",
		"prop_hedge_trimmer_01",
		"prop_helipad_01",
		"prop_helipad_02",
		"prop_henna_disp_01",
		"prop_henna_disp_02",
		"prop_henna_disp_03",
		"prop_hifi_01",
		"prop_hobo_stove_01",
		"prop_hockey_bag_01",
		"prop_hole_plug_01",
		"prop_holster_01",
		"prop_homeless_matress_01",
		"prop_homeless_matress_02",
		"prop_hose_1",
		"prop_hose_2",
		"prop_hose_3",
		"prop_hose_nozzle",
		"prop_hospitaldoors_start",
		"prop_hospital_door_l",
		"prop_hospital_door_r",
		"prop_hotel_clock_01",
		"prop_hotel_trolley",
		"prop_hottub2",
		"prop_huf_rag_01",
		"prop_huge_display_01",
		"prop_huge_display_02",
		"prop_hunterhide",
		"prop_hw1_03_gardoor_01",
		"prop_hw1_04_door_l1",
		"prop_hw1_04_door_r1",
		"prop_hw1_23_door",
		"prop_hwbowl_pseat_6x1",
		"prop_hwbowl_seat_01",
		"prop_hwbowl_seat_02",
		"prop_hwbowl_seat_03",
		"prop_hwbowl_seat_03b",
		"prop_hwbowl_seat_6x6",
		"prop_hydro_platform_01",
		"prop_ice_box_01",
		"prop_ice_box_01_l1",
		"prop_ice_cube_01",
		"prop_ice_cube_02",
		"prop_ice_cube_03",
		"prop_id2_11_gdoor",
		"prop_id2_20_clock",
		"prop_idol_01",
		"prop_idol_01_error",
		"prop_idol_case",
		"prop_idol_case_01",
		"prop_idol_case_02",
		"prop_id_21_gardoor_01",
		"prop_id_21_gardoor_02",
		"prop_indus_meet_door_l",
		"prop_indus_meet_door_r",
		"prop_ind_barge_01",
		"prop_ind_barge_02",
		"prop_ind_coalcar_01",
		"prop_ind_coalcar_02",
		"prop_ind_coalcar_03",
		"prop_ind_conveyor_01",
		"prop_ind_conveyor_02",
		"prop_ind_conveyor_04",
		"prop_ind_crusher",
		"prop_ind_deiseltank",
		"prop_ind_light_01a",
		"prop_ind_light_01b",
		"prop_ind_light_01c",
		"prop_ind_mech_01c",
		"prop_ind_mech_02a",
		"prop_ind_mech_02b",
		"prop_ind_mech_03a",
		"prop_ind_mech_04a",
		"prop_ind_oldcrane",
		"prop_ind_washer_02",
		"prop_inflatearch_01",
		"prop_inflategate_01",
		"prop_ing_camera_01",
		"prop_ing_crowbar",
		"prop_inhaler_01",
		"prop_int_gate01",
		"prop_irish_sign_01",
		"prop_irish_sign_02",
		"prop_irish_sign_03",
		"prop_irish_sign_04",
		"prop_irish_sign_05",
		"prop_irish_sign_06",
		"prop_irish_sign_07",
		"prop_irish_sign_08",
		"prop_irish_sign_09",
		"prop_irish_sign_10",
		"prop_irish_sign_11",
		"prop_irish_sign_12",
		"prop_irish_sign_13",
		"prop_iron_01",
		"prop_jb700_covered",
		"prop_jeans_01",
		"prop_jetski_ramp_01",
		"prop_jet_bloodsplat_01",
		"prop_jewel_02a",
		"prop_jewel_02b",
		"prop_jewel_02c",
		"prop_jewel_03a",
		"prop_jewel_03b",
		"prop_jewel_04a",
		"prop_jewel_04b",
		"prop_jewel_pickup_new_01",
		"prop_juice_dispenser",
		"prop_juice_pool_01",
		"prop_jukebox_01",
		"prop_jukebox_02",
		"prop_jyard_block_01a",
		"prop_j_disptray_01",
		"prop_j_disptray_01b",
		"prop_j_disptray_01_dam",
		"prop_j_disptray_02",
		"prop_j_disptray_02_dam",
		"prop_j_disptray_03",
		"prop_j_disptray_03_dam",
		"prop_j_disptray_04",
		"prop_j_disptray_04b",
		"prop_j_disptray_05",
		"prop_j_disptray_05b",
		"prop_j_heist_pic_01",
		"prop_j_heist_pic_02",
		"prop_j_heist_pic_03",
		"prop_j_heist_pic_04",
		"prop_j_neck_disp_01",
		"prop_j_neck_disp_02",
		"prop_j_neck_disp_03",
		"prop_kayak_01",
		"prop_kayak_01b",
		"prop_kebab_grill",
		"prop_keg_01",
		"prop_kettle",
		"prop_kettle_01",
		"prop_keyboard_01a",
		"prop_keyboard_01b",
		"prop_kino_light_01",
		"prop_kino_light_03",
		"prop_kitch_juicer",
		"prop_kitch_pot_fry",
		"prop_kitch_pot_huge",
		"prop_kitch_pot_lrg",
		"prop_kitch_pot_lrg2",
		"prop_kitch_pot_med",
		"prop_kitch_pot_sm",
		"prop_knife",
		"prop_knife_stand",
		"prop_kt1_06_door_l",
		"prop_kt1_06_door_r",
		"prop_kt1_10_mpdoor_l",
		"prop_kt1_10_mpdoor_r",
		"prop_ladel",
		"prop_laptop_02_closed",
		"prop_laptop_jimmy",
		"prop_laptop_lester",
		"prop_laptop_lester2",
		"prop_large_gold",
		"prop_large_gold_alt_a",
		"prop_large_gold_alt_b",
		"prop_large_gold_alt_c",
		"prop_large_gold_empty",
		"prop_lawnmower_01",
		"prop_ld_alarm_01",
		"prop_ld_alarm_01_dam",
		"prop_ld_alarm_alert",
		"prop_ld_ammo_pack_01",
		"prop_ld_ammo_pack_02",
		"prop_ld_ammo_pack_03",
		"prop_ld_armour",
		"prop_ld_balcfnc_01a",
		"prop_ld_balcfnc_02a",
		"prop_ld_balcfnc_02c",
		"prop_ld_bankdoors_02",
		"prop_ld_barrier_01",
		"prop_ld_binbag_01",
		"prop_ld_bomb",
		"prop_ld_bomb_01",
		"prop_ld_bomb_01_open",
		"prop_ld_bomb_anim",
		"prop_ld_cable",
		"prop_ld_cable_tie_01",
		"prop_ld_can_01",
		"prop_ld_case_01",
		"prop_ld_case_01_lod",
		"prop_ld_case_01_s",
		"prop_ld_contact_card",
		"prop_ld_container",
		"prop_ld_contain_dl",
		"prop_ld_contain_dl2",
		"prop_ld_contain_dr",
		"prop_ld_contain_dr2",
		"prop_ld_crate_01",
		"prop_ld_crate_lid_01",
		"prop_ld_crocclips01",
		"prop_ld_crocclips02",
		"prop_ld_dummy_rope",
		"prop_ld_fags_01",
		"prop_ld_fags_02",
		"prop_ld_fan_01",
		"prop_ld_fan_01_old",
		"prop_ld_faucet",
		"prop_ld_ferris_wheel",
		"prop_ld_fireaxe",
		"prop_ld_flow_bottle",
		"prop_ld_fragwall_01a",
		"prop_ld_garaged_01",
		"prop_ld_gold_tooth",
		"prop_ld_greenscreen_01",
		"prop_ld_handbag",
		"prop_ld_handbag_s",
		"prop_ld_hat_01",
		"prop_ld_haybail",
		"prop_ld_hdd_01",
		"prop_ld_health_pack",
		"prop_ld_hook",
		"prop_ld_int_safe_01",
		"prop_ld_jail_door",
		"prop_ld_jeans_01",
		"prop_ld_jeans_02",
		"prop_ld_jerrycan_01",
		"prop_ld_keypad_01",
		"prop_ld_keypad_01b",
		"prop_ld_keypad_01b_lod",
		"prop_ld_lap_top",
		"prop_ld_monitor_01",
		"prop_ld_peep_slider",
		"prop_ld_pipe_single_01",
		"prop_ld_planning_pin_01",
		"prop_ld_planning_pin_02",
		"prop_ld_planning_pin_03",
		"prop_ld_purse_01",
		"prop_ld_purse_01_lod",
		"prop_ld_rail_01",
		"prop_ld_rail_02",
		"prop_ld_rope_t",
		"prop_ld_rubble_01",
		"prop_ld_rubble_02",
		"prop_ld_rubble_03",
		"prop_ld_rubble_04",
		"prop_ld_rub_binbag_01",
		"prop_ld_scrap",
		"prop_ld_shirt_01",
		"prop_ld_shoe_01",
		"prop_ld_shoe_02",
		"prop_ld_shovel",
		"prop_ld_shovel_dirt",
		"prop_ld_snack_01",
		"prop_ld_suitcase_01",
		"prop_ld_suitcase_02",
		"prop_ld_test_01",
		"prop_ld_toilet_01",
		"prop_ld_tooth",
		"prop_ld_tshirt_01",
		"prop_ld_tshirt_02",
		"prop_ld_vault_door",
		"prop_ld_wallet_01",
		"prop_ld_wallet_01_s",
		"prop_ld_wallet_02",
		"prop_ld_wallet_pickup",
		"prop_ld_w_me_machette",
		"prop_leaf_blower_01",
		"prop_lectern_01",
		"prop_letterbox_04",
		"prop_lev_crate_01",
		"prop_lev_des_barge_01",
		"prop_lev_des_barge_02",
		"prop_lifeblurb_01",
		"prop_lifeblurb_01b",
		"prop_lifeblurb_02",
		"prop_lifeblurb_02b",
		"prop_life_ring_02",
		"prop_lift_overlay_01",
		"prop_lift_overlay_02",
		"prop_litter_picker",
		"prop_loggneon",
		"prop_logpile_05",
		"prop_logpile_06",
		"prop_logpile_06b",
		"prop_logpile_07",
		"prop_logpile_07b",
		"prop_log_01",
		"prop_log_02",
		"prop_log_03",
		"prop_loose_rag_01",
		"prop_lrggate_01c_l",
		"prop_lrggate_01c_r",
		"prop_lrggate_01_l",
		"prop_lrggate_01_pst",
		"prop_lrggate_01_r",
		"prop_lrggate_02_ld",
		"prop_lrggate_03a",
		"prop_lrggate_03b",
		"prop_lrggate_03b_ld",
		"prop_lrggate_04a",
		"prop_lrggate_05a",
		"prop_lrggate_06a",
		"prop_luggage_01a",
		"prop_luggage_02a",
		"prop_luggage_03a",
		"prop_luggage_04a",
		"prop_luggage_05a",
		"prop_luggage_06a",
		"prop_luggage_07a",
		"prop_luggage_08a",
		"prop_luggage_09a",
		"prop_magenta_door",
		"prop_makeup_trail_01",
		"prop_makeup_trail_02",
		"prop_map_door_01",
		"prop_mast_01",
		"prop_mat_box",
		"prop_mb_cargo_01a",
		"prop_mb_cargo_02a",
		"prop_mb_cargo_03a",
		"prop_mb_cargo_04a",
		"prop_mb_cargo_04b",
		"prop_mb_crate_01a",
		"prop_mb_crate_01a_set",
		"prop_mb_crate_01b",
		"prop_mb_hesco_06",
		"prop_mb_ordnance_01",
		"prop_mb_ordnance_03",
		"prop_mb_sandblock_01",
		"prop_mb_sandblock_02",
		"prop_mb_sandblock_03",
		"prop_mb_sandblock_04",
		"prop_mb_sandblock_05",
		"prop_medal_01",
		"prop_medstation_02",
		"prop_medstation_03",
		"prop_medstation_04",
		"prop_med_bag_01",
		"prop_med_bag_01b",
		"prop_med_jet_01",
		"prop_megaphone_01",
		"prop_mem_candle_04",
		"prop_mem_candle_05",
		"prop_mem_candle_06",
		"prop_metalfoodjar_01",
		"prop_metal_plates01",
		"prop_metal_plates02",
		"prop_meth_bag_01",
		"prop_michaels_credit_tv",
		"prop_michael_backpack",
		"prop_michael_balaclava",
		"prop_michael_door",
		"prop_michael_sec_id",
		"prop_microphone_02",
		"prop_microwave_1",
		"prop_micro_01",
		"prop_micro_02",
		"prop_micro_cs_01",
		"prop_micro_cs_01_door",
		"prop_military_pickup_01",
		"prop_mil_crate_01",
		"prop_mil_crate_02",
		"prop_minigun_01",
		"prop_mobile_mast_1",
		"prop_mobile_mast_2",
		"prop_money_bag_01",
		"prop_monitor_01c",
		"prop_monitor_01d",
		"prop_monitor_02",
		"prop_monitor_03b",
		"prop_motel_door_09",
		"prop_mouse_01",
		"prop_mouse_01a",
		"prop_mouse_01b",
		"prop_mouse_02",
		"prop_movie_rack",
		"prop_mp3_dock",
		"prop_mp_arrow_barrier_01",
		"prop_mp_barrier_01",
		"prop_mp_barrier_01b",
		"prop_mp_barrier_02",
		"prop_mp_barrier_02b",
		"prop_mp_base_marker",
		"prop_mp_boost_01",
		"prop_mp_cant_place_lrg",
		"prop_mp_cant_place_med",
		"prop_mp_cant_place_sm",
		"prop_mp_cone_01",
		"prop_mp_cone_02",
		"prop_mp_cone_03",
		"prop_mp_cone_04",
		"prop_mp_drug_package",
		"prop_mp_drug_pack_blue",
		"prop_mp_drug_pack_red",
		"prop_mp_icon_shad_lrg",
		"prop_mp_icon_shad_med",
		"prop_mp_icon_shad_sm",
		"prop_mp_max_out_lrg",
		"prop_mp_max_out_med",
		"prop_mp_max_out_sm",
		"prop_mp_num_0",
		"prop_mp_num_1",
		"prop_mp_num_2",
		"prop_mp_num_3",
		"prop_mp_num_4",
		"prop_mp_num_5",
		"prop_mp_num_6",
		"prop_mp_num_7",
		"prop_mp_num_8",
		"prop_mp_num_9",
		"prop_mp_placement",
		"prop_mp_placement_lrg",
		"prop_mp_placement_maxd",
		"prop_mp_placement_med",
		"prop_mp_placement_red",
		"prop_mp_placement_sm",
		"prop_mp_ramp_01",
		"prop_mp_ramp_02",
		"prop_mp_ramp_03",
		"prop_mp_repair",
		"prop_mp_repair_01",
		"prop_mp_respawn_02",
		"prop_mp_rocket_01",
		"prop_mp_spike_01",
		"prop_mr_rasberryclean",
		"prop_mr_raspberry_01",
		"prop_muscle_bench_01",
		"prop_muscle_bench_02",
		"prop_muscle_bench_03",
		"prop_muscle_bench_04",
		"prop_muscle_bench_05",
		"prop_muscle_bench_06",
		"prop_muster_wboard_01",
		"prop_muster_wboard_02",
		"prop_m_pack_int_01",
		"prop_necklace_board",
		"prop_news_disp_02a_s",
		"prop_new_drug_pack_01",
		"prop_nigel_bag_pickup",
		"prop_night_safe_01",
		"prop_notepad_01",
		"prop_notepad_02",
		"prop_novel_01",
		"prop_npc_phone",
		"prop_npc_phone_02",
		"prop_office_alarm_01",
		"prop_office_desk_01",
		"prop_offroad_bale01",
		"prop_offroad_barrel01",
		"prop_offroad_tyres01",
		"prop_off_chair_01",
		"prop_off_chair_03",
		"prop_off_chair_04",
		"prop_off_chair_04b",
		"prop_off_chair_04_s",
		"prop_off_chair_05",
		"prop_off_phone_01",
		"prop_oiltub_01",
		"prop_oiltub_02",
		"prop_oiltub_03",
		"prop_oiltub_05",
		"prop_oiltub_06",
		"prop_oil_derrick_01",
		"prop_oil_guage_01",
		"prop_oil_spool_02",
		"prop_oil_valve_01",
		"prop_oil_valve_02",
		"prop_oil_wellhead_01",
		"prop_oil_wellhead_03",
		"prop_oil_wellhead_04",
		"prop_oil_wellhead_05",
		"prop_oil_wellhead_06",
		"prop_oldplough1",
		"prop_old_boot",
		"prop_old_churn_01",
		"prop_old_churn_02",
		"prop_old_deck_chair",
		"prop_old_deck_chair_02",
		"prop_old_farm_01",
		"prop_old_farm_02",
		"prop_old_wood_chair",
		"prop_old_wood_chair_lod",
		"prop_orang_can_01",
		"prop_outdoor_fan_01",
		"prop_out_door_speaker",
		"prop_overalls_01",
		"prop_owl_totem_01",
		"prop_paints_can01",
		"prop_paints_can02",
		"prop_paints_can03",
		"prop_paints_can04",
		"prop_paints_can05",
		"prop_paints_can06",
		"prop_paints_can07",
		"prop_paint_brush01",
		"prop_paint_brush02",
		"prop_paint_brush03",
		"prop_paint_brush04",
		"prop_paint_brush05",
		"prop_paint_roller",
		"prop_paint_spray01a",
		"prop_paint_spray01b",
		"prop_paint_stepl01",
		"prop_paint_stepl01b",
		"prop_paint_stepl02",
		"prop_paint_tray",
		"prop_paint_wpaper01",
		"prop_pallettruck_01",
		"prop_palm_fan_02_a",
		"prop_palm_fan_02_b",
		"prop_palm_fan_03_a",
		"prop_palm_fan_03_b",
		"prop_palm_fan_03_c",
		"prop_palm_fan_03_d",
		"prop_palm_fan_04_a",
		"prop_palm_fan_04_b",
		"prop_palm_fan_04_c",
		"prop_palm_fan_04_d",
		"prop_palm_huge_01a",
		"prop_palm_huge_01b",
		"prop_palm_med_01a",
		"prop_palm_med_01b",
		"prop_palm_med_01c",
		"prop_palm_med_01d",
		"prop_palm_sm_01a",
		"prop_palm_sm_01d",
		"prop_palm_sm_01e",
		"prop_palm_sm_01f",
		"prop_paper_bag_01",
		"prop_paper_bag_small",
		"prop_paper_ball",
		"prop_paper_box_01",
		"prop_paper_box_02",
		"prop_paper_box_03",
		"prop_paper_box_04",
		"prop_paper_box_05",
		"prop_pap_camera_01",
		"prop_parachute",
		"prop_parapack_01",
		"prop_parasol_01",
		"prop_parasol_01_b",
		"prop_parasol_01_c",
		"prop_parasol_01_down",
		"prop_parasol_02",
		"prop_parasol_02_b",
		"prop_parasol_02_c",
		"prop_parasol_03",
		"prop_parasol_03_b",
		"prop_parasol_03_c",
		"prop_parasol_04e",
		"prop_parasol_04e_lod1",
		"prop_parasol_bh_48",
		"prop_parking_sign_06",
		"prop_parking_sign_07",
		"prop_parking_sign_1",
		"prop_parking_sign_2",
		"prop_parking_wand_01",
		"prop_park_ticket_01",
		"prop_partsbox_01",
		"prop_passport_01",
		"prop_patio_heater_01",
		"prop_patio_lounger1",
		"prop_patio_lounger1b",
		"prop_patio_lounger1_table",
		"prop_patio_lounger_2",
		"prop_patio_lounger_3",
		"prop_patriotneon",
		"prop_paynspray_door_l",
		"prop_paynspray_door_r",
		"prop_pc_01a",
		"prop_pc_02a",
		"prop_peanut_bowl_01",
		"prop_ped_pic_01",
		"prop_ped_pic_01_sm",
		"prop_ped_pic_02",
		"prop_ped_pic_02_sm",
		"prop_ped_pic_03",
		"prop_ped_pic_03_sm",
		"prop_ped_pic_04",
		"prop_ped_pic_04_sm",
		"prop_ped_pic_05",
		"prop_ped_pic_05_sm",
		"prop_ped_pic_06",
		"prop_ped_pic_06_sm",
		"prop_ped_pic_07",
		"prop_ped_pic_07_sm",
		"prop_ped_pic_08",
		"prop_ped_pic_08_sm",
		"prop_pencil_01",
		"prop_pharm_sign_01",
		"prop_phonebox_05a",
		"prop_phone_ing",
		"prop_phone_ing_02",
		"prop_phone_ing_03",
		"prop_phone_overlay_01",
		"prop_phone_overlay_02",
		"prop_phone_overlay_anim",
		"prop_phone_proto",
		"prop_phone_proto_back",
		"prop_phone_proto_battery",
		"prop_picnictable_02",
		"prop_piercing_gun",
		"prop_pier_kiosk_01",
		"prop_pier_kiosk_02",
		"prop_pier_kiosk_03",
		"prop_pile_dirt_01",
		"prop_pile_dirt_02",
		"prop_pile_dirt_03",
		"prop_pile_dirt_04",
		"prop_pile_dirt_06",
		"prop_pile_dirt_07",
		"prop_ping_pong",
		"prop_pipes_01a",
		"prop_pipes_01b",
		"prop_pipes_03b",
		"prop_pipes_04a",
		"prop_pipes_05a",
		"prop_pipes_conc_01",
		"prop_pipes_conc_02",
		"prop_pipe_single_01",
		"prop_pistol_holster",
		"prop_pitcher_01_cs",
		"prop_pizza_box_01",
		"prop_pizza_box_02",
		"prop_pizza_oven_01",
		"prop_planer_01",
		"prop_plant_01a",
		"prop_plant_01b",
		"prop_plant_base_01",
		"prop_plant_base_02",
		"prop_plant_base_03",
		"prop_plant_cane_01a",
		"prop_plant_cane_01b",
		"prop_plant_cane_02a",
		"prop_plant_cane_02b",
		"prop_plant_clover_01",
		"prop_plant_clover_02",
		"prop_plant_fern_01a",
		"prop_plant_fern_01b",
		"prop_plant_fern_02a",
		"prop_plant_fern_02b",
		"prop_plant_fern_02c",
		"prop_plant_flower_01",
		"prop_plant_flower_02",
		"prop_plant_flower_03",
		"prop_plant_flower_04",
		"prop_plant_group_01",
		"prop_plant_group_02",
		"prop_plant_group_03",
		"prop_plant_group_04",
		"prop_plant_group_05",
		"prop_plant_group_05b",
		"prop_plant_group_05c",
		"prop_plant_group_05d",
		"prop_plant_group_06a",
		"prop_plant_group_06b",
		"prop_plant_group_06c",
		"prop_plant_int_02a",
		"prop_plant_int_02b",
		"prop_plant_int_05a",
		"prop_plant_int_05b",
		"prop_plant_int_06a",
		"prop_plant_int_06b",
		"prop_plant_int_06c",
		"prop_plant_paradise",
		"prop_plant_paradise_b",
		"prop_plastic_cup_02",
		"prop_plas_barier_01a",
		"prop_plate_04",
		"prop_plate_stand_01",
		"prop_plate_warmer",
		"prop_player_gasmask",
		"prop_player_phone_01",
		"prop_player_phone_02",
		"prop_pliers_01",
		"prop_plywoodpile_01a",
		"prop_plywoodpile_01b",
		"prop_podium_mic",
		"prop_police_door_l",
		"prop_police_door_l_dam",
		"prop_police_door_r",
		"prop_police_door_r_dam",
		"prop_police_door_surround",
		"prop_police_phone",
		"prop_police_radio_handset",
		"prop_police_radio_main",
		"prop_poly_bag_01",
		"prop_poly_bag_money",
		"prop_poolball_1",
		"prop_poolball_10",
		"prop_poolball_11",
		"prop_poolball_12",
		"prop_poolball_13",
		"prop_poolball_14",
		"prop_poolball_15",
		"prop_poolball_2",
		"prop_poolball_3",
		"prop_poolball_4",
		"prop_poolball_5",
		"prop_poolball_6",
		"prop_poolball_7",
		"prop_poolball_8",
		"prop_poolball_9",
		"prop_poolball_cue",
		"prop_poolskimmer",
		"prop_pooltable_02",
		"prop_pooltable_3b",
		"prop_pool_ball_01",
		"prop_pool_cue",
		"prop_pool_rack_01",
		"prop_pool_rack_02",
		"prop_pool_tri",
		"prop_porn_mag_01",
		"prop_porn_mag_02",
		"prop_porn_mag_03",
		"prop_porn_mag_04",
		"prop_portable_hifi_01",
		"prop_portacabin01",
		"prop_portasteps_01",
		"prop_portasteps_02",
		"prop_postcard_rack",
		"prop_poster_tube_01",
		"prop_poster_tube_02",
		"prop_postit_drive",
		"prop_postit_gun",
		"prop_postit_it",
		"prop_postit_lock",
		"prop_potatodigger",
		"prop_pot_01",
		"prop_pot_02",
		"prop_pot_03",
		"prop_pot_04",
		"prop_pot_05",
		"prop_pot_06",
		"prop_pot_plant_02a",
		"prop_pot_plant_02b",
		"prop_pot_plant_02c",
		"prop_pot_plant_02d",
		"prop_pot_plant_03a",
		"prop_pot_plant_04a",
		"prop_pot_plant_05d_l1",
		"prop_pot_plant_bh1",
		"prop_pot_rack",
		"prop_power_cell",
		"prop_power_cord_01",
		"prop_premier_fence_01",
		"prop_premier_fence_02",
		"prop_printer_01",
		"prop_printer_02",
		"prop_pris_bars_01",
		"prop_pris_bench_01",
		"prop_pris_door_01_l",
		"prop_pris_door_01_r",
		"prop_pris_door_02",
		"prop_pris_door_03",
		"prop_prlg_gravestone_05a_l1",
		"prop_prlg_gravestone_06a",
		"prop_projector_overlay",
		"prop_prologue_phone",
		"prop_prop_tree_01",
		"prop_prop_tree_02",
		"prop_protest_sign_01",
		"prop_protest_table_01",
		"prop_prototype_minibomb",
		"prop_proxy_chateau_table",
		"prop_punch_bag_l",
		"prop_pylon_01",
		"prop_pylon_02",
		"prop_pylon_03",
		"prop_pylon_04",
		"prop_p_jack_03_col",
		"prop_p_spider_01a",
		"prop_p_spider_01c",
		"prop_p_spider_01d",
		"prop_ql_revolving_door",
		"prop_quad_grid_line",
		"prop_radiomast01",
		"prop_radiomast02",
		"prop_rad_waste_barrel_01",
		"prop_ragganeon",
		"prop_rag_01",
		"prop_railsleepers01",
		"prop_railsleepers02",
		"prop_railstack01",
		"prop_railstack02",
		"prop_railstack03",
		"prop_railstack04",
		"prop_railstack05",
		"prop_rail_boxcar",
		"prop_rail_boxcar2",
		"prop_rail_boxcar3",
		"prop_rail_boxcar4",
		"prop_rail_boxcar5",
		"prop_rail_boxcar5_d",
		"prop_rail_buffer_01",
		"prop_rail_buffer_02",
		"prop_rail_controller",
		"prop_rail_crane_01",
		"prop_rail_points01",
		"prop_rail_points02",
		"prop_rail_sigbox01",
		"prop_rail_sigbox02",
		"prop_rail_signals02",
		"prop_rail_tankcar",
		"prop_rail_tankcar2",
		"prop_rail_tankcar3",
		"prop_rail_wellcar",
		"prop_rail_wellcar2",
		"prop_range_target_01",
		"prop_range_target_02",
		"prop_range_target_03",
		"prop_rebar_pile01",
		"prop_recyclebin_02a",
		"prop_recyclebin_02b",
		"prop_recyclebin_02_c",
		"prop_recyclebin_02_d",
		"prop_recyclebin_03_a",
		"prop_recyclebin_04_a",
		"prop_recyclebin_04_b",
		"prop_recyclebin_05_a",
		"prop_ret_door",
		"prop_ret_door_02",
		"prop_ret_door_03",
		"prop_ret_door_04",
		"prop_rf_conc_pillar",
		"prop_riding_crop_01",
		"prop_riot_shield",
		"prop_rio_del_01",
		"prop_roadcone01a",
		"prop_roadcone01b",
		"prop_roadcone01c",
		"prop_roadcone02a",
		"prop_roadcone02b",
		"prop_roadcone02c",
		"prop_roadheader_01",
		"prop_rock_1_a",
		"prop_rock_1_b",
		"prop_rock_1_c",
		"prop_rock_1_d",
		"prop_rock_1_e",
		"prop_rock_1_f",
		"prop_rock_1_g",
		"prop_rock_1_h",
		"prop_rock_1_i",
		"prop_rock_2_a",
		"prop_rock_2_c",
		"prop_rock_2_d",
		"prop_rock_2_f",
		"prop_rock_2_g",
		"prop_rock_3_a",
		"prop_rock_3_b",
		"prop_rock_3_c",
		"prop_rock_3_d",
		"prop_rock_3_e",
		"prop_rock_3_f",
		"prop_rock_3_g",
		"prop_rock_3_h",
		"prop_rock_3_i",
		"prop_rock_3_j",
		"prop_rock_4_c",
		"prop_rock_4_d",
		"prop_rock_chair_01",
		"prop_rolled_sock_01",
		"prop_rolled_sock_02",
		"prop_rolled_yoga_mat",
		"prop_roller_car_01",
		"prop_roller_car_02",
		"prop_ron_door_01",
		"prop_roofpipe_01",
		"prop_roofpipe_02",
		"prop_roofpipe_03",
		"prop_roofpipe_04",
		"prop_roofpipe_05",
		"prop_roofpipe_06",
		"prop_roofvent_011a",
		"prop_roofvent_01a",
		"prop_roofvent_01b",
		"prop_roofvent_02a",
		"prop_roofvent_02b",
		"prop_roofvent_03a",
		"prop_roofvent_04a",
		"prop_roofvent_05a",
		"prop_roofvent_05b",
		"prop_roofvent_07a",
		"prop_roofvent_08a",
		"prop_roofvent_09a",
		"prop_roofvent_10a",
		"prop_roofvent_10b",
		"prop_roofvent_11b",
		"prop_roofvent_11c",
		"prop_roofvent_12a",
		"prop_roofvent_13a",
		"prop_roofvent_15a",
		"prop_roofvent_16a",
		"prop_rope_family_3",
		"prop_rope_hook_01",
		"prop_roundbailer01",
		"prop_roundbailer02",
		"prop_rub_bike_01",
		"prop_rub_bike_02",
		"prop_rub_bike_03",
		"prop_rub_binbag_sd_01",
		"prop_rub_binbag_sd_02",
		"prop_rub_busdoor_01",
		"prop_rub_busdoor_02",
		"prop_rub_buswreck_01",
		"prop_rub_buswreck_03",
		"prop_rub_buswreck_06",
		"prop_rub_cabinet",
		"prop_rub_cabinet01",
		"prop_rub_cabinet02",
		"prop_rub_cabinet03",
		"prop_rub_cage01a",
		"prop_rub_carpart_02",
		"prop_rub_carpart_03",
		"prop_rub_carpart_04",
		"prop_rub_chassis_01",
		"prop_rub_chassis_02",
		"prop_rub_chassis_03",
		"prop_rub_cont_01a",
		"prop_rub_cont_01b",
		"prop_rub_cont_01c",
		"prop_rub_flotsam_01",
		"prop_rub_flotsam_02",
		"prop_rub_flotsam_03",
		"prop_rub_frklft",
		"prop_rub_litter_01",
		"prop_rub_litter_02",
		"prop_rub_litter_03",
		"prop_rub_litter_03b",
		"prop_rub_litter_03c",
		"prop_rub_litter_04",
		"prop_rub_litter_04b",
		"prop_rub_litter_05",
		"prop_rub_litter_06",
		"prop_rub_litter_07",
		"prop_rub_litter_09",
		"prop_rub_litter_8",
		"prop_rub_matress_01",
		"prop_rub_matress_02",
		"prop_rub_matress_03",
		"prop_rub_matress_04",
		"prop_rub_monitor",
		"prop_rub_pile_01",
		"prop_rub_pile_02",
		"prop_rub_planks_01",
		"prop_rub_planks_02",
		"prop_rub_planks_03",
		"prop_rub_planks_04",
		"prop_rub_railwreck_1",
		"prop_rub_railwreck_2",
		"prop_rub_railwreck_3",
		"prop_rub_scrap_02",
		"prop_rub_scrap_03",
		"prop_rub_scrap_04",
		"prop_rub_scrap_05",
		"prop_rub_scrap_06",
		"prop_rub_scrap_07",
		"prop_rub_stool",
		"prop_rub_sunktyre",
		"prop_rub_t34",
		"prop_rub_trainers_01",
		"prop_rub_trolley01a",
		"prop_rub_trolley02a",
		"prop_rub_trolley03a",
		"prop_rub_trukwreck_1",
		"prop_rub_trukwreck_2",
		"prop_rub_tyre_01",
		"prop_rub_tyre_02",
		"prop_rub_tyre_03",
		"prop_rub_tyre_dam1",
		"prop_rub_tyre_dam2",
		"prop_rub_tyre_dam3",
		"prop_rub_washer_01",
		"prop_rub_wheel_01",
		"prop_rub_wheel_02",
		"prop_rub_wreckage_3",
		"prop_rub_wreckage_4",
		"prop_rub_wreckage_5",
		"prop_rub_wreckage_6",
		"prop_rub_wreckage_7",
		"prop_rub_wreckage_8",
		"prop_rub_wreckage_9",
		"prop_rural_windmill_l1",
		"prop_rural_windmill_l2",
		"prop_rus_olive",
		"prop_rus_olive_wint",
		"prop_sacktruck_01",
		"prop_sacktruck_02a",
		"prop_safety_glasses",
		"prop_sam_01",
		"prop_sandwich_01",
		"prop_satdish_2_a",
		"prop_satdish_2_f",
		"prop_satdish_2_g",
		"prop_satdish_3_b",
		"prop_satdish_3_c",
		"prop_satdish_3_d",
		"prop_satdish_l_01",
		"prop_satdish_s_03",
		"prop_satdish_s_05a",
		"prop_satdish_s_05b",
		"prop_sc1_06_gate_l",
		"prop_sc1_06_gate_r",
		"prop_sc1_12_door",
		"prop_sc1_21_g_door_01",
		"prop_scaffold_pole",
		"prop_scafold_01a",
		"prop_scafold_01c",
		"prop_scafold_01f",
		"prop_scafold_02a",
		"prop_scafold_02c",
		"prop_scafold_03a",
		"prop_scafold_03b",
		"prop_scafold_03c",
		"prop_scafold_03f",
		"prop_scafold_04a",
		"prop_scafold_05a",
		"prop_scafold_06a",
		"prop_scafold_06b",
		"prop_scafold_06c",
		"prop_scafold_07a",
		"prop_scafold_08a",
		"prop_scafold_09a",
		"prop_scafold_frame1a",
		"prop_scafold_frame1b",
		"prop_scafold_frame1c",
		"prop_scafold_frame1f",
		"prop_scafold_frame2a",
		"prop_scafold_frame2b",
		"prop_scafold_frame2c",
		"prop_scafold_frame3a",
		"prop_scafold_frame3c",
		"prop_scafold_rail_01",
		"prop_scafold_rail_02",
		"prop_scafold_rail_03",
		"prop_scafold_xbrace",
		"prop_scalpel",
		"prop_scn_police_torch",
		"prop_scourer_01",
		"prop_scrap_2_crate",
		"prop_scrap_win_01",
		"prop_scrim_01",
		"prop_scythemower",
		"prop_section_garage_01",
		"prop_securityvan_lightrig",
		"prop_security_case_01",
		"prop_security_case_02",
		"prop_sec_gate_01b",
		"prop_sec_gate_01c",
		"prop_sec_gate_01d",
		"prop_set_generator_01",
		"prop_sewing_fabric",
		"prop_sewing_machine",
		"prop_sglasses_stand_01",
		"prop_sglasses_stand_02",
		"prop_sglasses_stand_02b",
		"prop_sglasses_stand_03",
		"prop_sglasss_1b_lod",
		"prop_sglasss_1_lod",
		"prop_shamal_crash",
		"prop_shelves_01",
		"prop_shelves_02",
		"prop_shelves_03",
		"prop_shopping_bags01",
		"prop_shopping_bags02",
		"prop_shop_front_door_l",
		"prop_shop_front_door_r",
		"prop_shots_glass_cs",
		"prop_shower_rack_01",
		"prop_shower_towel",
		"prop_showroom_door_l",
		"prop_showroom_door_r",
		"prop_showroom_glass_1b",
		"prop_shredder_01",
		"prop_shrub_rake",
		"prop_shuttering01",
		"prop_shuttering02",
		"prop_shuttering03",
		"prop_shuttering04",
		"prop_sh_beer_pissh_01",
		"prop_sh_bong_01",
		"prop_sh_cigar_01",
		"prop_sh_joint_01",
		"prop_sh_mr_rasp_01",
		"prop_sh_shot_glass",
		"prop_sh_tall_glass",
		"prop_sh_tt_fridgedoor",
		"prop_sh_wine_glass",
		"prop_side_lights",
		"prop_side_spreader",
		"prop_sign_airp_01a",
		"prop_sign_airp_02a",
		"prop_sign_airp_02b",
		"prop_sign_big_01",
		"prop_sign_mallet",
		"prop_sign_road_04g_l1",
		"prop_single_grid_line",
		"prop_single_rose",
		"prop_sink_02",
		"prop_sink_04",
		"prop_sink_05",
		"prop_sink_06",
		"prop_skate_flatramp",
		"prop_skate_funbox",
		"prop_skate_halfpipe",
		"prop_skate_kickers",
		"prop_skate_quartpipe",
		"prop_skate_rail",
		"prop_skate_spiner",
		"prop_skid_chair_01",
		"prop_skid_chair_02",
		"prop_skid_chair_03",
		"prop_skid_sleepbag_1",
		"prop_skid_tent_01",
		"prop_skid_tent_01b",
		"prop_skid_tent_03",
		"prop_skip_01a",
		"prop_skip_02a",
		"prop_skip_03",
		"prop_skip_04",
		"prop_skip_05a",
		"prop_skip_05b",
		"prop_skip_06a",
		"prop_skip_08a",
		"prop_skip_08b",
		"prop_skip_10a",
		"prop_skip_rope_01",
		"prop_skunk_bush_01",
		"prop_skylight_01",
		"prop_skylight_02",
		"prop_skylight_03",
		"prop_skylight_04",
		"prop_skylight_05",
		"prop_skylight_06b",
		"prop_skylight_06c",
		"prop_sky_cover_01",
		"prop_slacks_01",
		"prop_slacks_02",
		"prop_sluicegate",
		"prop_sluicegatel",
		"prop_sluicegater",
		"prop_slush_dispenser",
		"prop_sm1_11_doorl",
		"prop_sm1_11_doorr",
		"prop_sm1_11_garaged",
		"prop_smg_holster_01",
		"prop_sm_10_mp_door",
		"prop_sm_14_mp_gar",
		"prop_sm_19_clock",
		"prop_sm_27_door",
		"prop_sm_27_gate",
		"prop_sm_27_gate_02",
		"prop_sm_27_gate_03",
		"prop_sm_27_gate_04",
		"prop_sm_locker_door",
		"prop_snow_bailer_01",
		"prop_snow_barrel_pile_03",
		"prop_snow_bench_01",
		"prop_snow_bin_01",
		"prop_snow_bin_02",
		"prop_snow_bush_01_a",
		"prop_snow_bush_02_a",
		"prop_snow_bush_02_b",
		"prop_snow_bush_03",
		"prop_snow_bush_04",
		"prop_snow_bush_04b",
		"prop_snow_cam_03",
		"prop_snow_cam_03a",
		"prop_snow_diggerbkt_01",
		"prop_snow_dumpster_01",
		"prop_snow_elecbox_16",
		"prop_snow_facgate_01",
		"prop_snow_field_01",
		"prop_snow_field_02",
		"prop_snow_field_03",
		"prop_snow_field_04",
		"prop_snow_flower_01",
		"prop_snow_flower_02",
		"prop_snow_fnclink_03crnr2",
		"prop_snow_fnclink_03h",
		"prop_snow_fnclink_03i",
		"prop_snow_fncwood_14a",
		"prop_snow_fncwood_14b",
		"prop_snow_fncwood_14c",
		"prop_snow_fncwood_14d",
		"prop_snow_fncwood_14e",
		"prop_snow_fnc_01",
		"prop_snow_gate_farm_03",
		"prop_snow_grain_01",
		"prop_snow_grass_01",
		"prop_snow_light_01",
		"prop_snow_oldlight_01b",
		"prop_snow_rail_signals02",
		"prop_snow_rub_trukwreck_2",
		"prop_snow_side_spreader_01",
		"prop_snow_streetlight01",
		"prop_snow_streetlight_01_frag_",
		"prop_snow_sub_frame_01a",
		"prop_snow_sub_frame_04b",
		"prop_snow_telegraph_01a",
		"prop_snow_telegraph_02a",
		"prop_snow_telegraph_03",
		"prop_snow_traffic_rail_1a",
		"prop_snow_traffic_rail_1b",
		"prop_snow_trailer01",
		"prop_snow_tree_03_e",
		"prop_snow_tree_03_h",
		"prop_snow_tree_03_i",
		"prop_snow_tree_04_d",
		"prop_snow_tree_04_f",
		"prop_snow_truktrailer_01a",
		"prop_snow_tyre_01",
		"prop_snow_t_ml_01",
		"prop_snow_t_ml_02",
		"prop_snow_t_ml_03",
		"prop_snow_wall_light_15a",
		"prop_snow_watertower01",
		"prop_snow_watertower01_l2",
		"prop_snow_watertower03",
		"prop_snow_woodpile_04a",
		"prop_soap_disp_01",
		"prop_solarpanel_01",
		"prop_solarpanel_02",
		"prop_solarpanel_03",
		"prop_sol_chair",
		"prop_space_pistol",
		"prop_space_rifle",
		"prop_speaker_01",
		"prop_speaker_02",
		"prop_speaker_03",
		"prop_speaker_05",
		"prop_speaker_06",
		"prop_speaker_07",
		"prop_speaker_08",
		"prop_speedball_01",
		"prop_sponge_01",
		"prop_sports_clock_01",
		"prop_spot_01",
		"prop_spot_clamp",
		"prop_spot_clamp_02",
		"prop_sprayer",
		"prop_spraygun_01",
		"prop_spray_backpack_01",
		"prop_spray_jackframe",
		"prop_spray_jackleg",
		"prop_sprink_crop_01",
		"prop_sprink_golf_01",
		"prop_sprink_park_01",
		"prop_spycam",
		"prop_squeegee",
		"prop_ss1_05_mp_door",
		"prop_ss1_08_mp_door_l",
		"prop_ss1_08_mp_door_r",
		"prop_ss1_10_door_l",
		"prop_ss1_10_door_r",
		"prop_ss1_14_garage_door",
		"prop_ss1_mpint_door_l",
		"prop_ss1_mpint_door_r",
		"prop_ss1_mpint_garage",
		"prop_ss1_mpint_garage_cl",
		"prop_stag_do_rope",
		"prop_start_finish_line_01",
		"prop_start_grid_01",
		"prop_staticmixer_01",
		"prop_stat_pack_01",
		"prop_steam_basket_01",
		"prop_steam_basket_02",
		"prop_steps_big_01",
		"prop_stickbfly",
		"prop_stickhbird",
		"prop_still",
		"prop_stockade_wheel",
		"prop_stockade_wheel_flat",
		"prop_stool_01",
		"prop_storagetank_01",
		"prop_storagetank_02",
		"prop_storagetank_03",
		"prop_storagetank_03a",
		"prop_storagetank_03b",
		"prop_storagetank_04",
		"prop_storagetank_05",
		"prop_storagetank_06",
		"prop_storagetank_07a",
		"prop_stripmenu",
		"prop_strip_door_01",
		"prop_strip_pole_01",
		"prop_studio_light_02",
		"prop_studio_light_03",
		"prop_sub_chunk_01",
		"prop_sub_cover_01",
		"prop_sub_crane_hook",
		"prop_sub_frame_01a",
		"prop_sub_frame_01b",
		"prop_sub_frame_01c",
		"prop_sub_frame_02a",
		"prop_sub_frame_03a",
		"prop_sub_frame_04a",
		"prop_sub_frame_04b",
		"prop_sub_gantry",
		"prop_sub_release",
		"prop_sub_trans_01a",
		"prop_sub_trans_02a",
		"prop_sub_trans_03a",
		"prop_sub_trans_04a",
		"prop_sub_trans_05b",
		"prop_sub_trans_06b",
		"prop_suitcase_01",
		"prop_suitcase_01b",
		"prop_suitcase_01c",
		"prop_suitcase_01d",
		"prop_suitcase_02",
		"prop_suitcase_03",
		"prop_suitcase_03b",
		"prop_surf_board_ldn_01",
		"prop_surf_board_ldn_02",
		"prop_surf_board_ldn_03",
		"prop_surf_board_ldn_04",
		"prop_syringe_01",
		"prop_s_pine_dead_01",
		"prop_tablesaw_01",
		"prop_tablesmall_01",
		"prop_table_02",
		"prop_table_03b_cs",
		"prop_table_04",
		"prop_table_04_chr",
		"prop_table_05",
		"prop_table_05_chr",
		"prop_table_06",
		"prop_table_06_chr",
		"prop_table_07",
		"prop_table_07_l1",
		"prop_table_08",
		"prop_table_08_chr",
		"prop_table_08_side",
		"prop_table_mic_01",
		"prop_table_para_comb_04",
		"prop_table_tennis",
		"prop_table_ten_bat",
		"prop_taco_01",
		"prop_taco_02",
		"prop_tail_gate_col",
		"prop_tapeplayer_01",
		"prop_target_arm",
		"prop_target_arm_b",
		"prop_target_arm_long",
		"prop_target_arm_sm",
		"prop_target_backboard",
		"prop_target_backboard_b",
		"prop_target_blue",
		"prop_target_blue_arrow",
		"prop_target_bull",
		"prop_target_bull_b",
		"prop_target_comp_metal",
		"prop_target_comp_wood",
		"prop_target_frame_01",
		"prop_target_inner1",
		"prop_target_inner2",
		"prop_target_inner2_b",
		"prop_target_inner3",
		"prop_target_inner3_b",
		"prop_target_inner_b",
		"prop_target_orange_arrow",
		"prop_target_oran_cross",
		"prop_target_ora_purp_01",
		"prop_target_purp_arrow",
		"prop_target_purp_cross",
		"prop_target_red",
		"prop_target_red_arrow",
		"prop_target_red_blue_01",
		"prop_target_red_cross",
		"prop_tarp_strap",
		"prop_taxi_meter_1",
		"prop_taxi_meter_2",
		"prop_tea_trolly",
		"prop_tea_urn",
		"prop_telegraph_01a",
		"prop_telegraph_01b",
		"prop_telegraph_01c",
		"prop_telegraph_01d",
		"prop_telegraph_01e",
		"prop_telegraph_01f",
		"prop_telegraph_01g",
		"prop_telegraph_02a",
		"prop_telegraph_02b",
		"prop_telegraph_03",
		"prop_telegraph_04a",
		"prop_telegraph_04b",
		"prop_telegraph_05a",
		"prop_telegraph_05b",
		"prop_telegraph_05c",
		"prop_telegraph_06a",
		"prop_telegraph_06b",
		"prop_telegraph_06c",
		"prop_telegwall_01a",
		"prop_telegwall_01b",
		"prop_telegwall_02a",
		"prop_telegwall_03a",
		"prop_telegwall_03b",
		"prop_telegwall_04a",
		"prop_telescope",
		"prop_telescope_01",
		"prop_temp_block_blocker",
		"prop_tennis_bag_01",
		"prop_tennis_ball",
		"prop_tennis_ball_lobber",
		"prop_tennis_rack_01",
		"prop_tennis_rack_01b",
		"prop_test_boulder_01",
		"prop_test_boulder_02",
		"prop_test_boulder_03",
		"prop_test_boulder_04",
		"prop_test_elevator",
		"prop_test_elevator_dl",
		"prop_test_elevator_dr",
		"prop_tick",
		"prop_tick_02",
		"prop_till_01_dam",
		"prop_till_02",
		"prop_till_03",
		"prop_time_capsule_01",
		"prop_tint_towel",
		"prop_tint_towels_01",
		"prop_tint_towels_01b",
		"prop_toaster_01",
		"prop_toaster_02",
		"prop_toiletfoot_static",
		"prop_toilet_01",
		"prop_toilet_02",
		"prop_toilet_brush_01",
		"prop_toilet_roll_01",
		"prop_toilet_roll_02",
		"prop_toilet_roll_05",
		"prop_toilet_shamp_01",
		"prop_toilet_shamp_02",
		"prop_toilet_soap_01",
		"prop_toilet_soap_02",
		"prop_toilet_soap_03",
		"prop_toilet_soap_04",
		"prop_toolchest_01",
		"prop_toolchest_02",
		"prop_toolchest_03",
		"prop_toolchest_04",
		"prop_toolchest_05",
		"prop_tool_adjspanner",
		"prop_tool_bench01",
		"prop_tool_bluepnt",
		"prop_tool_box_01",
		"prop_tool_box_02",
		"prop_tool_box_03",
		"prop_tool_box_04",
		"prop_tool_box_05",
		"prop_tool_box_06",
		"prop_tool_box_07",
		"prop_tool_broom",
		"prop_tool_broom2",
		"prop_tool_broom2_l1",
		"prop_tool_cable01",
		"prop_tool_cable02",
		"prop_tool_consaw",
		"prop_tool_drill",
		"prop_tool_fireaxe",
		"prop_tool_hammer",
		"prop_tool_hardhat",
		"prop_tool_jackham",
		"prop_tool_mallet",
		"prop_tool_mopbucket",
		"prop_tool_nailgun",
		"prop_tool_pickaxe",
		"prop_tool_pliers",
		"prop_tool_rake",
		"prop_tool_rake_l1",
		"prop_tool_sawhorse",
		"prop_tool_screwdvr01",
		"prop_tool_screwdvr02",
		"prop_tool_screwdvr03",
		"prop_tool_shovel",
		"prop_tool_shovel006",
		"prop_tool_shovel2",
		"prop_tool_shovel3",
		"prop_tool_shovel4",
		"prop_tool_shovel5",
		"prop_tool_sledgeham",
		"prop_tool_spanner01",
		"prop_tool_spanner02",
		"prop_tool_spanner03",
		"prop_tool_torch",
		"prop_tool_wrench",
		"prop_toothbrush_01",
		"prop_toothb_cup_01",
		"prop_toothpaste_01",
		"prop_tornado_wheel",
		"prop_torture_01",
		"prop_torture_ch_01",
		"prop_tourist_map_01",
		"prop_towel2_01",
		"prop_towel2_02",
		"prop_towel_01",
		"prop_towel_rail_01",
		"prop_towel_rail_02",
		"prop_towel_shelf_01",
		"prop_towercrane_01a",
		"prop_towercrane_02a",
		"prop_towercrane_02b",
		"prop_towercrane_02c",
		"prop_towercrane_02d",
		"prop_towercrane_02e",
		"prop_towercrane_02el",
		"prop_towercrane_02el2",
		"prop_traffic_rail_1c",
		"prop_traffic_rail_2",
		"prop_trailer01",
		"prop_trailer01_up",
		"prop_trailer_01_new",
		"prop_trailer_door_closed",
		"prop_trailer_door_open",
		"prop_trailer_monitor_01",
		"prop_trailr_base",
		"prop_trailr_base_static",
		"prop_train_ticket_02",
		"prop_tram_pole_double01",
		"prop_tram_pole_double02",
		"prop_tram_pole_double03",
		"prop_tram_pole_roadside",
		"prop_tram_pole_single01",
		"prop_tram_pole_single02",
		"prop_tram_pole_wide01",
		"prop_tree_birch_01",
		"prop_tree_birch_02",
		"prop_tree_birch_03",
		"prop_tree_birch_03b",
		"prop_tree_birch_04",
		"prop_tree_birch_05",
		"prop_tree_cedar_02",
		"prop_tree_cedar_03",
		"prop_tree_cedar_04",
		"prop_tree_cedar_s_01",
		"prop_tree_cedar_s_02",
		"prop_tree_cedar_s_04",
		"prop_tree_cedar_s_05",
		"prop_tree_cedar_s_06",
		"prop_tree_cypress_01",
		"prop_tree_eng_oak_01",
		"prop_tree_eucalip_01",
		"prop_tree_fallen_01",
		"prop_tree_fallen_02",
		"prop_tree_fallen_pine_01",
		"prop_tree_jacada_01",
		"prop_tree_jacada_02",
		"prop_tree_lficus_02",
		"prop_tree_lficus_03",
		"prop_tree_lficus_05",
		"prop_tree_lficus_06",
		"prop_tree_log_01",
		"prop_tree_log_02",
		"prop_tree_maple_02",
		"prop_tree_maple_03",
		"prop_tree_mquite_01",
		"prop_tree_oak_01",
		"prop_tree_olive_01",
		"prop_tree_pine_01",
		"prop_tree_pine_02",
		"prop_tree_stump_01",
		"prop_trevor_rope_01",
		"prop_trev_sec_id",
		"prop_trev_tv_01",
		"prop_triple_grid_line",
		"prop_tri_finish_banner",
		"prop_tri_pod",
		"prop_tri_pod_lod",
		"prop_tri_start_banner",
		"prop_tri_table_01",
		"prop_trough1",
		"prop_truktrailer_01a",
		"prop_tshirt_box_02",
		"prop_tshirt_shelf_1",
		"prop_tshirt_shelf_2",
		"prop_tshirt_shelf_2a",
		"prop_tshirt_shelf_2b",
		"prop_tshirt_shelf_2c",
		"prop_tshirt_stand_01",
		"prop_tshirt_stand_01b",
		"prop_tshirt_stand_02",
		"prop_tshirt_stand_04",
		"prop_tt_screenstatic",
		"prop_tumbler_01",
		"prop_tumbler_01b",
		"prop_tumbler_01_empty",
		"prop_tunnel_liner01",
		"prop_tunnel_liner02",
		"prop_tunnel_liner03",
		"prop_turkey_leg_01",
		"prop_turnstyle_01",
		"prop_tv_02",
		"prop_tv_03_overlay",
		"prop_tv_04",
		"prop_tv_05",
		"prop_tv_06",
		"prop_tv_07",
		"prop_tv_cabinet_03",
		"prop_tv_cabinet_04",
		"prop_tv_cabinet_05",
		"prop_tv_cam_02",
		"prop_tv_flat_01",
		"prop_tv_flat_01_screen",
		"prop_tv_flat_02b",
		"prop_tv_flat_03",
		"prop_tv_flat_03b",
		"prop_tv_flat_michael",
		"prop_tv_test",
		"prop_tyre_rack_01",
		"prop_tyre_spike_01",
		"prop_t_coffe_table",
		"prop_t_shirt_ironing",
		"prop_t_shirt_row_01",
		"prop_t_shirt_row_02",
		"prop_t_shirt_row_02b",
		"prop_t_shirt_row_03",
		"prop_t_shirt_row_04",
		"prop_t_shirt_row_05l",
		"prop_t_shirt_row_05r",
		"prop_t_sofa",
		"prop_t_sofa_02",
		"prop_t_telescope_01b",
		"prop_umpire_01",
		"prop_utensil",
		"prop_valet_03",
		"prop_vault_shutter",
		"prop_vb_34_tencrt_lighting",
		"prop_vcr_01",
		"prop_veg_corn_01",
		"prop_veg_crop_01",
		"prop_veg_crop_02",
		"prop_veg_crop_04",
		"prop_veg_crop_04_leaf",
		"prop_veg_crop_05",
		"prop_veg_crop_06",
		"prop_veg_crop_orange",
		"prop_veg_crop_tr_01",
		"prop_veg_crop_tr_02",
		"prop_veg_grass_01_a",
		"prop_veg_grass_01_b",
		"prop_veg_grass_01_c",
		"prop_veg_grass_01_d",
		"prop_veg_grass_02_a",
		"prop_vehicle_hook",
		"prop_vend_coffe_01",
		"prop_vend_condom_01",
		"prop_vend_fags_01",
		"prop_vend_fridge01",
		"prop_vend_snak_01",
		"prop_venice_board_01",
		"prop_venice_board_02",
		"prop_venice_board_03",
		"prop_venice_counter_01",
		"prop_venice_counter_02",
		"prop_venice_counter_03",
		"prop_venice_counter_04",
		"prop_venice_shop_front_01",
		"prop_venice_sign_09",
		"prop_venice_sign_10",
		"prop_venice_sign_11",
		"prop_venice_sign_12",
		"prop_venice_sign_14",
		"prop_venice_sign_15",
		"prop_venice_sign_16",
		"prop_venice_sign_17",
		"prop_venice_sign_18",
		"prop_ventsystem_01",
		"prop_ventsystem_02",
		"prop_ventsystem_03",
		"prop_ventsystem_04",
		"prop_ven_market_stool",
		"prop_ven_market_table1",
		"prop_ven_shop_1_counter",
		"prop_vertdrill_01",
		"prop_voltmeter_01",
		"prop_v_15_cars_clock",
		"prop_v_5_bclock",
		"prop_v_bmike_01",
		"prop_v_cam_01",
		"prop_v_door_44",
		"prop_v_hook_s",
		"prop_v_m_phone_01",
		"prop_v_m_phone_o1s",
		"prop_v_parachute",
		"prop_waiting_seat_01",
		"prop_wait_bench_01",
		"prop_walllight_ld_01b",
		"prop_wall_light_08a",
		"prop_wall_light_10a",
		"prop_wall_light_10b",
		"prop_wall_light_10c",
		"prop_wall_light_11",
		"prop_wall_light_12",
		"prop_wall_light_17b",
		"prop_wall_light_18a",
		"prop_wall_vent_01",
		"prop_wall_vent_02",
		"prop_wall_vent_03",
		"prop_wall_vent_04",
		"prop_wall_vent_05",
		"prop_wall_vent_06",
		"prop_wardrobe_door_01",
		"prop_warehseshelf01",
		"prop_warehseshelf02",
		"prop_warehseshelf03",
		"prop_warninglight_01",
		"prop_washer_01",
		"prop_washer_02",
		"prop_washer_03",
		"prop_washing_basket_01",
		"prop_watercrate_01",
		"prop_wateringcan",
		"prop_watertower01",
		"prop_watertower02",
		"prop_watertower03",
		"prop_watertower04",
		"prop_waterwheela",
		"prop_waterwheelb",
		"prop_water_bottle",
		"prop_water_bottle_dark",
		"prop_water_corpse_01",
		"prop_water_corpse_02",
		"prop_water_ramp_01",
		"prop_water_ramp_02",
		"prop_water_ramp_03",
		"prop_weed_01",
		"prop_weed_02",
		"prop_weed_block_01",
		"prop_weed_bottle",
		"prop_weed_pallet",
		"prop_weed_tub_01",
		"prop_weed_tub_01b",
		"prop_weight_10k",
		"prop_weight_15k",
		"prop_weight_1_5k",
		"prop_weight_20k",
		"prop_weight_2_5k",
		"prop_weight_5k",
		"prop_weight_rack_01",
		"prop_weight_rack_02",
		"prop_welding_mask_01",
		"prop_weld_torch",
		"prop_wheat_grass_empty",
		"prop_wheat_grass_glass",
		"prop_wheelbarrow01a",
		"prop_wheelbarrow02a",
		"prop_wheelchair_01",
		"prop_wheel_01",
		"prop_wheel_02",
		"prop_wheel_03",
		"prop_wheel_04",
		"prop_wheel_05",
		"prop_wheel_06",
		"prop_wheel_hub_01",
		"prop_wheel_hub_02_lod_02",
		"prop_wheel_rim_01",
		"prop_wheel_rim_02",
		"prop_wheel_rim_03",
		"prop_wheel_rim_04",
		"prop_wheel_rim_05",
		"prop_wheel_tyre",
		"prop_whisk",
		"prop_white_keyboard",
		"prop_winch_hook_long",
		"prop_winch_hook_short",
		"prop_windmill2",
		"prop_windmill_01_l1",
		"prop_windmill_01_slod",
		"prop_windmill_01_slod2",
		"prop_windowbox_a",
		"prop_windowbox_b",
		"prop_windowbox_broken",
		"prop_windowbox_small",
		"prop_win_plug_01",
		"prop_win_plug_01_dam",
		"prop_win_trailer_ld",
		"prop_wok",
		"prop_woodpile_02a",
		"prop_worklight_01a_l1",
		"prop_worklight_04a",
		"prop_worklight_04b_l1",
		"prop_worklight_04c_l1",
		"prop_worklight_04d_l1",
		"prop_workwall_01",
		"prop_workwall_02",
		"prop_wreckedcart",
		"prop_wrecked_buzzard",
		"prop_w_board_blank",
		"prop_w_board_blank_2",
		"prop_w_fountain_01",
		"prop_w_r_cedar_01",
		"prop_w_r_cedar_dead",
		"prop_xmas_tree_int",
		"prop_xmas_ext",
		"prop_yacht_lounger",
		"prop_yacht_seat_01",
		"prop_yacht_seat_02",
		"prop_yacht_seat_03",
		"prop_yacht_table_01",
		"prop_yacht_table_02",
		"prop_yacht_table_03",
		"prop_yaught_chair_01",
		"prop_yaught_sofa_01",
		"prop_yell_plastic_target",
		"prop_yoga_mat_01",
		"prop_yoga_mat_02",
		"prop_yoga_mat_03",
		"prop_ztype_covered",
		"p_ing_skiprope_01",
		"p_ing_skiprope_01_s",
		"p_skiprope_r_s",
		"test_prop_gravestones_04a",
		"test_prop_gravestones_05a",
		"test_prop_gravestones_07a",
		"test_prop_gravestones_08a",
		"test_prop_gravestones_09a",
		"test_prop_gravetomb_01a",
		"test_prop_gravetomb_02a",
		"prop_cs_dog_lead_a_s",
		"prop_cs_dog_lead_b_s",
		"prop_welding_mask_01_s",
		"prop_wheelchair_01_s",
		"p_a4_sheets_s",
		"p_amanda_note_01_s",
		"p_armchair_01_s",
		"p_arm_bind_cut_s",
		"p_ashley_neck_01_s",
		"p_banknote_onedollar_s",
		"p_banknote_s",
		"p_binbag_01_s",
		"p_bison_winch_s",
		"p_bloodsplat_s",
		"p_blueprints_01_s",
		"p_brain_chunk_s",
		"p_bs_map_door_01_s",
		"p_cablecar_s",
		"p_cablecar_s_door_l",
		"p_cablecar_s_door_r",
		"p_cash_envelope_01_s",
		"p_cctv_s",
		"p_chem_vial_02b_s",
		"p_clb_officechair_s",
		"p_cletus_necklace_s",
		"p_clothtarp_down_s",
		"p_clothtarp_s",
		"p_clothtarp_up_s",
		"p_controller_01_s",
		"p_crahsed_heli_s",
		"p_cs1_14b_train_s",
		"p_cs1_14b_train_s_col",
		"p_cs1_14b_train_s_colopen",
		"p_csbporndudes_necklace_s",
		"p_csh_strap_01_pro_s",
		"p_csh_strap_01_s",
		"p_csh_strap_03_s",
		"p_cs_15m_rope_s",
		"p_cs_bandana_s",
		"p_cs_beachtowel_01_s",
		"p_cs_beverly_lanyard_s",
		"p_cs_bowl_01b_s",
		"p_cs_ciggy_01b_s",
		"p_cs_clothes_box_s",
		"p_cs_coke_line_s",
		"p_cs_cuffs_02_s",
		"p_cs_duffel_01_s",
		"p_cs_laz_ptail_s",
		"p_cs_leaf_s",
		"p_cs_mp_jet_01_s",
		"p_cs_newspaper_s",
		"p_cs_pamphlet_01_s",
		"p_cs_panties_03_s",
		"p_cs_para_ropebit_s",
		"p_cs_para_ropes_s",
		"p_cs_polaroid_s",
		"p_cs_police_torch_s",
		"p_cs_pour_tube_s",
		"p_cs_power_cord_s",
		"p_cs_rope_tie_01_s",
		"p_cs_sack_01_s",
		"p_cs_saucer_01_s",
		"p_cs_scissors_s",
		"p_cs_script_s",
		"p_cs_shirt_01_s",
		"p_cs_shot_glass_2_s",
		"p_cs_shot_glass_s",
		"p_cs_sub_hook_01_s",
		"p_cs_toaster_s",
		"p_cs_tracy_neck2_s",
		"p_cs_trolley_01_s",
		"p_defilied_ragdoll_01_s",
		"p_devin_box_01_s",
		"p_dinechair_01_s",
		"p_d_scuba_mask_s",
		"p_d_scuba_tank_s",
		"p_ecg_01_cable_01_s",
		"p_fib_rubble_s",
		"p_finale_bld_ground_s",
		"p_finale_bld_pool_s",
		"p_fin_vaultdoor_s",
		"p_flatbed_strap_s",
		"p_folding_chair_01_s",
		"p_gaffer_tape_s",
		"p_gaffer_tape_strip_s",
		"p_gasmask_s",
		"p_gcase_s",
		"p_hand_toilet_s",
		"p_hw1_22_doors_s",
		"p_hw1_22_table_s",
		"p_ice_box_01_s",
		"p_ice_box_proxy_col",
		"p_idol_case_s",
		"p_ilev_p_easychair_s",
		"p_inhaler_01_s",
		"p_jimmyneck_03_s",
		"p_jimmy_necklace_s",
		"p_kitch_juicer_s",
		"p_lamarneck_01_s",
		"p_laptop_02_s",
		"p_large_gold_s",
		"p_lazlow_shirt_s",
		"p_laz_j01_s",
		"p_laz_j02_s",
		"p_ld_cable_tie_01_s",
		"p_ld_crocclips01_s",
		"p_ld_crocclips02_s",
		"p_ld_heist_bag_s_1",
		"p_ld_heist_bag_s_2",
		"p_ld_heist_bag_s_pro",
		"p_ld_heist_bag_s_pro2_s",
		"p_ld_heist_bag_s_pro_o",
		"p_leg_bind_cut_s",
		"p_lestersbed_s",
		"p_lev_sofa_s",
		"p_lifeinv_neck_01_s",
		"p_loose_rag_01_s",
		"p_mbbed_s",
		"p_medal_01_s",
		"p_med_jet_01_s",
		"p_meth_bag_01_s",
		"p_michael_backpack_s",
		"p_michael_scuba_mask_s",
		"p_michael_scuba_tank_s",
		"p_mp_showerdoor_s",
		"p_mrk_harness_s",
		"p_mr_raspberry_01_s",
		"p_novel_01_s",
		"p_omega_neck_01_s",
		"p_omega_neck_02_s",
		"p_orleans_mask_s",
		"p_ortega_necklace_s",
		"p_oscar_necklace_s",
		"p_overalls_02_s",
		"p_pistol_holster_s",
		"p_po1_01_doorm_s",
		"p_police_radio_hset_s",
		"p_poly_bag_01_s",
		"p_pour_wine_s",
		"p_rail_controller_s",
		"p_res_sofa_l_s",
		"p_rpulley_s",
		"p_sec_case_02_s",
		"p_shower_towel_s",
		"p_single_rose_s",
		"p_smg_holster_01_s",
		"p_soloffchair_s",
		"p_spinning_anus_s",
		"p_steve_scuba_hood_s",
		"p_stretch_necklace_s",
		"p_syringe_01_s",
		"p_s_scuba_mask_s",
		"p_s_scuba_tank_s",
		"p_till_01_s",
		"p_tmom_earrings_s",
		"p_tourist_map_01_s",
		"p_trevor_prologe_bally_s",
		"p_trev_rope_01_s",
		"p_trev_ski_mask_s",
		"p_tumbler_01_s",
		"p_tumbler_02_s1",
		"p_tumbler_cs2_s",
		"p_tv_cam_02_s",
		"p_t_shirt_pile_s",
		"p_v_43_safe_s",
		"p_v_ilev_chopshopswitch_s",
		"p_v_med_p_sofa_s",
		"p_v_res_tt_bed_s",
		"p_wade_necklace_s",
		"p_wboard_clth_s",
		"p_weed_bottle_s",
		"p_whiskey_bottle_s",
		"p_winch_long_s",
		"s_prop_hdphones",
		"s_prop_hdphones_1",
		"v_ilev_fib_door1_s",
		"v_res_msonbed_s",
		"v_res_tre_sofa_s",
		"v_tre_sofa_mess_a_s",
		"v_tre_sofa_mess_b_s",
		"v_tre_sofa_mess_c_s",
		"prop_ar_arrow_1",
		"prop_ar_arrow_2",
		"prop_ar_arrow_3",
		"prop_ar_ring_01",
		"prop_mk_arrow_3d",
		"prop_mk_arrow_flat",
		"prop_mk_bike_logo_1",
		"prop_mk_bike_logo_2",
		"prop_mk_boost",
		"prop_mk_cone",
		"prop_mk_cylinder",
		"prop_mk_flag",
		"prop_mk_flag_2",
		"prop_mk_heli",
		"prop_mk_lap",
		"prop_mk_mp_ring_01",
		"prop_mk_mp_ring_01b",
		"prop_mk_num_0",
		"prop_mk_num_1",
		"prop_mk_num_2",
		"prop_mk_num_3",
		"prop_mk_num_4",
		"prop_mk_num_5",
		"prop_mk_num_6",
		"prop_mk_num_7",
		"prop_mk_num_8",
		"prop_mk_num_9",
		"prop_mk_plane",
		"prop_mk_race_chevron_01",
		"prop_mk_race_chevron_02",
		"prop_mk_race_chevron_03",
		"prop_mk_repair",
		"prop_mk_ring",
		"prop_mk_ring_flat",
		"prop_mk_sphere",
		"prop_mk_tri_cycle",
		"prop_mk_tri_run",
		"prop_mk_tri_swim",
		"prop_mp_arrow_ring",
		"prop_mp_halo",
		"prop_mp_halo_lrg",
		"prop_mp_halo_med",
		"prop_mp_halo_point",
		"prop_mp_halo_point_lrg",
		"prop_mp_halo_point_med",
		"prop_mp_halo_point_sm",
		"prop_mp_halo_rotate",
		"prop_mp_halo_rotate_lrg",
		"prop_mp_halo_rotate_med",
		"prop_mp_halo_rotate_sm",
		"prop_mp_halo_sm",
		"prop_mp_pointer_ring",
		"prop_mp_solid_ring",
		"proc_litter_01",
		"proc_litter_02",
		"proc_mntn_stone01",
		"proc_mntn_stone02",
		"proc_mntn_stone03",
		"proc_searock_01",
		"proc_searock_02",
		"proc_searock_03",
		"proc_sml_stones01",
		"proc_sml_stones02",
		"proc_sml_stones03",
		"prop_coral_01",
		"prop_coral_02",
		"prop_coral_03",
		"prop_seabrain_01",
		"prop_seagroup_02",
		"prop_sealife_01",
		"prop_sealife_02",
		"prop_sealife_03",
		"prop_sealife_04",
		"prop_sealife_05",
		"prop_seaweed_01",
		"prop_seaweed_02",
		"prop_starfish_01",
		"prop_starfish_02",
		"prop_starfish_03",
		"prop_aerial_01a",
		"prop_aerial_01b",
		"prop_aerial_01c",
		"prop_aerial_01d",
		"prop_aircon_l_01",
		"prop_aircon_l_02",
		"prop_aircon_l_03",
		"prop_aircon_l_04",
		"prop_aircon_m_09",
		"prop_aircon_s_01a",
		"prop_aircon_s_02a",
		"prop_aircon_s_02b",
		"prop_aircon_s_03a",
		"prop_aircon_s_03b",
		"prop_aircon_s_04a",
		"prop_aircon_s_05a",
		"prop_aircon_s_06a",
		"prop_aircon_s_07a",
		"prop_aircon_s_07b",
		"prop_alarm_01",
		"prop_alarm_02",
		"prop_billb_frame01a",
		"prop_billb_frame01b",
		"prop_billb_frame03a",
		"prop_billb_frame03b",
		"prop_billb_frame03c",
		"prop_billb_frame04a",
		"prop_billb_frame04b",
		"prop_bmu_01",
		"prop_bmu_01_b",
		"prop_bmu_02",
		"prop_bmu_02_ld",
		"prop_bmu_02_ld_cab",
		"prop_bmu_02_ld_sup",
		"prop_bmu_track01",
		"prop_bmu_track02",
		"prop_bmu_track03",
		"prop_fireescape_01a",
		"prop_fireescape_01b",
		"prop_fireescape_02a",
		"prop_fireescape_02b",
		"prop_flagpole_1a",
		"prop_flagpole_2a",
		"prop_flagpole_3a",
		"prop_helipad_01",
		"prop_helipad_02",
		"prop_radiomast01",
		"prop_radiomast02",
		"prop_roofpipe_01",
		"prop_roofpipe_02",
		"prop_roofpipe_03",
		"prop_roofpipe_04",
		"prop_roofpipe_05",
		"prop_roofpipe_06",
		"prop_roofvent_011a",
		"prop_roofvent_01a",
		"prop_roofvent_01b",
		"prop_roofvent_02a",
		"prop_roofvent_02b",
		"prop_roofvent_03a",
		"prop_roofvent_04a",
		"prop_roofvent_05a",
		"prop_roofvent_05b",
		"prop_roofvent_07a",
		"prop_roofvent_08a",
		"prop_roofvent_09a",
		"prop_roofvent_10a",
		"prop_roofvent_10b",
		"prop_roofvent_11b",
		"prop_roofvent_11c",
		"prop_roofvent_12a",
		"prop_roofvent_13a",
		"prop_roofvent_15a",
		"prop_roofvent_16a",
		"prop_satdish_2_a",
		"prop_satdish_2_f",
		"prop_satdish_2_g",
		"prop_satdish_3_b",
		"prop_satdish_3_c",
		"prop_satdish_3_d",
		"prop_satdish_l_01",
		"prop_satdish_s_03",
		"prop_satdish_s_05a",
		"prop_satdish_s_05b",
		"prop_skylight_01",
		"prop_skylight_02",
		"prop_skylight_03",
		"prop_skylight_04",
		"prop_skylight_05",
		"prop_skylight_06b",
		"prop_skylight_06c",
		"prop_solarpanel_01",
		"prop_solarpanel_02",
		"prop_solarpanel_03",
		"prop_ventsystem_01",
		"prop_ventsystem_02",
		"prop_ventsystem_03",
		"prop_ventsystem_04",
		"prop_wall_vent_01",
		"prop_wall_vent_02",
		"prop_wall_vent_03",
		"prop_wall_vent_04",
		"prop_wall_vent_05",
		"prop_wall_vent_06",
		"prop_watertower01",
		"prop_watertower02",
		"prop_watertower03",
		"prop_watertower04",
		"prop_bahammenu",
		"prop_barrachneon",
		"prop_bar_coastbarr",
		"prop_bar_coastchamp",
		"prop_bar_coastdusc",
		"prop_bar_coastmount",
		"prop_bar_cooler_01",
		"prop_bar_cooler_03",
		"prop_bar_fridge_01",
		"prop_bar_fridge_02",
		"prop_bar_fridge_03",
		"prop_bar_fridge_04",
		"prop_bar_ice_01",
		"prop_bar_napkindisp",
		"prop_bar_pump_01",
		"prop_bar_pump_04",
		"prop_bar_pump_05",
		"prop_bar_pump_06",
		"prop_bar_pump_07",
		"prop_bar_pump_08",
		"prop_bar_pump_09",
		"prop_bar_pump_10",
		"prop_bar_sink_01",
		"prop_bar_stool_01",
		"prop_beerneon",
		"prop_beer_bison",
		"prop_cherenneon",
		"prop_cockneon",
		"prop_cs_kitchen_cab_l2",
		"prop_cs_kitchen_cab_ld",
		"prop_cs_kitchen_cab_rd",
		"prop_irish_sign_01",
		"prop_irish_sign_02",
		"prop_irish_sign_03",
		"prop_irish_sign_04",
		"prop_irish_sign_05",
		"prop_irish_sign_06",
		"prop_irish_sign_07",
		"prop_irish_sign_08",
		"prop_irish_sign_09",
		"prop_irish_sign_10",
		"prop_irish_sign_11",
		"prop_irish_sign_12",
		"prop_irish_sign_13",
		"prop_loggneon",
		"prop_patriotneon",
		"prop_pitcher_01_cs",
		"prop_ragganeon",
		"prop_shots_glass_cs",
		"prop_stripmenu",
		"prop_bread_rack_01",
		"prop_bread_rack_02",
		"prop_chip_fryer",
		"prop_coffee_mac_02",
		"prop_detergent_01a",
		"prop_detergent_01b",
		"prop_ff_counter_01",
		"prop_ff_counter_02",
		"prop_ff_counter_03",
		"prop_ff_noodle_01",
		"prop_ff_noodle_02",
		"prop_ff_shelves_01",
		"prop_ff_sink_01",
		"prop_ff_sink_02",
		"prop_food_bin_01",
		"prop_food_bin_02",
		"prop_food_bs_bshelf",
		"prop_food_bs_cups01",
		"prop_food_bs_cups03",
		"prop_food_bs_soda_01",
		"prop_food_bs_soda_02",
		"prop_food_bs_tray_01",
		"prop_food_bs_tray_06",
		"prop_food_burg1",
		"prop_food_burg2",
		"prop_food_cb_bshelf",
		"prop_food_cb_burg01",
		"prop_food_cb_cups01",
		"prop_food_cb_donuts",
		"prop_food_cb_nugets",
		"prop_food_cb_soda_01",
		"prop_food_cb_soda_02",
		"prop_food_cb_tray_01",
		"prop_food_cups1",
		"prop_food_napkin_01",
		"prop_food_napkin_02",
		"prop_food_tray_01",
		"prop_griddle_01",
		"prop_griddle_02",
		"prop_handtowels",
		"prop_juice_dispenser",
		"prop_kebab_grill",
		"prop_microwave_1",
		"prop_pizza_oven_01",
		"prop_slush_dispenser",
		"prop_bumper_01",
		"prop_bumper_02",
		"prop_bumper_03",
		"prop_bumper_04",
		"prop_bumper_05",
		"prop_bumper_06",
		"prop_carcreeper",
		"prop_carjack",
		"prop_carjack_l2",
		"prop_car_battery_01",
		"prop_car_bonnet_01",
		"prop_car_bonnet_02",
		"prop_car_door_01",
		"prop_car_door_02",
		"prop_car_door_03",
		"prop_car_door_04",
		"prop_car_engine_01",
		"prop_car_exhaust_01",
		"prop_car_seat",
		"prop_cheetah_covered",
		"prop_compressor_01",
		"prop_compressor_02",
		"prop_compressor_03",
		"prop_entityxf_covered",
		"prop_jb700_covered",
		"prop_spray_jackframe",
		"prop_spray_jackleg",
		"prop_toolchest_01",
		"prop_toolchest_02",
		"prop_toolchest_03",
		"prop_toolchest_04",
		"prop_toolchest_05",
		"prop_wheel_01",
		"prop_wheel_02",
		"prop_wheel_03",
		"prop_wheel_04",
		"prop_wheel_05",
		"prop_wheel_06",
		"prop_wheel_hub_01",
		"prop_wheel_hub_02_lod_02",
		"prop_wheel_rim_01",
		"prop_wheel_rim_02",
		"prop_wheel_rim_03",
		"prop_wheel_rim_04",
		"prop_wheel_rim_05",
		"prop_wheel_tyre",
		"prop_ztype_covered",
		"prop_cabinet_01",
		"prop_cabinet_01b",
		"prop_cabinet_02b",
		"prop_coathook_01",
		"prop_copier_01",
		"prop_fan_01",
		"prop_fax_01",
		"prop_folder_01",
		"prop_folder_02",
		"prop_office_alarm_01",
		"prop_office_desk_01",
		"prop_off_chair_01",
		"prop_off_chair_03",
		"prop_off_chair_04",
		"prop_off_chair_04b",
		"prop_off_chair_05",
		"prop_off_phone_01",
		"prop_paper_box_01",
		"prop_paper_box_02",
		"prop_paper_box_03",
		"prop_paper_box_04",
		"prop_paper_box_05",
		"prop_printer_01",
		"prop_printer_02",
		"prop_shredder_01",
		"prop_sol_chair",
		"prop_tablesmall_01",
		"prop_waiting_seat_01",
		"prop_wait_bench_01",
		"prop_water_bottle",
		"prop_water_bottle_dark",
		"prop_air_bagloader",
		"prop_air_bagloader2",
		"prop_air_barrier",
		"prop_air_bench_01",
		"prop_air_bench_02",
		"prop_air_bigradar_l1",
		"prop_air_bigradar_l2",
		"prop_air_bigradar_slod",
		"prop_air_blastfence_01",
		"prop_air_blastfence_02",
		"prop_air_bridge01",
		"prop_air_bridge02",
		"prop_air_cargoloader_01",
		"prop_air_cargo_01a",
		"prop_air_cargo_01b",
		"prop_air_cargo_01c",
		"prop_air_cargo_02a",
		"prop_air_cargo_02b",
		"prop_air_cargo_03a",
		"prop_air_cargo_04a",
		"prop_air_cargo_04b",
		"prop_air_cargo_04c",
		"prop_air_chock_01",
		"prop_air_chock_03",
		"prop_air_chock_04",
		"prop_air_fueltrail1",
		"prop_air_fueltrail2",
		"prop_air_gasbogey_01",
		"prop_air_generator_01",
		"prop_air_generator_03",
		"prop_air_lights_01a",
		"prop_air_lights_01b",
		"prop_air_lights_03a",
		"prop_air_luggtrolley",
		"prop_air_mast_01",
		"prop_air_mast_02",
		"prop_air_monhut_01",
		"prop_air_monhut_02",
		"prop_air_monhut_03",
		"prop_air_propeller01",
		"prop_air_radar_01",
		"prop_air_stair_01",
		"prop_air_stair_02",
		"prop_air_stair_03",
		"prop_air_stair_04a",
		"prop_air_stair_04b",
		"prop_air_towbar_01",
		"prop_air_towbar_02",
		"prop_air_towbar_03",
		"prop_air_trailer_4a",
		"prop_air_trailer_4b",
		"prop_air_trailer_4c",
		"prop_air_watertank1",
		"prop_air_watertank2",
		"prop_air_windsock_base",
		"prop_air_woodsteps",
		"prop_luggage_01a",
		"prop_luggage_02a",
		"prop_luggage_03a",
		"prop_luggage_04a",
		"prop_luggage_05a",
		"prop_luggage_06a",
		"prop_luggage_07a",
		"prop_luggage_08a",
		"prop_luggage_09a",
		"prop_mb_cargo_01a",
		"prop_mb_cargo_02a",
		"prop_mb_cargo_03a",
		"prop_mb_cargo_04a",
		"prop_mb_cargo_04b",
		"prop_mb_crate_01a",
		"prop_mb_crate_01b",
		"prop_mb_hesco_06",
		"prop_mb_ordnance_01",
		"prop_mb_ordnance_03",
		"prop_mb_sandblock_01",
		"prop_mb_sandblock_02",
		"prop_mb_sandblock_03",
		"prop_mb_sandblock_04",
		"prop_mb_sandblock_05",
		"prop_boxcar5_handle",
		"prop_byard_bench01",
		"prop_byard_bench02",
		"prop_byard_block_01",
		"prop_byard_boat01",
		"prop_byard_boat02",
		"prop_byard_chains01",
		"prop_byard_dingy",
		"prop_byard_elecbox01",
		"prop_byard_elecbox02",
		"prop_byard_elecbox03",
		"prop_byard_elecbox04",
		"prop_byard_floatpile",
		"prop_byard_float_01",
		"prop_byard_float_01b",
		"prop_byard_float_02",
		"prop_byard_float_02b",
		"prop_byard_hoist",
		"prop_byard_hoist_2",
		"prop_byard_hoses01",
		"prop_byard_hoses02",
		"prop_byard_ladder01",
		"prop_byard_machine01",
		"prop_byard_machine02",
		"prop_byard_machine03",
		"prop_byard_motor_01",
		"prop_byard_motor_02",
		"prop_byard_motor_03",
		"prop_byard_net02",
		"prop_byard_phone",
		"prop_byard_pipes01",
		"prop_byard_pipe_01",
		"prop_byard_planks01",
		"prop_byard_pulley01",
		"prop_byard_rack",
		"prop_byard_ramp",
		"prop_byard_rampold",
		"prop_byard_rowboat1",
		"prop_byard_rowboat2",
		"prop_byard_rowboat3",
		"prop_byard_rowboat4",
		"prop_byard_rowboat5",
		"prop_byard_scfhold01",
		"prop_byard_sleeper01",
		"prop_byard_sleeper02",
		"prop_byard_steps_01",
		"prop_byard_tank_01",
		"prop_byard_trailer01",
		"prop_byard_trailer02",
		"prop_crane_01_truck1",
		"prop_crane_01_truck2",
		"prop_dock_bouy_1",
		"prop_dock_bouy_2",
		"prop_dock_bouy_3",
		"prop_dock_bouy_5",
		"prop_dock_crane_01",
		"prop_dock_crane_02",
		"prop_dock_crane_02_cab",
		"prop_dock_crane_02_hook",
		"prop_dock_crane_02_ld",
		"prop_dock_crane_04",
		"prop_dock_crane_lift",
		"prop_dock_float_1",
		"prop_dock_float_1b",
		"prop_dock_moor_01",
		"prop_dock_moor_04",
		"prop_dock_moor_05",
		"prop_dock_moor_06",
		"prop_dock_moor_07",
		"prop_dock_ropefloat",
		"prop_dock_ropetyre1",
		"prop_dock_ropetyre2",
		"prop_dock_ropetyre3",
		"prop_dock_rtg_01",
		"prop_dock_rtg_ld",
		"prop_dock_shippad",
		"prop_dock_sign_01",
		"prop_dock_woodpole1",
		"prop_dock_woodpole2",
		"prop_dock_woodpole3",
		"prop_dock_woodpole4",
		"prop_dock_woodpole5",
		"prop_ind_barge_01",
		"prop_ind_barge_02",
		"prop_ind_coalcar_01",
		"prop_ind_coalcar_02",
		"prop_ind_coalcar_03",
		"prop_ind_oldcrane",
		"prop_jetski_ramp_01",
		"prop_railsleepers01",
		"prop_railsleepers02",
		"prop_railstack01",
		"prop_railstack02",
		"prop_railstack03",
		"prop_railstack04",
		"prop_railstack05",
		"prop_rail_boxcar",
		"prop_rail_boxcar2",
		"prop_rail_boxcar3",
		"prop_rail_boxcar4",
		"prop_rail_boxcar5",
		"prop_rail_boxcar5_d",
		"prop_rail_buffer_01",
		"prop_rail_buffer_02",
		"prop_rail_crane_01",
		"prop_rail_points01",
		"prop_rail_points02",
		"prop_rail_sigbox01",
		"prop_rail_sigbox02",
		"prop_rail_signals02",
		"prop_rail_tankcar",
		"prop_rail_tankcar2",
		"prop_rail_tankcar3",
		"prop_rail_wellcar",
		"prop_rail_wellcar2",
		"p_dock_rtg_ld_cab",
		"p_dock_rtg_ld_spdr",
		"p_dock_rtg_ld_wheel",
		"prop_fac_machine_02",
		"prop_ind_conveyor_01",
		"prop_ind_conveyor_02",
		"prop_ind_conveyor_04",
		"prop_ind_crusher",
		"prop_ind_deiseltank",
		"prop_ind_mech_01c",
		"prop_ind_mech_02a",
		"prop_ind_mech_02b",
		"prop_ind_mech_03a",
		"prop_ind_mech_04a",
		"prop_jyard_block_01a",
		"prop_oil_derrick_01",
		"prop_oil_guage_01",
		"prop_oil_spool_02",
		"prop_oil_valve_01",
		"prop_oil_valve_02",
		"prop_oil_wellhead_01",
		"prop_oil_wellhead_03",
		"prop_oil_wellhead_04",
		"prop_oil_wellhead_05",
		"prop_oil_wellhead_06",
		"prop_portacabin01",
		"prop_portasteps_01",
		"prop_portasteps_02",
		"prop_pylon_01",
		"prop_pylon_02",
		"prop_pylon_03",
		"prop_pylon_04",
		"prop_sluicegate",
		"prop_sluicegatel",
		"prop_sluicegater",
		"prop_storagetank_01",
		"prop_storagetank_02",
		"prop_storagetank_03",
		"prop_storagetank_03a",
		"prop_storagetank_03b",
		"prop_storagetank_04",
		"prop_storagetank_05",
		"prop_storagetank_06",
		"prop_storagetank_07a",
		"prop_sub_crane_hook",
		"prop_sub_frame_01a",
		"prop_sub_frame_01b",
		"prop_sub_frame_01c",
		"prop_sub_frame_02a",
		"prop_sub_frame_03a",
		"prop_sub_frame_04a",
		"prop_sub_frame_04b",
		"prop_sub_gantry",
		"prop_sub_trans_01a",
		"prop_sub_trans_02a",
		"prop_sub_trans_03a",
		"prop_sub_trans_04a",
		"prop_sub_trans_05b",
		"prop_sub_trans_06b",
		"prop_windmill_01_l1",
		"prop_windmill_01_slod",
		"prop_windmill_01_slod2",
		"prop_acc_guitar_01",
		"prop_acc_guitar_01_d1",
		"prop_alien_egg_01",
		"prop_amb_handbag_01",
		"prop_amb_phone",
		"prop_asteroid_01",
		"prop_attache_case_01",
		"prop_bank_shutter",
		"prop_bank_vaultdoor",
		"prop_barrel_float_1",
		"prop_barrel_float_2",
		"prop_beggers_sign_01",
		"prop_beggers_sign_02",
		"prop_beggers_sign_03",
		"prop_beggers_sign_04",
		"prop_bh1_16_display",
		"prop_big_bag_01",
		"prop_big_clock_01",
		"prop_biotech_store",
		"prop_bomb_01",
		"prop_bonesaw",
		"prop_bskball_01",
		"prop_b_board_blank",
		"prop_cable_hook_01",
		"prop_camera_strap",
		"prop_cash_dep_bag_01",
		"prop_cash_trolly",
		"prop_chem_grill",
		"prop_chem_grill_bit",
		"prop_chem_vial_02",
		"prop_chem_vial_02b",
		"prop_choc_ego",
		"prop_choc_meto",
		"prop_choc_pq",
		"prop_clapper_brd_01",
		"prop_cone_float_1",
		"prop_container_hole",
		"prop_cont_chiller_01",
		"prop_cork_board",
		"prop_cranial_saw",
		"prop_crate_float_1",
		"prop_cs_20m_rope",
		"prop_cs_30m_rope",
		"prop_cs_aircon_01",
		"prop_cs_aircon_fan",
		"prop_cs_ashtray",
		"prop_cs_bin_01",
		"prop_cs_bin_01_lid",
		"prop_cs_bin_02",
		"prop_cs_bin_03",
		"prop_cs_box_clothes",
		"prop_cs_cardbox_01",
		"prop_cs_dumpster_01a",
		"prop_cs_dumpster_lidl",
		"prop_cs_dumpster_lidr",
		"prop_cs_fertilizer",
		"prop_cs_folding_chair_01",
		"prop_cs_fridge",
		"prop_cs_fridge_door",
		"prop_cs_heist_bag_01",
		"prop_cs_heist_bag_02",
		"prop_cs_heist_bag_strap_01",
		"prop_cs_h_bag_strap_01",
		"prop_cs_ice_locker",
		"prop_cs_ice_locker_door_l",
		"prop_cs_ice_locker_door_r",
		"prop_cs_keys_01",
		"prop_cs_leaf",
		"prop_cs_office_chair",
		"prop_cs_package_01",
		"prop_cs_panel_01",
		"prop_cs_panties",
		"prop_cs_panties_02",
		"prop_cs_panties_03",
		"prop_cs_planning_photo",
		"prop_cs_polaroid",
		"prop_cs_remote_01",
		"prop_cs_rope_tie_01",
		"prop_cs_rub_binbag_01",
		"prop_cs_scissors",
		"prop_cs_shopping_bag",
		"prop_cs_spray_can",
		"prop_cs_tablet",
		"prop_cs_tablet_02",
		"prop_cs_vial_01",
		"prop_cs_walking_stick",
		"prop_cub_door_lifeblurb",
		"prop_cub_lifeblurb",
		"prop_cuff_keys_01",
		"prop_defilied_ragdoll_01",
		"prop_drop_armscrate_01",
		"prop_drop_armscrate_01b",
		"prop_dummy_01",
		"prop_dummy_light",
		"prop_dummy_plane",
		"prop_egg_clock_01",
		"prop_ejector_seat_01",
		"prop_el_guitar_01",
		"prop_el_guitar_02",
		"prop_el_guitar_03",
		"prop_fbibombbin",
		"prop_fbibombcupbrd",
		"prop_fbibombfile",
		"prop_fbibombplant",
		"prop_feed_sack_01",
		"prop_feed_sack_02",
		"prop_fib_broken_window",
		"prop_flash_unit",
		"prop_flatbed_strap_b",
		"prop_floor_duster_01",
		"prop_folded_polo_shirt",
		"prop_foundation_sponge",
		"prop_fruit_basket",
		"prop_f_duster_02",
		"prop_gascyl_ramp_01",
		"prop_gascyl_ramp_door_01",
		"prop_gas_grenade",
		"prop_gold_bar",
		"prop_gold_trolly",
		"prop_gold_trolly_full",
		"prop_grapes_01",
		"prop_grapes_02",
		"prop_hacky_sack_01",
		"prop_hd_seats_01",
		"prop_hole_plug_01",
		"prop_hotel_clock_01",
		"prop_hotel_trolley",
		"prop_huge_display_01",
		"prop_huge_display_02",
		"prop_idol_01_error",
		"prop_idol_case_01",
		"prop_ing_camera_01",
		"prop_int_gate01",
		"prop_jewel_02a",
		"prop_jewel_02b",
		"prop_jewel_02c",
		"prop_jewel_03a",
		"prop_jewel_03b",
		"prop_jewel_04a",
		"prop_jewel_04b",
		"prop_jewel_pickup_new_01",
		"prop_j_disptray_01",
		"prop_j_disptray_01b",
		"prop_j_disptray_01_dam",
		"prop_j_disptray_02",
		"prop_j_disptray_02_dam",
		"prop_j_disptray_03",
		"prop_j_disptray_03_dam",
		"prop_j_disptray_04",
		"prop_j_disptray_04b",
		"prop_j_disptray_05",
		"prop_j_disptray_05b",
		"prop_j_heist_pic_01",
		"prop_j_heist_pic_02",
		"prop_j_heist_pic_03",
		"prop_j_heist_pic_04",
		"prop_j_neck_disp_01",
		"prop_j_neck_disp_02",
		"prop_j_neck_disp_03",
		"prop_large_gold",
		"prop_large_gold_alt_a",
		"prop_large_gold_alt_b",
		"prop_large_gold_alt_c",
		"prop_large_gold_empty",
		"prop_ld_alarm_01",
		"prop_ld_alarm_01_dam",
		"prop_ld_alarm_alert",
		"prop_ld_armour",
		"prop_ld_binbag_01",
		"prop_ld_bomb",
		"prop_ld_cable",
		"prop_ld_cable_tie_01",
		"prop_ld_case_01",
		"prop_ld_case_01_lod",
		"prop_ld_container",
		"prop_ld_contain_dl",
		"prop_ld_contain_dl2",
		"prop_ld_contain_dr",
		"prop_ld_contain_dr2",
		"prop_ld_crate_01",
		"prop_ld_crate_lid_01",
		"prop_ld_fan_01",
		"prop_ld_fan_01_old",
		"prop_ld_greenscreen_01",
		"prop_ld_handbag",
		"prop_ld_haybail",
		"prop_ld_health_pack",
		"prop_ld_hook",
		"prop_ld_int_safe_01",
		"prop_ld_jerrycan_01",
		"prop_ld_keypad_01",
		"prop_ld_keypad_01b",
		"prop_ld_keypad_01b_lod",
		"prop_ld_lap_top",
		"prop_ld_monitor_01",
		"prop_ld_pipe_single_01",
		"prop_ld_planning_pin_01",
		"prop_ld_planning_pin_02",
		"prop_ld_planning_pin_03",
		"prop_ld_purse_01",
		"prop_ld_purse_01_lod",
		"prop_ld_rope_t",
		"prop_ld_rub_binbag_01",
		"prop_ld_suitcase_01",
		"prop_ld_suitcase_02",
		"prop_ld_toilet_01",
		"prop_ld_vault_door",
		"prop_ld_wallet_01",
		"prop_ld_w_me_machette",
		"prop_lectern_01",
		"prop_lev_crate_01",
		"prop_lifeblurb_01",
		"prop_lifeblurb_01b",
		"prop_lifeblurb_02",
		"prop_lifeblurb_02b",
		"prop_mast_01",
		"prop_medal_01",
		"prop_med_bag_01",
		"prop_med_bag_01b",
		"prop_michael_backpack",
		"prop_mr_rasberryclean",
		"prop_mr_raspberry_01",
		"prop_muster_wboard_01",
		"prop_muster_wboard_02",
		"prop_necklace_board",
		"prop_new_drug_pack_01",
		"prop_night_safe_01",
		"prop_novel_01",
		"prop_npc_phone",
		"prop_npc_phone_02",
		"prop_out_door_speaker",
		"prop_overalls_01",
		"prop_paper_bag_01",
		"prop_paper_bag_small",
		"prop_paper_ball",
		"prop_pap_camera_01",
		"prop_parachute",
		"prop_parapack_01",
		"prop_paynspray_door_l",
		"prop_paynspray_door_r",
		"prop_ped_pic_01",
		"prop_ped_pic_01_sm",
		"prop_ped_pic_02",
		"prop_ped_pic_02_sm",
		"prop_ped_pic_03",
		"prop_ped_pic_03_sm",
		"prop_ped_pic_04",
		"prop_ped_pic_04_sm",
		"prop_ped_pic_05",
		"prop_ped_pic_05_sm",
		"prop_ped_pic_06",
		"prop_ped_pic_06_sm",
		"prop_ped_pic_07",
		"prop_ped_pic_07_sm",
		"prop_ped_pic_08",
		"prop_ped_pic_08_sm",
		"prop_phone_ing",
		"prop_phone_ing_02",
		"prop_phone_ing_03",
		"prop_phone_overlay_01",
		"prop_phone_overlay_02",
		"prop_phone_overlay_anim",
		"prop_phone_proto",
		"prop_phone_proto_back",
		"prop_phone_proto_battery",
		"prop_player_phone_01",
		"prop_player_phone_02",
		"prop_podium_mic",
		"prop_police_phone",
		"prop_poly_bag_01",
		"prop_poly_bag_money",
		"prop_pool_ball_01",
		"prop_postit_drive",
		"prop_postit_gun",
		"prop_postit_it",
		"prop_postit_lock",
		"prop_prologue_phone",
		"prop_protest_sign_01",
		"prop_protest_table_01",
		"prop_prototype_minibomb",
		"prop_rag_01",
		"prop_rf_conc_pillar",
		"prop_riding_crop_01",
		"prop_rock_chair_01",
		"prop_rolled_yoga_mat",
		"prop_rope_hook_01",
		"prop_scalpel",
		"prop_scrap_win_01",
		"prop_security_case_01",
		"prop_security_case_02",
		"prop_shamal_crash",
		"prop_shopping_bags01",
		"prop_shopping_bags02",
		"prop_showroom_glass_1b",
		"prop_side_lights",
		"prop_single_rose",
		"prop_sky_cover_01",
		"prop_sm_19_clock",
		"prop_sm_locker_door",
		"prop_spot_01",
		"prop_spot_clamp",
		"prop_spot_clamp_02",
		"prop_stat_pack_01",
		"prop_strip_pole_01",
		"prop_sub_chunk_01",
		"prop_tail_gate_col",
		"prop_taxi_meter_1",
		"prop_taxi_meter_2",
		"prop_telescope_01",
		"prop_tennis_bag_01",
		"prop_test_elevator",
		"prop_test_elevator_dl",
		"prop_test_elevator_dr",
		"prop_tick",
		"prop_tick_02",
		"prop_toiletfoot_static",
		"prop_torture_ch_01",
		"prop_tri_table_01",
		"prop_tyre_spike_01",
		"prop_t_coffe_table",
		"prop_t_sofa",
		"prop_t_sofa_02",
		"prop_t_telescope_01b",
		"prop_vb_34_tencrt_lighting",
		"prop_vehicle_hook",
		"prop_v_bmike_01",
		"prop_v_cam_01",
		"prop_v_hook_s",
		"prop_v_m_phone_01",
		"prop_v_parachute",
		"prop_water_ramp_01",
		"prop_water_ramp_02",
		"prop_water_ramp_03",
		"prop_wheelchair_01",
		"prop_windowbox_a",
		"prop_windowbox_b",
		"prop_windowbox_broken",
		"prop_windowbox_small",
		"prop_win_trailer_ld",
		"prop_w_board_blank",
		"prop_w_board_blank_2",
		"prop_yoga_mat_01",
		"prop_yoga_mat_02",
		"prop_yoga_mat_03",
		"p_amb_bag_bottle_01",
		"p_amb_brolly_01",
		"p_amb_brolly_01_s",
		"p_amb_clipboard_01",
		"p_amb_coffeecup_01",
		"p_amb_joint_01",
		"p_amb_lap_top_01",
		"p_amb_lap_top_02",
		"p_amb_phone_01",
		"p_car_keys_01",
		"p_counter_01_glass",
		"p_counter_01_glass_plug",
		"p_counter_02_glass",
		"p_counter_03_glass",
		"p_counter_04_glass",
		"p_cs_cam_phone",
		"p_cs_comb_01",
		"p_cs_laptop_02",
		"p_cs_laptop_02_w",
		"p_cs_locker_01",
		"p_cs_locker_02",
		"p_cs_locker_door_01",
		"p_cs_locker_door_01b",
		"p_cs_locker_door_02",
		"p_cs_paper_disp_02",
		"p_cs_paper_disp_1",
		"p_disp_02_door_01",
		"p_ferris_car_01",
		"p_f_duster_handle_01",
		"p_f_duster_head_01",
		"p_ing_skiprope_01",
		"p_int_jewel_mirror",
		"p_int_jewel_plant_01",
		"p_int_jewel_plant_02",
		"p_jewel_necklace_02",
		"p_ld_am_ball_01",
		"p_ld_coffee_vend_01",
		"p_ld_conc_cyl_01",
		"p_ld_frisbee_01",
		"p_ld_heist_bag_01",
		"p_ld_sax",
		"p_ld_soc_ball_01",
		"p_new_j_counter_01",
		"p_new_j_counter_02",
		"p_new_j_counter_03",
		"p_num_plate_01",
		"p_num_plate_02",
		"p_num_plate_03",
		"p_num_plate_04",
		"p_oil_slick_01",
		"p_pharm_unit_01",
		"p_pharm_unit_02",
		"p_planning_board_01",
		"p_planning_board_02",
		"p_planning_board_03",
		"p_planning_board_04",
		"p_secret_weapon_02",
		"p_stinger_02",
		"p_stinger_03",
		"p_stinger_04",
		"p_stinger_piece_01",
		"p_stinger_piece_02",
		"p_watch_01",
		"p_watch_02",
		"p_watch_03",
		"p_watch_04",
		"p_watch_05",
		"p_watch_06",
		"prop_bomb_01_s",
		"prop_cs_bin_01_skinned",
		"prop_cs_bucket_s",
		"prop_cs_bucket_s_lod",
		"prop_cs_mop_s",
		"prop_f_duster_01_s",
		"prop_ld_case_01_s",
		"prop_ld_handbag_s",
		"prop_ld_wallet_01_s",
		"prop_news_disp_02a_s",
		"prop_off_chair_04_s",
		"prop_p_jack_03_col",
		"prop_rub_binbag_sd_01",
		"prop_rub_binbag_sd_02",
		"prop_v_m_phone_o1s",
		"p_abat_roller_1",
		"p_abat_roller_1_col",
		"p_amb_drain_water_double",
		"p_amb_drain_water_longstrip",
		"p_amb_drain_water_single",
		"p_attache_case_01_s",
		"p_balaclavamichael_s",
		"p_barierbase_test_s",
		"p_barier_test_s",
		"p_beefsplitter_s",
		"p_cargo_chute_s",
		"p_cigar_pack_02_s",
		"p_cs_locker_01_s",
		"p_cs_script_bottle_s",
		"p_dock_crane_cable_s",
		"p_dock_crane_cabl_s",
		"p_dock_crane_sld_s",
		"p_dumpster_t",
		"p_fag_packet_01_s",
		"p_ferris_wheel_amo_l",
		"p_ferris_wheel_amo_l2",
		"p_ferris_wheel_amo_p",
		"p_gar_door_01_s",
		"p_gar_door_02_s",
		"p_gar_door_03_s",
		"p_gdoor1colobject_s",
		"p_gdoor1_s",
		"p_ing_skiprope_01_s",
		"p_jewel_necklace01_s",
		"p_jewel_necklace02_s",
		"p_jewel_pickup33_s",
		"p_ld_coffee_vend_s",
		"p_ld_heist_bag_s",
		"p_ld_stinger_s",
		"p_litter_picker_s",
		"p_mast_01_s",
		"p_notepad_01_s",
		"p_oil_pjack_01_amo",
		"p_oil_pjack_01_frg_s",
		"p_oil_pjack_02_amo",
		"p_oil_pjack_02_frg_s",
		"p_oil_pjack_03_amo",
		"p_oil_pjack_03_frg_s",
		"p_pallet_02a_s",
		"p_panties_s",
		"p_parachute1_mp_s",
		"p_parachute1_s",
		"p_parachute1_sp_s",
		"p_parachute_fallen_s",
		"p_parachute_s",
		"p_parachute_s_shop",
		"p_para_broken1_s",
		"p_patio_lounger1_s",
		"p_phonebox_02_s",
		"p_pliers_01_s",
		"p_rcss_folded",
		"p_rcss_s",
		"p_ringbinder_01_s",
		"p_sec_gate_01_s",
		"p_sec_gate_01_s_col",
		"p_shoalfish_s",
		"p_skiprope_r_s",
		"p_sofa_s",
		"p_sub_crane_s",
		"p_sunglass_m_s",
		"p_tennis_bag_01_s",
		"p_tram_crash_s",
		"p_watch_01_s",
		"p_watch_02_s",
		"p_watch_03_s",
		"p_waterboardc_s",
		"p_yacht_chair_01_s",
		"p_yacht_sofa_01_s",
		"p_yoga_mat_01_s",
		"p_yoga_mat_02_s",
		"p_yoga_mat_03_s",
		"pop_v_bank_door_l",
		"pop_v_bank_door_r",
		"prop_abat_roller_static",
		"prop_abat_slide",
		"prop_artgallery_02_dl",
		"prop_artgallery_02_dr",
		"prop_artgallery_dl",
		"prop_artgallery_dr",
		"prop_barn_door_l",
		"prop_barn_door_r",
		"prop_bh1_03_gate_l",
		"prop_bh1_03_gate_r",
		"prop_bh1_08_mp_gar",
		"prop_bh1_09_mp_gar",
		"prop_bh1_09_mp_l",
		"prop_bh1_09_mp_r",
		"prop_bh1_44_door_01l",
		"prop_bh1_44_door_01r",
		"prop_bh1_48_backdoor_l",
		"prop_bh1_48_backdoor_r",
		"prop_bh1_48_gate_1",
		"prop_bhhotel_door_l",
		"prop_bhhotel_door_r",
		"prop_biolab_g_door",
		"prop_broken_cell_gate_01",
		"prop_bs_map_door_01",
		"prop_burto_gate_01",
		"prop_casino_door_01l",
		"prop_casino_door_01r",
		"prop_ch1_07_door_01l",
		"prop_ch1_07_door_01r",
		"prop_ch1_07_door_02l",
		"prop_ch1_07_door_02r",
		"prop_ch2_05d_g_door",
		"prop_ch2_07b_20_g_door",
		"prop_ch2_09b_door",
		"prop_ch2_09c_garage_door",
		"prop_ch3_01_trlrdoor_l",
		"prop_ch3_01_trlrdoor_r",
		"prop_ch3_04_door_01l",
		"prop_ch3_04_door_01r",
		"prop_ch3_04_door_02",
		"prop_ch_025c_g_door_01",
		"prop_com_gar_door_01",
		"prop_com_ls_door_01",
		"prop_control_rm_door_01",
		"prop_cs1_14b_traind",
		"prop_cs1_14b_traind_dam",
		"prop_cs4_05_tdoor",
		"prop_cs4_10_tr_gd_01",
		"prop_cs4_11_door",
		"prop_cs6_03_door_l",
		"prop_cs6_03_door_r",
		"prop_cs_freightdoor_l1",
		"prop_cs_freightdoor_r1",
		"prop_cs_gravyard_gate_l",
		"prop_cs_gravyard_gate_r",
		"prop_cs_sc1_11_gate",
		"prop_cs_sm_27_gate",
		"prop_damdoor_01",
		"prop_door_01",
		"prop_door_balcony_frame",
		"prop_door_balcony_left",
		"prop_door_balcony_right",
		"prop_dt1_20_mp_door_l",
		"prop_dt1_20_mp_door_r",
		"prop_dt1_20_mp_gar",
		"prop_d_balcony_l_light",
		"prop_d_balcony_r_light",
		"prop_epsilon_door_l",
		"prop_epsilon_door_r",
		"prop_faceoffice_door_l",
		"prop_faceoffice_door_r",
		"prop_gar_door_01",
		"prop_gar_door_02",
		"prop_gar_door_03",
		"prop_gar_door_03_ld",
		"prop_gar_door_04",
		"prop_gar_door_05",
		"prop_gar_door_05_l",
		"prop_gar_door_05_r",
		"prop_gar_door_a_01",
		"prop_gar_door_plug",
		"prop_gd_ch2_08",
		"prop_grumandoor_l",
		"prop_grumandoor_r",
		"prop_hanger_door_1",
		"prop_hospitaldoors_start",
		"prop_hospital_door_l",
		"prop_hospital_door_r",
		"prop_hw1_03_gardoor_01",
		"prop_hw1_04_door_l1",
		"prop_hw1_04_door_r1",
		"prop_hw1_23_door",
		"prop_id2_11_gdoor",
		"prop_id_21_gardoor_01",
		"prop_id_21_gardoor_02",
		"prop_indus_meet_door_l",
		"prop_indus_meet_door_r",
		"prop_kt1_06_door_l",
		"prop_kt1_06_door_r",
		"prop_kt1_10_mpdoor_l",
		"prop_kt1_10_mpdoor_r",
		"prop_ld_garaged_01",
		"prop_ld_jail_door",
		"prop_magenta_door",
		"prop_map_door_01",
		"prop_michael_door",
		"prop_motel_door_09",
		"prop_police_door_l",
		"prop_police_door_l_dam",
		"prop_police_door_r",
		"prop_police_door_r_dam",
		"prop_police_door_surround",
		"prop_pris_door_01_l",
		"prop_pris_door_01_r",
		"prop_pris_door_02",
		"prop_pris_door_03",
		"prop_ql_revolving_door",
		"prop_ret_door",
		"prop_ret_door_02",
		"prop_ret_door_03",
		"prop_ret_door_04",
		"prop_ron_door_01",
		"prop_sc1_06_gate_l",
		"prop_sc1_06_gate_r",
		"prop_sc1_12_door",
		"prop_sc1_21_g_door_01",
		"prop_section_garage_01",
		"prop_sec_gate_01b",
		"prop_sec_gate_01c",
		"prop_sec_gate_01d",
		"prop_showroom_door_l",
		"prop_showroom_door_r",
		"prop_sm1_11_doorl",
		"prop_sm1_11_doorr",
		"prop_sm1_11_garaged",
		"prop_sm_10_mp_door",
		"prop_sm_14_mp_gar",
		"prop_sm_27_door",
		"prop_sm_27_gate",
		"prop_sm_27_gate_02",
		"prop_sm_27_gate_03",
		"prop_sm_27_gate_04",
		"prop_ss1_05_mp_door",
		"prop_ss1_08_mp_door_l",
		"prop_ss1_08_mp_door_r",
		"prop_ss1_10_door_l",
		"prop_ss1_10_door_r",
		"prop_ss1_14_garage_door",
		"prop_ss1_mpint_door_l",
		"prop_ss1_mpint_door_r",
		"prop_ss1_mpint_garage",
		"prop_ss1_mpint_garage_cl",
		"prop_strip_door_01",
		"prop_vault_shutter",
		"prop_v_door_44",
		"prop_wardrobe_door_01",
		"prop_win_plug_01",
		"prop_win_plug_01_dam",
		"p_cut_door_01",
		"p_cut_door_02",
		"p_cut_door_03",
		"p_jewel_door_l",
		"p_jewel_door_r1",
		"prop_a4_pile_01",
		"prop_a4_sheet_01",
		"prop_a4_sheet_02",
		"prop_a4_sheet_03",
		"prop_a4_sheet_04",
		"prop_a4_sheet_05",
		"prop_aiprort_sign_01",
		"prop_aiprort_sign_02",
		"prop_amanda_note_01",
		"prop_amanda_note_01b",
		"prop_amb_40oz_02",
		"prop_amb_40oz_03",
		"prop_amb_beer_bottle",
		"prop_amb_ciggy_01",
		"prop_amb_donut",
		"prop_anim_cash_note",
		"prop_anim_cash_note_b",
		"prop_anim_cash_pile_01",
		"prop_anim_cash_pile_02",
		"prop_arc_blueprints_01",
		"prop_armour_pickup",
		"prop_artifact_01",
		"prop_ashtray_01",
		"prop_aviators_01",
		"prop_a_trailer_door_01",
		"prop_ballistic_shield",
		"prop_ballistic_shield_lod1",
		"prop_barry_table_detail",
		"prop_basejump_target_01",
		"prop_battery_01",
		"prop_battery_02",
		"prop_beer_box_01",
		"prop_beta_tape",
		"prop_big_shit_01",
		"prop_big_shit_02",
		"prop_binoc_01",
		"prop_bison_winch",
		"prop_blackjack_01",
		"prop_blox_spray",
		"prop_bodyarmour_02",
		"prop_bodyarmour_03",
		"prop_bodyarmour_04",
		"prop_bodyarmour_05",
		"prop_bodyarmour_06",
		"prop_bongos_01",
		"prop_bong_01",
		"prop_boombox_01",
		"prop_bottle_cap_01",
		"prop_bowl_crisps",
		"prop_broken_cboard_p1",
		"prop_broken_cboard_p2",
		"prop_busker_hat_01",
		"prop_c4_final",
		"prop_c4_final_green",
		"prop_candy_pqs",
		"prop_cargo_int",
		"prop_carrier_bag_01",
		"prop_carrier_bag_01_lod",
		"prop_carwash_roller_horz",
		"prop_carwash_roller_vert",
		"prop_car_ignition",
		"prop_casey_sec_id",
		"prop_cash_case_01",
		"prop_cash_case_02",
		"prop_cash_crate_01",
		"prop_cash_envelope_01",
		"prop_cash_note_01",
		"prop_cash_pile_01",
		"prop_cash_pile_02",
		"prop_ceramic_jug_01",
		"prop_ceramic_jug_cork",
		"prop_cigar_01",
		"prop_cigar_02",
		"prop_cigar_03",
		"prop_cigar_pack_01",
		"prop_cigar_pack_02",
		"prop_cliff_paper",
		"prop_clippers_01",
		"prop_coffee_cup_trailer",
		"prop_coffin_02",
		"prop_coffin_02b",
		"prop_coke_block_01",
		"prop_coke_block_half_a",
		"prop_coke_block_half_b",
		"prop_controller_01",
		"prop_crisp",
		"prop_crisp_small",
		"prop_cs_abattoir_switch",
		"prop_cs_amanda_shoe",
		"prop_cs_bandana",
		"prop_cs_bar",
		"prop_cs_beer_bot_01",
		"prop_cs_beer_bot_01b",
		"prop_cs_beer_bot_01lod",
		"prop_cs_beer_bot_02",
		"prop_cs_beer_bot_03",
		"prop_cs_beer_bot_40oz",
		"prop_cs_beer_bot_40oz_02",
		"prop_cs_beer_bot_40oz_03",
		"prop_cs_beer_bot_test",
		"prop_cs_binder_01",
		"prop_cs_book_01",
		"prop_cs_bottle_opener",
		"prop_cs_bowie_knife",
		"prop_cs_bowl_01",
		"prop_cs_bowl_01b",
		"prop_cs_box_step",
		"prop_cs_brain_chunk",
		"prop_cs_bs_cup",
		"prop_cs_burger_01",
		"prop_cs_business_card",
		"prop_cs_cash_note_01",
		"prop_cs_cctv",
		"prop_cs_champ_flute",
		"prop_cs_ciggy_01",
		"prop_cs_ciggy_01b",
		"prop_cs_clothes_box",
		"prop_cs_coke_line",
		"prop_cs_cont_latch",
		"prop_cs_crackpipe",
		"prop_cs_credit_card",
		"prop_cs_creeper_01",
		"prop_cs_crisps_01",
		"prop_cs_cuffs_01",
		"prop_cs_diaphram",
		"prop_cs_dildo_01",
		"prop_cs_documents_01",
		"prop_cs_dog_lead_2a",
		"prop_cs_dog_lead_2b",
		"prop_cs_dog_lead_2c",
		"prop_cs_dog_lead_3a",
		"prop_cs_dog_lead_3b",
		"prop_cs_dog_lead_a",
		"prop_cs_dog_lead_b",
		"prop_cs_dog_lead_c",
		"prop_cs_duffel_01",
		"prop_cs_duffel_01b",
		"prop_cs_dvd",
		"prop_cs_dvd_case",
		"prop_cs_envolope_01",
		"prop_cs_film_reel_01",
		"prop_cs_fork",
		"prop_cs_frank_photo",
		"prop_cs_fuel_hose",
		"prop_cs_fuel_nozle",
		"prop_cs_gascutter_1",
		"prop_cs_gascutter_2",
		"prop_cs_glass_scrap",
		"prop_cs_gunrack",
		"prop_cs_hand_radio",
		"prop_cs_heist_rope",
		"prop_cs_heist_rope_b",
		"prop_cs_hotdog_01",
		"prop_cs_hotdog_02",
		"prop_cs_ilev_blind_01",
		"prop_cs_ironing_board",
		"prop_cs_katana_01",
		"prop_cs_kettle_01",
		"prop_cs_lazlow_ponytail",
		"prop_cs_lazlow_shirt_01",
		"prop_cs_lazlow_shirt_01b",
		"prop_cs_leg_chain_01",
		"prop_cs_lester_crate",
		"prop_cs_lipstick",
		"prop_cs_magazine",
		"prop_cs_marker_01",
		"prop_cs_meth_pipe",
		"prop_cs_milk_01",
		"prop_cs_mini_tv",
		"prop_cs_mopbucket_01",
		"prop_cs_nail_file",
		"prop_cs_newspaper",
		"prop_cs_overalls_01",
		"prop_cs_padlock",
		"prop_cs_pamphlet_01",
		"prop_cs_paper_cup",
		"prop_cs_para_ropebit",
		"prop_cs_para_ropes",
		"prop_cs_pebble",
		"prop_cs_pebble_02",
		"prop_cs_petrol_can",
		"prop_cs_phone_01",
		"prop_cs_photoframe_01",
		"prop_cs_pills",
		"prop_cs_plane_int_01",
		"prop_cs_plate_01",
		"prop_cs_police_torch",
		"prop_cs_pour_tube",
		"prop_cs_power_cell",
		"prop_cs_power_cord",
		"prop_cs_protest_sign_01",
		"prop_cs_protest_sign_02",
		"prop_cs_protest_sign_02b",
		"prop_cs_protest_sign_03",
		"prop_cs_protest_sign_04a",
		"prop_cs_protest_sign_04b",
		"prop_cs_rage_statue_p1",
		"prop_cs_rage_statue_p2",
		"prop_cs_rolled_paper",
		"prop_cs_rub_box_01",
		"prop_cs_rub_box_02",
		"prop_cs_r_business_card",
		"prop_cs_sack_01",
		"prop_cs_saucer_01",
		"prop_cs_script_bottle",
		"prop_cs_script_bottle_01",
		"prop_cs_server_drive",
		"prop_cs_sheers",
		"prop_cs_shirt_01",
		"prop_cs_shot_glass",
		"prop_cs_silver_tray",
		"prop_cs_sink_filler",
		"prop_cs_sink_filler_02",
		"prop_cs_sink_filler_03",
		"prop_cs_sol_glasses",
		"prop_cs_steak",
		"prop_cs_stock_book",
		"prop_cs_street_binbag_01",
		"prop_cs_street_card_01",
		"prop_cs_street_card_02",
		"prop_cs_sub_hook_01",
		"prop_cs_sub_rope_01",
		"prop_cs_swipe_card",
		"prop_cs_toaster",
		"prop_cs_trolley_01",
		"prop_cs_trowel",
		"prop_cs_truck_ladder",
		"prop_cs_tshirt_ball_01",
		"prop_cs_t_shirt_pile",
		"prop_cs_valve",
		"prop_cs_vent_cover",
		"prop_cs_walkie_talkie",
		"prop_cs_whiskey_bottle",
		"prop_cs_whiskey_bot_stop",
		"prop_cs_wrench",
		"prop_devin_box_01",
		"prop_devin_rope_01",
		"prop_direct_chair_01",
		"prop_direct_chair_02",
		"prop_disp_razor_01",
		"prop_distantcar_day",
		"prop_distantcar_night",
		"prop_distantcar_truck",
		"prop_donut_01",
		"prop_donut_02",
		"prop_donut_02b",
		"prop_door_bell_01",
		"prop_double_grid_line",
		"prop_drug_package",
		"prop_drug_package_02",
		"prop_dummy_car",
		"prop_ear_defenders_01",
		"prop_ecg_01",
		"prop_ecg_01_cable_01",
		"prop_ecg_01_cable_02",
		"prop_ecola_can",
		"prop_employee_month_01",
		"prop_employee_month_02",
		"prop_energy_drink",
		"prop_face_rag_01",
		"prop_fag_packet_01",
		"prop_fbi3_coffee_table",
		"prop_fib_badge",
		"prop_fib_skylight_piece",
		"prop_fishing_rod_01",
		"prop_fishing_rod_02",
		"prop_fish_slice_01",
		"prop_flare_01",
		"prop_flare_01b",
		"prop_flatbed_strap",
		"prop_flight_box_01",
		"prop_flight_box_insert",
		"prop_flight_box_insert2",
		"prop_franklin_dl",
		"prop_f_b_insert_broken",
		"prop_gaffer_arm_bind",
		"prop_gaffer_arm_bind_cut",
		"prop_gaffer_leg_bind",
		"prop_gaffer_leg_bind_cut",
		"prop_gaffer_tape",
		"prop_gaffer_tape_strip",
		"prop_glasscutter_01",
		"prop_glass_suck_holder",
		"prop_gold_cont_01",
		"prop_gold_cont_01b",
		"prop_gold_trolly_strap_01",
		"prop_gun_case_01",
		"prop_gun_case_02",
		"prop_gun_frame",
		"prop_hand_toilet",
		"prop_hard_hat_01",
		"prop_headphones_01",
		"prop_headset_01",
		"prop_hockey_bag_01",
		"prop_holster_01",
		"prop_hose_nozzle",
		"prop_huf_rag_01",
		"prop_ice_cube_01",
		"prop_ice_cube_02",
		"prop_ice_cube_03",
		"prop_id2_20_clock",
		"prop_idol_01",
		"prop_idol_case",
		"prop_idol_case_02",
		"prop_ing_crowbar",
		"prop_inhaler_01",
		"prop_iron_01",
		"prop_jet_bloodsplat_01",
		"prop_juice_pool_01",
		"prop_ld_ammo_pack_01",
		"prop_ld_ammo_pack_02",
		"prop_ld_ammo_pack_03",
		"prop_ld_barrier_01",
		"prop_ld_bomb_01",
		"prop_ld_bomb_01_open",
		"prop_ld_bomb_anim",
		"prop_ld_can_01",
		"prop_ld_contact_card",
		"prop_ld_crocclips01",
		"prop_ld_crocclips02",
		"prop_ld_dummy_rope",
		"prop_ld_fags_01",
		"prop_ld_fags_02",
		"prop_ld_faucet",
		"prop_ld_ferris_wheel",
		"prop_ld_fireaxe",
		"prop_ld_flow_bottle",
		"prop_ld_gold_tooth",
		"prop_ld_hdd_01",
		"prop_ld_peep_slider",
		"prop_ld_rail_01",
		"prop_ld_rail_02",
		"prop_ld_rubble_01",
		"prop_ld_rubble_02",
		"prop_ld_rubble_03",
		"prop_ld_rubble_04",
		"prop_ld_scrap",
		"prop_ld_shovel",
		"prop_ld_shovel_dirt",
		"prop_ld_snack_01",
		"prop_ld_test_01",
		"prop_ld_tooth",
		"prop_ld_wallet_02",
		"prop_ld_wallet_pickup",
		"prop_lev_des_barge_01",
		"prop_lev_des_barge_02",
		"prop_lift_overlay_01",
		"prop_lift_overlay_02",
		"prop_litter_picker",
		"prop_loose_rag_01",
		"prop_med_jet_01",
		"prop_megaphone_01",
		"prop_mem_candle_04",
		"prop_mem_candle_05",
		"prop_mem_candle_06",
		"prop_meth_bag_01",
		"prop_michael_balaclava",
		"prop_michael_sec_id",
		"prop_microphone_02",
		"prop_military_pickup_01",
		"prop_mil_crate_01",
		"prop_mil_crate_02",
		"prop_minigun_01",
		"prop_money_bag_01",
		"prop_mp3_dock",
		"prop_mp_arrow_barrier_01",
		"prop_mp_barrier_01",
		"prop_mp_barrier_01b",
		"prop_mp_barrier_02",
		"prop_mp_barrier_02b",
		"prop_mp_base_marker",
		"prop_mp_boost_01",
		"prop_mp_cant_place_lrg",
		"prop_mp_cant_place_med",
		"prop_mp_cant_place_sm",
		"prop_mp_cone_01",
		"prop_mp_cone_02",
		"prop_mp_cone_03",
		"prop_mp_cone_04",
		"prop_mp_drug_package",
		"prop_mp_drug_pack_blue",
		"prop_mp_drug_pack_red",
		"prop_mp_icon_shad_lrg",
		"prop_mp_icon_shad_med",
		"prop_mp_icon_shad_sm",
		"prop_mp_max_out_lrg",
		"prop_mp_max_out_med",
		"prop_mp_max_out_sm",
		"prop_mp_num_0",
		"prop_mp_num_1",
		"prop_mp_num_2",
		"prop_mp_num_3",
		"prop_mp_num_4",
		"prop_mp_num_5",
		"prop_mp_num_6",
		"prop_mp_num_7",
		"prop_mp_num_8",
		"prop_mp_num_9",
		"prop_mp_placement",
		"prop_mp_placement_lrg",
		"prop_mp_placement_maxd",
		"prop_mp_placement_med",
		"prop_mp_placement_red",
		"prop_mp_placement_sm",
		"prop_mp_ramp_01",
		"prop_mp_ramp_02",
		"prop_mp_ramp_03",
		"prop_mp_repair",
		"prop_mp_repair_01",
		"prop_mp_respawn_02",
		"prop_mp_rocket_01",
		"prop_mp_spike_01",
		"prop_m_pack_int_01",
		"prop_nigel_bag_pickup",
		"prop_notepad_01",
		"prop_notepad_02",
		"prop_old_boot",
		"prop_orang_can_01",
		"prop_parking_wand_01",
		"prop_passport_01",
		"prop_peanut_bowl_01",
		"prop_pencil_01",
		"prop_piercing_gun",
		"prop_ping_pong",
		"prop_pistol_holster",
		"prop_plastic_cup_02",
		"prop_player_gasmask",
		"prop_pliers_01",
		"prop_police_radio_handset",
		"prop_police_radio_main",
		"prop_poster_tube_01",
		"prop_poster_tube_02",
		"prop_power_cell",
		"prop_power_cord_01",
		"prop_premier_fence_01",
		"prop_premier_fence_02",
		"prop_quad_grid_line",
		"prop_rad_waste_barrel_01",
		"prop_rail_controller",
		"prop_range_target_01",
		"prop_range_target_02",
		"prop_range_target_03",
		"prop_riot_shield",
		"prop_rolled_sock_01",
		"prop_rolled_sock_02",
		"prop_rope_family_3",
		"prop_safety_glasses",
		"prop_sam_01",
		"prop_sandwich_01",
		"prop_scaffold_pole",
		"prop_scn_police_torch",
		"prop_scourer_01",
		"prop_scrap_2_crate",
		"prop_sewing_fabric",
		"prop_sewing_machine",
		"prop_shower_towel",
		"prop_sh_beer_pissh_01",
		"prop_sh_bong_01",
		"prop_sh_cigar_01",
		"prop_sh_joint_01",
		"prop_sh_mr_rasp_01",
		"prop_sh_shot_glass",
		"prop_sh_tall_glass",
		"prop_sh_tt_fridgedoor",
		"prop_sh_wine_glass",
		"prop_single_grid_line",
		"prop_smg_holster_01",
		"prop_space_pistol",
		"prop_space_rifle",
		"prop_spycam",
		"prop_squeegee",
		"prop_stag_do_rope",
		"prop_start_finish_line_01",
		"prop_start_grid_01",
		"prop_stockade_wheel",
		"prop_stockade_wheel_flat",
		"prop_sub_cover_01",
		"prop_sub_release",
		"prop_syringe_01",
		"prop_table_mic_01",
		"prop_table_ten_bat",
		"prop_taco_01",
		"prop_taco_02",
		"prop_tapeplayer_01",
		"prop_target_arm",
		"prop_target_arm_b",
		"prop_target_arm_long",
		"prop_target_arm_sm",
		"prop_target_backboard",
		"prop_target_backboard_b",
		"prop_target_blue",
		"prop_target_blue_arrow",
		"prop_target_bull",
		"prop_target_bull_b",
		"prop_target_comp_metal",
		"prop_target_comp_wood",
		"prop_target_frame_01",
		"prop_target_inner1",
		"prop_target_inner2",
		"prop_target_inner2_b",
		"prop_target_inner3",
		"prop_target_inner3_b",
		"prop_target_inner_b",
		"prop_target_orange_arrow",
		"prop_target_oran_cross",
		"prop_target_ora_purp_01",
		"prop_target_purp_arrow",
		"prop_target_purp_cross",
		"prop_target_red",
		"prop_target_red_arrow",
		"prop_target_red_blue_01",
		"prop_target_red_cross",
		"prop_tarp_strap",
		"prop_tea_trolly",
		"prop_temp_block_blocker",
		"prop_tennis_ball",
		"prop_tennis_rack_01",
		"prop_tennis_rack_01b",
		"prop_time_capsule_01",
		"prop_tornado_wheel",
		"prop_torture_01",
		"prop_tourist_map_01",
		"prop_trailer_01_new",
		"prop_trailer_door_closed",
		"prop_trailer_door_open",
		"prop_trevor_rope_01",
		"prop_trev_sec_id",
		"prop_triple_grid_line",
		"prop_tri_finish_banner",
		"prop_tri_start_banner",
		"prop_tumbler_01",
		"prop_tumbler_01b",
		"prop_tumbler_01_empty",
		"prop_turkey_leg_01",
		"prop_tv_cam_02",
		"prop_tv_test",
		"prop_t_shirt_ironing",
		"prop_voltmeter_01",
		"prop_water_corpse_01",
		"prop_water_corpse_02",
		"prop_weed_01",
		"prop_weed_02",
		"prop_weed_block_01",
		"prop_weed_bottle",
		"prop_weed_pallet",
		"prop_weed_tub_01",
		"prop_weed_tub_01b",
		"prop_welding_mask_01",
		"prop_weld_torch",
		"prop_wheat_grass_empty",
		"prop_wheat_grass_glass",
		"prop_winch_hook_long",
		"prop_winch_hook_short",
		"prop_wrecked_buzzard",
		"prop_yacht_lounger",
		"prop_yacht_seat_01",
		"prop_yacht_seat_02",
		"prop_yacht_seat_03",
		"prop_yacht_table_01",
		"prop_yacht_table_02",
		"prop_yacht_table_03",
		"prop_yell_plastic_target",
		"p_amb_bagel_01",
		"p_cs_bbbat_01",
		"p_cs_bottle_01",
		"p_cs_clipboard",
		"p_cs_joint_01",
		"p_cs_joint_02",
		"p_cs_lighter_01",
		"p_cs_papers_01",
		"p_cs_papers_02",
		"p_cs_papers_03",
		"p_ing_bagel_01",
		"p_ing_microphonel_01",
		"p_ld_bs_bag_01",
		"p_ld_id_card_002",
		"p_ld_id_card_01",
		"p_rc_handset",
		"p_whiskey_notop",
		"p_whiskey_notop_empty",
		"prop_crashed_heli",
		"prop_ld_bankdoors_02",
		"prop_ld_fragwall_01a",
		"prop_pipe_single_01",
		"prop_test_boulder_01",
		"prop_test_boulder_02",
		"prop_test_boulder_03",
		"prop_test_boulder_04",
		"prop_trailr_base",
		"prop_trailr_base_static",
		"proair_hoc_puck",
		"prop_airhockey_01",
		"prop_air_hoc_paddle_01",
		"prop_air_hoc_paddle_02",
		"prop_arcade_01",
		"prop_arcade_02",
		"prop_ice_box_01",
		"prop_ice_box_01_l1",
		"prop_jukebox_01",
		"prop_jukebox_02",
		"prop_park_ticket_01",
		"prop_pier_kiosk_01",
		"prop_pier_kiosk_02",
		"prop_pier_kiosk_03",
		"prop_pooltable_02",
		"prop_pooltable_3b",
		"prop_pool_cue",
		"prop_pool_rack_01",
		"prop_pool_rack_02",
		"prop_pool_tri",
		"prop_telescope",
		"prop_train_ticket_02",
		"prop_turnstyle_01",
		"prop_vend_coffe_01",
		"prop_vend_condom_01",
		"prop_vend_fags_01",
		"prop_vend_fridge01",
		"prop_vend_snak_01",
		"prop_apple_box_01",
		"prop_apple_box_02",
		"prop_arm_wrestle_01",
		"prop_a_base_bars_01",
		"prop_barbell_01",
		"prop_barbell_02",
		"prop_barbell_100kg",
		"prop_barbell_10kg",
		"prop_barbell_20kg",
		"prop_barbell_30kg",
		"prop_barbell_40kg",
		"prop_barbell_50kg",
		"prop_barbell_60kg",
		"prop_barbell_80kg",
		"prop_basketball_net",
		"prop_bball_arcade_01",
		"prop_beachbag_01",
		"prop_beachbag_02",
		"prop_beachbag_03",
		"prop_beachbag_04",
		"prop_beachbag_05",
		"prop_beachbag_06",
		"prop_beachbag_combo_01",
		"prop_beachbag_combo_02",
		"prop_beachball_02",
		"prop_beachflag_le",
		"prop_beach_bars_01",
		"prop_beach_bars_02",
		"prop_beach_bbq",
		"prop_beach_dip_bars_01",
		"prop_beach_dip_bars_02",
		"prop_beach_fire",
		"prop_beach_lg_float",
		"prop_beach_lg_stretch",
		"prop_beach_lg_surf",
		"prop_beach_lotion_01",
		"prop_beach_lotion_02",
		"prop_beach_lotion_03",
		"prop_beach_punchbag",
		"prop_beach_rings_01",
		"prop_beach_sculp_01",
		"prop_beach_towel_02",
		"prop_beach_volball01",
		"prop_beach_volball02",
		"prop_bikini_disp_01",
		"prop_bikini_disp_02",
		"prop_bikini_disp_03",
		"prop_bikini_disp_04",
		"prop_bikini_disp_05",
		"prop_bikini_disp_06",
		"prop_bleachers_01",
		"prop_bleachers_02",
		"prop_bleachers_03",
		"prop_bleachers_04",
		"prop_bleachers_05",
		"prop_boogbd_stack_01",
		"prop_boogbd_stack_02",
		"prop_boogieboard_01",
		"prop_boogieboard_02",
		"prop_boogieboard_03",
		"prop_boogieboard_04",
		"prop_boogieboard_05",
		"prop_boogieboard_06",
		"prop_boogieboard_07",
		"prop_boogieboard_08",
		"prop_boogieboard_09",
		"prop_boogieboard_10",
		"prop_bowling_ball",
		"prop_bowling_pin",
		"prop_boxing_glove_01",
		"prop_buck_spade_01",
		"prop_buck_spade_02",
		"prop_buck_spade_03",
		"prop_buck_spade_04",
		"prop_buck_spade_05",
		"prop_buck_spade_06",
		"prop_buck_spade_07",
		"prop_buck_spade_08",
		"prop_buck_spade_09",
		"prop_buck_spade_10",
		"prop_bumper_car_01",
		"prop_can_canoe",
		"prop_cap_01",
		"prop_cap_01b",
		"prop_cap_row_01",
		"prop_cap_row_01b",
		"prop_cap_row_02",
		"prop_cap_row_02b",
		"prop_clothes_rail_02",
		"prop_clothes_rail_03",
		"prop_clothes_rail_2b",
		"prop_clothes_tub_01",
		"prop_cs_beachtowel_01",
		"prop_cup_saucer_01",
		"prop_curl_bar_01",
		"prop_dart_1",
		"prop_dart_2",
		"prop_dart_bd_01",
		"prop_dart_bd_cab_01",
		"prop_display_unit_01",
		"prop_display_unit_02",
		"prop_disp_cabinet_002",
		"prop_disp_cabinet_01",
		"prop_dolly_01",
		"prop_dolly_02",
		"prop_dress_disp_01",
		"prop_dress_disp_02",
		"prop_dress_disp_03",
		"prop_dress_disp_04",
		"prop_drug_burner",
		"prop_exer_bike_01",
		"prop_ferris_car_01",
		"prop_ferris_car_01_lod1",
		"prop_film_cam_01",
		"prop_flipchair_01",
		"prop_food_van_01",
		"prop_food_van_02",
		"prop_freeweight_01",
		"prop_freeweight_02",
		"prop_front_seat_01",
		"prop_front_seat_02",
		"prop_front_seat_03",
		"prop_front_seat_04",
		"prop_front_seat_05",
		"prop_front_seat_06",
		"prop_front_seat_07",
		"prop_front_seat_row_01",
		"prop_ftowel_01",
		"prop_ftowel_07",
		"prop_ftowel_08",
		"prop_ftowel_10",
		"prop_game_clock_01",
		"prop_game_clock_02",
		"prop_golf_bag_01",
		"prop_golf_bag_01b",
		"prop_golf_bag_01c",
		"prop_golf_ball",
		"prop_golf_ball_p2",
		"prop_golf_ball_p3",
		"prop_golf_ball_p4",
		"prop_golf_ball_tee",
		"prop_golf_driver",
		"prop_golf_iron_01",
		"prop_golf_marker_01",
		"prop_golf_pitcher_01",
		"prop_golf_putter_01",
		"prop_golf_tee",
		"prop_golf_wood_01",
		"prop_hat_box_01",
		"prop_hat_box_02",
		"prop_hat_box_03",
		"prop_hat_box_04",
		"prop_hat_box_05",
		"prop_hat_box_06",
		"prop_henna_disp_01",
		"prop_henna_disp_02",
		"prop_henna_disp_03",
		"prop_hwbowl_pseat_6x1",
		"prop_hwbowl_seat_01",
		"prop_hwbowl_seat_02",
		"prop_hwbowl_seat_03",
		"prop_hwbowl_seat_03b",
		"prop_hwbowl_seat_6x6",
		"prop_hydro_platform_01",
		"prop_inflatearch_01",
		"prop_inflategate_01",
		"prop_jeans_01",
		"prop_kayak_01",
		"prop_kayak_01b",
		"prop_kino_light_01",
		"prop_kino_light_03",
		"prop_ld_hat_01",
		"prop_ld_jeans_01",
		"prop_ld_jeans_02",
		"prop_ld_shirt_01",
		"prop_ld_shoe_01",
		"prop_ld_shoe_02",
		"prop_ld_tshirt_01",
		"prop_ld_tshirt_02",
		"prop_life_ring_02",
		"prop_makeup_trail_01",
		"prop_makeup_trail_02",
		"prop_mat_box",
		"prop_movie_rack",
		"prop_muscle_bench_01",
		"prop_muscle_bench_02",
		"prop_muscle_bench_03",
		"prop_muscle_bench_04",
		"prop_muscle_bench_05",
		"prop_muscle_bench_06",
		"prop_offroad_bale01",
		"prop_offroad_barrel01",
		"prop_offroad_tyres01",
		"prop_plate_stand_01",
		"prop_poolball_1",
		"prop_poolball_10",
		"prop_poolball_11",
		"prop_poolball_12",
		"prop_poolball_13",
		"prop_poolball_14",
		"prop_poolball_15",
		"prop_poolball_2",
		"prop_poolball_3",
		"prop_poolball_4",
		"prop_poolball_5",
		"prop_poolball_6",
		"prop_poolball_7",
		"prop_poolball_8",
		"prop_poolball_9",
		"prop_poolball_cue",
		"prop_porn_mag_01",
		"prop_porn_mag_02",
		"prop_porn_mag_03",
		"prop_porn_mag_04",
		"prop_postcard_rack",
		"prop_pris_bars_01",
		"prop_pris_bench_01",
		"prop_prop_tree_01",
		"prop_prop_tree_02",
		"prop_punch_bag_l",
		"prop_roller_car_01",
		"prop_roller_car_02",
		"prop_scrim_01",
		"prop_set_generator_01",
		"prop_sglasses_stand_01",
		"prop_sglasses_stand_02",
		"prop_sglasses_stand_02b",
		"prop_sglasses_stand_03",
		"prop_sglasss_1b_lod",
		"prop_sglasss_1_lod",
		"prop_shop_front_door_l",
		"prop_shop_front_door_r",
		"prop_skate_flatramp",
		"prop_skate_funbox",
		"prop_skate_halfpipe",
		"prop_skate_kickers",
		"prop_skate_quartpipe",
		"prop_skate_rail",
		"prop_skate_spiner",
		"prop_skip_rope_01",
		"prop_slacks_01",
		"prop_slacks_02",
		"prop_speedball_01",
		"prop_sports_clock_01",
		"prop_studio_light_02",
		"prop_studio_light_03",
		"prop_suitcase_01",
		"prop_suitcase_01b",
		"prop_suitcase_01c",
		"prop_suitcase_01d",
		"prop_suitcase_02",
		"prop_suitcase_03",
		"prop_suitcase_03b",
		"prop_surf_board_ldn_01",
		"prop_surf_board_ldn_02",
		"prop_surf_board_ldn_03",
		"prop_surf_board_ldn_04",
		"prop_table_tennis",
		"prop_tennis_ball_lobber",
		"prop_tint_towel",
		"prop_tint_towels_01",
		"prop_tint_towels_01b",
		"prop_towel2_01",
		"prop_towel2_02",
		"prop_towel_shelf_01",
		"prop_tri_pod",
		"prop_tri_pod_lod",
		"prop_tshirt_box_02",
		"prop_tshirt_shelf_1",
		"prop_tshirt_shelf_2",
		"prop_tshirt_shelf_2a",
		"prop_tshirt_shelf_2b",
		"prop_tshirt_shelf_2c",
		"prop_tshirt_stand_01",
		"prop_tshirt_stand_01b",
		"prop_tshirt_stand_02",
		"prop_tshirt_stand_04",
		"prop_t_shirt_row_01",
		"prop_t_shirt_row_02",
		"prop_t_shirt_row_02b",
		"prop_t_shirt_row_03",
		"prop_t_shirt_row_04",
		"prop_t_shirt_row_05l",
		"prop_t_shirt_row_05r",
		"prop_venice_board_01",
		"prop_venice_board_02",
		"prop_venice_board_03",
		"prop_venice_counter_01",
		"prop_venice_counter_02",
		"prop_venice_counter_03",
		"prop_venice_counter_04",
		"prop_venice_shop_front_01",
		"prop_venice_sign_09",
		"prop_venice_sign_10",
		"prop_venice_sign_11",
		"prop_venice_sign_12",
		"prop_venice_sign_14",
		"prop_venice_sign_15",
		"prop_venice_sign_16",
		"prop_venice_sign_17",
		"prop_venice_sign_18",
		"prop_ven_market_stool",
		"prop_ven_market_table1",
		"prop_ven_shop_1_counter",
		"prop_v_15_cars_clock",
		"prop_v_5_bclock",
		"prop_weight_10k",
		"prop_weight_15k",
		"prop_weight_1_5k",
		"prop_weight_20k",
		"prop_weight_2_5k",
		"prop_weight_5k",
		"prop_weight_rack_01",
		"prop_weight_rack_02",
		"p_film_set_static_01",
		"prop_handdry_01",
		"prop_handdry_02",
		"prop_shower_rack_01",
		"prop_sink_02",
		"prop_sink_04",
		"prop_sink_05",
		"prop_sink_06",
		"prop_soap_disp_01",
		"prop_sponge_01",
		"prop_toilet_01",
		"prop_toilet_02",
		"prop_toilet_brush_01",
		"prop_toilet_roll_01",
		"prop_toilet_roll_02",
		"prop_toilet_roll_05",
		"prop_toilet_shamp_01",
		"prop_toilet_shamp_02",
		"prop_toilet_soap_01",
		"prop_toilet_soap_02",
		"prop_toilet_soap_03",
		"prop_toilet_soap_04",
		"prop_toothbrush_01",
		"prop_toothb_cup_01",
		"prop_toothpaste_01",
		"prop_towel_01",
		"prop_towel_rail_01",
		"prop_towel_rail_02",
		"prop_w_fountain_01",
		"prop_amp_01",
		"prop_cctv_02_sm",
		"prop_cctv_cont_01",
		"prop_cctv_cont_03",
		"prop_cctv_cont_04",
		"prop_cctv_cont_05",
		"prop_cctv_cont_06",
		"prop_cctv_unit_01",
		"prop_cctv_unit_02",
		"prop_cctv_unit_05",
		"prop_console_01",
		"prop_cs_dvd_player",
		"prop_cs_keyboard_01",
		"prop_cs_mouse_01",
		"prop_cs_tv_stand",
		"prop_dj_deck_01",
		"prop_dj_deck_02",
		"prop_flatscreen_overlay",
		"prop_ghettoblast_02",
		"prop_hifi_01",
		"prop_keyboard_01a",
		"prop_keyboard_01b",
		"prop_laptop_02_closed",
		"prop_laptop_jimmy",
		"prop_laptop_lester",
		"prop_laptop_lester2",
		"prop_michaels_credit_tv",
		"prop_monitor_01c",
		"prop_monitor_01d",
		"prop_monitor_02",
		"prop_monitor_03b",
		"prop_mouse_01",
		"prop_mouse_01a",
		"prop_mouse_01b",
		"prop_mouse_02",
		"prop_pc_01a",
		"prop_pc_02a",
		"prop_portable_hifi_01",
		"prop_projector_overlay",
		"prop_speaker_01",
		"prop_speaker_02",
		"prop_speaker_03",
		"prop_speaker_05",
		"prop_speaker_06",
		"prop_speaker_07",
		"prop_speaker_08",
		"prop_till_01_dam",
		"prop_till_02",
		"prop_till_03",
		"prop_trailer_monitor_01",
		"prop_trev_tv_01",
		"prop_tt_screenstatic",
		"prop_tv_02",
		"prop_tv_03_overlay",
		"prop_tv_04",
		"prop_tv_05",
		"prop_tv_06",
		"prop_tv_07",
		"prop_tv_cabinet_03",
		"prop_tv_cabinet_04",
		"prop_tv_cabinet_05",
		"prop_tv_flat_01",
		"prop_tv_flat_01_screen",
		"prop_tv_flat_02b",
		"prop_tv_flat_03",
		"prop_tv_flat_03b",
		"prop_tv_flat_michael",
		"prop_vcr_01",
		"prop_white_keyboard",
		"prop_bbq_2",
		"prop_bbq_3",
		"prop_beware_dog_sign",
		"prop_forsalejr2",
		"prop_forsalejr3",
		"prop_forsalejr4",
		"prop_fountain1",
		"prop_fountain2",
		"prop_garden_dreamcatch_01",
		"prop_garden_edging_01",
		"prop_garden_edging_02",
		"prop_garden_zapper_01",
		"prop_gardnght_01",
		"prop_gazebo_03",
		"prop_glf_roller",
		"prop_glf_spreader",
		"prop_gravestones_01a",
		"prop_gravestones_02a",
		"prop_gravestones_03a",
		"prop_gravestones_04a",
		"prop_gravestones_05a",
		"prop_gravestones_06a",
		"prop_gravestones_07a",
		"prop_gravestones_08a",
		"prop_gravestones_09a",
		"prop_gravestones_10a",
		"prop_gravetomb_01a",
		"prop_gravetomb_02a",
		"prop_hedge_trimmer_01",
		"prop_hose_1",
		"prop_hose_2",
		"prop_hose_3",
		"prop_hottub2",
		"prop_lawnmower_01",
		"prop_leaf_blower_01",
		"prop_letterbox_04",
		"prop_outdoor_fan_01",
		"prop_owl_totem_01",
		"prop_poolskimmer",
		"prop_prlg_gravestone_05a_l1",
		"prop_prlg_gravestone_06a",
		"prop_shrub_rake",
		"prop_sign_mallet",
		"prop_spray_backpack_01",
		"prop_sprink_crop_01",
		"prop_sprink_golf_01",
		"prop_sprink_park_01",
		"prop_stickbfly",
		"prop_stickhbird",
		"prop_wateringcan",
		"prop_windmill2",
		"test_prop_gravestones_04a",
		"test_prop_gravestones_05a",
		"test_prop_gravestones_07a",
		"test_prop_gravestones_08a",
		"test_prop_gravestones_09a",
		"test_prop_gravetomb_01a",
		"test_prop_gravetomb_02a",
		"prop_breadbin_01",
		"prop_cleaver",
		"prop_cooker_03",
		"prop_copper_pan",
		"prop_fridge_01",
		"prop_fridge_03",
		"prop_ind_washer_02",
		"prop_kettle",
		"prop_kettle_01",
		"prop_kitch_juicer",
		"prop_kitch_pot_fry",
		"prop_kitch_pot_huge",
		"prop_kitch_pot_lrg",
		"prop_kitch_pot_lrg2",
		"prop_kitch_pot_med",
		"prop_kitch_pot_sm",
		"prop_knife",
		"prop_knife_stand",
		"prop_ladel",
		"prop_metalfoodjar_01",
		"prop_micro_01",
		"prop_micro_02",
		"prop_micro_cs_01",
		"prop_micro_cs_01_door",
		"prop_plate_04",
		"prop_plate_warmer",
		"prop_pot_01",
		"prop_pot_02",
		"prop_pot_03",
		"prop_pot_04",
		"prop_pot_05",
		"prop_pot_06",
		"prop_pot_rack",
		"prop_steam_basket_01",
		"prop_steam_basket_02",
		"prop_tea_urn",
		"prop_toaster_01",
		"prop_toaster_02",
		"prop_utensil",
		"prop_washer_01",
		"prop_washer_02",
		"prop_washer_03",
		"prop_washing_basket_01",
		"prop_whisk",
		"prop_wok",
		"prop_armchair_01",
		"prop_couch_01",
		"prop_couch_03",
		"prop_couch_04",
		"prop_couch_lg_02",
		"prop_couch_lg_05",
		"prop_couch_lg_06",
		"prop_couch_lg_07",
		"prop_couch_lg_08",
		"prop_couch_sm1_07",
		"prop_couch_sm2_07",
		"prop_couch_sm_02",
		"prop_couch_sm_05",
		"prop_couch_sm_06",
		"prop_couch_sm_07",
		"prop_yaught_chair_01",
		"prop_yaught_sofa_01",
		"prop_bin_04a",
		"prop_bin_10a",
		"prop_bin_10b",
		"prop_bin_11a",
		"prop_bin_11b",
		"prop_bin_12a",
		"prop_bin_13a",
		"prop_bin_14a",
		"prop_bin_14b",
		"prop_bin_beach_01d",
		"prop_bin_delpiero",
		"prop_bin_delpiero_b",
		"prop_dumpster_3a",
		"prop_dumpster_3step",
		"prop_dumpster_4a",
		"prop_dumpster_4b",
		"prop_recyclebin_02a",
		"prop_recyclebin_02b",
		"prop_recyclebin_02_c",
		"prop_recyclebin_02_d",
		"prop_recyclebin_03_a",
		"prop_recyclebin_04_a",
		"prop_recyclebin_04_b",
		"prop_recyclebin_05_a",
		"prop_skip_01a",
		"prop_skip_02a",
		"prop_skip_03",
		"prop_skip_04",
		"prop_skip_05a",
		"prop_skip_05b",
		"prop_skip_06a",
		"prop_skip_08a",
		"prop_skip_08b",
		"prop_skip_10a",
		"prop_bandsaw_01",
		"prop_barier_conc_01b",
		"prop_barier_conc_01c",
		"prop_barier_conc_02b",
		"prop_barier_conc_02c",
		"prop_barier_conc_03a",
		"prop_barier_conc_04a",
		"prop_barier_conc_05a",
		"prop_barier_conc_05b",
		"prop_barriercrash_03",
		"prop_barriercrash_04",
		"prop_barrier_wat_01a",
		"prop_barrier_wat_03b",
		"prop_barrier_work01c",
		"prop_cablespool_01a",
		"prop_cablespool_01b",
		"prop_cablespool_02",
		"prop_cablespool_03",
		"prop_cablespool_04",
		"prop_cablespool_05",
		"prop_cablespool_06",
		"prop_cementmixer_01a",
		"prop_cementmixer_02a",
		"prop_conc_sacks_02a",
		"prop_conschute",
		"prop_consign_01c",
		"prop_consign_02a",
		"prop_conslift_base",
		"prop_conslift_brace",
		"prop_conslift_cage",
		"prop_conslift_door",
		"prop_conslift_lift",
		"prop_conslift_rail",
		"prop_conslift_rail2",
		"prop_conslift_steps",
		"prop_cons_crate",
		"prop_cons_plank",
		"prop_cons_ply01",
		"prop_cons_ply02",
		"prop_crosssaw_01",
		"prop_diggerbkt_01",
		"prop_drywallpile_01",
		"prop_drywallpile_02",
		"prop_ducktape_01",
		"prop_etricmotor_01",
		"prop_generator_02a",
		"prop_generator_03a",
		"prop_generator_04",
		"prop_girder_01a",
		"prop_logpile_05",
		"prop_logpile_06",
		"prop_logpile_06b",
		"prop_logpile_07",
		"prop_logpile_07b",
		"prop_log_01",
		"prop_log_02",
		"prop_log_03",
		"prop_medstation_02",
		"prop_medstation_03",
		"prop_medstation_04",
		"prop_metal_plates01",
		"prop_metal_plates02",
		"prop_oiltub_01",
		"prop_oiltub_02",
		"prop_oiltub_03",
		"prop_oiltub_05",
		"prop_oiltub_06",
		"prop_paints_can01",
		"prop_paints_can02",
		"prop_paints_can03",
		"prop_paints_can04",
		"prop_paints_can05",
		"prop_paints_can06",
		"prop_paints_can07",
		"prop_paint_brush01",
		"prop_paint_brush02",
		"prop_paint_brush03",
		"prop_paint_brush04",
		"prop_paint_brush05",
		"prop_paint_roller",
		"prop_paint_spray01a",
		"prop_paint_spray01b",
		"prop_paint_stepl01",
		"prop_paint_stepl01b",
		"prop_paint_stepl02",
		"prop_paint_tray",
		"prop_paint_wpaper01",
		"prop_partsbox_01",
		"prop_pile_dirt_01",
		"prop_pile_dirt_02",
		"prop_pile_dirt_03",
		"prop_pile_dirt_04",
		"prop_pile_dirt_06",
		"prop_pile_dirt_07",
		"prop_pipes_01a",
		"prop_pipes_01b",
		"prop_pipes_03b",
		"prop_pipes_04a",
		"prop_pipes_05a",
		"prop_pipes_conc_01",
		"prop_pipes_conc_02",
		"prop_planer_01",
		"prop_plas_barier_01a",
		"prop_plywoodpile_01a",
		"prop_plywoodpile_01b",
		"prop_rebar_pile01",
		"prop_roadcone01a",
		"prop_roadcone01b",
		"prop_roadcone01c",
		"prop_roadcone02a",
		"prop_roadcone02b",
		"prop_roadcone02c",
		"prop_roadheader_01",
		"prop_scafold_01a",
		"prop_scafold_01c",
		"prop_scafold_01f",
		"prop_scafold_02a",
		"prop_scafold_02c",
		"prop_scafold_03a",
		"prop_scafold_03b",
		"prop_scafold_03c",
		"prop_scafold_03f",
		"prop_scafold_04a",
		"prop_scafold_05a",
		"prop_scafold_06a",
		"prop_scafold_06b",
		"prop_scafold_06c",
		"prop_scafold_07a",
		"prop_scafold_08a",
		"prop_scafold_09a",
		"prop_scafold_frame1a",
		"prop_scafold_frame1b",
		"prop_scafold_frame1c",
		"prop_scafold_frame1f",
		"prop_scafold_frame2a",
		"prop_scafold_frame2b",
		"prop_scafold_frame2c",
		"prop_scafold_frame3a",
		"prop_scafold_frame3c",
		"prop_scafold_rail_01",
		"prop_scafold_rail_02",
		"prop_scafold_rail_03",
		"prop_scafold_xbrace",
		"prop_shuttering01",
		"prop_shuttering02",
		"prop_shuttering03",
		"prop_shuttering04",
		"prop_spraygun_01",
		"prop_staticmixer_01",
		"prop_steps_big_01",
		"prop_tablesaw_01",
		"prop_tool_adjspanner",
		"prop_tool_bench01",
		"prop_tool_bluepnt",
		"prop_tool_box_01",
		"prop_tool_box_02",
		"prop_tool_box_03",
		"prop_tool_box_04",
		"prop_tool_box_05",
		"prop_tool_box_06",
		"prop_tool_box_07",
		"prop_tool_broom",
		"prop_tool_broom2",
		"prop_tool_broom2_l1",
		"prop_tool_cable01",
		"prop_tool_cable02",
		"prop_tool_consaw",
		"prop_tool_drill",
		"prop_tool_fireaxe",
		"prop_tool_hammer",
		"prop_tool_hardhat",
		"prop_tool_jackham",
		"prop_tool_mallet",
		"prop_tool_mopbucket",
		"prop_tool_nailgun",
		"prop_tool_pickaxe",
		"prop_tool_pliers",
		"prop_tool_rake",
		"prop_tool_rake_l1",
		"prop_tool_sawhorse",
		"prop_tool_screwdvr01",
		"prop_tool_screwdvr02",
		"prop_tool_screwdvr03",
		"prop_tool_shovel",
		"prop_tool_shovel006",
		"prop_tool_shovel2",
		"prop_tool_shovel3",
		"prop_tool_shovel4",
		"prop_tool_shovel5",
		"prop_tool_sledgeham",
		"prop_tool_spanner01",
		"prop_tool_spanner02",
		"prop_tool_spanner03",
		"prop_tool_torch",
		"prop_tool_wrench",
		"prop_towercrane_01a",
		"prop_towercrane_02a",
		"prop_towercrane_02b",
		"prop_towercrane_02c",
		"prop_towercrane_02d",
		"prop_towercrane_02e",
		"prop_towercrane_02el",
		"prop_towercrane_02el2",
		"prop_tunnel_liner01",
		"prop_tunnel_liner02",
		"prop_tunnel_liner03",
		"prop_vertdrill_01",
		"prop_wheelbarrow01a",
		"prop_wheelbarrow02a",
		"prop_woodpile_02a",
		"prop_worklight_01a_l1",
		"prop_worklight_04a",
		"prop_worklight_04b_l1",
		"prop_worklight_04c_l1",
		"prop_worklight_04d_l1",
		"prop_workwall_01",
		"prop_workwall_02",
		"prop_armenian_gate",
		"prop_arm_gate_l",
		"prop_const_fence01a",
		"prop_const_fence01b",
		"prop_const_fence02a",
		"prop_const_fence02b",
		"prop_const_fence03b",
		"prop_facgate_01",
		"prop_facgate_01b",
		"prop_facgate_02pole",
		"prop_facgate_02_l",
		"prop_facgate_03post",
		"prop_facgate_03_l",
		"prop_facgate_03_ld_l",
		"prop_facgate_03_ld_r",
		"prop_facgate_03_r",
		"prop_facgate_04_l",
		"prop_facgate_04_r",
		"prop_facgate_05_r",
		"prop_facgate_05_r_dam_l1",
		"prop_facgate_05_r_l1",
		"prop_facgate_06_l",
		"prop_facgate_06_r",
		"prop_facgate_07",
		"prop_facgate_07b",
		"prop_facgate_08",
		"prop_facgate_08_frame",
		"prop_facgate_08_ld2",
		"prop_facgate_id1_27",
		"prop_fence_log_01",
		"prop_fence_log_02",
		"prop_fncconstruc_02a",
		"prop_fnclog_01a",
		"prop_fnclog_01b",
		"prop_fncpeir_03a",
		"prop_fncres_02a",
		"prop_fncres_02b",
		"prop_fncres_02c",
		"prop_fncres_02d",
		"prop_fncres_02_gate1",
		"prop_fncres_03gate1",
		"prop_fncres_05c_l1",
		"prop_fncsec_01a",
		"prop_fncsec_01b",
		"prop_fncsec_01crnr",
		"prop_fncsec_01gate",
		"prop_fncsec_01pole",
		"prop_fncsec_02a",
		"prop_fncsec_02pole",
		"prop_fncwood_07gate1",
		"prop_fncwood_11a_l1",
		"prop_fncwood_16a",
		"prop_fncwood_16b",
		"prop_fncwood_16c",
		"prop_fncwood_18a",
		"prop_gatecom_02",
		"prop_gate_airport_01",
		"prop_gate_cult_01_l",
		"prop_gate_cult_01_r",
		"prop_gate_docks_ld",
		"prop_gate_farm_01a",
		"prop_gate_farm_post",
		"prop_gate_frame_01",
		"prop_gate_frame_02",
		"prop_gate_frame_04",
		"prop_gate_frame_05",
		"prop_gate_frame_06",
		"prop_gate_military_01",
		"prop_gate_prison_01",
		"prop_gate_tep_01_l",
		"prop_gate_tep_01_r",
		"prop_ld_balcfnc_01a",
		"prop_ld_balcfnc_02a",
		"prop_ld_balcfnc_02c",
		"prop_lrggate_01c_l",
		"prop_lrggate_01c_r",
		"prop_lrggate_01_l",
		"prop_lrggate_01_pst",
		"prop_lrggate_01_r",
		"prop_lrggate_02_ld",
		"prop_lrggate_03a",
		"prop_lrggate_03b",
		"prop_lrggate_03b_ld",
		"prop_lrggate_04a",
		"prop_lrggate_05a",
		"prop_lrggate_06a",
		"prop_fnccorgm_05a",
		"prop_fnccorgm_05b",
		"prop_fnccorgm_06a",
		"prop_fnccorgm_06b",
		"prop_fnclink_01gate1",
		"prop_fnclink_02gate1",
		"prop_fnclink_02gate2",
		"prop_fnclink_02gate5",
		"prop_fnclink_02gate6_l",
		"prop_fnclink_02gate6_r",
		"prop_fnclink_02gate7",
		"prop_fnclink_03gate1",
		"prop_fnclink_03gate2",
		"prop_fnclink_03gate4",
		"prop_fnclink_03gate5",
		"prop_fnclink_04gate1",
		"prop_fnclink_04h_l2",
		"prop_fnclink_06gate2",
		"prop_fnclink_06gate3",
		"prop_fnclink_06gatepost",
		"prop_fnclink_07gate1",
		"prop_fnclink_07gate2",
		"prop_fnclink_07gate3",
		"prop_fnclink_09gate1",
		"prop_fnclink_10a",
		"prop_fnclink_10b",
		"prop_fnclink_10c",
		"prop_fnclink_10d",
		"prop_fnclink_10e",
		"prop_fncsec_04a",
		"prop_gate_bridge_ld",
		"prop_hobo_stove_01",
		"prop_homeless_matress_01",
		"prop_homeless_matress_02",
		"prop_pizza_box_01",
		"prop_pizza_box_02",
		"prop_rub_bike_01",
		"prop_rub_bike_02",
		"prop_rub_bike_03",
		"prop_rub_busdoor_01",
		"prop_rub_busdoor_02",
		"prop_rub_buswreck_01",
		"prop_rub_buswreck_03",
		"prop_rub_buswreck_06",
		"prop_rub_cabinet",
		"prop_rub_cabinet01",
		"prop_rub_cabinet02",
		"prop_rub_cabinet03",
		"prop_rub_cage01a",
		"prop_rub_carpart_02",
		"prop_rub_carpart_03",
		"prop_rub_carpart_04",
		"prop_rub_chassis_01",
		"prop_rub_chassis_02",
		"prop_rub_chassis_03",
		"prop_rub_cont_01a",
		"prop_rub_cont_01b",
		"prop_rub_cont_01c",
		"prop_rub_flotsam_01",
		"prop_rub_flotsam_02",
		"prop_rub_flotsam_03",
		"prop_rub_frklft",
		"prop_rub_litter_01",
		"prop_rub_litter_02",
		"prop_rub_litter_03",
		"prop_rub_litter_03b",
		"prop_rub_litter_03c",
		"prop_rub_litter_04",
		"prop_rub_litter_04b",
		"prop_rub_litter_05",
		"prop_rub_litter_06",
		"prop_rub_litter_07",
		"prop_rub_litter_09",
		"prop_rub_litter_8",
		"prop_rub_matress_01",
		"prop_rub_matress_02",
		"prop_rub_matress_03",
		"prop_rub_matress_04",
		"prop_rub_monitor",
		"prop_rub_pile_01",
		"prop_rub_pile_02",
		"prop_rub_planks_01",
		"prop_rub_planks_02",
		"prop_rub_planks_03",
		"prop_rub_planks_04",
		"prop_rub_railwreck_1",
		"prop_rub_railwreck_2",
		"prop_rub_railwreck_3",
		"prop_rub_scrap_02",
		"prop_rub_scrap_03",
		"prop_rub_scrap_04",
		"prop_rub_scrap_05",
		"prop_rub_scrap_06",
		"prop_rub_scrap_07",
		"prop_rub_stool",
		"prop_rub_sunktyre",
		"prop_rub_t34",
		"prop_rub_trainers_01",
		"prop_rub_trolley01a",
		"prop_rub_trolley02a",
		"prop_rub_trolley03a",
		"prop_rub_trukwreck_1",
		"prop_rub_trukwreck_2",
		"prop_rub_tyre_01",
		"prop_rub_tyre_02",
		"prop_rub_tyre_03",
		"prop_rub_tyre_dam1",
		"prop_rub_tyre_dam2",
		"prop_rub_tyre_dam3",
		"prop_rub_washer_01",
		"prop_rub_wheel_01",
		"prop_rub_wheel_02",
		"prop_rub_wreckage_3",
		"prop_rub_wreckage_4",
		"prop_rub_wreckage_5",
		"prop_rub_wreckage_6",
		"prop_rub_wreckage_7",
		"prop_rub_wreckage_8",
		"prop_rub_wreckage_9",
		"prop_skid_chair_01",
		"prop_skid_chair_02",
		"prop_skid_chair_03",
		"prop_skid_sleepbag_1",
		"prop_skid_tent_01",
		"prop_skid_tent_01b",
		"prop_skid_tent_03",
		"prop_still",
		"prop_bench_01b",
		"prop_bench_01c",
		"prop_bench_04",
		"prop_bench_05",
		"prop_bench_09",
		"prop_chair_01a",
		"prop_chair_01b",
		"prop_chair_02",
		"prop_chair_03",
		"prop_chair_04a",
		"prop_chair_04b",
		"prop_chair_05",
		"prop_chair_06",
		"prop_chair_07",
		"prop_chair_08",
		"prop_chair_09",
		"prop_chair_10",
		"prop_chair_pile_01",
		"prop_chateau_chair_01",
		"prop_clown_chair",
		"prop_old_deck_chair",
		"prop_old_deck_chair_02",
		"prop_old_wood_chair",
		"prop_old_wood_chair_lod",
		"prop_parasol_01",
		"prop_parasol_01_b",
		"prop_parasol_01_c",
		"prop_parasol_01_down",
		"prop_parasol_02",
		"prop_parasol_02_b",
		"prop_parasol_02_c",
		"prop_parasol_03",
		"prop_parasol_03_b",
		"prop_parasol_03_c",
		"prop_parasol_04e",
		"prop_parasol_04e_lod1",
		"prop_parasol_bh_48",
		"prop_patio_heater_01",
		"prop_patio_lounger1",
		"prop_patio_lounger1b",
		"prop_patio_lounger1_table",
		"prop_patio_lounger_2",
		"prop_patio_lounger_3",
		"prop_picnictable_02",
		"prop_proxy_chateau_table",
		"prop_stool_01",
		"prop_table_02",
		"prop_table_03b_cs",
		"prop_table_04",
		"prop_table_04_chr",
		"prop_table_05",
		"prop_table_05_chr",
		"prop_table_06",
		"prop_table_06_chr",
		"prop_table_07",
		"prop_table_07_l1",
		"prop_table_08",
		"prop_table_08_chr",
		"prop_table_08_side",
		"prop_table_para_comb_04",
		"prop_umpire_01",
		"prop_afsign_amun",
		"prop_afsign_vbike",
		"prop_beer_neon_01",
		"prop_beer_neon_02",
		"prop_beer_neon_03",
		"prop_beer_neon_04",
		"prop_billboard_01",
		"prop_billboard_02",
		"prop_billboard_03",
		"prop_billboard_04",
		"prop_billboard_05",
		"prop_billboard_06",
		"prop_billboard_07",
		"prop_billboard_08",
		"prop_billboard_09",
		"prop_billboard_09wall",
		"prop_billboard_10",
		"prop_billboard_11",
		"prop_billboard_12",
		"prop_billboard_13",
		"prop_billboard_14",
		"prop_billboard_15",
		"prop_billboard_16",
		"prop_gas_01",
		"prop_gas_02",
		"prop_gas_03",
		"prop_gas_04",
		"prop_gas_05",
		"prop_pharm_sign_01",
		"prop_sign_airp_01a",
		"prop_sign_airp_02a",
		"prop_sign_airp_02b",
		"prop_sign_big_01",
		"prop_sign_road_04g_l1",
		"prop_barrel_01a",
		"prop_barrel_02a",
		"prop_barrel_02b",
		"prop_barrel_03a",
		"prop_barrel_03d",
		"prop_boxpile_10a",
		"prop_boxpile_10b",
		"prop_box_ammo01a",
		"prop_box_ammo02a",
		"prop_box_ammo03a_set",
		"prop_box_ammo03a_set2",
		"prop_box_ammo04a",
		"prop_box_ammo05b",
		"prop_box_ammo07a",
		"prop_box_ammo07b",
		"prop_box_guncase_01a",
		"prop_box_guncase_02a",
		"prop_box_guncase_03a",
		"prop_box_tea01a",
		"prop_box_wood05a",
		"prop_box_wood05b",
		"prop_box_wood08a",
		"prop_buckets_02",
		"prop_bucket_01a",
		"prop_bucket_01b",
		"prop_bucket_02a",
		"prop_cntrdoor_ld_l",
		"prop_cntrdoor_ld_r",
		"prop_container_01a",
		"prop_container_01b",
		"prop_container_01c",
		"prop_container_01d",
		"prop_container_01e",
		"prop_container_01f",
		"prop_container_01g",
		"prop_container_01h",
		"prop_container_01mb",
		"prop_container_02a",
		"prop_container_03a",
		"prop_container_03b",
		"prop_container_03mb",
		"prop_container_03_ld",
		"prop_container_04a",
		"prop_container_04mb",
		"prop_container_05mb",
		"prop_container_door_mb_l",
		"prop_container_door_mb_r",
		"prop_container_ld",
		"prop_container_ld2",
		"prop_container_old1",
		"prop_contnr_pile_01a",
		"prop_cratepile_07a_l1",
		"prop_crate_01a",
		"prop_crate_02a",
		"prop_crate_08a",
		"prop_crate_09a",
		"prop_crate_10a",
		"prop_crate_11a",
		"prop_crate_11b",
		"prop_crate_11c",
		"prop_crate_11d",
		"prop_dog_cage_01",
		"prop_dog_cage_02",
		"prop_drop_crate_01",
		"prop_drop_crate_01_set",
		"prop_drop_crate_01_set2",
		"prop_flattrailer_01a",
		"prop_flattruck_01a",
		"prop_fruitstand_b_nite",
		"prop_gascage01",
		"prop_keg_01",
		"prop_mb_crate_01a_set",
		"prop_pallettruck_01",
		"prop_sacktruck_01",
		"prop_sacktruck_02a",
		"prop_shelves_01",
		"prop_shelves_02",
		"prop_shelves_03",
		"prop_truktrailer_01a",
		"prop_warehseshelf01",
		"prop_warehseshelf02",
		"prop_warehseshelf03",
		"prop_watercrate_01",
		"prop_chall_lamp_01",
		"prop_chall_lamp_01n",
		"prop_chall_lamp_02",
		"prop_construcionlamp_01",
		"prop_dt1_13_groundlight",
		"prop_dt1_13_walllightsource",
		"prop_ind_light_01a",
		"prop_ind_light_01b",
		"prop_ind_light_01c",
		"prop_securityvan_lightrig",
		"prop_traffic_rail_1c",
		"prop_traffic_rail_2",
		"prop_walllight_ld_01b",
		"prop_wall_light_08a",
		"prop_wall_light_10a",
		"prop_wall_light_10b",
		"prop_wall_light_10c",
		"prop_wall_light_11",
		"prop_wall_light_12",
		"prop_wall_light_17b",
		"prop_wall_light_18a",
		"prop_warninglight_01",
		"prop_atm_02",
		"prop_atm_03",
		"prop_bikerack_2",
		"prop_bollard_01a",
		"prop_bollard_01b",
		"prop_bollard_01c",
		"prop_bollard_03a",
		"prop_elecbox_03a",
		"prop_elecbox_10",
		"prop_elecbox_12",
		"prop_elecbox_13",
		"prop_elecbox_14",
		"prop_elecbox_15",
		"prop_elecbox_16",
		"prop_elecbox_17",
		"prop_elecbox_18",
		"prop_elecbox_19",
		"prop_elecbox_20",
		"prop_elecbox_21",
		"prop_elecbox_22",
		"prop_elecbox_23",
		"prop_elecbox_24",
		"prop_elecbox_24b",
		"prop_elecbox_25",
		"prop_fire_driser_1a",
		"prop_fire_driser_1b",
		"prop_fire_driser_2b",
		"prop_fire_driser_3b",
		"prop_fire_driser_4a",
		"prop_fire_driser_4b",
		"prop_fire_hosereel",
		"prop_fleeca_atm",
		"prop_gshotsensor_01",
		"prop_mobile_mast_1",
		"prop_mobile_mast_2",
		"prop_parking_sign_06",
		"prop_parking_sign_07",
		"prop_parking_sign_1",
		"prop_parking_sign_2",
		"prop_phonebox_05a",
		"prop_telegraph_01a",
		"prop_telegraph_01b",
		"prop_telegraph_01c",
		"prop_telegraph_01d",
		"prop_telegraph_01e",
		"prop_telegraph_01f",
		"prop_telegraph_01g",
		"prop_telegraph_02a",
		"prop_telegraph_02b",
		"prop_telegraph_03",
		"prop_telegraph_04a",
		"prop_telegraph_04b",
		"prop_telegraph_05a",
		"prop_telegraph_05b",
		"prop_telegraph_05c",
		"prop_telegraph_06a",
		"prop_telegraph_06b",
		"prop_telegraph_06c",
		"prop_telegwall_01a",
		"prop_telegwall_01b",
		"prop_telegwall_02a",
		"prop_telegwall_03a",
		"prop_telegwall_03b",
		"prop_telegwall_04a",
		"prop_tram_pole_double01",
		"prop_tram_pole_double02",
		"prop_tram_pole_double03",
		"prop_tram_pole_roadside",
		"prop_tram_pole_single01",
		"prop_tram_pole_single02",
		"prop_tram_pole_wide01",
		"prop_tyre_rack_01",
		"prop_valet_03",
		"prop_cartwheel_01",
		"prop_cattlecrush",
		"prop_chickencoop_a",
		"prop_feeder1",
		"prop_grain_hopper",
		"prop_handrake",
		"prop_haybailer_01",
		"prop_haybale_01",
		"prop_haybale_02",
		"prop_haybale_stack_01",
		"prop_hunterhide",
		"prop_oldplough1",
		"prop_old_churn_01",
		"prop_old_churn_02",
		"prop_old_farm_01",
		"prop_old_farm_02",
		"prop_potatodigger",
		"prop_roundbailer01",
		"prop_roundbailer02",
		"prop_rural_windmill_l1",
		"prop_rural_windmill_l2",
		"prop_scythemower",
		"prop_side_spreader",
		"prop_snow_bailer_01",
		"prop_snow_barrel_pile_03",
		"prop_snow_bench_01",
		"prop_snow_bin_01",
		"prop_snow_bin_02",
		"prop_snow_cam_03",
		"prop_snow_cam_03a",
		"prop_snow_diggerbkt_01",
		"prop_snow_dumpster_01",
		"prop_snow_elecbox_16",
		"prop_snow_facgate_01",
		"prop_snow_flower_01",
		"prop_snow_flower_02",
		"prop_snow_fnclink_03crnr2",
		"prop_snow_fnclink_03h",
		"prop_snow_fnclink_03i",
		"prop_snow_fncwood_14a",
		"prop_snow_fncwood_14b",
		"prop_snow_fncwood_14c",
		"prop_snow_fncwood_14d",
		"prop_snow_fncwood_14e",
		"prop_snow_fnc_01",
		"prop_snow_gate_farm_03",
		"prop_snow_grain_01",
		"prop_snow_light_01",
		"prop_snow_oldlight_01b",
		"prop_snow_rail_signals02",
		"prop_snow_rub_trukwreck_2",
		"prop_snow_side_spreader_01",
		"prop_snow_streetlight01",
		"prop_snow_streetlight_01_frag_",
		"prop_snow_sub_frame_01a",
		"prop_snow_sub_frame_04b",
		"prop_snow_telegraph_01a",
		"prop_snow_telegraph_02a",
		"prop_snow_telegraph_03",
		"prop_snow_traffic_rail_1a",
		"prop_snow_traffic_rail_1b",
		"prop_snow_trailer01",
		"prop_snow_truktrailer_01a",
		"prop_snow_tyre_01",
		"prop_snow_wall_light_15a",
		"prop_snow_watertower01",
		"prop_snow_watertower01_l2",
		"prop_snow_watertower03",
		"prop_snow_woodpile_04a",
		"prop_sprayer",
		"prop_trailer01",
		"prop_trailer01_up",
		"prop_trough1",
		"prop_waterwheela",
		"prop_waterwheelb",
		"prop_wreckedcart",
		"prop_am_box_wood_01",
		"prop_bush_ivy_01_1m",
		"prop_bush_ivy_01_2m",
		"prop_bush_ivy_01_bk",
		"prop_bush_ivy_01_l",
		"prop_bush_ivy_01_pot",
		"prop_bush_ivy_01_r",
		"prop_bush_ivy_01_top",
		"prop_bush_ivy_02_1m",
		"prop_bush_ivy_02_2m",
		"prop_bush_ivy_02_l",
		"prop_bush_ivy_02_pot",
		"prop_bush_ivy_02_r",
		"prop_bush_ivy_02_top",
		"prop_bush_lrg_01",
		"prop_bush_lrg_01b",
		"prop_bush_lrg_01c",
		"prop_bush_lrg_01d",
		"prop_bush_lrg_01e",
		"prop_bush_lrg_02",
		"prop_bush_lrg_02b",
		"prop_bush_lrg_03",
		"prop_bush_lrg_04b",
		"prop_bush_lrg_04c",
		"prop_bush_lrg_04d",
		"prop_bush_med_01",
		"prop_bush_med_02",
		"prop_bush_med_03",
		"prop_bush_med_05",
		"prop_bush_med_06",
		"prop_bush_med_07",
		"prop_bush_neat_01",
		"prop_bush_neat_02",
		"prop_bush_neat_03",
		"prop_bush_neat_04",
		"prop_bush_neat_05",
		"prop_bush_neat_06",
		"prop_bush_neat_07",
		"prop_bush_neat_08",
		"prop_bush_ornament_01",
		"prop_bush_ornament_02",
		"prop_bush_ornament_03",
		"prop_bush_ornament_04",
		"prop_creosote_b_01",
		"prop_desert_iron_01",
		"prop_plant_group_01",
		"prop_plant_group_02",
		"prop_plant_group_03",
		"prop_plant_group_04",
		"prop_plant_group_05",
		"prop_plant_group_05b",
		"prop_plant_group_05c",
		"prop_plant_group_05d",
		"prop_plant_group_06a",
		"prop_plant_group_06b",
		"prop_plant_group_06c",
		"prop_skunk_bush_01",
		"prop_bush_grape_01",
		"prop_coral_bush_01",
		"prop_coral_flat_01",
		"prop_coral_flat_01_l1",
		"prop_coral_flat_02",
		"prop_coral_flat_brainy",
		"prop_coral_flat_clam",
		"prop_coral_grass_01",
		"prop_coral_grass_02",
		"prop_coral_kelp_01",
		"prop_coral_kelp_01_l1",
		"prop_coral_kelp_02",
		"prop_coral_kelp_02_l1",
		"prop_coral_kelp_03",
		"prop_coral_kelp_03a",
		"prop_coral_kelp_03b",
		"prop_coral_kelp_03c",
		"prop_coral_kelp_03d",
		"prop_coral_kelp_03_l1",
		"prop_coral_kelp_04",
		"prop_coral_kelp_04_l1",
		"prop_coral_pillar_01",
		"prop_coral_pillar_02",
		"prop_coral_spikey_01",
		"prop_coral_stone_03",
		"prop_coral_stone_04",
		"prop_coral_sweed_01",
		"prop_coral_sweed_02",
		"prop_coral_sweed_03",
		"prop_coral_sweed_04",
		"prop_cora_clam_01",
		"prop_tree_birch_05",
		"prop_veg_corn_01",
		"prop_veg_crop_01",
		"prop_veg_crop_02",
		"prop_veg_crop_04",
		"prop_veg_crop_04_leaf",
		"prop_veg_crop_05",
		"prop_veg_crop_06",
		"prop_veg_crop_orange",
		"prop_veg_crop_tr_01",
		"prop_veg_crop_tr_02",
		"prop_agave_01",
		"prop_agave_02",
		"prop_aloevera_01",
		"prop_bush_dead_02",
		"prop_cat_tail_01",
		"prop_cs_plant_01",
		"prop_grass_dry_02",
		"prop_grass_dry_03",
		"prop_plant_01a",
		"prop_plant_01b",
		"prop_plant_base_01",
		"prop_plant_base_02",
		"prop_plant_base_03",
		"prop_plant_cane_01a",
		"prop_plant_cane_01b",
		"prop_plant_cane_02a",
		"prop_plant_cane_02b",
		"prop_plant_clover_01",
		"prop_plant_clover_02",
		"prop_plant_fern_01a",
		"prop_plant_fern_01b",
		"prop_plant_fern_02a",
		"prop_plant_fern_02b",
		"prop_plant_fern_02c",
		"prop_plant_flower_01",
		"prop_plant_flower_02",
		"prop_plant_flower_03",
		"prop_plant_flower_04",
		"prop_plant_paradise",
		"prop_plant_paradise_b",
		"prop_p_spider_01a",
		"prop_p_spider_01c",
		"prop_p_spider_01d",
		"prop_veg_grass_01_a",
		"prop_veg_grass_01_b",
		"prop_veg_grass_01_c",
		"prop_veg_grass_01_d",
		"prop_veg_grass_02_a",
		"prop_fan_palm_01a",
		"prop_palm_fan_02_a",
		"prop_palm_fan_02_b",
		"prop_palm_fan_03_a",
		"prop_palm_fan_03_b",
		"prop_palm_fan_03_c",
		"prop_palm_fan_03_d",
		"prop_palm_fan_04_a",
		"prop_palm_fan_04_b",
		"prop_palm_fan_04_c",
		"prop_palm_fan_04_d",
		"prop_palm_huge_01a",
		"prop_palm_huge_01b",
		"prop_palm_med_01a",
		"prop_palm_med_01b",
		"prop_palm_med_01c",
		"prop_palm_med_01d",
		"prop_palm_sm_01a",
		"prop_palm_sm_01d",
		"prop_palm_sm_01e",
		"prop_palm_sm_01f",
		"prop_plant_int_02a",
		"prop_plant_int_02b",
		"prop_plant_int_05a",
		"prop_plant_int_05b",
		"prop_plant_int_06a",
		"prop_plant_int_06b",
		"prop_plant_int_06c",
		"prop_pot_plant_02a",
		"prop_pot_plant_02b",
		"prop_pot_plant_02c",
		"prop_pot_plant_02d",
		"prop_pot_plant_03a",
		"prop_pot_plant_04a",
		"prop_pot_plant_05d_l1",
		"prop_pot_plant_bh1",
		"prop_rock_1_a",
		"prop_rock_1_b",
		"prop_rock_1_c",
		"prop_rock_1_d",
		"prop_rock_1_e",
		"prop_rock_1_f",
		"prop_rock_1_g",
		"prop_rock_1_h",
		"prop_rock_1_i",
		"prop_rock_2_a",
		"prop_rock_2_c",
		"prop_rock_2_d",
		"prop_rock_2_f",
		"prop_rock_2_g",
		"prop_rock_3_a",
		"prop_rock_3_b",
		"prop_rock_3_c",
		"prop_rock_3_d",
		"prop_rock_3_e",
		"prop_rock_3_f",
		"prop_rock_3_g",
		"prop_rock_3_h",
		"prop_rock_3_i",
		"prop_rock_3_j",
		"prop_rock_4_c",
		"prop_rock_4_d",
		"rock_4_cl_2_1",
		"rock_4_cl_2_2",
		"prop_snow_bush_01_a",
		"prop_snow_bush_02_a",
		"prop_snow_bush_02_b",
		"prop_snow_bush_03",
		"prop_snow_bush_04",
		"prop_snow_bush_04b",
		"prop_snow_field_01",
		"prop_snow_field_02",
		"prop_snow_field_03",
		"prop_snow_field_04",
		"prop_snow_grass_01",
		"prop_snow_tree_03_e",
		"prop_snow_tree_03_h",
		"prop_snow_tree_03_i",
		"prop_snow_tree_04_d",
		"prop_snow_tree_04_f",
		"prop_snow_t_ml_01",
		"prop_snow_t_ml_02",
		"prop_snow_t_ml_03",
		"prop_rio_del_01",
		"prop_rus_olive",
		"prop_rus_olive_wint",
		"prop_s_pine_dead_01",
		"prop_tree_birch_01",
		"prop_tree_birch_02",
		"prop_tree_birch_03",
		"prop_tree_birch_03b",
		"prop_tree_birch_04",
		"prop_tree_cedar_02",
		"prop_tree_cedar_03",
		"prop_tree_cedar_04",
		"prop_tree_cedar_s_01",
		"prop_tree_cedar_s_02",
		"prop_tree_cedar_s_04",
		"prop_tree_cedar_s_05",
		"prop_tree_cedar_s_06",
		"prop_tree_cypress_01",
		"prop_tree_eng_oak_01",
		"prop_tree_eucalip_01",
		"prop_tree_fallen_01",
		"prop_tree_fallen_02",
		"prop_tree_fallen_pine_01",
		"prop_tree_jacada_01",
		"prop_tree_jacada_02",
		"prop_tree_lficus_02",
		"prop_tree_lficus_03",
		"prop_tree_lficus_05",
		"prop_tree_lficus_06",
		"prop_tree_log_01",
		"prop_tree_log_02",
		"prop_tree_maple_02",
		"prop_tree_maple_03",
		"prop_tree_mquite_01",
		"prop_tree_oak_01",
		"prop_tree_olive_01",
		"prop_tree_pine_01",
		"prop_tree_pine_02",
		"prop_tree_stump_01",
		"prop_w_r_cedar_01",
		"prop_w_r_cedar_dead",
		"w_am_baseball",
		"w_am_brfcase",
		"w_am_case",
		"w_am_digiscanner",
		"w_am_fire_exting",
		"w_am_flare",
		"w_am_jerrycan",
		"w_ar_advancedrifle",
		"w_ar_advancedrifle_mag1",
		"w_ar_advancedrifle_mag2",
		"w_ar_assaultrifle",
		"w_ar_assaultrifle_mag1",
		"w_ar_assaultrifle_mag2",
		"w_ar_carbinerifle",
		"w_ar_carbinerifle_mag1",
		"w_ar_carbinerifle_mag2",
		"w_at_ar_afgrip",
		"w_at_ar_flsh",
		"w_at_ar_supp",
		"w_at_ar_supp_02",
		"w_at_pi_flsh",
		"w_at_pi_supp",
		"w_at_railcover_01",
		"w_at_scope_large",
		"w_at_scope_macro",
		"w_at_scope_max",
		"w_at_scope_medium",
		"w_at_scope_small",
		"w_at_sr_supp",
		"w_ex_grenadefrag",
		"w_ex_grenadesmoke",
		"w_ex_molotov",
		"w_ex_pe",
		"w_lr_40mm",
		"w_lr_grenadelauncher",
		"w_lr_rpg",
		"w_lr_rpg_rocket",
		"w_me_bat",
		"w_me_crowbar",
		"w_me_gclub",
		"w_me_hammer",
		"w_me_knife_01",
		"w_me_nightstick",
		"w_mg_combatmg",
		"w_mg_combatmg_mag1",
		"w_mg_combatmg_mag2",
		"w_mg_mg",
		"w_mg_mg_mag1",
		"w_mg_mg_mag2",
		"w_mg_minigun",
		"w_pi_appistol",
		"w_pi_appistol_mag1",
		"w_pi_appistol_mag2",
		"w_pi_combatpistol",
		"w_pi_combatpistol_mag1",
		"w_pi_combatpistol_mag2",
		"w_pi_pistol",
		"w_pi_pistol50",
		"w_pi_pistol50_mag1",
		"w_pi_pistol50_mag2",
		"w_pi_pistol_mag1",
		"w_pi_pistol_mag2",
		"w_pi_stungun",
		"w_sb_assaultsmg",
		"w_sb_assaultsmg_mag1",
		"w_sb_assaultsmg_mag2",
		"w_sb_microsmg",
		"w_sb_microsmg_mag1",
		"w_sb_microsmg_mag2",
		"w_sb_smg",
		"w_sb_smg_mag1",
		"w_sb_smg_mag2",
		"w_sg_assaultshotgun",
		"w_sg_assaultshotgun_mag1",
		"w_sg_assaultshotgun_mag2",
		"w_sg_bullpupshotgun",
		"w_sg_pumpshotgun",
		"w_sg_sawnoff",
		"w_sr_heavysniper",
		"w_sr_heavysniper_mag1",
		"w_sr_sniperrifle",
		"w_sr_sniperrifle_mag1",
	};




	void FullObjectsMenu() {
		AddTitle("Full Object List");
		for (int i = 0; i < ARRAYSIZE(propList); i++) {
			AddObject(propList[i]);
		}
	}

	void SpawnedObjectsDatabase() {
		AddTitle("Manage Spawned Objects");
		for (Object n : objectsDB) {
			if (ENTITY::DOES_ENTITY_EXIST(n) && n != NULL) {
				AddObjectDBElement("VALID OBJECT", n, null, nullFunc, SUB::OBJECT_EDITOR);
			}
			else {
				objectsDB.remove(n);
			}
		}
	}









	void ObjectsMenu()
	{
		AddTitle("OBJECTS");
		AddOption("Spawn Object", null, nullFunc, SUB::SELECT_OBJECT_CATEGORY);
		AddOption("Edit Latest Object", null, nullFunc, SUB::OBJECT_EDITOR);
		AddOption("Manage Spawned Objects", null, nullFunc, SUB::SPAWNED_OBJECTS_DATABASE);
		AddOption("Clear Area", null, nullFunc, SUB::CLEAR_AREA);
	}


	void WorldMenu()
	{
		bool hour_plus = 0, hour_minus = 0, timescale_plus = 0, timescale_minus = 0;

		int sample_hour = TIME::GET_CLOCK_HOURS();
		AddTitle("WORLD");
		AddOption("Map Mods", null, nullFunc, SUB::MAPMOD);
		AddOption("IPL", null, nullFunc, SUB::IPL);
		AddOption("Weather", null, nullFunc, SUB::WEATHER);
		AddNumber("Hour", TIME::GET_CLOCK_HOURS(), 0, null, hour_plus, hour_minus);
		AddNumber("Time Scale", current_timescale, 2, null, timescale_plus, timescale_minus);



		if (hour_plus) {
			if (sample_hour + 1 == 24) NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(0, 0, 0);
			else NETWORK::NETWORK_OVERRIDE_CLOCK_TIME((sample_hour + 1), 0, 0);
			return;
		}
		else if (hour_minus) {
			if ((sample_hour - 1) == -1) NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(23, 0, 0);
			else NETWORK::NETWORK_OVERRIDE_CLOCK_TIME((sample_hour - 1), 0, 0);
			return;
		}

		if (timescale_plus) {
			if (current_timescale < 2.0f) current_timescale += 0.05f;
			GAMEPLAY::SET_TIME_SCALE(current_timescale);
			return;
		}
		else if (timescale_minus) {
			if (current_timescale > 0.0f) current_timescale -= 0.05f;
			GAMEPLAY::SET_TIME_SCALE(current_timescale);
			return;
		}
	}

	void TeleportIntoClosestVehicle() {
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		int closestVehicle = VEHICLE::GET_CLOSEST_VEHICLE(playerPosition.x, playerPosition.y, playerPosition.z, 100.0, 0, 71);
		PED::SET_PED_INTO_VEHICLE(playerPed, closestVehicle, -1);
	}

	void TeleportMenu()
	{
		AddTitle("TELEPORT");
		AddOption("Teleport To Waypoint", null, teleport_to_marker);
		AddOption("Teleport to Mission Objective", null, teleport_to_objective);
		AddOption("Teleport into Closest Vehicle", null, TeleportIntoClosestVehicle);
		AddBreak("Cool Locations");
		AddTele("High in the Sky!!", -129.9f, 8130.8f, 9000.0f);
		AddTele("Mount Chilliad", 496.0635f, 5584.5142f, 793.9454f);
		AddTele("Mine Shaft", -596.93, 2094.12f, 132.00f);
		AddTele("Military Base", -2138.234, 3250.8606f, 34.00f);
		AddTele("Airport", -1135.1100, -2885.2030f, 15.00f);
		AddTele("Trevers Airfield", 1590.6788, 3267.6698f, 43.0000f);
		AddTele("Out To Sea", 1845.673, -13787.4884f, 0.0000f);
		AddTele("Island", -2159.147, 5196.89f, 20.00f);
		AddTele("Under Water UFO", 760.461, 7392.8032f, -110.0774f);
		AddTele("Under Water Plane Crash", 1846, -2946.855f, -33.32f);
		AddTele("Water Fall", -552.0047, 4439.4487f, 35.123f);
		AddTele("Humane Labs Level 1", 3617.3726, 3738.2727f, 30.6901f);
		AddTele("Humane Labs Level 2", 3525.6133, 3709.2998f, 22.9918f);
		AddTele("Inside FIB Building", 136.3807, -749.0196f, 258.1517f);
		AddTele("Inside Fire FIB", 137.8378, -747.39f, 253.152f);
		AddTele("Inside FIB Lift", 133.1019, -735.7224f, 235.63f);
		AddTele("Inside IAA Building", 127.49, -618.26f, 207.04f);
		AddTele("Maze Bank ", -75.5003, -819.0528f, 327.00f);
		AddTele("Del Perro Pier", -1838.834, -1223.333f, 15.00f);
		AddTele("Vinewood Sign", 729.909, 1204.37f, 326.0209f);
		AddTele("Beeker's Garage", 139.67, 6595.94f, 33.00f);
		AddTele("400k/500k Apartment", -793.36, 296.86f, 87.84f);
		AddTele("Appartment", -778.34, 339.97f, 208.62f);
		AddTele("Mors Mutual Insurance", -232.74, -1163.556f, 24.95f);
		AddTele("Impound Lot", 408.923, -1633.860f, 30.29f);
		AddTele("Inside 10 Car Garage", 228.71, -989.98f, -96.00f);
		AddTele("Under The Map", 132.1470, -739.5430f, 39.00f);
		AddTele("Strip Club", 125.428, -1290.40f, 30.00f);
		AddTele("Prison", 1696.3642, 2561.377f, 47.56f);
		AddTele("Maze", -2311.01, 234.33f, 170.63f);
		AddTele("Ammunation", 233.3912, -41.08f, 69.67f);
		AddTele("Race Track", 1201.36, 95.65f, 82.03f);
		AddTele("Parachute Jump", -521.35, 4422.00f, 89.00f);
		AddTele("Michael's House", -827.13, 175.47f, 70.82f);
		AddTele("Michael's House Inside", -814.38, 178.92f, 73.00f);
		AddTele("Franklins's House Old", -14.31, -1437.00f, 30.00f);
		AddTele("Franklins's House New", 7.05, 524.33f, 174.97f);
		AddTele("Helicopter Pad", -741.54, -1456.00f, 3.00f);
		AddTele("LS Customs", -363.9027, -132.71f, 39.00f);
		AddTele("Random", 2861.426, 5927.89, 361.29f);
		AddTele("Trevers Meth Lab", 1390.28, 3608.60f, 39.00f);
		AddTele("Dam 1", 1663.123, 24.18f, 169.00f);
		AddTele("Dam 2", 115.28, 785.81f, 212.00f);
		AddTele("Cave", -1911.3, 1389.29f, 219.00f);
		AddTele("Farest Island North", 32.51, 7688.99f, 4.00f);
		AddTele("Farest Island South", 1799.90, -2823.90, 5.00f);
		AddTele("Flight School", -1153.10, -2713.39f, 20.00f);
		AddTele("Tram Station", 104.50, -1718.30f, 31.00f);
		AddTele("Golf", -1079.71, 10.04f, 51.00f);
		AddTele("Stage", 684.97, 574.32f, 131.00f);
		AddTele("Drift Mountain", 860.32, 1316.65f, 356.00f);
		AddTele("Consturction Building", -161.26, -937.87f, 268.52f);
	}

	void ESPMenu() {
		bool redC = 0, blueC = 0, greenC = 0, oC = 0, showNamesC = 0, nameSizeC = 0;
		AddTitle("ESP");
		AddToggle("Enabled", esp);
		AddBoxType("Box Type");
		AddToggle("Show Names", showESPNames, showNamesC, showNamesC);
	//	AddFloatEasy("Esp Name Size", ESPNameSize, &ESPNameSize, 1, false, null, true, 99999, 0);
		AddIntEasy("Red", boxColor.R, boxColor.R, 1, 1, redC);
		AddIntEasy("Blue", boxColor.G, boxColor.G, 1, 1, blueC);
		AddIntEasy("Green", boxColor.B, boxColor.B, 1, 1, greenC);
		AddIntEasy("Opacity", boxColor.A, boxColor.A, 1, 1, oC);
		std::string esp;
		if (showESPNames) esp = "true"; else esp = "false";
		if (boxColor.R > 255) boxColor.R = 0; else if (boxColor.R < 0) boxColor.R = 255;
		if (boxColor.G > 255) boxColor.G = 0; else if (boxColor.G < 0) boxColor.G = 255;
		if (boxColor.B > 255) boxColor.B = 0; else if (boxColor.B < 0) boxColor.B = 255;
		if (boxColor.A > 255) boxColor.A = 0; else if (boxColor.A < 0) boxColor.A = 255;
	}

	Object mapMods[250];
	int mapModsIndex = 0;

	void PlaceObjectByHash(Hash hash, float x, float y, float z, float pitch, float roll, float yaw, float heading, int id) {
		if (STREAMING::IS_MODEL_IN_CDIMAGE(hash)) {
			STREAMING::REQUEST_MODEL(hash);
			while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);
			Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(hash, x, y, z, 1, 0, 0);
			ENTITY::SET_ENTITY_LOD_DIST(obj, 696969);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(obj);
			ENTITY::FREEZE_ENTITY_POSITION(obj, 1);
			ENTITY::SET_ENTITY_ROTATION(obj, pitch, roll, yaw, 0, 1);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(obj, 1, 1);
			mapMods[mapModsIndex] = obj;
			if (mapModsIndex <= 250) mapModsIndex++;
			WAIT(5);
		}
	}

	void MapModsMenu() {
		AddTitle("Map Mods");
		AddOption("Maze Bank Demolition", null, nullFunc, SUB::MAPMOD_MAZEDEMO);
		AddOption("Maze Bank Roof Ramp", null, nullFunc, SUB::MAPMOD_MAZEROOFRAMP);
		AddOption("Beach Ferris-Ramp", null, nullFunc, SUB::MAPMOD_BEACHFERRISRAMP);
		AddOption("Mount Chilliad Ramp", null, nullFunc, SUB::MAPMOD_MOUNTCHILLIADRAMP);
		AddOption("Airport Mini Ramp", null, nullFunc, SUB::MAPMOD_AIRPORTMINIRAMP);
		AddOption("Airport Gate Ramp", null, nullFunc, SUB::MAPMOD_AIRPORTGATERAMP);
		AddOption("UFO Tower with FIB Building", null, nullFunc, SUB::MAPMOD_UFOTOWER);
		AddOption("Maze Bank 4 Ramps", null, nullFunc, SUB::MAPMOD_MAZEBANKRAMPS);
		AddOption("Freestyle Motocross Fort Zancudo", null, nullFunc, SUB::MAPMOD_FREESTYLEMOTOCROSS);
		AddOption("Halfpipe Fun Track", null, nullFunc, SUB::MAPMOD_HALFPIPEFUNTRACK);
		AddOption("Airport Loop", null, nullFunc, SUB::MAPMOD_AIRPORTLOOP);
		AddOption("Maze Bank Ramp", null, nullFunc, SUB::MAPMOD_MAZEBANKMEGARAMP);
	}


	void MapMod(int mapIndex = -1) {

		if (mapIndex == -1) {
			bool unload = 0;
			AddTitle("Map Mods");
			AddOption("~r~UNLOAD ALL", unload);
			if (unload) {
				for (int i = 0; i < 250; i++) {
					RequestControlOfEnt(mapMods[i]);
					ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&mapMods[i]);
					ENTITY::DELETE_ENTITY(&mapMods[i]);
					PrintStringBottomCentre("~b~Cleared !");
				}
			}
		}
		else if (mapIndex == 0) {



			bool load = 0, unload = 0, teleport = 0;

			AddTitle("~b~Maze Bank Demolition");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-74.94243f, -818.63446f, 326.174347f);
			}
			if (load) {
				PlaceObjectByHash(2475986526, -80.9087, -830.357, 325.097, -14.4264, -8.53774, 152.209, -1, 0);
				PlaceObjectByHash(2475986526, -79.2316, -831.297, 325.108, -14.4054, 4.26887, 164.271, -1, 0);
				PlaceObjectByHash(2475986526, -76.7563, -831.549, 325.123, -14.4263, -0, 171.818, -1, 0);
				PlaceObjectByHash(2475986526, -74.2595, -831.691, 325.09, -14.2059, -1.06722, -176.517, -1, 0);
				PlaceObjectByHash(2475986526, -71.9478, -831.257, 325.113, -14.4264, 2.13443, -166.006, -1, 0);
				PlaceObjectByHash(2475986526, -69.5981, -830.542, 325.066, -14.5918, -4.26887, -155.474, -1, 0);
				PlaceObjectByHash(2475986526, -67.4174, -829.035, 325.097, -14.4263, -4.26887, -140.651, -1, 0);
				PlaceObjectByHash(2475986526, -65.7121, -827.409, 325.068, -14.3048, -1.70755, -130.667, -1, 0);
				PlaceObjectByHash(2475986526, -64.2717, -825.422, 325.112, -14.4263, -0, -121.256, -1, 0);
				PlaceObjectByHash(2475986526, -63.2876, -823.434, 325.114, -14.5345, -0, -111.446, -1, 0);
				PlaceObjectByHash(2475986526, -62.4924, -821.128, 325.082, -14.5467, 2.13443, -102.385, -1, 0);
				PlaceObjectByHash(2475986526, -62.233, -818.528, 325.065, -14.6184, -8.00413, -88.1861, -1, 0);
				PlaceObjectByHash(2475986526, -62.8694, -815.926, 325.118, -14.4265, -6.4033, -76.5979, -1, 0);
				PlaceObjectByHash(2475986526, -63.6142, -813.68, 325.112, -14.3655, 8.53774, -66.7885, -1, 0);
				PlaceObjectByHash(2475986526, -64.9883, -811.439, 325.107, -14.4263, 1.28066, -52.8494, -1, 0);
				PlaceObjectByHash(2475986526, -66.5913, -808.328, 325.238, -12.4517, -0, -40.3246, -1, 0);
				PlaceObjectByHash(2475986526, -68.2603, -807.899, 325.336, -13.8689, -0, -33.062, -1, 0);
				PlaceObjectByHash(2475986526, -71.1526, -807.598, 325.153, -12.0416, 4.26887, -28.0523, -1, 0);
				PlaceObjectByHash(2475986526, -73.2853, -806.628, 325.151, -11.7962, -0, -19.1717, -1, 0);
				PlaceObjectByHash(2475986526, -75.2243, -806.286, 325.164, -12.0419, -0, 1.55726, -1, 0);
				PlaceObjectByHash(2475986526, -77.5757, -806.312, 325.088, -14.1843, -0, 12.6263, -1, 0);
				PlaceObjectByHash(2475986526, -79.8704, -807.22, 325.143, -14.049, -4.26887, 21.4769, -1, 0);
				PlaceObjectByHash(2475986526, -82.0222, -807.83, 325.036, -14.1422, -4.26887, 32.7605, -1, 0);
				PlaceObjectByHash(2475986526, -83.8934, -809.424, 325.073, -14.5264, -8.53774, 46.5132, -1, 0);
				PlaceObjectByHash(2475986526, -85.2523, -810.983, 325.043, -14.859, -0, 53.5324, -1, 0);
				PlaceObjectByHash(2475986526, -86.5177, -813.202, 325.089, -14.5267, 4.26887, 64.6634, -1, 0);
				PlaceObjectByHash(2475986526, -87.6645, -815.707, 325.059, -14.8589, 4.26887, 73.157, -1, 0);
				PlaceObjectByHash(2475986526, -87.7973, -817.987, 325.119, -14.8468, -1.33402, 89.3982, -1, 0);
				PlaceObjectByHash(2475986526, -87.5801, -821.034, 325.059, -14.8593, -0, 95.4435, -1, 0);
				PlaceObjectByHash(2475986526, -87.2815, -822.239, 325.126, -15.6308, -4.26887, 100.311, -1, 0);
				PlaceObjectByHash(2475986526, -86.7602, -824.03, 325.044, -15.9224, -0, 116.957, -1, 0);
				PlaceObjectByHash(2475986526, -85.3743, -826.099, 325.136, -15.7025, 2.56132, 124.307, -1, 0);
				PlaceObjectByHash(2475986526, -83.4737, -828.611, 325.076, -15.0688, -0, 132.538, -1, 0);
				PlaceObjectByHash(2475986526, -87.9554, -832.877, 325.894, -14.1563, 4.26887, 132.995, -1, 0);
				PlaceObjectByHash(2475986526, -89.3865, -831.322, 325.887, -14.1562, -0, 126.154, -1, 0);
				PlaceObjectByHash(2475986526, -86.4247, -834.407, 325.915, -14.2701, 4.26887, 143.277, -1, 0);
				PlaceObjectByHash(2475986526, -85.1736, -833.789, 325.653, -14.4072, -4.26887, 145.777, -1, 0);
				PlaceObjectByHash(2475986526, -83.8118, -835.765, 326.063, -12.243, 4.26887, 151.527, -1, 0);
				PlaceObjectByHash(2475986526, -80.7015, -837.145, 326.059, -12.3172, 2.13443, 162.332, -1, 0);
				PlaceObjectByHash(2475986526, -77.6428, -837.649, 326.163, -10.8391, 3.20165, 171.297, -1, 0);
				PlaceObjectByHash(2475986526, -75.479, -837.909, 326.025, -12.3172, -1.06722, 174.574, -1, 0);
				PlaceObjectByHash(2475986526, -73.861, -837.826, 326.061, -12.3173, 5.33609, -176.632, -1, 0);
				PlaceObjectByHash(2475986526, -70.4799, -837.265, 326.09, -12.3173, -0, -166.182, -1, 0);
				PlaceObjectByHash(2475986526, -67.0415, -836.185, 326.018, -12.3171, -0, -156.039, -1, 0);
				PlaceObjectByHash(2475986526, -64.8504, -834.996, 325.951, -11.5263, -0, -145.834, -1, 0);
				PlaceObjectByHash(2475986526, -63.5702, -833.725, 326.1, -11.2947, -0, -140.961, -1, 0);
				PlaceObjectByHash(2475986526, -60.9992, -831.419, 326.075, -11.5262, -4.26887, -130.963, -1, 0);
				PlaceObjectByHash(2475986526, -58.9923, -828.729, 326.116, -11.5262, 4.26887, -121.973, -1, 0);
				PlaceObjectByHash(2475986526, -57.5045, -825.626, 326.114, -11.5263, -0, -110.959, -1, 0);
				PlaceObjectByHash(2475986526, -56.5533, -822.397, 326.08, -11.1311, -6.4033, -102, -1, 0);
				PlaceObjectByHash(2475986526, -56.0911, -820.05, 326.049, -11.0325, 2.13443, -100.794, -1, 0);
				PlaceObjectByHash(2475986526, -56.0681, -818.32, 326.087, -11.1312, -2.66804, -87.9469, -1, 0);
				PlaceObjectByHash(2475986526, -56.2989, -816.237, 326.048, -11.0324, 2.13443, -83.2139, -1, 0);
				PlaceObjectByHash(2475986526, -56.8952, -814.518, 326.142, -11.0324, -2.13443, -76.5476, -1, 0);
				PlaceObjectByHash(2475986526, -58.1209, -811.23, 326.116, -10.9697, -0, -66.7674, -1, 0);
				PlaceObjectByHash(2475986526, -59.0622, -809.17, 326.095, -11.0574, 4.26887, -62.782, -1, 0);
				PlaceObjectByHash(2475986526, -60.096, -807.639, 326.119, -11.5544, -0, -52.7596, -1, 0);
				PlaceObjectByHash(2475986526, -62.081, -805.317, 326.116, -11.1035, -0, -40.7682, -1, 0);
				PlaceObjectByHash(2475986526, -64.1466, -804.55, 326.283, -11.1035, -0, -30.477, -1, 0);
				PlaceObjectByHash(2475986526, -67.9795, -798.8, 326.717, -10.1561, -0, -29.3495, -1, 0);
				PlaceObjectByHash(2475986526, -67.5734, -802.52, 326.262, -10.471, -8.53774, -31.2185, -1, 0);
				PlaceObjectByHash(2475986526, -70.9341, -800.541, 326.198, -10.5317, -0, -20.0064, -1, 0);
				PlaceObjectByHash(2475986526, -75.3309, -801.285, 325.849, -10.2407, -0, 1.58401, -1, 0);
				PlaceObjectByHash(2475986526, -74.0222, -799.865, 326.177, -10.7327, -0, -5.98314, -1, 0);
				PlaceObjectByHash(2475986526, -76.5167, -797.998, 326.32, -12.4969, -2.66804, 1.58883, -1, 0);
				PlaceObjectByHash(2475986526, -79.2787, -800.531, 326.011, -12.9433, 4.26887, 13.0054, -1, 0);
				PlaceObjectByHash(2475986526, -81.6721, -801.017, 325.9, -12.4601, 2.13443, 17.3792, -1, 0);
				PlaceObjectByHash(2475986526, -83.6027, -801.744, 325.971, -12.9433, -0, 26.3052, -1, 0);
				PlaceObjectByHash(2475986526, -85.6586, -802.789, 325.95, -12.8791, 1.28066, 32.5856, -1, 0);
				PlaceObjectByHash(2475986526, -87.5086, -804.25, 325.978, -12.9432, 4.26887, 42.3279, -1, 0);
				PlaceObjectByHash(2475986526, -88.9923, -805.73, 325.89, -11.9333, -4.26887, 46.0613, -1, 0);
				PlaceObjectByHash(2475986526, -90.167, -807.318, 325.946, -13.0244, -0, 53.178, -1, 0);
				PlaceObjectByHash(2475986526, -93.5987, -807.353, 326.343, -11.5713, 4.26887, 60.8753, -1, 0);
				PlaceObjectByHash(2475986526, -93.5166, -813.963, 325.942, -13.4341, -4.26887, 73.0256, -1, 0);
				PlaceObjectByHash(2475986526, -92.121, -810.584, 325.996, -13.4339, -4.26887, 64.9353, -1, 0);
				PlaceObjectByHash(2475986526, -93.9931, -815.866, 325.924, -13.0519, -0, 79.5966, -1, 0);
				PlaceObjectByHash(2475986526, -93.8716, -817.904, 325.988, -13.4339, -0, 88.8361, -1, 0);
				PlaceObjectByHash(2475986526, -93.7912, -821.777, 325.946, -13.6946, -2.66804, 91.1427, -1, 0);
				PlaceObjectByHash(2475986526, -93.2951, -823.554, 325.966, -13.157, -0, 101.424, -1, 0);
				PlaceObjectByHash(2475986526, -92.5757, -827.033, 325.87, -13.5323, -0, 104.668, -1, 0);
				PlaceObjectByHash(2475986526, -91.53, -828.342, 325.842, -14.1563, 4.26887, 120.328, -1, 0);
				PlaceObjectByHash(2475986526, -90.5203, -829.611, 325.936, -14.1563, -0, 124.573, -1, 0);
				PlaceObjectByHash(2475986526, -95.5355, -833.068, 327.049, -9.63525, 1.70755, 124.512, -1, 0);
				PlaceObjectByHash(2475986526, -94.2445, -835.1, 326.976, -9.27617, -1.28066, 128.396, -1, 0);
				PlaceObjectByHash(2475986526, -92.513, -837.087, 327.008, -9.63523, -4.26887, 132.871, -1, 0);
				PlaceObjectByHash(2475986526, -90.07, -839.341, 327.025, -9.63574, 4.26887, 143.545, -1, 0);
				PlaceObjectByHash(2475986526, -86.7336, -841.135, 327.284, -9.63566, -0, 150.983, -1, 0);
				PlaceObjectByHash(2475986526, -84.8343, -842.167, 327.254, -9.36742, -4.26887, 152.377, -1, 0);
				PlaceObjectByHash(2475986526, -90.0883, -842.661, 327.589, -7.98782, -8.53774, 146.409, -1, 0);
				PlaceObjectByHash(2475986526, -82.595, -843.001, 327.277, -9.6352, -0, 161.654, -1, 0);
				PlaceObjectByHash(2475986526, -80.8027, -843.618, 327.263, -9.36755, -2.13443, 165.215, -1, 0);
				PlaceObjectByHash(2475986526, -78.5619, -843.703, 327.458, -9.63545, -2.13443, 171.015, -1, 0);
				PlaceObjectByHash(2475986526, -76.2479, -844.026, 327.261, -9.36765, 1.06722, 175.986, -1, 0);
				PlaceObjectByHash(2475986526, -73.5382, -843.999, 327.285, -9.6355, -0, -177.212, -1, 0);
				PlaceObjectByHash(2475986526, -71.2047, -843.988, 327.3, -9.36764, -1.06722, -172.013, -1, 0);
				PlaceObjectByHash(2475986526, -69.036, -843.266, 327.309, -9.63525, 4.26887, -166.686, -1, 0);
				PlaceObjectByHash(2475986526, -67.2981, -840.996, 326.756, -9.37509, -2.13443, -159.014, -1, 0);
				PlaceObjectByHash(2475986526, -66.7067, -842.714, 327.222, -9.37501, 2.13443, -159.27, -1, 0);
				PlaceObjectByHash(2475986526, -64.5693, -841.792, 327.24, -9.63515, 4.26887, -156.16, -1, 0);
				PlaceObjectByHash(2475986526, -61.8874, -840.436, 327.231, -9.37483, 4.26887, -146.534, -1, 0);
				PlaceObjectByHash(2475986526, -59.7118, -838.501, 327.384, -9.63533, -0, -141.372, -1, 0);
				PlaceObjectByHash(2475986526, -57.9491, -837.16, 327.309, -9.37471, 4.26887, -135.839, -1, 0);
				PlaceObjectByHash(2475986526, -56.3494, -835.471, 327.34, -9.63578, 4.26887, -131.675, -1, 0);
				PlaceObjectByHash(2475986526, -54.9387, -833.93, 327.334, -9.37482, -0, -127.887, -1, 0);
				PlaceObjectByHash(2475986526, -53.727, -832.032, 327.367, -9.63521, -4.26887, -122.142, -1, 0);
				PlaceObjectByHash(2475986526, -52.5928, -830.077, 327.332, -9.37496, -0, -116.843, -1, 0);
				PlaceObjectByHash(2475986526, -51.7552, -827.819, 327.385, -9.63569, 6.4033, -111.077, -1, 0);
				PlaceObjectByHash(2475986526, -51.0061, -825.839, 327.369, -9.37494, 4.26887, -107.054, -1, 0);
				PlaceObjectByHash(2475986526, -50.5468, -823.622, 327.378, -9.63572, 4.26887, -101.598, -1, 0);
				PlaceObjectByHash(2475986526, -50.0992, -820.896, 327.345, -9.47333, -1.06722, -95.7976, -1, 0);
				PlaceObjectByHash(2475986526, -49.9295, -818.102, 327.381, -9.63531, -8.00413, -88.2146, -1, 0);
				PlaceObjectByHash(2475986526, -50.1895, -815.816, 327.358, -9.4734, -0, -82.8649, -1, 0);
				PlaceObjectByHash(2475986526, -50.9164, -813.132, 327.442, -9.63524, 2.13443, -76.865, -1, 0);
				PlaceObjectByHash(2475986526, -51.1585, -811.568, 327.373, -9.58574, -0, -69.3402, -1, 0);
				PlaceObjectByHash(2475986526, -52.0622, -809.533, 327.354, -9.63541, 2.13443, -65.7624, -1, 0);
				PlaceObjectByHash(2475986526, -53.4048, -806.624, 327.376, -9.63526, 2.13443, -65.3971, -1, 0);
				PlaceObjectByHash(2475986526, -55.2978, -803.815, 327.389, -9.63524, 4.26887, -52.2107, -1, 0);
				PlaceObjectByHash(2475986526, -56.5179, -802.266, 327.366, -9.51013, 4.26887, -50.6537, -1, 0);
				PlaceObjectByHash(2475986526, -57.9995, -800.68, 327.42, -9.6353, 1.28066, -41.7027, -1, 0);
				PlaceObjectByHash(2475986526, -61.0278, -799.404, 327.549, -9.63516, 8.53774, -31.016, -1, 0);
				PlaceObjectByHash(2475986526, -64.37, -797.284, 327.603, -9.6351, -0, -31.6732, -1, 0);
				PlaceObjectByHash(2475986526, -66.3998, -795.965, 327.526, -9.42422, 8.53773, -29.018, -1, 0);
				PlaceObjectByHash(2475986526, -68.8079, -794.744, 327.535, -9.63558, -2.13443, -20.0341, -1, 0);
				PlaceObjectByHash(2475986526, -72.1225, -793.825, 327.497, -9.57894, -2.13443, -12.2336, -1, 0);
				PlaceObjectByHash(2475986526, -75.6415, -795.169, 327.2, -9.63555, -1.60083, 2.8097, -1, 0);
				PlaceObjectByHash(2475986526, -77.9613, -794.235, 327.223, -8.9769, -5.33608, 4.53814, -1, 0);
				PlaceObjectByHash(2475986526, -75.3695, -789.507, 328.306, -8.84722, -8.33763, -0.0879073, -1, 0);
				PlaceObjectByHash(2475986526, -80.6908, -794.505, 327.217, -9.63537, 4.26887, 13.0745, -1, 0);
				PlaceObjectByHash(2475986526, -83.5673, -795.148, 327.101, -9.92985, 2.13443, 17.5819, -1, 0);
				PlaceObjectByHash(2475986526, -86.3087, -796.203, 327.177, -9.63542, -4.26887, 25.9296, -1, 0);
				PlaceObjectByHash(2475986526, -88.9655, -797.634, 327.118, -9.92994, -4.26887, 33.0571, -1, 0);
				PlaceObjectByHash(2475986526, -91.6251, -799.702, 327.176, -9.63539, -0, 42.2513, -1, 0);
				PlaceObjectByHash(2475986526, -93.414, -801.299, 327.124, -9.92995, -0, 48.7085, -1, 0);
				PlaceObjectByHash(2475986526, -95.1453, -803.637, 327.147, -9.63537, -8.53774, 53.6544, -1, 0);
				PlaceObjectByHash(2475986526, -96.5885, -805.701, 327.144, -9.8947, -0, 60.5096, -1, 0);
				PlaceObjectByHash(2475986526, -97.6945, -807.971, 327.174, -9.63569, 4.26887, 64.7568, -1, 0);
				PlaceObjectByHash(2475986526, -98.7075, -809.885, 327.026, -8.13758, -0, 67.8881, -1, 0);
				PlaceObjectByHash(2475986526, -99.394, -812.176, 327.105, -9.63525, -4.26887, 73.0223, -1, 0);
				PlaceObjectByHash(2475986526, -100.025, -814.868, 327.097, -9.97277, 2.13443, 83.1537, -1, 0);
				PlaceObjectByHash(2475986526, -100.012, -817.789, 327.15, -9.63535, -1.33402, 88.8234, -1, 0);
				PlaceObjectByHash(2475986526, -100.069, -819.76, 327.099, -9.95297, -1.33402, 90.8729, -1, 0);
				PlaceObjectByHash(2475986526, -99.969, -821.91, 327.11, -9.63541, -2.66804, 91.5501, -1, 0);
				PlaceObjectByHash(2475986526, -99.3358, -824.801, 327.138, -9.63539, 2.13443, 101.678, -1, 0);
				PlaceObjectByHash(2475986526, -98.5443, -828.598, 327.033, -9.63553, -0, 104.64, -1, 0);
				PlaceObjectByHash(2475986526, -97.0896, -831.054, 326.937, -10.0741, 4.26887, 118.72, -1, 0);
				PlaceObjectByHash(2475986526, -102.435, -833.952, 328.506, -5.26399, -0, 118.502, -1, 0);
				PlaceObjectByHash(2475986526, -103.536, -831.932, 328.513, -5.42142, 4.26887, 111.099, -1, 0);
				PlaceObjectByHash(2475986526, -100.644, -836.571, 328.636, -5.26398, -0, 124.006, -1, 0);
				PlaceObjectByHash(2475986526, -99.0448, -838.912, 328.589, -5.26395, -2.13443, 128.175, -1, 0);
				PlaceObjectByHash(2475986526, -96.9401, -841.184, 328.589, -5.26384, -2.13443, 132.615, -1, 0);
				PlaceObjectByHash(2475986526, -95.4409, -842.718, 328.551, -5.01006, -2.13443, 136.57, -1, 0);
				PlaceObjectByHash(2475986526, -93.6584, -844.231, 328.606, -5.26388, -0, 143.429, -1, 0);
				PlaceObjectByHash(2475986526, -92.1044, -845.82, 328.655, -5.01307, -2.13443, 147.428, -1, 0);
				PlaceObjectByHash(2475986526, -89.6061, -846.328, 328.851, -5.26389, -0, 150.62, -1, 0);
				PlaceObjectByHash(2475986526, -87.5884, -847.552, 328.829, -5.6777, -0, 153.36, -1, 0);
				PlaceObjectByHash(2475986526, -84.5215, -848.802, 328.867, -5.26405, 5.33608, 161.164, -1, 0);
				PlaceObjectByHash(2475986526, -81.9779, -849.605, 328.821, -5.67769, 1.06722, 166.961, -1, 0);
				PlaceObjectByHash(2475986526, -79.5282, -849.717, 329.046, -5.26392, 1.06722, 170.517, -1, 0);
				PlaceObjectByHash(2475986526, -76.7555, -850.113, 328.885, -4.93224, 2.66804, 175.995, -1, 0);
				PlaceObjectByHash(2475986526, -73.2336, -850.06, 328.883, -5.26397, -0, -177.431, -1, 0);
				PlaceObjectByHash(2475986526, -70.4067, -849.836, 328.854, -4.82287, -5.33608, -172.2, -1, 0);
				PlaceObjectByHash(2475986526, -67.6252, -849.166, 328.911, -5.26394, -0, -166.741, -1, 0);
				PlaceObjectByHash(2475986526, -64.6525, -848.331, 328.792, -4.82267, -2.13443, -160.74, -1, 0);
				PlaceObjectByHash(2475986526, -62.1086, -847.355, 328.837, -5.26389, 2.13443, -156.346, -1, 0);
				PlaceObjectByHash(2475986526, -60.2755, -846.895, 328.808, -5.97307, -2.13443, -151.031, -1, 0);
				PlaceObjectByHash(2475986526, -58.5152, -845.543, 328.833, -5.26392, 1.06722, -147.129, -1, 0);
				PlaceObjectByHash(2475986526, -55.9339, -843.258, 328.987, -5.26394, 2.13443, -141.8, -1, 0);
				PlaceObjectByHash(2475986526, -53.6636, -841.564, 328.905, -5.18348, -0, -136.192, -1, 0);
				PlaceObjectByHash(2475986526, -51.8013, -839.526, 328.926, -5.26393, -2.13443, -131.788, -1, 0);
				PlaceObjectByHash(2475986526, -49.9112, -837.51, 328.916, -5.18352, -8.53774, -125.894, -1, 0);
				PlaceObjectByHash(2475986526, -48.5833, -835.261, 328.968, -5.26388, 2.13443, -122.598, -1, 0);
				PlaceObjectByHash(2475986526, -47.1369, -832.806, 328.936, -5.18352, -0, -117.146, -1, 0);
				PlaceObjectByHash(2475986526, -46.1092, -830.019, 328.985, -5.26389, -0, -111.097, -1, 0);
				PlaceObjectByHash(2475986526, -45.2549, -827.659, 328.957, -5.18353, -1.06722, -105.915, -1, 0);
				PlaceObjectByHash(2475986526, -44.5598, -824.856, 328.973, -5.26387, -0, -101.582, -1, 0);
				PlaceObjectByHash(2475986526, -44.0346, -821.522, 328.953, -5.26387, 5.33608, -95.978, -1, 0);
				PlaceObjectByHash(2475986526, -43.8673, -817.92, 328.98, -5.26387, -4.00206, -88.1556, -1, 0);
				PlaceObjectByHash(2475986526, -44.1983, -815.072, 328.956, -5.26387, -0, -82.8806, -1, 0);
				PlaceObjectByHash(2475986526, -45.0463, -811.788, 329.021, -5.26392, -0, -77.2513, -1, 0);
				PlaceObjectByHash(2475986526, -45.6154, -809.566, 328.95, -5.25705, -2.13443, -72.2094, -1, 0);
				PlaceObjectByHash(2475986526, -46.5685, -807.149, 328.929, -5.26395, -2.13443, -66.9958, -1, 0);
				PlaceObjectByHash(2475986526, -47.9752, -804.122, 328.959, -5.26398, -0, -65.1505, -1, 0);
				PlaceObjectByHash(2475986526, -49.0785, -802.078, 328.914, -5.31539, 4.26887, -57.7224, -1, 0);
				PlaceObjectByHash(2475986526, -50.5092, -800.141, 328.99, -5.26406, -0, -52.4683, -1, 0);
				PlaceObjectByHash(2475986526, -52.041, -798.134, 329.001, -5.31536, -2.13443, -49.2493, -1, 0);
				PlaceObjectByHash(2475986526, -53.8808, -796.134, 329.041, -5.26389, -0, -42.3308, -1, 0);
				PlaceObjectByHash(2475986526, -55.4375, -794.682, 329.045, -5.32055, 2.13443, -37.3601, -1, 0);
				PlaceObjectByHash(2475986526, -57.7537, -794.2, 329.16, -5.26393, 2.13443, -32.2267, -1, 0);
				PlaceObjectByHash(2475986526, -61.0299, -792.042, 329.172, -5.26389, -2.13443, -32.1174, -1, 0);
				PlaceObjectByHash(2475986526, -63.5163, -790.736, 329.085, -5.04535, -4.26887, -29.2933, -1, 0);
				PlaceObjectByHash(2475986526, -64.7324, -789.882, 329.081, -4.987, -2.13443, -27.7917, -1, 0);
				PlaceObjectByHash(2475986526, -66.7775, -788.94, 329.155, -5.04558, 1.06722, -19.5666, -1, 0);
				PlaceObjectByHash(2475986526, -68.6555, -788.272, 329.103, -5.30654, 3.20165, -16.9146, -1, 0);
				PlaceObjectByHash(2475986526, -70.8259, -787.837, 329.128, -5.04546, 1.06722, -12.2941, -1, 0);
				PlaceObjectByHash(2475986526, -74.5572, -787.022, 329.08, -4.61724, 1.06722, -10.7316, -1, 0);
				PlaceObjectByHash(2475986526, -75.8754, -788.646, 328.671, -6.78921, -0, 2.98721, -1, 0);
				PlaceObjectByHash(2475986526, -78.4, -788.132, 328.83, -5.91899, 2.66804, 3.75875, -1, 0);
				PlaceObjectByHash(2475986526, -80.5351, -788.179, 328.782, -5.80051, -0, 7.26539, -1, 0);
				PlaceObjectByHash(2475986526, -82.1189, -788.558, 328.793, -5.9192, 1.06722, 12.7168, -1, 0);
				PlaceObjectByHash(2475986526, -85.4054, -789.317, 328.666, -5.79433, -0, 17.1877, -1, 0);
				PlaceObjectByHash(2475986526, -87.4651, -789.98, 328.647, -5.63204, -0, 20.2315, -1, 0);
				PlaceObjectByHash(2475986526, -88.9795, -790.697, 328.76, -5.79416, -0, 25.9501, -1, 0);
				PlaceObjectByHash(2475986526, -90.9922, -791.487, 328.684, -6.26149, -0, 27.0819, -1, 0);
				PlaceObjectByHash(2475986526, -92.3298, -792.474, 328.677, -5.79412, -0, 33.3113, -1, 0);
				PlaceObjectByHash(2475986526, -94.2322, -793.73, 328.669, -5.58479, -2.13443, 37.4974, -1, 0);
				PlaceObjectByHash(2475986526, -95.7282, -795.2, 328.764, -5.79431, -0, 41.8672, -1, 0);
				PlaceObjectByHash(2475986526, -97.9782, -797.316, 328.695, -5.58488, -0, 48.9171, -1, 0);
				PlaceObjectByHash(2475986526, -100.042, -800.063, 328.731, -5.79425, 2.13443, 53.7039, -1, 0);
				PlaceObjectByHash(2475986526, -101.884, -802.718, 328.706, -5.5848, 2.13443, 60.3613, -1, 0);
				PlaceObjectByHash(2475986526, -103.09, -805.421, 328.718, -5.79425, -0, 64.506, -1, 0);
				PlaceObjectByHash(2475986526, -104.284, -807.711, 328.691, -5.58488, -2.13443, 65.966, -1, 0);
				PlaceObjectByHash(2475986526, -105.262, -810.369, 328.729, -5.26384, 1.06722, 73.2414, -1, 0);
				PlaceObjectByHash(2475986526, -105.769, -812.146, 328.645, -5.25205, -1.06722, 75.8091, -1, 0);
				PlaceObjectByHash(2475986526, -106.155, -814.128, 328.687, -5.26388, -1.06722, 82.8157, -1, 0);
				PlaceObjectByHash(2475986526, -106.062, -817.685, 328.758, -5.26388, 2.66804, 88.7458, -1, 0);
				PlaceObjectByHash(2475986526, -106.154, -819.723, 328.716, -5.25204, -0, 90.1628, -1, 0);
				PlaceObjectByHash(2475986526, -106.082, -822.072, 328.729, -5.26387, -1.33402, 91.2972, -1, 0);
				PlaceObjectByHash(2475986526, -105.911, -823.815, 328.7, -5.52135, 5.33608, 99.4393, -1, 0);
				PlaceObjectByHash(2475986526, -105.28, -826.029, 328.734, -5.26391, 1.06722, 101.615, -1, 0);
				PlaceObjectByHash(2475986526, -105.06, -827.904, 328.644, -5.29978, 1.06722, 102.515, -1, 0);
				PlaceObjectByHash(2475986526, -104.327, -830.112, 328.614, -5.26391, -2.13443, 104.719, -1, 0);
				PlaceObjectByHash(3291218330, -108.551, -853.416, 327.387, 2.94456, 89.1111, -166.155, -1, 0);
				PlaceObjectByHash(3291218330, -80.2509, -866.418, 327.301, 3.7405, 89.3, 146.641, -1, 0);
				PlaceObjectByHash(3291218330, -55.8513, -863.921, 327.333, 6.87468, 89.6184, 149.776, -1, 0);
				PlaceObjectByHash(3291218330, -37.3907, -848.122, 327.717, 2.33633, 88.8797, -16.2595, -1, 0);
				PlaceObjectByHash(3291218330, -26.1908, -818.332, 328.76, 0.490556, 84.6598, -18.107, -1, 0);
				PlaceObjectByHash(3291218330, -37.891, -789.138, 328.134, 1.11673, 87.6571, 42.7186, -1, 0);
				PlaceObjectByHash(3291218330, -63.492, -772.044, 327.866, 3.09962, 89.1556, 44.702, -1, 0);
				PlaceObjectByHash(3291218330, -93.4916, -774.848, 327.398, 2.73771, 89.0443, 122.539, -1, 0);
				PlaceObjectByHash(3291218330, -115.991, -795.259, 327.27, 3.28432, 89.2033, 123.086, -1, 0);
				PlaceObjectByHash(3291218330, -122.551, -825.074, 327.213, 173.37, 89.6048, 4.27077, -1, 0);
				PlaceObjectByHash(118627012, -74.8438, -819.617, 323.685, 0, 0, -3.37511, -1, 0);
				PlaceObjectByHash(2475986526, -67.6253, -820.244, 323.793, -14.4263, -8.53774, -100.02, -1, 0);
			}
		}
		else if (mapIndex == 1) {


			bool load = 0, unload = 0, teleport = 0;

			AddTitle("~b~Maze Bank Roof Ramp");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-74.94243f, -818.63446f, 326.174347f);
			}
			if (load) {
				PlaceObjectByHash(1600026313, -78.4864, -807.943, 323.202, 109.364, -89.9209, 0, -1, 1);
				PlaceObjectByHash(1600026313, -79.2766, -805.701, 323.204, 109.364, -89.9209, 0, -1, 1);
				PlaceObjectByHash(1600026313, -79.8373, -803.709, 323.205, 109.364, -89.9209, 0, -1, 1);
				PlaceObjectByHash(1600026313, -80.4295, -801.947, 323.207, 109.364, -89.9209, 0, -1, 1);
				PlaceObjectByHash(4143853297, -97.4731, -778.557, 308.877, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(1087520462, -84.2429, -793.182, 321.08, -63.5625, -0, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -83.5429, -795.106, 322.073, -63.5625, -0, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -84.9429, -791.108, 319.956, -62.6673, 8.53774e-007, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -85.8122, -788.585, 318.638, -63.5625, -0, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -89.1589, -779.487, 313.336, -59.5389, 0.0405551, -160.567, -1, 1);
				PlaceObjectByHash(1087520462, -90.7065, -774.863, 310.09, -57.4959, 0.322988, -160.758, -1, 1);
				PlaceObjectByHash(1087520462, -91.4887, -772.564, 308.403, -55.1692, 0.383369, -161.049, -1, 1);
				PlaceObjectByHash(1087520462, -91.7565, -771.74, 307.844, -56.4466, 0.0442451, -160.565, -1, 1);
				PlaceObjectByHash(1087520462, -93.6941, -766.245, 302.736, -45.9996, 0.0556114, -160.556, -1, 1);
				PlaceObjectByHash(1087520462, -94.2969, -764.648, 301.067, -44.7623, -1.70755e-006, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -94.2969, -764.648, 301.067, -44.7623, -1.70755e-006, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -94.886, -762.996, 298.741, -36.7051, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -95.4855, -761.334, 296.406, -36.7051, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -95.4855, -761.334, 296.406, -36.7051, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -96.1606, -759.499, 294.259, -42.0766, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -96.0707, -759.689, 293.709, -36.7051, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -96.0707, -759.689, 293.709, -36.7051, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -96.0707, -759.689, 293.46, -36.7051, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -96.8807, -757.391, 292.506, -51.0291, -8.53774e-007, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -96.8807, -757.391, 292.506, -51.0291, -8.53774e-007, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -97.3203, -756.159, 291.688, -57.2958, -0, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -97.9597, -754.358, 290.78, -62.6673, 8.53774e-007, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -97.9597, -754.358, 290.78, -62.6673, 8.53774e-007, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -98.7192, -752.356, 290.042, -69.9278, 3.20165e-005, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -99.0244, -751.684, 290.499, -90, -8.46346e-007, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -99.3223, -750.534, 290.479, -90, -8.46346e-007, -160.249, -1, 1);
				PlaceObjectByHash(1087520462, -100.348, -747.881, 290.452, -89.5256, -1.33402e-008, -159.354, -1, 1);
				PlaceObjectByHash(1087520462, -100.26, -748.154, 290.462, -76.096, 4.26887e-007, 19.6954, -1, 1);
				PlaceObjectByHash(1087520462, -100.687, -747.053, 290.731, -62.6673, -8.53774e-007, 20.5907, -1, 1);
				PlaceObjectByHash(1087520462, -101.346, -745.387, 291.611, -58.191, 1.70755e-006, 19.6954, -1, 1);
				PlaceObjectByHash(1087520462, -102.234, -743.119, 293.091, -52.2249, 0.00051141, 21.3426, -1, 1);
				PlaceObjectByHash(2475986526, -102.154, -739.285, 294.83, 9.80014, 0.295618, 18.7802, -1, 1);
				PlaceObjectByHash(2475986526, -105.054, -740.282, 294.827, 9.80014, 0.295618, 18.7802, -1, 1);
				PlaceObjectByHash(1087520462, -103.071, -741.047, 294.832, -48.0666, 0.000519094, 21.3419, -1, 1);
				PlaceObjectByHash(1087520462, -103.75, -739.405, 296.413, -45.1472, 0.000547269, 21.3416, -1, 1);
				PlaceObjectByHash(4143853297, -90.3515, -798.112, 319.893, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -93.2293, -790.348, 317.189, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -95.3479, -784.483, 313.696, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -100.01, -771.31, 304.367, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -101.829, -766.277, 299.666, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -103.318, -762.175, 293.966, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -104.948, -757.681, 288.866, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -108.146, -748.798, 288.866, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -108.146, -748.798, 295.608, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -108.225, -748.694, 302.608, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(3167053133, -103.451, -740.541, 307.317, -0.900199, -1.19985, 20.9076, -1, 1);
				PlaceObjectByHash(2375650849, -102.454, -742.6, 309.309, 0, 0, 20.9393, -1, 1);
				PlaceObjectByHash(4143853297, -101.483, -746.044, 305.602, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -94.7458, -743.402, 295.608, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -94.7566, -743.406, 288.866, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -94.7426, -743.595, 302.651, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(1157292806, -99.7732, -750.516, 309.575, 0, 0, 24.1761, -1, 1);
				PlaceObjectByHash(4143853297, -89.9785, -756.476, 293.966, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -91.5378, -752.285, 288.866, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -87.9094, -762.07, 299.666, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -86.2094, -766.939, 304.367, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -84.0215, -772.971, 309.575, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -81.6733, -779.348, 313.696, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -79.5187, -785.083, 317.189, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(4143853297, -76.5616, -793.191, 319.493, 0, -0, -159.831, -1, 1);
				PlaceObjectByHash(1998517203, -81.0993, -790.139, 326.57, 0, 0, -82.6177, -1, 1);
				PlaceObjectByHash(1998517203, -88.8988, -792.911, 326.95, 0, 0, -82.6177, -1, 1);
				PlaceObjectByHash(803874239, -88.3376, -794.173, 327.042, 0, 0, 31.5501, -1, 1);
				PlaceObjectByHash(803874239, -90.0376, -795.174, 327.262, 0, 0, 31.5501, -1, 1);
				PlaceObjectByHash(803874239, -91.6375, -796.175, 327.482, 0, 0, 31.5501, -1, 1);
				PlaceObjectByHash(803874239, -79.0283, -791.31, 326.763, 0, -0, 100.953, -1, 1);
				PlaceObjectByHash(803874239, -76.8377, -790.87, 326.823, 0, -0, 100.953, -1, 1);
				PlaceObjectByHash(803874239, -81.0088, -791.22, 326.713, 0, -0, 100.953, -1, 1);
			}

		}
		else if (mapIndex == 2) {



			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Beach Ferris-Ramp");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-1513.0f, -1192.0f, 1.0f);
			}
			if (load) {

				PlaceObjectByHash(1952396163, -1497.76, -1113.84, -3.08, -90, 6.14715e-007, 165.792, -1, 2);
				PlaceObjectByHash(2475986526, -1461.92, -1216.88, 2.5836, -2.3048, -0, -154.878, -1, 2);
				PlaceObjectByHash(3291218330, -1465.62, -1217.64, 18, 166.516, -5.12264e-006, 24.1717, -1, 2);
				PlaceObjectByHash(3291218330, -1458.89, -1214.4, 18, -38.4956, 8.53774e-007, -153.982, -1, 2);
				PlaceObjectByHash(2475986526, -1460.32, -1219.97, 4.3801, 12.6953, -0, -154.878, -1, 2);
				PlaceObjectByHash(2475986526, -1457, -1226.67, 11.8772, 31.7229, -0, -154.382, -1, 2);
				PlaceObjectByHash(2475986526, -1458.4, -1223.77, 7.9937, 23.6001, -0.0916355, -154.918, -1, 2);
				PlaceObjectByHash(2475986526, -1456.4, -1228.27, 14.9608, 48.674, -0, -153.982, -1, 2);
				PlaceObjectByHash(2475986526, -1456, -1229.07, 19.7441, 68.6628, -0, -153.982, -1, 2);
				PlaceObjectByHash(2475986526, -1456.2, -1228.47, 24.8276, 82.6252, 3.80938, -152.828, -1, 2);
				PlaceObjectByHash(2475986526, -1456.9, -1226.47, 28.9111, 108.498, -8.51368, -157.244, -1, 2);
				PlaceObjectByHash(2475986526, -1458.59, -1223.37, 31.5945, 130.616, -4.72983, -155.087, -1, 2);
				PlaceObjectByHash(2475986526, -1460.59, -1218.38, 33.5779, 143.744, -3.95611, -152.581, -1, 2);
				PlaceObjectByHash(2475986526, -1462.79, -1214.28, 34.161, 163.63, -2.68302, -155.763, -1, 2);
				PlaceObjectByHash(2475986526, -1465.3, -1209.78, 32.5228, -172.187, 4.69576e-006, -152.192, -1, 2);
				PlaceObjectByHash(2475986526, -1465.3, -1209.78, 32.5228, -172.187, 4.69576e-006, -152.192, -1, 2);
				PlaceObjectByHash(2475986526, -1466.9, -1205.68, 29.0062, -155.178, 9.47689e-005, -153.087, -1, 2);
				PlaceObjectByHash(2475986526, -1468.3, -1202.98, 24.1897, -131.11, 6.74481e-005, -153.088, -1, 2);
				PlaceObjectByHash(2475986526, -1468.59, -1202.68, 19.3732, -107.429, 3.07358e-005, -153.087, -1, 2);
				PlaceObjectByHash(2475986526, -1467.99, -1203.88, 13.5732, -89.6528, -0.153235, -155.853, -1, 2);
				PlaceObjectByHash(2475986526, -1467.11, -1205.68, 10.7072, -63.5491, 8.53774e-007, -156.504, -1, 2);
				PlaceObjectByHash(4109455646, -1465.05, -1210.03, 7.9503, 9.53319, 1.38057, 24.2606, -1, 2);
				PlaceObjectByHash(2975320548, -1460.95, -1218.79, 7.66, -29.9323, -0.173323, 24.7221, -1, 2);
				PlaceObjectByHash(2975320548, -1463.05, -1214.19, 6.7879, -6.50192, 1.391, 24.2651, -1, 2);
			}

		}
		else if (mapIndex == 3) {



			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Mount Chilliad Ramp");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(500, 5593, 795);
			}
			if (load) {

				PlaceObjectByHash(1952396163, -1497.76, -1113.84, -3.08, -90, -0, 165.792, 90, 3);
				PlaceObjectByHash(2475986526, -1461.92, -1216.88, 2.5836, -2.3048, 0, -154.878, 205.14, 3);
				PlaceObjectByHash(3291218330, -1458.89, -1214.4, 18, -38.4956, 0, -153.982, 211.95, 3);
				PlaceObjectByHash(2475986526, -1460.32, -1219.97, 4.3801, 12.6953, 0, -154.878, 205.672, 3);
				PlaceObjectByHash(2975320548, -1463.05, -1214.19, 6.7879, -6.5, -1.391, 24.2651, 24.4244, 3);
				PlaceObjectByHash(3291218330, -1465.62, -1217.64, 18, 166.516, 180, 24.1717, 155.224, 3);
				PlaceObjectByHash(4109455646, -1465.05, -1210.03, 7.9503, 9.5304, -1.3806, 24.2606, 24.5148, 3);
				PlaceObjectByHash(2975320548, -1460.95, -1218.79, 7.66, -29.9322, 0.1733, 24.7221, 27.9617, 3);
				PlaceObjectByHash(2475986526, -1458.4, -1223.77, 7.9937, 23.6001, 0.0916, -154.918, 207.065, 3);
				PlaceObjectByHash(2475986526, -1467.11, -1205.68, 10.7072, -63.5491, 0, -156.505, 224.303, 3);
				PlaceObjectByHash(2475986526, -1457, -1226.67, 11.8772, 31.7229, 0, -154.382, 209.411, 3);
				PlaceObjectByHash(2475986526, -1456.4, -1228.27, 14.9608, 48.674, 0, -153.982, 216.471, 3);
				PlaceObjectByHash(2475986526, -1456, -1229.07, 19.7441, 68.6628, 0, -153.982, 233.298, 3);
				PlaceObjectByHash(2475986526, -1456.2, -1228.47, 24.8276, 81.7043, -3.8094, -152.828, 252.429, 3);
				PlaceObjectByHash(2475986526, -1456.9, -1226.47, 28.9111, 110.301, 171.486, -157.244, 312.201, 3);
				PlaceObjectByHash(2475986526, -1458.59, -1223.37, 31.5945, 130.843, 175.27, -155.087, 325.759, 3);
				PlaceObjectByHash(2475986526, -1460.59, -1218.38, 33.5779, 143.844, 176.044, -152.581, 327.979, 3);
				PlaceObjectByHash(2475986526, -1462.79, -1214.28, 34.161, 163.648, 177.317, -155.763, 335.024, 3);
				PlaceObjectByHash(2475986526, -1465.3, -1209.78, 32.5228, -172.187, -180, -152.192, 331.971, 3);
				PlaceObjectByHash(2475986526, -1466.9, -1205.68, 29.0062, -155.178, -180, -153.087, 330.783, 3);
				PlaceObjectByHash(2475986526, -1468.3, -1202.98, 24.1897, -131.11, -180, -153.088, 322.332, 3);
				PlaceObjectByHash(2475986526, -1468.59, -1202.68, 19.3732, -107.429, -180, -153.087, 300.544, 3);
				PlaceObjectByHash(2475986526, -1467.99, -1203.88, 13.5732, -89.6205, 0.1532, -155.853, 269.072, 3);
				PlaceObjectByHash(3966705493, 509.842, 5589.24, 791.066, 0.141, 0, 65.3998, 65.3999, 3);
				PlaceObjectByHash(3966705493, 520.5, 5584.38, 790.503, 5.441, 0, 65.3998, 65.4976, 3);
				PlaceObjectByHash(3966705493, 531.057, 5579.54, 788.691, 12.441, 0, 65.3998, 65.9111, 3);
				PlaceObjectByHash(3966705493, 568.672, 5562.32, 767.428, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 576.972, 5558.53, 759.566, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 560.174, 5566.2, 774.698, 35.0403, 0, 65.3998, 69.4512, 3);
				PlaceObjectByHash(3966705493, 541.325, 5574.84, 785.49, 19.4409, 0, 65.3998, 66.6484, 3);
				PlaceObjectByHash(3966705493, 551.066, 5570.37, 780.799, 27.5407, 0, 65.3998, 67.9049, 3);
				PlaceObjectByHash(3966705493, 585.249, 5554.75, 751.745, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 618.334, 5539.62, 720.386, 40.7936, 0, 65.3998, 70.8829, 3);
				PlaceObjectByHash(3966705493, 626.602, 5535.85, 712.547, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 610.065, 5543.4, 728.217, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 601.777, 5547.19, 736.076, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 593.507, 5550.97, 743.917, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 634.862, 5532.07, 704.725, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 643.121, 5528.29, 696.894, 40.7936, 0, 65.3998, 70.8829, 3);
				PlaceObjectByHash(3966705493, 651.391, 5524.51, 689.053, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 659.651, 5520.73, 681.221, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 667.911, 5516.94, 673.389, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 676.171, 5513.17, 665.558, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 684.431, 5509.38, 657.727, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 692.691, 5505.61, 649.905, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 700.95, 5501.83, 642.074, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 709.22, 5498.05, 634.243, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 717.46, 5494.28, 626.431, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 725.72, 5490.5, 618.6, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 733.98, 5486.72, 610.778, 40.7396, 0, 65.3998, 70.8685, 3);
				PlaceObjectByHash(3966705493, 742.6, 5482.78, 603.167, 36.9395, 0, 65.3998, 69.9005, 3);
				PlaceObjectByHash(3966705493, 751.83, 5478.55, 596.335, 31.0392, 0, 65.3998, 68.5807, 3);
				PlaceObjectByHash(3966705493, 761.71, 5474.02, 590.613, 24.5989, 0, 65.3998, 67.3986, 3);
				PlaceObjectByHash(3966705493, 772.07, 5469.28, 586.08, 18.9288, 0, 65.3998, 66.5835, 3);
				PlaceObjectByHash(3966705493, 782.84, 5464.34, 582.86, 11.5788, 0, 65.3998, 65.8427, 3);
				PlaceObjectByHash(3966705493, 793.89, 5459.28, 581.117, 5.0787, 0, 65.3998, 65.485, 3);
				PlaceObjectByHash(3966705493, 805.1, 5454.15, 580.876, -2.5212, 0, 65.3998, 65.4208, 3);
				PlaceObjectByHash(3966705493, 816.17, 5449.08, 581.975, -7.6213, 0, 65.3998, 65.5917, 3);
				PlaceObjectByHash(3966705493, 827.191, 5444.04, 584.582, -16.6212, 0, 65.3998, 66.3125, 3);
				PlaceObjectByHash(3966705493, 837.681, 5439.24, 588.899, -24.421, 0, 65.3998, 67.3698, 3);
				PlaceObjectByHash(2580877897, 522.61, 5584.49, 779.214, 79.7153, -9.2252, 55.7018, 77.7612, 3);
				PlaceObjectByHash(3862788492, 522.445, 5583.69, 779.551, -0.9197, -69.229, -167.468, 184.555, 3);
			}
		}

		else if (mapIndex == 4) {



			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Airport Mini Ramp");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-1208, -2950, 13);
			}
			if (load) {
#pragma region MAP MOD
				PlaceObjectByHash(2475986526, -1242.08, -2931.15, 12.9924, -0.1046, -3.33505e-009, 61.0607, -1, 4);
				PlaceObjectByHash(2475986526, -1247.11, -2928.46, 15.013, -0.1046, -3.33505e-009, 61.0607, -1, 4);
				PlaceObjectByHash(2475986526, -1251.58, -2926.05, 16.7865, -0.1046, -3.33505e-009, 61.0607, -1, 4);
				PlaceObjectByHash(2475986526, -1254.69, -2924.35, 18.25, -0.1046, -3.33505e-009, 61.0607, -1, 4);
				PlaceObjectByHash(3966705493, -1276.69, -2912.99, 23.0019, 0, 0.05, 60.9705, -1, 4);
				PlaceObjectByHash(2475986526, -1258.35, -2922.28, 20.2135, -0.1046, -3.33505e-009, 61.0607, -1, 4);
				PlaceObjectByHash(3966705493, -1270.89, -2916.22, 23.0123, 0, 0, 60.8909, -1, 4);
				PlaceObjectByHash(3966705493, -1270.25, -2914.99, 23.0137, 0, 0, 60.8909, -1, 4);
				PlaceObjectByHash(3966705493, -1274.87, -2909.4, 23.0049, 0, 0.05, 60.9705, -1, 4);
				PlaceObjectByHash(3966705493, -1269.01, -2912.64, 22.9993, 0, 0.05, 60.9705, -1, 4);
				PlaceObjectByHash(3966705493, -1267.87, -2915.44, 28.3632, 0, -0, 147.299, -1, 4);
				PlaceObjectByHash(3966705493, -1272.13, -2918.33, 28.4791, 0, 0.05, 60.9705, -1, 4);
				PlaceObjectByHash(3966705493, -1272.11, -2918.35, 25.6708, -0.48, 0.0499982, 60.9701, -1, 4);
				PlaceObjectByHash(3966705493, -1277.93, -2915.14, 25.604, 0, 0.05, 60.9705, -1, 4);
				PlaceObjectByHash(3966705493, -1279.69, -2909.85, 25.6358, 0, -0, -151.239, -1, 4);
				PlaceObjectByHash(3966705493, -1279.69, -2909.85, 28.4844, 0, -0, -151.239, -1, 4);
				PlaceObjectByHash(2475986526, -1261.82, -2920.38, 21.767, -0.1046, -3.33505e-009, 61.0607, -1, 4);
				PlaceObjectByHash(3966705493, -1273.65, -2907.11, 22.9763, 0, 0.05, 60.9705, -1, 4);
				PlaceObjectByHash(3966705493, -1267.77, -2910.37, 22.9978, 0, 0.05, 60.9705, -1, 4);
				PlaceObjectByHash(3966705493, -1266.49, -2908.08, 22.9987, 0, -0, -119.462, -1, 4);
				PlaceObjectByHash(3966705493, -1265.15, -2905.8, 23.0042, 0, -0, -119.462, -1, 4);
				PlaceObjectByHash(3966705493, -1266.44, -2905.21, 25.6255, 0, -0, -118.761, -1, 4);
				PlaceObjectByHash(3966705493, -1265.66, -2911.99, 25.6968, 0, 0, -30.9603, -1, 4);
				PlaceObjectByHash(3966705493, -1264.88, -2910.66, 25.6982, 0, 0, -30.9603, -1, 4);
				PlaceObjectByHash(3966705493, -1264.84, -2905.14, 25.624, 0, -0, -118.761, -1, 4);
				PlaceObjectByHash(3966705493, -1272.37, -2900.96, 25.6199, 0, -0, -118.761, -1, 4);
				PlaceObjectByHash(3966705493, -1276.35, -2903.91, 25.6214, 0, -0, -151.239, -1, 4);
				PlaceObjectByHash(3966705493, -1276.35, -2903.91, 28.4329, 0, -0, -151.239, -1, 4);
				PlaceObjectByHash(3966705493, -1272.37, -2900.96, 28.4385, 0, -0, -118.761, -1, 4);
				PlaceObjectByHash(3966705493, -1266.44, -2905.21, 28.437, 0, -0, -118.761, -1, 4);
				PlaceObjectByHash(3966705493, -1265.17, -2905.14, 28.3426, 0, -0, -118.861, -1, 4);
				PlaceObjectByHash(3966705493, -1271.09, -2902.58, 23.0057, 0, -0, -119.462, -1, 4);
				PlaceObjectByHash(3966705493, -1272.37, -2904.83, 22.9972, 0, -0, -119.462, -1, 4);
#pragma endregion	
			}
		}
		else if (mapIndex == 5) {




			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Airport Gate Ramp");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-1046, -2538, 20);
			}
			if (load) {
#pragma region MAP MOD
				PlaceObjectByHash(2475986526, -1098.36, -2631.17, 19, 0, -0, 152.671, -1, 5);
				PlaceObjectByHash(2475986526, -1100.26, -2634.64, 21.1976, 16.2002, 0.192059, 150.427, -1, 5);
				PlaceObjectByHash(2475986526, -1102.26, -2638.02, 25.01, 26.7003, 0.178675, 149.261, -1, 5);
				PlaceObjectByHash(2475986526, -1103.96, -2640.91, 29.04, 28.3717, -0, 146.82, -1, 5);
				PlaceObjectByHash(1952396163, -1119.61, -2670.96, -5.125, 0, -0, 150.514, -1, 5);
				PlaceObjectByHash(1952396163, -1119.61, -2670.96, -5.125, 0, -0, 150.401, -1, 5);
				PlaceObjectByHash(3137065507, -1044.69, -2530.08, 20.4011, 94.8962, 4.26887e-007, 147.716, -1, 5);
#pragma endregion	
			}
		}
		else if (mapIndex == 6) {



			bool load = 0, unload = 0, teleport = 0;
			AddTitle("UFO Tower");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(70, -674, 680);
			}
			if (load) {
#pragma region MAP MOD
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 654.365, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 646.186, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 638.008, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 629.829, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 621.65, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 613.471, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 605.292, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 597.114, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 588.935, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 580.756, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 572.577, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 564.399, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 556.22, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 662.544, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 548.041, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 539.862, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 531.683, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 523.505, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 515.326, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 507.147, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 498.968, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 490.79, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 482.611, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 474.432, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 466.253, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 458.074, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 449.896, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 441.717, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 433.538, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 425.359, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 417.18, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 409.001, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 400.823, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 392.644, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 384.465, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 376.286, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 368.107, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 359.929, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 351.75, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 343.571, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 335.392, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 327.213, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 319.035, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 310.856, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 302.677, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 294.498, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 286.319, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 278.141, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 269.962, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 261.783, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 253.604, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 245.425, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 237.247, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 229.068, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 220.889, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 212.71, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 204.531, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 196.353, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 188.174, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 179.995, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 171.816, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 163.637, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 155.459, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 147.28, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 139.101, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 130.922, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 122.743, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 114.565, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 106.386, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 98.207, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 90.0282, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 81.8494, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 73.6706, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 65.4918, 0.660085, -0.919939, -109.32, -1, 6);
				PlaceObjectByHash(3026699584, 70.2592, -674.044, 57.313, 0.660085, -0.919939, -109.32, -1, 6);
#pragma endregion	
			}
		}
		else if (mapIndex == 7) {


			bool load = 0, unload = 0, teleport = 0;
			AddTitle("4 Maze Bank Ramps");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-74.94243f, -818.63446f, 326.174347f);
			}
			if (load) {
#pragma region MAP MOD
				PlaceObjectByHash(3522933110, -81.3886, -814.648, 325.169, 0, -0, 180, -1, 7);
				PlaceObjectByHash(3681122061, -81.7456, -809.064, 324.799, 0.500021, 2.66804, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -86.1333, -802.279, 321.92, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -89.7406, -796.701, 316.539, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -93.601, -790.725, 310.777, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -97.4741, -784.73, 304.997, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -101.373, -778.696, 299.179, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -105.233, -772.72, 293.417, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -109.106, -766.725, 287.637, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -112.954, -760.769, 281.894, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -116.827, -754.773, 276.113, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -120.687, -748.798, 270.352, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -124.518, -742.868, 264.636, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -128.358, -736.925, 258.909, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -132.22, -730.949, 253.151, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -136.081, -724.974, 247.394, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -139.943, -718.998, 241.636, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -143.826, -712.99, 235.846, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -147.667, -707.047, 230.12, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -151.508, -701.104, 224.394, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -155.369, -695.128, 218.636, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -159.252, -689.12, 212.846, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -163.072, -683.209, 207.152, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -166.976, -677.168, 201.331, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -170.838, -671.193, 195.573, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -174.7, -665.217, 189.815, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -178.583, -659.209, 184.026, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -182.444, -653.233, 178.268, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -186.327, -647.225, 172.479, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -190.189, -641.249, 166.721, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -194.03, -635.306, 160.994, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -197.871, -629.363, 155.268, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -201.711, -623.42, 149.542, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -205.552, -617.477, 143.815, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -209.393, -611.534, 138.089, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -213.255, -605.559, 132.331, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -217.095, -599.616, 126.605, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -220.957, -593.64, 120.847, -38.9999, -1.45141, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -222.245, -591.648, 118.928, -33.8999, 1.02453, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -223.349, -589.94, 117.561, -29.31, 1.79292, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -224.58, -588.036, 116.288, -26.25, 5.12264, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -225.869, -586.04, 115.116, -24.7199, -1.10991, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -227.127, -584.095, 114.05, -21.6599, 1.8783, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -228.615, -581.794, 112.961, -19.6199, 1.02453, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -230.201, -579.341, 111.92, -17.0699, -0, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -232.121, -576.369, 110.833, -12.9899, 4.26887, 32.8808, -1, 7);
				PlaceObjectByHash(3681122061, -234.105, -573.302, 109.991, -9.9299, -2.98821, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -236.628, -569.396, 109.329, -7.3799, -4.26887, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -239.81, -564.475, 108.721, -4.3199, 1.28066, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -241.76, -561.459, 108.549, -0.7499, -1.12058, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -244.04, -557.932, 108.494, 2.82011, -2.77476, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -246.372, -554.326, 108.705, 5.8801, -2.77476, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -248.668, -550.777, 109.14, 10.4701, 8.96462, 32.8806, -1, 7);
				PlaceObjectByHash(3681122061, -251.664, -546.138, 110.313, 13.5301, 1.15259, 32.8806, -1, 7);
				PlaceObjectByHash(3681122061, -254.537, -541.694, 111.791, 16.5901, 4.26887, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -256.28, -538.999, 112.748, 19.6501, -1.19528, 32.8807, -1, 7);
				PlaceObjectByHash(3681122061, -65.9078, -814.752, 326.106, 19.89, 4.26887, -53.8105, -1, 7);
				PlaceObjectByHash(3681122061, -58.6541, -809.444, 327.336, -4.08004, -2.13443, -53.8103, -1, 7);
				PlaceObjectByHash(3681122061, -52.4476, -804.909, 323.715, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -47.2332, -801.09, 317.168, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -42.0187, -797.272, 310.621, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -36.8326, -793.474, 304.109, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -31.5898, -789.635, 297.526, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -26.4037, -785.838, 291.014, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -21.1893, -782.019, 284.467, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -15.9748, -778.201, 277.919, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -10.7604, -774.383, 271.372, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -5.57426, -770.585, 264.86, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, -0.359839, -766.767, 258.313, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 4.82623, -762.969, 251.799, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 10.0123, -759.171, 245.285, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 15.2268, -755.353, 238.735, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 20.4412, -751.535, 232.184, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 25.6273, -747.737, 225.67, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 30.8135, -743.939, 219.155, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 36.0279, -740.121, 212.605, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 41.214, -736.323, 206.091, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 46.4285, -732.505, 199.54, -45.3899, 2.56132, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 48.4122, -731.052, 197.049, -41.8198, 1.62217, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 49.5549, -730.218, 195.782, -38.2499, 3.24434, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 51.171, -729.035, 194.203, -36.2098, 2.39057, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 52.8966, -727.773, 192.637, -33.1499, -6.83019, -53.8101, -1, 7);
				PlaceObjectByHash(3681122061, 54.586, -726.537, 191.27, -30.0898, 8.70849, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 56.5413, -725.105, 189.866, -25.4998, 7.59859, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 58.8359, -723.425, 188.509, -22.4398, 4.26887, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 60.738, -722.033, 187.536, -18.3599, 1.10991, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 63.1509, -720.268, 186.544, -15.8098, 5.03727, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 65.131, -718.821, 185.849, -12.7498, 5.07995, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 67.1384, -717.352, 185.286, -9.17981, 4.78113, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 69.2894, -715.776, 184.855, -4.5898, 4.18349, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 71.7831, -713.952, 184.607, 0.000193536, 4.16213, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 74.0832, -712.268, 184.607, 3.06019, 3.7566, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 76.0175, -710.853, 184.736, 8.1602, 4.35424, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 77.7752, -709.567, 185.048, 13.2602, 5.50684, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 79.6997, -708.158, 185.61, 17.3402, 3.7566, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 81.3947, -706.918, 186.266, 21.9302, 4.26887, -53.81, -1, 7);
				PlaceObjectByHash(3681122061, 83.3036, -705.52, 187.219, 26.0102, 9.39151, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 85.6244, -703.821, 188.622, 29.0702, 1.96368, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 87.3526, -702.556, 189.812, 33.1501, 2.90283, -53.8098, -1, 7);
				PlaceObjectByHash(3681122061, 89.2107, -701.196, 191.316, 37.2301, 4.86651, -53.8098, -1, 7);
				PlaceObjectByHash(3681122061, 90.8492, -699.998, 192.859, 41.82, -2.56132, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 92.6236, -698.701, 194.826, 46.41, 8.2816, -53.8099, -1, 7);
				PlaceObjectByHash(3681122061, 94.2096, -697.539, 196.89, 52.0199, 6.57406, -53.8098, -1, 7);
				PlaceObjectByHash(3681122061, 95.6251, -696.503, 199.137, 56.61, 9.22075, -53.8097, -1, 7);
				PlaceObjectByHash(3681122061, 96.9799, -695.512, 201.683, 61.7098, 8.53774, -53.8097, -1, 7);
				PlaceObjectByHash(3681122061, 98.1658, -694.646, 204.413, 65.7899, 5.03726, -53.8096, -1, 7);
				PlaceObjectByHash(3681122061, -69.0186, -829.452, 324.775, 0, -0, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -65.276, -836.288, 321.491, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -62.2554, -842.061, 315, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -59.2515, -847.802, 308.544, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -56.2313, -853.574, 302.053, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -53.1945, -859.378, 295.526, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -50.2071, -865.088, 289.106, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -47.2032, -870.829, 282.65, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -44.1829, -876.602, 276.159, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -41.1626, -882.374, 269.667, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -38.1751, -888.084, 263.247, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -35.1713, -893.825, 256.791, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -32.1674, -899.566, 250.335, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -29.1635, -905.307, 243.879, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -26.1432, -911.079, 237.388, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -23.1393, -916.821, 230.932, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -20.119, -922.593, 224.44, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -17.1152, -928.334, 217.985, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -14.1112, -934.075, 211.529, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -11.1235, -939.785, 205.108, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -8.13589, -945.495, 198.687, -44.8796, 3.24434, -152.398, -1, 7);
				PlaceObjectByHash(3681122061, -5.28891, -951.101, 192.102, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, -2.35757, -956.552, 185.364, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 0.589775, -962.033, 178.59, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 3.5211, -967.483, 171.852, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 9.3998, -978.414, 158.339, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 6.46847, -972.964, 165.077, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 12.3311, -983.865, 151.601, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 15.2944, -989.375, 144.789, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 21.1569, -1000.28, 131.313, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 18.2417, -994.856, 138.014, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 24.0722, -1005.7, 124.612, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 27.0355, -1011.21, 117.801, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 29.9828, -1016.69, 111.026, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 35.8294, -1027.56, 97.5867, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 37.2241, -1030.15, 94.4555, -44.3698, -1.70754, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 38.7475, -1032.99, 91.3086, -39.7798, -4.26887, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 39.7483, -1034.85, 89.5491, -36.7197, 3.4151, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 40.9818, -1037.14, 87.6062, -33.6597, -2.21981, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 42.1248, -1039.27, 85.999, -30.0898, -1.96368, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 32.9141, -1022.14, 104.288, -47.4298, -1.79292, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 43.3132, -1041.47, 84.5449, -26.5197, -1.79293, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 44.7115, -1044.08, 83.0715, -23.4597, -2.39056, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 45.9496, -1046.38, 81.937, -20.3997, -2.47594, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 47.0815, -1048.48, 81.0483, -17.3397, -2.09174, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 48.302, -1050.75, 80.2436, -14.7897, -8.96462, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 50.0647, -1054.03, 79.2608, -13.2597, -2.77476, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 52.0238, -1057.67, 78.2861, -11.7297, -1.45141, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 53.7395, -1060.86, 77.5341, -9.17973, -1.1099, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 55.7265, -1064.56, 76.8558, -6.11973, -6.61674, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 57.3746, -1067.62, 76.4825, -3.56972, -9.60496, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 59.3125, -1071.23, 76.2272, 0.000276446, -9.05818, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 61.112, -1074.58, 76.2272, 4.08028, -8.00412, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 62.529, -1077.21, 76.4405, 7.65027, -7.04364, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 64.0779, -1080.1, 76.8796, 10.7103, -1.28066, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 65.9388, -1083.55, 77.623, 14.2802, -1.70755, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 67.4076, -1086.29, 78.4126, 17.3403, -1.57948, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, 68.7639, -1088.81, 79.3066, 21.4202, 1.36604, -151.734, -1, 7);
				PlaceObjectByHash(3681122061, -86.0915, -825.576, 324.775, 0, -0, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -99.1939, -833.684, 315.911, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -105.248, -837.511, 310.056, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -93.1729, -829.876, 321.734, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -111.268, -841.319, 304.233, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -123.245, -848.891, 292.651, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -147.333, -864.12, 269.359, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -141.278, -860.292, 275.213, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -135.256, -856.485, 281.036, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -129.266, -852.699, 286.828, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -117.224, -845.084, 298.474, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -165.367, -875.521, 251.921, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -213.415, -905.895, 205.464, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -207.426, -902.108, 211.255, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -201.403, -898.301, 217.078, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -195.414, -894.515, 222.87, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -183.434, -886.942, 234.452, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -177.445, -883.156, 240.244, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -171.422, -879.348, 246.067, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -159.378, -871.734, 257.713, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -153.355, -867.927, 263.536, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -189.424, -890.728, 228.661, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -231.449, -917.296, 188.027, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -225.46, -913.509, 193.818, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -219.47, -909.723, 199.609, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -237.439, -921.082, 182.235, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -284.806, -951.016, 167.673, 28.5601, -7.5132, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -289.048, -953.697, 170.578, 34.1701, -3.41509, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -290.727, -954.757, 171.926, 37.23, 3.41509, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -286.998, -952.399, 169.084, 31.62, -1.36604, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -282.375, -949.481, 166.27, 26.0101, 3.50047, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -280.014, -947.991, 165.174, 21.4201, -6.83019, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -277.4, -946.336, 164.178, 17.8501, -3.15896, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -270.266, -941.827, 162.896, 4.59013, -1.79292, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -266.683, -939.562, 163.103, -6.11989, -1.38738, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -259.328, -934.913, 165.339, -17.8499, -4.69576, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -253.478, -931.22, 168.474, -26.01, -4.18349, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -248.103, -927.823, 172.247, -33.66, -5.97642, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -243.429, -924.868, 176.444, -39.27, -2.30519, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -256.593, -933.187, 166.676, -22.4399, -5.97641, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -262.183, -936.72, 164.252, -14.2799, -3.20165, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -268.321, -940.597, 162.896, 0.000125527, -1.95033, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -272.668, -943.344, 163.123, 8.67012, -2.86014, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -275.091, -944.877, 163.561, 12.7501, -5.1226, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -249.757, -928.867, 170.944, -31.11, -6.0617, 122.286, -1, 7);
				PlaceObjectByHash(3681122061, -251.566, -930.012, 169.653, -27.54, -3.58585, 122.286, -1, 7);
#pragma endregion
			}
		}
		else if (mapIndex == 8) {


			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Freestyle Motocross");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-2731, 3259, 32);
			}
			if (load) {
				PlaceObjectByHash(3213433054, -2661.22, 3209.43, 32.7118, 0, -0, -120.437, -1, 8);
				PlaceObjectByHash(3213433054, -2657.86, 3207.56, 32.7118, 0, 0, 59.6808, -1, 8);
				PlaceObjectByHash(3213433054, -2659.52, 3212.33, 32.7118, 0, -0, -120.437, -1, 8);
				PlaceObjectByHash(3213433054, -2656.2, 3210.41, 32.7118, 0, 0, 59.6808, -1, 8);
				PlaceObjectByHash(3213433054, -2654.54, 3213.26, 32.7118, 0, 0, 59.6808, -1, 8);
				PlaceObjectByHash(3213433054, -2657.84, 3215.18, 32.7118, 0, -0, -120.437, -1, 8);
				PlaceObjectByHash(3213433054, -2615.14, 3183, 32.8118, 0, -0, -120.45, -1, 8);
				PlaceObjectByHash(3213433054, -2611.82, 3181.02, 32.8118, 0, 0, 58.529, -1, 8);
				PlaceObjectByHash(3213433054, -2613.47, 3185.85, 32.8118, 0, -0, -120.45, -1, 8);
				PlaceObjectByHash(3213433054, -2610.11, 3183.91, 32.8118, 0, 0, 59.589, -1, 8);
				PlaceObjectByHash(3213433054, -2611.79, 3188.7, 32.8118, 0, -0, -120.45, -1, 8);
				PlaceObjectByHash(3213433054, -2608.46, 3186.71, 32.8118, 0, 0, 58.529, -1, 8);
				PlaceObjectByHash(3213433054, -2550.83, 3162.65, 32.7702, 0, -0, -117.989, -1, 8);
				PlaceObjectByHash(4109455646, -2547.01, 3160.66, 34.9496, 11.66, -2.56132e-006, -118.95, -1, 8);
				PlaceObjectByHash(3213433054, -2476.76, 3120.72, 32.7718, 0, -0, -119.612, -1, 8);
				PlaceObjectByHash(4109455646, -2473.51, 3118.83, 34.5672, 8.47412, -0.0223369, -120.965, -1, 8);
				PlaceObjectByHash(2975320548, -2442.43, 3105.7, 35.6224, -7.42001, -5.12265e-006, 150.074, -1, 8);
				PlaceObjectByHash(2975320548, -2443.67, 3101.83, 35.8732, -11.66, 3.84198e-006, 59.738, -1, 8);
				PlaceObjectByHash(2975320548, -2438.8, 3099, 36.0155, -11.66, -6.40331e-006, 59.7379, -1, 8);
				PlaceObjectByHash(2975320548, -2434.05, 3096.22, 36.6871, -22.26, -1.45141e-005, 59.7379, -1, 8);
				PlaceObjectByHash(2975320548, -2446.85, 3098.2, 35.6088, -7.42, 3.20165e-006, -31.8463, -1, 8);
				PlaceObjectByHash(2402097066, -2448.77, 3097.65, 35.4732, 0, -0, -120.616, -1, 8);
				PlaceObjectByHash(2402097066, -2442.95, 3108.08, 35.4832, 0, -0, -117.436, -1, 8);
				PlaceObjectByHash(2402097066, -2442.95, 3108.08, 35.4832, 0, -0, -117.436, -1, 8);
				PlaceObjectByHash(3681122061, -2389.55, 3069.11, 36.5952, 179.801, -3.76861e-006, -117.806, -1, 8);
				PlaceObjectByHash(1982829832, -2363.33, 3056.01, 31.8257, 0, -0, -119.219, -1, 8);
				PlaceObjectByHash(3681122061, -2389.55, 3069.11, 36.5952, 179.801, -3.76861e-006, -117.806, -1, 8);
				PlaceObjectByHash(2609922146, -2358.79, 3060.59, 31.8217, 0, -0, -119.371, -1, 8);
				PlaceObjectByHash(2975320548, -2325.93, 3034.99, 33.3214, 19.8, 3.41509e-006, -120.09, -1, 8);
				PlaceObjectByHash(2975320548, -2321.78, 3032.58, 36.3899, 25.7399, -8.62311e-005, -120.09, -1, 8);
				PlaceObjectByHash(2975320548, -2317.79, 3030.29, 39.6222, 25.0799, -6.23256e-005, -120.09, -1, 8);
				PlaceObjectByHash(2975320548, -2313.74, 3027.94, 42.9228, 25.7399, -8.62311e-005, -120.09, -1, 8);
				PlaceObjectByHash(2975320548, -2309.83, 3025.69, 46.2289, 27.06, -8.2816e-005, -120.09, -1, 8);
				PlaceObjectByHash(2975320548, -2306.07, 3023.49, 49.5919, 29.0399, -0.000116113, -120.09, -1, 8);
				PlaceObjectByHash(2975320548, -2283.14, 3009.97, 44.7284, 14.12, -2.04906e-005, 60.0397, -1, 8);
				PlaceObjectByHash(2975320548, -2287.5, 3012.47, 46.9591, 13.6, 0.680011, 60.0397, -1, 8);
				PlaceObjectByHash(2975320548, -2302.26, 3021.28, 53.174, 29.6999, -0.000100745, -120.09, -1, 8);
				PlaceObjectByHash(2975320548, -2292.06, 3015.11, 49.2546, 13.6, 1.53679e-005, 60.0397, -1, 8);
				PlaceObjectByHash(2975320548, -2298.56, 3019.12, 56.7472, 30.36, -8.79386e-005, -120.09, -1, 8);
				PlaceObjectByHash(2052512905, -2294.52, 3015.08, 58.6366, 82.6616, 0.00430302, -31.2919, -1, 8);
				PlaceObjectByHash(2052512905, -2293.13, 3017.4, 58.6822, 80.9428, 0.00560716, 149.187, -1, 8);
				PlaceObjectByHash(2787492567, -2293.66, 3016.58, 31.8318, -90, 0.0833042, 109.919, -1, 8);
				PlaceObjectByHash(3213433054, -2202.78, 2963.39, 32.8003, 0, -0, -120.04, -1, 8);
				PlaceObjectByHash(3213433054, -2199.53, 2961.53, 34.17, -40.5599, -2.56132e-006, 59.8803, -1, 8);
				PlaceObjectByHash(3681122061, -2137.1, 2904.97, 32.8327, 16.8, -1.10991e-005, -141.061, -1, 8);
				PlaceObjectByHash(3681122061, -2132.27, 2897.94, 34.4465, 16.8, -14, -141.061, -1, 8);
				PlaceObjectByHash(3681122061, -2127.12, 2890.88, 36.4432, 17.92, -29.68, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2119.98, 2885.33, 38.8379, 17.92, -29.68, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2113.02, 2880, 41.2705, 17.92, -29.68, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2085.18, 2857.71, 49.9177, 19.04, -43.12, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2078.1, 2852.44, 51.662, 19.0399, -50.4001, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2092.05, 2863.54, 48.2285, 17.92, -34.16, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2098.91, 2869.18, 46.2053, 17.92, -29.68, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2105.97, 2874.59, 43.7379, 17.92, -29.68, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2070.42, 2847.69, 53.5814, 19.0399, -50.4001, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2062.85, 2843.01, 55.4739, 19.0399, -50.4001, -136.581, -1, 8);
				PlaceObjectByHash(3681122061, -2055.32, 2838.69, 56.5097, 17.7868, -43.8868, -131.905, -1, 8);
				PlaceObjectByHash(3681122061, -2047.61, 2834.88, 58.9097, 26.1867, -43.8868, -131.905, -1, 8);
				PlaceObjectByHash(3681122061, -2039.74, 2832.2, 62.2769, 38.5067, -45.5668, -131.905, -1, 8);
				PlaceObjectByHash(3681122061, -1996.98, 2830.2, 48.384, 0.202822, -14.4337, -105.503, -1, 8);
				PlaceObjectByHash(3681122061, -1996.42, 2832.89, 59.0601, -179.433, 12.3451, 76.9258, -1, 8);
				PlaceObjectByHash(3213433054, -1951.86, 2849.63, 34.5146, -47.5199, -7.59859e-005, 59.6261, -1, 8);
				PlaceObjectByHash(3213433054, -1950.16, 2852.52, 34.5146, -47.5199, -7.59859e-005, 59.6261, -1, 8);
				PlaceObjectByHash(3213433054, -1953.57, 2854.49, 32.8004, 0, -0, -120.091, -1, 8);
				PlaceObjectByHash(3213433054, -1955.25, 2851.59, 32.8004, 0, -0, -120.091, -1, 8);
				PlaceObjectByHash(4111834409, -1960.72, 2857.38, 31.7305, 0, -0, -118.505, -1, 8);
				PlaceObjectByHash(4109455646, -2144, 2967.21, 36.0606, 9.35852, -0.00134085, 59.8371, -1, 8);
				PlaceObjectByHash(4109455646, -2139.63, 2964.67, 33.9985, 5.84852, -0.0013321, 59.8371, -1, 8);
				PlaceObjectByHash(4109455646, -2135.45, 2962.3, 32.4604, 0, 0, 60.4792, -1, 8);
				PlaceObjectByHash(2975320548, -2193.23, 2995.21, 35.0684, 11.6996, -0.00262322, -119.238, -1, 8);
				PlaceObjectByHash(2975320548, -2197.74, 2997.74, 32.8074, 15.2099, 2.04906e-005, -119.328, -1, 8);
				PlaceObjectByHash(3213433054, -2246.82, 3026.19, 33.0318, 0.0331696, 0.0056356, 58.6423, -1, 8);
				PlaceObjectByHash(3213433054, -2256.38, 3032.02, 35.4343, 6.5707, 0.0279573, 58.7685, -1, 8);
				PlaceObjectByHash(3213433054, -2265.19, 3037.37, 38.408, 10.1262, 0.0254109, 58.7585, -1, 8);
				PlaceObjectByHash(3213433054, -2273.45, 3042.38, 40.214, 8.95404, -0.00182451, 58.7729, -1, 8);
				PlaceObjectByHash(3213433054, -2281.36, 3047.19, 42.7382, 8.89319, 0.151422, 58.8279, -1, 8);
				PlaceObjectByHash(3213433054, -2289.41, 3052.05, 46.2871, 13.2, 0.000150264, 58.7642, -1, 8);
				PlaceObjectByHash(3213433054, -2397.86, 3114.2, 32.8449, 0, 0, 60.2049, -1, 8);
				PlaceObjectByHash(3213433054, -2402.38, 3116.77, 34.7648, 0, 0, 60.2049, -1, 8);
				PlaceObjectByHash(2475986526, -2394.65, 3118.07, 32.5452, 0, 0, 56.6241, -1, 8);
				PlaceObjectByHash(2475986526, -2397.73, 3120.09, 34.1452, 2.04, -1.38738e-006, 56.6241, -1, 8);
				PlaceObjectByHash(2475986526, -2401.78, 3122.77, 36.6227, 8.16, 1.28066e-006, 56.6239, -1, 8);
				PlaceObjectByHash(2475986526, -2405.48, 3125.2, 39.5571, 14.28, 1.02453e-005, 56.6239, -1, 8);
				PlaceObjectByHash(2475986526, -2409.12, 3127.6, 43.2064, 20.4, -2.39057e-005, 56.6239, -1, 8);
				PlaceObjectByHash(2475986526, -2412.29, 3129.71, 46.9494, 24.4781, -1.43125, 56.2632, -1, 8);
				PlaceObjectByHash(2475986526, -2415.18, 3131.49, 51.529, 38.3931, -3.70399, 55.299, -1, 8);
				PlaceObjectByHash(2475986526, -2416.96, 3132.28, 56.2986, 54.0331, -3.70398, 53.2589, -1, 8);
				PlaceObjectByHash(2475986526, -2417.37, 3132.16, 61.6124, 73.753, -3.70394, 53.2588, -1, 8);
				PlaceObjectByHash(2475986526, -2416.48, 3131.04, 66.996, 90.9129, -3.70395, 53.2587, -1, 8);
				PlaceObjectByHash(2475986526, -2414.88, 3129.5, 70.998, 104.113, -3.70383, 50.6186, -1, 8);
				PlaceObjectByHash(2475986526, -2412.46, 3127.2, 74.61, 116.653, -3.70392, 50.6185, -1, 8);
				PlaceObjectByHash(2475986526, -2409.58, 3124.71, 77.6119, 121.273, -3.70395, 50.6185, -1, 8);
				PlaceObjectByHash(2475986526, -2406.75, 3122.18, 80.0586, 127.213, -3.70391, 50.6184, -1, 8);
				PlaceObjectByHash(2475986526, -2403.38, 3119.23, 82.2502, 135.793, -3.70396, 50.6185, -1, 8);
				PlaceObjectByHash(2475986526, -2369.71, 3092.81, 68.2807, -146.327, -3.7039, 50.6183, -1, 8);
				PlaceObjectByHash(2475986526, -2367.45, 3091.4, 63.3347, -134.447, -3.70392, 50.6182, -1, 8);
				PlaceObjectByHash(2475986526, -2366, 3090.66, 58.0814, -123.887, -3.7039, 50.6182, -1, 8);
				PlaceObjectByHash(2475986526, -2365.38, 3090.57, 53.1623, -112.007, -3.70391, 50.6182, -1, 8);
				PlaceObjectByHash(2475986526, -2365.62, 3091.18, 48.0172, -99.4666, -3.70393, 50.6181, -1, 8);
				PlaceObjectByHash(2475986526, -2366.77, 3092.54, 43.04, -86.2661, -3.70399, 50.6181, -1, 8);
				PlaceObjectByHash(2475986526, -2368.73, 3094.52, 38.5669, -74.386, -3.70392, 50.6181, -1, 8);
				PlaceObjectByHash(2475986526, -2371.25, 3096.8, 35.0692, -59.206, -3.70384, 55.2379, -1, 8);
				PlaceObjectByHash(2475986526, -2375.18, 3099.61, 32.3997, -42.0459, -3.70387, 57.2179, -1, 8);
				PlaceObjectByHash(2475986526, -2395.69, 3112.77, 84.6355, 152.292, -3.70389, 50.6184, -1, 8);
				PlaceObjectByHash(2475986526, -2391.54, 3109.37, 84.6603, 162.192, -3.70393, 50.6184, -1, 8);
				PlaceObjectByHash(2475986526, -2387.13, 3105.84, 83.6595, 172.752, -3.70391, 50.6184, -1, 8);
				PlaceObjectByHash(2475986526, -2382.97, 3102.56, 81.8101, -179.988, -3.70391, 50.6184, -1, 8);
				PlaceObjectByHash(2475986526, -2372.52, 3094.76, 72.6855, -154.907, -3.70391, 50.6183, -1, 8);
				PlaceObjectByHash(2475986526, -2379.11, 3099.59, 79.371, -172.728, -3.70391, 50.6183, -1, 8);
				PlaceObjectByHash(2475986526, -2375.46, 3096.85, 76.1692, -162.168, -3.70388, 50.6182, -1, 8);
				PlaceObjectByHash(2475986526, -2399.8, 3116.19, 83.7512, 143.712, -3.70387, 50.6184, -1, 8);
				PlaceObjectByHash(3213433054, -2510.73, 3180.4, 32.8111, 0, 0, 59.4291, -1, 8);
				PlaceObjectByHash(209943352, -2302.92, 3059.95, 50.2208, 76.8397, -0.679965, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2298.84, 3057.5, 48.7042, 71.3997, -0.679954, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2290.6, 3052.58, 47.3498, 84.3198, -0.679946, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2290.6, 3052.58, 47.3498, 84.3198, -0.679946, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2294.73, 3055.05, 47.6692, 76.8398, -0.680059, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2533.2, 3193.91, 37.3948, 0, -0, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2533.2, 3193.91, 37.3948, 0, -0, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2425.58, 3091.36, 36.493, 0, -0, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2425.58, 3091.36, 36.493, 0, -0, -120.716, -1, 8);
				PlaceObjectByHash(209943352, -2293.7, 3012.65, 55.3685, -89.7587, -0.659716, -30.2946, -1, 8);
				PlaceObjectByHash(209943352, -2293.7, 3012.65, 55.3685, -89.7587, -0.659716, -30.2946, -1, 8);
			}
		}
		else if (mapIndex == 9) {



			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Halfpipe Fun Track");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-1003, -2916, 14);
			}
			if (load) {
				PlaceObjectByHash(3681122061, -1018.78, -2937.26, 12.9646, 0, 0, -30.3132, -1, 9);
				PlaceObjectByHash(3681122061, -1023.38, -2945.17, 12.9646, 0, 0, -30.3132, -1, 9);
				PlaceObjectByHash(3681122061, -1028.02, -2953.13, 12.9646, 0, 0, -30.3132, -1, 9);
				PlaceObjectByHash(3681122061, -1032.66, -2961.06, 12.9646, 0, 0, -30.3132, -1, 9);
				PlaceObjectByHash(3681122061, -1037.32, -2969.04, 12.9646, 0, 0, -30.3132, -1, 9);
				PlaceObjectByHash(3681122061, -1041.95, -2976.96, 12.9646, 0, 0, -30.3132, -1, 9);
				PlaceObjectByHash(3681122061, -1046.18, -2984.19, 12.9646, 0, 0, -30.3132, -1, 9);
				PlaceObjectByHash(3681122061, -1050.78, -2992.12, 12.9646, 0, 0, -29.8732, -1, 9);
				PlaceObjectByHash(3681122061, -1053.22, -2998.13, 12.9646, 0, 0, -14.2534, -1, 9);
				PlaceObjectByHash(3681122061, -1054.14, -3005.28, 12.9646, 0, 0, -0.613478, -1, 9);
				PlaceObjectByHash(3681122061, -1053.45, -3012.85, 12.9646, 0, 0, 11.4866, -1, 9);
				PlaceObjectByHash(3681122061, -1051.19, -3020.08, 12.9646, 0, 0, 23.3667, -1, 9);
				PlaceObjectByHash(3681122061, -1047.43, -3026.73, 12.9646, 0, 0, 35.2469, -1, 9);
				PlaceObjectByHash(3681122061, -1042.42, -3032.37, 12.9646, 0, 0, 47.7871, -1, 9);
				PlaceObjectByHash(3681122061, -1037.1, -3038.16, 12.9646, 0, 0, 37.2273, -1, 9);
				PlaceObjectByHash(3681122061, -1033.11, -3044.75, 12.9646, 0, 0, 25.5675, -1, 9);
				PlaceObjectByHash(3681122061, -1030.43, -3052.11, 12.9646, 0, 0, 14.5676, -1, 9);
				PlaceObjectByHash(3681122061, -1029.18, -3059.85, 12.9646, 0, 0, 4.00757, -1, 9);
				PlaceObjectByHash(3681122061, -1029.37, -3067.7, 12.9646, 0, 0, -6.55247, -1, 9);
				PlaceObjectByHash(3681122061, -1031, -3075.33, 12.9646, 0, 0, -17.5525, -1, 9);
				PlaceObjectByHash(3681122061, -1034.09, -3082.35, 12.9646, 0, 0, -29.6525, -1, 9);
				PlaceObjectByHash(3681122061, -1038.6, -3088.77, 12.9646, 0, 0, -40.2127, -1, 9);
				PlaceObjectByHash(3681122061, -1044.19, -3094.15, 12.9646, 0, 0, -51.653, -1, 9);
				PlaceObjectByHash(3681122061, -1050.65, -3098.2, 12.9646, 0, 0, -63.7531, -1, 9);
				PlaceObjectByHash(3681122061, -1057.89, -3100.91, 12.9646, 0, 0, -75.1935, -1, 9);
				PlaceObjectByHash(3681122061, -1065.18, -3101.87, 12.9646, 0, 0, -89.7139, -1, 9);
				PlaceObjectByHash(3681122061, -1073.03, -3101.2, 12.9646, 0, -0, -100.054, -1, 9);
				PlaceObjectByHash(3681122061, -1080.63, -3099.11, 12.9646, 0, -0, -110.615, -1, 9);
				PlaceObjectByHash(3681122061, -1087.92, -3095.65, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1095.95, -3091.03, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1104.01, -3086.4, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1112.04, -3081.79, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1120.04, -3077.19, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1128.1, -3072.56, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1136.15, -3067.93, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1144.2, -3063.31, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1152.22, -3058.7, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1160.24, -3054.09, 12.9646, 0, -0, -119.855, -1, 9);
				PlaceObjectByHash(3681122061, -1168.22, -3049.48, 12.9646, 0, -0, -120.295, -1, 9);
				PlaceObjectByHash(3681122061, -1176.21, -3044.8, 12.9646, 0, -0, -120.295, -1, 9);
				PlaceObjectByHash(3681122061, -1183.28, -3040.14, 12.9646, 0, -0, -126.455, -1, 9);
				PlaceObjectByHash(3681122061, -1189.23, -3034.89, 12.9646, 0, -0, -136.356, -1, 9);
				PlaceObjectByHash(3681122061, -1193.86, -3028.84, 12.9646, 0, -0, -148.677, -1, 9);
				PlaceObjectByHash(3681122061, -1197.2, -3021.86, 12.9646, 0, -0, -159.898, -1, 9);
				PlaceObjectByHash(3681122061, -1198.78, -3014.77, 12.9646, 0, -0, -174.639, -1, 9);
				PlaceObjectByHash(3681122061, -1198.72, -3007.04, 12.9646, 0, -0, 173.701, -1, 9);
				PlaceObjectByHash(3681122061, -1197, -2999.97, 12.9646, 0, -0, 158.962, -1, 9);
				PlaceObjectByHash(3681122061, -1193.5, -2993.3, 12.9646, 0, -0, 145.982, -1, 9);
				PlaceObjectByHash(3681122061, -1188.51, -2987.1, 12.9646, 0, -0, 136.083, -1, 9);
				PlaceObjectByHash(3681122061, -1182.5, -2981.85, 12.9646, 0, -0, 126.183, -1, 9);
				PlaceObjectByHash(3681122061, -1175.98, -2978.23, 12.9646, 0, -0, 112.104, -1, 9);
				PlaceObjectByHash(3681122061, -1168.67, -2976.15, 12.9646, 0, -0, 99.7843, -1, 9);
				PlaceObjectByHash(3681122061, -1160.82, -2975.53, 12.9646, 0, 0, 89.4449, -1, 9);
				PlaceObjectByHash(3681122061, -1152.93, -2976.29, 12.9646, 0, 0, 79.5455, -1, 9);
				PlaceObjectByHash(3681122061, -1145.21, -2978.39, 12.9646, 0, 0, 70.0859, -1, 9);
				PlaceObjectByHash(3681122061, -1138.14, -2981.75, 12.9646, 0, 0, 59.0863, -1, 9);
				PlaceObjectByHash(3681122061, -1130.27, -2986.43, 12.9646, 0, 0, 59.0863, -1, 9);
				PlaceObjectByHash(3681122061, -1122.46, -2991.09, 12.9646, 0, 0, 59.0863, -1, 9);
				PlaceObjectByHash(3681122061, -1115.12, -2994.75, 12.9646, 0, 0, 67.1435, -1, 9);
				PlaceObjectByHash(3681122061, -1107.63, -2997.13, 12.9646, 0, 0, 76.9913, -1, 9);
				PlaceObjectByHash(3681122061, -1099.8, -2998.14, 12.9646, 0, 0, 86.8389, -1, 9);
				PlaceObjectByHash(3681122061, -1091.94, -2997.76, 12.9646, 0, -0, 97.5819, -1, 9);
				PlaceObjectByHash(3681122061, -1084.47, -2995.95, 12.9646, 0, -0, 108.325, -1, 9);
				PlaceObjectByHash(3681122061, -1077.64, -2992.78, 12.9646, 0, -0, 119.963, -1, 9);
				PlaceObjectByHash(3681122061, -1077.64, -2992.78, 12.9646, 0, -0, 119.963, -1, 9);
				PlaceObjectByHash(3681122061, -1071.68, -2988.3, 12.9646, 0, -0, 132.496, -1, 9);
				PlaceObjectByHash(3681122061, -1066.33, -2982.53, 12.9646, 0, -0, 141.449, -1, 9);
				PlaceObjectByHash(3681122061, -1053.01, -2960.01, 12.9646, 0, 0, -28.5532, -1, 9);
				PlaceObjectByHash(3681122061, -1048.58, -2951.88, 12.9646, 0, 0, -28.5532, -1, 9);
				PlaceObjectByHash(3681122061, -1044.16, -2943.76, 12.9646, 0, 0, -28.5532, -1, 9);
				PlaceObjectByHash(3681122061, -1039.74, -2935.64, 12.9646, 0, 0, -28.5532, -1, 9);
				PlaceObjectByHash(3681122061, -1035.5, -2927.86, 12.9646, 0, 0, -28.5532, -1, 9);
				PlaceObjectByHash(3608473212, -1063.23, -2993.67, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1067.37, -2998.06, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1063.7, -2994.67, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1064.21, -2995.73, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1064.71, -2996.8, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1065.21, -2997.84, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1067.1, -2999.45, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1065.43, -3000.24, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1068.29, -3000.06, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1065.92, -3001.18, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1066.44, -3002.28, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1065.35, -3002.77, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1065.6, -3003.96, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1066.11, -3005.04, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1064.21, -2999.56, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1069.53, -3000.82, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1070.75, -3001.52, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1068.76, -3002.46, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1069.29, -3003.54, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1068.44, -3005.23, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1071.58, -3003.74, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(2475986526, -1072.98, -3006.81, 16.0846, 0, 0, -26.0348, -1, 9);
				PlaceObjectByHash(3608473212, -1072.44, -3002.05, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(3608473212, -1073.73, -3002.72, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(2475986526, -1076.54, -3005.07, 16.0846, 0, 0, -26.0348, -1, 9);
				PlaceObjectByHash(2475986526, -1078.92, -3009.92, 13.7046, 0, 0, -26.0348, -1, 9);
				PlaceObjectByHash(2475986526, -1075.31, -3011.69, 13.7046, 0, 0, -26.0348, -1, 9);
				PlaceObjectByHash(2475986526, -1069.34, -3008.59, 16.0846, 0, 0, -26.0348, -1, 9);
				PlaceObjectByHash(2475986526, -1071.71, -3013.45, 13.7046, 0, 0, -26.0348, -1, 9);
				PlaceObjectByHash(2475986526, -1068.14, -3015.2, 13.7046, 0, 0, -26.0348, -1, 9);
				PlaceObjectByHash(3608473212, -1065.36, -3006.66, 15.3449, 0, 0, -25.1145, -1, 9);
				PlaceObjectByHash(2475986526, -1065.75, -3010.35, 16.0846, 0, 0, -26.0348, -1, 9);
			}
		}
		else if (mapIndex == 10) {



			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Airport Loop");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-1074, -3201, 13);
			}
			if (load) {
				PlaceObjectByHash(3966705493, -1041.89, -3219.51, 10.1797, -2.43331, 5.32208, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1037.79, -3221.47, 10.3641, -2.43331, 5.32208, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1034.16, -3223.3, 10.5366, -2.43331, 5.32208, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1036.76, -3219.45, 10.1526, -2.43331, 5.32208, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1033.12, -3221.28, 10.3251, -2.43331, 5.32208, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1029.37, -3225.6, 11.1956, -11.6033, 5.32207, 62.9335, -1, 10);
				PlaceObjectByHash(3966705493, -1028.33, -3223.58, 10.9842, -11.6033, 5.32207, 62.9335, -1, 10);
				PlaceObjectByHash(3966705493, -1024.27, -3225.54, 12.1104, -18.1533, 5.32205, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1025.44, -3227.83, 12.3497, -18.1533, 5.32205, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1020.36, -3230.06, 15.7972, -40.4234, 5.32214, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1019.22, -3227.83, 15.5634, -40.4234, 5.32214, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1014.85, -3229.56, 20.4393, -50.9034, 5.3221, 62.9337, -1, 10);
				PlaceObjectByHash(3966705493, -1016.07, -3231.95, 20.6898, -50.9034, 5.3221, 62.9337, -1, 10);
				PlaceObjectByHash(3966705493, -1012.88, -3232.96, 26.0664, -64.0034, 5.32209, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1011.63, -3230.51, 25.8104, -64.0034, 5.32209, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1009.27, -3231.06, 32.0819, -73.1735, 5.32204, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1010.52, -3233.51, 32.3379, -73.1735, 5.32214, 62.9337, -1, 10);
				PlaceObjectByHash(3966705493, -1009.36, -3233.48, 38.2311, -83.6535, 5.32208, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1008.09, -3230.98, 37.9695, -83.6535, 5.32208, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1007.71, -3230.43, 44.185, -92.8235, 6.63212, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1009.05, -3228.93, 49.9682, -119.024, 6.63217, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1008.93, -3232.81, 44.4969, -92.8235, 6.63212, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1010.27, -3231.31, 50.2801, -119.024, 6.63217, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1011.96, -3226.91, 54.0691, -142.604, 6.6321, 62.9337, -1, 10);
				PlaceObjectByHash(3966705493, -1013.1, -3229.14, 54.3602, -142.604, 6.6321, 62.9337, -1, 10);
				PlaceObjectByHash(3966705493, -1017.49, -3226.51, 57.2125, -159.634, 6.63211, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1022.14, -3223.91, 58.9186, -168.804, 6.63213, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1035.97, -3216.05, 58.7162, 155.826, 1.39214, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1031.37, -3218.71, 60.1775, 176.786, 6.63213, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1026.33, -3221.63, 59.8766, -168.804, 6.63212, 60.3136, -1, 10);
				PlaceObjectByHash(3966705493, -1020.98, -3221.63, 58.6206, -168.804, 6.63213, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1024.97, -3219.25, 59.5578, -168.804, 6.63212, 60.3136, -1, 10);
				PlaceObjectByHash(3966705493, -1016.33, -3224.24, 56.9145, -159.634, 6.63211, 62.9336, -1, 10);
				PlaceObjectByHash(3966705493, -1029.99, -3216.28, 59.8517, 176.786, 6.63213, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1029.99, -3216.28, 59.8517, 176.786, 6.63213, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1034.73, -3213.86, 58.655, 155.826, 1.39214, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1039.18, -3211.42, 55.2255, 138.796, 1.39206, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1039.18, -3211.42, 55.2255, 138.796, 1.39206, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1040.42, -3213.61, 55.2867, 138.796, 1.39206, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1044.34, -3211.51, 50.6082, 128.316, 1.39213, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1043.09, -3209.33, 50.547, 128.316, 1.39213, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1046.16, -3207.74, 45.1535, 117.837, 1.39215, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1048.17, -3206.74, 39.6252, 104.737, 1.39214, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1048.92, -3206.44, 33.1586, 87.6005, 0.0914728, 60.6227, -1, 10);
				PlaceObjectByHash(3966705493, -1048.18, -3206.88, 26.5446, 77.3408, 0.0913896, 60.6229, -1, 10);
				PlaceObjectByHash(3966705493, -1049.44, -3209.13, 26.5487, 77.3407, 0.0913427, 60.6228, -1, 10);
				PlaceObjectByHash(3966705493, -1047.3, -3210.37, 21.3947, 56.6411, 0.0914017, 58.823, -1, 10);
				PlaceObjectByHash(3966705493, -1047.3, -3210.37, 21.3947, 56.6411, 0.0914017, 58.823, -1, 10);
				PlaceObjectByHash(3966705493, -1045.93, -3208.12, 21.3905, 56.6411, 0.0914017, 58.823, -1, 10);
				PlaceObjectByHash(3966705493, -1042.61, -3210.12, 16.8766, 42.1517, 0.0913785, 58.8231, -1, 10);
				PlaceObjectByHash(3966705493, -1038.64, -3212.63, 13.6141, 28.2018, 0.0914187, 58.8231, -1, 10);
				PlaceObjectByHash(3966705493, -1039.97, -3214.83, 13.6182, 28.2018, 0.0914187, 58.8231, -1, 10);
				PlaceObjectByHash(3966705493, -1034.82, -3217.71, 11.1985, 16.4116, 0.0913871, 69.303, -1, 10);
				PlaceObjectByHash(3966705493, -1033.53, -3215.55, 11.1081, 16.4117, 0.0913619, 66.683, -1, 10);
				PlaceObjectByHash(3966705493, -1043.97, -3212.37, 16.8808, 42.1517, 0.0913785, 58.8231, -1, 10);
				PlaceObjectByHash(3966705493, -1050.19, -3208.69, 33.1627, 87.6005, 0.0914728, 60.6227, -1, 10);
				PlaceObjectByHash(3966705493, -1049.45, -3208.98, 39.6879, 104.737, 1.39214, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1047.44, -3209.98, 45.2161, 117.837, 1.39215, 60.3137, -1, 10);
				PlaceObjectByHash(3966705493, -1047.44, -3209.98, 45.2161, 117.837, 1.39215, 60.3137, -1, 10);
			}
		}

		else if (mapIndex == 11) {



			bool load = 0, unload = 0, teleport = 0;
			AddTitle("Maze Bank Mega Ramp");
			AddOption("Teleport", teleport);
			AddOption("Load", load);
			if (teleport) {
				TeleportMeTo(-74.94243f, -818.63446f, 326.174347f);
			}
			if (load) {
				PlaceObjectByHash(3681122061, -82.9657, -818.944, 325.175, 0, -0, 91.03, -1, 11);
				PlaceObjectByHash(3681122061, -91.0941, -819.089, 322.355, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -98.36, -819.224, 316.632, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -105.626, -819.358, 310.91, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -112.892, -819.492, 305.187, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -120.158, -819.626, 299.464, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -127.424, -819.761, 293.741, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -134.69, -819.895, 288.018, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -141.956, -820.029, 282.296, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -149.222, -820.163, 276.573, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -156.487, -820.298, 270.85, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -163.753, -820.432, 265.127, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -171.019, -820.566, 259.404, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -178.285, -820.701, 253.682, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -185.551, -820.835, 247.959, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -192.817, -820.969, 242.236, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -200.083, -821.103, 236.513, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -207.349, -821.238, 230.79, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -214.615, -821.372, 225.068, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -221.881, -821.506, 219.345, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -229.147, -821.641, 213.622, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -236.413, -821.775, 207.899, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -243.679, -821.909, 202.176, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -250.945, -822.043, 196.453, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -258.21, -822.178, 190.731, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -265.476, -822.312, 185.008, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -287.274, -822.715, 167.839, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -280.008, -822.58, 173.562, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -272.742, -822.446, 179.285, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -294.54, -822.849, 162.117, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -301.806, -822.983, 156.394, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -309.072, -823.118, 150.671, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -316.338, -823.252, 144.948, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -323.604, -823.386, 139.225, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -330.87, -823.52, 133.503, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -338.136, -823.655, 127.78, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -345.402, -823.789, 122.057, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -352.668, -823.923, 116.334, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -359.934, -824.057, 110.611, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -367.199, -824.192, 104.889, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -374.465, -824.326, 99.1657, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -381.731, -824.46, 93.4429, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -388.997, -824.595, 87.7201, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -396.263, -824.729, 81.9973, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -403.529, -824.863, 76.2745, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -411.479, -825.022, 68.7971, -19.78, -3.43377e-005, 91.1094, -1, 11);
				PlaceObjectByHash(3681122061, -410.795, -824.997, 70.5517, -38.2199, 0.00787841, 91.0529, -1, 11);
				PlaceObjectByHash(3681122061, -411.96, -825.029, 69.097, -27.6, 1.15259e-005, 91.1095, -1, 11);
				PlaceObjectByHash(3681122061, -412.719, -825.046, 67.8516, -10.58, -2.92151e-006, 91.1095, -1, 11);
				PlaceObjectByHash(3681122061, -413.903, -825.068, 67.2075, -3.21999, 5.66959e-007, 91.1095, -1, 11);
				PlaceObjectByHash(3681122061, -415.378, -825.099, 66.7734, 3.68002, -4.58236e-006, 91.1095, -1, 11);
				PlaceObjectByHash(3681122061, -416.883, -825.126, 66.57, 9.66002, -8.44435e-006, 91.1096, -1, 11);
				PlaceObjectByHash(3681122061, -418.526, -825.157, 66.5571, 15.64, -1.80093e-005, 91.1095, -1, 11);
				PlaceObjectByHash(3681122061, -419.945, -825.184, 66.6727, 20.7001, 8.69782e-006, 91.1094, -1, 11);
				PlaceObjectByHash(3681122061, -421.727, -825.218, 67.0936, 25.7601, -2.09975e-005, 91.1095, -1, 11);
				PlaceObjectByHash(3681122061, -422.006, -825.234, 66.966, 30.8199, 0.114757, 90.6829, -1, 11);
				PlaceObjectByHash(3681122061, -429.913, -825.328, 71.6856, 30.8199, 0.114757, 90.6829, -1, 11);
			}
		}
	}

	void IPL() {

		bool ufo1 = 0, cship = 0, nyankton = 0, lfact = 0, Yacht1 = 0, dhosp = 0, cbell = 0, Mmorg = 0, hcarrier = 0, ufo2 = 0, trainCrash = 0, planeCrash = 0;
		AddTitle("IPL");
		AddOption("North Yankton", nyankton);
		AddOption("UFO Ft. Zancudo", ufo1);
		AddOption("UFO Desert", ufo2);
		AddOption("Heist Carrier", hcarrier);
		AddOption("Cluckin Bell", cbell);
		AddOption("Morgue", Mmorg);
		AddOption("Cargo Ship", cship);
		AddOption("Destroyed hosptal", dhosp);
		AddOption("Heist Yacht", Yacht1);
		AddOption("Lester's Factory", lfact);

		//code
		if (nyankton) {
			STREAMING::REQUEST_IPL("plg_01");
			STREAMING::REQUEST_IPL("prologue01");
			STREAMING::REQUEST_IPL("prologue01_lod");
			STREAMING::REQUEST_IPL("prologue01c");
			STREAMING::REQUEST_IPL("prologue01c_lod");
			STREAMING::REQUEST_IPL("prologue01d");
			STREAMING::REQUEST_IPL("prologue01d_lod");
			STREAMING::REQUEST_IPL("prologue01e");
			STREAMING::REQUEST_IPL("prologue01e_lod");
			STREAMING::REQUEST_IPL("prologue01f");
			STREAMING::REQUEST_IPL("prologue01f_lod");
			STREAMING::REQUEST_IPL("prologue01g");
			STREAMING::REQUEST_IPL("prologue01h");
			STREAMING::REQUEST_IPL("prologue01h_lod");
			STREAMING::REQUEST_IPL("prologue01i");
			STREAMING::REQUEST_IPL("prologue01i_lod");
			STREAMING::REQUEST_IPL("prologue01j");
			STREAMING::REQUEST_IPL("prologue01j_lod");
			STREAMING::REQUEST_IPL("prologue01k");
			STREAMING::REQUEST_IPL("prologue01k_lod");
			STREAMING::REQUEST_IPL("prologue01z");
			STREAMING::REQUEST_IPL("prologue01z_lod");
			STREAMING::REQUEST_IPL("plg_02");
			STREAMING::REQUEST_IPL("prologue02");
			STREAMING::REQUEST_IPL("prologue02_lod");
			STREAMING::REQUEST_IPL("plg_03");
			STREAMING::REQUEST_IPL("prologue03");
			STREAMING::REQUEST_IPL("prologue03_lod");
			STREAMING::REQUEST_IPL("prologue03b");
			STREAMING::REQUEST_IPL("prologue03b_lod");
			STREAMING::REQUEST_IPL("prologue03_grv_dug");
			STREAMING::REQUEST_IPL("prologue03_grv_dug_lod");
			STREAMING::REQUEST_IPL("prologue_grv_torch");
			STREAMING::REQUEST_IPL("plg_04");
			STREAMING::REQUEST_IPL("prologue04");
			STREAMING::REQUEST_IPL("prologue04_lod");
			STREAMING::REQUEST_IPL("prologue04b");
			STREAMING::REQUEST_IPL("prologue04b_lod");
			STREAMING::REQUEST_IPL("prologue04_cover");
			STREAMING::REQUEST_IPL("des_protree_end");
			STREAMING::REQUEST_IPL("des_protree_start");
			STREAMING::REQUEST_IPL("des_protree_start_lod");
			STREAMING::REQUEST_IPL("plg_05");
			STREAMING::REQUEST_IPL("prologue05");
			STREAMING::REQUEST_IPL("prologue05_lod");
			STREAMING::REQUEST_IPL("prologue05b");
			STREAMING::REQUEST_IPL("prologue05b_lod");
			STREAMING::REQUEST_IPL("plg_06");
			STREAMING::REQUEST_IPL("prologue06");
			STREAMING::REQUEST_IPL("prologue06_lod");
			STREAMING::REQUEST_IPL("prologue06b");
			STREAMING::REQUEST_IPL("prologue06b_lod");
			STREAMING::REQUEST_IPL("prologue06_int");
			STREAMING::REQUEST_IPL("prologue06_int_lod");
			STREAMING::REQUEST_IPL("prologue06_pannel");
			STREAMING::REQUEST_IPL("prologue06_pannel_lod");
			STREAMING::REQUEST_IPL("prologue_m2_door");
			STREAMING::REQUEST_IPL("prologue_m2_door_lod");
			STREAMING::REQUEST_IPL("plg_occl_00");
			STREAMING::REQUEST_IPL("prologue_occl");
			STREAMING::REQUEST_IPL("plg_rd");
			STREAMING::REQUEST_IPL("prologuerd");
			STREAMING::REQUEST_IPL("prologuerdb");
			STREAMING::REQUEST_IPL("prologuerd_lod");
			STREAMING::REMOVE_IPL("prologue03_grv_cov");
			STREAMING::REMOVE_IPL("prologue03_grv_cov_lod");
			STREAMING::REMOVE_IPL("prologue03_grv_fun");

			TeleportMeTo(3360.19f, -4849.67f, 111.8f);
		}
		if (lfact) {
			STREAMING::REQUEST_IPL("id2_14_on_fire");
			TeleportMeTo(716.84f, -962.05f, 31.59f);
		}
		if (Yacht1) {
			STREAMING::REQUEST_IPL("hei_yacht_heist");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Bar");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Bedrm");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Bridge");
			STREAMING::REQUEST_IPL("hei_yacht_heist_DistantLights");
			STREAMING::REQUEST_IPL("hei_yacht_heist_enginrm");
			STREAMING::REQUEST_IPL("hei_yacht_heist_LODLights");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Lounge");
			TeleportMeTo(-2045.8f, -1031.2f, 11.9f);
		}
		if (dhosp) {
			STREAMING::REQUEST_IPL("RC12B_Destroyed");
			STREAMING::REQUEST_IPL("RC12B_HospitalInterior");
			TeleportMeTo(356.8f, -590.1f, 43.3f);
		}
		if (ufo1) {
			STREAMING::REQUEST_IPL("ufo");
			TeleportMeTo(-2051.99463, 3237.05835, 1456.97021);
		}
		if (ufo2) {
			STREAMING::REQUEST_IPL("ufo");
			TeleportMeTo(2490.47729, 3774.84351, 2414.035);
		}
		if (Mmorg) {
			STREAMING::REQUEST_IPL("Coroner_Int_on");
			TeleportMeTo(244.9f, -1374.7f, 39.5f);
		}
		if (cship) {
			STREAMING::REQUEST_IPL("cargoship");
			TeleportMeTo(-90.0f, -2365.8f, 14.3f);
		}
		if (hcarrier) {
			STREAMING::REQUEST_IPL("hei_carrier");
			STREAMING::REQUEST_IPL("hei_carrier_DistantLights");
			STREAMING::REQUEST_IPL("hei_Carrier_int1");
			STREAMING::REQUEST_IPL("hei_Carrier_int2");
			STREAMING::REQUEST_IPL("hei_Carrier_int3");
			STREAMING::REQUEST_IPL("hei_Carrier_int4");
			STREAMING::REQUEST_IPL("hei_Carrier_int5");
			STREAMING::REQUEST_IPL("hei_Carrier_int6");
			STREAMING::REQUEST_IPL("hei_carrier_LODLights");
			TeleportMeTo(-72.68752, 6253.72656, 31.08991);
		}
		if (cbell) {

			STREAMING::REMOVE_IPL("CS1_02_cf_offmission1");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission1");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission2");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission3");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission4");
			TeleportMeTo(-72.68752, 6253.72656, 31.08991);
		}
	}

	void WtfMode() {
		if (wtfMode) {
			char* dict = "rcmcollect_paperleadinout@";
			char* anim = "meditiate_idle";


			//Setup the array
			const int numElements = 10;
			const int arrSize = numElements * 2 + 2;
			const int numElementsp = 30;
			const int arrSizep = numElements * 2 + 2;
			Entity veh[arrSize];
			Entity Peds[arrSize];
			//0 index is the size of the array
			veh[0] = numElements;
			Peds[0] = numElementsp;
			int pedCount = PED::GET_PED_NEARBY_PEDS(playerPed, Peds, 1);
			int count = PED::GET_PED_NEARBY_VEHICLES(playerPed, veh);

			try {
				if (Peds != NULL) {
					for (int i = 0; i < pedCount; i++) {
						int offsettedID = i * 2 + 2;
						if (Peds[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(Peds[offsettedID]))
						{
							//Do something
							if (get_key_pressed(VK_DIVIDE)) {
								Entity mped = Peds[offsettedID];
								RequestControlOfEnt(mped);
								ENTITY::APPLY_FORCE_TO_ENTITY(mped, 1, 0, 0, 4, 0, 0, 0, 1, 0, 1, 1, 1, 1);
							}
						}
					}
				}
				if (veh != NULL)
				{
					//Simple loop to go through results
					for (int i = 0; i < count; i++)
					{
						int offsettedID = i * 2 + 2;
						//Make sure it exists
						if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
						{
							//Do something
							if (get_key_pressed(VK_DIVIDE)) {
								Entity mveh = veh[offsettedID];
								RequestControlOfEnt(mveh);
								ENTITY::APPLY_FORCE_TO_ENTITY(mveh, 1, 0, 0, 4, 0, 0, 0, 1, 0, 1, 1, 1, 1);
								STREAMING::REQUEST_ANIM_DICT(dict);
								AI::TASK_PLAY_ANIM(playerPed, dict, anim, 1, 1, -1, 16, 0, false, 0, false);
							}
						}
					}
				}
			}
			catch (...) {

			}
		}
	}
	static bool disableMobilePhone = false;

	/*void NoMobilePhone() {
	if (disableMobilePhone) {
	MOBILE::DESTROY_MOBILE_PHONE();
	}
	}*/

	void ExplodeNearbyVehicles()
	{
		if (explodeNearbyVehicles) {
			const int numElements = 10;
			const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
			int vehs[arrSize];
			//0 index is the size of the array
			vehs[0] = numElements;

			int count = PED::GET_PED_NEARBY_VEHICLES(playerPed, vehs);
			for (int i = 0; i < count; ++i)
			{
				int offsettedID = i * 2 + 2;
				if (ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
				{
					if ((vehs[offsettedID]) == PED::GET_VEHICLE_PED_IS_USING(playerPed)) continue;
					{
						if (VEHICLE::GET_VEHICLE_ENGINE_HEALTH(vehs[offsettedID]) > 1)
						{
							Vector3 nearbyPedPos = ENTITY::GET_ENTITY_COORDS(vehs[offsettedID], 0);
							FIRE::ADD_OWNED_EXPLOSION(playerPed, nearbyPedPos.x, nearbyPedPos.y, nearbyPedPos.z, 3, 2, 0, 1, 0);
						}
					}
				}
			}
		}
	}

	void MaxNearbyVehicles() {
		if (maxNearbyVehicles) {
			const int numElements = 10;
			const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
			int vehs[arrSize];
			//0 index is the size of the array
			vehs[0] = numElements;

			int count = PED::GET_PED_NEARBY_VEHICLES(playerPed, vehs);
			for (int i = 0; i < count; ++i)
			{
				int offsettedID = i * 2 + 2;
				if (ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
				{
					if ((vehs[offsettedID]) == PED::GET_VEHICLE_PED_IS_USING(playerPed)) continue;
					{
						if (VEHICLE::GET_VEHICLE_ENGINE_HEALTH(vehs[offsettedID]) > 1)
						{
							MaxThisVehicle(vehs[offsettedID]);
						}
					}
				}
			}
		}
	}

	static int newPaintTimer = -1;
	static int paintDelay = 1000; // 1 seconds
	void RainbowNearbyVehicles() {
		if (rainbowNearbyVehicles) {
			if (newPaintTimer == -1) {
				newPaintTimer = GAMEPLAY::GET_GAME_TIMER() + paintDelay; // every second, paint the vehicle
			}
			if (newPaintTimer <= GAMEPLAY::GET_GAME_TIMER()) {
				const int numElements = 10;
				const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
				int vehs[arrSize];
				//0 index is the size of the array
				vehs[0] = numElements;

				int count = PED::GET_PED_NEARBY_VEHICLES(playerPed, vehs);
				for (int i = 0; i < count; ++i)
				{
					int offsettedID = i * 2 + 2;
					if (ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
					{
						if ((vehs[offsettedID]) == PED::GET_VEHICLE_PED_IS_USING(playerPed)) continue;
						{
							if (VEHICLE::GET_VEHICLE_ENGINE_HEALTH(vehs[offsettedID]) > 1)
							{
								VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehs[offsettedID], RandomRGB(), RandomRGB(), RandomRGB());
								VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehs[offsettedID], RandomRGB(), RandomRGB(), RandomRGB());
							}
						}
					}
				}
				newPaintTimer = GAMEPLAY::GET_GAME_TIMER() + paintDelay;
			}
		}
	}

	static int newJumpTimer = -1;
	static int jumpDelay = 1600; 
	void JumpLoopNearbyVehicles() {
		if (jumpLoopNearbyVehicles) {
			if (newPaintTimer == -1) {
				newJumpTimer = GAMEPLAY::GET_GAME_TIMER() + jumpDelay; 
			}
			if (newJumpTimer <= GAMEPLAY::GET_GAME_TIMER()) {
				const int numElements = 10;
				const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
				int vehs[arrSize];
				//0 index is the size of the array
				vehs[0] = numElements;

				int count = PED::GET_PED_NEARBY_VEHICLES(playerPed, vehs);
				for (int i = 0; i < count; ++i)
				{
					int offsettedID = i * 2 + 2;
					if (ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
					{
						if ((vehs[offsettedID]) == PED::GET_VEHICLE_PED_IS_USING(playerPed)) continue;
						{
							if (VEHICLE::GET_VEHICLE_ENGINE_HEALTH(vehs[offsettedID]) > 1)
							{
								ENTITY::APPLY_FORCE_TO_ENTITY(vehs[offsettedID], 3, 0, 0, 7, 0, 0, 0, 0, 1, 1, 1, 0, 1);
							}
						}
					}
				}
				newJumpTimer = GAMEPLAY::GET_GAME_TIMER() + jumpDelay;
			}
		}
	}
	void KillNearbyPeds() {
		if (killNearbyPeds) {
			int peds[402];
			int count = PED::GET_PED_NEARBY_PEDS(playerPed, peds, -1);

			for (int i = 0; i < count; i++)
			{
				int offsettedID = i * 2 + 2;
				if (peds[offsettedID] != null && ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
				{
					RequestControlOfEnt(peds[offsettedID]);
					PED::APPLY_DAMAGE_TO_PED(peds[offsettedID], 500, true);
				}
			}
		}
	}

	void SetBlackout(bool state) {
		GRAPHICS::_SET_BLACKOUT(state);
	}

	void InfoMenu() {
		AddTitle("Menu Info");
		AddBreak("Developed by d3sk1ng");
		AddBreak("Big thanks to:");
		AddBreak("Sudo for Sudomod");
		AddBreak("~b~Bluetexon");
		AddBreak("Infamous Community");
	}
	void MiscMenu()
	{
		AddTitle("MISC");
		AddOption("ESP", null, nullFunc, SUB::ESP);
		AddOption("Protections", null, nullFunc, SUB::MENU_PROTECTIONS);
		AddOption("Screen Effects", null, nullFunc, SUB::SCREEN_EFFECTS);
		AddOption("Nearby Vehicles", null, nullFunc, SUB::NEARBY_VEHICLES);
		AddOption("Nearby Peds", null, nullFunc, SUB::NEARBY_PEDS);
		AddToggle("Mobile Radio", mobileRadio);
		AddToggle("Display Coords", displayCoords);
		//AddToggle("No Mobile Phone", disableMobilePhone);
		AddLightToggle("Blackout", isBlackout, SetBlackout);
		if (isPaidVersion) AddToggle("WTF Mode (Numpad /)", wtfMode);
		AddOption("Menu info", null, nullFunc, SUB::MENU_INFO);
	}

	void NearbyVehiclesMenu() {
		AddTitle("Nearby Vehicles");
		AddToggle("Explode Nearby Vehicles", explodeNearbyVehicles);
		AddToggle("Max Upgrade Nearby Vehicles", maxNearbyVehicles);
		AddToggle("Jump Loop Nearby Vehicles", jumpLoopNearbyVehicles);
		AddToggle("Rainbow Nearby Vehicles", rainbowNearbyVehicles);
	}

	void NearbyPedsMenu() {
		AddTitle("Nearby Peds");
		AddToggle("Kill Nearby Peds (not working atm)", killNearbyPeds);
	}

	static bool rpIncreaser = false;
	static int rpDelay = 250;
	bool waitPart = 0;
	void RPIncreaser()
	{
		if (rpIncreaser) {
			Player myPlayer = playerID;
			Ped myPed = playerPed;

			if (waitPart) {
				PLAYER::SET_PLAYER_WANTED_LEVEL(myPlayer, 5, 0);
				PLAYER::SET_PLAYER_WANTED_CENTRE_POSITION(myPlayer, 10000, 10000, -10000);
				waitPart = false;
			}
			else {
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(myPlayer, 0);
				PLAYER::CLEAR_PLAYER_WANTED_LEVEL(myPlayer);
				waitPart = true;
			}
		}
	}

	static void MaxSkills() {
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STAM"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STRN"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_LUNG"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_DRIV"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_FLY"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_SHO"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STL"), 100, 1);

		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STAM"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STRN"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_LUNG"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_DRIV"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_FLY"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_SHO"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STL"), 100, 1);
		PrintBottomLeft("Skills Maxed");
	}

	static void UnlockAllTattoos() {
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_WINS"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TDM_MVP"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_TOTALKILLS"), 500, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMATTGANGHQ"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMBBETWIN"), 50000, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINEVERYGAMEMODE"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMRACEWORLDRECHOLDER"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMFULLYMODDEDCAR"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMMOSTKILLSSURVIVE"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMKILL3ANDWINGTARACE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMKILLBOUNTY"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMREVENGEKILLSDM"), 50, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMKILLSTREAKSDM"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_HOLD_UP_SHOPS"), 20, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_LAPDANCES"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_SECURITY_CARS_ROBBED"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_RACES_WON"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_PLAYER_HEADSHOTS"), 500, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DB_PLAYER_KILLS"), 1000, 1);

		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_WINS"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TDM_MVP"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_TOTALKILLS"), 500, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMATTGANGHQ"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMBBETWIN"), 50000, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINEVERYGAMEMODE"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMRACEWORLDRECHOLDER"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMFULLYMODDEDCAR"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMMOSTKILLSSURVIVE"), 1, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMKILL3ANDWINGTARACE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMKILLBOUNTY"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMREVENGEKILLSDM"), 50, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMKILLSTREAKSDM"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_HOLD_UP_SHOPS"), 20, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_LAPDANCES"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_SECURITY_CARS_ROBBED"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_RACES_WON"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_PLAYER_HEADSHOTS"), 500, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DB_PLAYER_KILLS"), 1000, 1);
		PrintBottomLeft("~g~All Tattoos Unlocked");
	}

	static void UnlockAllCamos() {
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_1_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_2_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_3_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_4_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_5_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_6_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_7_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_8_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_9_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_10_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_11_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_12_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE2"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE3"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE4"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE5"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE6"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE7"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE8"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE9"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE10"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE11"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHASE12"), -1, 1);


		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_1_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_2_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_3_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_4_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_5_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_6_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_7_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_8_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_9_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_10_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_11_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_12_FM_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE2"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE3"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE4"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE5"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE6"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE7"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE8"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE9"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE10"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE11"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHASE12"), -1, 1);
		PrintBottomLeft("~g~All Camos Unlocked");
	}

	static void UnlockChromeRims() {
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_CAPTURES"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_DROPOFF_CAP_PACKAGES"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_KILL_CARRIER_CAPTURE"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FINISH_HEISTS"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FINISH_HEIST_SETUP_JOB"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_NIGHTVISION_KILLS, 100"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_LAST_TEAM_STANDINGS"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_ONLY_PLAYER_ALIVE_LTS"), 50, 1);
		PrintBottomLeft("~g~Chrome Rims Unlocked");
	}

	static void UnlockSpecialTShirt() {
		// UNLOCK SPECIAL T-SHIRTS
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMHORDWAVESSURVIVE"), 10, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMPICKUPDLCCRATE1ST"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_CAPTURE_DONT_DYING"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_DO_HEIST_AS_MEMBER"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_PICKUP_CAP_PACKAGES"), 100, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FINISH_HEIST_NO_DAMAGE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_GOLD_MEDAL_HEISTS"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_KILL_TEAM_YOURSELF_LTS"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_KILL_PSYCHOPATHS"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_DO_HEIST_AS_THE_LEADER"), 25, 1);
		STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_STORE_20_CAR_IN_GARAGES"), 1, 1);

		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_WIN_CAPTURES"), 25, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_DROPOFF_CAP_PACKAGES"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_KILL_CARRIER_CAPTURE"), 100, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FINISH_HEISTS"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FINISH_HEIST_SETUP_JOB"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_NIGHTVISION_KILLS, 100"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_WIN_LAST_TEAM_STANDINGS"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_ONLY_PLAYER_ALIVE_LTS"), 50, 1);
		PrintBottomLeft("~g~Special T-Shirt Unlocked");
	}

	static void UnlockAllLSC() {
		// LSC
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_RACES_WON"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_CARMOD_1_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_CARMOD_2_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_CARMOD_3_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_CARMOD_4_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_CARMOD_5_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_CARMOD_6_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_CARMOD_7_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMRALLYWONDRIVE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMRALLYWONNAV"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINSEARACE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINAIRRACE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NUMBER_TURBO_STARTS_IN_RACE"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_USJS_COMPLETED"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_RACES_FASTEST_LAP"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NUMBER_SLIPSTREAMS_IN_RACE"), 100, 1);

		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_RACES_WON"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_CARMOD_1_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_CARMOD_2_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_CARMOD_3_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_CARMOD_4_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_CARMOD_5_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_CARMOD_6_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_CARMOD_7_UNLCK"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMRALLYWONDRIVE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMRALLYWONNAV"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINSEARACE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINAIRRACE"), 1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NUMBER_TURBO_STARTS_IN_RACE"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_USJS_COMPLETED"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_RACES_FASTEST_LAP"), 50, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NUMBER_SLIPSTREAMS_IN_RACE"), 100, 1);
		PrintBottomLeft("~g~All LSC Unlocked");
	}

	static void UnlockAllHairStyles() {
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_7"), -1, 1);

		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
		PrintBottomLeft("All Hairstyles ~g~Unlocked");
	}

	static void MaxSnakesAndCompany() {
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NO_BOUGHT_YUM_SNACKS"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NO_BOUGHT_HEALTH_SNACKS"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NO_BOUGHT_EPIC_SNACKS"), 200000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NUMBER_OF_ORANGE_BOUGHT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CIGARETTES_BOUGHT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_1_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_2_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_3_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_4_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_5_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_BLUE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_BLUE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_BLUE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_BLUE"), 2000000, 1);

		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NO_BOUGHT_YUM_SNACKS"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NO_BOUGHT_HEALTH_SNACKS"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NO_BOUGHT_EPIC_SNACKS"), 200000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NUMBER_OF_ORANGE_BOUGHT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CIGARETTES_BOUGHT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_1_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_2_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_3_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_4_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_5_COUNT"), 20000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_BLUE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_BLUE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_BLUE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_WHITE"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_RED"), 2000000, 1);
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_BLUE"), 2000000, 1);
	}
	static int pedDropDelay = 800;
	void RecoveryMenu()
	{
		AddTitle("RECOVERY");
		AddOption("Max Skills", null, MaxSkills);
		AddOption("Unlock All Tattoos", null, UnlockAllTattoos);
		AddOption("Unlock All Camos", null, UnlockAllCamos);
		AddOption("Unlock Chrome Rims", null, UnlockChromeRims);
		AddOption("Unlock Special T-Shirts", null, UnlockSpecialTShirt);
		AddOption("Unlock All LSC", null, UnlockAllLSC);
		AddOption("Unlock All Hair Styles", null, UnlockAllHairStyles);
		AddOption("Max Snakes, Smokes, Armor, Fireworks", null, MaxSnakesAndCompany);
		if (isPaidVersion) AddToggle("10k Ped Drop", pedDropHack);
		if (isPaidVersion)AddIntEasy("Ped Drop Delay", pedDropDelay, pedDropDelay, 1, true, null, true, 99999, 0);
		if (isPaidVersion) AddToggle("Safe RP Increaser", rpIncreaser);
		//AddIntEasy("RP Increaser Delay", rpDelay, rpDelay, 1, 0, null, true, 99999, 80);
	}

	void SendInput(char KeyToPress)
	{
		INPUT inp[2];
		memset(inp, 0, sizeof(INPUT));
		inp[0].type = INPUT_KEYBOARD;
		inp[0].ki.dwFlags = KEYEVENTF_SCANCODE;
		inp[1] = inp[0];
		inp[1].ki.dwFlags |= KEYEVENTF_KEYUP;      // keyup event
		Sleep(100);
		inp[0].ki.wScan = KeyToPress;
		SendInput(2, inp, sizeof(INPUT));
		Sleep(90);
		inp[1].ki.wScan = KeyToPress;      // inp[0] = keydown, inp[1] = keyup
		Sleep(80);
		SendInput(2, inp, sizeof(INPUT));
	}

	void SettingsMenu()
	{
		bool settings_pos_plus = 0, settings_pos_minus = 0;

		AddTitle("SETTINGS");
		AddOption("Menu Colours", null, nullFunc, SUB::SETTINGS_COLOURS);
		AddOption("Menu Fonts", null, nullFunc, SUB::SETTINGS_FONTS);
		AddToggle("Centre Title", menu::bit_centre_title);
		AddToggle("Centre Options", menu::bit_centre_options);
		AddToggle("Centre Breaks", menu::bit_centre_breaks);
		AddNumber("Menu Position", (DWORD)menuPos / 100, 0, null, settings_pos_plus, settings_pos_minus);
		AddIntEasy("Auto Scroll Delay (ms)", scrollDelay, scrollDelay, 1, false, null, true, 60000, 30);
		AddIntEasy("Integer Scroll Speed", addIntEasyDelay, addIntEasyDelay, 1, false, null, true, 60000, 0);
		AddOption("Save All Settings", null, SaveAllIniSettings);
		AddOption("Reload Setting File", null, LoadAllIniSettings);

		if (settings_pos_plus) {
			if (menuPos < 0.68f) menuPos += 0.01f;
			return;
		}
		else if (settings_pos_minus) {
			if (menuPos > 0.0f) menuPos -= 0.01f;
			return;
		}

	}

	void AddsettingscolOption(char* text, RGBA &feature)
	{
		AddOption(text, null, nullFunc, SUB::SETTINGS_COLOURS2);

		if (menu::printingop == menu::currentop) settings_rgba = &feature;
	}

	void SettingsColours()
	{
		AddTitle("Menu Colours");
		AddsettingscolOption("Title Box", titlebox);
		AddsettingscolOption("Background", BG);
		AddsettingscolOption("Title Text", titletext);
		AddsettingscolOption("Option Text", optiontext);
		AddsettingscolOption("Selected Text", selectedtext);
		AddsettingscolOption("Option Breaks", optionbreaks);
		AddsettingscolOption("Option Count", optioncount);
		AddsettingscolOption("Selection Box", selectionhi);
		AddToggle("Rainbow", loop_RainbowBoxes);
	}
	void SettingsColours2()
	{
		bool settings_r_input = 0, settings_r_plus = 0, settings_r_minus = 0;
		int *settings_rgba2;

		AddTitle("Set Colour");
		AddNumber("Red", settings_rgba->R, 0, settings_r_input, settings_r_plus, settings_r_minus);
		AddNumber("Green", settings_rgba->G, 0, settings_r_input, settings_r_plus, settings_r_minus);
		AddNumber("Blue", settings_rgba->B, 0, settings_r_input, settings_r_plus, settings_r_minus);
		AddNumber("Opacity", settings_rgba->A, 0, settings_r_input, settings_r_plus, settings_r_minus);

		switch (menu::currentop)
		{
		case 1: settings_rgba2 = &settings_rgba->R; break;
		case 2: settings_rgba2 = &settings_rgba->G; break;
		case 3: settings_rgba2 = &settings_rgba->B; break;
		case 4: settings_rgba2 = &settings_rgba->A; break;
		}

		if (settings_r_plus) {
			if (*settings_rgba2 < 255) (*settings_rgba2)++;
			else *settings_rgba2 = 0;
			return;
		}
		else if (settings_r_minus) {
			if (*settings_rgba2 > 0) (*settings_rgba2)--;
			else *settings_rgba2 = 255;
			return;
		}
	}
	void AddsettingsfonOption(char* text, int font_index = -1, int &feature = inull)
	{
		bool bit_changefont = 0, bit_setfeature = 0;
		if (font_index == -1) AddOption(text, bit_setfeature, nullFunc, SUB::SETTINGS_FONTS2);
		else AddOption(text, bit_changefont);

		if (bit_setfeature) settings_font = &feature;
		else if (bit_changefont) *settings_font = font_index;
	}
	void SettingsFonts()
	{
		AddTitle("Menu Fonts");
		AddsettingsfonOption("Title", -1, font_title);
		AddsettingsfonOption("Options", -1, font_options);
		AddsettingsfonOption("Selected Option", -1, font_selection);
		AddsettingsfonOption("Option Breaks", -1, font_breaks);
	}
	void SettingsFonts2()
	{
		AddTitle("Set Font");
		AddsettingsfonOption("Normalish", 0);
		AddsettingsfonOption("Impactish", 4);
		AddsettingsfonOption("Italic", 1);
		AddsettingsfonOption("Pricedown", 7);
		AddsettingsfonOption("Caps", 2);
	}


	static char *vehicleName;


	static bool vehicleSpawnRequest = false;
	/*void SpawnVehicle() {

		float forward = 5.f;
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed);

		float xVect = forward * sin(degToRad(playerHeading)) * -1.0f;
		float yVect = forward * cos(degToRad(playerHeading));

		Hash toSpawnHash = GAMEPLAY::GET_HASH_KEY(vehicleName);

		Vehicle playerVeh;

		if (STREAMING::IS_MODEL_IN_CDIMAGE(toSpawnHash) && STREAMING::IS_MODEL_A_VEHICLE(toSpawnHash))
		{
			STREAMING::REQUEST_MODEL(toSpawnHash);


			while (!STREAMING::HAS_MODEL_LOADED(toSpawnHash))
			{
				WAIT(0);
			}


			playerVeh = VEHICLE::CREATE_VEHICLE(toSpawnHash, playerPos.x + xVect, playerPos.y + yVect, playerPos.z, playerHeading, TRUE, TRUE);

			RequestControlOfEnt(playerVeh);

			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(playerVeh);
			if (teleportInSpawnedVehicle) {
				PED::SET_PED_INTO_VEHICLE(playerPed, playerVeh, -1);
			}
			VEHICLE::SET_VEHICLE_ENGINE_ON(playerVeh, TRUE, TRUE, TRUE);

			if (highEndBypass) {
				BypassOnlineVehicleKick(playerVeh);
			}
			if (maxSpawnedVehicle) {
				MaxThisVehicle(playerVeh);
			}

			if (VEHICLE::IS_THIS_MODEL_A_HELI(toSpawnHash)) {
				VEHICLE::SET_HELI_BLADES_FULL_SPEED(playerVeh);
			}

			VEHICLE::SET_VEHICLE_DIRT_LEVEL(playerVeh, 0);

			AUDIO::SET_VEH_RADIO_STATION(playerVeh, "OFF");
			if (spawnedVehicle != NULL && ENTITY::DOES_ENTITY_EXIST(spawnedVehicle)) {
				vehicleSpawnRequest = false;
			}

		}
	}
	*/


	static std::string Super[] = {
		"ADDER", "BANSHEE2", "BULLET", "CHEETAH", "ENTITYXF",
		"FMJ", "SHEAVA", "INFERNUS", "ITALIGTB", "ITALIGTB2", "NERO", "NERO2",
		"OSIRIS", "LE7B", "PENETRATOR", "PFISTER811", "PROTOTIPO", "REAPER", "SULTANRS", "T20",
		"TEMPESTA", "TURISMOR", "TYRUS", "VACCA", "VOLTIC", "VOLTIC2", "ZENTORNO"
	};
	static std::string Sports[] = {
		"ALPHA", "BANSHEE", "BESTIAGTS", "BLISTA2", "BLISTA3",
		"BUFFALO", "BUFFALO2", "BUFFALO3", "CARBONIZZARE", "COMET2","COMET3",
		"COQUETTE","ELEGY", "ELEGY2", "FELTZER2", "FUROREGT", "FUSILADE",
		"FUTO", "JESTER", "JESTER2", "KHAMELION", "KURUMA",
		"KURUMA2", "LYNX", "MASSACRO", "MASSACRO2", "NINEF",
		"NINEF2", "OMNIS", "PENUMBRA", "RAPIDGT", "RAPIDGT2",
		"RAPTOR", "SCHAFTER3", "SCHAFTER4", "SCHWARTZER", "SEVEN70", "SPECTER", "SPECTER2",
		"SULTAN", "SURANO", "TAMPA2", "TROPOS", "VERLIERER2"
	};
	static std::string SportsClassics[] = {
		"BTYPE", "BTYPE2", "BTYPE3", "CASCO", "COQUETTE2",
		"FELTZER3", "JB700", "MAMBA", "MANANA", "MONROE",
		"PEYOTE", "PIGALLE", "STINGER", "STINGERGT", "TORNADO",
		"TORNADO2", "TORNADO3", "TORNADO4", "TORNADO5", "TORNADO6",
		"ZTYPE"
	};

	static std::string Coupes[] = {
		"COGCABRIO", "EXEMPLAR", "F620", "FELON", "FELON2",
		"JACKAL", "ORACLE", "ORACLE2", "SENTINEL", "SENTINEL2",
		"WINDSOR", "WINDSOR2", "ZION", "ZION2"
	};

	static std::string Muscle[] = {
		"BLADE", "BUCCANEER", "BUCCANEER2", "CHINO", "CHINO2",
		"COQUETTE3", "DOMINATOR", "DOMINATOR2", "DUKES", "DUKES2",
		"GAUNTLET", "GAUNTLET2", "FACTION", "FACTION2", "FACTION3",
		"HOTKNIFE", "LURCHER", "MOONBEAM", "MOONBEAM2", "NIGHTSHADE",
		"PHOENIX", "PICADOR", "RATLOADER", "RATLOADER2", "RUINER", "RUINER2", "RUINER3",
		"SABREGT", "SABREGT2", "SLAMVAN", "SLAMVAN2", "SLAMVAN3",
		"STALION", "STALION2", "TAMPA", "VIGERO", "VIRGO",
		"VIRGO2", "VIRGO3", "VOODOO", "VOODOO2"
	};

	static std::string OffRoad[] = {
		"BFINJECTION", "BIFTA", "BLAZER", "BLAZER2", "BLAZER3",
		"BLAZER4", "BLAZER5", "BODHI2", "BRAWLER", "DLOADER", "DUBSTA3",
		"DUNE", "DUNE2", "DUNE4", "DUNE5", "INSURGENT", "INSURGENT2", "KALAHARI",
		"MARSHALL", "MESA3", "MONSTER", "RANCHERXL", "RANCHERXL2",
		"REBEL", "REBEL2", "SANDKING", "SANDKING2", "TECHNICAL", "TECHNICAL2",
		"TROPHYTRUCK", "TROPHYTRUCK2"
	};

	static std::string Motorcycles[] = {
		"AKUMA", "AVARUS", "BAGGER", "BATI", "BATI2",
		"BF400", "CARBONRS", "CHIMERA", "CLIFFHANGER", "DAEMON",
		"DAEMON2", "DEFILER", "DIABLOUS", "DIABLOUS2", "DOUBLE", "ENDURO", "ESSKEY",
		"FAGGIO", "FAGGIO2", "FAGGIO3", "FCR", "FCR2", "GARGOYLE", "HAKUCHOU",
		"HAKUCHOU2", "HEXER", "INNOVATION", "LECTRO", "MANCHEZ",
		"NEMESIS", "NIGHTBLADE", "PCJ", "RATBIKE", "RUFFIAN",
		"SANCHEZ", "SANCHEZ2", "SANCTUS", "SHOTARO", "SOVEREIGN",
		"THRUST", "VADER", "VINDICATOR", "VORTEX", "WOLFSBANE",
		"ZOMBIEA", "ZOMBIEB"
	};

	static std::string Emergency[] = {
		"BARRACKS", "BARRACKS2", "BARRACKS3", "CRUSADER", "RHINO",
		"AMBULANCE", "FBI", "FBI2", "FIRETRUK", "LGUARD",
		"PBUS", "PRANGER", "POLICE", "POLICE2", "POLICE3",
		"POLICE4", "POLICEB", "POLICEOLD1", "POLICEOLD2", "POLICET",
		"SHERIFF", "SHERIFF2", "RIOT"
	};

	static std::string Service[] = {
		"AIRBUS", "BRICKADE", "BUS", "COACH", "RALLYTRUCK",
		"RENTALBUS", "TAXI", "TOURBUS", "TRASH", "TRASH2", "WASTELANDER"
	};

	static std::string Sedans[] = {
		"ASEA", "ASEA2", "ASTEROPE", "COG55", "COG552",
		"COGNOSCENTI", "COGNOSCENTI2", "EMPEROR", "EMPEROR2", "EMPEROR3",
		"FUGITIVE", "GLENDALE", "INGOT", "INTRUDER", "LIMO2",
		"PREMIER", "PRIMO", "PRIMO2", "REGINA", "ROMERO",
		"SCHAFTER2", "SCHAFTER5", "SCHAFTER6", "STANIER", "STRATUM",
		"STRETCH", "SUPERD", "SURGE", "TAILGATER", "WARRENER",
		"WASHINGTON"
	};
	static std::string Industrial[] = {
		"BULLDOZER", "CUTTER", "DUMP", "FLATBED", "GUARDIAN",
		"HANDLER", "MIXER", "MIXER2", "RUBBLE", "TIPTRUCK",
		"TIPTRUCK2"
	};
	static std::string Compacts[] = {
		"BLISTA", "BRIOSO", "DILETTANTE", "DILETTANTE2", "ISSI2",
		"PANTO", "PRAIRIE", "RHAPSODY"
	};
	static std::string Planes[] = {
		"BESRA", "CARGOPLANE", "CUBAN800", "DODO", "DUSTER",
		"HYDRA", "JET", "LAZER", "LUXOR", "LUXOR2",
		"MAMMATUS", "MILJET", "NIMBUS", "SHAMAL", "STUNT",
		"TITAN", "VELUM", "VELUM2", "VESTRA"
	};
	static std::string Helicopters[] = {
		"ANNIHILATOR", "BLIMP", "BLIMP2", "BUZZARD", "BUZZARD2",
		"CARGOBOB", "CARGOBOB2", "CARGOBOB3", "CARGOBOB4", "FROGGER",
		"FROGGER2", "MAVERICK", "POLMAV", "SAVAGE", "SKYLIFT",
		"SUPERVOLITO", "SUPERVOLITO2", "SWIFT", "SWIFT2", "VALKYRIE",
		"VALKYRIE2", "VOLATUS"
	};
	static std::string Boats[] = {
		"DINGHY", "DINGHY2", "DINGHY3", "DINGHY4", "JETMAX",
		"MARQUIS", "PREDATOR", "SEASHARK", "SEASHARK2", "SEASHARK3",
		"SPEEDER", "SPEEDER2", "SQUALO", "SUBMERSIBLE", "SUBMERSIBLE2",
		"SUNTRAP", "TORO", "TORO2", "TROPIC", "TROPIC2",
		"TUG"
	};
	static std::string Commercial[] = {
		"BENSON", "BIFF", "HAULER", "MULE", "MULE2",
		"MULE3", "PACKER", "PHANTOM", "PHANTOM2", "POUNDER", "STOCKADE",
		"STOCKADE3"
	};
	static std::string Cycles[] = {
		"BMX", "CRUISER", "FIXTER", "SCORCHER", "TRIBIKE",
		"TRIBIKE2", "TRIBIKE3"
	};
	static std::string SUVs[] = {
		"BALLER", "BALLER2", "BALLER3", "BALLER4", "BALLER5",
		"BALLER6", "BJXL", "CAVALCADE", "CAVALCADE2", "CONTENDER",
		"DUBSTA", "DUBSTA2", "FQ2", "GRANGER", "GRESLEY",
		"HABANERO", "HUNTLEY", "LANDSTALKER", "MESA", "MESA2",
		"PATRIOT", "RADI", "ROCOTO", "SEMINOLE", "SERRANO",
		"XLS", "XLS2"
	};
	static std::string Trailer[] = {
		"ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2", "BALETRAILER", "BOATTRAILER",
		"DOCKTRAILER", "FREIGHTTRAILER", "GRAINTRAILER", "PROPTRAILER", "RAKETRAILER",
		"TANKER", "TANKER2", "TR2", "TR3", "TR4",
		"TRAILERLOGS", "TRAILERS", "TRAILERS2", "TRAILERS3", "TRAILERSMALL",
		"TRFLAT", "TVTRAILER"
	};
	static std::string Trains[] = {
		"CABLECAR", "FREIGHT", "FREIGHTCAR", "FREIGHTCONT1", "FREIGHTCONT2",
		"FREIGHTGRAIN", "METROTRAIN", "TANKERCAR"
	};
	static std::string Utility[] = {
		"AIRTUG", "CADDY", "CADDY2", "DOCKTUG", "FORKLIFT",
		"MOWER", "RIPLEY", "SADLER", "SADLER2", "SCRAP",
		"TOWTRUCK", "TOWTRUCK2", "TRACTOR", "TRACTOR2", "TRACTOR3",
		"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3"
	};

	static std::string Vans[] = {
		"BISON", "BISON2", "BISON3", "BOBCATXL", "BOXVILLE",
		"BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BOXVILLE5", "BURRITO", "BURRITO2",
		"BURRITO3", "BURRITO4", "BURRITO5", "CAMPER", "GBURRITO",
		"GBURRITO2", "JOURNEY", "MINIVAN", "MINIVAN2", "PARADISE",
		"PONY", "PONY2", "RUMPO", "RUMPO2", "RUMPO3",
		"SPEEDO", "SPEEDO2", "SURFER", "SURFER2", "TACO",
		"YOUGA", "YOUGA2"
	};

	void AttachThingToThing(Any p0, Any p1, float x = 0, float y = 0, float z = 0, float rx = 0, float ry = 0, float rz = 0) {
		ENTITY::ATTACH_ENTITY_TO_ENTITY(p0, p1, 0, x, y, z, rx, ry, rz, 1, 0, 0, 2, 1, 1);
	}

	void SofaCar() {
		Vector3 myPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
		Vehicle zentorno = SpawnVehicleCustom("SURANO", myPos.x, myPos.y, myPos.z, false, false, true, true, false);
		ENTITY::SET_ENTITY_VISIBLE(zentorno, false, 0);
		ENTITY::SET_ENTITY_ALPHA(zentorno, 0.0f, 1);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(zentorno, 1);
		Object model = SpawnObject("PROP_YACHT_SEAT_01", myPos.x, myPos.y, myPos.z);
		ENTITY::FREEZE_ENTITY_POSITION(model, true);
		AttachThingToThing(model, zentorno, -0.4f, -0.79f, -0.9f, 0.0f, 0.0f, 180.0f);
	}

	void CupCar() {
		Vector3 myPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
		Vehicle zentorno = SpawnVehicleCustom("SURANO", myPos.x, myPos.y, myPos.z, false, false, true, true, false);
		ENTITY::SET_ENTITY_VISIBLE(zentorno, false, 0);
		ENTITY::SET_ENTITY_ALPHA(zentorno, 0.0f, 1);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(zentorno, 1);
		Object model = SpawnObject("PROP_CUP_SAUCER_01", myPos.x, myPos.y, myPos.z);
		ENTITY::FREEZE_ENTITY_POSITION(model, true);
		AttachThingToThing(model, zentorno, -0.4f, -0.5f, -0.4f, 0.0f, 0.0f, 180.0f);
	}

	void CargoCar() {
		Vector3 myPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
		Vehicle t20 = SpawnVehicleCustom("t20", myPos.x, myPos.y, myPos.z, false, false, true, true, false);

		Vehicle cargoplane = SpawnVehicleCustom("cargoplane", myPos.x, myPos.y, myPos.z, false, false, true, true, false);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(cargoplane, 1);

		AttachThingToThing(cargoplane, t20, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f);
		VEHICLE::GET_PED_IN_VEHICLE_SEAT(t20, -1);
	}
	void VehicleList() {
		if (vehicleCategoryName == "Super") {
			for (int i = 0; i < ARRAYSIZE(Super); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Super[i]);
				AddVehicle(StringToChar(Super[i]), StringToChar(Super[i]));
			}
		}
		else if (vehicleCategoryName == "Sports") {
			for (int i = 0; i < ARRAYSIZE(Sports); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Sports[i]);
				AddVehicle(StringToChar(Sports[i]), StringToChar(Sports[i]));
			}
		}
		else if (vehicleCategoryName == "Sports Classics") {
			for (int i = 0; i < ARRAYSIZE(SportsClassics); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(SportsClassics[i]);
				AddVehicle(StringToChar(SportsClassics[i]), StringToChar(SportsClassics[i]));
			}
		}
		else if (vehicleCategoryName == "Coupes") {
			for (int i = 0; i < ARRAYSIZE(Coupes); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Coupes[i]);
				AddVehicle(StringToChar(Coupes[i]), StringToChar(Coupes[i]));
			}
		}
		else if (vehicleCategoryName == "Muscle") {
			for (int i = 0; i < ARRAYSIZE(Muscle); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Muscle[i]);
				AddVehicle(StringToChar(Muscle[i]), StringToChar(Muscle[i]));
			}
		}
		else if (vehicleCategoryName == "OffRoad") {
			for (int i = 0; i < ARRAYSIZE(OffRoad); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(OffRoad[i]);
				AddVehicle(StringToChar(OffRoad[i]), StringToChar(OffRoad[i]));
			}
		}
		else if (vehicleCategoryName == "Motorcycles") {
			for (int i = 0; i < ARRAYSIZE(Motorcycles); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Motorcycles[i]);
				AddVehicle(StringToChar(Motorcycles[i]), StringToChar(Motorcycles[i]));
			}
		}
		else if (vehicleCategoryName == "Emergency") {
			for (int i = 0; i < ARRAYSIZE(Emergency); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Emergency[i]);
				AddVehicle(StringToChar(Emergency[i]), StringToChar(Emergency[i]));
			}
		}
		else if (vehicleCategoryName == "Service") {
			for (int i = 0; i < ARRAYSIZE(Service); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Service[i]);
				AddVehicle(StringToChar(Service[i]), StringToChar(Service[i]));
			}
		}
		else if (vehicleCategoryName == "Sedans") {
			for (int i = 0; i < ARRAYSIZE(Sedans); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Sedans[i]);
				AddVehicle(StringToChar(Sedans[i]), StringToChar(Sedans[i]));
			}
		}
		else if (vehicleCategoryName == "Industrial") {
			for (int i = 0; i < ARRAYSIZE(Industrial); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Industrial[i]);
				AddVehicle(StringToChar(Industrial[i]), StringToChar(Industrial[i]));
			}
		}
		else if (vehicleCategoryName == "Compacts") {
			for (int i = 0; i < ARRAYSIZE(Compacts); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Compacts[i]);
				AddVehicle(StringToChar(Compacts[i]), StringToChar(Compacts[i]));
			}
		}
		else if (vehicleCategoryName == "Planes") {
			for (int i = 0; i < ARRAYSIZE(Planes); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Planes[i]);
				AddVehicle(StringToChar(Planes[i]), StringToChar(Planes[i]));
			}
		}
		else if (vehicleCategoryName == "Helicopters") {
			for (int i = 0; i < ARRAYSIZE(Helicopters); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Helicopters[i]);
				AddVehicle(StringToChar(Helicopters[i]), StringToChar(Helicopters[i]));
			}
		}
		else if (vehicleCategoryName == "Commercial") {
			for (int i = 0; i < ARRAYSIZE(Commercial); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Commercial[i]);
				AddVehicle(StringToChar(Commercial[i]), StringToChar(Commercial[i]));
			}
		}
		else if (vehicleCategoryName == "Boats") {
			for (int i = 0; i < ARRAYSIZE(Boats); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Boats[i]);
				AddVehicle(StringToChar(Boats[i]), StringToChar(Boats[i]));
			}
		}
		else if (vehicleCategoryName == "Cycles") {
			for (int i = 0; i < ARRAYSIZE(Cycles); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Cycles[i]);
				AddVehicle(StringToChar(Cycles[i]), StringToChar(Cycles[i]));
			}
		}
		else if (vehicleCategoryName == "SUVs") {
			for (int i = 0; i < ARRAYSIZE(SUVs); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(SUVs[i]);
				AddVehicle(StringToChar(SUVs[i]), StringToChar(SUVs[i]));
			}
		}
		else if (vehicleCategoryName == "Trailer") {
			for (int i = 0; i < ARRAYSIZE(Trailer); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Trailer[i]);
				AddVehicle(StringToChar(Trailer[i]), StringToChar(Trailer[i]));
			}
		}
		else if (vehicleCategoryName == "Trains") {
			for (int i = 0; i < ARRAYSIZE(Trains); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Trains[i]);
				AddVehicle(StringToChar(Trains[i]), StringToChar(Trains[i]));
			}
		}
		else if (vehicleCategoryName == "Utility") {
			for (int i = 0; i < ARRAYSIZE(Utility); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Utility[i]);
				AddVehicle(StringToChar(Utility[i]), StringToChar(Utility[i]));
			}
		}
		else if (vehicleCategoryName == "Vans") {
			for (int i = 0; i < ARRAYSIZE(Vans); i++) {
				AddTitle(vehicleCategoryName);
				vehicleName = StringToChar(Vans[i]);
				AddVehicle(StringToChar(Vans[i]), StringToChar(Vans[i]));
			}
		}
		else if (vehicleCategoryName == "Modded Vehicles") {
			AddTitle(vehicleCategoryName);
			AddOption("Sofa Car", null, SofaCar);
			AddOption("Cup Car", null, CupCar);
		}
	}
	void VehicleSpawnerMenu() {
		AddTitle("VEHICLE SPAWNER");
		AddToggle("Teleport In", teleportInSpawnedVehicle);
		AddToggle("Max Upgrade", maxSpawnedVehicle);
		AddToggle("High-End Bypass", highEndBypass);

		AddVehicleCategory("Super", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Super");
		AddVehicleCategory("Sports", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Sports");
		AddVehicleCategory("Sports Classic", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Sports Classics");
		AddVehicleCategory("Coupes", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Coupes");
		AddVehicleCategory("Compacts", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Compacts");
		AddVehicleCategory("Sedans", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Sedans");
		AddVehicleCategory("Muscle", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Muscle");
		AddVehicleCategory("Utility", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Utility");
		AddVehicleCategory("SUVs", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "SUVs");
		AddVehicleCategory("Vans", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Vans");
		AddVehicleCategory("OffRoad", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "OffRoad");
		AddVehicleCategory("Commercial", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Commercial");
		AddVehicleCategory("Motorcycles", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Motorcycles");
		AddVehicleCategory("Emergency", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Emergency");
		AddVehicleCategory("Service", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Service");
		AddVehicleCategory("Industrial", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Industrial");
		AddVehicleCategory("Planes", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Planes");
		AddVehicleCategory("Helicopters", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Helicopters");
		AddVehicleCategory("Boats", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Boats");
		AddVehicleCategory("Cycles", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Cycles");
		AddVehicleCategory("Trailer", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Trailer");
		AddVehicleCategory("Trains", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Trains");
		if (isTesterVersion) AddVehicleCategory("Modded Vehicles", null, nullFunc, SUB::VEHICLE_SPAWNER_CHOOSE, "Modded Vehicles");
	}
	static int FEATURE_VEHICLE_MODS[48];
	void VehicleMobileLSCMenu() {
		AddTitle("MOBILE LSC");
		Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		for (int i = 0; i < ARRAYSIZE(FEATURE_VEHICLE_MODS); i++) {
			if (VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, i) >= 1) {
				if (i == 17 || i == 18 || i == 19 || i == 20 || i == 21 || i == 22) {
					FEATURE_VEHICLE_MODS[i] = VEHICLE::IS_TOGGLE_MOD_ON(playerVeh, i);
				}
				else {
					FEATURE_VEHICLE_MODS[i] = VEHICLE::GET_VEHICLE_MOD(playerVeh, i);
				}
			}
		}

		AddVehicleMod("Spoilers", FEATURE_VEHICLE_MODS[0], FEATURE_VEHICLE_MODS[0], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 0) - 1);
		AddVehicleMod("Front Bumper", FEATURE_VEHICLE_MODS[1], FEATURE_VEHICLE_MODS[1], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 1) - 1);
		AddVehicleMod("Rear Bumper", FEATURE_VEHICLE_MODS[2], FEATURE_VEHICLE_MODS[2], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 2) - 1);
		AddVehicleMod("Side Skirt", FEATURE_VEHICLE_MODS[3], FEATURE_VEHICLE_MODS[3],  VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 3) - 1);
		AddVehicleMod("Exhaust", FEATURE_VEHICLE_MODS[4], FEATURE_VEHICLE_MODS[4], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 4) - 1);
		AddVehicleMod("Frame", FEATURE_VEHICLE_MODS[5], FEATURE_VEHICLE_MODS[5], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 5) - 1);
		AddVehicleMod("Grille", FEATURE_VEHICLE_MODS[6], FEATURE_VEHICLE_MODS[6], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 6) - 1);
		AddVehicleMod("Hood", FEATURE_VEHICLE_MODS[7], FEATURE_VEHICLE_MODS[7], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 7) - 1);
		AddVehicleMod("Fender", FEATURE_VEHICLE_MODS[8], FEATURE_VEHICLE_MODS[8], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 8) - 1);
		AddVehicleMod("Right Fender", FEATURE_VEHICLE_MODS[9], FEATURE_VEHICLE_MODS[9], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 9) - 1);
		AddVehicleMod("Roof", FEATURE_VEHICLE_MODS[10], FEATURE_VEHICLE_MODS[10], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 10) - 1);
		AddVehicleMod("Engine", FEATURE_VEHICLE_MODS[11], FEATURE_VEHICLE_MODS[11], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 11) - 1);
		AddVehicleMod("Brakes", FEATURE_VEHICLE_MODS[12], FEATURE_VEHICLE_MODS[12], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 12) - 1);
		AddVehicleMod("Transmission", FEATURE_VEHICLE_MODS[13], FEATURE_VEHICLE_MODS[13], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 13) - 1);
		AddVehicleMod("Horns", FEATURE_VEHICLE_MODS[14], FEATURE_VEHICLE_MODS[14], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 14) - 1);
		AddVehicleMod("Suspension", FEATURE_VEHICLE_MODS[15], FEATURE_VEHICLE_MODS[15], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 15) - 1);
		AddVehicleMod("Armor", FEATURE_VEHICLE_MODS[16], FEATURE_VEHICLE_MODS[16], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 16) - 1);
		AddVehicleMod("Front Wheels", FEATURE_VEHICLE_MODS[23], FEATURE_VEHICLE_MODS[23], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 23) - 1);
		AddVehicleMod("Back Wheels", FEATURE_VEHICLE_MODS[24], FEATURE_VEHICLE_MODS[24], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 24) - 1);
		AddVehicleMod("Plate holders", FEATURE_VEHICLE_MODS[25], FEATURE_VEHICLE_MODS[25], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 25) - 1);
		AddVehicleMod("Trim Design", FEATURE_VEHICLE_MODS[27], FEATURE_VEHICLE_MODS[27],  VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 27) - 1);
		AddVehicleMod("Ornaments", FEATURE_VEHICLE_MODS[28], FEATURE_VEHICLE_MODS[28], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 28) - 1);
		AddVehicleMod("Dial Design", FEATURE_VEHICLE_MODS[30], FEATURE_VEHICLE_MODS[30], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 30) - 1);
		AddVehicleMod("Steering Wheel", FEATURE_VEHICLE_MODS[33], FEATURE_VEHICLE_MODS[33], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 33) - 1);
		AddVehicleMod("Shifter Leavers", FEATURE_VEHICLE_MODS[34], FEATURE_VEHICLE_MODS[34], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 34) - 1);
		AddVehicleMod("Plaques", FEATURE_VEHICLE_MODS[35], FEATURE_VEHICLE_MODS[35], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 35) - 1);
		AddVehicleMod("Hydraulics", FEATURE_VEHICLE_MODS[38], FEATURE_VEHICLE_MODS[38], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 38) - 1);
		AddVehicleMod("Livery", FEATURE_VEHICLE_MODS[48], FEATURE_VEHICLE_MODS[48], VEHICLE::GET_NUM_VEHICLE_MODS(playerVeh, 48) - 1);
		AddIntToggle("UNK17", FEATURE_VEHICLE_MODS[17]);
		AddIntToggle("Turbo", FEATURE_VEHICLE_MODS[18]);
		AddIntToggle("UNK19", FEATURE_VEHICLE_MODS[19]);
		AddIntToggle("Tire Smoke", FEATURE_VEHICLE_MODS[20]);
		AddIntToggle("UNK21", FEATURE_VEHICLE_MODS[21]);
		AddIntToggle("Xenon Headlights", FEATURE_VEHICLE_MODS[22]);
	}

	void VehicleMultipliers() {
		bool vehDamageMult = 0, vehDefenseMult = 0, lights = 0, rpmChanged = 0, rpmDisabled = 0, torqueChanged = 0, torqueDisabled = 0;
		AddTitle("Vehicle Multipliers");
		AddToggle("Vehicle RPM", vehrpm, null, rpmDisabled);
		AddIntEasy("Set RPM", rpm, rpm, 1, 0, rpmChanged);
		AddToggle("Vehicle Torque", vehTorque, null, torqueDisabled);
		AddIntEasy("Set Torque", torque, torque, 1, 0, torqueChanged);
		AddIntEasy("Damage Multiplier", vehDamMult, vehDamMult, 1, 0, vehDamageMult);
		AddIntEasy("Defense Multiplier", vehDefMult, vehDefMult, 1, 0, vehDefenseMult);
		AddFloat("Acceleration Multiplier", AccelerationMultiplier, 2, AccelerationMultiplier, 0.01f);
		AddFloat("Brakes Multiplier", BrakesMultiplier, 2, BrakesMultiplier, 0.01f);
		AddFloat("Suspension Height", SuspensionHeight, 2, SuspensionHeight, 0.01f);
		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (lightMultiplier < 1) lightMultiplier = 1;
		if (lights)
			VEHICLE::SET_VEHICLE_LIGHT_MULTIPLIER(veh, lightMultiplier);
		/*MULTIPLIERS*/
		if (vehDamageMult) {
			RequestControlOfEnt(veh);
			PLAYER::SET_PLAYER_VEHICLE_DAMAGE_MODIFIER(playerID, vehDamMult);
		}
		if (vehDefenseMult) {
			RequestControlOfEnt(veh);
			PLAYER::SET_PLAYER_VEHICLE_DEFENSE_MODIFIER(playerID, vehDamMult);
		}
		if (rpmChanged && vehrpm)
		{

			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				RequestControlOfEnt(veh);
				VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, rpm);
			}
		}
		if (rpmDisabled) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				RequestControlOfEnt(veh);
				VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 0);
			}
		}
		if (torqueChanged && vehTorque)
		{

			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				RequestControlOfEnt(veh);
				VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, torque);
			}
		}
		if (torqueDisabled) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				RequestControlOfEnt(veh);
				VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 0);
			}
		}
	}

	static bool fxProtection = false;
	static void FXProtection() {
		if (fxProtection) {
			Vector3 myPos = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			GRAPHICS::REMOVE_PARTICLE_FX_IN_RANGE(myPos.x, myPos.y, myPos.z, 10);
		}
	}

	static bool cloneCrashProtection = false;
	static void AntiClonesCrash() {
		if (cloneCrashProtection) {
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			Ped clone;
			PED::GET_CLOSEST_PED(coords.x, coords.y, coords.z, 4.0, 0, 1, &clone, 0, 0, -1);
			if (ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(clone, playerPed)) {
				RequestControlOfEnt(clone);
				int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(clone);
				RequestControlOfid(netID);
				NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
				ENTITY::DETACH_ENTITY(clone, 1, 1);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(clone, 1, 1);
				ENTITY::DELETE_ENTITY(&clone);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&clone);
				//ENTITY::SET_ENTITY_COORDS(playerPed, coords.x, coords.y, coords.z - 1000, 0, 0, 0, 1);
			}
		}
	}

	static bool bunkerMode = false;
	float speedMult = 0.5f;
	static Object freeCamAttach;
	static bool bunkerJustDisabled = false;
	static void BunkerMode() {
		PrintBottomLeft("~g~Here you are 100% protected from crashes :)");
		freeCamAttach = SpawnObject("prop_cs_dildo_01", 0, 0, 0);
		ENTITY::SET_ENTITY_VISIBLE(freeCamAttach, false, 0);
		ENTITY::FREEZE_ENTITY_POSITION(freeCamAttach, 1);
		ENTITY::SET_ENTITY_COLLISION(freeCamAttach, 0, 1);
		Vector3 pc = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		ENTITY::SET_ENTITY_COORDS(freeCamAttach, 760.461, 7392.8032f, -180.0774f, 0, 0, 0, 1);
		initialized = 0;
		ENTITY::FREEZE_ENTITY_POSITION(playerPed, 1);
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) {
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 0);
			RequestControlOfEnt(veh);
			ENTITY::FREEZE_ENTITY_POSITION(veh, 1);
		}
	}

	void MenuProtections() {
		AddTitle("Protections");
		AddToggle("Anti ParticleFX", fxProtection);
		AddToggle("Anti Clones Crash", cloneCrashProtection);
		if (isPaidVersion) AddOption("~b~Bunker Mode", bunkerMode);
	}

	void WeatherMenu() {
		AddTitle("Weather");
		AddWeather("EXTRASUNNY");
		AddWeather("CLEAR");
		AddWeather("CLOUDS");
		AddWeather("OVERCAST");
		AddWeather("RAIN");
		AddWeather("CLEARING");
		AddWeather("THUNDER");
		AddWeather("SMOG");
		AddWeather("FOGGY");
		AddWeather("XMAS");
		AddWeather("SNOWLIGHT");
		AddWeather("BLIZZARD");
	}

	void ScreenEffectsMenu() {
		AddTitle("Screen Effects");
		AddOption("Stop Latest Effect", null, DisableLatestScreenFx);
		AddScreenFx("SwitchHUDIn", "SwitchHUDIn");
		AddScreenFx("SwitchHUDOut", "SwitchHUDOut");
		AddScreenFx("FocusIn", "FocusIn");
		AddScreenFx("FocusOut", "FocusOut");
		AddScreenFx("MinigameEndNeutral", "MinigameEndNeutral");
		AddScreenFx("MinigameEndTrevor", "MinigameEndTrevor");
		AddScreenFx("MinigameEndFranklin", "MinigameEndFranklin");
		AddScreenFx("MinigameEndMichael", "MinigameEndMichael");
		AddScreenFx("MinigameTransitionOut", "MinigameTransitionOut");
		AddScreenFx("MinigameTransitionIn", "MinigameTransitionIn");
		AddScreenFx("SwitchShortNeutralIn", "SwitchShortNeutralIn");
		AddScreenFx("SwitchShortFranklinIn", "SwitchShortFranklinIn");
		AddScreenFx("SwitchShortTrevorIn", "SwitchShortTrevorIn");
		AddScreenFx("SwitchShortMichaelIn", "SwitchShortMichaelIn");
		AddScreenFx("SwitchOpenMichaelIn", "SwitchOpenMichaelIn");
		AddScreenFx("SwitchOpenFranklinIn", "SwitchOpenFranklinIn");
		AddScreenFx("SwitchOpenTrevorIn", "SwitchOpenTrevorIn");
		AddScreenFx("SwitchHUDMichaelOut", "SwitchHUDMichaelOut");
		AddScreenFx("SwitchHUDFranklinOut", "SwitchHUDFranklinOut");
		AddScreenFx("SwitchHUDTrevorOut", "SwitchHUDTrevorOut");
		AddScreenFx("SwitchShortFranklinMid", "SwitchShortFranklinMid");
		AddScreenFx("SwitchShortMichaelMid", "SwitchShortMichaelMid");
		AddScreenFx("SwitchShortTrevorMid", "SwitchShortTrevorMid");
		AddScreenFx("DeathFailOut", "DeathFailOut");
		AddScreenFx("CamPushInNeutral", "CamPushInNeutral");
		AddScreenFx("CamPushInFranklin", "CamPushInFranklin");
		AddScreenFx("CamPushInMichael", "CamPushInMichael");
		AddScreenFx("CamPushInTrevor", "CamPushInTrevor");
		AddScreenFx("SwitchOpenMichaelIn", "SwitchOpenMichaelIn");
		AddScreenFx("SwitchSceneFranklin", "SwitchSceneFranklin");
		AddScreenFx("SwitchSceneTrevor", "SwitchSceneTrevor");
		AddScreenFx("SwitchSceneMichael", "SwitchSceneMichael");
		AddScreenFx("SwitchSceneNeutral", "SwitchSceneNeutral");
		AddScreenFx("MP_Celeb_Win", "MP_Celeb_Win");
		AddScreenFx("MP_Celeb_Win_Out", "MP_Celeb_Win_Out");
		AddScreenFx("MP_Celeb_Lose", "MP_Celeb_Lose");
		AddScreenFx("MP_Celeb_Lose_Out", "MP_Celeb_Lose_Out");
		AddScreenFx("DeathFailNeutralIn", "DeathFailNeutralIn");
		AddScreenFx("DeathFailMPDark", "DeathFailMPDark");
		AddScreenFx("DeathFailMPIn", "DeathFailMPIn");
		AddScreenFx("MP_Celeb_Preload_Fade", "MP_Celeb_Preload_Fade");
		AddScreenFx("PeyoteEndOut", "PeyoteEndOut");
		AddScreenFx("PeyoteEndIn", "PeyoteEndIn");
		AddScreenFx("PeyoteIn", "PeyoteIn");
		AddScreenFx("PeyoteOut", "PeyoteOut");
		AddScreenFx("MP_race_crash", "MP_race_crash");
		AddScreenFx("SuccessFranklin", "SuccessFranklin");
		AddScreenFx("SuccessTrevor", "SuccessTrevor");
		AddScreenFx("SuccessMichael", "SuccessMichael");
		AddScreenFx("DrugsMichaelAliensFightIn", "DrugsMichaelAliensFightIn");
		AddScreenFx("DrugsMichaelAliensFight", "DrugsMichaelAliensFight");
		AddScreenFx("DrugsMichaelAliensFightOut", "DrugsMichaelAliensFightOut");
		AddScreenFx("DrugsTrevorClownsFightIn", "DrugsTrevorClownsFightIn");
		AddScreenFx("DrugsTrevorClownsFight", "DrugsTrevorClownsFight");
		AddScreenFx("DrugsTrevorClownsFightOut", "DrugsTrevorClownsFightOut");
		AddScreenFx("HeistCelebPass", "HeistCelebPass");
		AddScreenFx("HeistCelebPassBW", "HeistCelebPassBW");
		AddScreenFx("HeistCelebEnd", "HeistCelebEnd");
		AddScreenFx("HeistCelebToast", "HeistCelebToast");
		AddScreenFx("MenuMGHeistIn", "MenuMGHeistIn");
		AddScreenFx("MenuMGTournamentIn", "MenuMGTournamentIn");
		AddScreenFx("MenuMGSelectionIn", "MenuMGSelectionIn");
		AddScreenFx("ChopVision", "ChopVision");
		AddScreenFx("DMT_flight_intro", "DMT_flight_intro");
		AddScreenFx("DMT_flight", "DMT_flight");
		AddScreenFx("DrugsDrivingIn", "DrugsDrivingIn");
		AddScreenFx("DrugsDrivingOut", "DrugsDrivingOut");
		AddScreenFx("SwitchOpenNeutralFIB5", "SwitchOpenNeutralFIB5");
		AddScreenFx("HeistLocate", "HeistLocate");
		AddScreenFx("MP_job_load", "MP_job_load");
		AddScreenFx("RaceTurbo", "RaceTurbo");
		AddScreenFx("MP_intro_logo", "MP_intro_logo");
		AddScreenFx("HeistTripSkipFade", "HeistTripSkipFade");
		AddScreenFx("MenuMGHeistOut", "MenuMGHeistOut");
		AddScreenFx("MP_corona_switch", "MP_corona_switch");
		AddScreenFx("MenuMGSelectionTint", "MenuMGSelectionTint");
		AddScreenFx("SuccessNeutral", "SuccessNeutral");
		AddScreenFx("ExplosionJosh3", "ExplosionJosh3");
		AddScreenFx("SniperOverlay", "SniperOverlay");
		AddScreenFx("RampageOut", "RampageOut");
		AddScreenFx("Rampage", "Rampage");
		AddScreenFx("Dont_tazeme_bro", "Dont_tazeme_bro");
		AddScreenFx("DeathFailOut", "DeathFailOut");
	}
}


static int shortTimer = 0;
static int vehicleWeaponsTimer = 0;
static int longTimer = 0;

static int rpTimer = 0;
void menu::loops()
{
	/*	Make calls to functions that you want looped over here, e.g. ambient lights, whale guns, explosions, checks, flying deers, etc.
	Can also be used for (bool) options that are to be executed from many parts of the script. */

	if (shortTimer % 80 == 0) {
		GodMode();
		NeverWanted();
		SuperJump();
		GhostMode();
		SuperRun();
		NoRagdoll();
		sub::NeverDraggedOut();

		sub::FreezeFreezedPlayers();

		VehicleGodMode();
		InvisibleVehicle();
		NoBikeFall();
		SuperCarMode();

		VehicleCollisions();
		AutoFixVehicle();
		sub::LockAllVehicleDoors();
		sub::VehicleLowGrip();
		sub::VehicleAlwaysBurnout();
		FlyMode();
		DrawAimingLasers();

		AimBot();
		OneShotKill();
		ExplosiveBullets();
		ExplosiveMelee();
		HeatVisionOnAim();

		RapidFire();
		InfiniteAmmo();
		WeaponsEverywhere();
		DeleteGun();
		DriveItGun();
		TeleportGun();

		ShootRhino();
		//ONLINE PLAYERS RELATED LOOPS GO HERE
		sub::EntitySpam(sub::entitySpamVehicleName);

		sub::ExplodeNearbyVehicles();
		sub::MaxNearbyVehicles();
		sub::RainbowNearbyVehicles();
		sub::JumpLoopNearbyVehicles();

		sub::KillNearbyPeds();

		MobileRadio();
		DisplayCoords();
		ESP();

		sub::WtfMode();

		sub::AntiClonesCrash();
		sub::FXProtection();
		shortTimer = 0;
	}
	else {
		shortTimer++;
	}

	if (vehicleWeaponsTimer <= 400) {
		CarWeaponLoop(vehicleWeapons[selectedVehicleWeapon]);
		vehicleWeaponsTimer = 0;
	}
	else {
		vehicleWeaponsTimer++;
	}



	if (loop_RainbowBoxes && GAMEPLAY::GET_GAME_TIMER() >= livetimer)
	{
		titlebox.R = RandomRGB(); titlebox.G = RandomRGB(); titlebox.B = RandomRGB();
		BG.R = RandomRGB(); BG.G = RandomRGB(); BG.B = RandomRGB();
		selectedtext.R = RandomRGB(); selectedtext.G = RandomRGB(); selectedtext.B = RandomRGB();
	};

	sub::PedMoneyDrop();
	// VEHICLE SPAWNER ~ loop the vehicleSpawner function until a valid vehicle is spawned
	/*if (sub::vehicleSpawnRequest) {
		sub::SpawnVehicle();
	}
	*/

	if (sub::vehicleSpawnRequestCustom) {
		sub::SpawnVehicleCustom(sub::customVehicleName, sub::vehicleCustomX, sub::vehicleCustomY, sub::vehicleCustomZ, sub::vehicleCustomGodMode, sub::vehicleCustomMaxUpgrade, sub::vehicleCustomVisible, sub::vehicleCustomSetMeIntoVehicle, sub::vehicleCustomRandomColors); // this will take a while
	}

}


// badsport code gameplay::_0xBFE5756E7407064A(vVar0, vVar3, iVar6, 1, joaat("weapon_airstrike_rocket"), player::player_ped_id(), 1, 0, -1f, player::player_ped_id(), 1, 1, 0, 0); am_airstrike   @Kiro skid

void menu::submenu_switch()
{ // Make calls to submenus over here.
  // BUG HERE
	switch (currentsub)
	{
	case SUB::MAINMENU:						sub::MainMenu(); break;
	case SUB::SELF:				     		sub::SelfMenu(); break;
	case SUB::ONLINE_PLAYERS:				sub::OnlinePlayersMenu(); break;
	case SUB::ALL_ONLINE_PLAYERS:			sub::AllOnlinePlayersMenu(); break;
	case SUB::VEHICLES:				     	sub::VehiclesMenu(); break;
	case SUB::WEAPONS:				     	sub::WeaponsMenu(); break;
	case SUB::OBJECTS:				     	sub::ObjectsMenu(); break;
	case SUB::WORLD:				     	sub::WorldMenu(); break;
	case SUB::TELEPORT:						sub::TeleportMenu(); break;
	case SUB::MISC:							sub::MiscMenu(); break;
	case SUB::PARTICLEFX:					sub::SelfPtfxMenu(); break;
	case SUB::RECOVERY:				     	sub::RecoveryMenu(); break;
	case SUB::SETTINGS:						sub::SettingsMenu(); break;

	case SUB::SELF_MODEL_CHANGER:			sub::ModelChangerMenu(); break;
	case SUB::SELF_ANIMATIONS:				sub::SelfAnimationsMenu(); break;
	case SUB::SELF_SCENARIOS:				sub::SelfScenariosMenu(); break;
	case SUB::OUTFITS:						sub::OutfitsMenu(); break;

	case SUB::SELECT_PLAYER_MODEL:			sub::SelectPlayerModelMenu(); break;
	case SUB::ONLINE_PLAYER_ACTIONS:		sub::OnlinePlayerActions(); break;

	case SUB::ALL_ONLINE_ATTACKERS:         sub::LobbyAttackersMenu(); break;
	case SUB::ATTACH_OBJECT_TO_ALL_PLAYERS: sub::AttachObjectToAllMenu(); break;
	case SUB::PLAYER_VEHICLE:			    sub::PlayerVehicleActions(); break;
	case SUB::PLAYER_PED_OPTIONS:			sub::PlayerPedActions(); break;
	case SUB::PLAYER_UNSAFE_THINGS:			sub::UnsafeThingsMenu(); break;
	case SUB::PLAYER_TELEPORT_THINGS:       sub::PlayerTeleportThingsMenu(); break;
	case SUB::ATTACH_PLAYER_THINGS:         sub::AttachObjectToPlayerMenu(); break;
	case SUB::ENTITY_SPAM_MENU:             sub::EntitySpamMenu(); break;
	case SUB::PARTICLE_FX_MENU:             sub::ParticleFxMenu(); break;

	case SUB::PLAYER_WEAPONS:				sub::WeaponsMenu2(); break;
	case SUB::PLAYER_GRIEFING:				sub::GriefingMenu(); break;
	case SUB::PLAYER_SEND_ATTACKERS:        sub::AttackersMenu(); break;
	case SUB::PLAYER_DOES_ANIM:				sub::PlayerDoesAnimMenu(); break;   //TODO----SLOW THIS SHIT DOWN


	case SUB::VEHICLE_SPAWNER:				sub::VehicleSpawnerMenu(); break;
	case SUB::VEHICLE_SPAWNER_CHOOSE:		sub::VehicleList(); break;
	case SUB::VEHICLE_MOBILE_LSC:			sub::VehicleMobileLSCMenu(); break;

	case SUB::SELECT_OBJECT_CATEGORY:		sub::ObjectListMenu(); break;
	case SUB::COOL_OBJECTS_LIST:			sub::CoolObjectsMenu(); break;
	case SUB::STUNT_DLC_LIST:				sub::StuntObjectsMenu(); break;
	case SUB::FULL_OBJECTS_LIST:			sub::FullObjectsMenu(); break;
	case SUB::SPAWNED_OBJECTS_DATABASE:		sub::SpawnedObjectsDatabase(); break;
	case SUB::CLEAR_AREA:					sub::ClearArea(); break;
	case SUB::ESP:							sub::ESPMenu(); break;

	case SUB::MAPMOD:						sub::MapModsMenu(); break;
	case SUB::MAPMOD_MAZEDEMO:				sub::MapMod(0); break;
	case SUB::MAPMOD_MAZEROOFRAMP:			sub::MapMod(1); break;
	case SUB::MAPMOD_BEACHFERRISRAMP:		sub::MapMod(2); break;
	case SUB::MAPMOD_MOUNTCHILLIADRAMP:		sub::MapMod(3); break;
	case SUB::MAPMOD_AIRPORTMINIRAMP:		sub::MapMod(4); break;
	case SUB::MAPMOD_AIRPORTGATERAMP:		sub::MapMod(5); break;
	case SUB::MAPMOD_UFOTOWER:				sub::MapMod(6); break;
	case SUB::MAPMOD_MAZEBANKRAMPS:			sub::MapMod(7); break;
	case SUB::MAPMOD_FREESTYLEMOTOCROSS:	sub::MapMod(8); break;
	case SUB::MAPMOD_HALFPIPEFUNTRACK:		sub::MapMod(9); break;
	case SUB::MAPMOD_AIRPORTLOOP:			sub::MapMod(10); break;
	case SUB::MAPMOD_MAZEBANKMEGARAMP:		sub::MapMod(11); break;


	case SUB::IPL:							sub::IPL(); break;

	case SUB::VEHICLE_MULTIPLIERS:			sub::VehicleMultipliers(); break;

	case SUB::MENU_PROTECTIONS:				sub::MenuProtections(); break;
	case SUB::WEATHER:						sub::WeatherMenu(); break;
	case SUB::SCREEN_EFFECTS:				sub::ScreenEffectsMenu(); break;
	case SUB::NEARBY_VEHICLES:				sub::NearbyVehiclesMenu(); break;
	case SUB::NEARBY_PEDS:					sub::NearbyPedsMenu(); break;
	case SUB::MENU_INFO:					sub::InfoMenu(); break;
	case SUB::OBJECT_EDITOR:				sub::ObjectEditor(); break;
	case SUB::SETTINGS_COLOURS:				sub::SettingsColours(); break;
	case SUB::SETTINGS_COLOURS2:			sub::SettingsColours2(); break;
	case SUB::SETTINGS_FONTS:				sub::SettingsFonts(); break;
	case SUB::SETTINGS_FONTS2:				sub::SettingsFonts2(); break;

	}
}


void menu::sub_handler()
{
	if (!isMenuOpen) {
		while_closed();
	}

	else {
		submenu_switch(); 		// THE BUG IS HERE
		if (SetSub_delayed != NULL)
		{
			SetSub_new(SetSub_delayed);
			SetSub_delayed = NULL;
		}

		while_opened();
	}
}

static int GUITimer = 0;
static int authTimer = 0;

//http://pcpertutti.altervista.org/menu/login.php          menuVersion=ITalone_WORKS&project=FUCK_ROCKSTAR

static bool rebornMe = false;
void Main() {
	while (true) {
		if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE) || rebornMe) {
			if (IsKeyPressed(VK_F12)) {
				rebornMe = true;
			}
			WAIT(0);
		}
		else {
			rebornMe = false;

			playerID = PLAYER::PLAYER_ID();
			playerPed = PLAYER::PLAYER_PED_ID();

			if (KeyJustUp(teleportToWaypoint_HOTKEY)) {
				teleport_to_marker();
			}
			if (IsKeyPressed(teleportUp_HOTKEY)) {
				Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
				sub::TeleportMeTo(playerPos.x, playerPos.y, playerPos.z + 0.5);
			}

			if (IsKeyPressed(VK_F12)) {

			}

			if (GUITimer <= 8) {
				menu::base();
				GUITimer = 0;
			}
			else {
				GUITimer++;
			}
			menu::sub_handler(); // MUST BE THIS ONE
			menu::loops();

			WAIT(0);
		}
	}
}


void ScriptMain() {
	srand(GetTickCount());
	MenuStartSetup();
	Main();
}