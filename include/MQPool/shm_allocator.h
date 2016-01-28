#ifndef MQPOOL_INCLUDE_SHM_ALLOCATOR_H_
#define MQPOOL_INCLUDE_SHM_ALLOCATOR_H_

#include <string>

using std::string;

/*
 * 共享内存区是可用IPC形式当中最快的，但是一定要加上保护
 */
class ShmAllocator {
 public:
  explicit ShmAllocator();
  explicit ShmAllocator(string shmFile, uint64_t shmSize);
  ShmAllocator(const ShmAllocator&) = delete;
  ShmAllocator& operator=(const ShmAllocator&) = delete;
  virtual ~ShmAllocator();
  void Dump() const;

  int GetShmID() const;

  string GetShmFile() const;

  uint64_t GetShmSize() const;

  /*
   * 获取到的是最顶部的地址，要想从内存池当中分配内存，就使用Allocate函数
   */
  void* GetShmAddr() const;

  void Attach();

  void Detach();

  // 关键函数：从内存池当中取出size大小的内存
  void* Allocate(uint64_t size);
  // 关键函数：将ptr指向的内存释放会内存池当中
  bool Deallocate(void *ptr);

 private:
  //enum hack
  enum { BLOCK_SIZE = 8 };                    // 对齐的SIZE
  enum { MAX_BYTES = 4 * 1024 * 1024 };       // 最大分配的SIZE
  enum { MIN_BYTES = 8 };                     // 最小分配的SIZE，设置的时候不能小于BLOCK_SIZE
  enum { READY_FLAG = 9693823 };              // 是否已经准备好的标志值, 其实这里可以直接使用1和0

 private:
  // 在共享内存当中的head，存储了共享内存的信息
  typedef struct {
    uint64_t memorySize;                    // 共享内存SIZE
    uint64_t minBytes;                      // 单次分配最小SIZE
    uint64_t maxBytes;                      // 单次分配最大SIZE
    uint64_t blockSize;                     // 对齐SIZE，每次分配内存空间就是它的整数倍
    uint64_t memoryCount;                   // 已经分配出去的内存数量
    uint64_t currentOffset;                 // 当前可分配的地址偏移量, shmAddr_ + currentOffset就是当前可用的地址
    uint64_t managedSize;                   // 管理中的碎片空间
    int iReady;                             // 是否已经准备好
    uint64_t szFreeList[0];                 // 管理各个大小的空闲Buffer列表,之所以把它写在Head_t当中是为了利用它来进行数组越界
  } Head_t;

  // 每次调用Allocate从内存池当中取出内存的时候，这个数据结构就表明了分配的内存大小以及下一个可用内存的偏移量
  typedef struct {
    uint64_t size;
    uint64_t next;
  } Pointer_t;

 private:
  // 将size向上取整，就是取到blockSize的最小整数倍
  uint64_t RoundUp(uint64_t size) const;

 private:
  int shmid_;

  //用于ftok的shmFile_
  string shmFile_;

  void* shmAddr_;   // 使用shmat获取到的值
  uint64_t shmSize_;  // 初始化共享内存的时候指定的大小

  Head_t* pHead;
};

#endif /* MQPOOL_INCLUDE_SHM_ALLOCATOR_H_ */
