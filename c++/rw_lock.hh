#include <condition_variable>
#include <mutex>

using Lock = std::unique_lock<std::mutex>;

/**
 * implement with unique_lock and condition_variable
 * read first
 */
class readwrite_lock_1 {
   public:
    readwrite_lock_1() : stat(0) {}

    void readLock() {
        Lock lock(mutex);
        while (stat < 0) cv.wait(lock);
        ++stat;
    }

    void readUnlock() {
        Lock lock(mutex);
        if (--stat == 0) cv.notify_one();  // 叫醒一个等待的写操作
    }

    void writeLock() {
        Lock lock(mutex);
        while (stat != 0) cv.wait(lock);
        stat = -1;
    }

    void writeUnlock() {
        Lock lock(mutex);
        stat = 0;
        cv.notify_all();  // 叫醒所有等待的读和写操作
    }

   private:
    std::mutex mutex;
    std::condition_variable cv;
    int stat;  // == 0 无锁；> 0 已加读锁个数；< 0 已加写锁
};

/**
 * implement with two mutex
 * read first
 */
class readwrite_lock_2 {
   public:
    readwrite_lock_2() : read_cnt(0) {}

    void readLock() {
        read_mtx.lock();
        if (++read_cnt == 1) write_mtx.lock();

        read_mtx.unlock();
    }

    void readUnlock() {
        read_mtx.lock();
        if (--read_cnt == 0) write_mtx.unlock();

        read_mtx.unlock();
    }

    void writeLock() { write_mtx.lock(); }
    void writeUnlock() { write_mtx.unlock(); }

   private:
    std::mutex read_mtx;
    std::mutex write_mtx;
    int read_cnt;  // 已加读锁个数
};

/**
 * implement with mutex and condition_variable
 * write first
 */
class readwrite_lock_3 {
   private:
    std::mutex mutex;
    std::condition_variable cv;
    int rd_cnt;  //等待读的数量
    int wr_cnt;  //等待写的数量

   public:
    readwrite_lock_3() : rd_cnt(0), wr_cnt(0) {}

    void readLock() {
        Lock lock(mutex);
        ++rd_cnt;
        while (wr_cnt > 0) cv.wait(lock);
    }

    void readUnlock() {
        Lock lock(mutex);
        --rd_cnt;
        if (rd_cnt == 0) cv.notify_all();
    }

    void writeLock() {
        Lock lock(mutex);
        ++wr_cnt;
        while (wr_cnt + rd_cnt >= 2) cv.wait(lock);
    }

    void writerUnlock() {
        Lock lock(mutex);
        --wr_cnt;
        if (wr_cnt == 0) cv.notify_all();
    }
};