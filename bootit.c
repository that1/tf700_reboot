// bootit.c - kernel module to reboot

//#define __KERNEL__
//#define MODULE

#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <asm/io.h>
#include <mach/iomap.h>

// #include "pm.h"
#define PMC_SCRATCH0 0x50

MODULE_DESCRIPTION("Tegra3 rebooter");
MODULE_LICENSE("GPL");

// from mach-tegra/common.c
#define   RECOVERY_MODE        BIT(31)
#define   BOOTLOADER_MODE      BIT(30)
#define   FORCED_RECOVERY_MODE BIT(1)


static int __init bootit_init(void)
{
	void __iomem *reset = IO_ADDRESS(TEGRA_PMC_BASE + 0x00);
	u32 reg;

	reg = readl_relaxed(reset + PMC_SCRATCH0);
	/* Writing recovery kernel or Bootloader mode in SCRATCH0 31:30:1 */
	/*
	if (cmd) {
		if (!strcmp(cmd, "recovery"))
			reg |= RECOVERY_MODE;
		else if (!strcmp(cmd, "bootloader"))
			reg |= BOOTLOADER_MODE;
		else if (!strcmp(cmd, "forced-recovery"))
			reg |= FORCED_RECOVERY_MODE;
		else
			reg &= ~(BOOTLOADER_MODE | RECOVERY_MODE | FORCED_RECOVERY_MODE);
	}
	else {
	*/
		/* Clearing SCRATCH0 31:30:1 on default reboot */
		reg &= ~(BOOTLOADER_MODE | RECOVERY_MODE | FORCED_RECOVERY_MODE);
	/* } */
	writel_relaxed(reg, reset + PMC_SCRATCH0);
	/* use *_related to avoid spinlock since caches are off */
	reg = readl_relaxed(reset);
	reg |= 0x10;
	writel_relaxed(reg, reset);

	return 0;
}

static void __exit bootit_exit(void)
{
}

module_init(bootit_init);
module_exit(bootit_exit);

