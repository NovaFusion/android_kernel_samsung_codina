/*
 * Copyright (C) STMicroelectronics 2009
 * Copyright (C) ST-Ericsson SA 2010-2011
 *
 * License Terms: GNU General Public License v2
 * Author: Sundar Iyer for ST-Ericsson
 *
 */

#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/context.h>

/*
 * ST-Interconnect context
 */

/* priority, bw limiter register offsets */
#define NODE_HIBW1_ESRAM_IN_0_PRIORITY		0x00
#define NODE_HIBW1_ESRAM_IN_1_PRIORITY		0x04
#define NODE_HIBW1_ESRAM_IN_2_PRIORITY		0x08
#define NODE_HIBW1_ESRAM_IN_3_PRIORITY		0x0C
#define NODE_HIBW1_ESRAM_IN_0_ARB_1_LIMIT	0x30
#define NODE_HIBW1_ESRAM_IN_0_ARB_2_LIMIT	0x34
#define NODE_HIBW1_ESRAM_IN_0_ARB_3_LIMIT	0x38
#define NODE_HIBW1_ESRAM_IN_1_ARB_1_LIMIT	0x3C
#define NODE_HIBW1_ESRAM_IN_1_ARB_2_LIMIT	0x40
#define NODE_HIBW1_ESRAM_IN_1_ARB_3_LIMIT	0x44
#define NODE_HIBW1_ESRAM_IN_2_ARB_1_LIMIT	0x48
#define NODE_HIBW1_ESRAM_IN_2_ARB_2_LIMIT	0x4C
#define NODE_HIBW1_ESRAM_IN_2_ARB_3_LIMIT	0x50
#define NODE_HIBW1_ESRAM_IN_3_ARB_1_LIMIT	0x54
#define NODE_HIBW1_ESRAM_IN_3_ARB_2_LIMIT	0x58
#define NODE_HIBW1_ESRAM_IN_3_ARB_3_LIMIT	0x5C
#define NODE_HIBW1_DDR0_IN_0_PRIORITY		0x400
#define NODE_HIBW1_DDR0_IN_1_PRIORITY		0x404
#define NODE_HIBW1_DDR0_IN_2_PRIORITY		0x408
#define NODE_HIBW1_DDR0_IN_3_PRIORITY		0x40C
#define NODE_HIBW1_DDR0_IN_0_LIMIT			0x430
#define NODE_HIBW1_DDR0_IN_1_LIMIT			0x434
#define NODE_HIBW1_DDR0_IN_2_LIMIT			0x438
#define NODE_HIBW1_DDR0_IN_3_LIMIT			0x43C
#define NODE_HIBW1_DDR0_OUT_0_PRIORITY		0x440
#define NODE_HIBW2_ESRAM_IN_0_PRIORITY		0x800
#define NODE_HIBW2_ESRAM_IN_1_PRIORITY		0x804
#define NODE_HIBW2_ESRAM_IN_0_ARB_1_LIMIT	0x818
#define NODE_HIBW2_ESRAM_IN_0_ARB_2_LIMIT	0x81C
#define NODE_HIBW2_ESRAM_IN_0_ARB_3_LIMIT	0x820
#define NODE_HIBW2_ESRAM_IN_1_ARB_1_LIMIT	0x824
#define NODE_HIBW2_ESRAM_IN_1_ARB_2_LIMIT	0x828
#define NODE_HIBW2_ESRAM_IN_1_ARB_3_LIMIT	0x82C

#define NODE_HIBW2_DDR0_IN_0_PRIORITY		0xC00
#define NODE_HIBW2_DDR0_IN_1_PRIORITY		0xC04

#define NODE_HIBW2_DDR0_IN_0_LIMIT			0xC18
#define NODE_HIBW2_DDR0_IN_1_LIMIT			0xC1C

#define NODE_HIBW2_DDR0_OUT_0_PRIORITY		0xC20

/*
 * Note the following addresses are presented in
 * db8500 design spec v3.1 and v3.3, table 10.
 * But their addresses are not the same as in the
 * description. The addresses in the description
 * of each registers are correct.
 * NODE_HIBW2_DDR_IN_3_LIMIT is only present in v1.
 *
 * Faulty registers addresses in table 10:
 * NODE_HIBW2_DDR_IN_2_LIMIT 0xC38
 * NODE_HIBW2_DDR_IN_3_LIMIT 0xC3C
 * NODE_HIBW2_DDR_OUT_0_PRIORITY 0xC40
 */

#define NODE_ESRAM0_IN_0_PRIORITY			0x1000
#define NODE_ESRAM0_IN_1_PRIORITY			0x1004
#define NODE_ESRAM0_IN_2_PRIORITY			0x1008
#define NODE_ESRAM0_IN_3_PRIORITY			0x100C
#define NODE_ESRAM0_IN_0_LIMIT				0x1030
#define NODE_ESRAM0_IN_1_LIMIT				0x1034
#define NODE_ESRAM0_IN_2_LIMIT				0x1038
#define NODE_ESRAM0_IN_3_LIMIT				0x103C
#define NODE_ESRAM0_OUT_0_PRIORITY			0x1040

/* common */
#define NODE_ESRAM1_2_IN_0_PRIORITY			0x1400
#define NODE_ESRAM1_2_IN_1_PRIORITY			0x1404
#define NODE_ESRAM1_2_IN_2_PRIORITY			0x1408
#define NODE_ESRAM1_2_IN_3_PRIORITY			0x140C
#define NODE_ESRAM1_2_IN_0_ARB_1_LIMIT		0x1430
#define NODE_ESRAM1_2_IN_0_ARB_2_LIMIT		0x1434
#define NODE_ESRAM1_2_IN_1_ARB_1_LIMIT		0x1438
#define NODE_ESRAM1_2_IN_1_ARB_2_LIMIT		0x143C
#define NODE_ESRAM1_2_IN_2_ARB_1_LIMIT		0x1440
#define NODE_ESRAM1_2_IN_2_ARB_2_LIMIT		0x1444
#define NODE_ESRAM1_2_IN_3_ARB_1_LIMIT		0x1448
#define NODE_ESRAM1_2_IN_3_ARB_2_LIMIT		0x144C

#define NODE_ESRAM3_4_IN_0_PRIORITY			0x1800
#define NODE_ESRAM3_4_IN_1_PRIORITY			0x1804
#define NODE_ESRAM3_4_IN_2_PRIORITY			0x1808
#define NODE_ESRAM3_4_IN_3_PRIORITY			0x180C
#define NODE_ESRAM3_4_IN_0_ARB_1_LIMIT		0x1830
#define NODE_ESRAM3_4_IN_0_ARB_2_LIMIT		0x1834
#define NODE_ESRAM3_4_IN_1_ARB_1_LIMIT		0x1838
#define NODE_ESRAM3_4_IN_1_ARB_2_LIMIT		0x183C
#define NODE_ESRAM3_4_IN_2_ARB_1_LIMIT		0x1840
#define NODE_ESRAM3_4_IN_2_ARB_2_LIMIT		0x1844
#define NODE_ESRAM3_4_IN_3_ARB_1_LIMIT		0x1848
#define NODE_ESRAM3_4_IN_3_ARB_2_LIMIT		0x184C

#define NODE_HIBW1_DDR1_IN_0_PRIORITY_REG	0x1C00
#define NODE_HIBW1_DDR1_IN_1_PRIORITY_REG	0x1C04
#define NODE_HIBW1_DDR1_IN_2_PRIORITY_REG	0x1C08
#define NODE_HIBW1_DDR1_IN_3_PRIORITY_REG	0x1C0C

#define NODE_HIBW1_DDR1_IN_0_LIMIT_REG		0x1C30
#define NODE_HIBW1_DDR1_IN_1_LIMIT_REG		0x1C34
#define NODE_HIBW1_DDR1_IN_2_LIMIT_REG		0x1C38
#define NODE_HIBW1_DDR1_IN_3_LIMIT_REG		0x1C3C

#define NODE_HIBW1_DDR1_OUT_0_PRIORITY_REG	0x1C40

#define NODE_HIBW2_DDR1_IN_0_PRIORITY_REG	0x2000
#define NODE_HIBW2_DDR1_IN_1_PRIORITY_REG	0x2004

#define NODE_HIBW2_DDR1_IN_0_LIMIT_REG		0x2018
#define NODE_HIBW2_DDR1_IN_1_LIMIT_REG		0x201C
#define NODE_HIBW2_DDR1_OUT_0_PRIORITY_REG	0x2020
#define NODE_DDR_AWQOS_DSP_DDR0_REG			0x340C
#define NODE_DDR_AWQOS_DSP_DDR1_REG			0x3400
#define NODE_DDR_ARQOS_DSP_DDR0_REG			0x3408
#define NODE_DDR_ARQOS_DSP_DDR1_REG			0x3404
#define NODE_DDR_AWQOS_ARM_DDR_REG			0x3410
#define NODE_DDR_ARQOS_ARM_DDR_REG			0x3414
#define NODE_SGA_AWQOS_DDR0_HIBW2_REG		0x3418
#define NODE_SGA_ARQOS_DDR0_HIBW2_REG		0x341C
#define NODE_SGA_AWQOS_SGA_HIBW2_REG		0x3420
#define NODE_SGA_ARQOS_SGA_HIBW2_REG		0x3424

static struct {
	void __iomem *base;
	u32 hibw1_esram_in_pri[4];
	u32 hibw1_esram_in0_arb[3];
	u32 hibw1_esram_in1_arb[3];
	u32 hibw1_esram_in2_arb[3];
	u32 hibw1_esram_in3_arb[3];
	u32 hibw1_ddr0_in_prio[4];
	u32 hibw1_ddr0_in_limit[4];
	u32 hibw1_ddr0_out_prio;
	u32 hibw1_ddr1_in_prio[4];
	u32 hibw1_ddr1_in_limit[4];
	u32 hibw1_ddr1_out_prio;

	/* HiBw2 node registers */
	u32 hibw2_esram_in_pri[2];
	u32 hibw2_esram_in0_arblimit[3];
	u32 hibw2_esram_in1_arblimit[3];
	u32 hibw2_ddr0_in_prio[2];
	u32 hibw2_ddr0_in_limit[2];
	u32 hibw2_ddr0_out_prio;
	u32 hibw2_ddr1_in_prio[2];
	u32 hibw2_ddr1_in_limit[2];
	u32 hibw2_ddr1_out_prio;

	/* ESRAM node registers */
	u32 esram0_in_prio[4];
	u32 esram0_in_lim[4];
	u32 esram0_out_prio;
	u32 esram12_in_prio[4];
	u32 esram12_in_arb_lim[8];
	u32 esram34_in_prio[4];
	u32 esram34_in_arb_lim[8];

	u32 ddr_awqos_dsp_ddr0;
	u32 ddr_awqos_dsp_ddr1;
	u32 ddr_arqos_dsp_ddr0;
	u32 ddr_arqos_dsp_ddr1;
	u32 ddr_awqos_arm_ddr;
	u32 ddr_arqos_arm_ddr;

	u32 sga_awqos_ddr0_hibw2;
	u32 sga_arqos_ddr0_hibw2;

	u32 sga_awqos_sga_hibw2;
	u32 sga_arqos_sga_hibw2;
} context_icn;

/**
 * u8500_context_save_icn() - save ICN context
 *
 */
void u9540_context_save_icn(void)
{
	void __iomem *b = context_icn.base;

	context_icn.hibw1_esram_in_pri[0] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_0_PRIORITY);
	context_icn.hibw1_esram_in_pri[1] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_1_PRIORITY);
	context_icn.hibw1_esram_in_pri[2] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_2_PRIORITY);
	context_icn.hibw1_esram_in_pri[3] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_3_PRIORITY);

	context_icn.hibw1_esram_in0_arb[0] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_0_ARB_1_LIMIT);
	context_icn.hibw1_esram_in0_arb[1] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_0_ARB_2_LIMIT);
	context_icn.hibw1_esram_in0_arb[2] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_0_ARB_3_LIMIT);

	context_icn.hibw1_esram_in1_arb[0] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_1_ARB_1_LIMIT);
	context_icn.hibw1_esram_in1_arb[1] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_1_ARB_2_LIMIT);
	context_icn.hibw1_esram_in1_arb[2] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_1_ARB_3_LIMIT);

	context_icn.hibw1_esram_in2_arb[0] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_2_ARB_1_LIMIT);
	context_icn.hibw1_esram_in2_arb[1] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_2_ARB_2_LIMIT);
	context_icn.hibw1_esram_in2_arb[2] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_2_ARB_3_LIMIT);

	context_icn.hibw1_esram_in3_arb[0] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_3_ARB_1_LIMIT);
	context_icn.hibw1_esram_in3_arb[1] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_3_ARB_2_LIMIT);
	context_icn.hibw1_esram_in3_arb[2] =
		readl_relaxed(b + NODE_HIBW1_ESRAM_IN_3_ARB_3_LIMIT);

	context_icn.hibw1_ddr0_in_prio[0] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_0_PRIORITY);
	context_icn.hibw1_ddr0_in_prio[1] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_1_PRIORITY);
	context_icn.hibw1_ddr0_in_prio[2] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_2_PRIORITY);
	context_icn.hibw1_ddr0_in_prio[3] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_3_PRIORITY);

	context_icn.hibw1_ddr0_in_limit[0] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_0_LIMIT);
	context_icn.hibw1_ddr0_in_limit[1] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_1_LIMIT);
	context_icn.hibw1_ddr0_in_limit[2] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_2_LIMIT);
	context_icn.hibw1_ddr0_in_limit[3] =
		readl_relaxed(b + NODE_HIBW1_DDR0_IN_3_LIMIT);

	context_icn.hibw1_ddr0_out_prio =
		readl_relaxed(b + NODE_HIBW1_DDR0_OUT_0_PRIORITY);

	context_icn.hibw1_ddr1_in_prio[0] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_0_PRIORITY_REG);
	context_icn.hibw1_ddr1_in_prio[1] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_1_PRIORITY_REG);
	context_icn.hibw1_ddr1_in_prio[2] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_2_PRIORITY_REG);
	context_icn.hibw1_ddr1_in_prio[3] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_3_PRIORITY_REG);

	context_icn.hibw1_ddr1_in_limit[0] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_0_LIMIT_REG);
	context_icn.hibw1_ddr1_in_limit[1] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_1_LIMIT_REG);
	context_icn.hibw1_ddr1_in_limit[2] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_2_LIMIT_REG);
	context_icn.hibw1_ddr1_in_limit[3] =
		readl_relaxed(b + NODE_HIBW1_DDR1_IN_3_LIMIT_REG);

	context_icn.hibw1_ddr1_out_prio =
		readl_relaxed(b + NODE_HIBW1_DDR1_OUT_0_PRIORITY_REG);

	context_icn.hibw2_esram_in_pri[0] =
		readl_relaxed(b + NODE_HIBW2_ESRAM_IN_0_PRIORITY);
	context_icn.hibw2_esram_in_pri[1] =
		readl_relaxed(b + NODE_HIBW2_ESRAM_IN_1_PRIORITY);

	context_icn.hibw2_esram_in0_arblimit[0] =
		readl_relaxed(b + NODE_HIBW2_ESRAM_IN_0_ARB_1_LIMIT);
	context_icn.hibw2_esram_in0_arblimit[1] =
		 readl_relaxed(b + NODE_HIBW2_ESRAM_IN_0_ARB_2_LIMIT);
	context_icn.hibw2_esram_in0_arblimit[2] =
		 readl_relaxed(b + NODE_HIBW2_ESRAM_IN_0_ARB_3_LIMIT);

	context_icn.hibw2_esram_in1_arblimit[0] =
		readl_relaxed(b + NODE_HIBW2_ESRAM_IN_1_ARB_1_LIMIT);
	context_icn.hibw2_esram_in1_arblimit[1] =
		readl_relaxed(b + NODE_HIBW2_ESRAM_IN_1_ARB_2_LIMIT);
	context_icn.hibw2_esram_in1_arblimit[2] =
		readl_relaxed(b + NODE_HIBW2_ESRAM_IN_1_ARB_3_LIMIT);

	context_icn.hibw2_ddr0_in_prio[0] =
		readl_relaxed(b + NODE_HIBW2_DDR0_IN_0_PRIORITY);
	context_icn.hibw2_ddr0_in_prio[1] =
		readl_relaxed(b + NODE_HIBW2_DDR0_IN_1_PRIORITY);

	context_icn.hibw2_ddr0_in_limit[0] =
		readl_relaxed(b + NODE_HIBW2_DDR0_IN_0_LIMIT);
	context_icn.hibw2_ddr0_in_limit[1] =
		readl_relaxed(b + NODE_HIBW2_DDR0_IN_1_LIMIT);

	context_icn.hibw2_ddr0_out_prio =
		readl_relaxed(b + NODE_HIBW2_DDR0_OUT_0_PRIORITY);

	context_icn.hibw2_ddr1_in_prio[0] =
		readl_relaxed(b + NODE_HIBW2_DDR1_IN_0_PRIORITY_REG);
	context_icn.hibw2_ddr1_in_prio[1] =
		readl_relaxed(b + NODE_HIBW2_DDR1_IN_1_PRIORITY_REG);

	context_icn.hibw2_ddr1_in_limit[0] =
		readl_relaxed(b + NODE_HIBW2_DDR1_IN_0_LIMIT_REG);
	context_icn.hibw2_ddr1_in_limit[1] =
		readl_relaxed(b + NODE_HIBW2_DDR1_IN_1_LIMIT_REG);

	context_icn.hibw2_ddr1_out_prio =
		readl_relaxed(b + NODE_HIBW2_DDR1_OUT_0_PRIORITY_REG);

	context_icn.esram0_in_prio[0] =
		readl_relaxed(b + NODE_ESRAM0_IN_0_PRIORITY);
	context_icn.esram0_in_prio[1] =
		readl_relaxed(b + NODE_ESRAM0_IN_1_PRIORITY);
	context_icn.esram0_in_prio[2] =
		readl_relaxed(b + NODE_ESRAM0_IN_2_PRIORITY);
	context_icn.esram0_in_prio[3] =
		readl_relaxed(b + NODE_ESRAM0_IN_3_PRIORITY);

	context_icn.esram0_in_lim[0] =
		readl_relaxed(b + NODE_ESRAM0_IN_0_LIMIT);
	context_icn.esram0_in_lim[1] =
		readl_relaxed(b + NODE_ESRAM0_IN_1_LIMIT);
	context_icn.esram0_in_lim[2] =
		readl_relaxed(b + NODE_ESRAM0_IN_2_LIMIT);
	context_icn.esram0_in_lim[3] =
		readl_relaxed(b + NODE_ESRAM0_IN_3_LIMIT);

	context_icn.esram0_out_prio =
		readl_relaxed(b + NODE_ESRAM0_OUT_0_PRIORITY);

	context_icn.esram12_in_prio[0] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_0_PRIORITY);
	context_icn.esram12_in_prio[1] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_1_PRIORITY);
	context_icn.esram12_in_prio[2] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_2_PRIORITY);
	context_icn.esram12_in_prio[3] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_3_PRIORITY);

	context_icn.esram12_in_arb_lim[0] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_0_ARB_1_LIMIT);
	context_icn.esram12_in_arb_lim[1] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_0_ARB_2_LIMIT);
	context_icn.esram12_in_arb_lim[2] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_1_ARB_1_LIMIT);
	context_icn.esram12_in_arb_lim[3] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_1_ARB_2_LIMIT);
	context_icn.esram12_in_arb_lim[4] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_2_ARB_1_LIMIT);
	context_icn.esram12_in_arb_lim[5] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_2_ARB_2_LIMIT);
	context_icn.esram12_in_arb_lim[6] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_3_ARB_1_LIMIT);
	context_icn.esram12_in_arb_lim[7] =
		readl_relaxed(b + NODE_ESRAM1_2_IN_3_ARB_2_LIMIT);

	context_icn.esram34_in_prio[0] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_0_PRIORITY);
	context_icn.esram34_in_prio[1] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_1_PRIORITY);
	context_icn.esram34_in_prio[2] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_2_PRIORITY);
	context_icn.esram34_in_prio[3] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_3_PRIORITY);

	context_icn.esram34_in_arb_lim[0] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_0_ARB_1_LIMIT);
	context_icn.esram34_in_arb_lim[1] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_0_ARB_2_LIMIT);
	context_icn.esram34_in_arb_lim[2] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_1_ARB_1_LIMIT);
	context_icn.esram34_in_arb_lim[3] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_1_ARB_2_LIMIT);
	context_icn.esram34_in_arb_lim[4] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_2_ARB_1_LIMIT);
	context_icn.esram34_in_arb_lim[5] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_2_ARB_2_LIMIT);
	context_icn.esram34_in_arb_lim[6] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_3_ARB_1_LIMIT);
	context_icn.esram34_in_arb_lim[7] =
		readl_relaxed(b + NODE_ESRAM3_4_IN_3_ARB_2_LIMIT);

	context_icn.ddr_awqos_dsp_ddr0 =
		readl_relaxed(b + NODE_DDR_AWQOS_DSP_DDR0_REG);
	context_icn.ddr_awqos_dsp_ddr1 =
		readl_relaxed(b + NODE_DDR_AWQOS_DSP_DDR1_REG);
	context_icn.ddr_arqos_dsp_ddr0 =
		readl_relaxed(b + NODE_DDR_ARQOS_DSP_DDR0_REG);
	context_icn.ddr_arqos_dsp_ddr1 =
		readl_relaxed(b + NODE_DDR_ARQOS_DSP_DDR1_REG);
	context_icn.ddr_awqos_arm_ddr =
		readl_relaxed(b + NODE_DDR_AWQOS_ARM_DDR_REG);
	context_icn.ddr_arqos_arm_ddr =
		readl_relaxed(b + NODE_DDR_ARQOS_ARM_DDR_REG);

	context_icn.sga_awqos_ddr0_hibw2 =
		readl_relaxed(b + NODE_SGA_AWQOS_DDR0_HIBW2_REG);
	context_icn.sga_arqos_ddr0_hibw2 =
		readl_relaxed(b + NODE_SGA_ARQOS_DDR0_HIBW2_REG);

	context_icn.sga_awqos_sga_hibw2 =
		readl_relaxed(b + NODE_SGA_AWQOS_SGA_HIBW2_REG);
	context_icn.sga_arqos_sga_hibw2 =
		readl_relaxed(b + NODE_SGA_ARQOS_SGA_HIBW2_REG);
}

/**
 * u9540_context_restore_icn() - restore ICN context
 *
 */
void u9540_context_restore_icn(void)
{
	void __iomem *b = context_icn.base;

	writel_relaxed(context_icn.hibw1_esram_in_pri[0],
		       b + NODE_HIBW1_ESRAM_IN_0_PRIORITY);
	writel_relaxed(context_icn.hibw1_esram_in_pri[1],
		       b + NODE_HIBW1_ESRAM_IN_1_PRIORITY);
	writel_relaxed(context_icn.hibw1_esram_in_pri[2],
		       b + NODE_HIBW1_ESRAM_IN_2_PRIORITY);
	writel_relaxed(context_icn.hibw1_esram_in_pri[3],
		       b + NODE_HIBW1_ESRAM_IN_3_PRIORITY);

	writel_relaxed(context_icn.hibw1_esram_in0_arb[0],
		       b + NODE_HIBW1_ESRAM_IN_0_ARB_1_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in0_arb[1],
		       b + NODE_HIBW1_ESRAM_IN_0_ARB_2_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in0_arb[2],
		       b + NODE_HIBW1_ESRAM_IN_0_ARB_3_LIMIT);

	writel_relaxed(context_icn.hibw1_esram_in1_arb[0],
		       b + NODE_HIBW1_ESRAM_IN_1_ARB_1_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in1_arb[1],
		       b + NODE_HIBW1_ESRAM_IN_1_ARB_2_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in1_arb[2],
		       b + NODE_HIBW1_ESRAM_IN_1_ARB_3_LIMIT);

	writel_relaxed(context_icn.hibw1_esram_in2_arb[0],
		       b + NODE_HIBW1_ESRAM_IN_2_ARB_1_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in2_arb[1],
		       b + NODE_HIBW1_ESRAM_IN_2_ARB_2_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in2_arb[2],
		       b + NODE_HIBW1_ESRAM_IN_2_ARB_3_LIMIT);

	writel_relaxed(context_icn.hibw1_esram_in3_arb[0],
		       b + NODE_HIBW1_ESRAM_IN_3_ARB_1_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in3_arb[1],
		       b + NODE_HIBW1_ESRAM_IN_3_ARB_2_LIMIT);
	writel_relaxed(context_icn.hibw1_esram_in3_arb[2],
		       b + NODE_HIBW1_ESRAM_IN_3_ARB_3_LIMIT);

	writel_relaxed(context_icn.hibw1_ddr0_in_prio[0],
		       b + NODE_HIBW1_DDR0_IN_0_PRIORITY);
	writel_relaxed(context_icn.hibw1_ddr0_in_prio[1],
		       b + NODE_HIBW1_DDR0_IN_1_PRIORITY);
	writel_relaxed(context_icn.hibw1_ddr0_in_prio[2],
		       b + NODE_HIBW1_DDR0_IN_2_PRIORITY);
	writel_relaxed(context_icn.hibw1_ddr0_in_prio[3],
		       b + NODE_HIBW1_DDR0_IN_3_PRIORITY);

	writel_relaxed(context_icn.hibw1_ddr0_in_limit[0],
		       b + NODE_HIBW1_DDR0_IN_0_LIMIT);
	writel_relaxed(context_icn.hibw1_ddr0_in_limit[1],
		       b + NODE_HIBW1_DDR0_IN_1_LIMIT);
	writel_relaxed(context_icn.hibw1_ddr0_in_limit[2],
		       b + NODE_HIBW1_DDR0_IN_2_LIMIT);
	writel_relaxed(context_icn.hibw1_ddr0_in_limit[3],
		       b + NODE_HIBW1_DDR0_IN_3_LIMIT);

	writel_relaxed(context_icn.hibw1_ddr0_out_prio,
		       b + NODE_HIBW1_DDR0_OUT_0_PRIORITY);

	writel_relaxed(context_icn.hibw1_ddr1_in_prio[0],
		       b + NODE_HIBW1_DDR1_IN_0_PRIORITY_REG);
	writel_relaxed(context_icn.hibw1_ddr1_in_prio[1],
		       b + NODE_HIBW1_DDR1_IN_1_PRIORITY_REG);
	writel_relaxed(context_icn.hibw1_ddr1_in_prio[2],
		       b + NODE_HIBW1_DDR1_IN_2_PRIORITY_REG);
	writel_relaxed(context_icn.hibw1_ddr1_in_prio[3],
		       b + NODE_HIBW1_DDR1_IN_3_PRIORITY_REG);

	writel_relaxed(context_icn.hibw1_ddr1_in_limit[0],
		       b + NODE_HIBW1_DDR1_IN_0_LIMIT_REG);
	writel_relaxed(context_icn.hibw1_ddr1_in_limit[1],
		       b + NODE_HIBW1_DDR1_IN_1_LIMIT_REG);
	writel_relaxed(context_icn.hibw1_ddr1_in_limit[2],
		       b + NODE_HIBW1_DDR1_IN_2_LIMIT_REG);
	writel_relaxed(context_icn.hibw1_ddr1_in_limit[3],
		       b + NODE_HIBW1_DDR1_IN_3_LIMIT_REG);

	writel_relaxed(context_icn.hibw1_ddr1_out_prio,
		       b + NODE_HIBW1_DDR1_OUT_0_PRIORITY_REG);

	writel_relaxed(context_icn.hibw2_esram_in_pri[0],
		       b + NODE_HIBW2_ESRAM_IN_0_PRIORITY);
	writel_relaxed(context_icn.hibw2_esram_in_pri[1],
		       b + NODE_HIBW2_ESRAM_IN_1_PRIORITY);

	writel_relaxed(context_icn.hibw2_esram_in0_arblimit[0],
		       b + NODE_HIBW2_ESRAM_IN_0_ARB_1_LIMIT);
	writel_relaxed(context_icn.hibw2_esram_in0_arblimit[1],
		       b + NODE_HIBW2_ESRAM_IN_0_ARB_2_LIMIT);
	writel_relaxed(context_icn.hibw2_esram_in0_arblimit[2],
		       b + NODE_HIBW2_ESRAM_IN_0_ARB_3_LIMIT);

	writel_relaxed(context_icn.hibw2_esram_in1_arblimit[0],
		       b + NODE_HIBW2_ESRAM_IN_1_ARB_1_LIMIT);
	writel_relaxed(context_icn.hibw2_esram_in1_arblimit[1],
		       b + NODE_HIBW2_ESRAM_IN_1_ARB_2_LIMIT);
	writel_relaxed(context_icn.hibw2_esram_in1_arblimit[2],
		       b + NODE_HIBW2_ESRAM_IN_1_ARB_3_LIMIT);

	writel_relaxed(context_icn.hibw2_ddr0_in_prio[0],
		       b + NODE_HIBW2_DDR0_IN_0_PRIORITY);
	writel_relaxed(context_icn.hibw2_ddr0_in_prio[1],
		       b + NODE_HIBW2_DDR0_IN_1_PRIORITY);

	writel_relaxed(context_icn.hibw2_ddr0_in_limit[0],
		       b + NODE_HIBW2_DDR0_IN_0_LIMIT);
	writel_relaxed(context_icn.hibw2_ddr0_in_limit[1],
		       b + NODE_HIBW2_DDR0_IN_1_LIMIT);

	writel_relaxed(context_icn.hibw2_ddr0_out_prio,
		       b + NODE_HIBW2_DDR0_OUT_0_PRIORITY);

	writel_relaxed(context_icn.hibw2_ddr1_in_prio[0],
		       b + NODE_HIBW2_DDR1_IN_0_PRIORITY_REG);
	writel_relaxed(context_icn.hibw2_ddr1_in_prio[1],
		       b + NODE_HIBW2_DDR1_IN_1_PRIORITY_REG);

	writel_relaxed(context_icn.hibw2_ddr1_in_limit[0],
		       b + NODE_HIBW2_DDR1_IN_0_LIMIT_REG);
	writel_relaxed(context_icn.hibw2_ddr1_in_limit[1],
		       b + NODE_HIBW2_DDR1_IN_1_LIMIT_REG);

	writel_relaxed(context_icn.hibw2_ddr1_out_prio,
		       b + NODE_HIBW2_DDR1_OUT_0_PRIORITY_REG);

	writel_relaxed(context_icn.esram0_in_prio[0],
		       b + NODE_ESRAM0_IN_0_PRIORITY);
	writel_relaxed(context_icn.esram0_in_prio[1],
		       b + NODE_ESRAM0_IN_1_PRIORITY);
	writel_relaxed(context_icn.esram0_in_prio[2],
		       b + NODE_ESRAM0_IN_2_PRIORITY);
	writel_relaxed(context_icn.esram0_in_prio[3],
		       b + NODE_ESRAM0_IN_3_PRIORITY);

	writel_relaxed(context_icn.esram0_in_lim[0],
		       b + NODE_ESRAM0_IN_0_LIMIT);
	writel_relaxed(context_icn.esram0_in_lim[1],
		       b + NODE_ESRAM0_IN_1_LIMIT);
	writel_relaxed(context_icn.esram0_in_lim[2],
		       b + NODE_ESRAM0_IN_2_LIMIT);
	writel_relaxed(context_icn.esram0_in_lim[3],
		       b + NODE_ESRAM0_IN_3_LIMIT);

	writel_relaxed(context_icn.esram0_out_prio,
		       b + NODE_ESRAM0_OUT_0_PRIORITY);

	writel_relaxed(context_icn.esram12_in_prio[0],
		       b + NODE_ESRAM1_2_IN_0_PRIORITY);
	writel_relaxed(context_icn.esram12_in_prio[1],
		       b + NODE_ESRAM1_2_IN_1_PRIORITY);
	writel_relaxed(context_icn.esram12_in_prio[2],
		       b + NODE_ESRAM1_2_IN_2_PRIORITY);
	writel_relaxed(context_icn.esram12_in_prio[3],
		       b + NODE_ESRAM1_2_IN_3_PRIORITY);

	writel_relaxed(context_icn.esram12_in_arb_lim[0],
		       b + NODE_ESRAM1_2_IN_0_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram12_in_arb_lim[1],
		       b + NODE_ESRAM1_2_IN_0_ARB_2_LIMIT);
	writel_relaxed(context_icn.esram12_in_arb_lim[2],
		       b + NODE_ESRAM1_2_IN_1_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram12_in_arb_lim[3],
		       b + NODE_ESRAM1_2_IN_1_ARB_2_LIMIT);
	writel_relaxed(context_icn.esram12_in_arb_lim[4],
		       b + NODE_ESRAM1_2_IN_2_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram12_in_arb_lim[5],
		       b + NODE_ESRAM1_2_IN_2_ARB_2_LIMIT);
	writel_relaxed(context_icn.esram12_in_arb_lim[6],
		       b + NODE_ESRAM1_2_IN_3_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram12_in_arb_lim[7],
		       b + NODE_ESRAM1_2_IN_3_ARB_2_LIMIT);

	writel_relaxed(context_icn.esram34_in_prio[0],
		       b + NODE_ESRAM3_4_IN_0_PRIORITY);
	writel_relaxed(context_icn.esram34_in_prio[1],
		       b + NODE_ESRAM3_4_IN_1_PRIORITY);
	writel_relaxed(context_icn.esram34_in_prio[2],
		       b + NODE_ESRAM3_4_IN_2_PRIORITY);
	writel_relaxed(context_icn.esram34_in_prio[3],
		       b + NODE_ESRAM3_4_IN_3_PRIORITY);

	writel_relaxed(context_icn.esram34_in_arb_lim[0],
		       b + NODE_ESRAM3_4_IN_0_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram34_in_arb_lim[1],
		       b + NODE_ESRAM3_4_IN_0_ARB_2_LIMIT);
	writel_relaxed(context_icn.esram34_in_arb_lim[2],
		       b + NODE_ESRAM3_4_IN_1_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram34_in_arb_lim[3],
		       b + NODE_ESRAM3_4_IN_1_ARB_2_LIMIT);
	writel_relaxed(context_icn.esram34_in_arb_lim[4],
		       b + NODE_ESRAM3_4_IN_2_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram34_in_arb_lim[5],
		       b + NODE_ESRAM3_4_IN_2_ARB_2_LIMIT);
	writel_relaxed(context_icn.esram34_in_arb_lim[6],
		       b + NODE_ESRAM3_4_IN_3_ARB_1_LIMIT);
	writel_relaxed(context_icn.esram34_in_arb_lim[7],
		       b + NODE_ESRAM3_4_IN_3_ARB_2_LIMIT);

	writel_relaxed(context_icn.ddr_awqos_dsp_ddr0,
		       b + NODE_DDR_AWQOS_DSP_DDR0_REG);
	writel_relaxed(context_icn.ddr_awqos_dsp_ddr1,
		       b + NODE_DDR_AWQOS_DSP_DDR1_REG);
	writel_relaxed(context_icn.ddr_arqos_dsp_ddr0,
		       b + NODE_DDR_ARQOS_DSP_DDR0_REG);
	writel_relaxed(context_icn.ddr_arqos_dsp_ddr1,
		       b + NODE_DDR_ARQOS_DSP_DDR1_REG);
	writel_relaxed(context_icn.ddr_awqos_arm_ddr,
		       b + NODE_DDR_AWQOS_ARM_DDR_REG);
	writel_relaxed(context_icn.ddr_arqos_arm_ddr,
		       b + NODE_DDR_ARQOS_ARM_DDR_REG);

	writel_relaxed(context_icn.sga_awqos_ddr0_hibw2,
		       b + NODE_SGA_AWQOS_DDR0_HIBW2_REG);
	writel_relaxed(context_icn.sga_arqos_ddr0_hibw2,
		       b + NODE_SGA_ARQOS_DDR0_HIBW2_REG);

	writel_relaxed(context_icn.sga_awqos_sga_hibw2,
		       b + NODE_SGA_AWQOS_SGA_HIBW2_REG);
	writel_relaxed(context_icn.sga_arqos_sga_hibw2,
		       b + NODE_SGA_ARQOS_SGA_HIBW2_REG);
}

void u9540_context_init(void)
{
	context_icn.base = ioremap(U8500_ICN_BASE, SZ_16K);
}
