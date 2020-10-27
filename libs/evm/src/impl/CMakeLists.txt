target_sources(${PROJECT_NAME} PUBLIC
        ${EVM_INCLUDE_DIR}/impl/webcam_capture.hpp
        ${EVM_INCLUDE_DIR}/impl/video_capture.hpp
        ${EVM_INCLUDE_DIR}/impl/roi_default.hpp
        ${EVM_INCLUDE_DIR}/impl/roi_default_reconstructor.hpp
        ${EVM_INCLUDE_DIR}/impl/roi_face_capture.hpp
        ${EVM_INCLUDE_DIR}/impl/roi_fix_capture.hpp
        ${EVM_INCLUDE_DIR}/impl/roi_position_reconstructor.hpp

        PRIVATE
        webcam_capture.cpp
        video_capture.cpp
        roi_default.cpp
        roi_default_reconstructor.cpp
        roi_face_capture.cpp
        roi_position_reconstructor.cpp
        roi_fix_capture.cpp
        )