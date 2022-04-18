from utests import unittest
from system_vars import *
import fun


class StaticIntegerTestCase(unittest.TestCase):
    # Long Long Tests
    def test_long_long_min(self):
        fun.showname()
        c_type = integer[0]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_long_max(self):
        fun.showname()
        c_type = integer[0]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_long_min_overflow(self):
        fun.showname()
        c_type = integer[0]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_long_max_overflow(self):
        fun.showname()
        c_type = integer[0]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_long_1(self):
        fun.showname()
        c_type = integer[0]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_long_2(self):
        fun.showname()
        c_type = integer[0]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_long_3(self):
        fun.showname()
        c_type = integer[0]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Long Tests
    def test_long_min(self):
        fun.showname()
        c_type = integer[1]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_max(self):
        fun.showname()
        c_type = integer[1]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_min_overflow(self):
        fun.showname()
        c_type = integer[1]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_max_overflow(self):
        fun.showname()
        c_type = integer[1]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_1(self):
        fun.showname()
        c_type = integer[1]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_2(self):
        fun.showname()
        c_type = integer[1]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_3(self):
        fun.showname()
        c_type = integer[1]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Unsigned Long Tests
    def test_unsigned_long_min(self):
        fun.showname()
        c_type = integer[2]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_long_max(self):
        fun.showname()
        c_type = integer[2]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_long_min_overflow(self):
        fun.showname()
        c_type = integer[2]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_long_max_overflow(self):
        fun.showname()
        c_type = integer[2]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_long_1(self):
        fun.showname()
        c_type = integer[2]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_long_2(self):
        fun.showname()
        c_type = integer[2]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_long_3(self):
        fun.showname()
        c_type = integer[2]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Signed Tests
    def test_signed_min(self):
        fun.showname()
        c_type = integer[3]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_signed_max(self):
        fun.showname()
        c_type = integer[3]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_signed_min_overflow(self):
        fun.showname()
        c_type = integer[3]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_signed_max_overflow(self):
        fun.showname()
        c_type = integer[3]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_signed_1(self):
        fun.showname()
        c_type = integer[3]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_signed_2(self):
        fun.showname()
        c_type = integer[3]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_signed_3(self):
        fun.showname()
        c_type = integer[3]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Unsigned Tests
    def test_unsigned_min(self):
        fun.showname()
        c_type = integer[4]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_max(self):
        fun.showname()
        c_type = integer[4]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_min_overflow(self):
        fun.showname()
        c_type = integer[4]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_max_overflow(self):
        fun.showname()
        c_type = integer[4]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_1(self):
        fun.showname()
        c_type = integer[4]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_2(self):
        fun.showname()
        c_type = integer[4]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_3(self):
        fun.showname()
        c_type = integer[4]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Short Tests
    def test_short_min(self):
        fun.showname()
        c_type = integer[5]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_short_max(self):
        fun.showname()
        c_type = integer[5]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_short_min_overflow(self):
        fun.showname()
        c_type = integer[5]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_short_max_overflow(self):
        fun.showname()
        c_type = integer[5]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_short_1(self):
        fun.showname()
        c_type = integer[5]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_short_2(self):
        fun.showname()
        c_type = integer[5]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_short_3(self):
        fun.showname()
        c_type = integer[5]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Unsigned Short Tests
    def test_unsigned_short_min(self):
        fun.showname()
        c_type = integer[6]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_short_max(self):
        fun.showname()
        c_type = integer[6]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_short_min_overflow(self):
        fun.showname()
        c_type = integer[6]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_short_max_overflow(self):
        fun.showname()
        c_type = integer[6]
        value = c_type[-2] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_short_1(self):
        fun.showname()
        c_type = integer[6]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_short_2(self):
        fun.showname()
        c_type = integer[6]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_short_3(self):
        fun.showname()
        c_type = integer[6]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)


class StaticSpecificTestCase(unittest.TestCase):
    # Char Tests
    def test_char_min(self):
        fun.showname()
        c_type = integer[7]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_char_max(self):
        fun.showname()
        c_type = integer[7]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_char_min_overflow(self):
        fun.showname()
        c_type = integer[7]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_char_max_overflow(self):
        fun.showname()
        c_type = integer[7]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_char_1(self):
        fun.showname()
        c_type = integer[7]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_char_2(self):
        fun.showname()
        c_type = integer[7]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_char_3(self):
        fun.showname()
        c_type = integer[7]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Unsigned Char Tests
    def test_unsigned_char_min(self):
        fun.showname()
        c_type = integer[8]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_char_max(self):
        fun.showname()
        c_type = integer[8]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_char_min_overflow(self):
        fun.showname()
        c_type = integer[8]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_char_max_overflow(self):
        fun.showname()
        c_type = integer[8]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_char_1(self):
        fun.showname()
        c_type = integer[8]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_char_2(self):
        fun.showname()
        c_type = integer[8]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_unsigned_char_3(self):
        fun.showname()
        c_type = integer[8]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Bool Tests
    def test_bool_min(self):
        fun.showname()
        c_type = integer[9]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_bool_max(self):
        fun.showname()
        c_type = integer[9]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_bool_min_overflow(self):
        fun.showname()
        c_type = integer[9]
        value = c_type[-1] - 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_bool_max_overflow(self):
        fun.showname()
        c_type = integer[9]
        value = c_type[-2] + 18
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_bool_1(self):
        fun.showname()
        c_type = integer[9]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 1
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_bool_2(self):
        fun.showname()
        c_type = integer[9]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_bool_3(self):
        fun.showname()
        c_type = integer[9]
        value = c_type[-1] + (c_type[-2] + 1 - c_type[-1]) // 4 * 3
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)


class StaticFloatTestCase(unittest.TestCase):
    # Float Tests
    def test_float_min(self):
        fun.showname()
        c_type = real[0]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_float_max(self):
        fun.showname()
        c_type = real[0]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_float_min_overflow(self):
        fun.showname()
        c_type = real[0]
        value = f'{c_type[-1]} * 2'
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_float_max_overflow(self):
        fun.showname()
        c_type = real[0]
        value = f'{c_type[-2]} * 2'
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_float_1(self):
        fun.showname()
        c_type = real[0]
        value = c_type[-1] // 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_float_2(self):
        fun.showname()
        c_type = real[0]
        value = 1 / 9
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_float_3(self):
        fun.showname()
        c_type = real[0]
        value = c_type[-2] // 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Double Tests
    def test_double_min(self):
        fun.showname()
        c_type = real[1]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_double_max(self):
        fun.showname()
        c_type = real[1]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_double_min_overflow(self):
        fun.showname()
        c_type = real[1]
        value = f'{c_type[-1]} * 2'
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_double_max_overflow(self):
        fun.showname()
        c_type = real[1]
        value = f'{c_type[-2]} * 2'
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_double_1(self):
        fun.showname()
        c_type = real[1]
        value = c_type[-1] // 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_double_2(self):
        fun.showname()
        c_type = real[1]
        value = 1 / 9
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_double_3(self):
        fun.showname()
        c_type = real[1]
        value = c_type[-2] // 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    # Long Double Tests
    def test_long_double_min(self):
        fun.showname()
        c_type = real[2]
        value = c_type[-1]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_double_max(self):
        fun.showname()
        c_type = real[2]
        value = c_type[-2]
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_double_min_overflow(self):
        fun.showname()
        c_type = real[2]
        value = f'{c_type[-1]} * 2'
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_double_max_overflow(self):
        fun.showname()
        c_type = real[2]
        value = f'{c_type[-2]} * 2'
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_double_1(self):
        fun.showname()
        c_type = real[2]
        value = c_type[-1] // 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_double_2(self):
        fun.showname()
        c_type = real[2]
        value = 1 / 9
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    def test_long_double_3(self):
        fun.showname()
        c_type = real[2]
        value = c_type[-2] // 2
        declaration, names = fun.generate_vars([c_type], [value], [True])
        fun.generate1([c_type], declaration, names)
        fun.generate2([c_type], declaration, names)
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)
