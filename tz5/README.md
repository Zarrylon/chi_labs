# ТЗ 5
Написати шаблон класу Matrix (2 виміри) з одним шаблонним параметром внутрішнього типу. Аргумент за промовчуванням повинен бути int. Мартиця повинна мати наступний базовий функціонал:
* resize для параметрів вимірів;
* random access read/write (operator[]);            
* сортування рядків, стовпців. Включає можливість використовувати довільні компаратори для сортування;
* оператори +- з іншими мартицями;
* оператор виводу в ostream.

Матриця повинна внутрішньо використовувати аліаси для спрощення коду. Також додати спеціалізацію для стрінга, чисельний показник - довжина стрічки. Якщо матриця константна, повинна зберігати свій ранг через integral_constant. При виводі елементів розділяти їх через ';' для чисельних (integral) типів та '|' для інших. В main() показати імплементований функціонал.
Також в коді треба продемонструвати всі 3 типи спеціалізації шаблонів для різних типів: implicit/explicit instantiation, explicit specialization.

Бонусне завдання: креативно використати SFINAE, вказати де в коментарі!