Compiling/running unit tests
------------------------------------

Unit tests will be automatically compiled if dependencies were met in configure
and tests weren't explicitly disabled.

After configuring, they can be run with 'make check'.

To run the lobstexd tests manually, launch src/test/test_lobstex .

To add more lobstexd tests, add `BOOST_AUTO_TEST_CASE` functions to the existing
.cpp files in the test/ directory or add new .cpp files that
implement new BOOST_AUTO_TEST_SUITE sections.

To run the lobstex-qt tests manually, launch src/qt/test/lobstex-qt_test

To add more lobstex-qt tests, add them to the `src/qt/test/` directory and
the `src/qt/test/test_main.cpp` file.
