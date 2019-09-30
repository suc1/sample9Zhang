#include <climits>
#include <thread>
#include <vector>

/*
 *Same file:
 * libwindowmanager/unittest/inc/thread_helper.h
 * windowmanager/unittest/inc/thread_helper.h
 */

namespace windowmanager {
/*!
 * @class ThreadSynchronize
 *
 * @brief The current thread is waiting something happen, which is set by a callback in another thread
 */
class ThreadSynchronize {
public:
    ThreadSynchronize() = default;
    virtual ~ThreadSynchronize() = default;

    /*!
     * @brief The last step in callback function is to wake up waiting thread
     */
    void wakeupWaitingThread(std::unique_lock<std::mutex> &lock, bool notifyAll=false) {
        m_wakeUp = true;
        notifyAll ? m_cv.notify_all() : m_cv.notify_one();
    }

    void wakeupWaitingThread(bool notifyAll=false) {
        std::unique_lock<std::mutex> lock(m_mtx);
        wakeupWaitingThread(lock, notifyAll);
    }

    /*!
     * @brief The current thread is waiting something happen
     * @param[in] waitSeconds is the timeout. waitSeconds <=0 means infinite
     * @return false means timeout
     */
    bool waitSomethingHappen(std::unique_lock<std::mutex> &lock, int waitSeconds) {
        if(waitSeconds <= 0) {
            waitSeconds = INT_MAX; //32767 second = 9.1 hour
        }

        //a CV can exit from wait() without anyone having called notify_*() first
        m_cv.wait_for(lock, std::chrono::seconds(waitSeconds), [this] { return m_wakeUp; });
        return m_wakeUp;
    }

    bool waitSomethingHappen(int waitSeconds) {
        std::unique_lock<std::mutex> lock(m_mtx);
        return waitSomethingHappen(lock, waitSeconds);
    }

    /*!
     * @brief The waiting thread check it is wakeup or timeout
     */
    bool isWakeUp() const {return m_wakeUp;}

    /*!
     * @brief reuse this class
     */
    void reset() {m_wakeUp = false;}

public:
    std::mutex m_mtx;

protected:
    std::condition_variable m_cv;
    bool m_wakeUp = false; //avoid spurious wake-up
};

}
#endif
