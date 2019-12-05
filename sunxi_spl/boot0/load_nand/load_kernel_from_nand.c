
#include "common.h"
#include "spare_head.h"
#include "private_uboot.h"
#include "asm/arch/nand_boot0.h"
#include <private_toc.h>


static int load_kernel_from_nand(void)
{
       return -1;
}

int load_kernel(void)
{
       return load_kernel_from_nand();
}
