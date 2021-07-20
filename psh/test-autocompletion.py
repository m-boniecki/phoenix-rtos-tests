# Phoenix-RTOS
#
# phoenix-rtos-tests
#
# "auth" psh applet test
#
# Copyright 2021 Phoenix Systems
# Author: Mateusz Niewiadomski
#
# This file is part of Phoenix-RTOS.
#
# %LICENSE%
#
from psh.tools import run_psh, assert_only_prompt
import pexpect


def fallback_and_enter(p, backspaces):
    for i in range(0, backspaces):
        p.send('\x08')
    p.send('\n')


def harness(p):
    run_psh(p)
    assert_only_prompt(p)

    # Test autocompletion for directory
    p.send('de' + '\t')
    assert p.expect_exact(['dev/', pexpect.TIMEOUT]) == 0, 'Cannot autocomplete directory'
    fallback_and_enter(p, 10)
    p.expect_exact('(psh)%')

    # Test for autocompletion for nested directories
    p.send('mkdir dev/loremipsum')
    # Do not check  nested directories if there is no mkdir available
    p.expect_exact('mkdir dev/loremipsum')
    err = p.expect_exact(['(psh)%', 'Unknown command!', pexpect.TIMEOUT], timeout=1)
    if err == 0:
        p.send('dev/lorem' + '\t')
        assert p.expect_exact(['dev/loremipsum', pexpect.TIMEOUT]) == 0, 'Cannot autocomplete filename'

    # Test autocompletion for file
    p.send('touch loremipsum.txt')
    # Do not check filename autocompletion if there is no touch available
    p.expect_exact('touch loremipsum.txt')
    err = p.expect_exact(['(psh)%', 'Unknown command!', pexpect.TIMEOUT], timeout=1)
    if err == 0:
        p.send('lorem' + '\t')
        assert p.expect_exact(['loremipsum.txt', pexpect.TIMEOUT]) == 0, 'Cannot autocomplete filename'
