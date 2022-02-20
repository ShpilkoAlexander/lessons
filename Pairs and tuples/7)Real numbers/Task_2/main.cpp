#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
      NEW,          // новая
      IN_PROGRESS,  // в разработке
      TESTING,      // на тестировании
      DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
      // Получить статистику по статусам задач конкретного разработчика
      const TasksInfo& GetPersonTasksInfo(const string& person) const {

          return persons_tasks.at(person);
      }

      // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
      void AddNewTask(const string& person){
          ++persons_tasks[person][TaskStatus::NEW];
      }

      // Обновить статусы по данному количеству задач конкретного разработчика,
      // подробности см. ниже
      tuple<TasksInfo, TasksInfo> PerformPersonTasks(
              const string& person, int change_task_count){


          persons_tasks[person][TaskStatus::NEW];
          persons_tasks[person][TaskStatus::IN_PROGRESS];
          persons_tasks[person][TaskStatus::TESTING];
          persons_tasks[person][TaskStatus::DONE];
          TasksInfo updates_status;
          TasksInfo updated_tasks;
          TasksInfo untouched_tasks;
          if(persons_tasks.count(person) == 0){
              return tie(updated_tasks, untouched_tasks);
          }

          int new_task = 0;
          int changed_task = 0;
          for(const auto& [status, task_count] : persons_tasks[person]){
              changed_task = change_task_count;
              if(new_task != 0){
                  updated_tasks[status] = new_task;
              }

              if(status != TaskStatus::DONE){
                  updates_status[status] = new_task + UpdateTask(change_task_count, task_count);
              }
              else{
                  updates_status[status] = task_count + new_task;
              }
              if(updates_status[status]  > new_task && status != TaskStatus::DONE){
                untouched_tasks[status] = updates_status[status] - new_task;
              }
              new_task = changed_task >= task_count ? task_count : changed_task;



          }


          persons_tasks[person] = updates_status;
          return tie(updated_tasks, untouched_tasks);

      }

private:
      map<string, TasksInfo> persons_tasks;


      int UpdateTask (int& change_task_count, int task_count){
          if (change_task_count > task_count){
              change_task_count -= task_count;
              return 0;
          }
          else{
              int update_task_count = task_count - change_task_count;
              change_task_count = 0;
              return update_task_count;
          }
      }


};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь


void PrintTasksInfo(const TasksInfo& tasks_info) {
    if (tasks_info.count(TaskStatus::NEW)) {
        std::cout << "NEW: " << tasks_info.at(TaskStatus::NEW) << " ";
    }
    if (tasks_info.count(TaskStatus::IN_PROGRESS)) {
        std::cout << "IN_PROGRESS: " << tasks_info.at(TaskStatus::IN_PROGRESS) << " ";
    }
    if (tasks_info.count(TaskStatus::TESTING)) {
        std::cout << "TESTING: " << tasks_info.at(TaskStatus::TESTING) << " ";
    }
    if (tasks_info.count(TaskStatus::DONE)) {
        std::cout << "DONE: " << tasks_info.at(TaskStatus::DONE) << " ";
    }
}

void PrintTasksInfo(const TasksInfo& updated_tasks, const TasksInfo& untouched_tasks) {
    std::cout << "Updated: [";
    PrintTasksInfo(updated_tasks);
    std::cout << "] ";

    std::cout << "Untouched: [";
    PrintTasksInfo(untouched_tasks);
    std::cout << "] ";

    std::cout << std::endl;
}

int main() {
    TeamTasks tasks;
    TasksInfo updated_tasks;
    TasksInfo untouched_tasks;


    /* TEST 3 */
    std::cout << "\nLisa" << std::endl;

    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa");
    }

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 1);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]

    for (auto i = 0; i < 5; ++i) {
        tasks.AddNewTask("Lisa");
    }

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 4);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 4, "DONE": 1}, {"TESTING": 4}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"TESTING": 8, "DONE": 2}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 8}, {}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
    std::cout << std::endl;

    tasks.AddNewTask("Lisa");

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 1, "DONE": 10}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]

    PrintTasksInfo(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 1, "DONE": 10}
    std::cout << std::endl;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Bob", 3);
    PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]

    return 0;
}
