target_sources(${PROJECT_NAME} PUBLIC
        ${EVM_INCLUDE_DIR}/capturing_pipeline.hpp
        ${EVM_INCLUDE_DIR}/capture.hpp
        ${EVM_INCLUDE_DIR}/roi_capture.hpp
        ${EVM_INCLUDE_DIR}/processor.hpp
        ${EVM_INCLUDE_DIR}/display.hpp
        ${EVM_INCLUDE_DIR}/pyramid.hpp
        ${EVM_INCLUDE_DIR}/evm_pipeline.hpp
        ${EVM_INCLUDE_DIR}/roi_reconstructor.hpp
        ${EVM_INCLUDE_DIR}/roi_filter.hpp
        ${EVM_INCLUDE_DIR}/fps_calculator.hpp

        PRIVATE
        capturing_pipeline.cpp
        display.cpp
        processor.cpp
        pyramid.cpp
        evm_pipeline.cpp
        fps_calculator.cpp
        )
add_subdirectory(impl)
add_subdirectory(filter)