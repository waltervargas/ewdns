project(
  'dnsproxy',
  'c',
  default_options: ['warning_level=3']
)

# Declare how to build the main project
dnsproxy_sources = [
  'src/dnsproxy.c',
  'src/utils.c'
]
dnsproxy_exec = executable('dnsproxy', dnsproxy_sources)

# DNS Echo
dnsecho_sources = [
  'src/udp.c',
  'src/dnsecho.c',
  'src/epoll.c'
]
dnsecho_exec = executable('dnsecho', dnsecho_sources)

# Declare project dependencies here, for example:
check_dep = dependency('check')

# Include tests if check_dep dependency is found
if check_dep.found()
  utils_test_src = [
    'tests/utils_test.c',
    'src/utils.c'
  ]
  utils_test_exec = executable(
    'utils_test',
    utils_test_src,
    dependencies: [check_dep]
  )

  udp_test_src = [
    'tests/udp_test.c',
    'src/udp.c'
  ]
  udp_test_exec = executable(
    'udp_test',
    udp_test_src,
    dependencies: [check_dep]
  )

  test('utils_test', utils_test_exec)
  test('udp_test', udp_test_exec)
endif
