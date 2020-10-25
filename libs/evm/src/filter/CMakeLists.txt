target_sources(${PROJECT_NAME} PUBLIC
        ${EVM_INCLUDE_DIR}/filter/spatial_filter.hpp
        ${EVM_INCLUDE_DIR}/filter/temporal_filter.hpp
        ${EVM_INCLUDE_DIR}/filter/laplace_spatial_filter.hpp
        ${EVM_INCLUDE_DIR}/filter/ideal_bandpass_temporal_filter.hpp
        ${EVM_INCLUDE_DIR}/filter/amplifier.hpp
        ${EVM_INCLUDE_DIR}/filter/reconstructor.hpp
        ${EVM_INCLUDE_DIR}/filter/level_reconstructor.hpp

        PRIVATE
        laplace_pyramid.hpp
        laplace_pyramid.cpp
        laplace_spatial_filter.cpp
        ideal_bandpass_temporal_filter.cpp
        amplifier.cpp
        reconstructor.cpp
        level_reconstructor.cpp
        )