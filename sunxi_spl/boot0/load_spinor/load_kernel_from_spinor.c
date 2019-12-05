/*
 * (C) Copyright 2016
 *     zhuangqiubin@allwinnertech.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 */
#include "common.h"
#include "spare_head.h"
#include "private_boot0.h"
#include "private_uboot.h"
#include <private_toc.h>
#include <asm/arch/spinor.h>

#define  SUNXI_FLASH_READ_FIRST_SIZE      (32 * 1024)

#ifdef BOOT0_LOAD_KERNEL
static int load_kernel_from_spinor(void)
{
	int ret;
	u32 rbytes = 0;
	u32 rblock = 0;
	u32 start_block = start_block = BOOT0_LOAD_KERNEL_START_SECTOR;
	void *addr = (void *)BOOT0_LOAD_KERNEL_BUF;
	image_header_t *uz_hdr;

	if(spinor_init(0))
	{
		printf("spinor init fail\n");
		return -1;
	}

	if(boot0_spinor_sector_read(start_block+ CONFIG_SPINOR_LOGICAL_OFFSET,SUNXI_FLASH_READ_FIRST_SIZE/512, addr))
	{
		printf("Fail to load kernel head\n");
		return -1;
	}

	uz_hdr = (image_header_t *)addr;

	if (image_check_magic(uz_hdr))
	{
		//printf("kernel magic is ok\n");
		rbytes = image_get_data_size(uz_hdr) + image_get_header_size() + 512;
	}
	else
	{
		printf("kernel magic is error\n");
		return -1;
	}

	rblock = rbytes/512 - SUNXI_FLASH_READ_FIRST_SIZE/512;
	start_block += SUNXI_FLASH_READ_FIRST_SIZE/512;
	addr = (void *)(BOOT0_LOAD_KERNEL_BUF + SUNXI_FLASH_READ_FIRST_SIZE);
	debug("rblock=%d, start=%d\n", rblock, start_block);

	ret = boot0_spinor_sector_read(start_block+ CONFIG_SPINOR_LOGICAL_OFFSET,rblock, addr);

	return ret == 0 ? 0: -1;
}
#endif

int load_kernel(void)
{
#ifdef BOOT0_LOAD_KERNEL
       return load_kernel_from_spinor();
#else
       return -1;
#endif
}


