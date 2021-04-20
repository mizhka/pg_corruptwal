EXTENSION = pg_corruptwal        # the extensions name
DATA = pg_corruptwal--0.0.1.sql  # script files to install
MODULE_big = pg_corruptwal
OBJS = pg_corruptwal.o

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

