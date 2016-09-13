#ifndef WEAPON_H_
#define WEAPON_H_

#define SCREEN_WIDTH 1024

typedef INT WEAPON;

typedef enum
{
	SNIPE,
	LIGHT_SNIPE,
	RIFLE,
	SUBMACHINE,
	SHOOT_GUN,
	SHOOT_GUN2,
	EMPTY
}SHOOT_KIND;
//ÌØÕ÷ÖµËã·¨£ºflag += x ^ y
#if SCREEN_WIDTH == 800

#define AWM						0x29F2
#define FN_NUPAO				0x5D5D
#define AK47					0x2DC2
#define M4A1					0x2D4B
#define _9A91					0x2C44
#define AN94					0x2F56
#define DESERT_EAGLE			0x7488
#define NIBOER_HONGSHUIJING		0xC747
#define MALAIJIAN				0x5005
#define MAUSER_FIRE				0x84F8
#define M66_XIANGLONG			0x7679
#define M4A1_XIANGLONG			0x7F5D
#define M4A1_ANJIN				0xDA2D
#define BEILEITA687				0x8536
#define HUANGJINQICUI			0x7C8D
#define SPAS_12					0x4126
#define WA2000					0xBAD7

#elif SCREEN_WIDTH == 1024

#define AWM						0x28F0
#define FN_NUPAO				0x57D4
#define AK47					0x2CC8
#define AK74					0x2CFD
#define M4A1					0x2BA3
#define _9A91					0x2B55
#define AN94					0x2E77
#define DESERT_EAGLE			0x6FD8
#define NIBOER_HONGSHUIJING		0xFFFF
#define MP5KA4					0x4C4D
#define MP5						0x22E0
#define MALAIJIAN				0xFFFF
#define MAUSER_FIRE				0xFFFF
#define M66_XIANGLONG			0xFFFF
#define M4A1_XIANGLONG			0xFFFF
#define M4A1_ANJIN				0xCDED
#define BEILEITA687				0x78D5
#define HUANGJINQICUI			0xFFFF
#define SPAS_12					0xFFFF
#define WA2000					0xFFFF
#define G11ANJIN				0x5DE9
#define SHITAIER				0xC5BE
#define XM1014					0x435D
#define DRVGUNOV				0x6D9E
#define P90						0x1F74
#define POJIUFUTOU				0x6AED
#define AUG						0x7204
#define USP						0x1DF5
#define GLOCK					0x53FC
#define DAJIZHE_TIANYU			0x8464

#endif

#endif