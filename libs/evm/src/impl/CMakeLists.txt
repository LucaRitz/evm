target_sources(${PROJECT_NAME} PUBLIC
        ${EVM_INCLUDE_DIR}/impl/webcam_capture.hpp
        ${EVM_INCLUDE_DIR}/impl/video_capture.hpp
        ${EVM_INCLUDE_DIR}/impl/roi_default.hpp

        PRIVATE
        webcam_capture.cpp
        video_capture.cpp
        roi_default.cpp
        )