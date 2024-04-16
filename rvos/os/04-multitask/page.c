#include "os.h"

// 一页的地址4096Byte
#define PAGE_SIZE 4096

// 页是否被使用或者是否是最后一页
#define PAGE_USE (uint8_t)(1 << 0)
#define PAGE_LAST (uint8_t)(1 << 1)

// 分页的数量
static uint32_t _pages_num = 0;
// 地址分配的开始和结束 因为内存管理的数组也会占空间
static uint32_t _alloc_start = 0;
static uint32_t _alloc_end = 0;

// 内存管理结构体
struct Page
{
    uint8_t flags;
};

// 清理当前页
static inline void _clear(struct Page *page)
{
    page->flags = 0;
};

// 当前页是否为空
static inline int _is_free(struct Page *page)
{
    if (page->flags & PAGE_USE)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// 当前页是否是最后一页
static inline int _is_last(struct Page *page)
{
    if (page->flags & PAGE_LAST)
    {
        return 1;
    }
    else
    {
        return 0;
    }
};

// 设置页标签
static inline void _set_flag(struct Page *page, uint8_t flag)
{
    // 用或运算赋值
    page->flags |= flag;
};

// 4k对齐
static inline uint32_t
_align_page(uint32_t address)
{
    // 内存向上取整
    uint32_t mask = (1 << 12) - 1;
    return (address + mask) & (~mask);
};

// 分页初始化
void page_init()
{
    // 初始化分页的时候 先打印当前所有的段的信息

    printf("-----section info start-----\n");
    print_addr("TEXT_START\t", TEXT_START);
    print_addr("TEXT_END\t", TEXT_END);
    print_addr("RODATA_START\t", RODATA_START);
    print_addr("RODATA_END\t", RODATA_END);
    print_addr("DATA_START\t", DATA_START);
    print_addr("DATA_END\t", DATA_END);
    print_addr("BSS_START\t", BSS_START);
    print_addr("BSS_END\t\t", BSS_END);
    print_addr("HEAP_START\t", HEAP_START);
    print_addr("HEAP_SIZE\t", HEAP_SIZE);
    printf("------section info end------\n");

    // 拿到堆的起始地址
    uint32_t heap_start = HEAP_START;
    // 内存是128MB 128MB * 1024 * 1024 = 134217728 Byte 的内存需要被管理
    // 134217728 / 4096 = 32768 个页可以管理完
    // 使用连续内存保存空闲页 每8个字节管理一个页
    // 32768 / 4096 = 8 所以要用8个页的大小管理其余32760个页
    // 大概用8个页
    _pages_num = (HEAP_SIZE / PAGE_SIZE) - 8;
    printf("memory page number: %d \n", _pages_num);

    // 清理空间
    struct Page *page = (struct Page *)(heap_start);
    for (int i = 0; i < _pages_num; i++)
    {
        // 清理当前页
        _clear(page);
        page++;
    }

    // 地址分配的开始
    _alloc_start = _align_page(heap_start + 8 * 4096);
    _alloc_end = _alloc_start + 4096 * _pages_num;
    print_addr("address descriptor start", heap_start);
    print_addr("address allocate start", _alloc_start);
}

// 传入分配的页面数量 返回内存地址
void *page_alloc(int page_num)
{
    int found = 0;
    struct Page *page_i = (struct Page *)HEAP_START;
    for (int i = 0; i < _pages_num - page_num; i++)
    {
        // 如果找到了空的内存空间
        if (_is_free(page_i))
        {
            found = 1;
            // 进一步搜索
            struct Page *page_j = page_i + 1;
            // 从下一块区域搜索 搜索 (page_num+i)-(i+1) = page_num -1 次
            for (int j = i + 1; j < page_num + i; j++)
            {
                if (!_is_free(page_j))
                {
                    // 不符合情况
                    found = 0;
                    break;
                }
                page_j++;
            }
        }
        if (found)
        {
            // 找到了 修改flags
            struct Page *page = page_i;
            for (int k = i; k < (i + page_num); k++)
            {
                // 设置为已被使用
                _set_flag(page, PAGE_USE);
                page++;
            }
            // 最后一个块设置为last
            _set_flag(page - 1, PAGE_LAST);
            return (void *)(_alloc_start + PAGE_SIZE * i);
        }
        page_i++;
    }
    return NULL;
}

void page_free(void *p)
{
    // 清除地址为p的内存
    // 首先检查p是否合格
    if (!p || (uint32_t)p >= _alloc_end)
    {
        return;
    }
    // 拿到页描述符的地址
    struct Page *page = (struct Page *)HEAP_START;
    page += ((uint32_t)p - _alloc_start) / PAGE_SIZE;

    while (!_is_free(page))
    {
        if (_is_last(page))
        {
            _clear(page);
            break;
        }
        else
        {
            _clear(page);
            page++;
        }
    }
}