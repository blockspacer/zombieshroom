Global = {
	UserID = -1;
	IsClient = false;
}

ReservedUserID = {
	NONE = 0xFFFD;
	UNCONNECTED = 0xFFFE;
	ALL = 0xFFFF;
}

ReservedActionID = {
	NONE = 0xFFFFFFFD;
	CONNECT = 0xFFFFFFFE;
	ALL = 0xFFFFFFFF;
}

ReservedSequenceID = {
	NONE = 0xFFFD;
	CLIENT_ENTITY = 0xFFFE;
	ALL = 0xFFFF;
}

EntityState = {
	ASCENDING = 0;
	DESCENDING = 1;
	LANDING = 2;
	GROUNDED = 3;
	RAGDOLL = 4;
}

RenderPass = {
	RENDERPASS_DEFAULT = 0;
	RENDERPASS_SKYBOX = 1;
}

ShadowTechnique = {
	SHADOW_NONE = 0;
	SHADOW_OPAQUE = 1;
	SHADOW_TRANSPARANT = 2;
	SHADOW_DYNAMIC = 3;
	SHADOW_ANIMATED = 4;
}

LogLevel = {
	FATAL_ERROR = 0;
	NON_FATAL_ERROR = 1;
	WARNING = 2;
	SUCCESS = 3;
	DEBUG_PRINT = 4;
	INIT_PRINT = 5;
	MASS_DATA_PRINT = 6;

	FATAL_ERROR = 0;
	NON_FATAL_ERROR = 1;
	WARNING = 2;
	SUCCESS = 3;
	DEBUG_PRINT = 4;
	INIT_PRINT = 5;
	START_PRINT = 6;
	PINK_PRINT = 7;
	PACKET_PRINT = 8;
	MASS_DATA_PRINT = 9;
	NOLEVEL = 10;
	HELP_PRINT = 11;
	IDENTIFY_PRINT = 12;
}

PhysicsType = {
	TYPE_STATIC = 0;
	TYPE_ABILITY = 1;
	TYPE_DYNAMIC = 2;
	TYPE_PLAYER = 3;
  TYPE_RAGDOLL = 4;
  TYPE_ABILITYSPAWN = 5;
};

SoundMode = {
	SOUND_LOOP_OFF					= 0x00000001;
	SOUND_LOOP_NORMAL				= 0x00000002;
	SOUND_2D						= 0x00000008;
	SOUND_3D						= 0x00000010;
	SOUND_3D_LINEARROLLOFF			= 0x00200000;
	SOUND_3D_LINEARSQUAREROLLOFF	= 0x00400000;
};

AnimClip = {
	WALKING			= 0;
	IDLE			= 1;
	STRAFE_LEFT		= 2;
	STRAFE_RIGHT	= 3;
	JUMP_START		= 4;
	ASCEND 			= 5;
	DESCEND 		= 6;
	LANDING 		= 7;
	SHOOT1 			= 8;
	BACKWARDS		= 9;
	LEFTFORWARD		= 10;
	RIGHTFORWARD	= 11;
	LEFTBACK		= 12;
	RIGHTBACK		= 13;
	CHARGING1		= 14;
	SHOOT2			= 15;
	SHOOT3			= 16;
	SHOOT4			= 17;
	RAGDOLL 		= 18;
}