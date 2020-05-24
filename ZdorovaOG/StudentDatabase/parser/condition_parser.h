/**
 * @file
 * @brief      Заголовочный файл с описанием функций парсинга входных данных
 *
 * Данный файл содержит в себе определения функций, используемых
 * при построении дерева запросов, а так же списка запросов
 * для печати.
 */
#pragma once

#include <iostream>
#include <memory>

#include "node.h"


/**
 * @brief      Строит дерево запросов
 *
 * @param      is    Поток ввода
 *
 * @return     Указатель на вершину дерева
 */
std::shared_ptr<Node> ParseExpression(std::istream &is);

/**
 * @brief      Строит список полей для вывода
 *
 * @param      is    Поток ввода
 *
 * @return     Список полей для вывода
 */
std::pair<std::vector<std::string>, std::string> ParseFieldList(
    std::istream &is);

std::string ParseString(std::istream &is);
int ParseInteger(std::istream &is);
double ParseFloat(std::istream &is);