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
