#ifndef MQPOOL_INCLUDE_SHM_ALLOCATOR_H_
#define MQPOOL_INCLUDE_SHM_ALLOCATOR_H_

#include <string>
#include <map>

using std::string;
using std::map;
using std::make_pair;

/*
 * 共享内存区是可用IPC形式当中最快的，但是一定要加上保护
 */
class ShmAllocator {
 public:
  /*
   * 当server为true的时候，执行的是创建一个共享内存区
   * 当server为false的时候，执行的是获取已经存在的共享内存区
   */
  explicit ShmAllocator(bool server);
  explicit ShmAllocator(string shmFile, uint64_t shmSize, bool server);
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

  /*
   * 获取剩余的大块内存空间
   */
  uint64_t GetFreeSize() const;

  /*
   * 获取全部剩余空间：大块内存空间加上所有碎片空间
   */
  uint64_t GetTotalFreeSize() const;

  /*
   * 如果是创建一个不存在的共享内存区，那么调用Attach()之后要使用InitPHead来初始化pHead
   * 如果仅仅只是访问已经存在的内存区，就不要乱动InitPHead
   */
  void Attach();

  void InitPHead();

  void Detach();

  // 关键函数：从内存池当中取出size大小的内存, id是分配给这个内存区的id
  void* Allocate(uint64_t size, uint64_t id);
  // 关键函数：将ptr指向的内存释放会内存池当中
  bool Deallocate(void *ptr);

  //关键函数：通过id来获取到对应的内存块的地址
  void* GetMemById(uint64_t id);

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

/*

 * 用一个singleton来保存偏移量
 * 一个id就是我写在protobuf当中的id，比如说是test::JUST_TEST_REQUEST
 * 那么一个id可以有多个消息，这里就使用multimap而不是map

typedef std::multimap<uint64_t, uint64_t> IdToOffset;
inline IdToOffset& GlobalIdToOffset() {
  static IdToOffset themap;
  return themap;
}

inline void AddInIdToOffset(uint64_t key, uint64_t value) {
  IdToOffset &themap = GlobalIdToOffset();
  themap.insert(make_pair(key,value));
}

inline uint64_t GetFromIdToOffset(uint64_t key) {
  IdToOffset &themap = GlobalIdToOffset();
  if(themap.count(key)) {
	// multimap不支持下标运算，这里返回第一个key的value
	return themap.lower_bound(key)->second;
  } else {
    //TODO log
    return 0;
  }
}


 * 根据value来删除pair

inline bool EraseFromIdToOffset(uint64_t value) {
  IdToOffset &themap = GlobalIdToOffset();
  for (IdToOffset::iterator iter = themap.begin(); iter != themap.end(); ++iter) {
    if (iter->second == value) {
      themap.erase(iter);
      return true;
    }
  }
  return false;
}
*/

/*
 * 这也是一个共享内存，算是管理者内存，为的是把id和offset的映射关系保存下来
 * 一个id就是我写在protobuf当中的id，比如说是test::JUST_TEST_REQUEST
 * 这是一个singleton,而且使用了pImpl idiom手法
 */
class ManagerMem {
 public:
  Init();                         // 初始化
 protected:
  ManagerMem();
 private:
  typedef struct {
	char[32] shmFile;              // 注意：这里是被管理的共享内存的shmFile
    uint64_t length;               // 管理内存的总长度
  } ManagerMemHead_t;              // struct的字节对齐规则是这样的，先为char[32]分配内存，占32个字节，然后为length分配字节，由于length占4个字节
                                   // 而已经分配的32个字节是4的倍数，那么分配的字节就是32+4 = 36
  typedef struct {
	uint64_t id;
	uint64_t offset;
	bool isReaded;
	bool canBeOverwrite;
  } Map_t;                        // 这就是在管理内存当中存储的节点，管理内存的ManagerMemHead_t之后，这些节点是按照普通的链表来排序
                                  // 要想遍历这些节点效率较低，我正在思考如何使用红黑树的数据结构来存储节点
};

/*

inline ShmAllocator& GlobalManagerMem() {
  static ShmAllocator managerMem;
  return managerMem;
}
*/

#endif /* MQPOOL_INCLUDE_SHM_ALLOCATOR_H_ */
