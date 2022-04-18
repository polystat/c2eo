from utests import unittest
from system_vars import *
import fun


class BigTests(unittest.TestCase):
    def test_global_integer_vars(self):
        fun.showname()
        c_types = []
        values = []
        for _type in integer:
            c_types += [_type]*7
            values += [_type[-1], _type[-2], _type[-1] - 18, _type[-2] + 18,
                       _type[-1] + (_type[-2] + 1 - _type[-1]) // 4 * 1,
                       _type[-1] + (_type[-2] + 1 - _type[-1]) // 4 * 2,
                       _type[-1] + (_type[-2] + 1 - _type[-1]) // 4 * 3]

        declaration, names = fun.generate_vars(c_types, values)
        fun.generate1(c_types, declaration, names)
        fun.generate2(c_types, declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_static_integer_vars(self):
        fun.showname()
        c_types = []
        values = []
        for _type in integer:
            c_types += [_type]*7
            values += [_type[-1], _type[-2], _type[-1] - 18, _type[-2] + 18,
                       _type[-1] + (_type[-2] + 1 - _type[-1]) // 4 * 1,
                       _type[-1] + (_type[-2] + 1 - _type[-1]) // 4 * 2,
                       _type[-1] + (_type[-2] + 1 - _type[-1]) // 4 * 3]
        static = [True]*len(c_types)
        declaration, names = fun.generate_vars(c_types, values, static)
        fun.generate1(c_types, declaration, names)
        fun.generate2(c_types, declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_global_real_vars(self):
        fun.showname()
        c_types = []
        values = []
        for _type in real:
            c_types += [_type]*7
            values += [_type[-1], _type[-2],  f'{_type[-1]} * 2',  f'{_type[-2]} * 2',
                       _type[-2] // 2, _type[-1] // 2, 1 / 9]

        declaration, names = fun.generate_vars(c_types, values)
        fun.generate1(c_types, declaration, names)
        fun.generate2(c_types, declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_static_real_vars(self):
        fun.showname()
        c_types = []
        values = []
        for _type in real:
            c_types += [_type]*7
            values += [_type[-1], _type[-2],  f'{_type[-1]} * 2',  f'{_type[-2]} * 2',
                       _type[-2] // 2, _type[-1] // 2, 1 / 9]
        static = [True]*len(c_types)
        declaration, names = fun.generate_vars(c_types, values, static)
        fun.generate1(c_types, declaration, names)
        fun.generate2(c_types, declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)
