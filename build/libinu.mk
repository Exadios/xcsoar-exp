INU_SRC_DIR = $(SRC)/Engine/Navigation/INU

INU_SOURCES = \
	$(INU_SRC_DIR)/INUKalman.cpp \
	$(INU_SRC_DIR)/Inu.cpp

$(eval $(call link-library,libinu,INU))
