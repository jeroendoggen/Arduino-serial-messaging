import unittest


class TestTheSuite(unittest.TestCase):
    """A placeholder for unit tests of this Python program"""

    def setUp(self):
        self.seq = range(10)

    def test_exitValueZero(self):
        self.assertEqual(0, 0)

if __name__ == '__main__':
    unittest.main()
