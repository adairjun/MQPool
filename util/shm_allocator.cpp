#include "MQPool/shm_allocator.h"
#include "MQPool/logobj.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

ShmAllocator::ShmAllocator()
    : shmFile_("key"), shmSize_(MAX_BYTES) {
	key_t key = ftok(shmFile_.c_str(),'a');
	shmid_ = shmget(key, shmSize_, S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
	if (shmid_ == -1) {
      LOG(INFO) << shmFile_ << " is Exist";
	}
	shmid_ = shmget(key, shmSize_, S_IRUSR|S_IWUSR|IPC_CREAT);
    shmAddr_ = NULL;
}

/*
 * shmSize 用字节为单位指定内存区的大小
 */
ShmAllocator::ShmAllocator(string shmFile, uint64_t shmSize)
    : shmFile_(shmFile) {
	// 将shmSize向上取整
	shmSize_ = RoundUp(shmSize);
	key_t key = ftok(shmFile_.c_str(),'a');
	shmid_ = shmget(key, shmSize_, S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
	if (shmid_ == -1) {
	  LOG(INFO) << shmFile_ << " is Exist";
	}
	shmid_ = shmget(key, shmSize_, S_IRUSR|S_IWUSR|IPC_CREAT);
	shmAddr_ = NULL;
}

ShmAllocator::~ShmAllocator() {
  Detach();
}

void ShmAllocator::Dump() const {
  printf("\n=====ShmAllocator Dump START ========== \n");
  printf("shmid_=%d \n", shmid_);
  printf("shmFile_=%s \n", shmFile_.c_str());
  printf("shmSize_=%lu ", shmSize_);
  printf("\n=====ShmAllocator DUMP END =============\n");
}


int ShmAllocator::GetShmID() const {
  return shmid_;
}

string ShmAllocator::GetShmFile() const {
  return shmFile_;
}

uint64_t ShmAllocator::GetShmSize() const {
  return shmSize_;
}

void* ShmAllocator::GetShmAddr() const {
  return shmAddr_;
}

void ShmAllocator::Attach() {
  shmAddr_ = shmat(shmid_, NULL, 0);
  // 设置pHead
  pHead = (Head_t*)shmAddr_;
  pHead->memorySize = shmSize_;
  pHead->minBytes = MIN_BYTES;
  pHead->maxBytes = shmSize_ > MAX_BYTES ? MAX_BYTES : shmSize_;
  pHead->blockSize = BLOCK_SIZE;
  pHead->memoryCount = 0;
  pHead->managedSize = 0;
  pHead->iReady = READY_FLAG;

  uint64_t offset = sizeof(Head_t);
  // 初始化链表
  for (uint64_t i = 0; i < pHead->maxBytes / pHead->blockSize; ++i) {
    pHead->szFreeList[i] = 0;
    if (i != 0) {
      // 由于Head_t里面已经包含了szFreeList[0]，所以就不能把0给加上
      offset += sizeof(uint64_t);
    }
  }
  pHead->currentOffset = offset;  //还是offset - sizeof(uint64_t)?
}

void ShmAllocator::Detach() {
  if (shmAddr_) {
    shmdt(shmAddr_);
  }
}

uint64_t ShmAllocator::RoundUp(uint64_t size) const {
  return (size%(uint64_t)BLOCK_SIZE == 0) ? size : (size + (uint64_t)BLOCK_SIZE - size%(uint64_t)BLOCK_SIZE);
  // 其实有个dark magic的写法是这样子的：但是这个写法要求BLOCK_SIZE是2的n次幂
  // return ((size + (uint64_t)BLOCK_SIZE - 1) & ~((uint64_t)BLOCK_SIZE - 1));
}

void* ShmAllocator::Allocate(uint64_t size) {
  // 从内存池当中分配的内存空间也要加上一个头，这个头是uint64_t类型的，值就是size
  uint64_t realSize = RoundUp(size + sizeof(uint64_t));
  if (realSize == 0) {
	return NULL;
  }

  // 由于realSize必定是blockSize的整数倍，那么必定能够除尽，
  // 一个block算作一个桶，这个计算一下需要多少个桶才能放进去这个realSize
  // 但是这个桶并非在桶排序当中使用的，而仅仅是一个标记，这里建立了bucket和realSize的一一映射关系之后
  // szFreeList[bucket]和realSize也就有了一一映射的关系，在szFreeList[bucket]当中保存realSize的指针的偏移量
  // 那么当我需要realSize大小的内存的时候，就可以先通过szFreeList[bucket]检索出内存碎片的偏移量。
  int bucket = realSize / pHead->blockSize - 1;

  if (pHead->szFreeList[bucket] == 0) {
    // 如果检索出的内存碎片的偏移量是0，那么就说明：要么是这个内存池当中没有realSize大小的内存碎片，要么就是这个碎片现在正在被占用
	// 不管是哪一种情况，现在都需要创建realSize大小的内存碎片
	if (pHead->memorySize - pHead->currentOffset < realSize) {
	  // 由于没有合适大小的内存碎片，所以现在要从内存池当中分配内存了，这个时候realSize不能超过可用大小
	  return NULL;
	}

	Pointer_t* pCurr = (Pointer_t *)((char *)shmAddr_ + pHead->currentOffset);
	pCurr->size = realSize;
	// 虽然szFreeList[bucket]的值是0，但是还是不要写0
	// 因为pCurr->next始终指向下一个可用的内存空间，szFreeList[bucket]也是指向可用的内存空间，这两个变量的意义是一模一样的
	pCurr->next = pHead->szFreeList[bucket];

	// 分配的内存空间可用，那么就把这个新的地址放到szFreeList[bucket]当中去
	pHead->szFreeList[bucket] = pHead->currentOffset;
	pHead->currentOffset += realSize;
	// 这是个碎片空间，做个记录
	pHead->managedSize += realSize;
  }

  // 不管有没有进入上面的if语句，现在100%是有了可用的内存碎片了
  //（其实不能说100%，因为这个程序不是线程安全的，但是这样想：这个程序是为了单机上的进程间通信，如果还有线程参与进来，那么程序就变成了多进程+多线程，
  // 陈硕老师在他的书里面也说了多进程+多线程这个模式不仅没有多进程和多线程的优点，反而集合了它们的缺点，所以使用多进程+多线程的这个模式很少，大可不必担心）
  // 现在要返回可用的内存碎片的地址
  Pointer_t* pMem = (Pointer_t *)((char *)shmAddr_ + pHead->szFreeList[bucket]);
  // 拿到了碎片的地址，接下来就要把下一个可用的内存空间放入到szFreeList[bucket]当中去了，这个好理解
  pHead->szFreeList[bucket] = pMem->next;
  // 每分出去一个内存区，就做个记录
  pHead->memoryCount++;
  // 分出去的内存区不在碎片空间的统计当中了
  pHead->managedSize -= realSize;

  return (char *)pMem + sizeof(uint64_t);     //sizeof(uint64_t)是size的空间。只保留size就可以了，Pointer_t结构的next已经没用了，可以覆盖掉
}

bool ShmAllocator::Deallocate(void *ptr) {
  uint64_t offset = (char *)ptr - (char *)shmAddr_;
  if (offset < 0 || offset > pHead->maxBytes) {
    return false;
  }
  // 由于在Allocate的return当中加上了size的大小sizeof(uint64_t)，那么这里要还原的话就要把offset减去这个值
  offset -= sizeof(uint64_t);

  Pointer_t *pMem = (Pointer_t *)((char *)shmAddr_ + offset);
  int bucket = pMem->size / pHead->blockSize - 1;

  // 释放空间就是让这段空间可用，那么只要把它标记为可用就可以了
  pMem->next = pHead->szFreeList[bucket];
  pHead->szFreeList[bucket] = offset;

  pHead->memoryCount--;
  pHead->managedSize += pMem->size;
}

