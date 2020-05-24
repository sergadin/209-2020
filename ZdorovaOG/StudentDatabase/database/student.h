/**
 * @file
 * @brief      Заголовочный файл с описанием структуры Student
 *
 * Данный файл содержит в себе определение структуры Student,
 * которая является базовым объектом базы данных
 */

#pragma once

#include <string>

/**
 * @brief      Структура хранящая информацию о студенте
 */
struct Student {
  std::string name;  ///< ФИО студента
  int group;         ///< Номер группы студента
  double rating;     ///< Рейтинг студента
  std::string info;  ///< Дополнительная информация
  /**
   * @brief      Выводит в поток значение конкретного поля студента
   *
   * @param      os     Поток вывода
   * @param[in]  field  Поле для вывода
   */

  void Print(std::ostream &os, const std::string &field) const;
};