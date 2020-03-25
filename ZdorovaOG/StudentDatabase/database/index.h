#pragma once

#include "student.h"

#include <map>
#include <set>
#include <string>
#include <vector>

/**
 * @brief      Индекс базы данных. Содержит поля сортиованные по
 *             имени, группе, рейтингу.
 */
class Index {
public:
  Index() = default;
  Index(const Index &other);
  Index &operator=(const Index &other);
  Index &operator=(Index &&other);

  /**
   * @brief      Добавляет запись студента в индекс
   *
   * @param[in]  id    ИД записи
   * @param[in]  s     Студент
   *
   * @return     True если запись была добавлена, False иначе.
   */
  bool Insert(size_t id, const Student &s);

  /**
   * @brief      Удаляет запись о студенте из индекса
   *
   * @param[in]  id    ИД записи
   * @param[in]  s     Студент
   *
   * @return     True если запись была удалена, False иначе.
   */
  bool Remove(size_t id, const Student &s);

  /**
   * @brief      Очищает индекс
   */
  void Clear();

  /**
   * @brief      Определяет имеется ли запись с данным ИД в индексе
   *
   * @param[in]  id    ИД записи
   *
   * @return     True если запись имеется, False иначе.
   */
  bool HasId(size_t id) const;

  // Get set of ids by condition
  std::set<size_t> FindByConditionL(const std::string &name) const;
  std::set<size_t> FindByConditionL(int group) const;
  std::set<size_t> FindByConditionL(double rating) const;
  std::set<size_t> FindByConditionLE(const std::string &name) const;
  std::set<size_t> FindByConditionLE(int group) const;
  std::set<size_t> FindByConditionLE(double rating) const;
  std::set<size_t> FindByConditionG(const std::string &name) const;
  std::set<size_t> FindByConditionG(int group) const;
  std::set<size_t> FindByConditionG(double rating) const;
  std::set<size_t> FindByConditionGE(const std::string &name) const;
  std::set<size_t> FindByConditionGE(int group) const;
  std::set<size_t> FindByConditionGE(double rating) const;
  std::set<size_t> FindByConditionE(const std::string &name) const;
  std::set<size_t> FindByConditionE(int group) const;
  std::set<size_t> FindByConditionE(double rating) const;
  std::set<size_t> FindByConditionNE(const std::string &name) const;
  std::set<size_t> FindByConditionNE(int group) const;
  std::set<size_t> FindByConditionNE(double rating) const;
  // Get set of ids, with names in given range
  std::set<size_t> FindByConditionIn(const std::string &from,
                                     const std::string &to) const;
  std::set<size_t> FindByConditionNIn(const std::string &from,
                                      const std::string &to) const;
  const std::set<size_t> &GetIds() const;
  const std::map<std::string, std::set<size_t>> &GetNameMap() const;
  const std::map<int, std::set<size_t>> &GetGroupMap() const;
  const std::map<double, std::set<size_t>> &GetRatingMap() const;

private:
  std::set<size_t> _ids;
  std::map<std::string, std::set<size_t>> _id_by_name;
  std::map<int, std::set<size_t>> _id_by_group;
  std::map<double, std::set<size_t>> _id_by_rating;
};