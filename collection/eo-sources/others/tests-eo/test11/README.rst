Тестирование объекта while
============================

Пример из документации гласит, что while возможен::

    []
      memory > i
      while. > @
        i.less 100
        seq
          if.
            eq. (i.mod 2 0)
              stdout "even!"
          i.write (i.add 1)

А работает ли цикл?
Запустить не сумел.

Это тестовый код::

# Попытка выполнить цикл
[] > app
  memory > i
  seq > @
    i.write 1
    while.
      less.
        i
        10
      seq
        stdout
          sprintf
            "i = %d"
            i
        i.write (i.add 1)
