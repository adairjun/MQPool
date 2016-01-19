#ifndef MQPOOL_INCLUDE_SHM_ALLOCATOR_H_
#define MQPOOL_INCLUDE_SHM_ALLOCATOR_H_

#include <string>

using std::string;

/*
 * 共享内存区是可用IPC形式当中最快的，但是多线程环境下一定要加上保护,否则的话根本就不知到是哪一个线程对它进行了操作，根本没办法调试
 */
class ShmAllocator {
 public:
  explicit ShmAllocator();
  explicit ShmAllocator(string shmFile);
  ShmAllocator(const ShmAllocator&) = delete;
  ShmAllocator& operator=(const ShmAllocator&) = delete;
  virtual ~ShmAllocator();
  void Dump() const;

  int GetShmID() const;

  string GetShmFile() const;

  /*
   * 设置了shmFile_之后，连带生成新的shmid_
   */
  void SetShmFile(const string& msgFile);

  void Attach();

  void Detach();

  void* GetShmAddr() const;

 private:
  // enum hack
  enum { MAX_BYTES = 4 * 1024 * 1024 };       // 最大分配的SIZE

 private:
  int shmid_;

  //用于ftok的shmFile_
  string shmFile_;

  void* shmAddr_;   // 使用shmat获取到的值
};

#endif /* MQPOOL_INCLUDE_SHM_ALLOCATOR_H_ */
