#pragma once

#include "node.h"

#include <iostream>
#include <memory>

/**
 * @brief      Строит дерево запросов
 *
 * @param      is    Поток ввода
 *
 * @return     Указатель на вершину дерева
 */
std::shared_ptr<Node> ParseSelectCondition(std::istream &is);

/**
 * @brief      Строит список полей для вывода
 *
 * @param      is    Поток ввода
 *
 * @return     Список полей для вывода
 */
std::vector<std::string> ParsePrintCondition(std::istream &is);