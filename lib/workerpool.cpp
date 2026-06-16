#include "workerpool.h"

WorkerPool::WorkerPool(QObject *parent) : QObject(parent) {
  m_pool.setMaxThreadCount(QThread::idealThreadCount());
}

WorkerPool &WorkerPool::instance() {
  static WorkerPool pool;
  return pool;
}

void WorkerPool::setMaxThreadCount(int count) {
  m_pool.setMaxThreadCount(count);
}

template<typename Task>
void WorkerPool::run(Task &&task) {
  class Runnable : public QRunnable {
  public:
    explicit Runnable(Task&& fn) : m_fn(std::forward<Task>(fn)) {
      setAutoDelete(true);
    }
    void run() override {
      m_fn();
    }

  private:
    Task m_fn;
  };

  m_pool.start(new Runnable(std::forward<Task>(task)));
}

template<typename Task, typename Callback>
void WorkerPool::run(Task &&task, Callback &&callback) {
  using ResultType = std::invoke_result_t<std::decay_t<Task>>;

  class Runnable : public QRunnable {
  public:
    Runnable(Task&& task, Callback&& callback, QObject* receiver)
      : m_task(std::forward<Task>(task))
      , m_callback(std::forward<Callback>(callback))
      , m_receiver(receiver) {
      setAutoDelete(true);
    }

    void run() override {
      if constexpr(std::is_void_v<ResultType>) {
        m_task();

        QMetaObject::invokeMethod(m_receiver, [cb = m_callback]() {
            cb();
          }, Qt::QueuedConnection);
      }
      else {
        auto result = m_task();

        QMetaObject::invokeMethod( m_receiver, [cb = m_callback, result]() {
            cb(result);
          }, Qt::QueuedConnection);
      }
    }

  private:
    Task m_task;
    Callback m_callback;
    QObject* m_receiver;
  };

  m_pool.start(new Runnable(std::forward<Task>(task), std::forward<Callback>(callback), this));
}
