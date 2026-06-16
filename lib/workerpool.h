#ifndef WORKERPOOL_H
#define WORKERPOOL_H

#include <QObject>
#include <QThreadPool>
#include <QMetaObject>
#include <QRunnable>
//#include <functional>
#include <type_traits>

class WorkerPool : public QObject {
  Q_OBJECT

public:
  explicit WorkerPool(QObject* parent = nullptr);
  static WorkerPool& instance();
  void setMaxThreadCount(int count);

  template<typename Task>
  void run(Task&& task);

  template<typename Task, typename Callback>
  void run(Task&& task, Callback&& callback);

private:
  QThreadPool m_pool;
};
#endif // WORKERPOOL_H
