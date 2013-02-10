ifeq ($(DEBUG)$(CLANG),nn)

# These sources contain a lot of "hot" code, and should be optimized
# for speed, not for size.
HOT_SOURCES = \
	$(MATH_SOURCES) \
	$(GEO_SOURCES) \
	$(ZLIB_SOURCES) \
	$(JASPER) \
	$(SRC)/Topography/TopographyFileRenderer.cpp \
	$(SRC)/Terrain/RasterBuffer.cpp \
	$(SRC)/Terrain/RasterMap.cpp \
	$(SRC)/Terrain/HeightMatrix.cpp \
	$(SRC)/Terrain/RasterRenderer.cpp \
	$(SRC)/Terrain/RasterTile.cpp \
	$(SRC)/Terrain/ScanLine.cpp \
	$(SRC)/Terrain/Intersection.cpp \
	$(SRC)/Projection/Projection.cpp \
	$(ENGINE_SRC_DIR)/Task/Shapes/FAITriangleSector.cpp \
	$(ENGINE_SRC_DIR)/GlideSolvers/MacCready.cpp \
	$(ENGINE_SRC_DIR)/GlideSolvers/GlidePolar.cpp \
	$(ENGINE_SRC_DIR)/Navigation/INU/Inu.cpp \
	$(ENGINE_SRC_DIR)/Navigation/INU/INUKalman.cpp \
	$(ENGINE_SRC_DIR)/Route/FlatTriangleFan.cpp \
	$(ENGINE_SRC_DIR)/Route/FlatTriangleFanTree.cpp \
	$(ENGINE_SRC_DIR)/Route/ReachFan.cpp \
	$(ENGINE_SRC_DIR)/Route/RoutePolar.cpp \
	$(ENGINE_SRC_DIR)/Route/RouteLink.cpp \
	$(ENGINE_SRC_DIR)/Route/RoutePolars.cpp \
	$(ENGINE_SRC_DIR)/Contest/Solvers/ContestDijkstra.cpp

$(call SRC_TO_OBJ,$(HOT_SOURCES)): OPTIMIZE += -O3

endif
