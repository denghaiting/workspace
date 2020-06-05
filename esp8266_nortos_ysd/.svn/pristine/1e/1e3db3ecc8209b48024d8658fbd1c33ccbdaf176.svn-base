#ifndef __IWUOS_MEM_HEADER__
#define __IWUOS_MEM_HEADER__

/*
	功能:
		动态内存分配
	输入参数:
		@size: 分配内存的大小
	输出参数:
		@info: 保存系统信息
	返回值:
		void *:分配的内存
		NULL :失败
	说明:
*/
void *iw_malloc(u_int32_t size);
/*
	功能:
		动态内存分配 = size * n
	输入参数:
		@size: 分配内存的大小
		@n: 内存块数
	输出参数:
		@info: 保存系统信息
	返回值:
		void *:分配的内存
		NULL :失败
	说明:
*/
void *iw_calloc(u_int32_t size, u_int32_t n);
/*
	功能:
		释放分配的内存
	输入参数:
		@p: 内存地址
	输出参数:
	返回值:
	说明:
*/
void iw_free(void *p);

#endif
