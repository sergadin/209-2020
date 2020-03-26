#pragma once

#include <string>

struct Student {
  std::string name;
  int group;
  double rating;
  std::string info;
  /**
   * @brief      Выводит в поток значение конкретного поля студента
   *
   * @param      os     Поток вывода
   * @param[in]  field  Поле для вывода
   */
  
  void Print(std::ostream &os, const std::string &field) const;
  /**
   * @brief      Выводит в поток значение всех полей студента
   * 
   *
   * @param      os     Поток вывода
   * @param[in]  delim  Разделитель
   */
  void PrintAll(std::ostream &os, char delim) const;
};