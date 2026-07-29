//VADDR: 0x81000000 OFF: 0
// Exported
int SceLibLocationFactory_9E020010()
{
	// push {r4, r5, r6, r7, lr}
	sp = sp - 0xC;
	*(uint32_t *)(sp + 4) = (*(uint32_t *)((/*text_810009C0*/ 0x00000440)));
	*(uint32_t *)(sp) = 0;
	v1 = a3 + 0;
	FLAGS = v1;
	v2 = a2 + 0;
	FLAGS = v2;
	FLAGS = ((a1 + 0) - 4) - 0xF;
	if (C == 1 && Z == 0) // unsigned >
		goto loc_81000040;
	asm("tbb [pc, r0]\n");
	a1 = a2 >> 0x20;
	FLAGS = a1;
	v3 = a2 >> 0x18;
	FLAGS = v3;
	v3 = a2 >> 0x18;
	FLAGS = v3;
	v3 = a2 >> 0x18;
	FLAGS = v3;
	v3 = a2 >> 0x20;
	FLAGS = v3;
	a1 = a2 >> 0x18;
	FLAGS = a1;
	a1 = a2 >> 0x18;
	FLAGS = a1;
	v3 = a2 >> 0x20;
	FLAGS = v3;
	a1 = sub_81000198(sp + 0, a2, a3, *(uint32_t *)((/*text_810009C0*/ 0x00000440)), *(sp+0), *(sp+4));
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_81000064;
	goto loc_8100004A;

loc_81000040:
//VADDR: 0x81000040 OFF: 0x40
	a1 = /*data_8010124B*/;
	goto loc_81000064;

loc_8100004A:
//VADDR: 0x8100004a OFF: 0x4a
	a1 = sub_81000354(*(uint32_t *)(sp), v3 + 0, v2 + 0, v1 + 0);
	a1 = sub_810002AA(sp + 0);
	FLAGS = (a1 + 0) - 0;
	if (Z == 1) // ==
		goto loc_81000064;
	a1 = (a1 + 0) + 0;
	FLAGS = a1;

loc_81000064:
//VADDR: 0x81000064 OFF: 0x64
	FLAGS = (*(uint32_t *)(v4)) - (*(uint32_t *)(sp + 4));
	if (Z == 0) // !=
		goto loc_81000070;
	sp = sp + 0xC;
	return a1; // pop {r4, r5, r6, r7, pc}

loc_81000070:
//VADDR: 0x81000070 OFF: 0x70
	a1 = SceLibKernel_37691BF8();
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100007a OFF: 0x7a
int sub_8100007A(int arg1)
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0xC8;
	*(uint32_t *)(sp + 0xC0) = (*(uint32_t *)((/*text_810009C0*/ 0x00000440)));
	*(uint32_t *)(sp + 0x24) = 1;
	v4 = a1 + 0;
	FLAGS = v4;
	*(uint32_t *)(sp + 0x18) = 1;
	*(uint32_t *)(sp + 0x18 + 0x4) = 0xF00;
	*(uint32_t *)(sp + 0x20) = 0xF00;
	*(uint32_t *)(sp + 0x28) = 0xF00;
	*(uint32_t *)(sp + 0x28 + 0x4) = 0xF00;
	a3 = 4;
	FLAGS = a3;
	*(uint32_t *)(sp + 0x30) = 1;
	*(uint32_t *)(sp + 0x30 + 0x4) = 0;
	a4 = sp + 0x38;
	FLAGS = a4;

loc_810000B0:
//VADDR: 0x810000b0 OFF: 0xb0
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
		goto loc_810000B0;
	*(uint32_t *)(sp + 0xB8) = v1;
	*(uint32_t *)(sp + 0xB8 + 0x4) = -1;
	*(uint8_t *)(sp + 4) = v1;
	a1 = SceLibKernel_C458D60A((sp + 4) + 0, /*s_text_81000B20*/ "SceLocationLoc", 0x10, a4 + 0x20);
	*(uint32_t *)(sp + 0xB8) = 2;
	a1 = SceIpmi_4E255C31((sp + 4) + 0);
	a1 = r1(...);
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_8100010E;
	a1 = /*data_80101242*/;
	goto loc_81000182;

loc_8100010E:
//VADDR: 0x8100010e OFF: 0x10e
	a1 = SceIpmi_B282B430(v4 + 0, sp + 4, v4 + 0, v2 + 0);
	if (Z == 1) // ==
		goto loc_81000130;
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = (a1 + 0) + 0;
	FLAGS = a1;
	goto loc_81000182;

loc_81000130:
//VADDR: 0x81000130 OFF: 0x130
	a4 = sp + 0;
	a3 = 0;
	FLAGS = a3;
	a2 = 0;
	FLAGS = a2;
	a1 = r6(...);
	v4 = *(uint32_t *)(v4);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81000180;
	FLAGS = a1 - /*data_80020581*/;
	v1 = /*data_801012FF*/;
	if (Z == 0 && N == V) // signed >
		goto loc_81000166;
	FLAGS = a1 - /*data_80020580*/;
	if (Z == 1 || N != V) // signed <=
		goto loc_81000166;
	v1 = a1 + 0;
	FLAGS = v1;

loc_81000166:
//VADDR: 0x81000166 OFF: 0x166
	a1 = v4 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = v2 + 0;
	FLAGS = a1;
	a1 = r1(...);
	a1 = v1 + 0;
	FLAGS = a1;
	goto loc_81000182;

loc_81000180:
//VADDR: 0x81000180 OFF: 0x180
	a1 = v1 + 0;
	FLAGS = a1;

loc_81000182:
//VADDR: 0x81000182 OFF: 0x182
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0xC0));
	if (Z == 0) // !=
		goto loc_81000192;
	sp = sp + 0xC8;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_81000192:
//VADDR: 0x81000192 OFF: 0x192
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000198 OFF: 0x198
int sub_81000198(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
{
	// push {r4, r5, r6, r7, r8, lr}
	v1 = a1 + 0;
	FLAGS = v1;
	if ((*(uint8_t *)(/*data_81001000*/)) == 0)
		goto loc_810001DE;
	a1 = 0;
	FLAGS = a1;
	if ((*(uint32_t *)((/*data_81001020*/ 0x00000000))) != 0)
		goto loc_810001CE;
	a1 = sub_81000418(0x1000, *(uint32_t *)((/*data_81001020*/ 0x00000000)));
	a2 = *(uint32_t *)((/*data_81001020*/ 0x00000000));
	FLAGS = a1 - 0;
	if (Z == 0) // !=
		goto loc_810001DA;

loc_810001CE:
//VADDR: 0x810001ce OFF: 0x1ce
	*(uint32_t *)((/*data_81001020*/ 0x00000000)) = (a2 + 1);

loc_810001DA:
//VADDR: 0x810001da OFF: 0x1da
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 0) // !=
		goto loc_810002A2;

loc_810001DE:
//VADDR: 0x810001de OFF: 0x1de
	a1 = 4;
	FLAGS = a1;
	a1 = r1(...);
	if (Z == 1) // ==
		goto loc_81000200;
	*(uint32_t *)((a1 + 0)) = 0;
	a1 = sub_8100007A((a1 + 0) + 0);
	v2 = a1 + 0;
	FLAGS = v2;
	if (Z == 1) // ==
		goto loc_81000242;
	goto loc_81000246;

loc_81000200:
//VADDR: 0x81000200 OFF: 0x200
	if ((*(uint8_t *)(/*data_81001000*/)) == 0)
		goto loc_81000238;
	FLAGS = (*(uint32_t *)((/*data_81001020*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_8100022A;
	a1 = sub_810004A4(*(uint32_t *)((/*data_81001020*/ 0x00000000)), a2);
	a1 = *(uint32_t *)((/*data_81001020*/ 0x00000000));
	goto loc_8100022C;

loc_8100022A:
//VADDR: 0x8100022a OFF: 0x22a
	if (a1 == 0)
		goto loc_81000238;

loc_8100022C:
//VADDR: 0x8100022c OFF: 0x22c
	*(uint32_t *)((/*data_81001020*/ 0x00000000)) = (a1 - 1);

loc_81000238:
//VADDR: 0x81000238 OFF: 0x238
	a1 = /*data_80101242*/;
	goto loc_810002A4;

loc_81000242:
//VADDR: 0x81000242 OFF: 0x242
	*(uint32_t *)(v1) = v3;
	goto loc_810002A2;

loc_81000246:
//VADDR: 0x81000246 OFF: 0x246
	if ((*(uint32_t *)(v3)) == 0)
		goto loc_81000256;
	a1 = r1(...);

loc_81000256:
//VADDR: 0x81000256 OFF: 0x256
	*(uint32_t *)(v3) = 0;
	a1 = v3 + 0;
	FLAGS = a1;
	a1 = r1(...);
	*(uint32_t *)(v1) = 0;
	if ((*(uint8_t *)(/*data_81001000*/)) == 0)
		goto loc_810002A2;
	FLAGS = (*(uint32_t *)((/*data_81001020*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_81000294;
	a1 = sub_810004A4(/*data_81001020*/ 0x00000000, *(uint32_t *)((/*data_81001020*/ 0x00000000)));
	a2 = *(uint32_t *)((/*data_81001020*/ 0x00000000));
	goto loc_81000296;

loc_81000294:
//VADDR: 0x81000294 OFF: 0x294
	if (a2 == 0)
		goto loc_810002A2;

loc_81000296:
//VADDR: 0x81000296 OFF: 0x296
	*(uint32_t *)((/*data_81001020*/ 0x00000000)) = (a2 - 1);

loc_810002A2:
//VADDR: 0x810002a2 OFF: 0x2a2
	a1 = v2 + 0;
	FLAGS = a1;

loc_810002A4:
//VADDR: 0x810002a4 OFF: 0x2a4
	return a1; // pop {r4, r5, r6, r7, r8, pc}
}

//VADDR: 0x810002a8 OFF: 0x2a8
int sub_810002A8()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x810002aa OFF: 0x2aa
int sub_810002AA(int arg1)
{
	// push {r4, r5, r6, lr}
	v1 = /*data_801012FF*/;
	FLAGS = (*(uint32_t *)((a1 + 0))) - 0;
	if (Z == 1) // ==
		goto loc_81000350;
	if ((*(uint32_t *)((*(uint32_t *)((a1 + 0))))) != 0)
		goto loc_810002C2;
	goto loc_810002F0;

loc_810002C2:
//VADDR: 0x810002c2 OFF: 0x2c2
	a1 = r1(...);
	FLAGS = a1 - /*data_8002050A*/;
	if (Z == 1) // ==
		goto loc_810002EE;
	a1 = r1(...);
	a1 = *(uint32_t *)(v3);
	a1 = r1(...);
	*(uint32_t *)(v3) = 0;

loc_810002EE:
//VADDR: 0x810002ee OFF: 0x2ee
	v3 = *(uint32_t *)(v2);

loc_810002F0:
//VADDR: 0x810002f0 OFF: 0x2f0
	if (v3 == 0)
		goto loc_81000314;
	if ((*(uint32_t *)(v3)) == 0)
		goto loc_81000302;
	a1 = r1(...);

loc_81000302:
//VADDR: 0x81000302 OFF: 0x302
	*(uint32_t *)(v3) = 0;
	a1 = v3 + 0;
	FLAGS = a1;
	a1 = r1(...);

loc_81000314:
//VADDR: 0x81000314 OFF: 0x314
	*(uint32_t *)(v2) = 0;
	if ((*(uint8_t *)(/*data_81001000*/)) == 0)
		goto loc_81000350;
	FLAGS = (*(uint32_t *)((/*data_81001020*/ 0x00000000))) - 1;
	if (Z == 0) // !=
		goto loc_81000342;
	a1 = sub_810004A4(/*data_81001020*/ 0x00000000, *(uint32_t *)((/*data_81001020*/ 0x00000000)));
	a2 = *(uint32_t *)((/*data_81001020*/ 0x00000000));
	goto loc_81000344;

loc_81000342:
//VADDR: 0x81000342 OFF: 0x342
	if (a2 == 0)
		goto loc_81000350;

loc_81000344:
//VADDR: 0x81000344 OFF: 0x344
	*(uint32_t *)((/*data_81001020*/ 0x00000000)) = (a2 - 1);

loc_81000350:
//VADDR: 0x81000350 OFF: 0x350
	a1 = v1 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81000354 OFF: 0x354
int sub_81000354(int arg1, int arg2, int arg3, int arg4)
{
	// push {r4, r5, lr}
	sp = sp - 0x144;
	*(uint32_t *)(sp + 0x140) = (*(uint32_t *)((/*text_810009C0*/ 0x00000440)));
	FLAGS = a4 - 0;
	a1 = *(uint32_t *)(a1);
	if (Z == 0) // !=
		goto loc_8100036C;
	a4 = sp + 0;

loc_8100036C:
//VADDR: 0x8100036c OFF: 0x36c
	a1 = sub_8100039C();
	FLAGS = (*(uint32_t *)(v2)) - (*(uint32_t *)(sp + 0x140));
	if (Z == 0) // !=
		goto loc_8100037C;
	sp = sp + 0x144;
	return a1; // pop {r4, r5, pc}

loc_8100037C:
//VADDR: 0x8100037c OFF: 0x37c
	a1 = SceLibKernel_37691BF8();
	*(uint32_t *)((/*data_81001018*/ 0x00000000)) = (*(uint32_t *)((/*text_81000B18*/ 0x08F241B5)));
	*(uint32_t *)((/*data_81001018*/ 0x00000000) + 4) = (*(uint32_t *)((/*text_81000B18*/ 0x08F241B5) + 4));
	return a1;
}

//VADDR: 0x8100039c OFF: 0x39c
int sub_8100039C()
{
	// push {r4, r5, r6, r7, r8, lr}
	sp = sp - 0x38;
	*(uint32_t *)(sp + 0x34) = (*(uint32_t *)((/*text_810009C0*/ 0x00000440)));
	*(uint32_t *)(sp + 0x10) = 0;
	*(uint32_t *)(sp + 0x10 + 0x4) = a2;
	*(uint32_t *)(sp + 0x24) = (sp + 0x14);
	*(uint32_t *)(sp + 0x28) = 4;
	*(uint32_t *)(sp + 0x28 + 0x4) = a3;
	*(uint32_t *)(sp + 0x30) = 0x140;
	*(uint32_t *)(sp + 0x18) = a4;
	*(uint32_t *)(sp + 0x18 + 0x4) = 0x140;
	*(uint32_t *)(sp) = (sp + 0x10);
	*(uint32_t *)(sp + 0x4) = (sp + 0x18);
	a3 = sp + 0x24;
	*(uint32_t *)(sp + 8) = 1;
	a2 = /*data_12340013*/;
	a4 = 2;
	FLAGS = a4;
	a1 = r4(...);
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_810003EC;
	goto loc_810003EE;

loc_810003EC:
//VADDR: 0x810003ec OFF: 0x3ec
	a1 = *(uint32_t *)(sp + 0x10);

loc_810003EE:
//VADDR: 0x810003ee OFF: 0x3ee
	FLAGS = (*(uint32_t *)(v5)) - (*(uint32_t *)(sp + 0x34));
	if (Z == 0) // !=
		goto loc_810003FE;
	sp = sp + 0x38;
	return a1; // pop {r4, r5, r6, r7, r8, pc}

loc_810003FE:
//VADDR: 0x810003fe OFF: 0x3fe
	a1 = SceLibKernel_37691BF8();
}

//VADDR: 0x81000404 OFF: 0x404
// Exported
int module_start()
{
	// push {r4, lr}
	a1 = sub_810005EC();
	a1 = 0;
	FLAGS = a1;
	return a1; // pop {r4, pc}
}

//VADDR: 0x8100040e OFF: 0x40e
// Exported
int module_stop()
{
	// push {r4, lr}
	a1 = sub_81000664();
	a1 = 0;
	FLAGS = a1;
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000418 OFF: 0x418
int sub_81000418(int arg1, int arg2)
{
	// push {r4, r5, r6, lr}
	v2 = /*data_80101242*/;
	a1 = SceSysmem_B9D5EBDE(/*s_text_81000B30*/ "SceLocationDebugClient", /*data_0C20D060*/, (a1 + 0) + 0, 0);
	*(uint32_t *)((/*data_81001004*/ 0x00000000)) = a1;
	FLAGS = a1 - 0;
	if (N != V) // signed <
		goto loc_810004A0;
	a1 = SceSysmem_B8EF5818(a1, /*data_81001008*/ 0x00000000);
	FLAGS = (a1 + 0) - 0;
	if (N != V) // signed <
		goto loc_81000492;
	a1 = SceLibKernel_3B9E301A(*(uint32_t *)((/*data_81001008*/ 0x00000000)), (a1 + 0) + 0);
	*(uint32_t *)((/*data_81001008*/ 0x00000000)) = a1;
	FLAGS = a1 - 0;
	if (Z == 1) // ==
		goto loc_81000476;
	v2 = 0;
	FLAGS = v2;
	goto loc_810004A0;

loc_81000476:
//VADDR: 0x81000476 OFF: 0x476
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81001004*/ 0x00000000)));
	*(uint32_t *)((/*data_81001004*/ 0x00000000)) = 0;
	v2 = /*data_80101242*/;
	goto loc_810004A0;

loc_81000492:
//VADDR: 0x81000492 OFF: 0x492
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81001004*/ 0x00000000)));

loc_810004A0:
//VADDR: 0x810004a0 OFF: 0x4a0
	a1 = v2 + 0;
	FLAGS = a1;
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x810004a4 OFF: 0x4a4
int sub_810004A4(int arg1, int arg2)
{
	// push {r4, lr}
	if ((*(uint32_t *)((/*data_81001008*/ 0x00000000))) == 0)
		goto loc_810004C2;
	a1 = SceLibKernel_AE1A21EC(*(uint32_t *)((/*data_81001008*/ 0x00000000)));
	*(uint32_t *)((/*data_81001008*/ 0x00000000)) = 0;

loc_810004C2:
//VADDR: 0x810004c2 OFF: 0x4c2
	FLAGS = (*(uint32_t *)((/*data_81001004*/ 0x00000000))) - 0;
	if (N != V) // signed <
		goto loc_810004E0;
	a1 = SceSysmem_A91E15EE(*(uint32_t *)((/*data_81001004*/ 0x00000000)));
	*(uint32_t *)((/*data_81001004*/ 0x00000000)) = 0;

loc_810004E0:
//VADDR: 0x810004e0 OFF: 0x4e0
	return a1; // pop {r4, pc}
}

//VADDR: 0x810004e2 OFF: 0x4e2
int sub_810004E2()
{
	// push {r4, lr}
	a1 = SceLibKernel_86EF7680((*(uint32_t *)((/*data_81001008*/ 0x00000000))) + 0, a1 + 0, *(uint32_t *)((/*data_81001008*/ 0x00000000)));
	return a1; // pop {r4, pc}
}

//VADDR: 0x810004f8 OFF: 0x4f8
int sub_810004F8()
{
	// push {r4, lr}
	a1 = SceLibKernel_9C56B4D1((*(uint32_t *)((/*data_81001008*/ 0x00000000))) + 0, a1 + 0, *(uint32_t *)((/*data_81001008*/ 0x00000000)));
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000510 OFF: 0x510
int sub_81000510()
{
	return a1; // pop {r4, r5, r6, pc}
}

//VADDR: 0x81000512 OFF: 0x512
int sub_81000512(int arg1)
{
	// push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sp = sp - 4;
	FLAGS = a1 - (*(uint32_t *)((/*data_81001028*/ 0x00000000)));
	sl = /*data_81001830*/ 0x00000000;
	if (Z == 1) // ==
		goto loc_81000532;
	asm("bkpt #1\n");

loc_81000532:
//VADDR: 0x81000532 OFF: 0x532
	a1 = SceLibKernel_46E7BE7B(sl, 1, 0);
	FLAGS = fp - 0;
	if (Z == 1) // ==
		goto loc_810005DE;
	sb = ((*(uint32_t *)((/*data_8100100C*/ 0x00000000))) + 0);
	FLAGS = sb;
	if (Z == 1) // ==
		goto loc_810005DE;
	v5 = 0;
	FLAGS = v5;

loc_81000558:
//VADDR: 0x81000558 OFF: 0x558
	FLAGS = (*(uint32_t *)(v1 + 0xC)) - (*(uint32_t *)(fp));
	if (Z == 0) // !=
		goto loc_810005CE;
	FLAGS = v1 - v4;
	if (Z == 0) // !=
		goto loc_81000574;
	*(uint32_t *)((/*data_8100100C*/ 0x00000000)) = (*(uint32_t *)(v1));
	sb = (*(uint32_t *)(v1));

loc_81000574:
//VADDR: 0x81000574 OFF: 0x574
	FLAGS = v5 - 0;
	if (Z == 1) // ==
		goto loc_8100057E;
	*(uint32_t *)(v5) = v2;

loc_8100057E:
//VADDR: 0x8100057e OFF: 0x57e
	a1 = SceLibKernel_91FA6614(sl, 1);
	v4 = *(uint32_t *)(v1 + 4);
	v3 = *(uint32_t *)(v1 + 8);
	ip = /*data_81001030*/ 0x00000000;
	lr = 0;
	FLAGS = lr;

loc_81000596:
//VADDR: 0x81000596 OFF: 0x596
	FLAGS = ip - v1;
	if (Z == 0) // !=
		goto loc_810005A0;
	*(uint32_t *)(v1 + 4) = 0;
	goto loc_810005B4;

loc_810005A0:
//VADDR: 0x810005a0 OFF: 0x5a0
	ip = ip + 0x10;
	FLAGS = (lr + 1) - 0x80;
	if (N != V) // signed <
		goto loc_81000596;
	a1 = sub_81000682(v1 + 0);

loc_810005B4:
//VADDR: 0x810005b4 OFF: 0x5b4
	a1 = v3 + 0;
	FLAGS = a1;
	a1 = r7(...);
	a1 = SceLibKernel_46E7BE7B(sl, 1, 0);
	v4 = *(uint32_t *)((/*data_8100100C*/ 0x00000000));
	goto loc_810005D0;

loc_810005CE:
//VADDR: 0x810005ce OFF: 0x5ce
	v5 = v1;

loc_810005D0:
//VADDR: 0x810005d0 OFF: 0x5d0
	v1 = v2 + 0;
	FLAGS = v1;
	FLAGS = sb - v4;
	if (Z == 1) // ==
		goto loc_810005DA;
	sb = (v4 + 0);

loc_810005DA:
//VADDR: 0x810005da OFF: 0x5da
	FLAGS = v1 - 0;
	if (Z == 0) // !=
		goto loc_81000558;

loc_810005DE:
//VADDR: 0x810005de OFF: 0x5de
	a1 = SceLibKernel_91FA6614(sl, 1);
	sp = sp + 4;
	return a1; // pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
}

//VADDR: 0x810005ec OFF: 0x5ec
int sub_810005EC()
{
	// push {r4, r5, lr}
	sp = sp - 0xC;
	*(uint32_t *)(sp) = 0;
	a1 = SceLibKernel_DA6EC8EF(/*data_81001830*/ 0x00000000, /*s_text_81000B4C*/ "SceTinyLibc", 0x2002, 0, *(sp+0));
	*(uint32_t *)((/*data_81001028*/ 0x00000000)) = (/*data_81001028*/ 0x00000000);
	FLAGS = (/*data_81001010*/ 0x81000383) - (/*data_81001010*/ 0x81000383);
	v1 = /*data_81001014*/ 0x00000000;
	if (C == 1) // unsigned >=
		goto loc_81000648;

loc_81000634:
//VADDR: 0x81000634 OFF: 0x634
	if ((*(uint32_t *)(v2)) == 0)
		goto loc_8100063A;
	a1 = r0(...);

loc_8100063A:
//VADDR: 0x8100063a OFF: 0x63a
	FLAGS = (v2 + 4) - (/*data_81001010*/ 0x81000383);
	if (C == 0) // unsigned <
		goto loc_81000634;

loc_81000648:
//VADDR: 0x81000648 OFF: 0x648
	FLAGS = (/*data_81001010*/ 0x81000383) - v1;
	if (C == 1) // unsigned >=
		goto loc_81000660;

loc_81000654:
//VADDR: 0x81000654 OFF: 0x654
	if ((*(uint32_t *)(v2)) == 0)
		goto loc_8100065A;
	a1 = r0(...);

loc_8100065A:
//VADDR: 0x8100065a OFF: 0x65a
	FLAGS = (v2 + 4) - v1;
	if (C == 0) // unsigned <
		goto loc_81000654;

loc_81000660:
//VADDR: 0x81000660 OFF: 0x660
	sp = sp + 0xC;
	return a1; // pop {r4, r5, pc}
}

//VADDR: 0x81000664 OFF: 0x664
int sub_81000664()
{
	// push {r4, lr}
	a1 = sub_81000512(/*data_81001028*/ 0x00000000);
	a1 = SceLibKernel_244E76D2(/*data_81001830*/ 0x00000000);
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000680 OFF: 0x680
int sub_81000680()
{
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000682 OFF: 0x682
int sub_81000682(int arg1)
{
	// push {r4, lr}
	a1 = SceLibKernel_9C56B4D1((*(uint32_t *)((/*data_81001850*/ 0x00000000))) + 0, a1 + 0, *(uint32_t *)((/*data_81001850*/ 0x00000000)));
	return a1; // pop {r4, pc}
}

//VADDR: 0x81000698 OFF: 0x698
int sub_81000698()
{
	// push {r4, lr}
	if (/*data_00000000*/ == 0)
		goto loc_810006B2;
	a4 = 0;
	FLAGS = a4;
	a1 = ip(...);
	goto loc_810006B4;

loc_810006B2:
//VADDR: 0x810006b2 OFF: 0x6b2
	a1 = 9;
	FLAGS = a1;

loc_810006B4:
//VADDR: 0x810006b4 OFF: 0x6b4
	return a1; // pop {r4, pc}
}

int SceLibKernel_37691BF8();
int SceLibKernel_3B9E301A(int arg1, int arg2);
int SceLibKernel_86EF7680(int arg1, int arg2, int arg3);
int SceLibKernel_9C56B4D1(int arg1, int arg2, int arg3);
int SceLibKernel_AE1A21EC(int arg1);
int SceLibKernel_C458D60A(int arg1, int arg2, int arg3, int arg4);
int SceSysmem_A91E15EE(int arg1);
int SceSysmem_B8EF5818(int arg1, int arg2);
int SceSysmem_B9D5EBDE(int arg1, int arg2, int arg3, int arg4);
int SceIpmi_4E255C31(int arg1);
int SceIpmi_B282B430(int arg1, int arg2, int arg3, int arg4);
int SceLibKernel_244E76D2(int arg1);
int SceLibKernel_46E7BE7B(int arg1, int arg2, int arg3);
int SceLibKernel_91FA6614(int arg1, int arg2);
int SceLibKernel_DA6EC8EF(int arg1, int arg2, int arg3, int arg4, int arg5);
