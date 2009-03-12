PHP_ARG_ENABLE(baxtep, whether to enable baxtep support,
[  --enable-bahtep           Enable bahtep support])

PHP_NEW_EXTENSION(baxtep, baxtep.c baxtep_core.c baxtep_logger.c, $ext_shared)
