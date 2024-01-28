#include <stdio.h>
#include <memory>
#include <sys/time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "rkYolov5s.hpp"
#include "rknnPool.hpp"
#include "yolov8/yolov8.h"
#include <optional>

enum ModelVersion {
    YOLO_V5,
    YOLO_V8
};

class RknnWrapper
{
private:
    rkYolov5s yolov5;
    rkYolov8s yolov8;
    int m_numClasses;

public:
    RknnWrapper(const char *model_name, int numClasses, ModelVersion model_ver)
    { 
        switch (model_ver) {
            case YOLO_V5:
                yolov5 = rkYolov5s(model_name, numClasses);
                yolov5.init(yolov5.get_pctx(), false);
            break;
            case YOLO_V8:
                yolov8 = rkYolov8s();
                yolov8.init(model_name);
            break;
        }
    }

    detect_result_group_t forward(cv::Mat &img, DetectionFilterParams params) {
        detect_result_group_t ret;
        int code;
        switch (model_ver) {
            case YOLO_V5:
                code = yolov5.inferOnly(img, &ret, params);
            break;
            case YOLO_V8:
                code = yolov8.infer(img, &ret, params);
            break;
        }
        return ret;
    }

    // Let the rkYolov5s dtor take care of cleanup
    // how to destroy yolov8 processor?????
    ~RknnWrapper() = default;
};
