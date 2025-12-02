#pragma once

#include "msddef.hpp"

namespace msd {

// 内存块控制信息（隐藏在分配的内存块前面）
struct MemoryBlock {
    size_t block_size; // 块大小（包括控制头）
    bool is_allocated; // 分配状态
    MemoryBlock* next; // 指向下一个内存块
};

// 内存池类
class MemoryPool {
    private:
    uint8_t* pool_start;    // 内存池起始地址
    uint8_t* pool_end;      // 内存池结束地址
    size_t total_size;      // 总大小
    size_t used_size;       // 已使用大小
    size_t alloc_count;     // 分配次数
    MemoryBlock* free_list; // 空闲链表

    // 禁用拷贝和赋值
    MemoryPool(const MemoryPool&)            = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    public:
    MemoryPool() noexcept
    : pool_start(nullptr), pool_end(nullptr), total_size(0),
      used_size(0), alloc_count(0), free_list(nullptr) {}

    // 初始化内存池
    bool init(void* memory, size_t size) noexcept {
        if (!memory || size < sizeof(MemoryBlock) + 8) {
            return false;
        }

        pool_start  = static_cast<uint8_t*>(memory);
        pool_end    = pool_start + size;
        total_size  = size;
        used_size   = 0;
        alloc_count = 0;

        // 初始化空闲链表
        free_list               = reinterpret_cast<MemoryBlock*>(pool_start);
        free_list->block_size   = size;
        free_list->is_allocated = false;
        free_list->next         = nullptr;

        return true;
    }

    // 分配内存
    void* allocate(size_t size) noexcept {
        if (size == 0 || free_list == nullptr) {
            return nullptr;
        }

        // 对齐到8字节边界
        size = (size + 7) & ~7;

        // 添加控制头大小
        size_t required_size = size + sizeof(MemoryBlock);

        MemoryBlock* prev    = nullptr;
        MemoryBlock* current = free_list;

        // 首次适配算法
        while (current) {
            if (!current->is_allocated && current->block_size >= required_size) {
                // 找到合适的块
                if (current->block_size >= required_size + sizeof(MemoryBlock) + 8) {
                    // 可以分割内存块
                    MemoryBlock* new_block = reinterpret_cast<MemoryBlock*>(
                    reinterpret_cast<uint8_t*>(current) + required_size);
                    new_block->block_size   = current->block_size - required_size;
                    new_block->is_allocated = false;
                    new_block->next         = current->next;

                    current->block_size = required_size;
                    current->next       = new_block;
                }

                current->is_allocated = true;
                used_size += current->block_size;
                alloc_count++;

                // 从空闲链表中移除
                if (prev) {
                    prev->next = current->next;
                } else {
                    free_list = current->next;
                }

                // 返回用户可用内存（跳过控制头）
                return reinterpret_cast<uint8_t*>(current) + sizeof(MemoryBlock);
            }
            prev    = current;
            current = current->next;
        }

        return nullptr; // 没有足够的内存
    }

    // 释放内存
    bool deallocate(void* ptr) noexcept {
        if (!ptr || ptr < pool_start || ptr >= pool_end) {
            return false;
        }

        // 获取内存块控制头
        MemoryBlock* block = reinterpret_cast<MemoryBlock*>(
        static_cast<uint8_t*>(ptr) - sizeof(MemoryBlock));

        if (!block->is_allocated) {
            return false; // 重复释放
        }

        used_size -= block->block_size;
        block->is_allocated = false;

        // 合并相邻的空闲块
        merge_free_blocks(block);

        // 插入到空闲链表头部
        block->next = free_list;
        free_list   = block;

        return true;
    }

    // 获取统计信息
    size_t get_total_size() const noexcept { return total_size; }
    size_t get_used_size() const noexcept { return used_size; }
    size_t get_free_size() const noexcept { return total_size - used_size; }
    size_t get_allocation_count() const noexcept { return alloc_count; }
    bool empty() const noexcept { return used_size == 0; }

    // 重置内存池（危险！会丢失所有已分配内存的引用）
    void reset() noexcept {
        free_list               = reinterpret_cast<MemoryBlock*>(pool_start);
        free_list->block_size   = total_size;
        free_list->is_allocated = false;
        free_list->next         = nullptr;
        used_size               = 0;
        alloc_count             = 0;
    }

    private:
    // 合并空闲块
    void merge_free_blocks(MemoryBlock* block) noexcept {
        MemoryBlock* current = free_list;

        while (current) {
            // 检查是否可以与后面的块合并
            uint8_t* current_end = reinterpret_cast<uint8_t*>(current) + current->block_size;
            if (current_end == reinterpret_cast<uint8_t*>(block)) {
                // 当前块后面是释放的块
                current->block_size += block->block_size;
                return;
            }

            // 检查是否可以与前面的块合并
            uint8_t* block_end = reinterpret_cast<uint8_t*>(block) + block->block_size;
            if (block_end == reinterpret_cast<uint8_t*>(current)) {
                // 释放的块后面是当前块
                block->block_size += current->block_size;
                block->next = current->next;

                // 从链表中移除当前块
                MemoryBlock* prev = free_list;
                if (prev == current) {
                    free_list = block;
                } else {
                    while (prev && prev->next != current) {
                        prev = prev->next;
                    }
                    if (prev) {
                        prev->next = block;
                    }
                }
                return;
            }

            current = current->next;
        }
    }
};

// 固定大小的内存池（性能更高）
template <size_t BlockSize, size_t NumBlocks>
class FixedMemoryPool {
    private:
    struct Block {
        Block* next;
        uint8_t data[BlockSize];
    };

    union Align {
        Block block;
        void* ptr;
        size_t size;
    };

    static constexpr size_t Alignment = alignof(Align);
    static constexpr size_t TotalSize = NumBlocks * sizeof(Block);

    uint8_t memory[TotalSize];
    Block* free_list;

    public:
    FixedMemoryPool() noexcept {
        init();
    }

    void init() noexcept {
        // 初始化所有块并加入空闲链表
        free_list = reinterpret_cast<Block*>(memory);

        for (size_t i = 0; i < NumBlocks - 1; ++i) {
            Block* current = reinterpret_cast<Block*>(
            memory + i * sizeof(Block));
            Block* next = reinterpret_cast<Block*>(
            memory + (i + 1) * sizeof(Block));
            current->next = next;
        }

        reinterpret_cast<Block*>(memory + (NumBlocks - 1) * sizeof(Block))->next = nullptr;
    }

    void* allocate() noexcept {
        if (!free_list) {
            return nullptr;
        }

        Block* block = free_list;
        free_list    = block->next;
        return block->data;
    }

    void deallocate(void* ptr) noexcept {
        if (!ptr) return;

        // 计算块起始地址
        Block* block = reinterpret_cast<Block*>(
        static_cast<uint8_t*>(ptr) - offsetof(Block, data));

        block->next = free_list;
        free_list   = block;
    }

    size_t get_free_count() const noexcept {
        size_t count   = 0;
        Block* current = free_list;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }

    size_t get_used_count() const noexcept { return NumBlocks - get_free_count(); }
    size_t get_block_size() const noexcept { return BlockSize; }
    size_t get_total_blocks() const noexcept { return NumBlocks; }
};

} // namespace msd