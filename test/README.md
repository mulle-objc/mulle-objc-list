# Run tests

Use `mulle-sde test` or `mulle-test test` to run the tests. Each test is
specified by an `.args` file which gives command line parameters to the
executable **mulle-objc-list** to be tested.

* rebuild the main project with `mulle-sde test craft`
* run specific test with `mulle-sde test run <testfile>`
* rerun tests with `mulle-sde test rerun`

Extension   | Description
------------|-------------------------
`.args`     | Command arguments
`.stdin`    | Command standard input
`.stdout`   | Expected command standard output

There are quite a few more options to tweak each test. 
See [mulle-test](//github.com/mulle-sde/mulle-test) for more info.
