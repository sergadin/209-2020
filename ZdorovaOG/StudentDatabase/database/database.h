/**
 * @file
 * @brief      Заголовочный файл с описанием основных классов базы данных
 *
 * Данный файл содержит в себе определение класса DataHolder,
 * служащего для базовых операций ввода-вывода, и класса
 * DataBase, служащего для обработки основных запросов.
 */
#pragma once

#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "../config/config.h"
#include "index.h"
#include "student.h"

/**
 * @brief      Базовый класс для хранения записей
 *             Имеет функциональность добавления, удаления,
 *             загрузки и сохранения записей.
 */
class DataHolder {
 public:
  DataHolder() = default;
  /**
   * @brief      Добавляет запись о студенте в базу
   *
   * Добавляет запись о студенте в базу хранящуюся в оперативной памяти.
   * Не изменяет файл базы на диске. Для сохранения изменений, должна быть
   * вызвана функция Save(const std::string &)
   *
   * @param[in]  s     Структура Student с информацией по студенту
   */
  void Insert(const Student &s);

  /**
   * @brief      Удаляет запись о студенте из базы
   *
   * Удаляет запись о студенте из базы хранящейся в оперативной памяти.
   * Не изменяет файл базы на диске. Для сохранения изменений, должна быть
   * вызвана функция Save(const std::string &)
   *
   * @param[in]  id    ИД записи студента
   *
   * @return     True если студент был удален, False иначе.
   */
  bool Remove(size_t id);

  /**
   * @brief      Загружает данные в базу из файла
   *
   * @param[in]  file_name  Имя файла
   *
   * @return     True если загрузка была успешной, False иначе.
   */
  bool Load(const std::string &file_name);

  /**
   * @brief      Загружает данные из потока ввода
   *
   * @param      is    Поток ввода
   *
   * @return     True если загрузка была успешной, False иначе.
   */
  bool Load(std::istream &is);

  /**
   * @brief      Сохраняет данные в файл
   *
   * @param[in]  file_name  Имя файла
   *
   * @return     True если сохранение было успешным, False иначе.
   */
  bool Save(const std::string &file_name);

  /**
   * @brief      Выводит данные в поток вывода
   *
   * @param      os    Поток вывода
   */
  void Save(std::ostream &os);

 protected:
  using base = DataHolder;
  Index _index;  ///< Базовый индекс базы данных
  std::vector<Student> _data;  ///< Список записей с информацией по студентам
  std::mutex _mutex;
};

/**
 * @brief      Надстройка над классом DataHolder.
 *             Дополнительно умеет делать выборку по запросу.
 */
class DataBase : public DataHolder {
 public:
  /**
   * @brief      Производит выборку по запросу по всей базе
   *
   * @param      is    Поток ввода
   * @param[in]  pid   Идентификатор пользователя
   */
  void Select(std::istream &is, size_t pid);

  /**
   * @brief      Производит выбрку по запросу по предудущей выборке
   *
   * @param      is    Поток ввода
   * @param[in]  pid   Идентификатор пользователя
   */
  void Reselect(std::istream &is, size_t pid);

  /**
   * @brief      Выводит результаты последнего запроса в поток
   *
   * Извлекает из потока ввода список полей для вывода и поле
   * сортировки, если оно присутствует, и выводит в поток вывода
   * информацию по запрошенным полям отсортированную по указанному полю
   * (по умолчанию по ИД записи)
   *
   * @param      is    Поток ввода
   * @param      os    Поток вывода
   * @param[in]  pid   Идентификатор пользователя
   */
  void Print(std::istream &is, std::ostream &os, size_t pid);

  /**
   * @brief      Удаляет записи из последнего запроса
   *
   * Вызывает метод Remove() родительского класса DataHolder
   * для каждого ИД из последнего запроса и очищает индекс
   * текущего пользователя
   *
   * @param[in]  pid   Идентификатор пользователя
   */
  void Delete(size_t pid);

  /**
   * @brief      Удаляет записи из переданного списка
   *
   * Вызывает метод Remove() родительского класса DataHolder
   * для каждого ИД из переданного списка и удаляет их из
   * индекса текущего пользователя
   *
   * @param[in]  pid   Идентификатор пользователя
   */
  void Delete(size_t pid, const std::vector<size_t> &ids);

  /**
   * @brief      Добавляет запись о студенте
   *
   * Считывает с потока ввода данные и формирует из них структуру Student,
   * которую передает в метод Insert() родительского класса DataHolder
   *
   * @param      is    Поток ввода
   */
  void Add(std::istream &is);

  /**
   * @brief      Список состояний после функции Process
   */
  enum Status {
    Shutdown = -1,  ///< Указывает, что необходимо завершить программу
    Ok = 0,  ///< Указывает, что можно продолжать работать
    Close = 1  ///< Указывает, что необходимо закрыть сессию
  };
  
  /**
   * @brief      Обрабатывает запрос
   *
   * Базовый метод обработки запросов. Определяет по
   * потоку ввода тип запроса и передает поток ввода в
   * соответствующий метод для дальнейшей обработки
   *
   * @param      is    Поток ввода
   * @param      os    Поток вывода
   * @param[in]  pid   Идентификатор пользователя
   *
   * @return     Статус указывающий на необходимое действие
   */
  Status Process(std::istream &is, std::ostream &os, size_t pid);

  /**
   * @brief      Задает разделитель для вывода
   *
   * @param      delim    Символ разделителя
   */
  void SetDelim(char delim);

  /**
   * @brief      Регистрирует нвоого пользователя
   *
   * @return     Идентификатор пользователя
   */
  int RegisterUser();

  /**
   * @brief      Удаляет сессию пользователя
   *
   * @param[in]  pid   Идентификатор пользователя
   *
   * @return     True если сессия с данным ИД существовала, False иначе.
   */
  bool EraseUser(size_t pid);

 private:
  using Columns = std::vector<std::string>;
  using IdSet = std::set<size_t>;
  using NameMap = std::map<std::string, IdSet>;
  using GroupMap = std::map<int, IdSet>;
  using RatingMap = std::map<double, IdSet>;
  /**
   * @brief      Создает индекс по списку ИД
   *
   * @param[in]  ids   ИД студентов
   */
  Index ConstructIndex(const std::set<size_t> &ids);

  /**
   * @brief      Выводит в поток значения указанных полей,
   *             упорядоченные по ИД
   *
   * @param      os        Поток вывода
   * @param[in]  what_col  Список полей
   * @param[in]  ids       Список ИД
   */
  void Print(std::ostream &os, Columns what_col, const IdSet &ids);

  /**
   * @brief      Выводит в поток значения указанных полей,
   *             упорядоченные по именам
   *
   * @param      os        Поток вывода
   * @param[in]  what_col  Список полей
   * @param[in]  names     Список имён
   */
  void Print(std::ostream &os, Columns what_col, const NameMap &names);

  /**
   * @brief      Выводит в поток значения указанных полей,
   *             упорядоченные по группам
   *
   * @param      os        Поток вывода
   * @param[in]  what_col  Список полей
   * @param[in]  groups    Список групп
   */
  void Print(std::ostream &os, Columns what_col, const GroupMap &groups);

  /**
   * @brief      Выводит в поток значения указанных полей,
   *             упорядоченные по рейтингу
   *
   * @param      os        Поток вывода
   * @param[in]  what_col  Список полей
   * @param[in]  ratings   Список рейтингов
   */
  void Print(std::ostream &os, Columns what_col, const RatingMap &ratings);

  std::map<int, Index> _indexes;
  char _delim = ' ';
};