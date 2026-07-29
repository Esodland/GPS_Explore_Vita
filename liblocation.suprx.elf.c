//VADDR: 0x81000000 OFF: 0
// Exported
int SceLibLocation_09C4F674()
{
	// push {r4, lr}
	FLAGS = (uint8_t)a2 - 2;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000014;
	if ((a2 & ~0xFF) == 0)
		goto loc_8100001E;
	FLAGS = (a2 & ~0xFF) - 0x100;
	if (Z == 1) // ==
		goto loc_8100001E;

loc_81000014:
//VADDR: 0x81000014 OFF: 0x14
	a1 = /*data_8010124D*/;
	goto loc_81000022;

loc_8100001E:
//VADDR: 0x8100001e OFF: 0x1e
	a1 = sub_81000932();

loc_81000022:
//VADDR: 0x81000022 OFF: 0x22
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000024 OFF: 0x24
// Exported
int SceLibLocation_1E80199A()
{
	// push {r4, lr}
	a1 = sub_81000E3E();
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100002c OFF: 0x2c
// Exported
int SceLibLocation_DD271661()
{
	// push {r4, r5, r6, r7, r8, sb, sl, lr}
	sp = sp - 0x10;
	*(uint32_t *)(sp + 0xC) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v2 = a1 + 0;
	FLAGS = v2;
	*(uint32_t *)(sp + 8) = 0;
	v4 = a3;
	v3 = a2;
	if (Z == 0) // !=
		goto loc_81000058;
	a1 = /*data_80101240*/;
	goto loc_81000136;

loc_81000058:
//VADDR: 0x81000058 OFF: 0x58
	v1 = v3 & 0x100;
	FLAGS = v1;
	v5 = v3 & 0xF;
	sb = v3 & 0x70;
	if (Z == 1) // ==
		goto loc_8100007E;
	a1 = sub_81001C7E();
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_8100007E;
	*(uint32_t *)(v2) = -1;
	a1 = /*data_80101244*/;
	goto loc_81000136;

loc_8100007E:
//VADDR: 0x8100007e OFF: 0x7e
	a1 = (v3 & ~0x7F) & ~0x100;
	FLAGS = a1;
	if (Z == 0) // !=
		goto loc_810000A8;
	FLAGS = v5 - 5;
	if (C == 0 || Z == 1) // unsigned <=
		goto loc_81000094;
	FLAGS = v5 - 0xF;
	if (C == 0) // unsigned <
		goto loc_810000A8;

loc_81000094:
//VADDR: 0x81000094 OFF: 0x94
	FLAGS = v5 - 0xF;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_810000A8;
	FLAGS = sb - 0x50;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_810000A8;
	if (v1 == 0)
		goto loc_810000B8;
	FLAGS = v1 - 0x100;
	if (Z == 1) // ==
		goto loc_810000B8;

loc_810000A8:
//VADDR: 0x810000a8 OFF: 0xa8
	*(uint32_t *)(v2) = -1;
	a1 = /*data_80101244*/;
	goto loc_81000136;

loc_810000B8:
//VADDR: 0x810000b8 OFF: 0xb8
	a1 = v4 & ~/*data_80003777*/;
	FLAGS = a1;
	if (Z == 0) // !=
		goto loc_810000EE;
	FLAGS = (v4 & 7) - 4;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_810000EE;
	FLAGS = (v4 & 0x70) - 0x40;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_810000EE;
	FLAGS = (v4 & 0x700) - 0x600;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_810000EE;
	if ((v4 & 0x80000000) == 0)
		goto loc_810000FE;
	FLAGS = (v4 & 0x80000000) - 0x80000000;
	if (Z == 1) // ==
		goto loc_810000FE;

loc_810000EE:
//VADDR: 0x810000ee OFF: 0xee
	*(uint32_t *)(v2) = -1;
	a1 = /*data_80101245*/;
	goto loc_81000136;

loc_810000FE:
//VADDR: 0x810000fe OFF: 0xfe
	a1 = sub_81001294();
	a1 = sub_81001186(sp + 8);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81000114;
	*(uint32_t *)(v2) = -1;
	goto loc_81000136;

loc_81000114:
//VADDR: 0x81000114 OFF: 0x114
	a1 = sub_81001294();
	*(uint32_t *)(sp) = v4;
	a1 = sub_81001324(a1, v2 + 0, *(uint32_t *)(sp + 8), v3 + 0, *(sp+0));
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 1) // ==
		goto loc_81000134;
	a1 = sub_81000C72(*(uint32_t *)(v2));
	*(uint32_t *)(v2) = -1;

loc_81000134:
//VADDR: 0x81000134 OFF: 0x134
	a1 = v3 + 0;
	FLAGS = a1;

loc_81000136:
//VADDR: 0x81000136 OFF: 0x136
	FLAGS = (*(uint32_t *)(sl)) - (*(uint32_t *)(sp + 0xC));
	if (Z == 0) // !=
		goto loc_81000146;
	sp = sp + 0x10;
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, pc}

loc_81000146:
//VADDR: 0x81000146 OFF: 0x146
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100014c OFF: 0x14c
// Exported
int SceLibLocation_14FE76E8()
{
	// push {r4, lr}
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_81000160;
	a1 = /*data_80101241*/;
	goto loc_8100016A;

loc_81000160:
//VADDR: 0x81000160 OFF: 0x160
	a1 = sub_81001294();
	a1 = sub_81000CE8(a1, v1 + 0);

loc_8100016A:
//VADDR: 0x8100016a OFF: 0x16a
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100016c OFF: 0x16c
// Exported
int SceLibLocation_B1F55065()
{
	// push {r4, r5, r6, r7, r8, sb, sl, lr}
	v4 = a3 + 0;
	FLAGS = v4;
	v3 = a2 + 0;
	FLAGS = v3;
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_81000186;
	a1 = /*data_80101241*/;
	goto loc_81000228;

loc_81000186:
//VADDR: 0x81000186 OFF: 0x186
	v2 = v3 & 0x100;
	FLAGS = v2;
	v5 = v3 & 0xF;
	sb = v3 & 0x70;
	if (Z == 1) // ==
		goto loc_810001A6;
	a1 = sub_81001C7E();
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_810001A6;
	a1 = /*data_80101244*/;
	goto loc_81000228;

loc_810001A6:
//VADDR: 0x810001a6 OFF: 0x1a6
	a1 = (v3 & ~0x7F) & ~0x100;
	FLAGS = a1;
	if (Z == 0) // !=
		goto loc_810001D0;
	FLAGS = v5 - 5;
	if (C == 0 || Z == 1) // unsigned <=
		goto loc_810001BC;
	FLAGS = v5 - 0xF;
	if (C == 0) // unsigned <
		goto loc_810001D0;

loc_810001BC:
//VADDR: 0x810001bc OFF: 0x1bc
	FLAGS = v5 - 0xF;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_810001D0;
	FLAGS = sb - 0x50;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_810001D0;
	if (v2 == 0)
		goto loc_810001DA;
	FLAGS = v2 - 0x100;
	if (Z == 1) // ==
		goto loc_810001DA;

loc_810001D0:
//VADDR: 0x810001d0 OFF: 0x1d0
	a1 = /*data_80101244*/;
	goto loc_81000228;

loc_810001DA:
//VADDR: 0x810001da OFF: 0x1da
	a1 = v4 & ~/*data_80003777*/;
	FLAGS = a1;
	if (Z == 0) // !=
		goto loc_81000210;
	FLAGS = (v4 & 7) - 4;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000210;
	FLAGS = (v4 & 0x70) - 0x40;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000210;
	FLAGS = (v4 & 0x700) - 0x600;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000210;
	if ((v4 & 0x80000000) == 0)
		goto loc_8100021A;
	FLAGS = (v4 & 0x80000000) - 0x80000000;
	if (Z == 1) // ==
		goto loc_8100021A;

loc_81000210:
//VADDR: 0x81000210 OFF: 0x210
	a1 = /*data_80101245*/;
	goto loc_81000228;

loc_8100021A:
//VADDR: 0x8100021a OFF: 0x21a
	a1 = sub_81001294();
	a1 = sub_810013DC(a1, v1 + 0, v3 + 0, v4 + 0);

loc_81000228:
//VADDR: 0x81000228 OFF: 0x228
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, pc}
}

//VADDR: 0x8100022c OFF: 0x22c
// Exported
int SceLibLocation_188CE004()
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0xC;
	*(uint32_t *)(sp + 8) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v1 = a3 + 0;
	FLAGS = v1;
	v2 = a2 + 0;
	FLAGS = v2;
	a1 = sub_810012A0();
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 0) // !=
		goto loc_81000252;
	a1 = /*data_80101241*/;
	goto loc_81000280;

loc_81000252:
//VADDR: 0x81000252 OFF: 0x252
	a1 = v2 | v1;
	FLAGS = a1;
	if (Z == 0) // !=
		goto loc_81000262;
	a1 = /*data_80101240*/;
	goto loc_81000280;

loc_81000262:
//VADDR: 0x81000262 OFF: 0x262
	a1 = sub_81001294();
	a1 = sub_810014AA(a1, v3 + 0, sp + 0, sp + 4);
	v3 = *(uint32_t *)(sp + 4);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000280;
	if (v2 == 0)
		goto loc_8100027C;
	*(uint32_t *)(v2) = (*(uint32_t *)(sp));

loc_8100027C:
//VADDR: 0x8100027c OFF: 0x27c
	if (v1 == 0)
		goto loc_81000280;
	*(uint32_t *)(v1) = v3;

loc_81000280:
//VADDR: 0x81000280 OFF: 0x280
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 8));
	if (Z == 0) // !=
		goto loc_8100028C;
	sp = sp + 0xC;
	return a1; // pop {r4, r5, r6, r7, pc}

loc_8100028C:
//VADDR: 0x8100028c OFF: 0x28c
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000292 OFF: 0x292
// Exported
int SceLibLocation_15BC27C8()
{
	// push {r4, r5, r6, lr}
	v1 = a2 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_810002A2;
	a1 = /*data_80101240*/;
	goto loc_810002C2;

loc_810002A2:
//VADDR: 0x810002a2 OFF: 0x2a2
	a1 = sub_810012A0();
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_810002B4;
	a1 = /*data_80101241*/;
	goto loc_810002C2;

loc_810002B4:
//VADDR: 0x810002b4 OFF: 0x2b4
	a1 = sub_81001294();
	a1 = sub_810014B6(a1, v2 + 0, 0xF, v1 + 0);

loc_810002C2:
//VADDR: 0x810002c2 OFF: 0x2c2
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x810002c4 OFF: 0x2c4
// Exported
int SceLibLocation_16F41ED0()
{
	// push {r4, r5, r6, lr}
	v1 = a3 + 0;
	FLAGS = v1;
	v2 = a2;
	if (Z == 0) // !=
		goto loc_810002D6;
	a1 = /*data_80101240*/;
	goto loc_810002F6;

loc_810002D6:
//VADDR: 0x810002d6 OFF: 0x2d6
	a1 = sub_810012A0();
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 0) // !=
		goto loc_810002E8;
	a1 = /*data_80101241*/;
	goto loc_810002F6;

loc_810002E8:
//VADDR: 0x810002e8 OFF: 0x2e8
	a1 = sub_81001294();
	a1 = sub_810014B6(a1, v3 + 0, v2 + 0, v1 + 0);

loc_810002F6:
//VADDR: 0x810002f6 OFF: 0x2f6
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x810002f8 OFF: 0x2f8
// Exported
int SceLibLocation_71503251()
{
	// push {r4, lr}
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_8100030C;
	a1 = /*data_80101241*/;
	goto loc_81000316;

loc_8100030C:
//VADDR: 0x8100030c OFF: 0x30c
	a1 = sub_81001294();
	a1 = sub_81001604(a1, v1 + 0);

loc_81000316:
//VADDR: 0x81000316 OFF: 0x316
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000318 OFF: 0x318
// Exported
int SceLibLocation_12D1F0EA()
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0xC;
	v2 = a3 + 0;
	FLAGS = v2;
	v1 = a4;
	v3 = a2;
	if (Z == 0) // !=
		goto loc_8100032E;
	a1 = /*data_80101240*/;
	goto loc_81000350;

loc_8100032E:
//VADDR: 0x8100032e OFF: 0x32e
	a1 = sub_810012A0();
	v4 = a1 + 0;
	FLAGS = v4;
	if (Z == 0) // !=
		goto loc_81000340;
	a1 = /*data_80101241*/;
	goto loc_81000350;

loc_81000340:
//VADDR: 0x81000340 OFF: 0x340
	a1 = sub_81001294();
	*(uint32_t *)(sp) = v1;
	a1 = sub_81001626(a1, v4 + 0, v3 + 0, v2 + 0, *(sp+0));

loc_81000350:
//VADDR: 0x81000350 OFF: 0x350
	sp = sp + 0xC;
	return a1; // pop {r4, r5, r6, r7, pc}
}

//VADDR: 0x81000354 OFF: 0x354
// Exported
int SceLibLocation_ED378700()
{
	// push {r4, lr}
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_81000368;
	a1 = /*data_80101241*/;
	goto loc_81000372;

loc_81000368:
//VADDR: 0x81000368 OFF: 0x368
	a1 = sub_81001294();
	a1 = sub_810016DE(a1, v1 + 0);

loc_81000372:
//VADDR: 0x81000372 OFF: 0x372
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000374 OFF: 0x374
// Exported
int SceLibLocation_4E9E5ED9()
{
	// push {r4, r5, r6, lr}
	v1 = a2 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_81000384;
	a1 = /*data_80101240*/;
	goto loc_810003A2;

loc_81000384:
//VADDR: 0x81000384 OFF: 0x384
	a1 = sub_810012A0();
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_81000396;
	a1 = /*data_80101241*/;
	goto loc_810003A2;

loc_81000396:
//VADDR: 0x81000396 OFF: 0x396
	a1 = sub_81001294();
	a1 = sub_8100174C(a1, v2 + 0, v1 + 0);

loc_810003A2:
//VADDR: 0x810003a2 OFF: 0x3a2
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x810003a4 OFF: 0x3a4
// Exported
int SceLibLocation_07D4DFE0()
{
	// push {r4, r5, r6, lr}
	asm("vpush {s16, s17}\n");
	v2 = a2 + 0;
	FLAGS = v2;
	asm("vmov.f32 s16, s0\n");
	v1 = a3;
	if (Z == 0) // !=
		goto loc_810003BE;
	a1 = /*data_80101240*/;
	goto loc_81000412;

loc_810003BE:
//VADDR: 0x810003be OFF: 0x3be
	a1 = sub_810012A0();
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 0) // !=
		goto loc_810003D0;
	a1 = /*data_80101241*/;
	goto loc_81000412;

loc_810003D0:
//VADDR: 0x810003d0 OFF: 0x3d0
	a1 = 0;
	FLAGS = a1;
	asm("vmov s0, r0\n");
	asm("vcmp.f32 s16, s0\n");
	asm("vmr apsr_nzcv, fpscr\n");
	if (N == 1) // signed < 0
		goto loc_810003F6;
	a1 = /*data_43340000*/;
	asm("vmov s0, r0\n");
	asm("vcmp.f32 s16, s0\n");
	asm("vmr apsr_nzcv, fpscr\n");
	if (Z == 0 && N == V) // signed >
		goto loc_810003F6;
	goto loc_81000400;

loc_810003F6:
//VADDR: 0x810003f6 OFF: 0x3f6
	a1 = /*data_8010124D*/;
	goto loc_81000412;

loc_81000400:
//VADDR: 0x81000400 OFF: 0x400
	a1 = sub_81001294();
	asm("vmov.f32 s0, s16\n");
	a1 = sub_81001758(a1, v3 + 0, v2 + 0, v1 + 0);

loc_81000412:
//VADDR: 0x81000412 OFF: 0x412
	asm("vpop {s16, s17}\n");
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81000418 OFF: 0x418
// Exported
int SceLibLocation_92E53F94()
{
	// push {r4, lr}
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_8100042C;
	a1 = /*data_80101241*/;
	goto loc_81000436;

loc_8100042C:
//VADDR: 0x8100042c OFF: 0x42c
	a1 = sub_81001294();
	a1 = sub_8100181E(a1, v1 + 0);

loc_81000436:
//VADDR: 0x81000436 OFF: 0x436
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000438 OFF: 0x438
// Exported
int SceLibLocation_C895E567()
{
	// push {r4, lr}
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_8100044C;
	a1 = /*data_80101241*/;
	goto loc_81000456;

loc_8100044C:
//VADDR: 0x8100044c OFF: 0x44c
	a1 = sub_81001294();
	a1 = sub_810018DA(a1, v1 + 0);

loc_81000456:
//VADDR: 0x81000456 OFF: 0x456
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000458 OFF: 0x458
// Exported
int SceLibLocation_730FF842()
{
	// push {r4, r5, r6, lr}
	v1 = a2 + 0;
	FLAGS = v1;
	a1 = sub_810012A0();
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_8100046E;
	a1 = /*data_80101241*/;
	goto loc_81000486;

loc_8100046E:
//VADDR: 0x8100046e OFF: 0x46e
	if (v1 != 0)
		goto loc_8100047A;
	a1 = /*data_80101240*/;
	goto loc_81000486;

loc_8100047A:
//VADDR: 0x8100047a OFF: 0x47a
	a1 = sub_81001294();
	a1 = sub_810019B2(a1, v2 + 0, v1 + 0);

loc_81000486:
//VADDR: 0x81000486 OFF: 0x486
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81000488 OFF: 0x488
// Exported
int SceLibLocation_FF016C13()
{
	// push {r4, r5, r6, lr}
	v1 = a2 + 0;
	FLAGS = v1;
	a1 = sub_810012A0();
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_8100049E;
	a1 = /*data_80101241*/;
	goto loc_810004B6;

loc_8100049E:
//VADDR: 0x8100049e OFF: 0x49e
	if (v1 != 0)
		goto loc_810004AA;
	a1 = /*data_80101240*/;
	goto loc_810004B6;

loc_810004AA:
//VADDR: 0x810004aa OFF: 0x4aa
	a1 = sub_81001294();
	a1 = sub_810019CA(a1, v2 + 0, v1 + 0);

loc_810004B6:
//VADDR: 0x810004b6 OFF: 0x4b6
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x810004b8 OFF: 0x4b8
// Exported
int SceLibLocation_E3CBF875()
{
	// push {r4, lr}
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_810004CC;
	a1 = /*data_80101241*/;
	goto loc_810004D6;

loc_810004CC:
//VADDR: 0x810004cc OFF: 0x4cc
	a1 = sub_81001294();
	a1 = sub_810019F8(a1, v1 + 0);

loc_810004D6:
//VADDR: 0x810004d6 OFF: 0x4d6
	return a1; // pop {r4, pc}
}

//VADDR: 0x810004d8 OFF: 0x4d8
// Exported
int SceLibLocation_482622C6()
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0x1C;
	*(uint32_t *)(sp + 0x14) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v1 = a2 + 0;
	FLAGS = v1;
	a1 = sub_810012A0();
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_810004FC;
	a1 = /*data_80101241*/;
	goto loc_81000526;

loc_810004FC:
//VADDR: 0x810004fc OFF: 0x4fc
	a1 = SceLibKernel_632980D7((sp + 0) + 0, 0, 0x14);
	a1 = sub_81001294();
	a1 = sub_81001A0E(a1, v2 + 0, (sp + 0) + 0);
	v2 = a1 + 0;
	FLAGS = v2;
	FLAGS = v1 - 0;
	if (Z == 1) // ==
		goto loc_81000524;
	a1 = SceLibKernel_2E3B02A1(v1 + 0, sp + 0, 0x14);

loc_81000524:
//VADDR: 0x81000524 OFF: 0x524
	a1 = v2 + 0;
	FLAGS = a1;

loc_81000526:
//VADDR: 0x81000526 OFF: 0x526
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 0x14));
	if (Z == 0) // !=
		goto loc_81000532;
	sp = sp + 0x1C;
	return a1; // pop {r4, r5, r6, r7, pc}

loc_81000532:
//VADDR: 0x81000532 OFF: 0x532
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000538 OFF: 0x538
// Exported
int SceLibLocation_8AAF3FBD()
{
	// push {r4, lr}
	a1 = sub_810012A0();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_8100054C;
	a1 = /*data_80101241*/;
	goto loc_81000556;

loc_8100054C:
//VADDR: 0x8100054c OFF: 0x54c
	a1 = sub_81001294();
	a1 = sub_81001A1C(a1, v1 + 0);

loc_81000556:
//VADDR: 0x81000556 OFF: 0x556
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000558 OFF: 0x558
// Exported
int SceLibLocation_AA026B53()
{
	// push {r4, lr}
	a1 = sub_81001C2E();
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000560 OFF: 0x560
// Exported
int SceLibLocation_DE0A9EA4()
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x108;
	*(uint32_t *)(sp + 0x104) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_8100057E;
	a1 = /*data_80101240*/;
	goto loc_810005D8;

loc_8100057E:
//VADDR: 0x8100057e OFF: 0x57e
	a1 = SceLibKernel_632980D7((sp + 4) + 0, 0, 0x100);
	a1 = SceLibKernel_2E3B02A1((sp + 4) + 0, v1 + 0, 0xFF);
	a1 = SceLibKernel_B54C0BE4(/*s_text_81003304*/ "ux0:/data/gpsdata/", (sp + 4) + 0, 0x12);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810005B4;
	a1 = /*data_801012E0*/;
	goto loc_810005D8;

loc_810005B4:
//VADDR: 0x810005b4 OFF: 0x5b4
	a1 = sub_81001B6C(sp + 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_810005D8;
	a1 = sub_81001294();
	a1 = sub_81001C16(a1, *(uint32_t *)(sp), sp + 4);
	a1 = sub_81001BBA(sp + 0);
	FLAGS = (a1 + 0) - 0;
	if (Z == 1) // ==
		goto loc_810005D8;
	a1 = (a1 + 0) + 0;
	FLAGS = a1;

loc_810005D8:
//VADDR: 0x810005d8 OFF: 0x5d8
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x104));
	if (Z == 0) // !=
		goto loc_810005E4;
	sp = sp + 0x108;
	return a1; // pop {r4, r5, r6, pc}

loc_810005E4:
//VADDR: 0x810005e4 OFF: 0x5e4
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810005ea OFF: 0x5ea
// Exported
int SceLibLocation_C1CFEC92()
{
	// push {r4, r5, lr}
	sp = sp - 0xC;
	*(uint32_t *)(sp + 4) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_81000608;
	a1 = /*data_80101240*/;
	goto loc_8100063E;

loc_81000608:
//VADDR: 0x81000608 OFF: 0x608
	a1 = sub_81001C92();
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_8100061A;
	a1 = /*data_8010124F*/;
	goto loc_8100063E;

loc_8100061A:
//VADDR: 0x8100061a OFF: 0x61a
	a1 = sub_81001B6C(sp + 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_8100063E;
	a1 = sub_81001294();
	a1 = sub_81001C22(a1, *(uint32_t *)(sp), v1 + 0);
	a1 = sub_81001BBA(sp + 0);
	FLAGS = (a1 + 0) - 0;
	if (Z == 1) // ==
		goto loc_8100063E;
	a1 = (a1 + 0) + 0;
	FLAGS = a1;

loc_8100063E:
//VADDR: 0x8100063e OFF: 0x63e
	FLAGS = (*(uint32_t *)(v2)) - (*(uint32_t *)(sp + 4));
	if (Z == 0) // !=
		goto loc_8100064A;
	sp = sp + 0xC;
	return a1; // pop {r4, r5, pc}

loc_8100064A:
//VADDR: 0x8100064a OFF: 0x64a
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000650 OFF: 0x650
// Exported
int module_stop()
{
	// push {r4, lr}
	a1 = sub_81000D68();
	a1 = sub_81000E3E();
	a1 = 0;
	FLAGS = a1;
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100065e OFF: 0x65e
// Exported
int SceLibLocation_EFC9BD25()
{
	// push {r4, lr}
	a1 = sub_81001294();
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000666 OFF: 0x666
// Exported
int SceLibLocation_7F74AA94()
{
	// push {r4, lr}
	a1 = sub_8100130C();
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100066e OFF: 0x66e
// Exported
int SceLibLocation_0C0BB66A()
{
	// push {r4, lr}
	a1 = sub_810018D0();
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000676 OFF: 0x676
// Exported
int SceLibLocation_822BBF06()
{
	// push {r4, lr}
	a1 = sub_81001884(a1, (uint8_t)a2);
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000680 OFF: 0x680
// Exported
int SceLibLocation_991AF2D0()
{
	// push {r4, lr}
	a1 = sub_81001C5C();
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000688 OFF: 0x688
// Exported
int SceLibLocation_4C8B6F21()
{
	// push {r4, lr}
	v1 = a1 + 0;
	FLAGS = v1;
	a1 = sub_81001294();
	a1 = sub_81001C76();
	FLAGS = ((uint8_t)(a1 + 0)) - 2;
	a1 = 0;
	if (Z == 0) // !=
		goto loc_810006AE;
	FLAGS = (*(uint32_t *)((/*data_81004030*/ 0x00000000))) - 1;
	if (Z == 1) // ==
		goto loc_810006B8;

loc_810006AE:
//VADDR: 0x810006ae OFF: 0x6ae
	FLAGS = a3 - 0x10000;
	if (Z == 0) // !=
		goto loc_810006BE;
	FLAGS = a2 - 2;
	if (Z == 0) // !=
		goto loc_810006BE;

loc_810006B8:
//VADDR: 0x810006b8 OFF: 0x6b8
	a1 = v1 + 0;
	FLAGS = a1;
	a1 = sub_810012A0();

loc_810006BE:
//VADDR: 0x810006be OFF: 0x6be
	return a1; // pop {r4, pc}
}

//VADDR: 0x810006c0 OFF: 0x6c0
// Exported
int SceLibLocation_0341758B()
{
	// push {r4, lr}
	a1 = sub_81001294();
	a1 = sub_81001C76();
	return a1; // pop {r4, pc}
}

//VADDR: 0x810006cc OFF: 0x6cc
// Exported
int SceLibLocation_5C7185D2()
{
	*(uint8_t *)((/*data_81004034*/ 0x00000000)) = 1;
	return a1;
}

//VADDR: 0x810006da OFF: 0x6da
// Exported
int SceLibLocation_2311B24A()
{
	a1 = *(uint8_t *)((/*data_81004034*/ 0x00000000));
	return a1;
}

//VADDR: 0x810006e6 OFF: 0x6e6
// Exported
int SceLibLocation_41C1E68F()
{
	// push {r4, lr}
	a1 = sub_81001294();
	a1 = sub_81001C7A(a1, (a1 + 0) + 0);
	return a1; // pop {r4, pc}
}

//VADDR: 0x810006f8 OFF: 0x6f8
int sub_810006F8()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x810006fa OFF: 0x6fa
int sub_810006FA()
{
	// push {r4, r5, r6, r7, r8, lr}
	v1 = 0;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x60)) - 0;
	if (Z == 1) // ==
		goto loc_8100076C;
	v2 = 0;

loc_81000716:
//VADDR: 0x81000716 OFF: 0x716
	if ((*(uint32_t *)(a2 + v2)) == 0)
		goto loc_81000760;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2 + v2)) + 0x28)) - -1;
	if (Z == 1) // ==
		goto loc_8100072C;
	a1 = SceThreadmgr_1BBDE3D9(*(uint32_t *)((*(uint32_t *)(a2 + v2)) + 0x28));
	*(uint32_t *)((*(uint32_t *)(a2 + v2)) + 0x28) = -1;

loc_8100072C:
//VADDR: 0x8100072c OFF: 0x72c
	FLAGS = (*(uint32_t *)(v3 + 0x2C)) - -1;
	if (Z == 1) // ==
		goto loc_8100073E;
	a1 = SceThreadmgr_1BBDE3D9(*(uint32_t *)(v3 + 0x2C));
	*(uint32_t *)(v3 + 0x2C) = -1;

loc_8100073E:
//VADDR: 0x8100073e OFF: 0x73e
	if ((*(uint32_t *)(v3 + 0x30)) != 0)
		goto loc_81000750;
	a1 = SceLibKernel_244E76D2(v3 + 0x38);
	*(uint32_t *)(v3 + 0x30) = -1;

loc_81000750:
//VADDR: 0x81000750 OFF: 0x750
	a1 = v3 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a2 = *(uint32_t *)(v4);

loc_81000760:
//VADDR: 0x81000760 OFF: 0x760
	v2 = v2 + 4;
	FLAGS = v2;
	FLAGS = (v5 + 1) - (*(uint32_t *)(a2 + 0x60));
	if (C == 0) // unsigned <
		goto loc_81000716;

loc_8100076C:
//VADDR: 0x8100076c OFF: 0x76c
	if ((*(uint32_t *)(a2 + 0x34)) != 0)
		goto loc_81000780;
	a1 = SceLibKernel_244E76D2(a2 + 0x38);
	*(uint32_t *)((*(uint32_t *)(v4)) + 0x34) = -1;

loc_81000780:
//VADDR: 0x81000780 OFF: 0x780
	*(uint32_t *)(a2 + 0x64) = v1;
	*(uint32_t *)((/*data_81004008*/ 0x10000100)) = /*data_10000100*/;
	*(uint32_t *)((/*data_8100402C*/ 0x00000000)) = v1;
	a1 = v1 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x810007a4 OFF: 0x7a4
int sub_810007A4(int arg1)
{
	// push {r4, r5, r6, r7, r8, sb, lr}
	sp = sp - 0x14;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x60) = a1;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x64) = a2;
	a2 = 0x3320 + 0;
	FLAGS = a2;
	*(uint32_t *)(sp) = 0;
	a1 = SceLibKernel_DA6EC8EF((*(uint32_t *)(/*data_81004000*/)) + 0x38, 0x81000000 | a2, 0, 0, *(sp+0));
	FLAGS = a1 - 0;
	if (N == V) // signed >=
		goto loc_810007D8;
	goto loc_8100092A;

loc_810007D8:
//VADDR: 0x810007d8 OFF: 0x7d8
	*(uint32_t *)((*(uint32_t *)(v2)) + 0x34) = a1;
	v1 = /*data_801012FF*/;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(v2)) + 0x60)) - 0;
	if (Z == 1) // ==
		goto loc_81000928;
	v4 = v3 + 0;
	FLAGS = v4;

loc_810007EE:
//VADDR: 0x810007ee OFF: 0x7ee
	lr = v3;
	sb = a2;

loc_810007F4:
//VADDR: 0x810007f4 OFF: 0x7f4
	FLAGS = (*(uint32_t *)(sb)) - 0;
	if (Z == 0) // !=
		goto loc_81000900;
	a1 = 0x58;
	FLAGS = a1;
	a1 = r1(...);
	*(uint32_t *)((*(uint32_t *)(v2)) + v5) = a1;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(v2)) + v5)) - 0;
	if (Z == 0) // !=
		goto loc_81000828;
	v1 = /*data_80101242*/;
	goto loc_81000914;

loc_81000828:
//VADDR: 0x81000828 OFF: 0x828
	*(uint32_t *)(lr + 4) = v3;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5))) = v3;
	*(uint32_t *)(sp) = v3;
	a1 = SceLibKernel_DA6EC8EF(*(uint32_t *)((*(uint32_t *)(v2)) + v5), /*s_text_81003334*/ "SceIpcClientInfo", 0, 0, *(sp+0));
	FLAGS = a1 - 0;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x30) = a1;
	if (N == V) // signed >=
		goto loc_81000858;
	v1 = a1 + 0;
	FLAGS = v1;
	goto loc_81000914;

loc_81000858:
//VADDR: 0x81000858 OFF: 0x858
	*(uint32_t *)(sp) = v3;
	*(uint32_t *)(sp + 0x4) = (*(uint32_t *)((/*data_8100402C*/ 0x00000000)));
	*(uint32_t *)(sp + 8) = v3;
	a1 = SceLibKernel_C5C11EE7(/*s_text_81003348*/ "SceLocationCallback", sub_81001CA4, *(uint32_t *)((/*data_81004008*/ 0x10000100)), 0x1000, *(sp+0), *(sp+4), *(sp+8));
	FLAGS = a1 - 0;
	if (N == V) // signed >=
		goto loc_810008A0;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x28) = -1;
	v1 = a1 + 0;
	FLAGS = v1;
	goto loc_81000914;

loc_810008A0:
//VADDR: 0x810008a0 OFF: 0x8a0
	*(uint32_t *)(lr + 0x28) = a1;
	*(uint32_t *)(sp) = v3;
	*(uint32_t *)(sp + 0x4) = (*(uint32_t *)((/*data_8100402C*/ 0x00000000)));
	*(uint32_t *)(sp + 8) = v3;
	a1 = SceLibKernel_C5C11EE7(/*s_text_8100335C*/ "SceHeadingCallback", sub_81001E66, *(uint32_t *)((/*data_81004008*/ 0x10000100)), 0x1000, *(sp+0), *(sp+4), *(sp+8));
	FLAGS = (a1 + 0) - 0;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x2C) = (a1 + 0);
	if (N == V) // signed >=
		goto loc_810008FC;
	a1 = SceThreadmgr_1BBDE3D9(*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x28));
	v1 = *(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x2C);
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x28) = -1;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x2C) = -1;
	goto loc_81000914;

loc_810008FC:
//VADDR: 0x810008fc OFF: 0x8fc
	v1 = v3 + 0;
	FLAGS = v1;
	goto loc_8100091E;

loc_81000900:
//VADDR: 0x81000900 OFF: 0x900
	sb = sb + 4;
	v5 = v5 + 4;
	FLAGS = (lr + 1) - 8;
	if (C == 0) // unsigned <
		goto loc_810007F4;

loc_81000914:
//VADDR: 0x81000914 OFF: 0x914
	if (v1 == 0)
		goto loc_8100091E;
	a1 = sub_810006FA();
	a1 = v1 + 0;
	FLAGS = a1;
	goto loc_8100092A;

loc_8100091E:
//VADDR: 0x8100091e OFF: 0x91e
	FLAGS = (v4 + 1) - (*(uint32_t *)(a2 + 0x60));
	if (C == 0) // unsigned <
		goto loc_810007EE;

loc_81000928:
//VADDR: 0x81000928 OFF: 0x928
	a1 = v3 + 0;
	FLAGS = a1;

loc_8100092A:
//VADDR: 0x8100092a OFF: 0x92a
	sp = sp + 0x14;
	return a1; // pop {r4, r5, r6, r7, r8, sb, pc}
}

//VADDR: 0x81000930 OFF: 0x930
int sub_81000930()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000932 OFF: 0x932
int sub_81000932()
{
	// push {r4, r5, r6, r7, r8, lr}
	v5 = a2;
	v4 = a1 + 0;
	FLAGS = v4;
	if ((*(uint32_t *)((/*data_81004030*/ 0x00000000))) == 0)
		goto loc_81000950;
	a1 = /*data_8010124E*/;
	goto loc_81000B48;

loc_81000950:
//VADDR: 0x81000950 OFF: 0x950
	if (v4 == 0)
		goto loc_81000956;
	FLAGS = v4 - 8;
	if (C == 0 || Z == 1) // unsigned <=
		goto loc_81000960;

loc_81000956:
//VADDR: 0x81000956 OFF: 0x956
	a1 = /*data_8010124D*/;
	goto loc_81000B48;

loc_81000960:
//VADDR: 0x81000960 OFF: 0x960
	a1 = SceLibKernel_9F793F84(a1);
	FLAGS = (uint8_t)v5 - 1;
	if (Z == 0) // !=
		goto loc_81000980;
	a4 = 0x1000;
	FLAGS = a4;
	FLAGS = (a1 + 0) - 0x1800000;
	if (C == 1) // unsigned >=
		goto loc_8100097C;
	a4 = 0x2000;
	FLAGS = a4;

loc_8100097C:
//VADDR: 0x8100097c OFF: 0x97c
	a1 = a4 + 0;
	FLAGS = a1;
	goto loc_81000984;

loc_81000980:
//VADDR: 0x81000980 OFF: 0x980
	a1 = 0x1000;
	FLAGS = a1;

loc_81000984:
//VADDR: 0x81000984 OFF: 0x984
	*(uint32_t *)((/*data_8100400C*/ 0x00001000)) = a1;
	FLAGS = v1 - 0x2000000;
	if (C == 0) // unsigned <
		goto loc_810009AE;
	FLAGS = a3 - 2;
	if (Z == 0) // !=
		goto loc_810009AE;
	if ((*(uint8_t *)((/*data_81004034*/ 0x00000000))) != 0)
		goto loc_810009AE;
	a1 = /*data_8010124D*/;
	goto loc_81000B48;

loc_810009AE:
//VADDR: 0x810009ae OFF: 0x9ae
	if ((*(uint8_t *)((/*data_81004004*/ 0x00000001))) == 0)
		goto loc_81000A04;
	v2 = 0;
	FLAGS = v2;
	if ((*(uint32_t *)((/*data_81004028*/ 0x00000000))) != 0)
		goto loc_810009F2;
	*(uint32_t *)((/*data_81004020*/ 0x00000000)) = (*(uint32_t *)((/*text_81003318*/ 0x14F244B5)));
	*(uint32_t *)((/*data_81004020*/ 0x00000000) + 4) = (*(uint32_t *)((/*text_81003318*/ 0x14F244B5) + 4));
	a1 = sub_810025D0(a1, *(uint32_t *)((/*text_81003318*/ 0x14F244B5) + 4), *(uint32_t *)((/*text_81003318*/ 0x14F244B5)), /*data_81004020*/ 0x00000000);
	v2 = a1 + 0;
	FLAGS = v2;
	a3 = *(uint32_t *)((/*data_81004028*/ 0x00000000));
	if (Z == 0) // !=
		goto loc_810009FE;

loc_810009F2:
//VADDR: 0x810009f2 OFF: 0x9f2
	*(uint32_t *)((/*data_81004028*/ 0x00000000)) = (a3 + 1);

loc_810009FE:
//VADDR: 0x810009fe OFF: 0x9fe
	FLAGS = v2 - 0;
	if (Z == 0) // !=
		goto loc_81000B46;

loc_81000A04:
//VADDR: 0x81000a04 OFF: 0xa04
	FLAGS = (*(uint32_t *)(/*data_81004000*/)) - 0;
	if (Z == 0) // !=
		goto loc_81000A9E;
	a1 = 0x78;
	FLAGS = a1;
	a1 = r1(...);
	if (Z == 1) // ==
		goto loc_81000A50;
	a1 = SceLibKernel_632980D7((a1 + 0) + 0, 0, 0x20);
	*(uint32_t *)((a1 + 0) + 0x20) = 0;
	*(uint32_t *)((a1 + 0) + 0x24) = 0;
	*(uint32_t *)((a1 + 0) + 0x28) = 0;
	*(uint32_t *)((a1 + 0) + 0x2C) = 0;
	*(uint32_t *)((a1 + 0) + 0x58) = 0;
	*(uint32_t *)((a1 + 0) + 0x34) = -1;
	*(uint8_t *)((a1 + 0) + 0x5C) = 0;
	*(uint32_t *)((a1 + 0) + 0x60) = 0;
	*(uint32_t *)((a1 + 0) + 0x64) = 0;
	*(uint32_t *)((a1 + 0) + 0x68) = 0;
	*(uint32_t *)((a1 + 0) + 0x6C) = 0;
	*(uint32_t *)((a1 + 0) + 0x70) = 0;
	v2 = (a1 + 0) + 0;
	FLAGS = v2;

loc_81000A50:
//VADDR: 0x81000a50 OFF: 0xa50
	*(uint32_t *)(/*data_81004000*/) = v2;
	if (v2 != 0)
		goto loc_81000A9E;
	if ((*(uint8_t *)((/*data_81004004*/ 0x00000001))) == 0)
		goto loc_81000A94;
	FLAGS = (*(uint32_t *)((/*data_81004028*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_81000A86;
	a1 = sub_8100265C(*(uint32_t *)((/*data_81004028*/ 0x00000000)), a2, a3);
	a1 = *(uint32_t *)((/*data_81004028*/ 0x00000000));
	goto loc_81000A88;

loc_81000A86:
//VADDR: 0x81000a86 OFF: 0xa86
	if (a1 == 0)
		goto loc_81000A94;

loc_81000A88:
//VADDR: 0x81000a88 OFF: 0xa88
	*(uint32_t *)((/*data_81004028*/ 0x00000000)) = (a1 - 1);

loc_81000A94:
//VADDR: 0x81000a94 OFF: 0xa94
	a1 = /*data_80101242*/;
	goto loc_81000B48;

loc_81000A9E:
//VADDR: 0x81000a9e OFF: 0xa9e
	FLAGS = v1 - 0x1500000;
	if (C == 1) // unsigned >=
		goto loc_81000AB0;
	*(uint32_t *)((/*data_81004030*/ 0x00000000)) = 1;

loc_81000AB0:
//VADDR: 0x81000ab0 OFF: 0xab0
	FLAGS = v1 - 0x2100000;
	if (C == 1) // unsigned >=
		goto loc_81000ABC;
	a2 = v5 | 0x80000000;
	FLAGS = a2;

loc_81000ABC:
//VADDR: 0x81000abc OFF: 0xabc
	a1 = sub_810007A4(v4 + 0);
	v2 = a1 + 0;
	FLAGS = v2;
	v4 = *(uint32_t *)(/*data_81004000*/);
	if (Z == 0) // !=
		goto loc_81000AE4;
	FLAGS = v1 - 0x1500000;
	if (C == 0) // unsigned <
		goto loc_81000AE4;
	*(uint32_t *)((/*data_81004030*/ 0x00000000)) = 1;
	goto loc_81000B46;

loc_81000AE4:
//VADDR: 0x81000ae4 OFF: 0xae4
	FLAGS = v2 - 0;
	if (Z == 1) // ==
		goto loc_81000B46;
	if (v4 == 0)
		goto loc_81000B02;
	a1 = SceLibKernel_632980D7(v4 + 0, 0, 0x20);
	a1 = v4 + 0;
	FLAGS = a1;
	a1 = r1(...);

loc_81000B02:
//VADDR: 0x81000b02 OFF: 0xb02
	*(uint32_t *)(/*data_81004000*/) = 0;
	if ((*(uint8_t *)((/*data_81004004*/ 0x00000001))) == 0)
		goto loc_81000B46;
	FLAGS = (*(uint32_t *)((/*data_81004028*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_81000B38;
	a1 = sub_8100265C(/*data_81004028*/ 0x00000000, *(uint32_t *)((/*data_81004028*/ 0x00000000)), 0);
	a2 = *(uint32_t *)((/*data_81004028*/ 0x00000000));
	goto loc_81000B3A;

loc_81000B38:
//VADDR: 0x81000b38 OFF: 0xb38
	if (a2 == 0)
		goto loc_81000B46;

loc_81000B3A:
//VADDR: 0x81000b3a OFF: 0xb3a
	*(uint32_t *)((/*data_81004028*/ 0x00000000)) = (a2 - 1);

loc_81000B46:
//VADDR: 0x81000b46 OFF: 0xb46
	a1 = v2 + 0;
	FLAGS = a1;

loc_81000B48:
//VADDR: 0x81000b48 OFF: 0xb48
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x81000b4c OFF: 0xb4c
int sub_81000B4C(int arg1)
{
	// push {r4, lr}
	v1 = a1 + 0;
	FLAGS = v1;
	if ((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x6C)) != 0)
		goto loc_81000B68;
	a1 = /*data_801012FF*/;
	goto loc_81000BE2;

loc_81000B68:
//VADDR: 0x81000b68 OFF: 0xb68
	FLAGS = (*(uint8_t *)(a2 + 0x5C)) - 1;
	if (Z == 0) // !=
		goto loc_81000B8C;
	a1 = r1(...);
	FLAGS = a1 - /*data_8002050A*/;
	if (Z == 0) // !=
		goto loc_81000B8C;
	a1 = /*data_801012FF*/;
	goto loc_81000BE2;

loc_81000B8C:
//VADDR: 0x81000b8c OFF: 0xb8c
	FLAGS = (*(uint32_t *)(v1 + 0x24)) - 1;
	if (Z == 0) // !=
		goto loc_81000BA0;
	*(uint32_t *)(v1 + 0x24) = 0;
	a1 = SceLibKernel_DDB395A9(*(uint32_t *)(v1 + 0x2C), 0, 0);

loc_81000BA0:
//VADDR: 0x81000ba0 OFF: 0xba0
	*(uint32_t *)(v1 + 0xC) = 0;
	*(uint32_t *)(v1 + 0x1C) = 0;
	*(uint32_t *)(v1 + 0x14) = 0;
	FLAGS = (*(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C)) - 1;
	if (Z == 0) // !=
		goto loc_81000BE0;
	a1 = r1(...);
	a1 = *(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x6C);
	a1 = r1(...);
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x6C) = 0;

loc_81000BE0:
//VADDR: 0x81000be0 OFF: 0xbe0
	a1 = 0;
	FLAGS = a1;

loc_81000BE2:
//VADDR: 0x81000be2 OFF: 0xbe2
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000be4 OFF: 0xbe4
int sub_81000BE4(int arg1)
{
	// push {r4, r5, r6, lr}
	if ((*(uint32_t *)((a1 + 0) + 4)) != 0)
		goto loc_81000BF6;
	a1 = /*data_801012FF*/;
	goto loc_81000C70;

loc_81000BF6:
//VADDR: 0x81000bf6 OFF: 0xbf6
	a1 = r1(...);
	FLAGS = a1 - /*data_8002050A*/;
	if (Z == 0) // !=
		goto loc_81000C12;
	a1 = /*data_801012FF*/;
	goto loc_81000C70;

loc_81000C12:
//VADDR: 0x81000c12 OFF: 0xc12
	FLAGS = (*(uint32_t *)(v1 + 0x20)) - 1;
	if (Z == 0) // !=
		goto loc_81000C26;
	*(uint32_t *)(v1 + 0x20) = 0;
	a1 = SceLibKernel_DDB395A9(*(uint32_t *)(v1 + 0x28), 0, 0);

loc_81000C26:
//VADDR: 0x81000c26 OFF: 0xc26
	*(uint32_t *)(v1 + 8) = 0;
	*(uint32_t *)(v1 + 0x18) = 0;
	*(uint32_t *)(v1 + 0x10) = 0;
	a1 = r2(...);
	a1 = *(uint32_t *)(v1 + 4);
	a1 = r1(...);
	*(uint32_t *)(v1 + 4) = 0;
	if ((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x2C)) == 0)
		goto loc_81000C6E;
	a1 = r1(...);
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x2C) = 0;

loc_81000C6E:
//VADDR: 0x81000c6e OFF: 0xc6e
	a1 = 0;
	FLAGS = a1;

loc_81000C70:
//VADDR: 0x81000c70 OFF: 0xc70
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81000c72 OFF: 0xc72
int sub_81000C72(int arg1)
{
	// push {r4, r5, r6, r7, r8, lr}
	v1 = a1 + 0;
	FLAGS = v1;
	a1 = SceLibKernel_46E7BE7B((*(uint32_t *)(/*data_81004000*/)) + 0x38, 1, 0);
	a3 = (0 + 0) + 0;
	FLAGS = a3;
	a2 = (*(uint32_t *)(/*data_81004000*/)) + 0;
	FLAGS = a2;

loc_81000C98:
//VADDR: 0x81000c98 OFF: 0xc98
	if ((*(uint32_t *)(a2)) == 0)
		goto loc_81000CD0;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)))) - v1;
	if (Z == 0) // !=
		goto loc_81000CD0;
	a1 = sub_81000BE4((*(uint32_t *)(a2)) + 0);
	a1 = sub_81000B4C((*(uint32_t *)(a2)) + 0);
	*(uint32_t *)((*(uint32_t *)(a2))) = -1;
	v4 = a1 + 0;
	FLAGS = v4;
	a1 = *(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C);
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x58) = v2;
	*(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C) = (a1 - 1);
	a4 = *(uint32_t *)(/*data_81004000*/);
	goto loc_81000CD8;

loc_81000CD0:
//VADDR: 0x81000cd0 OFF: 0xcd0
	a2 = a2 + 4;
	FLAGS = a2;
	FLAGS = (a3 + 1) - 8;
	if (N != V) // signed <
		goto loc_81000C98;

loc_81000CD8:
//VADDR: 0x81000cd8 OFF: 0xcd8
	a1 = SceLibKernel_91FA6614(a4 + 0x38, 1);
	a1 = v4 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x81000ce8 OFF: 0xce8
int sub_81000CE8(int arg1, int arg2)
{
	// push {r4, r5, r6, r7, r8, lr}
	a1 = sub_81001FC8(*(uint32_t *)((a2 + 0) + 4), *(uint32_t *)((a2 + 0)));
	v4 = *(uint32_t *)((a2 + 0));
	a1 = *(uint32_t *)(/*data_81004000*/);
	a1 = SceLibKernel_46E7BE7B(a1 + 0x38, 1, 0);
	a3 = (0 + 0) + 0;
	FLAGS = a3;
	a2 = (*(uint32_t *)(/*data_81004000*/)) + 0;
	FLAGS = a2;

loc_81000D16:
//VADDR: 0x81000d16 OFF: 0xd16
	if ((*(uint32_t *)(a2)) == 0)
		goto loc_81000D50;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)))) - v4;
	if (Z == 0) // !=
		goto loc_81000D50;
	a1 = sub_81000BE4((*(uint32_t *)(a2)) + 0);
	a1 = sub_81000B4C((*(uint32_t *)(a2)) + 0);
	*(uint32_t *)((*(uint32_t *)(a2))) = -1;
	v3 = a1 + 0;
	FLAGS = v3;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x58) = v1;
	a3 = *(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C);
	*(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C) = (a3 - 1);
	a4 = *(uint32_t *)(/*data_81004000*/);
	goto loc_81000D58;

loc_81000D50:
//VADDR: 0x81000d50 OFF: 0xd50
	a2 = a2 + 4;
	FLAGS = a2;
	FLAGS = (a3 + 1) - 8;
	if (N != V) // signed <
		goto loc_81000D16;

loc_81000D58:
//VADDR: 0x81000d58 OFF: 0xd58
	a1 = SceLibKernel_91FA6614(a4 + 0x38, 1);
	a1 = v3 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x81000d68 OFF: 0xd68
int sub_81000D68()
{
	// push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sp = sp - 4;
	if ((*(uint32_t *)((/*data_81004030*/ 0x00000000))) != 0)
		goto loc_81000D84;
	a1 = /*data_8010124F*/;
	goto loc_81000E38;

loc_81000D84:
//VADDR: 0x81000d84 OFF: 0xd84
	sb = 0;

loc_81000D90:
//VADDR: 0x81000d90 OFF: 0xd90
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + sb)) - 0;
	if (Z == 1) // ==
		goto loc_81000E2A;
	FLAGS = (*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + sb)))) - 0;
	if (Z == 1) // ==
		goto loc_81000E2A;
	FLAGS = (*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + sb)))) - -1;
	if (Z == 1) // ==
		goto loc_81000E2A;
	a1 = sub_81001FC8(*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + sb)) + 4), (*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + sb)))) + 0);
	v2 = *(uint32_t *)((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + sb)));
	a1 = *(uint32_t *)(/*data_81004000*/);
	a1 = SceLibKernel_46E7BE7B(a1 + 0x38, 1, 0);
	ip = sl;
	lr = (*(uint32_t *)(/*data_81004000*/));

loc_81000DD4:
//VADDR: 0x81000dd4 OFF: 0xdd4
	if ((*(uint32_t *)(lr)) == 0)
		goto loc_81000E10;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(lr)))) - v2;
	if (Z == 0) // !=
		goto loc_81000E10;
	a1 = sub_81000BE4((*(uint32_t *)(lr)) + 0);
	a1 = sub_81000B4C((*(uint32_t *)(lr)) + 0);
	*(uint32_t *)((*(uint32_t *)(lr))) = -1;
	v3 = a1 + 0;
	FLAGS = v3;
	a1 = *(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C);
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x58) = fp;
	*(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C) = (a1 - 1);
	v4 = *(uint32_t *)(/*data_81004000*/);
	goto loc_81000E1E;

loc_81000E10:
//VADDR: 0x81000e10 OFF: 0xe10
	lr = lr + 4;
	FLAGS = (ip + 1) - 8;
	if (N != V) // signed <
		goto loc_81000DD4;

loc_81000E1E:
//VADDR: 0x81000e1e OFF: 0xe1e
	a1 = SceLibKernel_91FA6614(v4 + 0x38, 1);
	a1 = v3 + 0;
	FLAGS = a1;

loc_81000E2A:
//VADDR: 0x81000e2a OFF: 0xe2a
	sb = sb + 4;
	FLAGS = (v5 + 1) - 8;
	if (N != V) // signed <
		goto loc_81000D90;

loc_81000E38:
//VADDR: 0x81000e38 OFF: 0xe38
	sp = sp + 4;
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
}

//VADDR: 0x81000e3e OFF: 0xe3e
int sub_81000E3E()
{
	// push {r4, r5, r6, lr}
	if ((*(uint32_t *)((/*data_81004030*/ 0x00000000))) != 0)
		goto loc_81000E56;
	a1 = /*data_8010124F*/;
	goto loc_81000ED6;

loc_81000E56:
//VADDR: 0x81000e56 OFF: 0xe56
	a1 = sub_81000D68();
	a1 = sub_810006FA();
	v1 = a1 + 0;
	FLAGS = v1;
	if ((*(uint32_t *)(/*data_81004000*/)) == 0)
		goto loc_81000E84;
	a1 = SceLibKernel_632980D7((*(uint32_t *)(/*data_81004000*/)) + 0, 0, 0x20);
	a1 = (*(uint32_t *)(/*data_81004000*/)) + 0;
	FLAGS = a1;
	a1 = r1(...);

loc_81000E84:
//VADDR: 0x81000e84 OFF: 0xe84
	*(uint32_t *)(/*data_81004000*/) = 0;
	if ((*(uint8_t *)((/*data_81004004*/ 0x00000001))) == 0)
		goto loc_81000EC8;
	FLAGS = (*(uint32_t *)((/*data_81004028*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_81000EBA;
	a1 = sub_8100265C(/*data_81004028*/ 0x00000000, *(uint32_t *)((/*data_81004028*/ 0x00000000)), 0);
	a2 = *(uint32_t *)((/*data_81004028*/ 0x00000000));
	goto loc_81000EBC;

loc_81000EBA:
//VADDR: 0x81000eba OFF: 0xeba
	if (a2 == 0)
		goto loc_81000EC8;

loc_81000EBC:
//VADDR: 0x81000ebc OFF: 0xebc
	*(uint32_t *)((/*data_81004028*/ 0x00000000)) = (a2 - 1);

loc_81000EC8:
//VADDR: 0x81000ec8 OFF: 0xec8
	*(uint32_t *)((/*data_81004030*/ 0x00000000)) = 0;
	a1 = v1 + 0;
	FLAGS = a1;

loc_81000ED6:
//VADDR: 0x81000ed6 OFF: 0xed6
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81000ed8 OFF: 0xed8
int sub_81000ED8(int arg1)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0xC8;
	*(uint32_t *)(sp + 0xC0) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v4 = a1 + 0;
	FLAGS = v4;
	FLAGS = (*(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C)) - 1;
	if (Z == 0) // !=
		goto loc_81001016;
	*(uint32_t *)(sp + 0x18) = 1;
	*(uint32_t *)(sp + 0x18 + 0x4) = 0xF00;
	*(uint32_t *)(sp + 0x20) = 0xF00;
	*(uint32_t *)(sp + 0x20 + 0x4) = 1;
	a3 = 4;
	FLAGS = a3;
	*(uint32_t *)(sp + 0x28) = 0xF00;
	*(uint32_t *)(sp + 0x28 + 0x4) = 0xF00;
	a4 = sp + 0x38;
	FLAGS = a4;
	*(uint32_t *)(sp + 0x30) = 1;
	*(uint32_t *)(sp + 0x30 + 0x4) = 0;

loc_81000F22:
//VADDR: 0x81000f22 OFF: 0xf22
	*(uint32_t *)(a2 + 0x38) = v1;
	a3 = a3 - 1;
	FLAGS = a3;
	*(uint32_t *)(a4 + 4) = v1;
	a2 = a2 + 0x20;
	*(uint32_t *)(a4 + 8) = v1;
	*(uint32_t *)(a4 + 0xC) = v1;
	*(uint32_t *)(a4 + 0x10) = v1;
	*(uint32_t *)(a4 + 0x14) = v1;
	*(uint32_t *)(a4 + 0x18) = v1;
	*(uint32_t *)(a4 + 0x1C) = v1;
	if (Z == 0) // !=
		goto loc_81000F22;
	*(uint8_t *)(sp + 4) = v1;
	*(uint32_t *)(sp + 0xB8) = 2;
	*(uint32_t *)(sp + 0xB8 + 0x4) = -1;
	a1 = SceLibKernel_C458D60A((sp + 4) + 0, /*s_text_81003370*/ "SceLocationHead", 0x10, a4 + 0x20);
	a1 = SceIpmi_4E255C31((sp + 4) + 0);
	a1 = r1(...);
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_81000F7E;
	a1 = /*data_80101242*/;
	goto loc_8100101A;

loc_81000F7E:
//VADDR: 0x81000f7e OFF: 0xf7e
	a1 = SceIpmi_B282B430((*(uint32_t *)(/*data_81004000*/)) + 0x6C, sp + 4, *(uint32_t *)(/*data_81004000*/), v2 + 0);
	if (Z == 1) // ==
		goto loc_81000FAA;
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = (a1 + 0) + 0;
	FLAGS = a1;
	goto loc_8100101A;

loc_81000FAA:
//VADDR: 0x81000faa OFF: 0xfaa
	a4 = sp + 0;
	a3 = 0;
	FLAGS = a3;
	a2 = 0;
	FLAGS = a2;
	a1 = lr(...);
	a2 = *(uint32_t *)(/*data_81004000*/);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001016;
	FLAGS = a1 - /*data_80020581*/;
	v1 = /*data_801012FF*/;
	if (Z == 0 && N == V) // signed >
		goto loc_81000FEC;
	FLAGS = a1 - /*data_80020580*/;
	if (Z == 1 || N != V) // signed <=
		goto loc_81000FFC;
	goto loc_81000FFA;

loc_81000FEC:
//VADDR: 0x81000fec OFF: 0xfec
	FLAGS = a1 - /*data_80020583*/;
	if (Z == 0) // !=
		goto loc_81000FFC;
	goto loc_81000FFC;

loc_81000FFA:
//VADDR: 0x81000ffa OFF: 0xffa
	v1 = a1 + 0;
	FLAGS = v1;

loc_81000FFC:
//VADDR: 0x81000ffc OFF: 0xffc
	a1 = r1(...);
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = v1 + 0;
	FLAGS = a1;
	goto loc_8100101A;

loc_81001016:
//VADDR: 0x81001016 OFF: 0x1016
	*(uint32_t *)(v4 + 0x24) = v1;
	a1 = v1 + 0;
	FLAGS = a1;

loc_8100101A:
//VADDR: 0x8100101a OFF: 0x101a
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0xC0));
	if (Z == 0) // !=
		goto loc_8100102A;
	sp = sp + 0xC8;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_8100102A:
//VADDR: 0x8100102a OFF: 0x102a
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81001030 OFF: 0x1030
int sub_81001030(int arg1, int arg2)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0xC8;
	*(uint32_t *)(sp + 0xC0) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	if ((*(uint32_t *)((a2 + 0) + 4)) == 0)
		goto loc_81001054;
	a1 = /*data_801012FF*/;
	goto loc_81001170;

loc_81001054:
//VADDR: 0x81001054 OFF: 0x1054
	*(uint32_t *)(sp + 0x18) = 1;
	*(uint32_t *)(sp + 0x18 + 0x4) = 0xF00;
	*(uint32_t *)(sp + 0x20) = 0xF00;
	*(uint32_t *)(sp + 0x20 + 0x4) = 1;
	*(uint32_t *)(sp + 0x28) = 0xF00;
	*(uint32_t *)(sp + 0x28 + 0x4) = 0xF00;
	a4 = 4;
	FLAGS = a4;
	*(uint32_t *)(sp + 0x30) = 1;
	*(uint32_t *)(sp + 0x30 + 0x4) = 0;
	lr = sp + 0x38;

loc_81001074:
//VADDR: 0x81001074 OFF: 0x1074
	*(uint32_t *)(a3 + 0x38) = v1;
	*(uint32_t *)(lr + 4) = v1;
	a3 = a3 + 0x20;
	*(uint32_t *)(lr + 8) = v1;
	*(uint32_t *)(lr + 0xC) = v1;
	*(uint32_t *)(lr + 0x10) = v1;
	*(uint32_t *)(lr + 0x14) = v1;
	*(uint32_t *)(lr + 0x18) = v1;
	*(uint32_t *)(lr + 0x1C) = v1;
	lr = lr + 0x20;
	if (Z == 0) // !=
		goto loc_81001074;
	*(uint32_t *)(sp + 0xB8) = v1;
	*(uint32_t *)(sp + 0xB8 + 0x4) = -1;
	*(uint8_t *)(sp + 4) = v1;
	a1 = SceLibKernel_C458D60A((sp + 4) + 0, /*s_text_81003380*/ "SceLocationLoc", 0x10, a4 - 1);
	*(uint32_t *)(sp + 0xB8) = 2;
	a1 = SceIpmi_4E255C31((sp + 4) + 0);
	a1 = r1(...);
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_810010E0;
	a1 = /*data_80101242*/;
	goto loc_81001170;

loc_810010E0:
//VADDR: 0x810010e0 OFF: 0x10e0
	a1 = SceIpmi_B282B430(v4 + 4, sp + 4, *(uint32_t *)(/*data_81004000*/), v2 + 0);
	if (Z == 1) // ==
		goto loc_8100110A;
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = (a1 + 0) + 0;
	FLAGS = a1;
	goto loc_81001170;

loc_8100110A:
//VADDR: 0x8100110a OFF: 0x110a
	*(uint32_t *)(sp) = v1;
	a4 = sp + 0;
	a3 = 0;
	FLAGS = a3;
	a2 = 0;
	FLAGS = a2;
	a1 = r6(...);
	a3 = *(uint32_t *)(v4 + 4);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100116C;
	FLAGS = a1 - /*data_80020581*/;
	v1 = /*data_801012FF*/;
	if (Z == 0 && N == V) // signed >
		goto loc_81001142;
	FLAGS = a1 - /*data_80020580*/;
	if (Z == 1 || N != V) // signed <=
		goto loc_81001152;
	goto loc_81001150;

loc_81001142:
//VADDR: 0x81001142 OFF: 0x1142
	FLAGS = a1 - /*data_80020583*/;
	if (Z == 0) // !=
		goto loc_81001152;
	goto loc_81001152;

loc_81001150:
//VADDR: 0x81001150 OFF: 0x1150
	v1 = a1 + 0;
	FLAGS = v1;

loc_81001152:
//VADDR: 0x81001152 OFF: 0x1152
	a1 = a3 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = v1 + 0;
	FLAGS = a1;
	goto loc_81001170;

loc_8100116C:
//VADDR: 0x8100116c OFF: 0x116c
	*(uint32_t *)(v4 + 0x20) = v1;
	a1 = v1 + 0;
	FLAGS = a1;

loc_81001170:
//VADDR: 0x81001170 OFF: 0x1170
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0xC0));
	if (Z == 0) // !=
		goto loc_81001180;
	sp = sp + 0xC8;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_81001180:
//VADDR: 0x81001180 OFF: 0x1180
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81001186 OFF: 0x1186
int sub_81001186(int arg1)
{
	// push {r4, r5, r6, r7, r8, lr}
	v1 = a1 + 0;
	FLAGS = v1;
	if ((*(uint32_t *)((/*data_81004030*/ 0x00000000))) != 0)
		goto loc_810011A2;
	a1 = /*data_8010124F*/;
	goto loc_81001290;

loc_810011A2:
//VADDR: 0x810011a2 OFF: 0x11a2
	a1 = *(uint32_t *)(/*data_81004000*/);
	a1 = SceLibKernel_46E7BE7B(a1 + 0x38, 1, 0);
	FLAGS = a1 - 0;
	if (N != V) // signed <
		goto loc_8100128C;
	v3 = 0 + 0;
	FLAGS = v3;
	if ((*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x60)) == 0)
		goto loc_81001204;

loc_810011CC:
//VADDR: 0x810011cc OFF: 0x11cc
	if ((*(uint32_t *)((*(uint32_t *)(a1 + (a3 << 0x2))) + 4)) != 0)
		goto loc_810011FC;
	*(uint32_t *)(v1) = (*(uint32_t *)(a1 + (a3 << 0x2)));
	a2 = *(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C);
	*(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C) = (a2 + 1);
	a1 = sub_81001030(*(uint32_t *)(/*data_81004000*/), *(uint32_t *)((a3 << 2) + a1));
	a2 = *(uint32_t *)(/*data_81004000*/);
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_8100121A;
	goto loc_8100123E;

loc_810011FC:
//VADDR: 0x810011fc OFF: 0x11fc
	FLAGS = ((int8_t)(a3 + 1)) - a4;
	if (C == 0) // unsigned <
		goto loc_810011CC;

loc_81001204:
//VADDR: 0x81001204 OFF: 0x1204
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (N != V) // signed <
		goto loc_8100128C;
	a1 = /*data_80101243*/;
	goto loc_81001290;

loc_8100121A:
//VADDR: 0x8100121a OFF: 0x121a
	a1 = *(uint8_t *)(a2 + 0x5C);
	*(uint8_t *)(a2 + 0x5C) = (a1 - 1);
	a1 = *(uint32_t *)(/*data_81004000*/);
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (N != V) // signed <
		goto loc_8100128C;
	a1 = v1 + 0;
	FLAGS = a1;
	goto loc_81001290;

loc_8100123E:
//VADDR: 0x8100123e OFF: 0x123e
	a1 = sub_81000ED8(*(uint32_t *)(a2 + v2));
	if (Z == 1) // ==
		goto loc_8100127E;
	a1 = sub_81000BE4(*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + v2));
	a3 = *(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C);
	*(uint8_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x5C) = (a3 - 1);
	a1 = *(uint32_t *)(/*data_81004000*/);
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (N != V) // signed <
		goto loc_8100128C;
	a1 = (a1 + 0) + 0;
	FLAGS = a1;
	goto loc_81001290;

loc_8100127E:
//VADDR: 0x8100127e OFF: 0x127e
	a1 = a2 + 0;
	FLAGS = a1;
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (N == V) // signed >=
		goto loc_8100128E;

loc_8100128C:
//VADDR: 0x8100128c OFF: 0x128c
	goto loc_81001290;

loc_8100128E:
//VADDR: 0x8100128e OFF: 0x128e
	a1 = v3 + 0;
	FLAGS = a1;

loc_81001290:
//VADDR: 0x81001290 OFF: 0x1290
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x81001294 OFF: 0x1294
int sub_81001294()
{
	a1 = *(uint32_t *)(/*data_81004000*/);
	return a1;
}

//VADDR: 0x810012a0 OFF: 0x12a0
int sub_810012A0()
{
	// push {r4, r5, r6, lr}
	if ((*(uint32_t *)(/*data_81004000*/)) == 0)
		goto loc_810012B8;
	FLAGS = (a1 + 0) - -1;
	if (Z == 1) // ==
		goto loc_810012B8;
	if ((a1 + 0) != 0)
		goto loc_810012BC;

loc_810012B8:
//VADDR: 0x810012b8 OFF: 0x12b8
	a1 = 0;
	FLAGS = a1;
	goto loc_8100130A;

loc_810012BC:
//VADDR: 0x810012bc OFF: 0x12bc
	a1 = SceLibKernel_46E7BE7B(v2 + 0x38, 1, 0);
	a1 = (*(uint32_t *)(/*data_81004000*/)) + 0x38;
	FLAGS = a1;
	a3 = 0 + 0;
	FLAGS = a3;

loc_810012DA:
//VADDR: 0x810012da OFF: 0x12da
	if ((*(uint32_t *)(a2)) == 0)
		goto loc_810012F8;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)))) - v1;
	if (Z == 0) // !=
		goto loc_810012F8;
	a1 = SceLibKernel_91FA6614(a1, 1);
	a1 = *(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + v2);
	goto loc_8100130A;

loc_810012F8:
//VADDR: 0x810012f8 OFF: 0x12f8
	a2 = a2 + 4;
	FLAGS = a2;
	v2 = v2 + 4;
	FLAGS = v2;
	FLAGS = (a3 + 1) - 8;
	if (N != V) // signed <
		goto loc_810012DA;
	a1 = SceLibKernel_91FA6614(a1, 1);
	a1 = 0;
	FLAGS = a1;

loc_8100130A:
//VADDR: 0x8100130a OFF: 0x130a
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x8100130c OFF: 0x130c
int sub_8100130C()
{
	*(uint32_t *)(/*data_81004000*/) = a1;
	*(uint32_t *)((/*data_81004030*/ 0x00000000)) = 2;
	return a1;
}

//VADDR: 0x81001324 OFF: 0x1324
int sub_81001324(int arg1, int arg2, int arg3, int arg4, int arg5)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x18;
	*(uint32_t *)(sp + 0x10) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp) = (*(uint32_t *)((a1 + 0) + 0x64));
	*(uint32_t *)(sp + 0x4) = (sp + 8);
	a1 = sub_81001F54((*(uint32_t *)((a3 + 0) + 4)) + 0, (a2 + 0) + 0, a4 + 0, (*(uint32_t *)(sp + 0x30)) + 0, *(sp+0));
	*(uint32_t *)((a3 + 0)) = (*(uint32_t *)((a2 + 0)));
	FLAGS = (*(uint32_t *)(sp + 8)) - /*data_80101249*/;
	a2 = *(uint32_t *)((a1 + 0) + 0x64);
	a3 = *(uint32_t *)((a3 + 0) + 4);
	if (Z == 0) // !=
		goto loc_81001370;
	*(uint32_t *)((a1 + 0) + 0x58) = 1;

loc_81001370:
//VADDR: 0x81001370 OFF: 0x1370
	a2 = a2 & ~0xFF;
	FLAGS = a2;
	FLAGS = a1 - /*data_80101203*/;
	if (Z == 0) // !=
		goto loc_810013C6;
	if ((a2 & ~0xFF) != 0)
		goto loc_810013C6;
	*(uint32_t *)(v4 + 0x20) = 1;
	*(uint32_t *)(v4 + 0x24) = 0;
	a1 = a3 + 0;
	FLAGS = a1;
	a2 = 0;
	FLAGS = a2;
	*(uint32_t *)(sp) = (sp + 0xC);
	*(uint32_t *)(sp + 0x4) = 0;
	a3 = 0x280;
	FLAGS = a3;
	a4 = 5;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_810013C6;
	a3 = 2;
	FLAGS = a3;
	FLAGS = (*(uint32_t *)(sp + 0xC)) - 0x200;
	if (Z == 0) // !=
		goto loc_810013B2;
	*(uint32_t *)(v4 + 0x24) = 2;
	goto loc_810013C4;

loc_810013B2:
//VADDR: 0x810013b2 OFF: 0x13b2
	FLAGS = a2 - 0x80;
	if (Z == 0) // !=
		goto loc_810013BC;
	*(uint32_t *)(v4 + 0x24) = 1;
	goto loc_810013C4;

loc_810013BC:
//VADDR: 0x810013bc OFF: 0x13bc
	a1 = /*data_801012FF*/;

loc_810013C4:
//VADDR: 0x810013c4 OFF: 0x13c4
	*(uint32_t *)(v4 + 0x20) = a3;

loc_810013C6:
//VADDR: 0x810013c6 OFF: 0x13c6
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x10));
	if (Z == 0) // !=
		goto loc_810013D6;
	sp = sp + 0x18;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_810013D6:
//VADDR: 0x810013d6 OFF: 0x13d6
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810013dc OFF: 0x13dc
int sub_810013DC(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, r7, r8, sb, sl, lr}
	sp = sp - 0x10;
	*(uint32_t *)(sp + 0xC) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v5 = a3;
	v3 = a1 + 0;
	FLAGS = v3;
	sb = a4;
	a1 = SceLibKernel_46E7BE7B(((a2 + 0) + 0x38) + 0, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100140C;
	goto loc_81001494;

loc_8100140C:
//VADDR: 0x8100140c OFF: 0x140c
	*(uint32_t *)(v4 + 8) = 0;
	*(uint32_t *)(v4 + 0xC) = 0;
	*(uint32_t *)(v4 + 0x14) = 0;
	*(uint32_t *)(v4 + 0x18) = 0;
	*(uint32_t *)(v4 + 0x10) = 0;
	*(uint32_t *)(v4 + 0x1C) = 0;
	a1 = SceLibKernel_91FA6614(v2 + 0, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001428;
	goto loc_81001494;

loc_81001428:
//VADDR: 0x81001428 OFF: 0x1428
	a1 = sub_81002018(*(uint32_t *)(v4 + 4), *(uint32_t *)(v4), v5, sb);
	FLAGS = a1 - /*data_80101203*/;
	if (Z == 0) // !=
		goto loc_81001494;
	FLAGS = ((*(uint32_t *)(v3 + 0x64)) & ~0xFF) - 0;
	if (Z == 0) // !=
		goto loc_81001494;
	*(uint32_t *)(v3 + 0x20) = 1;
	*(uint32_t *)(v3 + 0x24) = 0;
	a1 = (*(uint32_t *)(v4 + 4)) + 0;
	FLAGS = a1;
	a2 = 0;
	FLAGS = a2;
	*(uint32_t *)(sp) = (sp + 8);
	*(uint32_t *)(sp + 0x4) = 0;
	a3 = 0x280;
	FLAGS = a3;
	a4 = 5;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001494;
	a3 = 2;
	FLAGS = a3;
	FLAGS = (*(uint32_t *)(sp + 8)) - 0x200;
	if (Z == 0) // !=
		goto loc_81001480;
	*(uint32_t *)(v3 + 0x24) = 2;
	goto loc_81001492;

loc_81001480:
//VADDR: 0x81001480 OFF: 0x1480
	FLAGS = a2 - 0x80;
	if (Z == 0) // !=
		goto loc_8100148A;
	*(uint32_t *)(v3 + 0x24) = 1;
	goto loc_81001492;

loc_8100148A:
//VADDR: 0x8100148a OFF: 0x148a
	a1 = /*data_801012FF*/;

loc_81001492:
//VADDR: 0x81001492 OFF: 0x1492
	*(uint32_t *)(v3 + 0x20) = a3;

loc_81001494:
//VADDR: 0x81001494 OFF: 0x1494
	FLAGS = (*(uint32_t *)(sl)) - (*(uint32_t *)(sp + 0xC));
	if (Z == 0) // !=
		goto loc_810014A4;
	sp = sp + 0x10;
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, pc}

loc_810014A4:
//VADDR: 0x810014a4 OFF: 0x14a4
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810014aa OFF: 0x14aa
int sub_810014AA(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, lr}
	a1 = sub_8100206C(*(uint32_t *)(a2 + 4), *(uint32_t *)(a2));
	return a1; // pop {r4, pc}
}

//VADDR: 0x810014b6 OFF: 0x14b6
int sub_810014B6(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sp = sp - 0x14;
	*(uint32_t *)(sp + 0x10) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0xC) = a4;
	a1 = sub_81002522(*(uint32_t *)(a2 + 4), *(uint32_t *)(a2));
	a4 = *(uint32_t *)(a2 + 4);
	FLAGS = (a1 + 0) - /*data_80101287*/;
	if (Z == 1) // ==
		goto loc_81001508;
	FLAGS = ((*(uint32_t *)((a1 + 0) + 0x64)) & ~0xFF) - 0x100;
	if (Z == 0) // !=
		goto loc_810015D0;
	FLAGS = (a1 + 0) - /*data_80101203*/;
	if (Z == 0) // !=
		goto loc_810015D0;

loc_81001508:
//VADDR: 0x81001508 OFF: 0x1508
	v4 = 0;
	FLAGS = v4;
	a4 = *(uint32_t *)((*(uint32_t *)((a4 + 0))) + 0x2C);
	sb = 1;
	FLAGS = sb;
	sl = 2;
	FLAGS = sl;

loc_81001518:
//VADDR: 0x81001518 OFF: 0x1518
	*(uint32_t *)(sp) = (sp + 8);
	*(uint32_t *)(sp + 0x4) = v4;
	a2 = 0;
	FLAGS = a2;
	a3 = 0x1E6;
	FLAGS = a3;
	a4 = 5;
	FLAGS = a4;
	a1 = lr(...);
	FLAGS = a1 - 0;
	a1 = *(uint32_t *)(v5 + 4);
	if (Z == 0) // !=
		goto loc_810015CA;
	FLAGS = (*(uint32_t *)(sp + 8)) - 2;
	if (Z == 0) // !=
		goto loc_81001554;
	FLAGS = fp - 0x100;
	if (Z == 0) // !=
		goto loc_81001546;
	*(uint32_t *)(v3 + 0x20) = sl;
	*(uint32_t *)(v3 + 0x24) = sl;

loc_81001546:
//VADDR: 0x81001546 OFF: 0x1546
	a1 = sub_810020D0(a1, *(uint32_t *)(v5), *(uint32_t *)(sp + 0xC));
	v1 = a1 + 0;
	FLAGS = v1;
	goto loc_810015EE;

loc_81001554:
//VADDR: 0x81001554 OFF: 0x1554
	FLAGS = a4 - 4;
	if (Z == 0) // !=
		goto loc_81001576;
	FLAGS = fp - 0x100;
	if (Z == 0) // !=
		goto loc_8100156C;
	FLAGS = (*(uint32_t *)(v3 + 0x20)) - 1;
	if (Z == 0) // !=
		goto loc_8100156C;
	*(uint32_t *)(v3 + 0x20) = sl;
	*(uint32_t *)(v3 + 0x24) = sl;

loc_8100156C:
//VADDR: 0x8100156c OFF: 0x156c
	v1 = /*data_80101202*/;
	goto loc_810015EE;

loc_81001576:
//VADDR: 0x81001576 OFF: 0x1576
	FLAGS = a4 - 0x40;
	if (Z == 0) // !=
		goto loc_81001586;
	*(uint32_t *)(v3 + 0x70) = sb;
	*(uint32_t *)(v3 + 0x20) = sb;
	*(uint32_t *)(v3 + 0x24) = v4;
	goto loc_810015CA;

loc_81001586:
//VADDR: 0x81001586 OFF: 0x1586
	FLAGS = a4 - 0x20;
	if (Z == 0) // !=
		goto loc_81001596;
	*(uint32_t *)(v3 + 0x20) = sl;
	*(uint32_t *)(v3 + 0x24) = sl;
	*(uint32_t *)(v3 + 0x70) = v4;
	goto loc_810015CA;

loc_81001596:
//VADDR: 0x81001596 OFF: 0x1596
	FLAGS = a4 - 0x80;
	if (Z == 0) // !=
		goto loc_810015AE;
	*(uint32_t *)(v3 + 0x20) = sl;
	v1 = /*data_80101203*/;
	*(uint32_t *)(v3 + 0x24) = sb;
	*(uint32_t *)(v3 + 0x70) = v4;
	goto loc_810015EE;

loc_810015AE:
//VADDR: 0x810015ae OFF: 0x15ae
	FLAGS = a4 - 0x100;
	if (Z == 0) // !=
		goto loc_810015C0;
	*(uint32_t *)(v3 + 0x70) = v4;
	v1 = /*data_80101203*/;
	goto loc_810015EE;

loc_810015C0:
//VADDR: 0x810015c0 OFF: 0x15c0
	v1 = /*data_801012FF*/;
	goto loc_810015EE;

loc_810015CA:
//VADDR: 0x810015ca OFF: 0x15ca
	a4 = *(uint32_t *)((*(uint32_t *)(a1)) + 0x2C);
	goto loc_81001518;

loc_810015D0:
//VADDR: 0x810015d0 OFF: 0x15d0
	FLAGS = v1 - /*data_80101205*/;
	if (Z == 0) // !=
		goto loc_810015EE;
	a1 = SceLibKernel_9F793F84(/*data_80101205*/);
	FLAGS = a1 - 0x1800000;
	if (C == 1) // unsigned >=
		goto loc_810015EE;
	v1 = /*data_80101200*/;

loc_810015EE:
//VADDR: 0x810015ee OFF: 0x15ee
	a1 = v1 + 0;
	FLAGS = a1;
	FLAGS = (*(uint32_t *)(v2)) - (*(uint32_t *)(sp + 0x10));
	if (Z == 0) // !=
		goto loc_810015FE;
	sp = sp + 0x14;
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}

loc_810015FE:
//VADDR: 0x810015fe OFF: 0x15fe
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81001604 OFF: 0x1604
int sub_81001604(int arg1, int arg2)
{
	// push {r4, lr}
	a1 = sub_8100212A(*(uint32_t *)(a2 + 4), (*(uint32_t *)(a2)) + 0, *(uint32_t *)(a2));
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001624;
	if ((*(uint32_t *)((a1 + 0) + 0x70)) == 0)
		goto loc_81001624;
	*(uint32_t *)((a1 + 0) + 0x20) = 2;
	*(uint32_t *)((a1 + 0) + 0x24) = 0;
	*(uint32_t *)((a1 + 0) + 0x70) = 0;

loc_81001624:
//VADDR: 0x81001624 OFF: 0x1624
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001626 OFF: 0x1626
int sub_81001626(int arg1, int arg2, int arg3, int arg4, int arg5)
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0xC;
	*(uint32_t *)(sp + 4) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp) = a2;
	v3 = *(uint32_t *)(sp + 0x20);
	v2 = a3 + 0;
	FLAGS = v2;
	v1 = a4 + 0;
	FLAGS = v1;
	a1 = SceLibKernel_46E7BE7B(a2 + 0x38, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001650;
	goto loc_810016CC;

loc_81001650:
//VADDR: 0x81001650 OFF: 0x1650
	FLAGS = (*(uint32_t *)((*(uint32_t *)(sp)) + 0x10)) - 1;
	if (Z == 0) // !=
		goto loc_81001672;
	a1 = (*(uint32_t *)(sp)) + 0;
	FLAGS = a1;
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001668;
	goto loc_810016CC;

loc_81001668:
//VADDR: 0x81001668 OFF: 0x1668
	a1 = /*data_80101246*/;
	goto loc_810016CC;

loc_81001672:
//VADDR: 0x81001672 OFF: 0x1672
	*(uint32_t *)(a2 + 0x18) = v3;
	*(uint32_t *)(a2 + 0x10) = 1;
	*(uint32_t *)((*(uint32_t *)(sp)) + 8) = v1;
	a1 = sub_8100217A(*(uint32_t *)((*(uint32_t *)(sp)) + 4), *(uint32_t *)((*(uint32_t *)(sp))), v2 + 0);
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 1) // ==
		goto loc_8100169C;
	*(uint32_t *)((*(uint32_t *)(sp)) + 8) = 0;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0x18) = 0;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0x10) = 0;
	goto loc_810016BA;

loc_8100169C:
//VADDR: 0x8100169c OFF: 0x169c
	if ((*(uint32_t *)(v3 + 0x20)) != 0)
		goto loc_810016BA;
	*(uint32_t *)(v3 + 0x20) = 1;
	a1 = SceLibKernel_F08DE149((*(uint32_t *)(v3 + 0x28)) + 0, 4, sp + 0, a4);
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 1) // ==
		goto loc_810016BA;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0x20) = 0;

loc_810016BA:
//VADDR: 0x810016ba OFF: 0x16ba
	a1 = SceLibKernel_91FA6614(v3 + 0x38, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810016CA;
	goto loc_810016CC;

loc_810016CA:
//VADDR: 0x810016ca OFF: 0x16ca
	a1 = v2 + 0;
	FLAGS = a1;

loc_810016CC:
//VADDR: 0x810016cc OFF: 0x16cc
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 4));
	if (Z == 0) // !=
		goto loc_810016D8;
	sp = sp + 0xC;
	return a1; // pop {r4, r5, r6, r7, pc}

loc_810016D8:
//VADDR: 0x810016d8 OFF: 0x16d8
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810016de OFF: 0x16de
int sub_810016DE(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	v2 = a1 + 0;
	FLAGS = v2;
	a1 = SceLibKernel_46E7BE7B(((a2 + 0) + 0x38) + 0, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001726;
	FLAGS = (*(uint32_t *)((a2 + 0) + 0x10)) - 1;
	if (Z == 1) // ==
		goto loc_81001712;
	a1 = SceLibKernel_91FA6614(((a2 + 0) + 0x38) + 0, 1);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001726;
	a1 = /*data_80101247*/;
	goto loc_8100174A;

loc_81001712:
//VADDR: 0x81001712 OFF: 0x1712
	*(uint32_t *)(v1 + 8) = 0;
	*(uint32_t *)(v1 + 0x18) = 0;
	*(uint32_t *)(v1 + 0x10) = 0;
	a1 = SceLibKernel_91FA6614(v3 + 0, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001728;

loc_81001726:
//VADDR: 0x81001726 OFF: 0x1726
	goto loc_8100174A;

loc_81001728:
//VADDR: 0x81001728 OFF: 0x1728
	FLAGS = (*(uint32_t *)(v1 + 0x20)) - 1;
	if (Z == 0) // !=
		goto loc_81001742;
	FLAGS = (*(uint32_t *)(v2 + 0x20)) - 1;
	if (Z == 1) // ==
		goto loc_81001742;
	*(uint32_t *)(v1 + 0x20) = 0;
	a1 = SceLibKernel_DDB395A9(*(uint32_t *)(v1 + 0x28), 0, 0);

loc_81001742:
//VADDR: 0x81001742 OFF: 0x1742
	a1 = sub_810021CC(*(uint32_t *)(v1 + 4), *(uint32_t *)(v1));

loc_8100174A:
//VADDR: 0x8100174a OFF: 0x174a
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x8100174c OFF: 0x174c
int sub_8100174C(int arg1, int arg2, int arg3)
{
	// push {r4, lr}
	a1 = sub_8100221C(*(uint32_t *)(a1 + 0x6C), *(uint32_t *)(a2));
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001758 OFF: 0x1758
int sub_81001758(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, r7, lr}
	asm("vpush {s16, s17}\n");
	sp = sp - 0xC;
	*(uint32_t *)(sp + 4) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v3 = a1 + 0;
	FLAGS = v3;
	asm("vmov.f32 s16, s0\n");
	*(uint32_t *)(sp) = a2;
	v2 = a3 + 0;
	FLAGS = v2;
	v1 = a4 + 0;
	FLAGS = v1;
	a1 = SceLibKernel_46E7BE7B(a2 + 0x38, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100178A;
	goto loc_81001808;

loc_8100178A:
//VADDR: 0x8100178a OFF: 0x178a
	FLAGS = (*(uint32_t *)((*(uint32_t *)(sp)) + 0x14)) - 1;
	if (Z == 0) // !=
		goto loc_810017AC;
	a1 = (*(uint32_t *)(sp)) + 0;
	FLAGS = a1;
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810017A2;
	goto loc_81001808;

loc_810017A2:
//VADDR: 0x810017a2 OFF: 0x17a2
	a1 = /*data_80101246*/;
	goto loc_81001808;

loc_810017AC:
//VADDR: 0x810017ac OFF: 0x17ac
	*(uint32_t *)(a2 + 0x1C) = v1;
	asm("vmov.f32 s0, s16\n");
	*(uint32_t *)((*(uint32_t *)(sp)) + 0xC) = v2;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0x14) = 1;
	a1 = sub_81002276(*(uint32_t *)(v3 + 0x6C), *(uint32_t *)((*(uint32_t *)(sp))), *(uint32_t *)(sp), 1);
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 1) // ==
		goto loc_810017D8;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0xC) = 0;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0x1C) = 0;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0x14) = 0;
	goto loc_810017F6;

loc_810017D8:
//VADDR: 0x810017d8 OFF: 0x17d8
	if ((*(uint32_t *)(a2 + 0x24)) != 0)
		goto loc_810017F6;
	*(uint32_t *)(a2 + 0x24) = 1;
	a1 = SceLibKernel_F08DE149((*(uint32_t *)(a2 + 0x2C)) + 0, 4, sp + 0, *(uint32_t *)(a2 + 0x2C));
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 1) // ==
		goto loc_810017F6;
	*(uint32_t *)((*(uint32_t *)(sp)) + 0x24) = 0;

loc_810017F6:
//VADDR: 0x810017f6 OFF: 0x17f6
	a1 = a2 + 0;
	FLAGS = a1;
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001806;
	goto loc_81001808;

loc_81001806:
//VADDR: 0x81001806 OFF: 0x1806
	a1 = v3 + 0;
	FLAGS = a1;

loc_81001808:
//VADDR: 0x81001808 OFF: 0x1808
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 4));
	if (Z == 0) // !=
		goto loc_81001818;
	sp = sp + 0xC;
	asm("vpop {s16, s17}\n");
	return a1; // pop {r4, r5, r6, r7, pc}

loc_81001818:
//VADDR: 0x81001818 OFF: 0x1818
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100181e OFF: 0x181e
int sub_8100181E(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	v2 = a1 + 0;
	FLAGS = v2;
	a1 = SceLibKernel_46E7BE7B(((a2 + 0) + 0x38) + 0, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001864;
	if ((*(uint32_t *)((a2 + 0) + 0x14)) != 0)
		goto loc_81001850;
	a1 = SceLibKernel_91FA6614(((a2 + 0) + 0x38) + 0, 1);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001864;
	a1 = /*data_80101247*/;
	goto loc_81001882;

loc_81001850:
//VADDR: 0x81001850 OFF: 0x1850
	*(uint32_t *)(v1 + 0xC) = 0;
	*(uint32_t *)(v1 + 0x1C) = 0;
	*(uint32_t *)(v1 + 0x14) = 0;
	a1 = SceLibKernel_91FA6614(v3 + 0, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001866;

loc_81001864:
//VADDR: 0x81001864 OFF: 0x1864
	goto loc_81001882;

loc_81001866:
//VADDR: 0x81001866 OFF: 0x1866
	FLAGS = (*(uint32_t *)(v1 + 0x24)) - 1;
	if (Z == 0) // !=
		goto loc_8100187A;
	*(uint32_t *)(v1 + 0x24) = 0;
	a1 = SceLibKernel_DDB395A9(*(uint32_t *)(v1 + 0x2C), 0, 0);

loc_8100187A:
//VADDR: 0x8100187a OFF: 0x187a
	a1 = sub_810022CA(*(uint32_t *)(v2 + 0x6C), *(uint32_t *)(v1));

loc_81001882:
//VADDR: 0x81001882 OFF: 0x1882
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81001884 OFF: 0x1884
int sub_81001884(int arg1, int arg2)
{
	a2 = /*data_8010128F*/;
	FLAGS = (uint8_t)a2 - 1;
	if (Z == 1) // ==
		goto loc_810018A0;
	a4 = *(uint32_t *)((/*data_81004030*/ 0x00000000));
	a4 = a4 | (uint8_t)a2;
	FLAGS = a4;
	if (Z == 0) // !=
		goto loc_810018CC;

loc_810018A0:
//VADDR: 0x810018a0 OFF: 0x18a0
	*(uint32_t *)((/*data_81004020*/ 0x00000000)) = (*(uint32_t *)(a1));
	*(uint32_t *)((/*data_81004020*/ 0x00000000) + 4) = (*(uint32_t *)(a1 + 4));
	FLAGS = a3 - 1;
	*(uint8_t *)((/*data_81004004*/ 0x00000001)) = a3;
	if (Z == 0) // !=
		goto loc_810018CA;
	*(uint32_t *)((/*data_81004030*/ 0x00000000)) = 2;

loc_810018CA:
//VADDR: 0x810018ca OFF: 0x18ca
	a2 = 0;
	FLAGS = a2;

loc_810018CC:
//VADDR: 0x810018cc OFF: 0x18cc
	a1 = a2 + 0;
	FLAGS = a1;
	return a1;
}

//VADDR: 0x810018d0 OFF: 0x18d0
int sub_810018D0()
{
	a1 = /*data_81004020*/ 0x00000000;
	return a1;
}

//VADDR: 0x810018da OFF: 0x18da
int sub_810018DA(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x818;
	*(uint32_t *)(sp + 0x810) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v2 = a1 + 0;
	FLAGS = v2;
	*(uint32_t *)(sp + 8) = a2;
	a1 = SceLibKernel_632980D7(sp + 0x10, 0, 0x800);
	a1 = *(uint32_t *)(sp + 8);
	a1 = SceLibKernel_46E7BE7B(a1 + 0x38, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001910;
	goto loc_8100199C;

loc_81001910:
//VADDR: 0x81001910 OFF: 0x1910
	FLAGS = (*(uint32_t *)(v2 + 0x20)) - 1;
	a1 = *(uint32_t *)(sp + 8);
	if (Z == 0) // !=
		goto loc_81001934;
	if ((*(uint32_t *)(v2 + 0x70)) != 0)
		goto loc_81001934;
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100192A;
	goto loc_8100199C;

loc_8100192A:
//VADDR: 0x8100192a OFF: 0x192a
	a1 = /*data_8010124A*/;
	goto loc_8100199C;

loc_81001934:
//VADDR: 0x81001934 OFF: 0x1934
	*(uint32_t *)(v2 + 0x20) = 1;
	FLAGS = (*(uint32_t *)(v2 + 0x68)) - 0;
	*(uint32_t *)(v2 + 0x24) = 0;
	if (Z == 1) // ==
		goto loc_81001956;
	*(uint32_t *)(sp) = (*(uint32_t *)(v2 + 0x2C));
	a1 = sub_8100231A(*(uint32_t *)(a1 + 4), *(uint32_t *)(a1), *(uint32_t *)(v2 + 0x28), (*(uint32_t *)(v2 + 0x30)) + 0, *(sp+0));
	goto loc_81001960;

loc_81001956:
//VADDR: 0x81001956 OFF: 0x1956
	*(uint32_t *)(sp) = (sp + 0x10);
	a1 = sub_8100231A(a1, a2, a3, 0, *(sp+0));

loc_81001960:
//VADDR: 0x81001960 OFF: 0x1960
	v1 = a1 + 0;
	FLAGS = v1;
	a2 = *(uint32_t *)(sp + 8);
	if (Z == 1) // ==
		goto loc_8100196C;
	*(uint32_t *)(v2 + 0x20) = 0;
	goto loc_8100198A;

loc_8100196C:
//VADDR: 0x8100196c OFF: 0x196c
	if ((*(uint32_t *)(a2 + 0x20)) != 0)
		goto loc_8100198A;
	*(uint32_t *)(a2 + 0x20) = 1;
	a1 = SceLibKernel_F08DE149((*(uint32_t *)(a2 + 0x28)) + 0, 4, sp + 8, *(uint32_t *)(a2 + 0x28));
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 1) // ==
		goto loc_8100198A;
	*(uint32_t *)((*(uint32_t *)(sp + 8)) + 0x20) = 0;

loc_8100198A:
//VADDR: 0x8100198a OFF: 0x198a
	a1 = a2 + 0;
	FLAGS = a1;
	a1 = SceLibKernel_91FA6614(a1 + 0x38, 1);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100199A;
	goto loc_8100199C;

loc_8100199A:
//VADDR: 0x8100199a OFF: 0x199a
	a1 = v1 + 0;
	FLAGS = a1;

loc_8100199C:
//VADDR: 0x8100199c OFF: 0x199c
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x810));
	if (Z == 0) // !=
		goto loc_810019AC;
	sp = sp + 0x818;
	return a1; // pop {r4, r5, r6, pc}

loc_810019AC:
//VADDR: 0x810019ac OFF: 0x19ac
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810019b2 OFF: 0x19b2
int sub_810019B2(int arg1, int arg2, int arg3)
{
	FLAGS = (*(uint32_t *)(a1 + 0x58)) - 1;
	if (Z == 0) // !=
		goto loc_810019C2;
	a1 = /*data_80101249*/;
	goto loc_810019C8;

loc_810019C2:
//VADDR: 0x810019c2 OFF: 0x19c2
	a1 = 0;
	FLAGS = a1;
	*(uint32_t *)(a3) = (*(uint32_t *)(a1 + 0x20));

loc_810019C8:
//VADDR: 0x810019c8 OFF: 0x19c8
	return a1;
}

//VADDR: 0x810019ca OFF: 0x19ca
int sub_810019CA(int arg1, int arg2, int arg3)
{
	// push {r4, lr}
	a4 = 0;
	FLAGS = a4;
	FLAGS = (*(uint32_t *)(a1 + 0x58)) - 1;
	if (Z == 0) // !=
		goto loc_810019DE;
	a1 = /*data_80101249*/;
	goto loc_810019F6;

loc_810019DE:
//VADDR: 0x810019de OFF: 0x19de
	a2 = a4 + 0;
	FLAGS = a2;
	FLAGS = (*(uint32_t *)(a1 + 0x20)) - 2;
	if (Z == 0) // !=
		goto loc_810019EA;
	a2 = *(uint32_t *)(a1 + 0x24);
	goto loc_810019F2;

loc_810019EA:
//VADDR: 0x810019ea OFF: 0x19ea
	a4 = /*data_80101248*/;

loc_810019F2:
//VADDR: 0x810019f2 OFF: 0x19f2
	*(uint32_t *)(a3) = a2;
	a1 = a4 + 0;
	FLAGS = a1;

loc_810019F6:
//VADDR: 0x810019f6 OFF: 0x19f6
	return a1; // pop {r4, pc}
}

//VADDR: 0x810019f8 OFF: 0x19f8
int sub_810019F8(int arg1, int arg2)
{
	// push {r4, lr}
	a1 = sub_810023DA(*(uint32_t *)(a2 + 4), (*(uint32_t *)(a2)) + 0, *(uint32_t *)(a2));
	*(uint32_t *)((a1 + 0) + 0x20) = 0;
	*(uint32_t *)((a1 + 0) + 0x24) = 0;
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001a0e OFF: 0x1a0e
int sub_81001A0E(int arg1, int arg2, int arg3)
{
	// push {r4, lr}
	a1 = sub_8100242A((*(uint32_t *)(a2 + 4)) + 0, *(uint32_t *)(a2), a3, *(uint32_t *)(a2 + 4));
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001a1c OFF: 0x1a1c
int sub_81001A1C(int arg1, int arg2)
{
	// push {r4, lr}
	a1 = sub_81002484((*(uint32_t *)(a2 + 4)) + 0, *(uint32_t *)(a2), *(uint32_t *)(a2 + 4));
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001a2a OFF: 0x1a2a
int sub_81001A2A(int arg1, int arg2)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0xC8;
	*(uint32_t *)(sp + 0xC0) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v4 = a2 + 0;
	FLAGS = v4;
	*(uint32_t *)(sp + 0x24) = 1;
	v3 = a1 + 0;
	FLAGS = v3;
	*(uint32_t *)(sp + 0x18) = 1;
	*(uint32_t *)(sp + 0x18 + 0x4) = 0xF00;
	*(uint32_t *)(sp + 0x20) = 0xF00;
	*(uint32_t *)(sp + 0x28) = 0xF00;
	*(uint32_t *)(sp + 0x28 + 0x4) = 0xF00;
	lr = 4;
	FLAGS = lr;
	*(uint32_t *)(sp + 0x30) = 1;
	*(uint32_t *)(sp + 0x30 + 0x4) = 0;
	ip = sp + 0x38;

loc_81001A64:
//VADDR: 0x81001a64 OFF: 0x1a64
	*(uint32_t *)(a4 + 0x38) = v1;
	lr = lr - 1;
	FLAGS = lr;
	*(uint32_t *)(ip + 4) = v1;
	*(uint32_t *)(ip + 8) = v1;
	*(uint32_t *)(ip + 0xC) = v1;
	*(uint32_t *)(ip + 0x10) = v1;
	*(uint32_t *)(ip + 0x14) = v1;
	*(uint32_t *)(ip + 0x18) = v1;
	*(uint32_t *)(ip + 0x1C) = v1;
	ip = ip + 0x20;
	if (Z == 0) // !=
		goto loc_81001A64;
	*(uint32_t *)(sp + 0xB8) = v1;
	*(uint32_t *)(sp + 0xB8 + 0x4) = -1;
	*(uint8_t *)(sp + 4) = v1;
	a1 = SceLibKernel_C458D60A((sp + 4) + 0, /*s_text_81003380*/ "SceLocationLoc", 0x10, a4 + 0x20);
	*(uint32_t *)(sp + 0xB8) = 2;
	a1 = SceIpmi_4E255C31((sp + 4) + 0);
	a1 = r1(...);
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_81001AD2;
	a1 = /*data_80101242*/;
	goto loc_81001B56;

loc_81001AD2:
//VADDR: 0x81001ad2 OFF: 0x1ad2
	a1 = SceIpmi_B282B430(v4 + 4, sp + 4, v3 + 0, v2 + 0);
	if (Z == 1) // ==
		goto loc_81001AF4;
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = (a1 + 0) + 0;
	FLAGS = a1;
	goto loc_81001B56;

loc_81001AF4:
//VADDR: 0x81001af4 OFF: 0x1af4
	a4 = sp + 0;
	a3 = 0;
	FLAGS = a3;
	a2 = 0;
	FLAGS = a2;
	a1 = r6(...);
	v4 = *(uint32_t *)(v4 + 4);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001B54;
	FLAGS = a1 - /*data_80020581*/;
	v1 = /*data_801012FF*/;
	if (Z == 0 && N == V) // signed >
		goto loc_81001B2A;
	FLAGS = a1 - /*data_80020580*/;
	if (Z == 1 || N != V) // signed <=
		goto loc_81001B3A;
	goto loc_81001B38;

loc_81001B2A:
//VADDR: 0x81001b2a OFF: 0x1b2a
	FLAGS = a1 - /*data_80020583*/;
	if (Z == 0) // !=
		goto loc_81001B3A;
	goto loc_81001B3A;

loc_81001B38:
//VADDR: 0x81001b38 OFF: 0x1b38
	v1 = a1 + 0;
	FLAGS = v1;

loc_81001B3A:
//VADDR: 0x81001b3a OFF: 0x1b3a
	a1 = v4 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = v1 + 0;
	FLAGS = a1;
	goto loc_81001B56;

loc_81001B54:
//VADDR: 0x81001b54 OFF: 0x1b54
	a1 = v1 + 0;
	FLAGS = a1;

loc_81001B56:
//VADDR: 0x81001b56 OFF: 0x1b56
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0xC0));
	if (Z == 0) // !=
		goto loc_81001B66;
	sp = sp + 0xC8;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_81001B66:
//VADDR: 0x81001b66 OFF: 0x1b66
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81001b6c OFF: 0x1b6c
int sub_81001B6C(int arg1)
{
	// push {r4, r5, r6, lr}
	v1 = a1 + 0;
	FLAGS = v1;
	a1 = 0x58;
	FLAGS = a1;
	a1 = r1(...);
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 0) // !=
		goto loc_81001B8C;
	a1 = /*data_80101242*/;
	goto loc_81001BB6;

loc_81001B8C:
//VADDR: 0x81001b8c OFF: 0x1b8c
	a1 = sub_81001A2A(*(uint32_t *)(/*data_81004000*/), v3 + 0);
	v2 = a1 + 0;
	FLAGS = v2;
	a2 = v3;
	if (Z == 1) // ==
		goto loc_81001BB2;
	a1 = v3 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a2 = 0;
	FLAGS = a2;

loc_81001BB2:
//VADDR: 0x81001bb2 OFF: 0x1bb2
	*(uint32_t *)(v1) = a2;
	a1 = v2 + 0;
	FLAGS = a1;

loc_81001BB6:
//VADDR: 0x81001bb6 OFF: 0x1bb6
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81001bb8 OFF: 0x1bb8
int sub_81001BB8()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001bba OFF: 0x1bba
int sub_81001BBA(int arg1)
{
	// push {r4, r5, r6, lr}
	v3 = /*data_801012FF*/;
	FLAGS = (*(uint32_t *)((a1 + 0))) - 0;
	if (Z == 1) // ==
		goto loc_81001C12;
	if ((*(uint32_t *)((*(uint32_t *)((a1 + 0))) + 4)) != 0)
		goto loc_81001BD2;
	goto loc_81001C00;

loc_81001BD2:
//VADDR: 0x81001bd2 OFF: 0x1bd2
	a1 = r1(...);
	FLAGS = a1 - /*data_8002050A*/;
	if (Z == 1) // ==
		goto loc_81001BFE;
	a1 = r1(...);
	a1 = *(uint32_t *)(v2 + 4);
	a1 = r1(...);
	*(uint32_t *)(v2 + 4) = 0;

loc_81001BFE:
//VADDR: 0x81001bfe OFF: 0x1bfe
	v2 = *(uint32_t *)(v1);

loc_81001C00:
//VADDR: 0x81001c00 OFF: 0x1c00
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	*(uint32_t *)(v1) = 0;

loc_81001C12:
//VADDR: 0x81001c12 OFF: 0x1c12
	a1 = v3 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81001c16 OFF: 0x1c16
int sub_81001C16(int arg1, int arg2, int arg3)
{
	// push {r4, lr}
	a1 = sub_810024D4(*(uint32_t *)(a2 + 4), a3 + 0);
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001c22 OFF: 0x1c22
int sub_81001C22(int arg1, int arg2, int arg3)
{
	// push {r4, lr}
	a1 = sub_8100257E(*(uint32_t *)(a2 + 4), a3 + 0);
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001c2e OFF: 0x1c2e
int sub_81001C2E()
{
	if ((*(uint32_t *)((/*data_81004030*/ 0x00000000))) == 0)
		goto loc_81001C44;
	a1 = /*data_8010124E*/;
	goto loc_81001C5A;

loc_81001C44:
//VADDR: 0x81001c44 OFF: 0x1c44
	*(uint32_t *)((/*data_81004008*/ 0x10000100)) = a1;
	*(uint32_t *)((/*data_8100402C*/ 0x00000000)) = a2;
	a1 = 0;
	FLAGS = a1;

loc_81001C5A:
//VADDR: 0x81001c5a OFF: 0x1c5a
	return a1;
}

//VADDR: 0x81001c5c OFF: 0x1c5c
int sub_81001C5C()
{
	*(uint32_t *)(a1) = (*(uint32_t *)((/*data_81004008*/ 0x10000100)));
	*(uint32_t *)(a2) = (*(uint32_t *)((/*data_8100402C*/ 0x00000000)));
	return a1;
}

//VADDR: 0x81001c76 OFF: 0x1c76
int sub_81001C76()
{
	a1 = *(uint32_t *)(a1 + 0x64);
	return a1;
}

//VADDR: 0x81001c7a OFF: 0x1c7a
int sub_81001C7A(int arg1, int arg2)
{
	*(uint32_t *)(a1 + 0x70) = a2;
	return a1;
}

//VADDR: 0x81001c7e OFF: 0x1c7e
int sub_81001C7E()
{
	a1 = 0;
	FLAGS = a1;
	FLAGS = (*(uint32_t *)((/*data_81004030*/ 0x00000000))) - 2;
	if (Z == 0) // !=
		goto loc_81001C90;
	a1 = 1;
	FLAGS = a1;

loc_81001C90:
//VADDR: 0x81001c90 OFF: 0x1c90
	return a1;
}

//VADDR: 0x81001c92 OFF: 0x1c92
int sub_81001C92()
{
	a1 = 0;
	FLAGS = a1;
	if ((*(uint32_t *)((/*data_81004030*/ 0x00000000))) == 0)
		goto loc_81001CA2;
	a1 = 1;
	FLAGS = a1;

loc_81001CA2:
//VADDR: 0x81001ca2 OFF: 0x1ca2
	return a1;
}

//VADDR: 0x81001ca4 OFF: 0x1ca4
int sub_81001CA4()
{
	// push {r4, r5, r6, r7, r8, sb, lr}
	sp = sp - 0x4C;
	*(uint32_t *)(sp + 0x48) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 8) = 0;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)) + 0x20)) - 0;
	if (Z == 1) // ==
		goto loc_81001E48;
	v5 = (*(uint32_t *)(a2)) + 0x38;
	v3 = 0 + 0;
	FLAGS = v3;

loc_81001CCC:
//VADDR: 0x81001ccc OFF: 0x1ccc
	*(uint32_t *)(sp + 0xC) = 0x2710;
	a2 = 0;
	FLAGS = a2;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0xC);
	a3 = 0x1C3;
	a4 = 5;
	FLAGS = a4;
	a1 = r7(...);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001E14;
	FLAGS = (*(uint32_t *)(sp + 0x10)) - 0x100;
	if (Z == 1) // ==
		goto loc_81001DAC;
	FLAGS = (*(uint32_t *)(sp + 0x10)) - 0x80;
	if (Z == 1) // ==
		goto loc_81001D92;
	FLAGS = (*(uint32_t *)(sp + 0x10)) - 0x40;
	if (Z == 1) // ==
		goto loc_81001D7A;
	FLAGS = (*(uint32_t *)(sp + 0x10)) - 3;
	if (Z == 1) // ==
		goto loc_81001DBA;
	FLAGS = (*(uint32_t *)(sp + 0x10)) - 1;
	if (Z == 0) // !=
		goto loc_81001E14;
	a1 = SceLibKernel_632980D7((sp + 0x18) + 0, 0, 0x30);
	a1 = sub_810020D0(*(uint32_t *)(v1 + 4), *(uint32_t *)(v1), (sp + 0x18) + 0);
	a1 = SceLibKernel_46E7BE7B(v5, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001E14;
	if ((*(uint32_t *)(v1 + 8)) == 0)
		goto loc_81001D70;
	FLAGS = (a1 + 0) - /*data_80101205*/;
	if (Z == 0) // !=
		goto loc_81001D64;
	a1 = SceLibKernel_9F793F84(/*data_80101205*/);
	FLAGS = a1 - 0x1800000;
	if (C == 0) // unsigned <
		goto loc_81001D5C;
	a1 = (*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x64)) & 0xFF;
	FLAGS = a1;
	if (Z == 0) // !=
		goto loc_81001D64;

loc_81001D5C:
//VADDR: 0x81001d5c OFF: 0x1d5c
	v4 = /*data_80101200*/;

loc_81001D64:
//VADDR: 0x81001d64 OFF: 0x1d64
	a2 = *(uint32_t *)(v1);
	a1 = v4 + 0;
	FLAGS = a1;
	a4 = *(uint32_t *)(v1 + 0x18);
	a3 = sp + 0x18;
	a1 = r7(...);

loc_81001D70:
//VADDR: 0x81001d70 OFF: 0x1d70
	a1 = SceLibKernel_91FA6614(v5, 1);
	goto loc_81001E14;

loc_81001D7A:
//VADDR: 0x81001d7a OFF: 0x1d7a
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x70) = 1;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x20) = 1;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x24) = v2;
	goto loc_81001E14;

loc_81001D92:
//VADDR: 0x81001d92 OFF: 0x1d92
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x70) = v2;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x20) = 2;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x24) = 1;
	goto loc_81001E14;

loc_81001DAC:
//VADDR: 0x81001dac OFF: 0x1dac
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x70) = v2;
	goto loc_81001E14;

loc_81001DBA:
//VADDR: 0x81001dba OFF: 0x1dba
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x58)) - 1;
	if (Z == 1) // ==
		goto loc_81001DCC;
	v3 = *(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x20);

loc_81001DCC:
//VADDR: 0x81001dcc OFF: 0x1dcc
	FLAGS = v3 - 1;
	if (Z == 0) // !=
		goto loc_81001E14;
	a1 = sub_81002382(*(uint32_t *)(v1 + 4), *(uint32_t *)(v1), sp + 8);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001E14;
	a1 = SceLibKernel_46E7BE7B(v5, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001E14;
	FLAGS = (*(uint32_t *)(sp + 8)) - 0;
	*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x24) = (*(uint32_t *)(sp + 8));
	lr = 2;
	a2 = *(uint32_t *)(/*data_81004000*/);
	if (Z == 0) // !=
		goto loc_81001E06;
	lr = v2;

loc_81001E06:
//VADDR: 0x81001e06 OFF: 0x1e06
	*(uint32_t *)((a2 + 0) + 0x20) = lr;
	a1 = SceLibKernel_91FA6614(v5, 1);

loc_81001E14:
//VADDR: 0x81001e14 OFF: 0x1e14
	a1 = SceLibKernel_46E7BE7B(v5, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001E40;
	if ((*(uint32_t *)(v1 + 0x10)) != 0)
		goto loc_81001E38;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x20)) - 1;
	if (Z == 1) // ==
		goto loc_81001E38;
	*(uint32_t *)(v1 + 0x20) = v2;

loc_81001E38:
//VADDR: 0x81001e38 OFF: 0x1e38
	a1 = SceLibKernel_91FA6614(v5, 1);

loc_81001E40:
//VADDR: 0x81001e40 OFF: 0x1e40
	FLAGS = (*(uint32_t *)(v1 + 0x20)) - 0;
	if (Z == 0) // !=
		goto loc_81001CCC;

loc_81001E48:
//VADDR: 0x81001e48 OFF: 0x1e48
	a1 = SceThreadmgrCoredumpTime_0C8A38E1(0);
	a1 = v2 + 0;
	FLAGS = a1;
	FLAGS = (*(uint32_t *)(sb)) - (*(uint32_t *)(sp + 0x48));
	if (Z == 0) // !=
		goto loc_81001E60;
	sp = sp + 0x4C;
	return a1; // pop {r4, r5, r6, r7, r8, sb, pc}

loc_81001E60:
//VADDR: 0x81001e60 OFF: 0x1e60
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81001e66 OFF: 0x1e66
int sub_81001E66()
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x38;
	*(uint32_t *)(sp + 0x30) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	v1 = 0;
	FLAGS = v1;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)) + 0x24)) - 0;
	if (Z == 1) // ==
		goto loc_81001F1C;
	v2 = (*(uint32_t *)(a2)) + 0x38;
	FLAGS = v2;

loc_81001E88:
//VADDR: 0x81001e88 OFF: 0x1e88
	*(uint32_t *)(sp + 0xC) = 0x2710;
	a3 = ((*(uint32_t *)(v3)) | 0x10000000) + 0;
	FLAGS = a3;
	*(uint32_t *)(sp) = (sp + 8);
	*(uint32_t *)(sp + 0x4) = (sp + 0xC);
	a2 = 0;
	FLAGS = a2;
	a4 = 4;
	FLAGS = a4;
	a1 = lr(...);
	FLAGS = (*(uint32_t *)(sp + 8)) - (((*(uint32_t *)(v3)) | 0x10000000));
	if (Z == 0) // !=
		goto loc_81001EFA;
	a3 = sp + 0x10;
	a1 = sub_8100221C(*(uint32_t *)((*(uint32_t *)(/*data_81004000*/)) + 0x6C), *(uint32_t *)(v3));
	*(uint32_t *)(sp + 8) = a1;
	a1 = SceLibKernel_46E7BE7B(v2 + 0, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001EFA;
	if ((*(uint32_t *)(v3 + 0xC)) == 0)
		goto loc_81001EF2;
	a1 = *(uint32_t *)(sp + 8);
	a3 = sp + 0x10;
	a2 = *(uint32_t *)(v3);
	a4 = *(uint32_t *)(v3 + 0x1C);
	a1 = lr(...);

loc_81001EF2:
//VADDR: 0x81001ef2 OFF: 0x1ef2
	a1 = SceLibKernel_91FA6614(v2 + 0, 1);

loc_81001EFA:
//VADDR: 0x81001efa OFF: 0x1efa
	a1 = SceLibKernel_46E7BE7B(v2 + 0, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81001F16;
	if ((*(uint32_t *)(v3 + 0x14)) != 0)
		goto loc_81001F0E;
	*(uint32_t *)(v3 + 0x24) = v1;

loc_81001F0E:
//VADDR: 0x81001f0e OFF: 0x1f0e
	a1 = SceLibKernel_91FA6614(v2 + 0, 1);

loc_81001F16:
//VADDR: 0x81001f16 OFF: 0x1f16
	FLAGS = (*(uint32_t *)(v3 + 0x24)) - 0;
	if (Z == 0) // !=
		goto loc_81001E88;

loc_81001F1C:
//VADDR: 0x81001f1c OFF: 0x1f1c
	a1 = SceThreadmgrCoredumpTime_0C8A38E1(0);
	a1 = v1 + 0;
	FLAGS = a1;
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x30));
	if (Z == 0) // !=
		goto loc_81001F34;
	sp = sp + 0x38;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_81001F34:
//VADDR: 0x81001f34 OFF: 0x1f34
	a1 = SceLibKernel_37691BF8();
	*(uint32_t *)((/*data_81004020*/ 0x00000000)) = (*(uint32_t *)((/*text_81003318*/ 0x14F244B5)));
	*(uint32_t *)((/*data_81004020*/ 0x00000000) + 4) = (*(uint32_t *)((/*text_81003318*/ 0x14F244B5) + 4));
	return a1;
}

//VADDR: 0x81001f54 OFF: 0x1f54
int sub_81001F54(int arg1, int arg2, int arg3, int arg4, int arg5)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x48;
	*(uint32_t *)(sp + 0x40) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x28) = a2;
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a3;
	*(uint32_t *)(sp + 0x1C) = (*(uint32_t *)(sp + 0x60));
	*(uint32_t *)(sp + 0x1C + 0x4) = (sp + 0x14);
	*(uint32_t *)(sp + 0x24) = 0xC;
	*(uint32_t *)(sp + 0x2C) = 4;
	*(uint32_t *)(sp + 0x18) = a4;
	*(uint32_t *)(sp + 0x34) = (*(uint32_t *)((sp + 0x60) + 4));
	*(uint32_t *)(sp + 0x34 + 0x4) = 4;
	a2 = 0 + 0;
	FLAGS = a2;
	a3 = sp + 0x20;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x28);
	a4 = 1;
	FLAGS = a4;
	*(uint32_t *)(sp + 8) = 2;
	a2 = 0x12340000 | a2;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001FB0;
	goto loc_81001FB2;

loc_81001FB0:
//VADDR: 0x81001fb0 OFF: 0x1fb0
	a1 = *(uint32_t *)(sp + 0x10);

loc_81001FB2:
//VADDR: 0x81001fb2 OFF: 0x1fb2
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x40));
	if (Z == 0) // !=
		goto loc_81001FC2;
	sp = sp + 0x48;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_81001FC2:
//VADDR: 0x81001fc2 OFF: 0x1fc2
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81001fc8 OFF: 0x1fc8
int sub_81001FC8(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x20) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_12340001*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002004;
	goto loc_81002006;

loc_81002004:
//VADDR: 0x81002004 OFF: 0x2004
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002006:
//VADDR: 0x81002006 OFF: 0x2006
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x20));
	if (Z == 0) // !=
		goto loc_81002012;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_81002012:
//VADDR: 0x81002012 OFF: 0x2012
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81002018 OFF: 0x2018
int sub_81002018(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0x2C;
	*(uint32_t *)(sp + 0x28) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x1C) = a4;
	*(uint32_t *)(sp + 0x1C + 0x4) = (sp + 0x14);
	*(uint32_t *)(sp + 0x24) = 0xC;
	*(uint32_t *)(sp + 0x18) = a3;
	a3 = sp + 0x20;
	a2 = /*data_12340002*/;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a4 = 1;
	FLAGS = a4;
	*(uint32_t *)(sp + 8) = 0;
	a1 = r6(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002058;
	goto loc_8100205A;

loc_81002058:
//VADDR: 0x81002058 OFF: 0x2058
	a1 = *(uint32_t *)(sp + 0x10);

loc_8100205A:
//VADDR: 0x8100205a OFF: 0x205a
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 0x28));
	if (Z == 0) // !=
		goto loc_81002066;
	sp = sp + 0x2C;
	return a1; // pop {r4, r5, r6, r7, pc}

loc_81002066:
//VADDR: 0x81002066 OFF: 0x2066
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100206c OFF: 0x206c
int sub_8100206C(int arg1, int arg2)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x40;
	*(uint32_t *)(sp + 0x38) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	*(uint32_t *)(sp + 0x1C + 0x4) = a3;
	*(uint32_t *)(sp + 0x24) = 4;
	*(uint32_t *)(sp + 0x2C) = a4;
	*(uint32_t *)(sp + 0x2C + 0x4) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x20);
	a2 = /*data_12340003*/;
	*(uint32_t *)(sp + 8) = 2;
	a4 = 1;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810020B8;
	goto loc_810020BA;

loc_810020B8:
//VADDR: 0x810020b8 OFF: 0x20b8
	a1 = *(uint32_t *)(sp + 0x10);

loc_810020BA:
//VADDR: 0x810020ba OFF: 0x20ba
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x38));
	if (Z == 0) // !=
		goto loc_810020CA;
	sp = sp + 0x40;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_810020CA:
//VADDR: 0x810020ca OFF: 0x20ca
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810020d0 OFF: 0x20d0
int sub_810020D0(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x30;
	*(uint32_t *)(sp + 0x2C) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	*(uint32_t *)(sp + 0x1C + 0x4) = a3;
	*(uint32_t *)(sp + 0x24) = 0x30;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x20);
	*(uint32_t *)(sp + 8) = 1;
	a3 = sp + 0x18;
	a2 = /*data_12340005*/;
	a4 = 1;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002116;
	goto loc_81002118;

loc_81002116:
//VADDR: 0x81002116 OFF: 0x2116
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002118:
//VADDR: 0x81002118 OFF: 0x2118
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x2C));
	if (Z == 0) // !=
		goto loc_81002124;
	sp = sp + 0x30;
	return a1; // pop {r4, r5, r6, pc}

loc_81002124:
//VADDR: 0x81002124 OFF: 0x2124
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100212a OFF: 0x212a
int sub_8100212A(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x20) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_12340006*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002166;
	goto loc_81002168;

loc_81002166:
//VADDR: 0x81002166 OFF: 0x2166
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002168:
//VADDR: 0x81002168 OFF: 0x2168
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x20));
	if (Z == 0) // !=
		goto loc_81002174;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_81002174:
//VADDR: 0x81002174 OFF: 0x2174
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100217a OFF: 0x217a
int sub_8100217A(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x24) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = a3;
	*(uint32_t *)(sp + 0x18 + 0x4) = (sp + 0x14);
	*(uint32_t *)(sp + 0x20) = 8;
	a3 = sp + 0x1C;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_12340007*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810021B8;
	goto loc_810021BA;

loc_810021B8:
//VADDR: 0x810021b8 OFF: 0x21b8
	a1 = *(uint32_t *)(sp + 0x10);

loc_810021BA:
//VADDR: 0x810021ba OFF: 0x21ba
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x24));
	if (Z == 0) // !=
		goto loc_810021C6;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_810021C6:
//VADDR: 0x810021c6 OFF: 0x21c6
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810021cc OFF: 0x21cc
int sub_810021CC(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x20) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_12340008*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002208;
	goto loc_8100220A;

loc_81002208:
//VADDR: 0x81002208 OFF: 0x2208
	a1 = *(uint32_t *)(sp + 0x10);

loc_8100220A:
//VADDR: 0x8100220a OFF: 0x220a
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x20));
	if (Z == 0) // !=
		goto loc_81002216;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_81002216:
//VADDR: 0x81002216 OFF: 0x2216
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100221c OFF: 0x221c
int sub_8100221C(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x30;
	*(uint32_t *)(sp + 0x2C) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	*(uint32_t *)(sp + 0x1C + 0x4) = a3;
	*(uint32_t *)(sp + 0x24) = 0x20;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x20);
	*(uint32_t *)(sp + 8) = 1;
	a3 = sp + 0x18;
	a2 = /*data_12340009*/;
	a4 = 1;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002262;
	goto loc_81002264;

loc_81002262:
//VADDR: 0x81002262 OFF: 0x2262
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002264:
//VADDR: 0x81002264 OFF: 0x2264
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x2C));
	if (Z == 0) // !=
		goto loc_81002270;
	sp = sp + 0x30;
	return a1; // pop {r4, r5, r6, pc}

loc_81002270:
//VADDR: 0x81002270 OFF: 0x2270
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81002276 OFF: 0x2276
int sub_81002276(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x24) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	asm("vstr s0, [sp, #0x18]\n");
	*(uint32_t *)(sp + 0x1C) = (sp + 0x14);
	*(uint32_t *)(sp + 0x20) = 8;
	*(uint32_t *)(sp + 0x14) = a2;
	a3 = sp + 0x1C;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_1234000A*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810022B6;
	goto loc_810022B8;

loc_810022B6:
//VADDR: 0x810022b6 OFF: 0x22b6
	a1 = *(uint32_t *)(sp + 0x10);

loc_810022B8:
//VADDR: 0x810022b8 OFF: 0x22b8
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x24));
	if (Z == 0) // !=
		goto loc_810022C4;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_810022C4:
//VADDR: 0x810022c4 OFF: 0x22c4
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810022ca OFF: 0x22ca
int sub_810022CA(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x20) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_1234000B*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002306;
	goto loc_81002308;

loc_81002306:
//VADDR: 0x81002306 OFF: 0x2306
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002308:
//VADDR: 0x81002308 OFF: 0x2308
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x20));
	if (Z == 0) // !=
		goto loc_81002314;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_81002314:
//VADDR: 0x81002314 OFF: 0x2314
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100231a OFF: 0x231a
int sub_8100231A(int arg1, int arg2, int arg3, int arg4, int arg5)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x38;
	*(uint32_t *)(sp + 0x30) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x1C) = a4;
	*(uint32_t *)(sp + 0x1C + 0x4) = (sp + 0x14);
	*(uint32_t *)(sp + 0x24) = 0xC;
	*(uint32_t *)(sp + 0x18) = a3;
	*(uint32_t *)(sp + 0x28) = (*(uint32_t *)(sp + 0x50));
	*(uint32_t *)(sp + 0x28 + 0x4) = 0x800;
	a2 = 0xC + 0;
	FLAGS = a2;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a3 = sp + 0x20;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 2;
	FLAGS = a4;
	a2 = 0x12340000 | a2;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100236A;
	goto loc_8100236C;

loc_8100236A:
//VADDR: 0x8100236a OFF: 0x236a
	a1 = *(uint32_t *)(sp + 0x10);

loc_8100236C:
//VADDR: 0x8100236c OFF: 0x236c
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x30));
	if (Z == 0) // !=
		goto loc_8100237C;
	sp = sp + 0x38;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_8100237C:
//VADDR: 0x8100237c OFF: 0x237c
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81002382 OFF: 0x2382
int sub_81002382(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0x34;
	*(uint32_t *)(sp + 0x2C) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	*(uint32_t *)(sp + 0x1C + 0x4) = a3;
	*(uint32_t *)(sp + 0x24) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x20);
	*(uint32_t *)(sp + 8) = 1;
	a2 = /*data_1234000E*/;
	a4 = 1;
	FLAGS = a4;
	a1 = r6(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810023C6;
	goto loc_810023C8;

loc_810023C6:
//VADDR: 0x810023c6 OFF: 0x23c6
	a1 = *(uint32_t *)(sp + 0x10);

loc_810023C8:
//VADDR: 0x810023c8 OFF: 0x23c8
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 0x2C));
	if (Z == 0) // !=
		goto loc_810023D4;
	sp = sp + 0x34;
	return a1; // pop {r4, r5, r6, r7, pc}

loc_810023D4:
//VADDR: 0x810023d4 OFF: 0x23d4
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810023da OFF: 0x23da
int sub_810023DA(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x20) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_1234000F*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002416;
	goto loc_81002418;

loc_81002416:
//VADDR: 0x81002416 OFF: 0x2416
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002418:
//VADDR: 0x81002418 OFF: 0x2418
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x20));
	if (Z == 0) // !=
		goto loc_81002424;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_81002424:
//VADDR: 0x81002424 OFF: 0x2424
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100242a OFF: 0x242a
int sub_8100242A(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x30;
	*(uint32_t *)(sp + 0x2C) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	*(uint32_t *)(sp + 0x1C + 0x4) = a3;
	*(uint32_t *)(sp + 0x24) = 0x14;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x20);
	*(uint32_t *)(sp + 8) = 1;
	a3 = sp + 0x18;
	a2 = /*data_12340010*/;
	a4 = 1;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81002470;
	goto loc_81002472;

loc_81002470:
//VADDR: 0x81002470 OFF: 0x2470
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002472:
//VADDR: 0x81002472 OFF: 0x2472
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x2C));
	if (Z == 0) // !=
		goto loc_8100247E;
	sp = sp + 0x30;
	return a1; // pop {r4, r5, r6, pc}

loc_8100247E:
//VADDR: 0x8100247e OFF: 0x247e
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81002484 OFF: 0x2484
int sub_81002484(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x28;
	*(uint32_t *)(sp + 0x20) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = (sp + 0x14);
	*(uint32_t *)(sp + 0x1C) = 4;
	a3 = sp + 0x18;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = /*data_12340011*/;
	*(uint32_t *)(sp + 8) = 0;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810024C0;
	goto loc_810024C2;

loc_810024C0:
//VADDR: 0x810024c0 OFF: 0x24c0
	a1 = *(uint32_t *)(sp + 0x10);

loc_810024C2:
//VADDR: 0x810024c2 OFF: 0x24c2
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x20));
	if (Z == 0) // !=
		goto loc_810024CE;
	sp = sp + 0x28;
	return a1; // pop {r4, r5, r6, pc}

loc_810024CE:
//VADDR: 0x810024ce OFF: 0x24ce
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x810024d4 OFF: 0x24d4
int sub_810024D4(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x20;
	*(uint32_t *)(sp + 0x1C) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = 0x100;
	a3 = sp + 0x14;
	a4 = 1;
	FLAGS = a4;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	*(uint32_t *)(sp + 8) = 0;
	a2 = /*data_12340012*/;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100250E;
	goto loc_81002510;

loc_8100250E:
//VADDR: 0x8100250e OFF: 0x250e
	a1 = *(uint32_t *)(sp + 0x10);

loc_81002510:
//VADDR: 0x81002510 OFF: 0x2510
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x1C));
	if (Z == 0) // !=
		goto loc_8100251C;
	sp = sp + 0x20;
	return a1; // pop {r4, r5, r6, pc}

loc_8100251C:
//VADDR: 0x8100251c OFF: 0x251c
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81002522 OFF: 0x2522
int sub_81002522(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x38;
	*(uint32_t *)(sp + 0x30) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = a3;
	*(uint32_t *)(sp + 0x18 + 0x4) = (sp + 0x14);
	*(uint32_t *)(sp + 0x20) = 8;
	*(uint32_t *)(sp + 0x20 + 0x4) = a4;
	*(uint32_t *)(sp + 0x28) = 0x30;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x24);
	*(uint32_t *)(sp + 8) = 1;
	a3 = sp + 0x1C;
	a2 = /*data_12340014*/;
	a4 = 1;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100256A;
	goto loc_8100256C;

loc_8100256A:
//VADDR: 0x8100256a OFF: 0x256a
	a1 = *(uint32_t *)(sp + 0x10);

loc_8100256C:
//VADDR: 0x8100256c OFF: 0x256c
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x30));
	if (Z == 0) // !=
		goto loc_81002578;
	sp = sp + 0x38;
	return a1; // pop {r4, r5, r6, pc}

loc_81002578:
//VADDR: 0x81002578 OFF: 0x2578
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x8100257e OFF: 0x257e
int sub_8100257E(int arg1, int arg2)
{
	// push {r4, r5, lr}
	sp = sp - 0x24;
	*(uint32_t *)(sp + 0x20) = (*(uint32_t *)((/*s_text_81002C9C*/ "@#")));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x18) = 4;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x14);
	*(uint32_t *)(sp + 8) = 1;
	a2 = /*data_12340017*/;
	a3 = 0;
	FLAGS = a3;
	a4 = 0;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810025BA;
	goto loc_810025BC;

loc_810025BA:
//VADDR: 0x810025ba OFF: 0x25ba
	a1 = *(uint32_t *)(sp + 0x10);

loc_810025BC:
//VADDR: 0x810025bc OFF: 0x25bc
	FLAGS = (*(uint32_t *)(v2)) - (*(uint32_t *)(sp + 0x20));
	if (Z == 0) // !=
		goto loc_810025C8;
	sp = sp + 0x24;
	return a1; // pop {r4, r5, pc}

loc_810025C8:
//VADDR: 0x810025c8 OFF: 0x25c8
	a1 = SceLibKernel_37691BF8();
	a1 = a1;
	FLAGS = a1;
}

//VADDR: 0x810025d0 OFF: 0x25d0
int sub_810025D0(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, lr}
	v2 = /*data_80101242*/;
	a1 = SceSysmem_B9D5EBDE(/*s_text_810033BC*/ "SceLocationClient", /*data_0C20D060*/, (a1 + 0) + 0, 0);
	*(uint32_t *)((/*data_81004010*/ 0x00000000)) = a1;
	FLAGS = a1 - 0;
	if (N != V) // signed <
		goto loc_81002658;
	a1 = SceSysmem_B8EF5818(a1, /*data_81004014*/ 0x00000000);
	FLAGS = (a1 + 0) - 0;
	if (N != V) // signed <
		goto loc_8100264A;
	a1 = SceLibKernel_3B9E301A(*(uint32_t *)((/*data_81004014*/ 0x00000000)), (a1 + 0) + 0);
	*(uint32_t *)((/*data_81004014*/ 0x00000000)) = a1;
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_8100262E;
	v2 = 0;
	FLAGS = v2;
	goto loc_81002658;

loc_8100262E:
//VADDR: 0x8100262e OFF: 0x262e
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81004010*/ 0x00000000)));
	*(uint32_t *)((/*data_81004010*/ 0x00000000)) = 0;
	v2 = /*data_80101242*/;
	goto loc_81002658;

loc_8100264A:
//VADDR: 0x8100264a OFF: 0x264a
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81004010*/ 0x00000000)));

loc_81002658:
//VADDR: 0x81002658 OFF: 0x2658
	a1 = v2 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x8100265c OFF: 0x265c
int sub_8100265C(int arg1, int arg2, int arg3)
{
	// push {r4, lr}
	if ((*(uint32_t *)((/*data_81004014*/ 0x00000000))) == 0)
		goto loc_8100267A;
	a1 = SceLibKernel_AE1A21EC(*(uint32_t *)((/*data_81004014*/ 0x00000000)));
	*(uint32_t *)((/*data_81004014*/ 0x00000000)) = 0;

loc_8100267A:
//VADDR: 0x8100267a OFF: 0x267a
	FLAGS = (*(uint32_t *)((/*data_81004010*/ 0x00000000))) - 0;
	if (N != V) // signed <
		goto loc_81002698;
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81004010*/ 0x00000000)));
	*(uint32_t *)((/*data_81004010*/ 0x00000000)) = 0;

loc_81002698:
//VADDR: 0x81002698 OFF: 0x2698
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100269a OFF: 0x269a
int sub_8100269A()
{
	// push {r4, lr}
	a1 = SceLibKernel_86EF7680((*(uint32_t *)((/*data_81004014*/ 0x00000000))) + 0, a1 + 0, *(uint32_t *)((/*data_81004014*/ 0x00000000)));
	return a1; // pop {r4, pc}
}

//VADDR: 0x810026b0 OFF: 0x26b0
int sub_810026B0()
{
	// push {r4, lr}
	a1 = SceLibKernel_9C56B4D1((*(uint32_t *)((/*data_81004014*/ 0x00000000))) + 0, a1 + 0, *(uint32_t *)((/*data_81004014*/ 0x00000000)));
	return a1; // pop {r4, pc}
}

//VADDR: 0x810026c8 OFF: 0x26c8
int sub_810026C8()
{
	// push {r4, lr}
	if (/*data_00000000*/ == 0)
		goto loc_810026E2;
	a4 = 0;
	FLAGS = a4;
	a1 = ip(...);
	goto loc_810026E4;

loc_810026E2:
//VADDR: 0x810026e2 OFF: 0x26e2
	a1 = 9;
	FLAGS = a1;

loc_810026E4:
//VADDR: 0x810026e4 OFF: 0x26e4
	return a1; // pop {r4, pc}
}

//VADDR: 0x810026e6 OFF: 0x26e6
int sub_810026E6()
{
	// push {r4, lr}
	if (/*data_00000000*/ == 0)
		goto loc_81002700;
	a4 = 1;
	FLAGS = a4;
	a1 = ip(...);
	goto loc_81002702;

loc_81002700:
//VADDR: 0x81002700 OFF: 0x2700
	a1 = 9;
	FLAGS = a1;

loc_81002702:
//VADDR: 0x81002702 OFF: 0x2702
	return a1; // pop {r4, pc}
}

//VADDR: 0x81002704 OFF: 0x2704
int sub_81002704()
{
	return a1; // pop {r4, pc}
}

int SceLibKernel_244E76D2(int arg1);
int SceLibKernel_2E3B02A1(int arg1, int arg2, int arg3);
int SceLibKernel_37691BF8();
int SceLibKernel_46E7BE7B(int arg1, int arg2, int arg3);
int SceLibKernel_632980D7(int arg1, int arg2, int arg3);
int SceLibKernel_91FA6614(int arg1, int arg2);
int SceLibKernel_9F793F84(int arg1);
int SceLibKernel_B54C0BE4(int arg1, int arg2, int arg3);
int SceLibKernel_C458D60A(int arg1, int arg2, int arg3, int arg4);
int SceLibKernel_C5C11EE7(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);
int SceLibKernel_DA6EC8EF(int arg1, int arg2, int arg3, int arg4, int arg5);
int SceLibKernel_DDB395A9(int arg1, int arg2, int arg3);
int SceLibKernel_F08DE149(int arg1, int arg2, int arg3, int arg4);
int SceThreadmgr_1BBDE3D9(int arg1);
int SceThreadmgrCoredumpTime_0C8A38E1(int arg1);
int SceIpmi_4E255C31(int arg1);
int SceIpmi_B282B430(int arg1, int arg2, int arg3, int arg4);
int SceLibKernel_3B9E301A(int arg1, int arg2);
int SceLibKernel_86EF7680(int arg1, int arg2, int arg3);
int SceLibKernel_9C56B4D1(int arg1, int arg2, int arg3);
int SceLibKernel_AE1A21EC(int arg1);
int SceSysmem_A91E15EE(int arg1);
int SceSysmem_B8EF5818(int arg1, int arg2);
int SceSysmem_B9D5EBDE(int arg1, int arg2, int arg3, int arg4);
