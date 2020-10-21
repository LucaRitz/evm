target_sources(${PROJECT_NAME} PUBLIC
        ${PROCESSING_INCLUDE_DIR}/laplace_pyramid.hpp
        ${PROCESSING_INCLUDE_DIR}/filter.hpp

        PRIVATE
        laplace_pyramid.cpp
        filter.cpp
        )