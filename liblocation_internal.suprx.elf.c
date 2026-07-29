//VADDR: 0x81000000 OFF: 0
// Exported
int SceLibLocationInternal_3500A98C()
{
	// push {r4, lr}
	FLAGS = a2 - 0x10000;
	if (Z == 1) // ==
		goto loc_81000012;
	a1 = /*data_8010124D*/;
	goto loc_81000040;

loc_81000012:
//VADDR: 0x81000012 OFF: 0x12
	a1 = sub_81000478();
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_8100003E;
	a1 = sub_8100082E();
	a1 = SceLibLocation_7F74AA94();
	a1 = sub_8100099C();
	FLAGS = a1 - 1;
	if (Z == 0) // !=
		goto loc_8100003E;
	a1 = sub_81000992();
	a1 = sub_8100099C();
	a1 = SceLibLocation_822BBF06((a1 + 0) + 0, a1 + 0);
	v1 = a1 + 0;
	FLAGS = v1;

loc_8100003E:
//VADDR: 0x8100003e OFF: 0x3e
	a1 = v1 + 0;
	FLAGS = a1;

loc_81000040:
//VADDR: 0x81000040 OFF: 0x40
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000042 OFF: 0x42
// Exported
int SceLibLocationInternal_A8531ABF()
{
	// push {r4, r5, r6, r8, sb, lr}
	sp = sp - 0x10;
	v1 = a3 + 0;
	FLAGS = v1;
	v2 = a2;
	v3 = a1;
	if (Z == 0) // !=
		goto loc_8100005A;
	a1 = /*data_80101240*/;
	goto loc_810000A0;

loc_8100005A:
//VADDR: 0x8100005a OFF: 0x5a
	a1 = SceLibLocation_EFC9BD25();
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_8100006C;
	a1 = /*data_80101241*/;
	goto loc_810000A0;

loc_8100006C:
//VADDR: 0x8100006c OFF: 0x6c
	a1 = sub_810008A6();
	a1 = sub_8100083A(v3 + 0);
	v3 = a1 + 0;
	FLAGS = v3;
	if (Z == 0) // !=
		goto loc_81000084;
	a1 = /*data_80101241*/;
	goto loc_810000A0;

loc_81000084:
//VADDR: 0x81000084 OFF: 0x84
	v5 = 0;
	sb = 0;
	a1 = sub_8100082E();
	asm("stm sp, {r8, sb}\n");
	*(uint32_t *)(sp + 8) = v2;
	*(uint32_t *)(sp + 8 + 0x4) = v1;
	a1 = sub_810008B2(a1, v3 + 0, 0, a4, *(sp+0), *(sp+4), *(sp+8));

loc_810000A0:
//VADDR: 0x810000a0 OFF: 0xa0
	sp = sp + 0x10;
	return a1; // pop {r4, r5, r6, r8, sb, pc}
}

//VADDR: 0x810000a6 OFF: 0xa6
// Exported
int SceLibLocationInternal_29267421()
{
	ip = sp;
	// push {r0, r1, r2, r3}
	// push {r4, r5, r6, ip, lr}
	sp = sp - 0x14;
	v1 = a1 + 0;
	FLAGS = v1;
	v3 = *(uint32_t *)(sp + 0x38);
	if ((*(uint32_t *)(sp + 0x3C)) != 0)
		goto loc_810000C4;
	a1 = /*data_80101240*/;
	goto loc_81000108;

loc_810000C4:
//VADDR: 0x810000c4 OFF: 0xc4
	a1 = SceLibLocation_EFC9BD25();
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_810000D6;
	a1 = /*data_80101241*/;
	goto loc_81000108;

loc_810000D6:
//VADDR: 0x810000d6 OFF: 0xd6
	a1 = sub_810008A6();
	a1 = sub_8100083A(v1 + 0);
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_810000EE;
	a1 = /*data_80101241*/;
	goto loc_81000108;

loc_810000EE:
//VADDR: 0x810000ee OFF: 0xee
	a1 = sub_8100082E();
	a2 = sp + 0x30;
	asm("ldm r1, {r2, r3}\n");
	asm("stm sp, {r2, r3}\n");
	*(uint32_t *)(sp + 8) = v3;
	*(uint32_t *)(sp + 8 + 0x4) = v2;
	a1 = sub_810008B2(a1, v1 + 0, 1, a4, *(sp+0), *(sp+4), *(sp+8));

loc_81000108:
//VADDR: 0x81000108 OFF: 0x108
	sp = sp + 0x14;
	asm("ldm sp, {r4, r5, r6, ip, lr}\n");
	sp = ip;
	return a1;
}

//VADDR: 0x81000112 OFF: 0x112
// Exported
int SceLibLocationInternal_EEF11F68()
{
	// push {r4, lr}
	a1 = sub_8100095A(a1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000128;
	a1 = SceLibLocation_822BBF06((a1 + 0) + 0, 0);

loc_81000128:
//VADDR: 0x81000128 OFF: 0x128
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100012a OFF: 0x12a
// Exported
int SceLibLocationInternal_209228A3()
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 8;
	v1 = a4 + 0;
	FLAGS = v1;
	v2 = a3 + 0;
	FLAGS = v2;
	v3 = a2 + 0;
	FLAGS = v3;
	v4 = a1 + 0;
	FLAGS = v4;
	a1 = SceLibLocation_EFC9BD25();
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_8100014A;
	a1 = /*data_80101241*/;
	goto loc_8100017A;

loc_8100014A:
//VADDR: 0x8100014a OFF: 0x14a
	a1 = sub_810008A6();
	a1 = sub_8100083A(v4 + 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000162;
	a1 = /*data_80101241*/;
	goto loc_8100017A;

loc_81000162:
//VADDR: 0x81000162 OFF: 0x162
	a1 = SceLibLocation_0C0BB66A();
	a1 = sub_8100082E();
	*(uint32_t *)(sp) = v3;
	*(uint32_t *)(sp + 0x4) = v2;
	a1 = sub_810009AA(a1, v4 + 0, a1, v1 + 0, *(sp+0));

loc_8100017A:
//VADDR: 0x8100017a OFF: 0x17a
	sp = sp + 8;
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x81000180 OFF: 0x180
// Exported
int module_stop()
{
	// push {r4, lr}
	a1 = sub_81000B50();
	a1 = sub_8100023C();
	a1 = 0;
	FLAGS = a1;
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000190 OFF: 0x190
int sub_81000190()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000192 OFF: 0x192
int sub_81000192()
{
	// push {r4, r5, r6, r7, r8, lr}
	v1 = 0;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x60)) - 0;
	if (Z == 1) // ==
		goto loc_81000204;
	v2 = 0;

loc_810001AE:
//VADDR: 0x810001ae OFF: 0x1ae
	if ((*(uint32_t *)(a2 + v2)) == 0)
		goto loc_810001F8;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2 + v2)) + 0x28)) - -1;
	if (Z == 1) // ==
		goto loc_810001C4;
	a1 = SceThreadmgr_1BBDE3D9(*(uint32_t *)((*(uint32_t *)(a2 + v2)) + 0x28));
	*(uint32_t *)((*(uint32_t *)(a2 + v2)) + 0x28) = -1;

loc_810001C4:
//VADDR: 0x810001c4 OFF: 0x1c4
	FLAGS = (*(uint32_t *)(v3 + 0x2C)) - -1;
	if (Z == 1) // ==
		goto loc_810001D6;
	a1 = SceThreadmgr_1BBDE3D9(*(uint32_t *)(v3 + 0x2C));
	*(uint32_t *)(v3 + 0x2C) = -1;

loc_810001D6:
//VADDR: 0x810001d6 OFF: 0x1d6
	if ((*(uint32_t *)(v3 + 0x30)) != 0)
		goto loc_810001E8;
	a1 = SceLibKernel_244E76D2(v3 + 0x38);
	*(uint32_t *)(v3 + 0x30) = -1;

loc_810001E8:
//VADDR: 0x810001e8 OFF: 0x1e8
	a1 = v3 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a2 = *(uint32_t *)(v4);

loc_810001F8:
//VADDR: 0x810001f8 OFF: 0x1f8
	v2 = v2 + 4;
	FLAGS = v2;
	FLAGS = (v5 + 1) - (*(uint32_t *)(a2 + 0x60));
	if (C == 0) // unsigned <
		goto loc_810001AE;

loc_81000204:
//VADDR: 0x81000204 OFF: 0x204
	if ((*(uint32_t *)(a2 + 0x34)) != 0)
		goto loc_81000218;
	a1 = SceLibKernel_244E76D2(a2 + 0x38);
	*(uint32_t *)((*(uint32_t *)(v4)) + 0x34) = -1;

loc_81000218:
//VADDR: 0x81000218 OFF: 0x218
	*(uint32_t *)(a2 + 0x64) = v1;
	*(uint32_t *)((/*data_81002008*/ 0x10000100)) = /*data_10000100*/;
	*(uint32_t *)((/*data_8100202C*/ 0x00000000)) = v1;
	a1 = v1 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x8100023c OFF: 0x23c
int sub_8100023C()
{
	// push {r4, lr}
	if ((*(uint32_t *)((/*data_81002030*/ 0x00000000))) != 0)
		goto loc_81000254;
	a1 = /*data_8010124F*/;
	goto loc_810002D0;

loc_81000254:
//VADDR: 0x81000254 OFF: 0x254
	a1 = sub_81000192();
	if ((*(uint32_t *)(/*data_81002000*/)) == 0)
		goto loc_8100027C;
	a1 = SceLibKernel_632980D7((*(uint32_t *)(/*data_81002000*/)) + 0, 0, 0x20);
	a1 = (*(uint32_t *)(/*data_81002000*/)) + 0;
	FLAGS = a1;
	a1 = r1(...);

loc_8100027C:
//VADDR: 0x8100027c OFF: 0x27c
	*(uint32_t *)(/*data_81002000*/) = 0;
	if ((*(uint8_t *)((/*data_81002004*/ 0x00000001))) == 0)
		goto loc_810002C4;
	FLAGS = (*(uint32_t *)((/*data_81002028*/ 0x00000000))) - 0;
	if (Z == 1 || N != V) // signed <=
		goto loc_810002C4;

loc_810002A2:
//VADDR: 0x810002a2 OFF: 0x2a2
	FLAGS = a1 - 1;
	if (Z == 0) // !=
		goto loc_810002B4;
	a1 = sub_81001108(a1, a2, a3);
	a1 = *(uint32_t *)((/*data_81002028*/ 0x00000000));

loc_810002B4:
//VADDR: 0x810002b4 OFF: 0x2b4
	*(uint32_t *)((/*data_81002028*/ 0x00000000)) = (a1 - 1);
	FLAGS = (a1 - 1) - 0;
	if (Z == 0 && N == V) // signed >
		goto loc_810002A2;

loc_810002C4:
//VADDR: 0x810002c4 OFF: 0x2c4
	*(uint32_t *)((/*data_81002030*/ 0x00000000)) = 0;

loc_810002D0:
//VADDR: 0x810002d0 OFF: 0x2d0
	return a1; // pop {r4, pc}
}

//VADDR: 0x810002d2 OFF: 0x2d2
int sub_810002D2(int arg1, int arg2)
{
	// push {r4, r5, r6, r7, r8, sb, lr}
	sp = sp - 0x14;
	*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x60) = a1;
	*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x64) = a2;
	a2 = 0x1A30 + 0;
	FLAGS = a2;
	*(uint32_t *)(sp) = 0;
	a1 = SceLibKernel_DA6EC8EF((*(uint32_t *)(/*data_81002000*/)) + 0x38, 0x81000000 | a2, 0, 0, *(sp+0));
	FLAGS = a1 - 0;
	if (N == V) // signed >=
		goto loc_81000306;
	goto loc_81000470;

loc_81000306:
//VADDR: 0x81000306 OFF: 0x306
	*(uint32_t *)((*(uint32_t *)(v2)) + 0x34) = a1;
	a1 = 0x2008 + 0;
	FLAGS = a1;
	a1 = SceLibLocation_991AF2D0(0x81000000 | a1, /*data_8100202C*/ 0x00000000, 0x2008);
	v1 = /*data_801012FF*/;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(v2)) + 0x60)) - 0;
	if (Z == 1) // ==
		goto loc_8100046E;
	v4 = v3 + 0;
	FLAGS = v4;

loc_81000334:
//VADDR: 0x81000334 OFF: 0x334
	lr = v3;
	sb = a2;

loc_8100033A:
//VADDR: 0x8100033a OFF: 0x33a
	FLAGS = (*(uint32_t *)(sb)) - 0;
	if (Z == 0) // !=
		goto loc_81000446;
	a1 = 0x58;
	FLAGS = a1;
	a1 = r1(...);
	*(uint32_t *)((*(uint32_t *)(v2)) + v5) = a1;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(v2)) + v5)) - 0;
	if (Z == 0) // !=
		goto loc_8100036E;
	v1 = /*data_80101242*/;
	goto loc_8100045A;

loc_8100036E:
//VADDR: 0x8100036e OFF: 0x36e
	*(uint32_t *)(lr + 4) = v3;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5))) = v3;
	*(uint32_t *)(sp) = v3;
	a1 = SceLibKernel_DA6EC8EF(*(uint32_t *)((*(uint32_t *)(v2)) + v5), /*s_text_81001A44*/ "SceIpcClientInfo", 0, 0, *(sp+0));
	FLAGS = a1 - 0;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x30) = a1;
	if (N == V) // signed >=
		goto loc_8100039E;
	v1 = a1 + 0;
	FLAGS = v1;
	goto loc_8100045A;

loc_8100039E:
//VADDR: 0x8100039e OFF: 0x39e
	*(uint32_t *)(sp) = v3;
	*(uint32_t *)(sp + 0x4) = (*(uint32_t *)((/*data_8100202C*/ 0x00000000)));
	*(uint32_t *)(sp + 8) = v3;
	a1 = SceLibKernel_C5C11EE7(/*s_text_81001A58*/ "SceLocationCallback", sub_81000C1C, *(uint32_t *)((/*data_81002008*/ 0x10000100)), 0x1000, *(sp+0), *(sp+4), *(sp+8));
	FLAGS = a1 - 0;
	if (N == V) // signed >=
		goto loc_810003E6;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x28) = -1;
	v1 = a1 + 0;
	FLAGS = v1;
	goto loc_8100045A;

loc_810003E6:
//VADDR: 0x810003e6 OFF: 0x3e6
	*(uint32_t *)(lr + 0x28) = a1;
	*(uint32_t *)(sp) = v3;
	*(uint32_t *)(sp + 0x4) = (*(uint32_t *)((/*data_8100202C*/ 0x00000000)));
	*(uint32_t *)(sp + 8) = v3;
	a1 = SceLibKernel_C5C11EE7(/*s_text_81001A6C*/ "SceHeadingCallback", sub_81000D8E, *(uint32_t *)((/*data_81002008*/ 0x10000100)), 0x1000, *(sp+0), *(sp+4), *(sp+8));
	FLAGS = (a1 + 0) - 0;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x2C) = (a1 + 0);
	if (N == V) // signed >=
		goto loc_81000442;
	a1 = SceThreadmgr_1BBDE3D9(*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x28));
	v1 = *(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x2C);
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x28) = -1;
	*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(v2)) + v5)) + 0x2C) = -1;
	goto loc_8100045A;

loc_81000442:
//VADDR: 0x81000442 OFF: 0x442
	v1 = v3 + 0;
	FLAGS = v1;
	goto loc_81000464;

loc_81000446:
//VADDR: 0x81000446 OFF: 0x446
	sb = sb + 4;
	v5 = v5 + 4;
	FLAGS = (lr + 1) - 8;
	if (C == 0) // unsigned <
		goto loc_8100033A;

loc_8100045A:
//VADDR: 0x8100045a OFF: 0x45a
	if (v1 == 0)
		goto loc_81000464;
	a1 = sub_81000192();
	a1 = v1 + 0;
	FLAGS = a1;
	goto loc_81000470;

loc_81000464:
//VADDR: 0x81000464 OFF: 0x464
	FLAGS = (v4 + 1) - (*(uint32_t *)(a2 + 0x60));
	if (C == 0) // unsigned <
		goto loc_81000334;

loc_8100046E:
//VADDR: 0x8100046e OFF: 0x46e
	a1 = v3 + 0;
	FLAGS = a1;

loc_81000470:
//VADDR: 0x81000470 OFF: 0x470
	sp = sp + 0x14;
	return a1; // pop {r4, r5, r6, r7, r8, sb, pc}
}

//VADDR: 0x81000476 OFF: 0x476
int sub_81000476()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000478 OFF: 0x478
int sub_81000478()
{
	// push {r4, r5, r6, r7, r8, lr}
	v4 = a2;
	if (Z == 1) // ==
		goto loc_81000486;
	FLAGS = (a1 + 0) - 8;
	if (C == 0 || Z == 1) // unsigned <=
		goto loc_81000490;

loc_81000486:
//VADDR: 0x81000486 OFF: 0x486
	a1 = /*data_8010124D*/;
	goto loc_81000702;

loc_81000490:
//VADDR: 0x81000490 OFF: 0x490
	a1 = SceLibLocation_EFC9BD25();
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810004A2;
	a1 = /*data_8010124E*/;
	goto loc_81000702;

loc_810004A2:
//VADDR: 0x810004a2 OFF: 0x4a2
	FLAGS = (*(uint32_t *)(/*data_81002000*/)) - 0;
	if (Z == 1) // ==
		goto loc_8100051A;
	FLAGS = (*(uint32_t *)((/*data_81002030*/ 0x00000000))) - 2;
	if (Z == 0) // !=
		goto loc_810004D6;
	a1 = SceLibKernel_632980D7((*(uint32_t *)(/*data_81002000*/)) + 0, 0, 0x20);
	a1 = (*(uint32_t *)(/*data_81002000*/)) + 0;
	FLAGS = a1;
	a1 = r1(...);

loc_810004D6:
//VADDR: 0x810004d6 OFF: 0x4d6
	*(uint32_t *)(/*data_81002000*/) = 0;
	if ((*(uint8_t *)((/*data_81002004*/ 0x00000001))) == 0)
		goto loc_8100051A;
	FLAGS = (*(uint32_t *)((/*data_81002028*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_8100050C;
	a1 = sub_81001108(/*data_81002028*/ 0x00000000, /*data_81002000*/, *(uint32_t *)((/*data_81002028*/ 0x00000000)));
	a3 = *(uint32_t *)((/*data_81002028*/ 0x00000000));
	goto loc_8100050E;

loc_8100050C:
//VADDR: 0x8100050c OFF: 0x50c
	if (a3 == 0)
		goto loc_8100051A;

loc_8100050E:
//VADDR: 0x8100050e OFF: 0x50e
	*(uint32_t *)((/*data_81002028*/ 0x00000000)) = (a3 - 1);

loc_8100051A:
//VADDR: 0x8100051a OFF: 0x51a
	a1 = SceLibKernel_9F793F84();
	FLAGS = v4 - 1;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000528;
	FLAGS = v4 - 0;
	if (Z == 1) // ==
		goto loc_81000558;
	goto loc_8100053E;

loc_81000528:
//VADDR: 0x81000528 OFF: 0x528
	FLAGS = v4 - 0x10000;
	if (Z == 0) // !=
		goto loc_81000558;
	*(uint32_t *)((/*data_8100200C*/ 0x00001000)) = 0x2000;
	goto loc_81000566;

loc_8100053E:
//VADDR: 0x8100053e OFF: 0x53e
	a3 = 0x1000;
	FLAGS = a3;
	FLAGS = a1 - 0x1800000;
	if (C == 1) // unsigned >=
		goto loc_8100054C;
	a3 = 0x2000;
	FLAGS = a3;

loc_8100054C:
//VADDR: 0x8100054c OFF: 0x54c
	*(uint32_t *)((/*data_8100200C*/ 0x00001000)) = a3;
	goto loc_81000566;

loc_81000558:
//VADDR: 0x81000558 OFF: 0x558
	*(uint32_t *)((/*data_8100200C*/ 0x00001000)) = 0x1000;

loc_81000566:
//VADDR: 0x81000566 OFF: 0x566
	FLAGS = a1 - 0x2000000;
	if (C == 0) // unsigned <
		goto loc_8100058E;
	FLAGS = v4 - 0x10000;
	if (Z == 0) // !=
		goto loc_8100058E;
	a1 = SceLibLocation_2311B24A();
	a3 = *(uint32_t *)((/*data_8100200C*/ 0x00001000));
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_8100058E;
	a1 = /*data_8010124D*/;
	goto loc_81000702;

loc_8100058E:
//VADDR: 0x8100058e OFF: 0x58e
	if ((*(uint8_t *)((/*data_81002004*/ 0x00000001))) == 0)
		goto loc_810005E6;
	a1 = 0;
	FLAGS = a1;
	if ((*(uint32_t *)((/*data_81002028*/ 0x00000000))) != 0)
		goto loc_810005D4;
	*(uint32_t *)((/*data_81002020*/ 0x00000000)) = (*(uint32_t *)((/*text_81001A28*/ 0x14F242B5)));
	*(uint32_t *)((/*data_81002020*/ 0x00000000) + 4) = (*(uint32_t *)((/*text_81001A28*/ 0x14F242B5) + 4));
	a1 = sub_8100107C(a3 + 0, *(uint32_t *)((/*text_81001A28*/ 0x14F242B5)), a3, /*data_81002020*/ 0x00000000);
	a4 = *(uint32_t *)((/*data_81002028*/ 0x00000000));
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_810005E0;

loc_810005D4:
//VADDR: 0x810005d4 OFF: 0x5d4
	*(uint32_t *)((/*data_81002028*/ 0x00000000)) = (a4 + 1);

loc_810005E0:
//VADDR: 0x810005e0 OFF: 0x5e0
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_81000700;

loc_810005E6:
//VADDR: 0x810005e6 OFF: 0x5e6
	FLAGS = (*(uint32_t *)(/*data_81002000*/)) - 0;
	if (Z == 0) // !=
		goto loc_8100067E;
	a1 = 0x70;
	FLAGS = a1;
	a1 = r1(...);
	if (Z == 1) // ==
		goto loc_81000630;
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
	v1 = (a1 + 0) + 0;
	FLAGS = v1;

loc_81000630:
//VADDR: 0x81000630 OFF: 0x630
	*(uint32_t *)(/*data_81002000*/) = v1;
	if (v1 != 0)
		goto loc_8100067E;
	if ((*(uint8_t *)((/*data_81002004*/ 0x00000001))) == 0)
		goto loc_81000674;
	FLAGS = (*(uint32_t *)((/*data_81002028*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_81000666;
	a1 = sub_81001108(*(uint32_t *)((/*data_81002028*/ 0x00000000)), a2, a3);
	a1 = *(uint32_t *)((/*data_81002028*/ 0x00000000));
	goto loc_81000668;

loc_81000666:
//VADDR: 0x81000666 OFF: 0x666
	if (a1 == 0)
		goto loc_81000674;

loc_81000668:
//VADDR: 0x81000668 OFF: 0x668
	*(uint32_t *)((/*data_81002028*/ 0x00000000)) = (a1 - 1);

loc_81000674:
//VADDR: 0x81000674 OFF: 0x674
	a1 = /*data_80101242*/;
	goto loc_81000702;

loc_8100067E:
//VADDR: 0x8100067e OFF: 0x67e
	a1 = sub_810002D2(v3 + 0, v4 + 0);
	v1 = a1 + 0;
	FLAGS = v1;
	if (Z == 0) // !=
		goto loc_81000698;
	*(uint32_t *)((/*data_81002030*/ 0x00000000)) = 2;
	goto loc_81000700;

loc_81000698:
//VADDR: 0x81000698 OFF: 0x698
	if ((*(uint32_t *)(/*data_81002000*/)) == 0)
		goto loc_810006BC;
	a1 = SceLibKernel_632980D7((*(uint32_t *)(/*data_81002000*/)) + 0, 0, 0x20);
	a1 = (*(uint32_t *)(/*data_81002000*/)) + 0;
	FLAGS = a1;
	a1 = r1(...);

loc_810006BC:
//VADDR: 0x810006bc OFF: 0x6bc
	*(uint32_t *)(/*data_81002000*/) = 0;
	if ((*(uint8_t *)((/*data_81002004*/ 0x00000001))) == 0)
		goto loc_81000700;
	FLAGS = (*(uint32_t *)((/*data_81002028*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_810006F2;
	a1 = sub_81001108(/*data_81002028*/ 0x00000000, *(uint32_t *)((/*data_81002028*/ 0x00000000)), 0);
	a2 = *(uint32_t *)((/*data_81002028*/ 0x00000000));
	goto loc_810006F4;

loc_810006F2:
//VADDR: 0x810006f2 OFF: 0x6f2
	if (a2 == 0)
		goto loc_81000700;

loc_810006F4:
//VADDR: 0x810006f4 OFF: 0x6f4
	*(uint32_t *)((/*data_81002028*/ 0x00000000)) = (a2 - 1);

loc_81000700:
//VADDR: 0x81000700 OFF: 0x700
	a1 = v1 + 0;
	FLAGS = a1;

loc_81000702:
//VADDR: 0x81000702 OFF: 0x702
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x81000706 OFF: 0x706
int sub_81000706(int arg1)
{
	// push {r4, r5, r6, lr}
	if ((*(uint32_t *)((a1 + 0) + 4)) != 0)
		goto loc_81000718;
	a1 = /*data_801012FF*/;
	goto loc_81000792;

loc_81000718:
//VADDR: 0x81000718 OFF: 0x718
	a1 = r1(...);
	FLAGS = a1 - /*data_8002050A*/;
	if (Z == 0) // !=
		goto loc_81000734;
	a1 = /*data_801012FF*/;
	goto loc_81000792;

loc_81000734:
//VADDR: 0x81000734 OFF: 0x734
	FLAGS = (*(uint32_t *)(v1 + 0x20)) - 1;
	if (Z == 0) // !=
		goto loc_81000748;
	*(uint32_t *)(v1 + 0x20) = 0;
	a1 = SceLibKernel_DDB395A9(*(uint32_t *)(v1 + 0x28), 0, 0);

loc_81000748:
//VADDR: 0x81000748 OFF: 0x748
	*(uint32_t *)(v1 + 8) = 0;
	*(uint32_t *)(v1 + 0x18) = 0;
	*(uint32_t *)(v1 + 0x10) = 0;
	a1 = r2(...);
	a1 = *(uint32_t *)(v1 + 4);
	a1 = r1(...);
	*(uint32_t *)(v1 + 4) = 0;
	if ((*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x2C)) == 0)
		goto loc_81000790;
	a1 = r1(...);
	*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x2C) = 0;

loc_81000790:
//VADDR: 0x81000790 OFF: 0x790
	a1 = 0;
	FLAGS = a1;

loc_81000792:
//VADDR: 0x81000792 OFF: 0x792
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81000794 OFF: 0x794
int sub_81000794(int arg1)
{
	// push {r4, lr}
	v1 = a1 + 0;
	FLAGS = v1;
	if ((*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x6C)) != 0)
		goto loc_810007B0;
	a1 = /*data_801012FF*/;
	goto loc_8100082A;

loc_810007B0:
//VADDR: 0x810007b0 OFF: 0x7b0
	FLAGS = (*(uint8_t *)(a2 + 0x5C)) - 1;
	if (Z == 0) // !=
		goto loc_810007D4;
	a1 = r1(...);
	FLAGS = a1 - /*data_8002050A*/;
	if (Z == 0) // !=
		goto loc_810007D4;
	a1 = /*data_801012FF*/;
	goto loc_8100082A;

loc_810007D4:
//VADDR: 0x810007d4 OFF: 0x7d4
	FLAGS = (*(uint32_t *)(v1 + 0x24)) - 1;
	if (Z == 0) // !=
		goto loc_810007E8;
	*(uint32_t *)(v1 + 0x24) = 0;
	a1 = SceLibKernel_DDB395A9(*(uint32_t *)(v1 + 0x2C), 0, 0);

loc_810007E8:
//VADDR: 0x810007e8 OFF: 0x7e8
	*(uint32_t *)(v1 + 0xC) = 0;
	*(uint32_t *)(v1 + 0x1C) = 0;
	*(uint32_t *)(v1 + 0x14) = 0;
	FLAGS = (*(uint8_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x5C)) - 1;
	if (Z == 0) // !=
		goto loc_81000828;
	a1 = r1(...);
	a1 = *(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x6C);
	a1 = r1(...);
	*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x6C) = 0;

loc_81000828:
//VADDR: 0x81000828 OFF: 0x828
	a1 = 0;
	FLAGS = a1;

loc_8100082A:
//VADDR: 0x8100082a OFF: 0x82a
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100082c OFF: 0x82c
int sub_8100082C()
{
	*(uint16_t *)(v3 + 0xE) = a1;
}

//VADDR: 0x8100082e OFF: 0x82e
int sub_8100082E()
{
	a1 = *(uint32_t *)(/*data_81002000*/);
	return a1;
}

//VADDR: 0x8100083a OFF: 0x83a
int sub_8100083A(int arg1)
{
	// push {r4, r5, r6, lr}
	if ((*(uint32_t *)(/*data_81002000*/)) == 0)
		goto loc_81000852;
	FLAGS = (a1 + 0) - -1;
	if (Z == 1) // ==
		goto loc_81000852;
	if ((a1 + 0) != 0)
		goto loc_81000856;

loc_81000852:
//VADDR: 0x81000852 OFF: 0x852
	a1 = 0;
	FLAGS = a1;
	goto loc_810008A4;

loc_81000856:
//VADDR: 0x81000856 OFF: 0x856
	a1 = SceLibKernel_46E7BE7B(v2 + 0x38, 1, 0);
	a1 = (*(uint32_t *)(/*data_81002000*/)) + 0x38;
	FLAGS = a1;
	a3 = 0 + 0;
	FLAGS = a3;

loc_81000874:
//VADDR: 0x81000874 OFF: 0x874
	if ((*(uint32_t *)(a2)) == 0)
		goto loc_81000892;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)))) - v1;
	if (Z == 0) // !=
		goto loc_81000892;
	a1 = SceLibKernel_91FA6614(a1, 1);
	a1 = *(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + v2);
	goto loc_810008A4;

loc_81000892:
//VADDR: 0x81000892 OFF: 0x892
	a2 = a2 + 4;
	FLAGS = a2;
	v2 = v2 + 4;
	FLAGS = v2;
	FLAGS = (a3 + 1) - 8;
	if (N != V) // signed <
		goto loc_81000874;
	a1 = SceLibKernel_91FA6614(a1, 1);
	a1 = 0;
	FLAGS = a1;

loc_810008A4:
//VADDR: 0x810008a4 OFF: 0x8a4
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x810008a6 OFF: 0x8a6
int sub_810008A6()
{
	*(uint32_t *)(/*data_81002000*/) = a1;
	return a1;
}

//VADDR: 0x810008b2 OFF: 0x8b2
int sub_810008B2(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
	// push {r4, r5, r6, r7, r8, sb, sl, lr}
	sp = sp - 0x18;
	*(uint32_t *)(sp + 0x14) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
	asm("ldm r5, {r8, sb}\n");
	asm("stm sp, {r8, sb}\n");
	*(uint32_t *)(sp + 8) = (*(uint32_t *)((sp + 0x38) + 8));
	*(uint32_t *)(sp + 8 + 0x4) = (*(uint32_t *)((sp + 0x38) + 0xC));
	a1 = sub_81000F96(*(uint32_t *)((a2 + 0) + 4), *(uint32_t *)((a2 + 0)), (uint8_t)a3 + 0, 1, *(sp+0), *(sp+4), *(sp+8));
	a4 = *(uint32_t *)((a2 + 0) + 4);
	FLAGS = a1 - /*data_80101287*/;
	if (Z == 0) // !=
		goto loc_81000942;
	a3 = 6;
	FLAGS = a3;
	if ((uint8_t)a3 == 0)
		goto loc_810008FC;
	a3 = 0x18;
	FLAGS = a3;

loc_810008FC:
//VADDR: 0x810008fc OFF: 0x8fc
	a1 = a4 + 0;
	FLAGS = a1;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = 0;
	a2 = 0;
	FLAGS = a2;
	a4 = 5;
	FLAGS = a4;
	a1 = r7(...);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000942;
	a2 = (*(uint32_t *)(sp + 0x10)) & 0xA;
	FLAGS = a2;
	if (Z == 1) // ==
		goto loc_8100092C;
	*(uint32_t *)(sp) = v2;
	a1 = sub_81001016(*(uint32_t *)(v1 + 4), *(uint32_t *)(v1), v3 + 0, 1, *(sp+0));
	goto loc_81000942;

loc_8100092C:
//VADDR: 0x8100092c OFF: 0x92c
	a1 = a1 & 0x14;
	FLAGS = a1;
	a1 = /*data_801012FF*/;
	if (Z == 1) // ==
		goto loc_81000942;
	a1 = /*data_80101202*/;

loc_81000942:
//VADDR: 0x81000942 OFF: 0x942
	FLAGS = (*(uint32_t *)(sl)) - (*(uint32_t *)(sp + 0x14));
	if (Z == 0) // !=
		goto loc_81000952;
	sp = sp + 0x18;
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, pc}

loc_81000952:
//VADDR: 0x81000952 OFF: 0x952
	a1 = SceLibKernel_37691BF8();
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100095a OFF: 0x95a
int sub_8100095A(int arg1, int arg2)
{
	a2 = *(uint32_t *)((/*data_81002030*/ 0x00000000));
	a2 = a2 | (uint8_t)a2;
	FLAGS = a2;
	a2 = /*data_8010128F*/;
	if (Z == 0) // !=
		goto loc_8100098E;
	*(uint32_t *)((/*data_81002020*/ 0x00000000)) = (*(uint32_t *)(a1));
	*(uint32_t *)((/*data_81002020*/ 0x00000000) + 4) = (*(uint32_t *)(a1 + 4));
	a2 = 0;
	FLAGS = a2;
	*(uint8_t *)((/*data_81002004*/ 0x00000001)) = (uint8_t)a2;

loc_8100098E:
//VADDR: 0x8100098e OFF: 0x98e
	a1 = a2 + 0;
	FLAGS = a1;
	return a1;
}

//VADDR: 0x81000992 OFF: 0x992
int sub_81000992()
{
	a1 = /*data_81002020*/ 0x00000000;
	return a1;
}

//VADDR: 0x8100099c OFF: 0x99c
int sub_8100099C()
{
	a1 = *(uint8_t *)((/*data_81002004*/ 0x00000001));
	return a1;
}

//VADDR: 0x810009a8 OFF: 0x9a8
int sub_810009A8()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x810009aa OFF: 0x9aa
int sub_810009AA(int arg1, int arg2, int arg3, int arg4, int arg5)
{
	// push {r4, r5, r6, r7, r8, sb, lr}
	sp = sp - 0xC;
	*(uint32_t *)(sp + 4) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
	v1 = *(uint32_t *)(sp + 0x28);
	v2 = *(uint32_t *)((sp + 0x28) + 4);
	*(uint16_t *)(sp) = 0xA;
	FLAGS = (*(uint32_t *)(a1 + 0x64)) - 0x10000;
	if (Z == 1) // ==
		goto loc_810009E0;
	a1 = /*data_801012FF*/;
	goto loc_81000B3A;

loc_810009E0:
//VADDR: 0x810009e0 OFF: 0x9e0
	*(uint32_t *)(v5 + 0x28) = a4;
	*(uint32_t *)(v5 + 0x68) = 1;
	if ((*(uint32_t *)(v5 + 0x2C)) != 0)
		goto loc_81000A10;
	a1 = 0x800;
	FLAGS = a1;
	a1 = r1(...);
	*(uint32_t *)(v5 + 0x2C) = a1;
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000A10;
	*(uint32_t *)(v5 + 0x68) = 0;
	a1 = /*data_80101242*/;
	goto loc_81000B3A;

loc_81000A10:
//VADDR: 0x81000a10 OFF: 0xa10
	a1 = SceLibKernel_632980D7(a1, 0, 0x800);
	FLAGS = v1 - 0;
	if (Z == 0) // !=
		goto loc_81000A22;
	v3 = 0;
	FLAGS = v3;
	goto loc_81000A3A;

loc_81000A22:
//VADDR: 0x81000a22 OFF: 0xa22
	a3 = v1 + 2;
	FLAGS = a3;
	v3 = 0;
	if ((*(uint16_t *)(v1)) == 0)
		goto loc_81000A38;
	v3 = 0;
	FLAGS = v3;

loc_81000A2E:
//VADDR: 0x81000a2e OFF: 0xa2e
	v3 = v3 + 1;
	FLAGS = v3;
	FLAGS = (*(uint16_t *)(a3)) - 0;
	if (Z == 0) // !=
		goto loc_81000A2E;

loc_81000A38:
//VADDR: 0x81000a38 OFF: 0xa38
	v3 = v3 << 1;
	FLAGS = v3;

loc_81000A3A:
//VADDR: 0x81000a3a OFF: 0xa3a
	if (v2 != 0)
		goto loc_81000A40;
	v4 = 0;
	FLAGS = v4;
	goto loc_81000A58;

loc_81000A40:
//VADDR: 0x81000a40 OFF: 0xa40
	a4 = v2 + 2;
	FLAGS = a4;
	v4 = 0;
	if ((*(uint16_t *)(v2)) == 0)
		goto loc_81000A56;
	v4 = 0;
	FLAGS = v4;

loc_81000A4C:
//VADDR: 0x81000a4c OFF: 0xa4c
	v4 = v4 + 1;
	FLAGS = v4;
	FLAGS = (*(uint16_t *)(a4)) - 0;
	if (Z == 0) // !=
		goto loc_81000A4C;

loc_81000A56:
//VADDR: 0x81000a56 OFF: 0xa56
	v4 = v4 << 1;
	FLAGS = v4;

loc_81000A58:
//VADDR: 0x81000a58 OFF: 0xa58
	FLAGS = v1 - 0;
	if (Z == 1) // ==
		goto loc_81000B02;
	FLAGS = v2 - 0;
	if (Z == 1) // ==
		goto loc_81000ADE;
	*(uint32_t *)(v5 + 0x30) = 3;
	FLAGS = v3 - 0x7FE;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000AD2;
	a1 = SceLibKernel_2E3B02A1(*(uint32_t *)(v5 + 0x2C), v1 + 0, v3 + 0);
	FLAGS = (v3 + 2) - 0x7FE;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000B38;
	a1 = SceLibKernel_2E3B02A1((*(uint32_t *)(v5 + 0x2C)) + v3, sp + 0, 2);
	FLAGS = (v3 + 4) - 0x7FE;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000B38;
	a1 = SceLibKernel_2E3B02A1((*(uint32_t *)(v5 + 0x2C)) + (v3 + 2), sp + 0, 2);
	FLAGS = ((v3 + 4) + v4) - 0x7FE;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000AC4;
	a1 = SceLibKernel_2E3B02A1((*(uint32_t *)(v5 + 0x2C)) + (v3 + 4), v2 + 0, v4 + 0);
	goto loc_81000B38;

loc_81000AC4:
//VADDR: 0x81000ac4 OFF: 0xac4
	a1 = SceLibKernel_2E3B02A1(a1, v2 + 0, (0 - v3) + 0x7FE);
	goto loc_81000B38;

loc_81000AD2:
//VADDR: 0x81000ad2 OFF: 0xad2
	a1 = SceLibKernel_2E3B02A1(a1, v1 + 0, 0x7FE);
	goto loc_81000B38;

loc_81000ADE:
//VADDR: 0x81000ade OFF: 0xade
	*(uint32_t *)(v5 + 0x30) = 1;
	FLAGS = v3 - 0x7FE;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000AF6;
	a1 = SceLibKernel_2E3B02A1(a1, v1 + 0, v3 + 0);
	goto loc_81000B38;

loc_81000AF6:
//VADDR: 0x81000af6 OFF: 0xaf6
	a1 = SceLibKernel_2E3B02A1(a1, v1 + 0, 0x7FE);
	goto loc_81000B38;

loc_81000B02:
//VADDR: 0x81000b02 OFF: 0xb02
	if (v2 == 0)
		goto loc_81000B2C;
	*(uint32_t *)(v5 + 0x30) = 2;
	FLAGS = v4 - 0x7FE;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000B20;
	a1 = SceLibKernel_2E3B02A1(*(uint32_t *)(v5 + 0x2C), v2 + 0, v4 + 0);
	goto loc_81000B38;

loc_81000B20:
//VADDR: 0x81000b20 OFF: 0xb20
	a1 = SceLibKernel_2E3B02A1(a1, v2 + 0, 0x7FE);
	goto loc_81000B38;

loc_81000B2C:
//VADDR: 0x81000b2c OFF: 0xb2c
	a1 = v1 | v2;
	FLAGS = a1;
	if (Z == 0) // !=
		goto loc_81000B38;
	*(uint32_t *)(v5 + 0x30) = 0;

loc_81000B38:
//VADDR: 0x81000b38 OFF: 0xb38
	a1 = 0;
	FLAGS = a1;

loc_81000B3A:
//VADDR: 0x81000b3a OFF: 0xb3a
	FLAGS = (*(uint32_t *)(sb)) - (*(uint32_t *)(sp + 4));
	if (Z == 0) // !=
		goto loc_81000B4A;
	sp = sp + 0xC;
	return a1; // pop {r4, r5, r6, r7, r8, sb, pc}

loc_81000B4A:
//VADDR: 0x81000b4a OFF: 0xb4a
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000b50 OFF: 0xb50
int sub_81000B50()
{
	// push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sp = sp - 4;
	if ((*(uint32_t *)((/*data_81002030*/ 0x00000000))) != 0)
		goto loc_81000B6C;
	a1 = /*data_8010124F*/;
	goto loc_81000C16;

loc_81000B6C:
//VADDR: 0x81000b6c OFF: 0xb6c
	sb = 0;

loc_81000B76:
//VADDR: 0x81000b76 OFF: 0xb76
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + sb)) - 0;
	if (Z == 1) // ==
		goto loc_81000C0C;
	FLAGS = (*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + sb)))) - 0;
	if (Z == 1) // ==
		goto loc_81000C0C;
	FLAGS = (*(uint32_t *)((*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + sb)))) - -1;
	if (Z == 1) // ==
		goto loc_81000C0C;
	a1 = SceLibKernel_46E7BE7B((*(uint32_t *)(/*data_81002000*/)) + 0x38, 1, 0);
	ip = sl;
	fp = (*(uint32_t *)(/*data_81002000*/));

loc_81000BB4:
//VADDR: 0x81000bb4 OFF: 0xbb4
	if ((*(uint32_t *)(fp)) == 0)
		goto loc_81000BF2;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(fp)))) - v2;
	if (Z == 0) // !=
		goto loc_81000BF2;
	a1 = sub_81000706((*(uint32_t *)(fp)) + 0);
	a1 = sub_81000794((*(uint32_t *)(fp)) + 0);
	*(uint32_t *)((*(uint32_t *)(fp))) = -1;
	v3 = a1 + 0;
	FLAGS = v3;
	a1 = *(uint8_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x5C);
	*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x58) = v5;
	*(uint8_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x5C) = (a1 - 1);
	lr = *(uint32_t *)(/*data_81002000*/);
	goto loc_81000C00;

loc_81000BF2:
//VADDR: 0x81000bf2 OFF: 0xbf2
	fp = fp + 4;
	FLAGS = (ip + 1) - 8;
	if (N != V) // signed <
		goto loc_81000BB4;

loc_81000C00:
//VADDR: 0x81000c00 OFF: 0xc00
	a1 = SceLibKernel_91FA6614(lr + 0x38, 1);
	a1 = v3 + 0;
	FLAGS = a1;

loc_81000C0C:
//VADDR: 0x81000c0c OFF: 0xc0c
	sb = sb + 4;
	FLAGS = (v1 + 1) - 8;
	if (N != V) // signed <
		goto loc_81000B76;

loc_81000C16:
//VADDR: 0x81000c16 OFF: 0xc16
	sp = sp + 4;
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
}

//VADDR: 0x81000c1c OFF: 0xc1c
int sub_81000C1C()
{
	// push {r4, r5, r6, r7, r8, sb, lr}
	sp = sp - 0x4C;
	*(uint32_t *)(sp + 0x48) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
	*(uint32_t *)(sp + 8) = 0;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)) + 0x20)) - 0;
	if (Z == 1) // ==
		goto loc_81000D70;
	v5 = (*(uint32_t *)(a2)) + 0x38;
	v3 = 0 + 0;
	FLAGS = v3;

loc_81000C44:
//VADDR: 0x81000c44 OFF: 0xc44
	*(uint32_t *)(sp + 0xC) = 0x2710;
	a2 = 0;
	FLAGS = a2;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0xC);
	a3 = 3;
	FLAGS = a3;
	a4 = 5;
	FLAGS = a4;
	a1 = r7(...);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000D3C;
	FLAGS = (*(uint32_t *)(sp + 0x10)) - 1;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000C70;
	FLAGS = (*(uint32_t *)(sp + 0x10)) - 0;
	if (Z == 1) // ==
		goto loc_81000D3C;
	goto loc_81000C76;

loc_81000C70:
//VADDR: 0x81000c70 OFF: 0xc70
	FLAGS = a2 - 3;
	if (Z == 1) // ==
		goto loc_81000CE2;
	goto loc_81000D3C;

loc_81000C76:
//VADDR: 0x81000c76 OFF: 0xc76
	a1 = SceLibKernel_632980D7((sp + 0x18) + 0, 0, 0x30);
	a1 = sub_81000E88(*(uint32_t *)(v1 + 4), *(uint32_t *)(v1), (sp + 0x18) + 0);
	a1 = SceLibKernel_46E7BE7B(v5, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000D3C;
	if ((*(uint32_t *)(v1 + 8)) == 0)
		goto loc_81000CD8;
	FLAGS = (a1 + 0) - /*data_80101205*/;
	if (Z == 0) // !=
		goto loc_81000CCC;
	a1 = SceLibKernel_9F793F84();
	FLAGS = a1 - 0x1800000;
	if (C == 0) // unsigned <
		goto loc_81000CC4;
	if ((*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x64)) != 0)
		goto loc_81000CCC;

loc_81000CC4:
//VADDR: 0x81000cc4 OFF: 0xcc4
	v4 = /*data_80101200*/;

loc_81000CCC:
//VADDR: 0x81000ccc OFF: 0xccc
	a2 = *(uint32_t *)(v1);
	a1 = v4 + 0;
	FLAGS = a1;
	a4 = *(uint32_t *)(v1 + 0x18);
	a3 = sp + 0x18;
	a1 = r7(...);

loc_81000CD8:
//VADDR: 0x81000cd8 OFF: 0xcd8
	a1 = SceLibKernel_91FA6614(v5, 1);
	goto loc_81000D3C;

loc_81000CE2:
//VADDR: 0x81000ce2 OFF: 0xce2
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x58)) - 1;
	if (Z == 1) // ==
		goto loc_81000CF4;
	v3 = *(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x20);

loc_81000CF4:
//VADDR: 0x81000cf4 OFF: 0xcf4
	FLAGS = v3 - 1;
	if (Z == 0) // !=
		goto loc_81000D3C;
	a1 = sub_81000F3E(*(uint32_t *)(v1 + 4), *(uint32_t *)(v1), sp + 8);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000D3C;
	a1 = SceLibKernel_46E7BE7B(v5, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000D3C;
	FLAGS = (*(uint32_t *)(sp + 8)) - 0;
	*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x24) = (*(uint32_t *)(sp + 8));
	lr = 2;
	a2 = *(uint32_t *)(/*data_81002000*/);
	if (Z == 0) // !=
		goto loc_81000D2E;
	lr = v2;

loc_81000D2E:
//VADDR: 0x81000d2e OFF: 0xd2e
	*(uint32_t *)((a2 + 0) + 0x20) = lr;
	a1 = SceLibKernel_91FA6614(v5, 1);

loc_81000D3C:
//VADDR: 0x81000d3c OFF: 0xd3c
	a1 = SceLibKernel_46E7BE7B(v5, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000D68;
	if ((*(uint32_t *)(v1 + 0x10)) != 0)
		goto loc_81000D60;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x20)) - 1;
	if (Z == 1) // ==
		goto loc_81000D60;
	*(uint32_t *)(v1 + 0x20) = v2;

loc_81000D60:
//VADDR: 0x81000d60 OFF: 0xd60
	a1 = SceLibKernel_91FA6614(v5, 1);

loc_81000D68:
//VADDR: 0x81000d68 OFF: 0xd68
	FLAGS = (*(uint32_t *)(v1 + 0x20)) - 0;
	if (Z == 0) // !=
		goto loc_81000C44;

loc_81000D70:
//VADDR: 0x81000d70 OFF: 0xd70
	a1 = SceThreadmgrCoredumpTime_0C8A38E1(0);
	a1 = v2 + 0;
	FLAGS = a1;
	FLAGS = (*(uint32_t *)(sb)) - (*(uint32_t *)(sp + 0x48));
	if (Z == 0) // !=
		goto loc_81000D88;
	sp = sp + 0x4C;
	return a1; // pop {r4, r5, r6, r7, r8, sb, pc}

loc_81000D88:
//VADDR: 0x81000d88 OFF: 0xd88
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000d8e OFF: 0xd8e
int sub_81000D8E()
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x38;
	*(uint32_t *)(sp + 0x30) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
	v1 = 0;
	FLAGS = v1;
	FLAGS = (*(uint32_t *)((*(uint32_t *)(a2)) + 0x24)) - 0;
	if (Z == 1) // ==
		goto loc_81000E44;
	v2 = (*(uint32_t *)(a2)) + 0x38;
	FLAGS = v2;

loc_81000DB0:
//VADDR: 0x81000db0 OFF: 0xdb0
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
		goto loc_81000E22;
	a1 = sub_81000EE4(*(uint32_t *)((*(uint32_t *)(/*data_81002000*/)) + 0x6C), *(uint32_t *)(v3), sp + 0x10);
	*(uint32_t *)(sp + 8) = a1;
	a1 = SceLibKernel_46E7BE7B(v2 + 0, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000E22;
	if ((*(uint32_t *)(v3 + 0xC)) == 0)
		goto loc_81000E1A;
	a1 = *(uint32_t *)(sp + 8);
	a3 = sp + 0x10;
	a2 = *(uint32_t *)(v3);
	a4 = *(uint32_t *)(v3 + 0x1C);
	a1 = lr(...);

loc_81000E1A:
//VADDR: 0x81000e1a OFF: 0xe1a
	a1 = SceLibKernel_91FA6614(v2 + 0, 1);

loc_81000E22:
//VADDR: 0x81000e22 OFF: 0xe22
	a1 = SceLibKernel_46E7BE7B(v2 + 0, 1, 0);
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000E3E;
	if ((*(uint32_t *)(v3 + 0x14)) != 0)
		goto loc_81000E36;
	*(uint32_t *)(v3 + 0x24) = v1;

loc_81000E36:
//VADDR: 0x81000e36 OFF: 0xe36
	a1 = SceLibKernel_91FA6614(v2 + 0, 1);

loc_81000E3E:
//VADDR: 0x81000e3e OFF: 0xe3e
	FLAGS = (*(uint32_t *)(v3 + 0x24)) - 0;
	if (Z == 0) // !=
		goto loc_81000DB0;

loc_81000E44:
//VADDR: 0x81000e44 OFF: 0xe44
	a1 = SceThreadmgrCoredumpTime_0C8A38E1(0);
	a1 = v1 + 0;
	FLAGS = a1;
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x30));
	if (Z == 0) // !=
		goto loc_81000E5C;
	sp = sp + 0x38;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_81000E5C:
//VADDR: 0x81000e5c OFF: 0xe5c
	a1 = SceLibKernel_37691BF8();
	*(uint32_t *)((/*data_81002020*/ 0x00000000)) = (*(uint32_t *)((/*text_81001A28*/ 0x14F242B5)));
	*(uint32_t *)((/*data_81002020*/ 0x00000000) + 4) = (*(uint32_t *)((/*text_81001A28*/ 0x14F242B5) + 4));
	return a1;
}

//VADDR: 0x81000e7c OFF: 0xe7c
// Exported
int module_start()
{
	// push {r4, lr}
	a1 = sub_81001174();
	a1 = 0;
	FLAGS = a1;
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000e86 OFF: 0xe86
int sub_81000E86()
{
	a1 = a1;
	FLAGS = a1;
}

//VADDR: 0x81000e88 OFF: 0xe88
int sub_81000E88(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x30;
	*(uint32_t *)(sp + 0x2C) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
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
		goto loc_81000ECE;
	goto loc_81000ED0;

loc_81000ECE:
//VADDR: 0x81000ece OFF: 0xece
	a1 = *(uint32_t *)(sp + 0x10);

loc_81000ED0:
//VADDR: 0x81000ed0 OFF: 0xed0
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x2C));
	if (Z == 0) // !=
		goto loc_81000EDC;
	sp = sp + 0x30;
	return a1; // pop {r4, r5, r6, pc}

loc_81000EDC:
//VADDR: 0x81000edc OFF: 0xedc
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000ee4 OFF: 0xee4
int sub_81000EE4(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x30;
	*(uint32_t *)(sp + 0x2C) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
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
		goto loc_81000F2A;
	goto loc_81000F2C;

loc_81000F2A:
//VADDR: 0x81000f2a OFF: 0xf2a
	a1 = *(uint32_t *)(sp + 0x10);

loc_81000F2C:
//VADDR: 0x81000f2c OFF: 0xf2c
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x2C));
	if (Z == 0) // !=
		goto loc_81000F38;
	sp = sp + 0x30;
	return a1; // pop {r4, r5, r6, pc}

loc_81000F38:
//VADDR: 0x81000f38 OFF: 0xf38
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000f3e OFF: 0xf3e
int sub_81000F3E(int arg1, int arg2, int arg3)
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0x34;
	*(uint32_t *)(sp + 0x2C) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
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
		goto loc_81000F82;
	goto loc_81000F84;

loc_81000F82:
//VADDR: 0x81000f82 OFF: 0xf82
	a1 = *(uint32_t *)(sp + 0x10);

loc_81000F84:
//VADDR: 0x81000f84 OFF: 0xf84
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 0x2C));
	if (Z == 0) // !=
		goto loc_81000F90;
	sp = sp + 0x34;
	return a1; // pop {r4, r5, r6, r7, pc}

loc_81000F90:
//VADDR: 0x81000f90 OFF: 0xf90
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000f96 OFF: 0xf96
int sub_81000F96(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x48;
	*(uint32_t *)(sp + 0x44) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
	*(uint8_t *)(sp + 0x1D) = a4;
	*(uint8_t *)(sp + 0x1C) = a3;
	asm("ldm r4, {r6, r7}\n");
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x30) = (sp + 0x18);
	*(uint32_t *)(sp + 0x30 + 0x4) = 0x18;
	*(uint32_t *)(sp + 0x18) = a2;
	*(uint32_t *)(sp + 0x20) = v3;
	*(uint32_t *)(sp + 0x20 + 0x4) = v4;
	*(uint32_t *)(sp + 0x28) = (*(uint32_t *)((sp + 0x60) + 8));
	*(uint32_t *)(sp + 0x38) = (*(uint32_t *)((sp + 0x60) + 0xC));
	*(uint32_t *)(sp + 0x38 + 0x4) = 0x30;
	a4 = 1;
	FLAGS = a4;
	a3 = sp + 0x30;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x38);
	a2 = /*data_12340015*/;
	*(uint32_t *)(sp + 8) = 1;
	a1 = r6(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81000FFE;
	goto loc_81001000;

loc_81000FFE:
//VADDR: 0x81000ffe OFF: 0xffe
	a1 = *(uint32_t *)(sp + 0x10);

loc_81001000:
//VADDR: 0x81001000 OFF: 0x1000
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x44));
	if (Z == 0) // !=
		goto loc_81001010;
	sp = sp + 0x48;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_81001010:
//VADDR: 0x81001010 OFF: 0x1010
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81001016 OFF: 0x1016
int sub_81001016(int arg1, int arg2, int arg3, int arg4, int arg5)
{
	// push {r4, r5, r6, lr}
	sp = sp - 0x38;
	*(uint32_t *)(sp + 0x30) = (*(uint32_t *)((/*text_81001774*/ 0x00000940)));
	*(uint8_t *)(sp + 0x19) = a4;
	*(uint8_t *)(sp + 0x18) = a3;
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x1C) = (sp + 0x14);
	*(uint32_t *)(sp + 0x20) = 8;
	*(uint32_t *)(sp + 0x24) = (*(uint32_t *)(sp + 0x48));
	*(uint32_t *)(sp + 0x24 + 0x4) = 0x30;
	a3 = sp + 0x1C;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x24);
	*(uint32_t *)(sp + 8) = 1;
	a2 = /*data_12340016*/;
	a4 = 1;
	FLAGS = a4;
	a1 = r5(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81001066;
	goto loc_81001068;

loc_81001066:
//VADDR: 0x81001066 OFF: 0x1066
	a1 = *(uint32_t *)(sp + 0x10);

loc_81001068:
//VADDR: 0x81001068 OFF: 0x1068
	FLAGS = (*(uint32_t *)(v3)) - (*(uint32_t *)(sp + 0x30));
	if (Z == 0) // !=
		goto loc_81001074;
	sp = sp + 0x38;
	return a1; // pop {r4, r5, r6, pc}

loc_81001074:
//VADDR: 0x81001074 OFF: 0x1074
	a1 = SceLibKernel_37691BF8();
	a1 = a1;
	FLAGS = a1;
}

//VADDR: 0x8100107c OFF: 0x107c
int sub_8100107C(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, r6, lr}
	v2 = /*data_80101242*/;
	a1 = SceSysmem_B9D5EBDE(/*s_text_81001AB0*/ "SceLocationClient", /*data_0C20D060*/, (a1 + 0) + 0, 0);
	*(uint32_t *)((/*data_81002010*/ 0x00000000)) = a1;
	FLAGS = a1 - 0;
	if (N != V) // signed <
		goto loc_81001104;
	a1 = SceSysmem_B8EF5818(a1, /*data_81002014*/ 0x00000000);
	FLAGS = (a1 + 0) - 0;
	if (N != V) // signed <
		goto loc_810010F6;
	a1 = SceLibKernel_3B9E301A(*(uint32_t *)((/*data_81002014*/ 0x00000000)), (a1 + 0) + 0);
	*(uint32_t *)((/*data_81002014*/ 0x00000000)) = a1;
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810010DA;
	v2 = 0;
	FLAGS = v2;
	goto loc_81001104;

loc_810010DA:
//VADDR: 0x810010da OFF: 0x10da
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81002010*/ 0x00000000)));
	*(uint32_t *)((/*data_81002010*/ 0x00000000)) = 0;
	v2 = /*data_80101242*/;
	goto loc_81001104;

loc_810010F6:
//VADDR: 0x810010f6 OFF: 0x10f6
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81002010*/ 0x00000000)));

loc_81001104:
//VADDR: 0x81001104 OFF: 0x1104
	a1 = v2 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81001108 OFF: 0x1108
int sub_81001108(int arg1, int arg2, int arg3)
{
	// push {r4, lr}
	if ((*(uint32_t *)((/*data_81002014*/ 0x00000000))) == 0)
		goto loc_81001126;
	a1 = SceLibKernel_AE1A21EC(*(uint32_t *)((/*data_81002014*/ 0x00000000)));
	*(uint32_t *)((/*data_81002014*/ 0x00000000)) = 0;

loc_81001126:
//VADDR: 0x81001126 OFF: 0x1126
	FLAGS = (*(uint32_t *)((/*data_81002010*/ 0x00000000))) - 0;
	if (N != V) // signed <
		goto loc_81001144;
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81002010*/ 0x00000000)));
	*(uint32_t *)((/*data_81002010*/ 0x00000000)) = 0;

loc_81001144:
//VADDR: 0x81001144 OFF: 0x1144
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001146 OFF: 0x1146
int sub_81001146()
{
	// push {r4, lr}
	a1 = SceLibKernel_86EF7680((*(uint32_t *)((/*data_81002014*/ 0x00000000))) + 0, a1 + 0, *(uint32_t *)((/*data_81002014*/ 0x00000000)));
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100115c OFF: 0x115c
int sub_8100115C()
{
	// push {r4, lr}
	a1 = SceLibKernel_9C56B4D1((*(uint32_t *)((/*data_81002014*/ 0x00000000))) + 0, a1 + 0, *(uint32_t *)((/*data_81002014*/ 0x00000000)));
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001174 OFF: 0x1174
int sub_81001174()
{
	// push {r4, r5, lr}
	sp = sp - 0xC;
	*(uint32_t *)(sp) = 0;
	a1 = SceLibKernel_DA6EC8EF(/*data_81002840*/ 0x00000000, /*s_text_81001AC4*/ "SceTinyLibc", 0x2002, 0, *(sp+0));
	*(uint32_t *)((/*data_81002038*/ 0x00000000)) = (/*data_81002038*/ 0x00000000);
	FLAGS = (/*data_81002018*/ 0x81000E63) - (/*data_81002018*/ 0x81000E63);
	v1 = /*data_8100201C*/ 0x00000000;
	if (C == 1) // unsigned >=
		goto loc_810011D0;

loc_810011BC:
//VADDR: 0x810011bc OFF: 0x11bc
	if ((*(uint32_t *)(v2)) == 0)
		goto loc_810011C2;
	a1 = r0(...);

loc_810011C2:
//VADDR: 0x810011c2 OFF: 0x11c2
	FLAGS = (v2 + 4) - (/*data_81002018*/ 0x81000E63);
	if (C == 0) // unsigned <
		goto loc_810011BC;

loc_810011D0:
//VADDR: 0x810011d0 OFF: 0x11d0
	FLAGS = (/*data_81002018*/ 0x81000E63) - v1;
	if (C == 1) // unsigned >=
		goto loc_810011E8;

loc_810011DC:
//VADDR: 0x810011dc OFF: 0x11dc
	if ((*(uint32_t *)(v2)) == 0)
		goto loc_810011E2;
	a1 = r0(...);

loc_810011E2:
//VADDR: 0x810011e2 OFF: 0x11e2
	FLAGS = (v2 + 4) - v1;
	if (C == 0) // unsigned <
		goto loc_810011DC;

loc_810011E8:
//VADDR: 0x810011e8 OFF: 0x11e8
	sp = sp + 0xC;
	return a1; // pop {r4, r5, pc}
}

//VADDR: 0x810011ec OFF: 0x11ec
int sub_810011EC()
{
	// push {r4, lr}
	if (/*data_00000000*/ == 0)
		goto loc_81001206;
	a4 = 0;
	FLAGS = a4;
	a1 = ip(...);
	goto loc_81001208;

loc_81001206:
//VADDR: 0x81001206 OFF: 0x1206
	a1 = 9;
	FLAGS = a1;

loc_81001208:
//VADDR: 0x81001208 OFF: 0x1208
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100120a OFF: 0x120a
int sub_8100120A()
{
	// push {r4, lr}
	if (/*data_00000000*/ == 0)
		goto loc_81001224;
	a4 = 1;
	FLAGS = a4;
	a1 = ip(...);
	goto loc_81001226;

loc_81001224:
//VADDR: 0x81001224 OFF: 0x1224
	a1 = 9;
	FLAGS = a1;

loc_81001226:
//VADDR: 0x81001226 OFF: 0x1226
	return a1; // pop {r4, pc}
}

//VADDR: 0x81001228 OFF: 0x1228
int sub_81001228()
{
	return a1; // pop {r4, pc}
}

int SceLibKernel_244E76D2(int arg1);
int SceLibKernel_2E3B02A1(int arg1, int arg2, int arg3);
int SceLibKernel_37691BF8();
int SceLibKernel_46E7BE7B(int arg1, int arg2, int arg3);
int SceLibKernel_632980D7(int arg1, int arg2, int arg3);
int SceLibKernel_91FA6614(int arg1, int arg2);
int SceLibKernel_9F793F84();
int SceLibKernel_C458D60A();
int SceLibKernel_C5C11EE7(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);
int SceLibKernel_DA6EC8EF(int arg1, int arg2, int arg3, int arg4, int arg5);
int SceLibKernel_DDB395A9(int arg1, int arg2, int arg3);
int SceThreadmgr_1BBDE3D9(int arg1);
int SceThreadmgrCoredumpTime_0C8A38E1(int arg1);
int SceIpmi_4E255C31();
int SceIpmi_B282B430();
int SceLibLocation_0C0BB66A();
int SceLibLocation_2311B24A();
int SceLibLocation_7F74AA94();
int SceLibLocation_822BBF06(int arg1, int arg2);
int SceLibLocation_991AF2D0(int arg1, int arg2, int arg3);
int SceLibLocation_EFC9BD25();
int SceLibKernel_3B9E301A(int arg1, int arg2);
int SceLibKernel_86EF7680(int arg1, int arg2, int arg3);
int SceLibKernel_9C56B4D1(int arg1, int arg2, int arg3);
int SceLibKernel_AE1A21EC(int arg1);
int SceSysmem_A91E15EE(int arg1);
int SceSysmem_B8EF5818(int arg1, int arg2);
int SceSysmem_B9D5EBDE(int arg1, int arg2, int arg3, int arg4);
