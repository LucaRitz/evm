target_sources(${PROJECT_NAME} PUBLIC
        ${PROCESSING_INCLUDE_DIR}/laplace_pyramid.hpp
        ${PROCESSING_INCLUDE_DIR}/filter.hpp
        ${PROCESSING_INCLUDE_DIR}/queue_worker.hpp

        PRIVATE
        laplace_pyramid.cpp
        filter.cpp
        queue_worker.cpp
        )