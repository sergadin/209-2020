#### StudentDatabase
База данных студентов. Содержит поля _name_, _group_, _rating_, _info_.

Для компиляции выполнить: _make_

Для запуска выполнить: _./\_main_

Для генерации тестовых данных выполнить: _make input_builder && \_input\_builder [&lt;number-of-samples&gt; &lt;output-file-name&gt;]_

На данный момент данные считываются из файла **\_\_database.txt**

Для получения справки по базе запустить _\_main_ и набрать _help_
#### ToDo
Написать тесты для класса DataBase

Разнести классы по namespace