#pragma once
#define DEBUG

#include "index.h"
#include "student.h"

#include <string>
#include <vector>

/**
 * @brief      Базовый класс для хранения записей
 *             Имеет функциональность добавления, удаления,
 *             загрузки и сохранения записей.
 */
class DataHolder {
public:
  DataHolder() = default;
  /**
   * @brief      Добавляет студента в базу
   *
   * @param[in]  s     Студент
   */
  void Insert(const Student &s);

  /**
   * @brief      Удаляет студента из базы
   *
   * @param[in]  id    ИД (номер записи) студента
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
  Index _index;
  std::vector<Student> _data;
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
   */
  void Select(std::istream &is);

  /**
   * @brief      Производит выбрку по запросу по предудущей выборке
   *
   * @param      is    Поток ввода
   */
  void Reselect(std::istream &is);

  /**
   * @brief      Выводит результаты последнего запроса в поток
   *
   * @param      is    Поток ввода
   * @param      os    Поток вывода
   */
  void Print(std::istream &is, std::ostream &os);

  /**
   * @brief      Удаляет записи из последнего запроса
   */
  void Delete();

  /**
   * @brief      Добавляет запись о студенте
   *
   * @param      is    Поток ввода
   */
  void Add(std::istream &is);

  /**
   * @brief      Обрабатывает запрос
   *
   * @param      is    Поток ввода
   * @param      os    Поток вывода
   */
  void Process(std::istream &is, std::ostream &os);

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
  void ConstructIndex(const std::set<size_t> &ids);

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
  Index _index;
};